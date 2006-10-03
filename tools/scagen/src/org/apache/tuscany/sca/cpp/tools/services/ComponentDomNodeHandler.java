/**
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */

/* @version $Rev$ $Date$ */

package org.apache.tuscany.sca.cpp.tools.services;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.tuscany.sca.cpp.tools.common.Headers;
import org.apache.tuscany.sca.cpp.tools.common.Signature;
import org.apache.tuscany.sca.cpp.tools.common.Utils;
import org.w3c.dom.Node;

/**
 * This class will do the required processing for the <component>element of a
 * sca composite file.
 */
public class ComponentDomNodeHandler extends GenericDomNodeHandler {

    /**
     * This method will do the "normal" processing and then trigger a call to
     * processComponentNode.
     * 
     * @param node
     *            the node being processed
     * @param contextXPath
     *            the XPath to the node
     * @param handlers
     *            the map pf element names to DomNodeHandlers
     * @param parameters
     *            a map of XPaths to parameters values found so far
     */

    public void handleNode(Node node, String contextXPath, Map handlers,
            Map parameters) {

        // Pick up attrs and the interface.cpp child elements
        super.handleNode(node, contextXPath, handlers, parameters);

        try {
            //OK now go and create the wrapper and proxy for the service
            processComponentNode(contextXPath, parameters);
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    /**
     * This method basically moved from the text names of things to operating on
     * the actual Files. It will also verify or work out the correct class name
     * for the implmentation and complain if this does match at least one
     * potential service method in the class.
     * 
     * @param contextXPath
     *            used to pull the correct values from the parameters map (as
     *            there can be multiple implementation.cpp elelements in there).
     * @param parameters
     *            a map of XPath keys to attribute values
     * @throws Exception
     */
    private void processComponentNode(String contextXPath, Map parameters)
            throws Exception {

        String implHeader = (String) parameters.get(contextXPath
                + "/implementation.cpp/@header");
        String implClass = (String) parameters.get(contextXPath
                + "/implementation.cpp/@class");

        File compositeOrFragmentFile = (File) parameters
                .get("compositeOrFragmentFile");
        File implHeaderFile = null;
        if (null != compositeOrFragmentFile) {
            File dir = compositeOrFragmentFile.getParentFile();
            implHeaderFile = new File(dir, implHeader);
        } else {
            throw new InternalError(
                    "Internal error: composite or fragment file not present in internal parameters");
        }

        try {
            String resolvedImplClassName = getClassName(implHeaderFile,
                    implClass);

            // Check or retrieve the impl Class name.
            if (null == resolvedImplClassName) {
                try {
                    //A class attribute was set but there were no methods of
                    // the
                    // class in the header
                    System.out
                            .println("Classname given ("
                                    + implClass
                                    + ") does not match any header file method's classes in file: "
                                    + implHeaderFile.getCanonicalPath());
                } catch (IOException e) {
                    System.out
                            .println("Classname given ("
                                    + implClass
                                    + ") does not match any header file method's classes in file: "
                                    + implHeaderFile.getAbsolutePath());
                }
                return;
            } else {
                File target = (File) parameters.get("targetFile");
                // go into the .componentType file and generate the cpp
                processComponentTypeFile(implHeaderFile, target,
                        resolvedImplClassName);

            }
        } catch (Exception e) {
            String compName = (String) parameters
                    .get("/compositeFragment/component/@name");
            Utils
                    .screenMessage("Problem interpreting header or class attributes in "
                            + compName
                            + " component, in "
                            + compositeOrFragmentFile.getPath() + " file");
            System.exit(-2);
        }

    }

    /**
     * The purpose of this method is to move from the DOM parameters to dealing
     * with the actual Files involved. It is from this method that we kick off
     * the processing of the componentType file.
     * 
     * @param header
     *            the implementation header
     * @param target
     *            the directory for the output
     * @param implClass
     * @throws Exception
     */
    private void processComponentTypeFile(File header, File target,
            String implClass) throws Exception {

        // The componentType files should be in the same dir as the Impl
        // header...
        if (header == null || target == null) {
            return;
        }

        File componentTypeDirectory = header.getParentFile();
        String headerFileName = header.getName();
        String componentTypeName = headerFileName.substring(0, headerFileName
                .lastIndexOf("."));

        File componentTypeFile = new File(componentTypeDirectory,
                componentTypeName + ".componentType");

        ComponentTypeFileHandler ctParser = new ComponentTypeFileHandler();

        // The implClass is used in the generated wrapper code so we need to
        // store
        // it so we can tunnel through the standard actOnFile signature.

        int namespaceEnd = -1;
        if (null != implClass) {
            namespaceEnd = implClass.lastIndexOf("::");
        }

        String namespace = null;

        if (-1 != namespaceEnd) {
            namespace = implClass.substring(0, namespaceEnd);
            ctParser.setParameter("implNamespace", namespace);
            implClass = implClass.substring(namespaceEnd + 2);
        }

        if (implClass != null) {
            ctParser.setParameter("implClass", implClass);
        }

        try {
            ctParser.handleComponentTypeFile(componentTypeFile, target);
        } catch (Exception e) {
            Utils
                    .screenMessage("There has been a problem parsing the componentType file: "
                            + componentTypeFile.getCanonicalPath());
            Utils.screenMessage(" the reported errors is "
                    + e.getLocalizedMessage());
            Utils.screenMessage(" and the java exception stack is below.");
            e.printStackTrace();
            throw e;
        }
    }

    /**
     * The resolve and check the classname of the service. If we have an
     * implementation class name we have to check that there is: at least one
     * (non-private, non constructor or finalizer) method of that class in the
     * header If there is no implementation class then we will return the class
     * of the first non-private/constructor/finalizer method we find.
     * 
     * @param header
     * @param implementationCppClass
     * @return
     * @throws Exception
     */
    private String getClassName(File header, String implementationCppClass)
            throws Exception {
        String methodClassName = null;
        List methods = null;

        if (null == header) {
            return null;
        }

        Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_ENCOUNTERED, header
                .getAbsolutePath());
        Utils.postEvent(Utils.EVENT_TYPE_FILE_PARSED,
                "Scagen processing C++ implementation header "
                        + header.getAbsolutePath());

        try {
            Headers headers = new Headers();

            headers.actOnFile(header, null, 1);

            methods = headers.getAllMethods();

        } catch (FileNotFoundException fnfe) {
            String path;
            try {
                path = header.getCanonicalPath();
            } catch (IOException e1) {
                path = header.getPath();
            }
            Utils.screenMessage("The header file: " + path
                    + " referenced cannot be found.");
            throw fnfe;
        } catch (Exception e) {
            String path = header.getPath();
            Utils.screenMessage("The header file: " + path
                    + " referenced is not valid. Reason given is "
                    + e.getLocalizedMessage());
            throw e;
        }

        // We need at least some methods
        if (null == methods) {
            return null;
        }

        // We need at least one service method of to do anything
        methods = trimMethodsOfPrivatesConstructorsAndDestrutors(methods);
        if (null == methods || methods.size() == 0) {
            return null;
        }

        // If the user specifies an implementation class then we need at
        // least one service method of that class
        if (implementationCppClass != null) {
            methods = filterMethodsToOneClass(methods, implementationCppClass);

            if (null == methods || methods.size() == 0) {
                return null;
            } else {
                // There was at least one method of the correct type
                return implementationCppClass;
            }
        } else {
            // Implementation class is null so we return the fully qualified classname of the
            // first service method
            Signature s = (Signature) methods.get(0); 
            String className = s.getTrimClassName();
            String namespace = s.getNamespace();
            if( namespace != null && namespace.length() > 0)
            {
                className = namespace + "::" + className;
            }
            
            return className;
        }
    }

    /**
     * Filter the mthods supplied to only ones fo the supplied class.
     * 
     * @param methods
     *            the list of methods
     * @param implementationCppClass
     *            the class we wish
     * @return a list of methods of the correct class
     */
    private List filterMethodsToOneClass(List methods,
            String implementationCppClass) {

        if (null == methods) {
            return null;
        }

        if (null == implementationCppClass
                || implementationCppClass.length() == 0) {
            return null;
        }

        for (Iterator iter = methods.listIterator(); iter.hasNext();) {
            Signature method = (Signature) iter.next();

            String className = method.getTrimClassName();
            String namespace = method.getNamespace();

            if (namespace != null && namespace.length() > 0) {
                className = namespace + "::" + className;
            }

            if (!implementationCppClass.equals(className)) {
                iter.remove();
            }
        }

        return methods;

    }

    /**
     * This method removes contructor and destructor methods from the list.
     * 
     * @param methods
     *            the list of methods
     * @return
     */
    private List trimMethodsOfPrivatesConstructorsAndDestrutors(List methods) {

        if (null == methods) {
            return null;
        }

        for (Iterator iter = methods.listIterator(); iter.hasNext();) {
            Signature method = (Signature) iter.next();

            if (method.isConstructor() || method.isDestructor()) {
                iter.remove();
            }
        }

        return methods;
    }

}
