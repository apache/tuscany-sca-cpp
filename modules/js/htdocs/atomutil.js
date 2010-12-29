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
 * ATOM data conversion functions.
 */
var atom = new Object();

/**
 * Convert a list of elements to a list of values representing an ATOM entry.
 */
atom.entryElementsToValues = function(e) {
    var lt = filter(selector(mklist(element, "'title")), e);
    var t = isNil(lt)? '' : elementValue(car(lt));
    var li = filter(selector(mklist(element, "'id")), e);
    var i = isNil(li)? '' : elementValue(car(li));
    var lc = filter(selector(mklist(element, "'content")), e);
    return mklist(t, i, elementValue(car(lc)));
};

/**
 * Convert a list of elements to a list of values representing ATOM entries.
 */
atom.entriesElementsToValues = function(e) {
    if (isNil(e))
        return e;
    return cons(atom.entryElementsToValues(car(e)), atom.entriesElementsToValues(cdr(e)));
};

/**
 * Return true if a list of strings represents an ATOM entry.
 */
atom.isATOMEntry = function(l) {
    if (!isXML(l))
        return false;
    return car(l).match('<entry') != null && car(l).match('<feed') == null && car(l).match('="http://www.w3.org/2005/Atom"') != null;
};

/**
 * Convert a DOM Document to a list of values representing an ATOM entry.
 */
atom.readATOMEntryDocument = function(doc) {
    var e = readXMLDocument(doc);
    if (isNil(e))
        return mklist();
    return atom.entryElementsToValues(car(e));
};

/**
 * Convert a list of strings to a list of values representing an ATOM entry.
 */
atom.readATOMEntry = function(l) {
    return atom.readATOMEntryDocument(parseXML(l));
};

/**
 * Convert a list of values representy an ATOM entry to a value.
 */
atom.entryValue = function(e) {
    var v = elementsToValues(mklist(caddr(e)));
    return cons(car(e), (cadr(e), cdr(car(v))));
};

/**
 * Return true if a list of strings represents an ATOM feed.
 */
atom.isATOMFeed = function(l) {
    if (!isXML(l))
        return false;
    return car(l).match('<feed') != null && car(l).match('="http://www.w3.org/2005/Atom"') != null;
};

/**
 * Convert a DOM document to a list of values representing an ATOM feed.
 */
atom.readATOMFeedDocument = function(doc) {
    var f = readXMLDocument(doc);
    if (isNil(f))
        return mklist();
    var t = filter(selector(mklist(element, "'title")), car(f));
    var i = filter(selector(mklist(element, "'id")), car(f));
    var e = filter(selector(mklist(element, "'entry")), car(f));
    if (isNil(e))
        return mklist(elementValue(car(t)), elementValue(car(i)));
    return cons(elementValue(car(t)), cons(elementValue(car(i)), atom.entriesElementsToValues(e)));
};

/**
 * Convert a list of strings to a list of values representing an ATOM feed.
 */
atom.readATOMFeed = function(l) {
    return atom.readAtomFeedDocument(parseXML(l));
};

/**
 * Convert an ATOM feed containing elements to an ATOM feed containing values.
 */
atom.feedValues = function(e) {
    function feedValuesLoop(e) {
        if (isNil(e))
            return e;
        return cons(entryValue(car(e)), feedValuesLoop(cdr(e)));
    }

    return cons(car(e), cons(cadr(e), feedValuesLoop(cddr(e))));
};

/**
 * Convert a list of values representy an ATOM entry to a list of elements.
 */
atom.entryElement = function(l) {
    return mklist(element, "'entry", mklist(attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
            mklist(element, "'title", mklist(attribute, "'type", "text"), car(l)),
            mklist(element, "'id", cadr(l)),
            mklist(element, "'content", mklist(attribute, "'type", (isList(caddr(l))? "application/xml" : "text")), caddr(l)),
            mklist(element, "'link", mklist(attribute, "'href", cadr(l))));
};

/**
 * Convert a list of values representing ATOM entries to a list of elements.
 */
atom.entriesElements = function(l) {
    if (isNil(l))
        return l;
    return cons(atom.entryElement(car(l)), atom.entriesElements(cdr(l)));
};

/**
 * Convert a list of values representing an ATOM entry to an ATOM entry.
 */
atom.writeATOMEntry = function(l) {
    return writeXML(mklist(atom.entryElement(l)), true);
};

/**
 * Convert a list of values representing an ATOM feed to an ATOM feed.
 */
atom.writeATOMFeed = function(l) {
    var f = mklist(element, "'feed", mklist(attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
            mklist(element, "'title", mklist(attribute, "'type", "text"), car(l)),
            mklist(element, "'id", cadr(l)));
    if (isNil(cddr(l)))
        return writeXML(mklist(f), true);
    var fe = append(f, atom.entriesElements(cddr(l)));
    return writeXML(mklist(fe), true);
};

/**
 * Convert an ATOM entry containing a value to an ATOM entry containing an item element.
 */
atom.entryValuesToElements = function(v) {
    if (isList(caddr(v)))
        return cons(car(v), cons(cadr(v), valuesToElements(mklist(cons("'item", caddr(v))))));
    return cons(car(v), cons(cadr(v), valuesToElements(mklist(mklist("'item", caddr(v))))));
};

/**
 * Convert an ATOM feed containing values to an ATOM feed containing elements.
 */
atom.feedValuesToElements = function(v) {
    function feedValuesToElementsLoop(v) {
        if (isNil(v))
            return v;
        return cons(atom.entryValuesToElements(car(v)), feedValuesToElementsLoop(cdr(v)));
    }

    return cons(car(v), cons(cadr(v), feedValuesToElementsLoop(cddr(v))));
};

