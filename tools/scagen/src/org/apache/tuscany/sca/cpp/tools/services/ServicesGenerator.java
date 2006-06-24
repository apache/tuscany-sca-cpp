/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package org.apache.tuscany.sca.cpp.tools.services;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.apache.tuscany.sca.cpp.tools.common.CParsingTool;
import org.apache.tuscany.sca.cpp.tools.common.Headers;
import org.apache.tuscany.sca.cpp.tools.common.Options;
import org.apache.tuscany.sca.cpp.tools.common.Parameter;
import org.apache.tuscany.sca.cpp.tools.common.Signature;
import org.apache.tuscany.sca.cpp.tools.common.Utils;
import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Text;

/**
 * This class is the main class that handles the function that parses a C++
 * interface header file into a DOM that holds all the semantic information
 * about the interface - method names, parameters and return values. It then
 * uses XSLT to generate different "views" of this data plus the parameter map
 * from other sources that are the C++ programs for the proxy and wrapper
 * implementations and headers.
 */
public class ServicesGenerator extends CParsingTool {

    private static final String OPERATION_NAME_ATTR = "operationNameAttr";

    private static final String HEADER_NAME_ATTR = "headerNameAttr";

    private static final String SCA_OPERATION = "scaOperation";

    private static final String SCA_SERVICE = "scaService";

    private static final String SCA_OPERATION_RETURN_TYPE = "scaOperationReturnType";

    private static final String SCA_OPERATION_PARAMETER = "scaOperationParameter";

    private static final String SCA_OPERATION_PARAMETER_NAME_ATTR = "scaOperationParameterNameAttr";

    private static final String SCA_OPERATION_PARAMETER_CONST_ATTR = "scaOperationParameterConstAttr";

    private static final String SCA_INTERFACE_NAME_ATTR = "scaInterfaceNameAttr";

    private static final String CPP_HEADER = "cppHeader";

    private static boolean TESTING = true;

    private static TransformerFactory txmf = null;

    private static ServicesGenerator instance = null;

    public ServicesGenerator(String[] args) throws Exception {
        super(args);
        txmf = TransformerFactory.newInstance();
    }

    /**
     * 
     * @param parameters
     * @param forReference
     *            true if we are creating a proxy for a reference (rather than
     *            for a service)
     * @throws Exception
     * 
     * The design is
     * <ul>
     * <li>handleInterfaceHeader
     * <li>createDOMofMethods
     * <li>createProxyCPPFromDom(outputDir, dom);
     * <li>createProxyHeaderFromDom(outputDir, dom);
     * </ul>
     * 
     * plus if we are not generating for a reference element
     * <ul>
     * <li>createWrapperCPPFromDom(outputDir, dom);
     * <li>createWrapperHeaderFromDom(outputDir, dom);
     * <ul>
     * Each of the create.... methods calls createOutputFromDom with a different
     * style sheet.
     *  
     */
    public static void handleInterfaceHeader(Map parameters,
            boolean forReference) throws Exception {
        boolean failed = false;

        String type = null;
        if (forReference) {
            type = "reference";
        } else {
            type = "service";
        }
        String interfaceHeaderFilename = (String) parameters
                .get("/componentType/" + type + "/interface.cpp/@header");

        String componentTypeFileHeaderName = interfaceHeaderFilename;

        String sca_module_root =  (String) Options.getOption("-dir");
        parameters.put("module_root", sca_module_root);

        if (sca_module_root != null && interfaceHeaderFilename != null
                && interfaceHeaderFilename.length() > 0) {
            String separator;
            // Stick in a "/" (File.separator) if required.
            if ((interfaceHeaderFilename.substring(0, 1).equals("/") || sca_module_root
                    .substring(sca_module_root.length() - 1,
                            sca_module_root.length()).equals("/"))
                    || (interfaceHeaderFilename.substring(0, 1).equals("\\") || sca_module_root
                            .substring(sca_module_root.length() - 1,
                                    sca_module_root.length()).equals("\\"))

            ) {
                separator = "";
            } else {
                separator = File.separator;
            }
            interfaceHeaderFilename = sca_module_root + separator
                    + interfaceHeaderFilename;
        }

        File outputDir = (File) parameters.get("targetDirectoryFile");

        String[] args = new String[] { "-source", interfaceHeaderFilename,
                "-target", outputDir.getPath() };
        ServicesGenerator gen = new ServicesGenerator(args);
        File file;
        try {
            if (null == interfaceHeaderFilename) {
                gen.printUsage();
                System.exit(-1);
            }

            file = new File(interfaceHeaderFilename);
            if (!file.isFile()) {
                if (file.isDirectory()) {
                    Utils
                            .rude("This tool works at the header file level and not for directories like "
                                    + file);
                }
                Utils.rude("Bad file or directory " + file);
            }
            File source = file;
            if (!outputDir.exists() && !outputDir.mkdir())
                Utils.rude("Failed to create directory " + outputDir);

            // The class below is the one that will go through the header
            // file(s)
            Headers headers = new Headers();

            if (null != interfaceHeaderFilename) {
                Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_ENCOUNTERED, file
                        .getAbsolutePath());
                Utils.postEvent(Utils.EVENT_TYPE_FILE_PARSED,
                        "Scagen processing C++ interface header "
                                + file.getAbsolutePath());
                headers.actOnFile(file, outputDir, 1);
                Utils.outputDebugString("Parsing files...");
            }

            String nameOfSorR = null;
            String referenceName = null;
            String serviceName = null;

            if (forReference) {

                Object rn = parameters.get("/componentType/reference/@name");

                if (rn instanceof String) {
                    referenceName = (String) rn;
                    nameOfSorR = referenceName;
                }
            } else {

                Object sn = parameters.get("/componentType/service/@name");

                if (sn instanceof String) {
                    serviceName = (String) sn;
                    nameOfSorR = serviceName;
                }
            }

            String moduleXmlFileHeader = null;
            String moduleXmlFileHeaderNoExt = null;
            Object moduleh = parameters
                    .get("/module/component/implementation.cpp/@header");

            if (moduleh == null) {
                moduleh = parameters
                        .get("/moduleFragment/component/implementation.cpp/@header");
            }

            if (moduleh instanceof String) {
                File f = new File((String) moduleh);
                moduleXmlFileHeader = (String) moduleh;

                String fname = f.getName();
                moduleXmlFileHeaderNoExt = fname.substring(0, fname
                        .lastIndexOf('.'));

            }

            String implClassNameAttrFromModuleFile = (String) parameters
            	.get("implClass");
            String implClassNamespaceAttrFromModuleFile = (String) parameters
            	.get("implNamespace");
            
            if(implClassNamespaceAttrFromModuleFile == null || implClassNamespaceAttrFromModuleFile.length() == 0)
            {
                implClassNamespaceAttrFromModuleFile = "";
            }
            else
            {
                implClassNamespaceAttrFromModuleFile += "::";
            }
            
            String interfaceClassNameAttrFromComponentTypeFile;
            if (forReference) {
                interfaceClassNameAttrFromComponentTypeFile = (String) parameters
                        .get("/componentType/reference/interface.cpp/@class");
            } else {
                interfaceClassNameAttrFromComponentTypeFile = (String) parameters
                        .get("/componentType/service/interface.cpp/@class");
            }

            List methods = headers.getAllMethods();
            
            // Pull out one of the methods' namespace attributes.
            String intfNamespace = null;
            if (methods.size() > 0) {
                Signature method = (Signature) methods.get(0);
                intfNamespace = method.getNamespace();
            }

            if (interfaceClassNameAttrFromComponentTypeFile != null) {
                methods = filterToPublicMethodsOfGivenClass(methods,
                        interfaceClassNameAttrFromComponentTypeFile, true);
            } else {
                //We want to filter to methods of the class whose
                //name matches the header file name.
                //      String intfClassName = (String) parameters.get("intfClass");
                //
                String headerFileBase = file.getName().replaceAll(
                        "\\.h|\\.hpp|\\.h++", "");

                methods = filterToPublicMethodsOfGivenClass(methods,
                        headerFileBase, false);
            }

            Document dom = createDOMofMethods(methods, source, serviceName,
                    referenceName, nameOfSorR, null,
                    componentTypeFileHeaderName, moduleXmlFileHeader,
                    moduleXmlFileHeaderNoExt, intfNamespace,
                    interfaceClassNameAttrFromComponentTypeFile,
                    implClassNameAttrFromModuleFile, implClassNamespaceAttrFromModuleFile);
           
//            // Print out the generated DOM
//            StringWriter sw = new StringWriter();
//            OutputFormat outputFormat = new OutputFormat("xml", "UTF-8", true);
//            XMLSerializer serializer = new XMLSerializer(sw, outputFormat);
//            serializer.serialize(dom);
//            System.out.println("DOM:\n"+sw.toString()); 
            
            
            createProxyCPPFromDom(outputDir, dom);
            createProxyHeaderFromDom(outputDir, dom);

            if (!forReference) {
                createWrapperCPPFromDom(outputDir, dom);
                createWrapperHeaderFromDom(outputDir, dom);
            }

        } catch (Exception exception) {
            exception.printStackTrace();
            failed = true;
        }

        if (failed) {
            Utils.outputDebugString("Finished! (but encountered problems)");
            System.exit(-2);
        }
    }

    /**
     * This methods takes a list of methods and filters them to only contain the
     * public methods of the given class
     * 
     * @param methods
     *            a list of methods
     * @param className
     *            we will return a list of only this classes methods from the
     *            methods parameter
     * @param attrSpecified
     *            if the user does not specify one we ignore namespaces
     * @return
     */
    private static List filterToPublicMethodsOfGivenClass(List methods,
            String className, boolean useNamespace) {
        if (methods != null && className != null && className.length() > 0) {
            for (Iterator iter = methods.listIterator(); iter.hasNext();) {
                Signature method = (Signature) iter.next();
                String scope = method.getScope();
                String sig = method.getOriginal();
                String namespace = method.getNamespace();

                String qualifiedClassName;
                if (useNamespace && null != namespace && 0 < namespace.length()) {
                    qualifiedClassName = namespace + "::"
                            + method.getTrimClassName();
                } else {
                    qualifiedClassName = method.getTrimClassName();
                }

                // If we are not public or the classname does not match
                // remove...
                if (!qualifiedClassName.equals(className)
                        || -1 == scope.indexOf("public")) {
                    iter.remove();
                }
            }
        }
        return methods;
    }

    /**
     * Use an XSLT transformation to create a Proxy "view" of the DOM
     * information
     * 
     * @param outputDir
     *            where to put the C++
     * @param dom
     *            the DOM of semantic method information
     * @throws TransformerException
     */
    private static void createProxyCPPFromDom(File outputDir, Document dom)
            throws TransformerException {

        //Create the Filename from the Service Name in the DOM
        if (dom == null) {
            return;
        }

        String serviceOrReferenceName = "noSorRNameDefined";

        String implClass = "nomoduleXmlFileHeaderDefined";

        Element topNode = dom.getDocumentElement();
        if (null != topNode) {
            Attr attr = topNode.getAttributeNode("nameOfSorR");
            if (attr != null) {
                serviceOrReferenceName = attr.getNodeValue();
            }

            attr = topNode.getAttributeNode("implClass");
            if (attr != null) {
                implClass = attr.getNodeValue();
            }
        }

        File proxyCPP = new File(outputDir, implClass + "_"
                + serviceOrReferenceName + "_Proxy.cpp");
        if (null != proxyCPP) {

            Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_GENERATED, proxyCPP
                    .getAbsolutePath());
            Utils.postEvent(Utils.EVENT_TYPE_FILE_CREATE,
                    "Scagen creating SCA for C++ proxy implementation "
                            + proxyCPP.getAbsolutePath());
        }

        createOutputFromDom(dom, proxyCPP,
                "org/apache/tuscany/sca/cpp/tools/services/xsl/SCA4CPPIntfProxyCPP.xsl");

    }

    /**
     * Use an XSLT transformation to create a Wrapper "view" of the DOM
     * information
     * 
     * @param outputDir
     *            where to put the C++
     * @param dom
     *            the DOM of semantic method information
     * @throws TransformerException
     */
    private static void createWrapperCPPFromDom(File outputDir, Document dom)
            throws TransformerException {

        //Create the Filename from the Service Name in the DOM
        if (dom == null) {
            return;
        }

        String serviceName = "noServiceDefined";

        String implClass = "nomoduleXmlFileHeaderDefined";

        Element topNode = dom.getDocumentElement();
        if (null != topNode) {
            Attr attr = topNode.getAttributeNode("serviceName");
            if (attr != null) {
                serviceName = attr.getNodeValue();
            }

            attr = topNode.getAttributeNode("implClass");
            if (attr != null) {
                implClass = attr.getNodeValue();
            }
        }

        File wrapperCPP = new File(outputDir, implClass + "_" + serviceName
                + "_Wrapper.cpp");
        if (null != wrapperCPP) {
            Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_GENERATED, wrapperCPP
                    .getAbsolutePath());
            Utils.postEvent(Utils.EVENT_TYPE_FILE_CREATE,
                    "Scagen creating SCA for C++ wrapper implementation "
                            + wrapperCPP.getAbsolutePath());
        }
        createOutputFromDom(dom, wrapperCPP,
                "org/apache/tuscany/sca/cpp/tools/services/xsl/SCA4CPPIntfWrapperCPP.xsl");

    }

    /**
     * Use an XSLT transformation to create a Proxy header "view" of the DOM
     * information
     * 
     * @param outputDir
     *            where to put the C++
     * @param dom
     *            the DOM of sematic method information
     * @throws TransformerException
     */
    private static void createProxyHeaderFromDom(File outputDir, Document dom)
            throws TransformerException {

        //Create the Filename from the Service Name in the DOM
        if (dom == null) {
            return;
        }

        String serviceOrReferenceName = "noServiceDefined";
        String intfClass = "noInterfaceClassNameOrDefaultDefined";
        String implClass = "noImplementationClassOrDefaultDefined";

        Element topNode = dom.getDocumentElement();
        if (null != topNode) {
            Attr attr = topNode.getAttributeNode("nameOfSorR");
            if (attr != null) {
                serviceOrReferenceName = attr.getNodeValue();
            }
            attr = topNode.getAttributeNode("intfClass");
            if (attr != null) {
                intfClass = attr.getNodeValue();
            }
            attr = topNode.getAttributeNode("implClass");
            if (attr != null) {
                implClass = attr.getNodeValue();
            }
        }

        File proxyHeader = new File(outputDir, implClass + "_"
                + serviceOrReferenceName + "_Proxy.h");
        if (null != proxyHeader) {
            Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_GENERATED, proxyHeader
                    .getAbsolutePath());
            Utils.postEvent(Utils.EVENT_TYPE_FILE_CREATE,
                    "Scagen creating SCA for C++ proxy header "
                            + proxyHeader.getAbsolutePath());
        }

        createOutputFromDom(dom, proxyHeader,
                "org/apache/tuscany/sca/cpp/tools/services/xsl/SCA4CPPIntfProxyHeader.xsl");

    }

    /**
     * Use an XSLT transformation to create a Wrapper header "view" of the DOM
     * information
     * 
     * @param outputDir
     *            where to put the C++
     * @param dom
     *            the DOM of sematic method information
     * @throws TransformerException
     */
    private static void createWrapperHeaderFromDom(File outputDir, Document dom)
            throws TransformerException {

        //Create the Filename from the Service Name in the DOM
        if (dom == null) {
            return;
        }

        String serviceName = "noServiceDefined";

        String implClass = "nomoduleXmlFileHeaderDefined";

        Element topNode = dom.getDocumentElement();
        if (null != topNode) {
            Attr attr = topNode.getAttributeNode("serviceName");
            if (attr != null) {
                serviceName = attr.getNodeValue();
            }

            attr = topNode.getAttributeNode("implClass");
            if (attr != null) {
                implClass = attr.getNodeValue();
            }
        }

        File wrapperHeader = new File(outputDir, implClass + "_" + serviceName
                + "_Wrapper.h");

        if (null != wrapperHeader) {
            Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_GENERATED, wrapperHeader
                    .getAbsolutePath());
            Utils.postEvent(Utils.EVENT_TYPE_FILE_CREATE,
                    "Scagen creating SCA for C++ wrapper header "
                            + wrapperHeader.getAbsolutePath());
        }
        createOutputFromDom(dom, wrapperHeader,
                "org/apache/tuscany/sca/cpp/tools/services/xsl/SCA4CPPIntfWrapperHeader.xsl");

    }

    /**
     * This method will return a class loader that can see the resources that
     * are parts of "this" package.
     */
    private static ClassLoader getALoader() {

        LittleClass lc;
        try {
            lc = new LittleClass();
            return lc.getClass().getClassLoader();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * This method will generically process the DOM using a stylesheet passed
     * in.
     * 
     * @param dom
     *            the source of data (the model)
     * @param outputFile
     *            where to put the result
     * @param xslTransformationFileName
     *            the xsl file containing the "view"
     * @throws TransformerException
     */
    private static void createOutputFromDom(Document dom, File outputFile,
            String xslTransformationFileName) throws TransformerException {
        
        if(Options.noGenerate()){
            return;
        }
        
        InputStream stream = getALoader().getResourceAsStream(
                xslTransformationFileName);
        StreamSource ss = new StreamSource(stream);
        StreamResult out = new StreamResult(outputFile);
        try {
            Transformer xslt = txmf.newTransformer(ss);

            // Unless a width is set, there will be only line breaks but no
            // indentation.
            // The IBM JDK and the Sun JDK don't agree on the property name,
            // so we set them both.
            //
            try {
                xslt.setOutputProperty(
                        "{http://xml.apache.org/xalan}indent-amount", "2");
            } catch (Throwable t) {
                //OK to ignore this, depends on XSLT engine and one will fail
            }
            try {
                xslt.setOutputProperty(
                        "{http://xml.apache.org/xslt}indent-amount", "2");
            } catch (Throwable t) {
                //OK to ignore this, depends on XSLT engine and one will fail
            }

            DOMSource from = new DOMSource(dom);

            xslt.transform(from, out);

        } catch (TransformerConfigurationException e) {
            e.printStackTrace();
            throw (e);
        } catch (TransformerException e) {
            e.printStackTrace();
            throw (e);
        } finally {
            try {
                stream.close();
            } catch (IOException e1) {
                // unlikely but if
                // we can't close it, we can't close it
            }
        }

    }

    //TODO clear up the parameter list below to make it smaller if
    // possible
    /**
     * This method will create A DOM containing all the semantic information
     * that it can extract from a C++ header file.
     * 
     * @param methods
     *            a list of methods that are going into the DOM
     * @param source
     *            the header file the methods came from
     * @param serviceName
     *            the name of the service
     * @referenceName the name of the reference
     * @nameOfSorR the non null one of the two parameters above
     * @headerClassName the name of the header class
     * @param moduleXmlFileImplHeaderNameWithPathAndExt
     *            the source filename
     * @param moduleXmlFileHeaderNoExtorPath
     *            the shortname of the source file
     * @param implClass
     *            the implementation class
     * @param intfClass
     *            the interface we are turning into a service
     * @return
     */
    private static Document createDOMofMethods(List methods, File source,
            String serviceName, String referenceName, String nameOfSorR,
            String headerClassName,
            String componentTypeXmlFileIntfHeaderNameWithPathAndExt,
            String moduleXmlFileImplHeaderNameWithPathAndExt,
            String moduleXmlFileHeaderNoExtorPath, String intfNamespace,
            String intfClass, String implClass, String implNamespace) {

        if (methods == null) {
            return null;
        }

        // Create a DOM of the methods
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        Document document = null;
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();
            document = builder.newDocument();

            Element root = (Element) document.createElement(CPP_HEADER);

            root.setAttribute(HEADER_NAME_ATTR, source.getPath());

            document.appendChild(root);

            Element intf = (Element) document.createElement(SCA_SERVICE);
            root.appendChild(intf);

            // Go through all the signatures we have collected...
            Signature s = null;
            for (Iterator iter = methods.iterator(); iter.hasNext();) {
                s = (Signature) iter.next();

                // Each signature is an operation
                Element op = document.createElement(SCA_OPERATION);
                op.setAttribute(OPERATION_NAME_ATTR, s.getMethodName());
                intf.appendChild(op);

                Parameter[] parms = s.getParameters();
                if (parms != null) {

                    for (int i = 0; i < parms.length; i++) {
                        Element parm = (Element) document
                                .createElement(SCA_OPERATION_PARAMETER);
                        String type_string = parms[i].getTypeWithoutConst()
                                .toString();

                        type_string = type_string.replaceAll(":: ", "::");

                        Text text = document.createTextNode(type_string);
                        parm.appendChild(text);
                        parm.setAttribute(SCA_OPERATION_PARAMETER_NAME_ATTR,
                                parms[i].getName());

                        if (parms[i].getTypeWithoutConst().intern() != parms[i]
                                .getType().intern()) {
                            parm.setAttribute(
                                    SCA_OPERATION_PARAMETER_CONST_ATTR, "true");
                        }

                        op.appendChild(parm);
                        // TO DO only really stores the value
                        // unsafely/temporarily
                        // which is fine while we handle everything at the
                        // end of parsing the "leaf" that represents that actual
                        // interface/service but the below will cause the second
                        // service
                        // processed to overwite the first one in the DOM. I we
                        // wish
                        // to do some overall processing at the end we will have
                        // to
                        // use a better more XPath like key that varies by
                        // instance
                        // of the service.
                        intf.setAttribute(SCA_INTERFACE_NAME_ATTR, s
                                .getTrimClassName());
                    }
                }

                Element rc = document.createElement(SCA_OPERATION_RETURN_TYPE);
                rc.appendChild(document.createTextNode(s.getReturnType()
                        .toString().replaceAll(":: ", "::")));
                op.appendChild(rc);

                root.appendChild(intf);
            }

            // Set the name of the Service
            // here, if we are not passed one we use
            // the classname from the last header function signature...
            if (serviceName == null && s != null) {
                serviceName = s.getTrimClassName();
            }

            // this is used for the proxy file name but we need to
            // use the reference name if this is pulled in due to it
            // being a reference!!!
            if (serviceName != null)
                root.setAttribute("serviceName", serviceName);
            if (referenceName != null)
                root.setAttribute("referenceName", referenceName);
            if (nameOfSorR != null)
                root.setAttribute("nameOfSorR", nameOfSorR);

            if (implClass != null) {
                root.setAttribute("implClass", implClass);
            } else {
                root.setAttribute("implClass", moduleXmlFileHeaderNoExtorPath);
            }

            if (implClass != null) {
                root.setAttribute("implNamespace", implNamespace);
            }

            // default class name to the name of the header...
            if (headerClassName == null && source != null) {
                headerClassName = source.getName().replaceAll(
                        "\\.h|\\.hpp|\\.h++", "");
            }
            root.setAttribute("headerClassName", headerClassName);

            if (null == intfClass) {
                intfClass = headerClassName;
            }

            if (null != intfNamespace) {
                root.setAttribute("namespace", intfNamespace);
                if (null != intfClass && !intfClass.startsWith(intfNamespace)) {
                    intfClass = intfNamespace + "::" + intfClass;
                }
            }

            root.setAttribute("intfClass", intfClass);

            if (componentTypeXmlFileIntfHeaderNameWithPathAndExt == null) {
                componentTypeXmlFileIntfHeaderNameWithPathAndExt = "componentTypeHeader";
            }
            root.setAttribute("componentTypeHeader",
                    componentTypeXmlFileIntfHeaderNameWithPathAndExt);

            if (moduleXmlFileImplHeaderNameWithPathAndExt == null) {
                moduleXmlFileImplHeaderNameWithPathAndExt = "moduleXmlFileImplHeader";
            }
            root.setAttribute("moduleXmlFileHeader",
                    moduleXmlFileImplHeaderNameWithPathAndExt);

            if (moduleXmlFileHeaderNoExtorPath == null) {
                moduleXmlFileHeaderNoExtorPath = "moduleXmlFileHeaderNoExt";
            }
            root.setAttribute("moduleXmlFileHeaderNoExt",
                    moduleXmlFileHeaderNoExtorPath);

        } catch (ParserConfigurationException pce) {
            // Parser with specified options can't be built
            pce.printStackTrace();
        }

        return document;

    }

}