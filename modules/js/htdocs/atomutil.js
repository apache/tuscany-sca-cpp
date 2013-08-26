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
    var t = mklist(element, "'title", isNull(lt) || !elementHasValue(car(lt))? '' : elementValue(car(lt)));

    var li = filter(selector(mklist(element, "'id")), e);
    var i = mklist(element, "'id", isNull(li)? '' : elementValue(car(li)));

    var la = filter(selector(mklist(element, "'author")), e);
    var lan = isNull(la)? nil : filter(selector(mklist(element, "'name")), car(la));
    var lae = isNull(la)? nil : filter(selector(mklist(element, "'email")), car(la));
    var laa = isNull(lan)? lae : lan;
    var a = isNull(laa)? nil : mklist(mklist(element, "'author", elementValue(car(laa))));

    var lu = filter(selector(mklist(element, "'updated")), e);
    var u = isNull(lu)? nil : mklist(mklist(element, "'updated", elementValue(car(lu))));

    var lr = filter(selector(mklist(element, "'rank")), e);
    var r = isNull(lr)? nil : mklist(mklist(element, "'rank", elementValue(car(lr))));

    var lc = filter(selector(mklist(element, "'content")), e);
    var c = isNull(lc)? nil : isAttribute(elementValue(car(lc)))? nil : mklist(mklist(element, "'content", elementValue(car(lc))));

    return append(append(append(append(mklist(element, "'entry", t, i), a), u), r), c);
};

/**
 * Convert a list of elements to a list of values representing ATOM entries.
 */
atom.entriesElementValues = function(e) {
    if (isNull(e))
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
    if (isNull(e))
        return nil;
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
    if (isNull(f))
        return nil;
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
    var etitle = namedElementChild("'title", l);
    var title = elementHasValue(etitle)? elementValue(etitle) : '';
    var id = elementValue(namedElementChild("'id", l));
    var author = namedElementChild("'author", l);
    var email = isNull(author)? false : (elementValue(author).indexOf('@') != -1);
    var updated = namedElementChild("'updated", l);
    var rank = namedElementChild("'rank", l);
    var content = namedElementChild("'content", l);
    var text = isNull(content)? false : elementHasValue(content);
    return append(append(append(append(append(
            mklist(element, "'entry", mklist(attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
                mklist(element, "'title", mklist(attribute, "'type", "text"), title), mklist(element, "'id", id)),
                isNull(author)? nil : mklist(mklist(element, "'author",
                    (email? mklist(element, "'email", elementValue(author)) : mklist(element, "'name", elementValue(author)))))),
                isNull(updated)? nil : mklist(mklist(element, "'updated", elementValue(updated)))),
                isNull(rank)? nil : mklist(mklist(element, "'rank", elementValue(rank)))),
                isNull(content)? nil :
                    mklist(append(mklist(element, "'content", mklist(attribute, "'type", text? "text" : "application/xml")),
                        text? mklist(elementValue(content)) : elementChildren(content)))),
                mklist(mklist(element, "'link", mklist(attribute, "'href", id))));
};

/**
 * Convert a list of values representing ATOM entries to a list of elements.
 */
atom.entriesElements = function(l) {
    if (isNull(l))
        return l;
    return cons(atom.entryElement(car(l)), atom.entriesElements(cdr(l)));
};

/**
 * Convert a list of values representing an ATOM entry to an ATOM entry.
 */
atom.writeATOMEntry = function(ll) {
    var l = isNull(ll)? ll : car(ll);
    return writeXML(mklist(atom.entryElement(l)), true);
};

/**
 * Convert a list of values representing an ATOM feed to an ATOM feed.
 */
atom.writeATOMFeed = function(ll) {
    var l = isNull(ll)? ll : car(ll);
    var lt = filter(selector(mklist(element, "'title")), l);
    var t = isNull(lt)? '' : elementValue(car(lt));
    var li = filter(selector(mklist(element, "'id")), l);
    var i = isNull(li)? '' : elementValue(car(li));
    var f = mklist(element, "'feed", mklist(attribute, "'xmlns", "http://www.w3.org/2005/Atom"),
            mklist(element, "'title", mklist(attribute, "'type", "text"), car(l)),
            mklist(element, "'id", cadr(l)));

    // Write ATOM entries
    var le = filter(selector(mklist(element, "'entry")), l);
    if (isNull(le))
        return writeXML(mklist(f), true);

    // Write a single ATOM entry element with a list of values
    if (!isNull(le) && !isNull(car(le)) && isList(car(caddr(car(le))))) {
        var fe = append(f, atom.entriesElements(caddr(car(le))));
        return writeXML(mklist(fe), true);
    }

    // Write separate ATOM entry elements
    var fe = append(f, atom.entriesElements(le));
    return writeXML(mklist(fe), true);
};

/**
 * Uncomment to test.
 */
/*
(function testAtom() {
    console.log('Testing...');
    var entry = mklist("'entry", mklist("'title", 'test'), mklist("'id", 'test'), mklist("'content", mklist("'rating", '1')));
    var entryxml = car(atom.writeATOMEntry(valuesToElements(mklist(entry))));
    assert(entryxml == '<?xml version="1.0" encoding="UTF-8"?>\n' +
        '<entry xmlns="http://www.w3.org/2005/Atom"><title type="text">test</title><id>test</id><content type="application/xml"><rating>1</rating></content><link href="test"/></entry>\n');
    console.log('OK');
})();
*/

