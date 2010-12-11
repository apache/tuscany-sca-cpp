/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.    
 */

/**
 * XML handling functions.
 */

/**
 * Convert a DOM node list to a regular list.
 */
function nodeList(n) {
    var l = new Array();
    if (isNil(n))
        return l;
    for (var i = 0; i < n.length; i++)
        l[i] = n[i];
    return l;
}

/**
 * Return the child attributes of an element.
 */
function childAttributes(e) {
    return filter(function(n) { return n.nodeType == 2; }, nodeList(e.attributes));
}

/**
 * Return the child elements of an element.
 */
function childElements(e) {
    return filter(function(n) { return n.nodeType == 1; }, nodeList(e.childNodes));
}

/**
 * Return the child text nodes of an element.
 */
function childText(e) {
    return filter(function(n) { return n.nodeType == 3; }, nodeList(e.childNodes));
}

/**
 * Read a list of XML attributes.
 */
function readAttributes(a) {
    if (isNil(a))
        return a;
    return cons(mklist(attribute, "'" + car(a).nodeName, car(a).nodeValue), readAttributes(cdr(a)));
}

/**
 * Read an XML element.
 */
function readElement(e) {
    var l = append(append(mklist(element, "'" + e.nodeName), readAttributes(childAttributes(e))), readElements(childElements(e)));
    var t = childText(e);
    if (isNil(t))
        return l;
    return append(l, mklist(car(t).nodeValue));
}

/**
 * Read a list of XML elements.
 */
function readElements(l) {
    if (isNil(l))
        return l;
    return cons(readElement(car(l)), readElements(cdr(l)));
}

/**
 * Parse a list of strings representing an XML document.
 */
function parseXML(l) {
    var s = writeStrings(l);
    if (window.DOMParser) {
        var p =new DOMParser();
        return p.parseFromString(s, "text/xml");
    }
    var doc;
    try {
        doc = new ActiveXObject("MSXML2.DOMDocument");
    } catch (e) {
        doc = new ActiveXObject("Microsoft.XMLDOM");
    }
    doc.async = 'false';
    doc.loadXML(s); 
    return doc;
}

/**
 * Read a list of values from an XML document.
 */
function readXMLDocument(doc) {
    var root = nodeList(doc.childNodes);
    if (isNil(root))
        return mklist();
    return mklist(readElement(car(root)));
}

/**
 * Read a list of values from a list of strings representing an XML document.
 */
function readXML(l) {
    return readXMLDocument(parseXML(l));
}

/**
 * Make an XML document.
 */
/**
 * Return a list of strings representing an XML document.
 */
function writeXMLDocument(doc) {
    if (typeof(XMLSerializer) != 'undefined')
        return mklist(new XMLSerializer().serializeToString(doc));
    return mklist(doc.xml);
}

/**
 * Write a list of XML element and attribute tokens.
 */
function expandElementValues(n, l) {
    if (isNil(l))
        return l;
    return cons(cons(element, cons(n, car(l))), expandElementValues(n, cdr(l)));
}

function writeList(l, node, doc) {
    if (isNil(l))
        return node;
    var token = car(l);
    if (isTaggedList(token, attribute)) {
        node.setAttribute(attributeName(token).substring(1), '' + attributeValue(token));
    } else if (isTaggedList(token, element)) {
        if (elementHasValue(token)) {
            var v = elementValue(token);
            if (isList(v)) {
                var e = expandElementValues(elementName(token), v);
                writeList(e, node, doc);
            } else {
                var child = doc.createElement(elementName(token).substring(1));
                writeList(elementChildren(token), child, doc);
                node.appendChild(child);
            }
        } else {
            var child = doc.createElement(elementName(token).substring(1));
            writeList(elementChildren(token), child, doc);
            node.appendChild(child);
        }
    } else
        node.appendChild(doc.createTextNode('' + token));
    writeList(cdr(l), node, doc);
    return node;
}

/**
 * Convert a list of values to a list of strings representing an XML document.
 */
function writeXML(l, xmlTag) {
    function mkdoc() { 
        if (document.implementation && document.implementation.createDocument)
            return document.implementation.createDocument('', '', null); 
        return new ActiveXObject("MSXML2.DOMDocument"); 
    }

    var doc = mkdoc();
    writeList(l, doc, doc);
    if (!xmlTag)
        return writeXMLDocument(doc);
    return mklist('<?xml version="1.0" encoding="UTF-8"?>\n' + writeXMLDocument(doc) + '\n');
}

