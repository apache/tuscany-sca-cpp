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
var atom = {};

/**
 * Convert a list of elements to a list of values representing an ATOM entry.
 */
atom.entryElementValues = function(e) {
    var lt = filter(selector(mklist(element, "'title")), e);
    var t = mklist(element, "'title", isNil(lt)? '' : elementValue(car(lt)));

    var li = filter(selector(mklist(element, "'id")), e);
    var i = mklist(element, "'id", isNil(li)? '' : elementValue(car(li)));

    var la = filter(selector(mklist(element, "'author")), e);
    var lan = isNil(la)? mklist() : filter(selector(mklist(element, "'name")), car(la));
    var lae = isNil(la)? mklist() : filter(selector(mklist(element, "'email")), car(la));
    var laa = isNil(lan)? lae : lan;
    var a = isNil(laa)? mklist() : mklist(mklist(element, "'author", elementValue(car(laa))));

    var lu = filter(selector(mklist(element, "'updated")), e);
    var u = isNil(lu)? mklist() : mklist(mklist(element, "'updated", elementValue(car(lu))));

    var lc = filter(selector(mklist(element, "'content")), e);
    var c = isNil(lc)? mklist() : isAttribute(elementValue(car(lc)))? mklist() : mklist(mklist(element, "'content", elementValue(car(lc))));

    return append(append(append(mklist(element, "'entry", t, i), a), u), c);
};

/**
 * Convert a list of elements to a list of values representing ATOM entries.
 */
atom.entriesElementValues = function(e) {
    if (isNil(e))
        return e;
    return cons(atom.entryElementValues(car(e)), atom.entriesElementValues(cdr(e)));
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
    return mklist(atom.entryElementValues(car(e)));
};

/**
 * Convert a list of strings to a list of values representing an ATOM entry.
 */
atom.readATOMEntry = function(l) {
    return atom.readATOMEntryDocument(parseXML(l));
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
    return mklist(append(
                mklist(element, "'feed", mklist(element, "'title", elementValue(car(t))), mklist(element, "'id", elementValue(car(i)))),
                atom.entriesElementValues(e)));
};

/**
 * Convert a list of strings to a list of values representing an ATOM feed.
 */
atom.readATOMFeed = function(l) {
    return atom.readATOMFeedDocument(parseXML(l));
};

/**
 * Convert a list of values representy an ATOM entry to a list of elements.
 */
atom.entryElement = function(l) {
    var title = elementValue(namedElementChild("'title", l));
    var id = elementValue(namedElementChild("'id", l));
    var author = namedElementChild("'author", l);
    var email = isNil(author)? false : (elementValue(author).indexOf('@') != -1);
    var updated = namedElementChild("'updated", l);
    var content = namedElementChild("'content", l);
    var text = isNil(content)? false : elementHasValue(content);
    return append(append(append(append(
            mklist(element, "'entry", mklist(attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
                mklist(element, "'title", mklist(attribute, "'type", "text"), title), mklist(element, "'id", id)),
                isNil(author)? mklist() : mklist(mklist(element, "'author",
                    (email? mklist(element, "'email", elementValue(author)) : mklist(element, "'name", elementValue(author)))))),
                isNil(updated)? mklist() : mklist(mklist(element, "'updated", elementValue(updated)))),
                isNil(content)? mklist() :
                    mklist(append(mklist(element, "'content", mklist(attribute, "'type", text? "text" : "application/xml")),
                        text? mklist(elementValue(content)) : elementChildren(content)))),
                mklist(mklist(element, "'link", mklist(attribute, "'href", id))));
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
atom.writeATOMEntry = function(ll) {
    var l = isNil(ll)? ll : car(ll);
    return writeXML(mklist(atom.entryElement(l)), true);
};

/**
 * Convert a list of values representing an ATOM feed to an ATOM feed.
 */
atom.writeATOMFeed = function(ll) {
    var l = isNil(ll)? ll : car(ll);
    var lt = filter(selector(mklist(element, "'title")), l);
    var t = isNil(lt)? '' : elementValue(car(lt));
    var li = filter(selector(mklist(element, "'id")), l);
    var i = isNil(li)? '' : elementValue(car(li));
    var f = mklist(element, "'feed", mklist(attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
            mklist(element, "'title", mklist(attribute, "'type", "text"), car(l)),
            mklist(element, "'id", cadr(l)));

    // Write ATOM entries
    var le = filter(selector(mklist(element, "'entry")), l);
    if (isNil(le))
        return writeXML(mklist(f), true);

    // Write a single ATOM entry element with a list of values
    if (!isNil(le) && !isNil(car(le)) && isList(car(caddr(car(le))))) {
        var fe = append(f, atom.entriesElements(caddr(car(le))));
        return writeXML(mklist(fe), true);
    }

    // Write separate ATOM entry elements
    var fe = append(f, atom.entriesElements(le));
    return writeXML(mklist(fe), true);
};

