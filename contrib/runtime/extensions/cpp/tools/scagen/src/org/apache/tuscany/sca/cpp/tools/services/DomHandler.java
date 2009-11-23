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

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * The purpose of this class it to provide a generic class that can handle both
 * a DOM and a DOM node.
 *  
 */
public class DomHandler extends GenericDomNodeHandler {
    /**
     * This method will run through the initial level of the DOM using the
     * handlers map
     * 
     * @param dom
     *            the document being consumed
     * @param handlers
     *            the map from element name to node handler
     * @param parameters
     *            a map of parameters - this is often used by a handler to place
     *            a name-value pair, the name is often an Xpath representation
     *            of the location of the data in the DOM but handlers are free
     *            to use whatever they like - the contextXpath is generated as
     *            an Xpath prefix for those handlers that wish to use it.
     */
    public static void handleDom(Document dom, Map handlers, Map parameters) {
        if (dom != null) {
            NodeList childNodes = dom.getChildNodes();
            for (int i = 0; i < childNodes.getLength(); i++) {
                Node childNode = childNodes.item(i);
                mapNodeToHandlerAndHandle(childNode, "/"
                        + childNode.getNodeName(), handlers, parameters);
            }
        }
    }

    /**
     * 
     * @param node
     *            The DOM node being consumed
     * @param contextXPath
     *            The XPath to this node
     * @param handlers
     *            The map from element name to node handler
     * @param parameters
     *            A map of parameters - this is often used by a handler to place
     *            a name-value pair, the name is often an Xpath representation
     *            of the location of the data in the DOM but handlers are free
     *            to use whatever they like - the contextXpath is generated as
     *            an Xpath prefix for those handlers that wish to use it.
     */
    public void handleNode(Node node, String contextXPath, Map handlers,
            Map parameters) {
        mapNodeToHandlerAndHandle(node, contextXPath, handlers, parameters);
    }

}