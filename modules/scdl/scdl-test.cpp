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

/**
 * Test SCDL read functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "scdl.hpp"

namespace tuscany {
namespace scdl {

const bool testComposite() {
    ifstream is("test.composite");
    const list<value> c = content(xml::readElements(streamList(is)));
    return true;
}

const bool testComponents() {
    ifstream is("test.composite");
    const list<value> c = components(content(xml::readElements(streamList(is))));
    assert(length(c) == 4);

    const value store = car(c);
    assert(name(store) == string("Store"));
    const value impl = implementation(store);
    assert(implementationType(impl) == "implementation.scheme");
    assert(attributeValue("script", impl) == string("store.scm"));

    const value catalog = named(string("Catalog"), c);
    assert(name(catalog) == string("Catalog"));

    const list<value> t = mkrbtree(sort(nameToElementAssoc(c)));
    assert(rbtreeAssoc<value>("Catalog", t) == mklist<value>("Catalog" , cadr(c)));
    return true;
}

const bool testServices() {
    ifstream is("test.composite");
    const list<value> c = components(content(xml::readElements(streamList(is))));
    const value store = car(c);

    assert(length(services(store)) == 1);
    const value widget = car(services(store));
    assert(name(widget) == string("Widget"));

    assert(length(bindings(widget)) == 1);
    const value binding = car(bindings(widget));
    assert(uri(binding) == string("/store"));
    assert(bindingType(binding) == "binding.http");
    return true;
}

const bool testReferences() {
    ifstream is("test.composite");
    const list<value> c = components(content(xml::readElements(streamList(is))));
    const value store = car(c);

    assert(length(references(store)) == 3);
    const value catalog = car(references(store));
    assert(name(catalog) == string("catalog"));
    assert(target(catalog) == string("Catalog"));

    assert(length(bindings(catalog)) == 1);
    const value binding = car(bindings(catalog));
    assert(uri(binding) == nilValue);
    assert(bindingType(binding) == "binding.jsonrpc");

    const list<value> t = mkrbtree(sort(referenceToTargetAssoc(references(store))));
    assert(rbtreeAssoc<value>("shoppingCart", t) == mklist<value>(string("shoppingCart"), string("ShoppingCart/Cart")));
    return true;
}

const bool testProperties() {
    ifstream is("test.composite");
    const list<value> c = components(content(xml::readElements(streamList(is))));
    const value catalog = named(string("Catalog"), c);

    assert(length(properties(catalog)) == 1);
    const value currencyCode = car(properties(catalog));
    assert(name(currencyCode) == string("currencyCode"));
    assert(propertyValue(currencyCode) == string("USD"));
    return true;
}

}
}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::scdl::testComposite();
    tuscany::scdl::testComponents();
    tuscany::scdl::testServices();
    tuscany::scdl::testReferences();
    tuscany::scdl::testProperties();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
