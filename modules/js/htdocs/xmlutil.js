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
 * Append a list of nodes to a parent node.
 */
function appendNodes(nodes, p) {
    if (isNull(nodes))
        return p;
    p.appendChild(car(nodes));
    return appendNodes(cdr(nodes), p);
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
    return filter(function(n) { return n.nodeType == 3 && n.nodeValue.trim().length != 0; }, nodeList(e.childNodes));
}

/**
 * Read a list of XML attributes.
 */
function readAttributes(a) {
    if (isNull(a))
        return a;
    var x = car(a);
    return cons(mklist(attribute, "'" + x.name, x.value), readAttributes(cdr(a)));
}

/**
 * Read an XML element.
 */
function readElement(e, childf) {
    var l = append(append(mklist(element, "'" + e.nodeName), readAttributes(childf(e))), readElements(childElements(e), childf));
    var t = childText(e);
    if (isNull(t))
        return l;
    var tv = reduce(function(a, n) { return a + n.nodeValue; }, '', t);
    return append(l, mklist(tv));
}

/**
 * Read a list of XML elements.
 */
function readElements(l, childf) {
    if (isNull(l))
        return l;
    return cons(readElement(car(l), childf), readElements(cdr(l), childf));
}

/**
 * Return true if a list of strings contains an XML document.
 */
function isXML(l) {
    if (isNull(l))
        return false;
    return car(l).substring(0, 5) == '<?xml';
}

/**
 * Parse a list of strings representing an XML document.
 */
var xmlParser = new DOMParser();
function parseXML(l) {
    var s = writeStrings(l);
    return xmlParser.parseFromString(s, "text/xml");
}

/**
 * Read a list of values from an XML document.
 */
function readXMLDocument(doc) {
    var root = childElements(doc);
    if (isNull(root))
        return nil;
    return mklist(readElement(car(root), childAttributes));
}

/**
 * Read a list of values from an XHTML element.
 */
function readXHTMLElement(xhtml) {
    return mklist(readElement(xhtml, childAttributes));
}

/**
 * Read a list of values from a list of strings representing an XML document.
 */
function readXML(l) {
    return readXMLDocument(parseXML(l));
}

/**
 * Return a list of strings representing an XML document.
 */
var xmlSerializer = new XMLSerializer();
function writeXMLDocument(doc) {
    return mklist(xmlSerializer.serializeToString(doc));
}

/**
 * Write a list of XML element and attribute tokens.
 */
function expandElementValues(n, l) {
    if (isNull(l))
        return l;
    return cons(cons(element, cons(n, car(l))), expandElementValues(n, cdr(l)));
}

function writeList(l, node, doc) {
    if (isNull(l))
        return node;

    var token = car(l);
    if (isTaggedList(token, attribute)) {
        if (isMSIE()) {
            var aname = attributeName(token).substring(1);
            if (aname != 'xmlns')
                node.setAttribute(aname, '' + attributeValue(token));
        } else
            node.setAttribute(attributeName(token).substring(1), '' + attributeValue(token));

    } else if (isTaggedList(token, element)) {

        function mkelem(tok, doc) {
            function xmlns(l) {
                if (isNull(l))
                    return null;
                var t = car(l);
                if (isTaggedList(t, attribute)) {
                    if (attributeName(t).substring(1) == 'xmlns')
                        return attributeValue(t);
                }
                return xmlns(cdr(l));
            }

            var ns = xmlns(elementChildren(tok));
            if (isMSIE())
                return doc.createElementNS(ns != null? ns : node.namespaceURI, elementName(tok).substring(1));
            if (ns == null)
                return doc.createElement(elementName(tok).substring(1));
            return doc.createElementNS(ns, elementName(tok).substring(1));
        }

        if (elementHasValue(token)) {
            var v = elementValue(token);
            if (isList(v)) {
                var e = expandElementValues(elementName(token), v);
                writeList(e, node, doc);
            } else {
                var child = mkelem(token, doc);
                writeList(elementChildren(token), child, doc);
                node.appendChild(child);
            }
        } else {
            var child = mkelem(token, doc);
            writeList(elementChildren(token), child, doc);
            node.appendChild(child);
        }
    } else
        node.appendChild(doc.createTextNode('' + token));

    writeList(cdr(l), node, doc);
    return node;
}

/**
 * Make a new XML document.
 */
function mkXMLDocument() { 
    return document.implementation.createDocument('', '', null); 
}

/**
 * Convert a list of values to a list of strings representing an XML document.
 */
function writeXML(l, xmlTag) {
    var doc = mkXMLDocument();
    writeList(l, doc, doc);
    if (!xmlTag)
        return writeXMLDocument(doc);
    return mklist('<?xml version="1.0" encoding="UTF-8"?>\n' + car(writeXMLDocument(doc)) + '\n');
}

