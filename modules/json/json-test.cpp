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
namespace json {

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
        write<std::ostringstream*>(cx, jsonWriter, &os, c);
        assert(os.str() == "{\"customer\":{\"name\":\"jdoe\",\"address\":{\"city\":\"san francisco\",\"state\":\"ca\"},\"account\":{\"id\":\"1234\",\"balance\":1000}}}");
    }
    {
        const list<value> phones = mklist<value> (std::string("408-1234"), std::string("650-1234"));
        const list<value> l = mklist<value> (mklist<value> (element, "phones", phones), mklist<value> (element, "lastName", std::string("test\ttab")), mklist<value> (element, "firstName", std::string("test1")));

        std::ostringstream os;
        write<std::ostringstream*>(cx, jsonWriter, &os, l);
        assert(os.str() == "{\"phones\":[\"408-1234\",\"650-1234\"],\"lastName\":\"test\\u0009tab\",\"firstName\":\"test1\"}");

        std::istringstream is(os.str());
        const list<std::string> il = streamList(is);
        const list<value> r = read(cx, il);
        assert(r == l);

        std::ostringstream wos;
        write(write(cx, r), wos);
        assert(wos.str() == os.str());
    }
    return true;
}

bool testJSONRPC() {
    JSONContext cx;
    {
        const std::string lm("{\"id\": 1, \"method\": \"system.listMethods\", \"params\": []}");
        const list<value> e = read(cx, mklist(lm));
        const list<value> v = elementsToValues(e);
        assert(assoc<value>("id", v) == mklist<value>("id", 1));
        assert(assoc<value>("method", v) == mklist<value>("method", std::string("system.listMethods")));
        assert(assoc<value>("params", v) == mklist<value>("params", list<value>()));
    }
    {
        const std::string i("{\"id\":3,\"result\":[{\"price\":\"$2.99\",\"name\":\"Apple\"},{\"price\":\"$3.55\",\"name\":\"Orange\"},{\"price\":\"$1.55\",\"name\":\"Pear\"}]}");
        const list<value> e = read(cx, mklist(i));
        const std::string i2("{\"id\":3,\"result\":{\"0\":{\"price\":\"$2.99\",\"name\":\"Apple\"},\"1\":{\"price\":\"$3.55\",\"name\":\"Orange\"},\"2\":{\"price\":\"$1.55\",\"name\":\"Pear\"}}}");
        const list<value> e2 = read(cx, mklist(i));
        assert(e == e2);
    }
    {
        const std::string i("{\"id\":3,\"result\":[{\"price\":\"$2.99\",\"name\":\"Apple\"},{\"price\":\"$3.55\",\"name\":\"Orange\"},{\"price\":\"$1.55\",\"name\":\"Pear\"}]}");
        const list<value> e = read(cx, mklist(i));
        std::ostringstream os;
        write(write(cx, e), os);
        assert(os.str() == i);
        const list<value> v = elementsToValues(e);
        const list<value> r = valuesToElements(v);
        assert(r == e);
    }
    {
        const list<value> r = mklist<value>(mklist<value>("id", 1), mklist<value>("result", mklist<value>(std::string("Service.get"), std::string("Service.getTotal"))));
        const list<value> e = valuesToElements(r);
        std::ostringstream os;
        write(write(cx, e), os);
        assert(os.str() == "{\"id\":1,\"result\":[\"Service.get\",\"Service.getTotal\"]}");
    }
    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::json::testJSEval();
    tuscany::json::testJSON();
    tuscany::json::testJSONRPC();

    std::cout << "OK" << std::endl;

    return 0;
}
