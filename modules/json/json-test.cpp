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
 * Test JSON read/write functions.
 */

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include "json.hpp"

namespace tuscany {

bool testJSEval() {
    JSONContext cx;
    std::string script("(function testJSON(n){ return JSON.parse(JSON.stringify(n)) })(5)");
    jsval rval;
    assert(JS_EvaluateScript(cx, cx.getGlobal(), script.c_str(), script.length(), "testJSON.js", 1, &rval));
    const std::string r(JS_GetStringBytes(JS_ValueToString(cx, rval)));
    assert(r == "5");
    return true;
}

bool testJSON() {
    JSONContext cx;

    list<value> phones = makeList<value> (std::string("408-1234"), std::string("650-1234"));
    list<value> l = makeList<value> (makeList<value> ("phones", phones), makeList<value> ("lastName", std::string("test\ttab")), makeList<value> ("firstName", std::string("test1")));
    print(l, std::cout);
    std::cout << std::endl;

    std::ostringstream sos;
    writeJSON(cx, l, sos);
    std::cout << sos.str() << std::endl;

    std::istringstream is(sos.str());
    list<value> r = readJSON(cx, is);
    print(r, std::cout);
    std::cout << std::endl;
    assert(r == l);

    return true;
}

}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::testJSEval();
    tuscany::testJSON();

    std::cout << "OK" << std::endl;

    return 0;
}
