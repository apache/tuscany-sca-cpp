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
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import org.apache.tuscany.sca.cpp.tools.common.Utils;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * 
 * 
 * 
 *  
 */
public class GenericDomNodeHandler implements DomNodeHandler {
    /**
     * This method parses the DOM attributes into name value pairs in the
     * parameter map each valued keyed by its XPath.
     * 
     * @param node
     *            the DOM node being processed
     * @param contextPath
     *            the XPath to the DOM node being processed
     * @param parameters
     *            a scratchpad map of name value pairs
     */
    public void transferAttrsToParameters(Node node, String contextPath,
            Map parameters) {

        if (node == null || contextPath == null || parameters == null)
            return;

        NamedNodeMap attrs = node.getAttributes();
        if (attrs != null) {
            for (int i = 0; i < attrs.getLength(); i++) {
                Node attr = attrs.item(i);
                parameters.put(contextPath + "/" + "@" + attr.getNodeName(),
                        attr.getNodeValue());

                // Report the dll name to the deployment tool if required.
                if ("dll".equals(attr.getNodeName())) {
                    //This is a path relative to the composite root.
                    //so we need to add it in.
                    File mod = (File) parameters.get("compositeOrFragmentFile");
                    if (null != mod) {
                        String separatorForRegex = File.separator.replaceAll(
                                "\\\\", "\\\\\\\\");

                        String end = attr.getNodeValue().replaceAll("[/\\\\]+",
                                separatorForRegex);

                        if (!end.startsWith(File.separator)) {
                            end = File.separator + end;
                        }

                        String fullPath = mod.getParentFile().getAbsolutePath()
                                + end;
                        Utils.postEvent(Utils.DEPLOYMENT_ARTEFACT_ENCOUNTERED,
                                fullPath);
                    }

                }

            }
        }
        return;
    }

    /**
     * This method will iterate through the DOM node's children and call the
     * appropriate handler for each one.
     * 
     * @param node
     *            the DOM node being processed
     * @param contextPath
     *            the XPath to the DOM node being processed
     * @param handlers
     *            a map of element name to handler objects
     * @param parameters
     *            a scratchpad map of name value pairs
     */
    public void handleChildElements(Node node, String contextPath,
            Map handlers, Map parameters) {

        if (node == null || contextPath == null)
            return;

        NodeList children = node.getChildNodes();
        if (children != null) {
            for (int i = 0; i < children.getLength(); i++) {
                Node child = children.item(i);
                mapNodeToHandlerAndHandle(child, contextPath + "/"
                        + child.getNodeName(), handlers, parameters);
            }
        }
        return;
    }

    /**
     * This method will resolve any registered handler for a particular DOM
     * element and call the handleNode method on it. If the handlers map has a
     * mapping from "entity" to an instance of EntityHandler which implements
     * the DomNodeHandler interface then any node that looks like <entity>
     * <any_other/><stuff/></entity>
     * 
     * Will be passed to the EntityHandler to process.
     * 
     * @param node
     *            the DOM node being processed
     * @param contextPath
     *            the XPath to the DOM node being processed
     * @param handlers
     *            a map of element name to handler objects
     * @param parameters
     *            a scratchpad map of name value pairs
     */
    public static void mapNodeToHandlerAndHandle(Node node, String contextPath,
            Map handlers, Map parameters) {
        if (node != null && node.getNodeType() == DomNodeHandler.ELEMENT) {
            String nodeName = node.getNodeName();
            if (nodeName != null && nodeName.length() > 0) {
                DomNodeHandler handler = (DomNodeHandler) handlers
                        .get(nodeName);
                if (handler != null) {
                    handler.handleNode(node, contextPath, handlers, parameters);
                }
            }
        }
    }

    /**
     * This method will place the attributes in this node into the parameter map
     * keyed by the XPath and recursively continue processing for any
     * sub-elements of the node.
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

        clearParametersAtThisXPath(contextXPath, parameters);
        transferAttrsToParameters(node, contextXPath, parameters);
        handleChildElements(node, contextXPath, handlers, parameters);
    }

    /**
     * @param contextXPath
     * @param parameters
     */
    private void clearParametersAtThisXPath(String contextXPath, Map parameters) {

        // TODO: Slow but works, improve parameters mechanism overall
        // to make this unecessary
        if (contextXPath == null || parameters == null) {
            return;
        }

        // We want to clear both /compositeFragment and /composite
        // subtrees when we come across the root of either..
        if (contextXPath.equals("/compositeFragment")) {
            //clear both this and "composite"
            contextXPath = "/composite";
        }

        Set parms = parameters.entrySet();
        if (null != parms) {
            Iterator iter = parms.iterator();
            Map.Entry item = null;
            String thisKey;

            while (iter.hasNext()) {
                item = (Entry) iter.next();
                thisKey = (String) item.getKey();
                if (thisKey.startsWith(contextXPath)) {
                    //System.out.println(" removing " + thisKey + " for " +
                    // item.getValue().toString() + " against " + contextXPath);
                    iter.remove();
                }
            }
        }
    }
}