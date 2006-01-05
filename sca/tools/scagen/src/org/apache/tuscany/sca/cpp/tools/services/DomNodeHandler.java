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

import org.w3c.dom.Node;

public interface DomNodeHandler {
    
    public static int ELEMENT = 1;
    /**
     * An interface for classes that can process DOM nodes.
     * 
     * @param node the DOM node being consumed
     * @param contextXPath the XPath to this node 
     * @param handlers The map from element name to node handler
     * @param parameters A map of parameters - this is often used by a handler to place
     * a name-value pair, the name is often an Xpath like representation of the
     * location of the data in the DOM but handlers are free to use whatever 
     * they like - the contextXpath is generated as an Xpath prefix for those
     * handlers that wish to use it.  
     */
     void handleNode( Node node, String contextXPath, Map handlers, Map parameters );

}
