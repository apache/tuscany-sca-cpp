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

#ifndef tuscany_scdl_hpp
#define tuscany_scdl_hpp

/**
 * SCDL read functions.
 */

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../modules/xml/xml.hpp"

namespace tuscany {
namespace scdl {

/**
 * Returns a list of components in a composite.
 */
const list<value> components(const value& l) {
    const list<value> cs = elementChildren("composite", l);
    if (isNull(cs))
        return cs;
    return elementChildren("component", car(cs));
}

/**
 * Returns a list of service promotions in a composite.
 */
const list<value> promotions(const value& l) {
    const list<value> cs = elementChildren("composite", l);
    if (isNull(cs))
        return cs;
    return elementChildren("service", car(cs));
}

/**
 * Returns the target or a service promotion.
 */
const value promote(const value& l) {
    return attributeValue("promote", l);
}

/**
 * Returns the name of a component, service or reference.
 */
const value name(const value& l) {
    return attributeValue("name", l);
}

/**
 * Convert a list of elements to a name -> element assoc list.
 */
const list<value> nameToElementAssoc(const list<value>& l) {
    if (isNull(l))
        return l;
    const value e(car(l));
    return cons<value>(mklist<value>(name(e), e), nameToElementAssoc(cdr(l)));
}

/**
 * Returns the scdl declaration with the given name.
 */
const value named(const value& n, const value& l) {
    const vblambda filterName = [n](const value& v) -> const bool {
        return name(v) == n;
    };
    const list<value> c = filter<value>(filterName, l);
    if (isNull(c))
        return nilValue;
    return car(c);
}

/**
 * Returns the implementation of a component.
 */
const value implementation(const value& l) {
    const vblambda filterImplementation = [](const value& v) -> const bool {
        return isElement(v) && contains(string(cadr<value>(v)), "implementation.");
    };
    const list<value> n = filter<value>(filterImplementation, l);
    if (isNull(n))
        return nilValue;
    return car(n);
}

/**
 * Returns the URI of a service, reference or implementation.
 */
const value uri(const value& l) {
    return attributeValue("uri", l);
}

/**
 * Returns true if a reference is declared as wired by impl.
 */
const bool wiredByImpl(const value& l) {
    return attributeValue("wiredByImpl", l) == string("true");
}

/**
 * Returns a list of services in a component.
 */
const list<value> services(const value& l) {
    return elementChildren("service", l);
}

/**
 * Returns a list of references in a component.
 */
const list<value> references(const value& l) {
    return elementChildren("reference", l);
}

/**
 * Returns a list of bindings in a service or reference.
 */
const bool filterBinding(const value& v) {
    return isElement(v) && contains(string(cadr<value>(v)), "binding.");
}

const list<value> bindings(const value& l) {
    return filter<value>(filterBinding, l);
}

/**
 * Returns the target of a reference.
 */
const value bindingsTarget(const list<value>& l) {
    if (isNull(l))
        return nilValue;
    const value u = uri(car(l));
    if (!isNull(u))
        return u;
    return bindingsTarget(cdr(l));
}

const value target(const value& l) {
    const value target = attributeValue("target", l);
    if (!isNull(target))
        return target;
    return bindingsTarget(bindings(l));
}

/**
 * Convert a list of references to a reference name -> target assoc list.
 */
const list<value> referenceToTargetAssoc(const list<value>& r) {
    if (isNull(r))
        return r;
    const value ref(car(r));
    return cons<value>(mklist<value>(scdl::name(ref), scdl::target(ref)), referenceToTargetAssoc(cdr(r)));
}

/**
 * Returns a list of properties in a component.
 */
const list<value> properties(const value& l) {
    return elementChildren("property", l);
}

/**
 * Returns the type of an implementation.
 */
const value implementationType(const value& l) {
    return elementName(l);
}

/**
 * Returns the type of a binding.
 */
const value bindingType(const value& l) {
    return elementName(l);
}

/**
 * Returns the value of a property.
 */
const value propertyValue(const value& l) {
    return elementValue(l);
}

/**
 * Returns the absolute path of a resource in a contribution.
 */
const string resourcePath(const string& contrib, const string& path) {
    return c_str(path)[0] == '/'? path : contrib + path;
}

}
}

#endif /* tuscany_scdl_hpp */
