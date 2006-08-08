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

import java.util.Map;

import org.apache.tuscany.sca.cpp.tools.common.Options;
import org.w3c.dom.Node;

/**
 * The purpose of this class is to process a <reference>element in a
 * componentType file and then trigger a call to the method in ServicesGenerator
 * to process the interface header file
 */
public class ReferenceDomNodeHandler extends GenericDomNodeHandler {

    /*
     * (non-Javadoc)
     * 
     * @see org.apache.tuscany.sca.cpp.services.DomNodeHandler#handleNode(org.w3c.dom.Node,
     *      java.lang.String, java.util.Map, java.util.Map)
     */
    public void handleNode(Node node, String contextXPath, Map handlers,
            Map parameters) {

        // Pick up attrs and children
        super.handleNode(node, contextXPath, handlers, parameters);

        //OK we know we are handling a reference
        //now go and create the wrapper and proxy for it
        createProxyForReference(parameters);

    }

    /**
     * This method is really just an adapter that adapts the -dir Option to a
     * value int he parameters map for "composite_root"
     * 
     * @param parameters
     *            the map of name-value parameters.
     */
    private void createProxyForReference(Map parameters) {
        try {
            String mr = (String) Options.getOption("-dir");
            parameters.put("composite_root", mr);
            ServicesGenerator.handleInterfaceHeader(parameters, true);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}