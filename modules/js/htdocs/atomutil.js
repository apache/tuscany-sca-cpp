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

/**
 * Convert a list of elements to a list of values representing an ATOM entry.
 */
function entryElementsToValues(e) {
    var lt = filter(selector(mklist(element, "'title")), e);
    var t = isNil(lt)? '' : elementValue(car(lt));
    var li = filter(selector(mklist(element, "'id")), e);
    var i = isNil(li)? '' : elementValue(car(li));
    var lc = filter(selector(mklist(element, "'content")), e);
    return mklist(t, i, elementValue(car(lc)));
}

/**
 * Convert a list of elements to a list of values representing ATOM entries.
 */
function entriesElementsToValues(e) {
    if (isNil(e))
        return e;
    return cons(entryElementsToValues(car(e)), entriesElementsToValues(cdr(e)));
}

/**
 * Convert a list of strings to a list of values representing an ATOM entry.
 */
function readATOMEntry(l) {
    var e = readXML(l);
    if (isNil(e))
        return mklist();
    return entryElementsToValues(car(e));
}

/**
 * Convert a list of values representy an ATOM entry to a value.
 */
function entryValue(e) {
    var v = elementsToValues(mklist(caddr(e)));
    return cons(car(e), (cadr(e), cdr(car(v))));
}

/**
 * Return true if a list of strings represents an ATOM feed.
 */
function isATOMFeed(l) {
    if (isNil(l))
        return false;
    if (car(l).substring(0, 5) != "<?xml")
        return false;
    return car(l).match("<feed") != null;
}

/**
 * Convert a DOM document to a list of values representing an ATOM feed.
 */
function readATOMFeedDocument(doc) {
    var f = readXMLDocument(doc);
    if (isNil(f))
        return mklist();
    var t = filter(selector(mklist(element, "'title")), car(f));
    var i = filter(selector(mklist(element, "'id")), car(f));
    var e = filter(selector(mklist(element, "'entry")), car(f));
    if (isNil(e))
        return mklist(elementValue(car(t)), elementValue(car(i)));
    return cons(elementValue(car(t)), cons(elementValue(car(i)), entriesElementsToValues(e)));
}

/**
 * Convert a list of strings to a list of values representing an ATOM feed.
 */
function readATOMFeed(l) {
    return readAtomFeedDocument(parseXML(l));
}

/**
 * Convert an ATOM feed containing elements to an ATOM feed containing values.
 */
function feedValuesLoop(e) {
    if (isNil(e))
        return e;
    return cons(entryValue(car(e)), feedValuesLoop(cdr(e)));
}

function feedValues(e) {
    return cons(car(e), cons(cadr(e), feedValuesLoop(cddr(e))));
}

/**
 * Convert a list of values representy an ATOM entry to a list of elements.
 */
function entryElement(l) {
    return mklist(element, "'entry", mklist(attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
            mklist(element, "'title", mklist(attribute, "'type", "text"), car(l)),
            mklist(element, "'id", cadr(l)),
            mklist(element, "'content", mklist(attribute, "'type", (isList(caddr(l))? "application/xml" : "text")), caddr(l)),
            mklist(element, "'link", mklist(attribute, "'href", cadr(l))));
}

/**
 * Convert a list of values representing ATOM entries to a list of elements.
 */
function entriesElements(l) {
    if (isNil(l))
        return l;
    return cons(entryElement(car(l)), entriesElements(cdr(l)));
}

/**
 * Convert a list of values representing an ATOM entry to an ATOM entry.
 */
function writeATOMEntry(l) {
    return writeXML(mklist(entryElement(l)), true);
}

/**
 * Convert a list of values representing an ATOM feed to an ATOM feed.
 */
function writeATOMFeed(l) {
    var f = mklist(element, "'feed", mklist(attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
            mklist(element, "'title", mklist(attribute, "'type", "text"), car(l)),
            mklist(element, "'id", cadr(l)));
    if (isNil(cddr(l)))
        return writeXML(mklist(f), true);
    var fe = append(f, entriesElements(cddr(l)));
    return writeXML(mklist(fe), true);
}

/**
 * Convert an ATOM entry containing a value to an ATOM entry containing an item element.
 */
function entryValuesToElements(v) {
    return cons(car(v), cons(cadr(v), valuesToElements(mklist(cons("'item", caddr(v))))));
}

/**
 * Convert an ATOM feed containing values to an ATOM feed containing elements.
 */
function feedValuesToElementsLoop(v) {
    if (isNil(v))
        return v;
    return cons(entryValuesToElements(car(v)), feedValuesToElementsLoop(cdr(v)));
}

function feedValuesToElements(v) {
    return cons(car(v), cons(cadr(v), feedValuesToElementsLoop(cddr(v))));
}

