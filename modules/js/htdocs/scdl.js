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
var scdl = new Object();

/**
 * Returns a list of components in a composite.
 */
scdl.components = function(l) {
    var cs = namedElementChildren("'composite", l);
    if (isNil(cs))
        return cs;
    return namedElementChildren("'component", car(cs));
};

/**
 * Returns the name of a component, service or reference.
 */
scdl.name = function(l) {
    return namedAttributeValue("'name", l);
};

/**
 * Returns the color of a component.
 */
scdl.color = function(l) {
    return namedAttributeValue("'color", l);
};

/**
 * Returns the implementation of a component.
 */
scdl.implementation = function(l) {
    function filterImplementation(v) {
        return isElement(v) && cadr(v).match("implementation.") != null;
    }

    var n = filter(filterImplementation, l);
    if (isNil(n))
        return null;
    return car(n);
};

/**
 * Returns the type of an implementation.
 */
scdl.implementationType = function(l) {
    return elementName(l).substring(1);
};

/**
 * Returns the URI of a service, reference or implementation.
 */
scdl.uri = function(l) {
    return namedAttributeValue("'uri", l);
};

/**
 * Returns the align attribute of a service or reference.
 */
scdl.align = function(l) {
    return namedAttributeValue("'align", l);
};

/**
 * Returns a list of services in a component.
 */
scdl.services = function(l) {
    return namedElementChildren("'service", l);
};

/**
 * Returns a list of references in a component.
 */
scdl.references = function(l) {
    return namedElementChildren("'reference", l);
};

/**
 * Returns a list of bindings in a service or reference.
 */
scdl.bindings = function(l) {
    function filterBinding(v) {
        return isElement(v) && cadr(v).match("binding.") != null;
    }

    return filter(filterBinding, l);
};

/**
 * Returns the type of a binding.
 */
scdl.bindingType = function(l) {
    return elementName(l).substring(1);
};

/**
 * Returns the target of a reference.
 */
scdl.target = function(l) {
    function targetURI() {
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
        return bindingsTarget(scdl.bindings(l));
    }
    var turi = targetURI();
    if (isNil(turi))
        return turi;
    return car(tokens(turi));
};

/**
 * Returns a list of properties in a component.
 */
scdl.properties = function(l) {
    return namedElementChildren("'property", l);
};

/**
 * Returns the value of a property.
 */
scdl.propertyValue = function(l) {
    return elementValue(l);
};

/**
 * Convert a list of elements to a name -> element assoc list.
 */
scdl.nameToElementAssoc = function(l) {
    if (isNil(l))
        return l;
    return cons(mklist(scdl.name(car(l)), car(l)), scdl.nameToElementAssoc(cdr(l)));
};

