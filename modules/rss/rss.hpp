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

/* $Rev$ $Date$ */

#ifndef tuscany_rss_hpp
#define tuscany_rss_hpp

/**
 * RSS data conversion functions.
 */

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../xml/xml.hpp"

namespace tuscany {
namespace rss {

/**
 * Tags used to tag feed and entry elements.
 */
const value feed("feed");
const value entry("entry");

/**
 * Convert a list of elements to a list of element values representing an RSS entry.
 */
const list<value> entryElementValues(const list<value>& e) {
    const list<value> lt = elementChildren("title", e);
    const value t = isNil(lt)? value(emptyString) : elementValue(car(lt));
    const list<value> li = elementChildren("link", e);
    const value i = isNil(li)? value(emptyString) : elementValue(car(li));
    const list<value> ld = elementChildren("description", e);
    return append<value>(nilListValue + element + entry 
                + value(nilListValue + element + value("title") + t)
                + value(nilListValue + element + value("id") + i),
                isNil(ld)? nilListValue : isAttribute(elementValue(car(ld)))? nilListValue :
                    mklist<value>(value(nilListValue + element + value("content") + elementValue(car(ld)))));
}

/**
 * Convert a list of elements to a list of element values representing ATOM entries.
 */
const list<value> entriesElementValues(const list<value>& e) {
    if (isNil(e))
        return e;
    return cons<value>(entryElementValues(car(e)), entriesElementValues(cdr(e)));
}

/**
 * Return true if a list of strings contains an RSS feed.
 */
const bool isRSSFeed(const list<string>& ls) {
    if (!xml::isXML(ls))
        return false;
    return contains(car(ls), "<rss");
}

/**
 * Convert a list of strings to a list of values representing an RSS entry.
 */
const failable<list<value> > readRSSEntry(const list<string>& ilist) {
    const list<value> e = content(xml::readElements(ilist));
    if (isNil(e))
        return mkfailure<list<value> >("Empty entry");
    return mklist<value>(entryElementValues(car(e)));
}

/**
 * Convert a list of strings to a list of values representing an RSS feed.
 */
const failable<list<value> > readRSSFeed(const list<string>& ilist) {
    const list<value> f = content(xml::readElements(ilist));
    if (isNil(f))
        return mkfailure<list<value> >("Empty feed");
    const list<value> c = elementChildren("channel", car(f));
    const list<value> t = elementChildren("title", car(c));
    const list<value> i = elementChildren("link", car(c));
    const list<value> e = elementChildren("item", car(c));
    return mklist<value>(append<value>(nilListValue + element + feed 
                + value(nilListValue + element + value("title") + elementValue(car(t)))
                + value(nilListValue + element + value("id") + elementValue(car(i))),
                entriesElementValues(e)));
}

/**
 * Convert a list of element values representing an RSS entry to a list of elements.
 */
const list<value> entryElement(const list<value>& l) {
    const value title = elementValue(elementChild("title", l));
    const value id = elementValue(elementChild("id", l));
    const value content = elementChild("content", l);
    const bool text = isNil(content)? false : elementHasValue(content);
    return append<value>(nilListValue
        + element + "item"
        + (nilListValue + element + "title" + title)
        + (nilListValue + element + "link" + id),
        isNil(content)?
            nilListValue :
            mklist<value>(append<value>(nilListValue + element + "description",
                text? mklist<value>(elementValue(content)) : elementChildren(content))));
}

/**
 * Convert a list of values representing RSS entries to a list of elements.
 */
const list<value> entriesElements(const list<value>& l) {
    if (isNil(l))
        return l;
    return cons<value>(entryElement(car(l)), entriesElements(cdr(l)));
}

/**
 * Convert a list of values representing an RSS entry to an RSS entry.
 * The first two values in the list are the entry id and title.
 */
template<typename R> const failable<R> writeRSSEntry(const lambda<const R(const string&, const R)>& reduce, const R& initial, const list<value>& ll) {
    const list<value> l = isNil(ll)? ll : (list<value>)car(ll);
    return xml::writeElements<R>(reduce, initial, mklist<value>(entryElement(l)));
}

const failable<list<string> > writeRSSEntry(const list<value>& l) {
    const failable<list<string> > ls = writeRSSEntry<list<string> >(rcons<string>, list<string>(), l);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

/**
 * Convert a list of values representing an RSS feed to an RSS feed.
 * The first two values in the list are the feed id and title.
 */
template<typename R> const failable<R> writeRSSFeed(const lambda<const R(const string&, const R)>& reduce, const R& initial, const list<value>& ll) {
    const list<value> l = isNil(ll)? ll : (list<value>)car(ll);
    const list<value> lt = elementChildren("title", l);
    const value t = isNil(lt)? value(emptyString) : elementValue(car(lt));
    const list<value> li = elementChildren("id", l);
    const value i = isNil(li)? value(emptyString) : elementValue(car(li));
    const list<value> c = nilListValue
        + (nilListValue + element + "title" + t)
        + (nilListValue + element + "link" + i)
        + (nilListValue + element + "description" + t);

    // Write RSS entries
    const list<value> le = elementChildren(entry, l);
    if (isNil(le)) {
        const list<value> fe = nilListValue
            + element + "rss" + (nilListValue + attribute + "version" + "2.0")
            + append(nilListValue + element + "channel", c);
        return xml::writeElements<R>(reduce, initial, mklist<value>(fe));
    }

    // Write a single RSS entry element with a list of values
    if (!isNil(le) && !isNil(car(le)) && isList(car<value>(caddr<value>(car(le))))) {
        const list<value> ce = append(c, entriesElements(caddr<value>(car(le))));
        const list<value> fe = nilListValue
            + element + "rss" + (nilListValue + attribute + "version" + "2.0")
            + append(nilListValue + element + "channel", ce);
        return xml::writeElements<R>(reduce, initial, mklist<value>(fe));
    }

    // Write separate RSS entry elements
    const list<value> ce = append(c, entriesElements(le));
    const list<value> fe = nilListValue
        + element + "rss" + (nilListValue + attribute + "version" + "2.0")
        + append(nilListValue + element + "channel", ce);
    return xml::writeElements<R>(reduce, initial, mklist<value>(fe));
}

/**
 * Convert a list of values representing an RSS feed to a list of strings.
 * The first two values in the list are the feed id and title.
 */
const failable<list<string> > writeRSSFeed(const list<value>& l) {
    const failable<list<string> > ls = writeRSSFeed<list<string>>(rcons<string>, list<string>(), l);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

}
}

#endif /* tuscany_rss_hpp */
