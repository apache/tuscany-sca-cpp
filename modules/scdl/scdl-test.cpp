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
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "slist.hpp"
#include "scdl.hpp"

namespace tuscany {
namespace scdl {

bool testComposite() {
    std::ifstream is("test.composite");
    const list<value> c = readXML(streamList(is));
    return true;
}

bool testComponents() {
    std::ifstream is("test.composite");
    const list<value> c = components(readXML(streamList(is)));
    assert(length(c) == 4);

    const value store = car(c);
    assert(name(store) == std::string("Store"));
    const value impl = implementation(store);
    assert(uri(impl) == std::string("store.html"));
    assert(implementationType(impl) == "t:implementation.widget");

    const value catalog = named(std::string("Catalog"), c);
    assert(name(catalog) == std::string("Catalog"));
    return true;
}

bool testServices() {
    std::ifstream is("test.composite");
    const list<value> c = components(readXML(streamList(is)));
    const value store = car(c);

    assert(length(services(store)) == 1);
    const value widget = car(services(store));
    assert(name(widget) == std::string("Widget"));

    assert(length(bindings(widget)) == 1);
    const value binding = car(bindings(widget));
    assert(uri(binding) == std::string("/store"));
    assert(bindingType(binding) == "t:binding.http");
    return true;
}

bool testReferences() {
    std::ifstream is("test.composite");
    const list<value> c = components(readXML(streamList(is)));
    const value store = car(c);

    assert(length(references(store)) == 3);
    const value catalog = car(references(store));
    assert(name(catalog) == std::string("catalog"));
    assert(target(catalog) == std::string("Catalog"));

    assert(length(bindings(catalog)) == 1);
    const value binding = car(bindings(catalog));
    assert(uri(binding) == value());
    assert(bindingType(binding) == "t:binding.jsonrpc");
    return true;
}

bool testProperties() {
    std::ifstream is("test.composite");
    const list<value> c = components(readXML(streamList(is)));
    const value catalog = named(std::string("Catalog"), c);

    assert(length(properties(catalog)) == 1);
    const value currencyCode = car(properties(catalog));
    assert(name(currencyCode) == std::string("currencyCode"));
    assert(propertyValue(currencyCode) == std::string("USD"));
    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::scdl::testComposite();
    tuscany::scdl::testComponents();
    tuscany::scdl::testServices();
    tuscany::scdl::testReferences();
    tuscany::scdl::testProperties();

    std::cout << "OK" << std::endl;

    return 0;
}
