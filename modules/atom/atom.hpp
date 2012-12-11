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
#include "../xml/xml.hpp"

namespace tuscany {
namespace atom {

/**
 * Tags used to tag feed and entry elements.
 */
const value feed("feed");
const value entry("entry");

/**
 * Convert a list of elements to a list of element values representing an ATOM entry.
 */
const list<value> entryElementValues(const list<value>& e) {
    const list<value> lt = filter<value>(selector(mklist<value>(element, "title")), e);
    const list<value> t = nilListValue + element + value("title") + (isNil(lt)? value(emptyString) : elementValue(car(lt)));

    const list<value> li = filter<value>(selector(mklist<value>(element, "id")), e);
    const list<value> i = nilListValue + element + value("id") + (isNil(li)? value(emptyString) : elementValue(car(li)));

    const list<value> la = filter<value>(selector(mklist<value>(element, "author")), e);
    const list<value> lan = isNil(la)? nilListValue : filter<value>(selector(mklist<value>(element, "name")), car(la));
    const list<value> lae = isNil(la)? nilListValue : filter<value>(selector(mklist<value>(element, "email")), car(la));
    const list<value> laa = isNil(lan)? lae : lan;
    const list<value> a = isNil(laa)? nilListValue : mklist<value>(nilListValue + element + value("author") + elementValue(car(laa)));

    const list<value> lu = filter<value>(selector(mklist<value>(element, "updated")), e);
    const list<value> u = isNil(lu)? nilListValue : mklist<value>(nilListValue + element + value("updated") + elementValue(car(lu)));

    const list<value> lc = filter<value>(selector(mklist<value>(element, "content")), e);
    const list<value> c = isNil(lc)? nilListValue : mklist<value>(nilListValue + element + value("content") + elementValue(car(lc)));

    return append<value>(append<value>(append<value>(nilListValue + element + entry + value(t) + value(i), a), u), c);
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
 * Return true if a list of strings contains an ATOM feed.
 */
const bool isATOMFeed(const list<string>& ls) {
    if (!xml::isXML(ls))
        return false;
    return contains(car(ls), "<feed") && contains(car(ls), "=\"http://www.w3.org/2005/Atom\"");
}

/**
 * Return true if a list of strings contains an ATOM entry.
 */
const bool isATOMEntry(const list<string>& ls) {
    if (!xml::isXML(ls))
        return false;
    return contains(car(ls), "<entry") && !contains(car(ls), "<feed") && contains(car(ls), "=\"http://www.w3.org/2005/Atom\"");
}

/**
 * Convert a list of strings to a list of values representing an ATOM entry.
 */
const failable<list<value> > readATOMEntry(const list<string>& ilist) {
    const list<value> e = content(xml::readElements(ilist));
    if (isNil(e))
        return mkfailure<list<value> >("Empty entry");
    return mklist<value>(entryElementValues(car(e)));
}

/**
 * Convert a list of strings to a list of values representing an ATOM feed.
 */
const failable<list<value> > readATOMFeed(const list<string>& ilist) {
    const list<value> f = content(xml::readElements(ilist));
    if (isNil(f))
        return mkfailure<list<value> >("Empty feed");
    const list<value> t = filter<value>(selector(mklist<value>(element, "title")), car(f));
    const list<value> i = filter<value>(selector(mklist<value>(element, "id")), car(f));
    const list<value> e = filter<value>(selector(mklist<value>(element, entry)), car(f));
    return mklist<value>(append<value>(nilListValue + element + feed 
                + value(nilListValue + element + value("title") + elementValue(car(t)))
                + value(nilListValue + element + value("id") + elementValue(car(i))),
                entriesElementValues(e)));
}

/**
 * Returns children of an ATOM content element.
 */
const list<value> contentElementChildren(const value& content) {
    return filter<value>([](const value& v) {
                return !(isAttribute(v) && attributeName((list<value>)v) == "type");
            }, elementChildren(content));
}

/**
 * Convert a list of element values representing an ATOM entry to a list of elements.
 */
const list<value> entryElement(const list<value>& l) {
    const value title = elementChild("title", l);
    const value id = elementChild("id", l);
    const value author = elementChild("author", l);
    const bool email = isNil(author)? false : contains(elementValue(author), "@");
    const value updated = elementChild("updated", l);
    const value content = elementChild("content", l);
    const bool text = isNil(content)? false : elementHasValue(content);
    return nilListValue
        + element + entry + (nilListValue + attribute + "xmlns" + "http://www.w3.org/2005/Atom")
        + (nilListValue + element + "title" + (nilListValue + attribute + "type" + "text") + elementValue(title))
        + (nilListValue + element + "id" + elementValue(id))
        + (isNil(author)? nilListValue : (nilListValue + element + "author" +
            (email? (nilListValue + element + "email" + elementValue(author)) : (nilListValue + element + "name" + elementValue(author)))))
        + (isNil(updated)? nilListValue : (nilListValue + element + "updated" + elementValue(updated)))
        + (isNil(content)?
            nilListValue :
            append<value>(nilListValue + element + "content" + (nilListValue + attribute + "type" + (text? "text" : "application/xml")),
                text? mklist<value>(elementValue(content)) : contentElementChildren(content)))
        + (nilListValue + element + "link" + (nilListValue + attribute + "href" + elementValue(id)));
}

/**
 * Convert a list of element values representing ATOM entries to a list of elements.
 */
const list<value> entriesElements(const list<value>& l) {
    if (isNil(l))
        return l;
    return cons<value>(entryElement(car(l)), entriesElements(cdr(l)));
}

/**
 * Convert a list of element values representing an ATOM entry to an ATOM entry.
 */
template<typename R> const failable<R> writeATOMEntry(const lambda<const R(const string&, const R)>& reduce, const R& initial, const list<value>& ll) {
    const list<value> l = isNil(ll)? ll : (list<value>)car(ll);
    return xml::writeElements<R>(reduce, initial, mklist<value>(entryElement(l)));
}

const failable<list<string> > writeATOMEntry(const list<value>& l) {
    const failable<list<string> > ls = writeATOMEntry<list<string> >(rcons<string>, list<string>(), l);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

/**
 * Convert a list of element values representing an ATOM feed to an ATOM feed.
 */
template<typename R> const failable<R> writeATOMFeed(const lambda<const R(const string&, const R)>& reduce, const R& initial, const list<value>& ll) {
    const list<value> l = isNil(ll)? ll : (list<value>)car(ll);
    const list<value> lt = filter<value>(selector(mklist<value>(element, "title")), l);
    const value t = isNil(lt)? value(emptyString) : elementValue(car(lt));
    const list<value> li = filter<value>(selector(mklist<value>(element, "id")), l);
    const value i = isNil(li)? value(emptyString) : elementValue(car(li));
    const list<value> f = nilListValue
        + element + feed + (nilListValue + attribute + "xmlns" + "http://www.w3.org/2005/Atom")
        + (nilListValue + element + "title" + (nilListValue + attribute + "type" + "text") + t)
        + (nilListValue + element + "id" + i);

    // Write ATOM entries
    const list<value> le = filter<value>(selector(mklist<value>(element, entry)), l);
    if (isNil(le))
        return xml::writeElements<R>(reduce, initial, mklist<value>(f));

    // Write a single ATOM entry element with a list of values
    if (!isNil(le) && !isNil(car(le)) && isList(car<value>(caddr<value>(car(le))))) {
        const list<value> fe = append(f, entriesElements(caddr<value>(car(le))));
        return xml::writeElements<R>(reduce, initial, mklist<value>(fe));
    }

    // Write separate ATOM entry elements
    const list<value> fe = append(f, entriesElements(le));
    return xml::writeElements<R>(reduce, initial, mklist<value>(fe));
}

/**
 * Convert a list of element values representing an ATOM feed to a list of strings.
 */
const failable<list<string> > writeATOMFeed(const list<value>& l) {
    const failable<list<string> > ls = writeATOMFeed<list<string>>(rcons<string>, list<string>(), l);
    if (!hasContent(ls))
        return ls;
    return reverse(list<string>(content(ls)));
}

}
}

#endif /* tuscany_atom_hpp */
