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
 * The purpose of this class is purely to specialise the handler map to one with
 * a specific ComponentDomNodeHandler.
 */
public class CompositeOrFragmentFileHandler extends XMLFileActor {

    static {

        GenericDomNodeHandler gdnh = new GenericDomNodeHandler();

        /*
         * We use a specific Component node handler in order to be able to
         * process multiple components in the same XML file
         */
        ComponentDomNodeHandler componentdnh = new ComponentDomNodeHandler();
        handlers.put("component", componentdnh);

        /*
         * We are interested inthe elements below but they only need standard
         * processing
         */
        handlers.put("composite", gdnh);
        handlers.put("compositeFragment", gdnh);
        handlers.put("implementation.cpp", gdnh);
    }

    /**
     * This method is the main FileActor method
     * 
     * @see FileActor#actOnFile(File, File, int) Here we create an initial DOM
     *      and kick off the processing (using the handler map that has been set
     *      up by the concrete subclass).
     * 
     * @param compositeXML
     *            the composite or fragment file
     * @param target
     *            the target directory
     * @param depth
     *            not uesed here but in the
     * @see FileActor#actOnFile(File, File, int) interface to allow for
     *      recursive diving into a directory structure.
     */
    public void actOnFile(File compositeXML, File target, int depth)
            throws Exception {

        if (null == compositeXML || null == target) {
            return;
        }

        parameters.put("compositeOrFragmentFile", compositeXML);

        Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_ENCOUNTERED, compositeXML
                .getAbsolutePath());

        Utils.postEvent(Utils.EVENT_TYPE_FILE_PARSED,
                "Scagen processing SCA composite file "
                        + compositeXML.getAbsolutePath());

        super.actOnFile(compositeXML, target, depth);

    }
}