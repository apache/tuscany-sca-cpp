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

#ifndef tuscany_atom_hpp
#define tuscany_atom_hpp

/**
 * ATOM data conversion functions.
 */

#include <string>
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "xml.hpp"

namespace tuscany {
namespace atom {

/**
 * Convert a list of elements to a list of values representing an ATOM entry.
 */
const list<value> entryValues(const list<value>& e) {
    const list<value> lt = filter<value>(selector(mklist<value>(element, "title")), e);
    const value t = isNil(lt)? value(std::string("")) : elementValue(car(lt));
    const list<value> li = filter<value>(selector(mklist<value>(element, "id")), e);
    const value i = isNil(li)? value(std::string("")) : elementValue(car(li));
    const list<value> lc = filter<value>(selector(mklist<value>(element, "content")), e);
    return mklist<value>(t, i, cadr(elementChildren(car(lc))));
}

/**
 * Convert a list of elements to a list of values representing ATOM entries.
 */
const list<value> entriesValues(const list<value>& e) {
    if (isNil(e))
        return e;
    return cons<value>(entryValues(car(e)), entriesValues(cdr(e)));
}

/**
 * Convert a list of strings to a list of values representing an ATOM entry.
 */
const failable<list<value>, std::string> readEntry(const list<std::string>& ilist) {
    const list<value> e = readXML(ilist);
    if (isNil(e))
        return mkfailure<list<value>, std::string>("Empty entry");
    return entryValues(car(e));
}

/**
 * Convert a list of values representing an ATOM entry to a value.
 */
const value entryValue(const list<value>& e) {
    const list<value> v = elementsToValues(mklist<value>(caddr(e)));
    return cons(car(e), mklist<value>(cadr(e), cdr<value>(car(v))));
}

/**
 * Convert a list of strings to a list of values representing an ATOM feed.
 */
const failable<list<value>, std::string> readFeed(const list<std::string>& ilist) {
    const list<value> f = readXML(ilist);
    if (isNil(f))
        return mkfailure<list<value>, std::string>("Empty feed");
    const list<value> t = filter<value>(selector(mklist<value>(element, "title")), car(f));
    const list<value> i = filter<value>(selector(mklist<value>(element, "id")), car(f));
    const list<value> e = filter<value>(selector(mklist<value>(element, "entry")), car(f));
    if (isNil(e))
        return mklist<value>(elementValue(car(t)), elementValue(car(i)));
    return cons<value>(elementValue(car(t)), cons(elementValue(car(i)), entriesValues(e)));
}

/**
 * Convert a list of values representing an ATOM entry to a list of elements.
 * The first two values in the list are the entry title and id.
 */
const list<value> entryElement(const list<value>& l) {
    return list<value>()
        << element << "entry" << (list<value>() << attribute << "xmlns" << "http://www.w3.org/2005/Atom")
        << (list<value>() << element << "title" << (list<value>() << attribute << "type" << "text") << car(l))
        << (list<value>() << element << "id" << cadr(l))
        << (list<value>() << element << "content" << (list<value>() << attribute << "type" << "application/xml") << caddr(l))
        << (list<value>() << element << "link" << (list<value>() << attribute << "href" << cadr(l)));
}

/**
 * Convert a list of values representing ATOM entries to a list of elements.
 */
const list<value> entriesElements(const list<value>& l) {
    if (isNil(l))
        return l;
    return cons<value>(entryElement(car(l)), entriesElements(cdr(l)));
}

/**
 * Convert a list of values representing an ATOM entry to an ATOM entry.
 * The first two values in the list are the entry id and title.
 */
template<typename R> const failable<R, std::string> writeATOMEntry(const lambda<R(const std::string&, const R)>& reduce, const R& initial, const list<value>& l) {
    return writeXML<R>(reduce, initial, mklist<value>(entryElement(l)));
}

const failable<list<std::string>, std::string> writeATOMEntry(const list<value>& l) {
    const failable<list<std::string>, std::string> ls = writeATOMEntry<list<std::string> >(rcons<std::string>, list<std::string>(), l);
    if (!hasContent(ls))
        return ls;
    return reverse(list<std::string>(content(ls)));
}

/**
 * Convert a list of values representing an ATOM feed to an ATOM feed.
 * The first two values in the list are the feed id and title.
 */
template<typename R> const failable<R, std::string> writeATOMFeed(const lambda<R(const std::string&, const R)>& reduce, const R& initial, const list<value>& l) {
    const list<value> f = list<value>()
        << element << "feed" << (list<value>() << attribute << "xmlns" << "http://www.w3.org/2005/Atom")
        << (list<value>() << element << "title" << (list<value>() << attribute << "type" << "text") << car(l))
        << (list<value>() << element << "id" << cadr(l));
    if (isNil(cddr(l)))
        return writeXML<R>(reduce, initial, mklist<value>(f));
    const list<value> fe = append(f, entriesElements(cddr(l)));
    return writeXML<R>(reduce, initial, mklist<value>(fe));
}

/**
 * Convert a list of values representing an ATOM feed to a list of strings.
 * The first two values in the list are the feed id and title.
 */
const failable<list<std::string>, std::string> writeATOMFeed(const list<value>& l) {
    const failable<list<std::string>, std::string> ls = writeATOMFeed<list<std::string> >(rcons<std::string>, list<std::string>(), l);
    if (!hasContent(ls))
        return ls;
    return reverse(list<std::string>(content(ls)));
}

/**
 * Convert an ATOM entry containing a value to an ATOM entry containing an item element.
 */
const list<value> entryValuesToElements(const list<value> val) {
    return cons(car(val), cons(cadr(val), valuesToElements(mklist<value>(cons<value>("item", (list<value>)caddr(val))))));
}

/**
 * Convert an ATOM feed containing values to an ATOM feed containing elements.
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

#endif /* tuscany_atom_hpp */
