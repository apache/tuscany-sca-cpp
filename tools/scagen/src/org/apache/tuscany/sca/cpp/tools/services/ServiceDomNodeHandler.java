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

import java.util.Map;

import org.apache.tuscany.sca.cpp.tools.common.Options;
import org.w3c.dom.Node;

public class ServiceDomNodeHandler extends GenericDomNodeHandler {

    /*
     * (non-Javadoc)
     * 
     * @see org.apache.tuscany.sca.cpp.services.DomNodeHandler#handleNode(org.w3c.dom.Node,
     *      java.lang.String, java.util.Map, java.util.Map)
     */
    public void handleNode(Node node, String contextXPath, Map handlers,
            Map parameters) {

        // Pick up attrs and the interface.cpp child elements
        super.handleNode(node, contextXPath, handlers, parameters);

        //OK now go and create the wrapper and proxy for the service
        createWrapperAndProxyForService(parameters);

    }

    /**
     * This method will generate the wrapper and proxy C++ and header files for
     * the service.
     * 
     * @param parameters
     *            a map of name-value parameters
     */
    private void createWrapperAndProxyForService(Map parameters) {

        try {
            String mr = (String) Options.getOption("-dir");
            parameters.put("composite_root", mr);
            ServicesGenerator.handleInterfaceHeader(parameters, false);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}