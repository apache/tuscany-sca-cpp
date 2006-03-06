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
import java.util.HashMap;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;

import org.apache.tuscany.sca.cpp.tools.common.FileActor;
import org.apache.tuscany.sca.cpp.tools.common.Utils;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

/**
 * The purpose of this abstract class is to provide a home for the standard
 * processing that is involved in turning a XML file into an internal DOM.
 */
public abstract class XMLFileActor implements FileActor {

    protected static Map handlers = new HashMap();

    protected static Map parameters = new HashMap();

    public boolean failed;
    
    private int filesActedOn=0;

    protected static TransformerFactory transformerFactory = TransformerFactory
            .newInstance();

    /**
     * This method is the main FileActor method
     * 
     * @see FileActor#actOnFile(File, File, int) Here we create an initial DOM
     *      and kick off the processing (using the handler map that has been set
     *      up by the concrete subclass).
     * 
     * @param moduleXML
     *            the sca.module or fragment file
     * @param target
     *            the target directory
     * @param depth
     *            not uesed here but in the
     * @see FileActor#actOnFile(File, File, int) interface to allow for
     *      recursive diving into a directory structure.
     */
    public void actOnFile(File moduleXML, File target, int depth)
            throws Exception {

        if (null == moduleXML || null == target) {
            return;
        }

        filesActedOn++;
        
        parameters.put("sourceFile", moduleXML);
        parameters.put("targetFile", target);

        if (transformerFactory.getFeature(DOMSource.FEATURE)
                && transformerFactory.getFeature(DOMResult.FEATURE)) {
            Document dom = createDomFromXMLFile(moduleXML);
            if (dom != null) {
                parameters.put("targetDirectoryFile", target);
                DomHandler.handleDom(dom, handlers, parameters);
            }
        }
    }

    /**
     * This method builds an in memory DOM from an XML file
     * 
     * @param xmlSourceFile
     *            the XML file we are handling
     * @return the resulting document
     */
    protected Document createDomFromXMLFile(File xmlSourceFile) {
        Document dom = null;
        DocumentBuilderFactory f = DocumentBuilderFactory.newInstance();
        //We do not validate via f.setValidating(true);
        f.setNamespaceAware(true);

        try {
            DocumentBuilder parser = f.newDocumentBuilder();
            dom = parser.parse(xmlSourceFile);
        } catch (SAXException sxe) {
            String path;
            try {
                path = xmlSourceFile.getCanonicalPath();
            } catch (IOException e) {
                path = xmlSourceFile.getPath();
            }
            Utils.screenMessage("There has been a SAXException of type "
                    + sxe.getLocalizedMessage());
            if (null != xmlSourceFile) {
                Utils.screenMessage(" when processing file " + path);
            } else {
                Utils.screenMessage(" as the input file is null.");
            }

// Leave for possible future debug option
//            Utils.screenMessage(" The returned Java exception is below.");
//            if (sxe.getException() != null)
//                sxe.getException().printStackTrace();
//            else
//                sxe.printStackTrace();
        } catch (ParserConfigurationException pce) {
            String path;
            try {
                path = xmlSourceFile.getCanonicalPath();
            } catch (IOException e) {
                path = xmlSourceFile.getPath();
            }
            Utils
                    .screenMessage("There has been a ParserConfigurationException of type "
                            + pce.getLocalizedMessage());
            if (null != xmlSourceFile) {
                Utils.screenMessage(" when processing file " + path);
            } else {
                Utils.screenMessage(" as the input file is null.");
            }
            
//          Leave for possible future debug option
//          Utils.screenMessage(" The returned Java exception is below.");
//          pce.printStackTrace();
            
        } catch (IOException ioe) {
            String path;
            try {
                path = xmlSourceFile.getCanonicalPath();
            } catch (IOException e) {
                path = xmlSourceFile.getPath();
            }
            
            Utils.screenMessage("Unable to open file  " + path);
            Utils.screenMessage(this.getContextMessage());
            
            
            
        }

        return dom;
    }

    /**
     * @return an error message - usually over-ridden.
     */
    private String getContextMessage() {
        return "Check the file exists and can be read.";
    }

    /**
     * Set a parameter
     * 
     * @param name
     * @param value
     */
    public void setParameter(String name, Object value) {
        parameters.put(name, value);
    }

    /**
     * Get a parameter
     * 
     * @param name
     * @param value
     * @return the value of the parameter
     */
    public Object getParameter(String name, Object value) {
        return parameters.get(name);
    }



    /**
     * @return Returns the filesActedOn.
     */
    public int getFilesActedOn() {
        return filesActedOn;
    }
}