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
 * SCDL parsing functions.
 */

/**
 * Returns a list of components in a composite.
 */
function components(l) {
    var cs = namedElementChildren("'composite", l);
    if (isNil(cs))
        return cs;
    return namedElementChildren("'component", car(cs));
}

/**
 * Returns the name of a component, service or reference.
 */
function name(l) {
    return namedAttributeValue("'name", l);
}

/**
 * Returns the implementation of a component.
 */
function implementation(l) {
    function filterImplementation(v) {
        return isElement(v) && cadr(v).match("implementation.") != null;
    }

    var n = filter(filterImplementation, l);
    if (isNil(n))
        return null;
    return car(n);
}

/**
 * Returns the type of an implementation.
 */
function implementationType(l) {
    return elementName(l).substring(1);
}

/**
 * Returns the URI of a service, reference or implementation.
 */
function uri(l) {
    return namedAttributeValue("'uri", l);
}

/**
 * Returns a list of services in a component.
 */
function services(l) {
    return namedElementChildren("'service", l);
}

/**
 * Returns a list of references in a component.
 */
function references(l) {
    return namedElementChildren("'reference", l);
}

/**
 * Returns a list of bindings in a service or reference.
 */
function bindings(l) {
    function filterBinding(v) {
        return isElement(v) && cadr(v).match("binding.") != null;
    }

    return filter(filterBinding, l);
}

/**
 * Returns the type of a binding.
 */
function bindingType(l) {
    return elementName(l).substring(1);
}

/**
 * Returns the target of a reference.
 */
function target(l) {
    function bindingsTarget(l) {
        if (isNil(l))
            return null;
        var u = uri(car(l));
        if (!isNil(u))
            return u;
        return bindingsTarget(cdr(l));
    }

    var t = namedAttributeValue("'target", l);
    if (!isNil(t))
        return t;
    return bindingsTarget(bindings(l));
}

/**
 * Returns a list of properties in a component.
 */
function properties(l) {
    return namedElementChildren("'property", l);
}

/**
 * Returns the value of a property.
 */
function propertyValue(l) {
    return elementValue(l);
}

