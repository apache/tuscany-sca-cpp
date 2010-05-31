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
#include "xml.hpp"

namespace tuscany {
namespace rss {

/**
 * Convert a list of elements to a list of values representing an RSS entry.
 */
const list<value> entryElementsToValues(const list<value>& e) {
    const list<value> lt = filter<value>(selector(mklist<value>(element, "title")), e);
    const value t = isNil(lt)? value(emptyString) : elementValue(car(lt));
    const list<value> li = filter<value>(selector(mklist<value>(element, "link")), e);
    const value i = isNil(li)? value(emptyString) : elementValue(car(li));
    const list<value> ld = filter<value>(selector(mklist<value>(element, "description")), e);
    return mklist<value>(t, i, elementValue(car(ld)));
}

/**
 * Convert a list of elements to a list of values representing RSS entries.
 */
const list<value> entriesElementsToValues(const list<value>& e) {
    if (isNil(e))
        return e;
    return cons<value>(entryElementsToValues(car(e)), entriesElementsToValues(cdr(e)));
}

/**
 * Return true if a list of strings contains an RSS feed.
 */
const bool isRSSFeed(const list<string>& ls) {
    if (!isXML(ls))
        return false;
    return contains(car(ls), "<rss");
}

/**
 * Convert a list of strings to a list of values representing an RSS entry.
 */
const failable<list<value> > readRSSEntry(const list<string>& ilist) {
    const list<value> e = readXML(ilist);
    if (isNil(e))
        return mkfailure<list<value> >("Empty entry");
    return entryElementsToValues(car(e));
}

/**
 * Convert a list of values representing an RSS entry to a value.
 */
const value entryValue(const list<value>& e) {
    const list<value> v = elementsToValues(mklist<value>(caddr(e)));
    return cons(car(e), mklist<value>(cadr(e), isList(car(v))? (value)cdr<value>(car(v)) : car(v)));
}

/**
 * Convert a list of strings to a list of values representing an RSS feed.
 */
const failable<list<value> > readRSSFeed(const list<string>& ilist) {
    const list<value> f = readXML(ilist);
    if (isNil(f))
        return mkfailure<list<value> >("Empty feed");
    const list<value> c = filter<value>(selector(mklist<value>(element, "channel")), car(f));
    const list<value> t = filter<value>(selector(mklist<value>(element, "title")), car(c));
    const list<value> i = filter<value>(selector(mklist<value>(element, "link")), car(c));
    const list<value> e = filter<value>(selector(mklist<value>(element, "item")), car(c));
    if (isNil(e))
        return mklist<value>(elementValue(car(t)), elementValue(car(i)));
    return cons<value>(elementValue(car(t)), cons(elementValue(car(i)), entriesElementsToValues(e)));
}

/**
 * Convert an RSS feed containing elements to an RSS feed containing values.
 */
const list<value> feedValuesLoop(const list<value> e) {
    if (isNil(e))
        return e;
    return cons<value>(entryValue(car(e)), feedValuesLoop(cdr(e)));
}

const list<value> feedValues(const list<value>& e) {
    return cons(car<value>(e), cons<value>(cadr<value>(e), feedValuesLoop(cddr<value>(e))));
}

/**
 * Convert a list of values representing an RSS entry to a list of elements.
 * The first two values in the list are the entry title and id.
 */
const list<value> entryElement(const list<value>& l) {
    return list<value>()
        + element + "item"
        + (list<value>() + element + "title" + car(l))
        + (list<value>() + element + "link" + cadr(l))
        + (list<value>() + element + "description" + caddr(l));
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
template<typename R> const failable<R> writeRSSEntry(const lambda<R(const string&, const R)>& reduce, const R& initial, const list<value>& l) {
    return writeXML<R>(reduce, initial, mklist<value>(entryElement(l)));
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
template<typename R> const failable<R> writeRSSFeed(const lambda<R(const string&, const R)>& reduce, const R& initial, const list<value>& l) {
    const list<value> c = list<value>()
        + (list<value>() + element + "title" + car(l))
        + (list<value>() + element + "link" + cadr(l))
        + (list<value>() + element + "description" + car(l));
    const list<value> ce = isNil(cddr(l))? c : append(c, entriesElements(cddr(l)));
    const list<value> fe = list<value>()
        + element + "rss" + (list<value>() + attribute + "version" + "2.0")
        + append(list<value>() + element + "channel", ce);
    return writeXML<R>(reduce, initial, mklist<value>(fe));
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

/**
 * Convert an RSS entry containing a value to an RSS entry containing an item element.
 */
const list<value> entryValuesToElements(const list<value> val) {
    return cons(car(val), cons(cadr(val), valuesToElements(mklist<value>(cons<value>("item", (list<value>)caddr(val))))));
}

/**
 * Convert an RSS feed containing values to an RSS feed containing elements.
 */
const list<value> feedValuesToElementsLoop(const list<value> val) {
    if (isNil(val))
        return val;
    return cons<value>(entryValuesToElements(car(val)), feedValuesToElementsLoop(cdr(val)));
}

const list<value> feedValuesToElements(const list<value>& val) {
    return cons(car<value>(val), cons<value>(cadr<value>(val), feedValuesToElementsLoop(cddr<value>(val))));
}

}
}

#endif /* tuscany_rss_hpp */
