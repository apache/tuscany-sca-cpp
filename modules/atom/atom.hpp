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

/**
 * Convert a list of elements to a list of values representing an ATOM entry.
 */
const list<value> atomEntry(const list<value>& e) {
    const list<value> t = filter<value>(selector(mklist<value>(element, "title")), e);
    const list<value> i = filter<value>(selector(mklist<value>(element, "id")), e);
    const list<value> c = filter<value>(selector(mklist<value>(element, "content")), e);
    return mklist<value>(elementValue(car(t)), elementValue(car(i)), cadr(elementChildren(car(c))));
}

/**
 * Convert a list of elements to a list of values representing ATOM entries.
 */
const list<value> atomEntries(const list<value>& e) {
    if (isNil(e))
        return list<value>();
    return cons<value>(atomEntry(car(e)), atomEntries(cdr(e)));
}

/**
 * Convert a list of strings to a list of values representing an ATOM entry.
 */
const failable<list<value>, std::string> readATOMEntry(const list<std::string>& ilist) {
    const list<value> e = readXML(ilist);
    if (isNil(e))
        return std::string("Empty entry");
    return atomEntry(car(e));
}

/**
 * Convert a list of strings to a list of values representing an ATOM feed.
 */
const failable<list<value>, std::string> readATOMFeed(const list<std::string>& ilist) {
    const list<value> f = readXML(ilist);
    if (isNil(f))
        return std::string("Empty feed");
    const list<value> t = filter<value>(selector(mklist<value>(element, "title")), car(f));
    const list<value> i = filter<value>(selector(mklist<value>(element, "id")), car(f));
    const list<value> e = filter<value>(selector(mklist<value>(element, "entry")), car(f));
    if (isNil(e))
        return mklist<value>(elementValue(car(t)), elementValue(car(i)));
    return cons<value>(elementValue(car(t)), cons(elementValue(car(i)), atomEntries(e)));
}

/**
 * Convert a list of values representing an ATOM entry to a list of elements.
 * The first two values in the list are the entry title and id.
 */
const list<value> atomEntryElement(const list<value>& l) {
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
const list<value> atomEntriesElements(const list<value>& l) {
    if (isNil(l))
        return list<value>();
    return cons<value>(atomEntryElement(car(l)), atomEntriesElements(cdr(l)));
}

/**
 * Convert a list of values representing an ATOM entry to an ATOM entry.
 * The first two values in the list are the entry id and title.
 */
template<typename R> const failable<R, std::string> writeATOMEntry(const lambda<R(R, std::string)>& reduce, const R& initial, const list<value>& l) {
    return writeXML<R>(reduce, initial, mklist<value>(atomEntryElement(l)));
}

/**
 * Convert a list of values representing an ATOM entry to a list of strings.
 * The first two values in the list are the entry id and title.
 */
const list<std::string> writeATOMList(const list<std::string>& listSoFar, const std::string& s) {
    return cons(s, listSoFar);
}

const failable<list<std::string>, std::string> writeATOMEntry(const list<value>& l) {
    const failable<list<std::string>, std::string> ls = writeATOMEntry<list<std::string> >(writeATOMList, list<std::string>(), l);
    if (!hasValue(ls))
        return ls;
    return reverse(list<std::string>(ls));
}

/**
 * Convert a list of values representing an ATOM feed to an ATOM feed.
 * The first two values in the list are the feed id and title.
 */
template<typename R> const failable<R, std::string> writeATOMFeed(const lambda<R(R, std::string)>& reduce, const R& initial, const list<value>& l) {
    const list<value> f = list<value>()
        << element << "feed" << (list<value>() << attribute << "xmlns" << "http://www.w3.org/2005/Atom")
        << (list<value>() << element << "title" << (list<value>() << attribute << "type" << "text") << car(l))
        << (list<value>() << element << "id" << cadr(l));
    if (isNil(cddr(l)))
        return writeXML<R>(reduce, initial, mklist<value>(f));
    const list<value> fe = append(f, atomEntriesElements(cddr(l)));
    return writeXML<R>(reduce, initial, mklist<value>(fe));
}

/**
 * Convert a list of values representing an ATOM feed to a list of strings.
 * The first two values in the list are the feed id and title.
 */
const failable<list<std::string>, std::string> writeATOMFeed(const list<value>& l) {
    const failable<list<std::string>, std::string> ls = writeATOMFeed<list<std::string> >(writeATOMList, list<std::string>(), l);
    if (!hasValue(ls))
        return ls;
    return reverse(list<std::string>(ls));
}

}

#endif /* tuscany_atom_hpp */
