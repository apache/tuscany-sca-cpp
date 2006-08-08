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

import org.apache.tuscany.sca.cpp.tools.common.FileActor;
import org.apache.tuscany.sca.cpp.tools.common.Utils;

/**
 * The purpose of this class is to specialise the map of XML element handlers
 * for a XXX.componentType file that is used by the XMLFileActor
 */
public class ComponentTypeFileHandler extends XMLFileActor {

    static {
        // We set up a map for each element type we wish to handle
        // this alows the XML handling code to be generic and type free
        // while the handlers don't have to do mcuh XML handling.

        GenericDomNodeHandler gdnh = new GenericDomNodeHandler();
        handlers.put("componentType", gdnh);
        handlers.put("interface.cpp", gdnh);

        ServiceDomNodeHandler sdnh = new ServiceDomNodeHandler();
        handlers.put("service", sdnh);

        ReferenceDomNodeHandler rdnh = new ReferenceDomNodeHandler();
        handlers.put("reference", rdnh);
    }

    /**
     * This method just exists to add the default starting depth of 1 to the
     * underlying actOnFile interface
     * 
     * @param componentTypeXML
     * @param target
     * @throws Exception
     */
    public void handleComponentTypeFile(File componentTypeXML, File target)
            throws Exception {
        // We have already set up the XML element handlers.
        actOnFile(componentTypeXML, target, 1);
        // We need do no more, the service and reference handlers
        // ServiceDomNodeHandler and ReferenceDomNodeHandler
        // will take appropriate action.
    }

    /**
     * This method is the main FileActor method
     * 
     * @see FileActor#actOnFile(File, File, int) Here we create an initial DOM
     *      and kick off the processing (using the handler map that has been set
     *      up by the concrete subclass).
     * 
     * @param compositeXML
     *            the sca.composite or fragment file
     * @param target
     *            the target directory
     * @param depth
     *            not uesed here but in the
     * @see FileActor#actOnFile(File, File, int) interface to allow for
     *      recursive diving into a directory structure.
     */
    public void actOnFile(File fileXML, File target, int depth)
            throws Exception {

        if (null == fileXML || null == target) {
            return;
        }

        parameters.put("componentTypeFile", fileXML);
        
        Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_ENCOUNTERED, fileXML.getAbsolutePath());
        Utils.postEvent(Utils.EVENT_TYPE_FILE_PARSED, "Scagen processing SCA componentType file " + fileXML.getAbsolutePath());

        super.actOnFile(fileXML, target, depth);

    }

    /**
     * @return an error message - usually over-ridden.
     */
    protected String getContextMessage() {

        String composite = ((File) parameters.get("compositeOrFragmentFile")).getPath();
        if (null == composite) {
            composite = "unknown";
        }

        String component = (String) parameters.get("/composite/component/@name");
        if (null == component) {
            component = (String) parameters
                    .get("/compositeFragment/component/@name");
        }
        if (null == component) {
            composite = "unknown";
        }

        String msg = "when processing composite " + composite;

        msg = msg
                + "\nin this composite file, the component \""
                + component
                + "\" has an implementation.cpp element with a header attribute \nwhere the C++ header can be found but it has no matching .componentType file present in\nthe same directory as the header.";

        return msg;
    }

}