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

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "xml.hpp"

namespace tuscany {
namespace atom {

/**
 * Convert a list of elements to a list of values representing an ATOM entry.
 */
const list<value> entryElementsToValues(const list<value>& e) {
    const list<value> lt = filter<value>(selector(mklist<value>(element, "title")), e);
    const value t = isNil(lt)? value(emptyString) : elementValue(car(lt));
    const list<value> li = filter<value>(selector(mklist<value>(element, "id")), e);
    const value i = isNil(li)? value(emptyString) : elementValue(car(li));
    const list<value> lc = filter<value>(selector(mklist<value>(element, "content")), e);
    return mklist<value>(t, i, isNil(lc)? (value)list<value>() : elementValue(car(lc)));
}

/**
 * Convert a list of elements to a list of values representing ATOM entries.
 */
const list<value> entriesElementsToValues(const list<value>& e) {
    if (isNil(e))
        return e;
    return cons<value>(entryElementsToValues(car(e)), entriesElementsToValues(cdr(e)));
}

/**
 * Return true if a list of strings contains an ATOM feed.
 */
const bool isATOMFeed(const list<string>& ls) {
    if (!isXML(ls))
        return false;
    return contains(car(ls), "<feed") && contains(car(ls), "=\"http://www.w3.org/2005/Atom\"");
}

/**
 * Return true if a list of strings contains an ATOM entry.
 */
const bool isATOMEntry(const list<string>& ls) {
    if (!isXML(ls))
        return false;
    return contains(car(ls), "<entry") && !contains(car(ls), "<feed") && contains(car(ls), "=\"http://www.w3.org/2005/Atom\"");
}

/**
 * Convert a list of strings to a list of values representing an ATOM entry.
 */
const failable<list<value> > readATOMEntry(const list<string>& ilist) {
    const list<value> e = readXML(ilist);
    if (isNil(e))
        return mkfailure<list<value> >("Empty entry");
    return entryElementsToValues(car(e));
}

/**
 * Convert a list of values representing an ATOM entry to a value.
 */
const value entryValue(const list<value>& e) {
    const list<value> v = elementsToValues(mklist<value>(caddr(e)));
    return cons(car(e), mklist<value>(cadr(e), isList(car(v))? (isNil((list<value>)car(v))? car(v) : (value)cdr<value>(car(v))) : car(v)));
}

/**
 * Convert a list of strings to a list of values representing an ATOM feed.
 */
const failable<list<value> > readATOMFeed(const list<string>& ilist) {
    const list<value> f = readXML(ilist);
    if (isNil(f))
        return mkfailure<list<value> >("Empty feed");
    const list<value> t = filter<value>(selector(mklist<value>(element, "title")), car(f));
    const list<value> i = filter<value>(selector(mklist<value>(element, "id")), car(f));
    const list<value> e = filter<value>(selector(mklist<value>(element, "entry")), car(f));
    if (isNil(e))
        return mklist<value>(elementValue(car(t)), elementValue(car(i)));
    return cons<value>(elementValue(car(t)), cons(elementValue(car(i)), entriesElementsToValues(e)));
}

/**
 * Convert an ATOM feed containing elements to an ATOM feed containing values.
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
 * Convert a list of values representing an ATOM entry to a list of elements.
 * The first two values in the list are the entry title and id.
 */
const list<value> entryElement(const list<value>& l) {
    return list<value>()
        + element + "entry" + (list<value>() + attribute + "xmlns" + "http://www.w3.org/2005/Atom")
        + (list<value>() + element + "title" + (list<value>() + attribute + "type" + "text") + car(l))
        + (list<value>() + element + "id" + cadr(l))
        + (isNil(cddr(l))?
            list<value>() :
            list<value>() + element + "content" + (list<value>() + attribute + "type" + (isList(caddr(l))? "application/xml" : "text")) + caddr(l))
        + (list<value>() + element + "link" + (list<value>() + attribute + "href" + cadr(l)));
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
template<typename R> const failable<R> writeATOMEntry(const lambda<R(const string&, const R)>& reduce, const R& initial, const list<value>& l) {
    return writeXML<R>(reduce, initial, mklist<value>(entryElement(l)));
}

const failable<list<string> > writeATOMEntry(const list<value>& l) {
    const failable<list<string> > ls = writeATOMEntry<list<string> >(rcons<string>, list<string>(), l);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

/**
 * Convert a list of values representing an ATOM feed to an ATOM feed.
 * The first two values in the list are the feed id and title.
 */
template<typename R> const failable<R> writeATOMFeed(const lambda<R(const string&, const R)>& reduce, const R& initial, const list<value>& l) {
    const list<value> f = list<value>()
        + element + "feed" + (list<value>() + attribute + "xmlns" + "http://www.w3.org/2005/Atom")
        + (list<value>() + element + "title" + (list<value>() + attribute + "type" + "text") + car(l))
        + (list<value>() + element + "id" + cadr(l));
    if (isNil(cddr(l)))
        return writeXML<R>(reduce, initial, mklist<value>(f));
    const list<value> fe = append(f, entriesElements(cddr(l)));
    return writeXML<R>(reduce, initial, mklist<value>(fe));
}

/**
 * Convert a list of values representing an ATOM feed to a list of strings.
 * The first two values in the list are the feed id and title.
 */
const failable<list<string> > writeATOMFeed(const list<value>& l) {
    const failable<list<string> > ls = writeATOMFeed<list<string>>(rcons<string>, list<string>(), l);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

/**
 * Convert an ATOM entry containing a value to an ATOM entry containing an item element.
 */
const list<value> entryValuesToElements(const list<value> val) {
    if (isList(caddr(val)))
        return cons(car(val), cons(cadr(val), valuesToElements(mklist<value>(cons<value>("item", (list<value>)caddr(val))))));
    return cons(car(val), cons(cadr(val), valuesToElements(mklist<value>(mklist<value>("item", caddr(val))))));
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
