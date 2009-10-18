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

/**
 * Returns the name of an attribute.
 */
const value attributeName(const list<value>& l) {
    return cadr(l);
}

/**
 * Returns the value of an attribute.
 */
const value attributeValue(const list<value>& l) {
    return caddr(l);
}

/**
 * Returns the name of an element.
 */
const value elementName(const list<value>& l) {
    return cadr(l);
}

/**
 * Returns the children of an element.
 */
const list<value> elementChildren(const list<value>& l) {
    return cddr(l);
}

/**
 * Returns true if an element has a value.
 */
const value elementHasValue(const list<value>& l) {
    if (isNil(cddr(l)))
        return false;
    const list<value> r = reverse(l);
    if(isList(car(r))) {
        const list<value> v = car(r);
        if(isSymbol(car(v))) {
            return false;
        }
    }
    return true;
}

/**
 * Returns the value of an element.
 */
const value elementValue(const list<value>& l) {
    return car(reverse(l));
}

/**
 * Converts a list of elements to a list of values.
 */
const list<value> elementsToValues(const list<value>& e) {
    if (isNil(e))
        return list<value>();
    const value t = car(e);
    if (isTaggedList(t, attribute))
        return cons<value>(mklist(attributeName(t), attributeValue(t)), elementsToValues(cdr(e)));
    if (isTaggedList(t, element)) {
        if (elementHasValue(t))
            return cons<value>(mklist(elementName(t), elementValue(t)), elementsToValues(cdr(e)));
        return cons<value>(cons(elementName(t), elementsToValues(elementChildren(t))), elementsToValues(cdr(e)));
    }
    return cons(t, elementsToValues(cdr(e)));
}

/**
 * Converts a list of values to a list of elements.
 */
const list<value> valuesToElements(const list<value>& e) {
    if (isNil(e))
        return list<value>();
    const value t = car(e);
    if (isList(t)) {
        const value v = cadr<value>(t);
        if (!isList(v))
            return cons<value>(mklist(attribute, car<value>(t), v), valuesToElements(cdr(e)));
        return cons<value>(cons(element, cons(car<value>(t), valuesToElements(cdr<value>(t)))), valuesToElements(cdr(e)));
    }
    return cons(t, valuesToElements(cdr(e)));
}

/**
 * Returns a selector lambda function which can be used to filter
 * elements against the given element pattern.
 */
struct selectorLambda {
    const list<value> select;
    selectorLambda(const list<value>& s) : select(s) {
    }
    const bool eval(const list<value>& s, const list<value> v) const {
        if (isNil(s))
            return true;
        if (isNil(v))
            return false;
        if (car(s) != car(v))
            return false;
        return eval(cdr(s), cdr(v));
    }
    const bool operator()(const value& v) const {
        if (!isList(v))
            return false;
        return eval(select, v);
    }
};

const lambda<bool(value)> selector(const list<value> s) {
    return selectorLambda(s);
}

}
#endif /* tuscany_element_hpp */
