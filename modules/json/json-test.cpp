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
 * Test JSON data conversion functions.
 */

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include "slist.hpp"
#include "json.hpp"

namespace tuscany {

bool testJSEval() {
    JSONContext cx;
    const std::string script("(function testJSON(n){ return JSON.parse(JSON.stringify(n)) })(5)");
    jsval rval;
    assert(JS_EvaluateScript(cx, cx.getGlobal(), script.c_str(), script.length(), "testJSON.js", 1, &rval));
    const std::string r(JS_GetStringBytes(JS_ValueToString(cx, rval)));
    assert(r == "5");
    return true;
}

std::ostringstream* jsonWriter(std::ostringstream* os, const std::string& s) {
    (*os) << s;
    return os;
}

bool testJSON() {
    const JSONContext cx;

    {
        const list<value> ad = mklist<value>(mklist<value>(attribute, "city", std::string("san francisco")), mklist<value>(attribute, "state", std::string("ca")));
        const list<value> ac = mklist<value>(mklist<value>(element, "id", std::string("1234")), mklist<value>(attribute, "balance", 1000));
        const list<value> cr = mklist<value>(mklist<value> (attribute, "name", std::string("jdoe")), cons<value>(element, cons<value>("address", ad)), cons<value>(element, cons<value>("account", ac)));
        const list<value> c = mklist<value>(cons<value>(element, cons<value>("customer", cr)));
        std::ostringstream os;
        writeJSON<std::ostringstream*>(cx, jsonWriter, &os, c);
        assert(os.str() == "{\"customer\":{\"name\":\"jdoe\",\"address\":{\"city\":\"san francisco\",\"state\":\"ca\"},\"account\":{\"id\":\"1234\",\"balance\":1000}}}");
    }
    {
        const list<value> phones = mklist<value> (std::string("408-1234"), std::string("650-1234"));
        const list<value> l = mklist<value> (mklist<value> (element, "phones", phones), mklist<value> (attribute, "lastName", std::string("test\ttab")), mklist<value> (attribute, "firstName", std::string("test1")));

        std::ostringstream os;
        writeJSON<std::ostringstream*>(cx, jsonWriter, &os, l);
        assert(os.str() == "{\"phones\":[\"408-1234\",\"650-1234\"],\"lastName\":\"test\\u0009tab\",\"firstName\":\"test1\"}");

        std::istringstream is(os.str());
        const list<std::string> il = streamList(is);
        const list<value> r = readJSON(cx, il);
        assert(r == l);

        std::ostringstream wos;
        write(writeJSON(cx, r), wos);
        assert(wos.str() == os.str());
    }
    return true;
}

bool testJSONRPC() {
    const std::string lm("{\"id\": 1, \"method\": \"system.listMethods\", \"params\": []}");
    JSONContext cx;
    const list<value> e = readJSON(cx, mklist(lm));
    const list<value> v = elementsToValues(e);

    assert(assoc<value>("id", v) == mklist<value>("id", 1));
    assert(assoc<value>("method", v) == mklist<value>("method", std::string("system.listMethods")));
    assert(assoc<value>("params", v) == mklist<value>("params", list<value>()));
    return true;
}

}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::testJSEval();
    tuscany::testJSON();
    tuscany::testJSONRPC();

    std::cout << "OK" << std::endl;

    return 0;
}
