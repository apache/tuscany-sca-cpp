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

#ifndef tuscany_element_hpp
#define tuscany_element_hpp

/**
 * Functions to help represent data as lists of elements and attributes.
 */

#include "list.hpp"
#include "value.hpp"

namespace tuscany
{

/**
 * Tags used to tag lists of elements and attributes.
 */
const value attribute("attribute");
const value element("element");
const string atsign("@");

/**
 * Returns true if a value is an element.
 */
inline const bool isElement(const value& v) {
    if (!isList(v) || isNil(v) || element != car<value>(v))
        return false;
    return true;
}

/**
 * Returns true if a value is an attribute.
 */
inline const bool isAttribute(const value& v) {
    if (!isList(v) || isNil(v) || attribute != car<value>(v))
        return false;
    return true;
}

/**
 * Returns the name of an attribute.
 */
inline const value attributeName(const list<value>& l) {
    return cadr(l);
}

/**
 * Returns the value of an attribute.
 */
inline const value attributeValue(const list<value>& l) {
    return caddr(l);
}

/**
 * Returns the name of an element.
 */
inline const value elementName(const list<value>& l) {
    return cadr(l);
}

/**
 * Returns true if an element has children.
 */
inline const bool elementHasChildren(const list<value>& l) {
    return !isNil(cddr(l));
}

/**
 * Returns the children of an element.
 */
inline const list<value> elementChildren(const list<value>& l) {
    return cddr(l);
}

/**
 * Returns true if an element has a value.
 */
inline const bool elementHasValue(const list<value>& l) {
    const list<value> r = reverse(l);
    if (isSymbol(car(r)))
        return false;
    if(isList(car(r)) && !isNil((list<value>)car(r)) && isSymbol(car<value>(car(r))))
        return false;
    return true;
}

/**
 * Returns the value of an element.
 */
inline const value elementValue(const list<value>& l) {
    return car(reverse(l));
}

/**
 * Convert an element to a value.
 */
inline const bool elementToValueIsList(const value& v) {
    if (!isList(v))
        return false;
    const list<value> l = v;
    return (isNil(l) || !isSymbol(car(l)));
}

inline const value elementToValue(const value& t) {
    const list<value> elementsToValues(const list<value>& e);

    // Convert an attribute
    if (isTaggedList(t, attribute))
        return mklist<value>(c_str(atsign + (string)attributeName(t)), attributeValue(t));

    // Convert an element
    if (isTaggedList(t, element)) {

        // Convert an element's value
        if (elementHasValue(t)) {

            // Convert a single value
            if (!elementToValueIsList(elementValue(t)))
                return mklist(elementName(t), elementValue(t));

            // Convert a list value
            return cons(elementName(t), mklist<value>(elementsToValues(elementValue(t))));
        }

        // Convert an element's children
        return cons(elementName(t), elementsToValues(elementChildren(t)));
    }

    // Convert a value
    if (!isList(t))
        return t;
    return elementsToValues(t);
}

/**
 * Convert a list of elements to a list of values.
 */
inline const bool elementToValueIsSymbol(const value& v) {
    if (!isList(v))
        return false;
    const list<value> l = v;
    if (isNil(l))
        return false;
    if (!isSymbol(car(l)))
        return false;
    return true;
}

inline const list<value> elementToValueGroupValues(const value& v, const list<value>& l) {
    if (isNil(l) || !elementToValueIsSymbol(v) || !elementToValueIsSymbol(car(l)))
        return cons(v, l);
    if (car<value>(car(l)) != car<value>(v))
        return cons(v, l);
    if (!elementToValueIsList(cadr<value>(car(l)))) {
        const value g = mklist<value>(car<value>(v), mklist<value>(isList(cadr<value>(v))? (value)cdr<value>(v) : cadr<value>(v), isList(cadr<value>(car(l)))? (value)cdr<value>(car(l)) : cadr<value>(car(l))));
        return elementToValueGroupValues(g, cdr(l));
    }
    const value g = mklist<value>(car<value>(v), cons<value>(isList(cadr<value>(v))? (value)cdr<value>(v) : cadr<value>(v), (list<value>)cadr<value>(car(l))));
    return elementToValueGroupValues(g, cdr(l));

}

inline const list<value> elementsToValues(const list<value>& e) {
    if (isNil(e))
        return e;
    return elementToValueGroupValues(elementToValue(car(e)), elementsToValues(cdr(e)));
}

/**
 * Convert a value to an element.
 */
inline const value valueToElement(const value& t) {
    const list<value> valuesToElements(const list<value>& l);

    // Convert a name value pair
    if (isList(t) && !isNil((list<value>)t) && isSymbol(car<value>(t))) {
        const value n = car<value>(t);
        const value v = isNil(cdr<value>(t))? nilValue : cadr<value>(t);

        // Convert a single value to an attribute or an element
        if (!isList(v)) {
            if (substr(n, 0, 1) == atsign)
                return mklist<value>(attribute, c_str(substr(n, 1)), v);
            return mklist(element, n, v);
        }

        // Convert a list value
        if (isNil((list<value>)v) || !isSymbol(car<value>(v)))
            return cons(element, cons(n, mklist<value>(valuesToElements(v))));

        // Convert a nested name value pair value
        return cons(element, cons(n, valuesToElements(cdr<value>(t))));
    }

    // Convert a value
    if (!isList(t))
        return t;
    return valuesToElements(t);
}

/**
 * Convert a list of values to a list of elements.
 */
inline const list<value> valuesToElements(const list<value>& l) {
    if (isNil(l))
        return l;
    return cons<value>(valueToElement(car(l)), valuesToElements(cdr(l)));
}

/**
 * Returns the value of the attribute with the given name.
 */
inline const value attributeValue(const value& name, const list<value>& l) {
    const list<value> f = filter<value>([name](const value& v) {
                return isAttribute(v) && attributeName((list<value>)v) == name;
           }, l);
    if (isNil(f))
        return nilValue;
    return caddr<value>(car(f));
}

/**
 * Returns child elements with the given name.
 */
inline const list<value> elementChildren(const value& name, const list<value>& l) {
    return filter<value>([name](const value& v) {
                return isElement(v) && elementName((list<value>)v) == name;
            }, l);
}

/**
 * Return the child element with the given name.
 */
inline const value elementChild(const value& name, const list<value>& l) {
    const list<value> f = elementChildren(name, l);
    if (isNil(f))
        return nilValue;
    return car(f);
}

}
#endif /* tuscany_element_hpp */
