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

std::ostringstream* jsonWriter(const std::string& s, std::ostringstream* os) {
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
        writeJSON<std::ostringstream*>(jsonWriter, &os, c, cx);
        assert(os.str() == "{\"customer\":{\"name\":\"jdoe\",\"address\":{\"city\":\"san francisco\",\"state\":\"ca\"},\"account\":{\"id\":\"1234\",\"balance\":1000}}}");
    }
    {
        const list<value> phones = mklist<value> (std::string("408-1234"), std::string("650-1234"));
        const list<value> l = mklist<value> (mklist<value> (element, "phones", phones), mklist<value> (element, "lastName", std::string("test\ttab")), mklist<value> (element, "firstName", std::string("test1")));

        std::ostringstream os;
        writeJSON<std::ostringstream*>(jsonWriter, &os, l, cx);
        assert(os.str() == "{\"phones\":[\"408-1234\",\"650-1234\"],\"lastName\":\"test\\u0009tab\",\"firstName\":\"test1\"}");

        std::istringstream is(os.str());
        const list<std::string> il = streamList(is);
        const list<value> r = readJSON(il, cx);
        assert(r == l);

        std::ostringstream wos;
        write(writeJSON(r, cx), wos);
        assert(wos.str() == os.str());
    }
    return true;
}

bool testJSONRPC() {
    JSONContext cx;
    {
        const std::string lm("{\"id\": 1, \"method\": \"system.listMethods\", \"params\": []}");
        const list<value> e = readJSON(mklist(lm), cx);
        const list<value> v = elementsToValues(e);
        assert(assoc<value>("id", v) == mklist<value>("id", 1));
        assert(assoc<value>("method", v) == mklist<value>("method", std::string("system.listMethods")));
        assert(assoc<value>("params", v) == mklist<value>("params", list<value>()));
    }
    {
        const std::string i("{\"id\":3,\"result\":[{\"price\":\"$2.99\",\"name\":\"Apple\"},{\"price\":\"$3.55\",\"name\":\"Orange\"},{\"price\":\"$1.55\",\"name\":\"Pear\"}]}");
        const list<value> e = readJSON(mklist(i), cx);
        const std::string i2("{\"id\":3,\"result\":{\"0\":{\"price\":\"$2.99\",\"name\":\"Apple\"},\"1\":{\"price\":\"$3.55\",\"name\":\"Orange\"},\"2\":{\"price\":\"$1.55\",\"name\":\"Pear\"}}}");
        const list<value> e2 = readJSON(mklist(i), cx);
        assert(e == e2);
    }
    {
        const std::string i("{\"id\":3,\"result\":[{\"price\":\"$2.99\",\"name\":\"Apple\"},{\"price\":\"$3.55\",\"name\":\"Orange\"},{\"price\":\"$1.55\",\"name\":\"Pear\"}]}");
        const list<value> e = readJSON(mklist(i), cx);
        std::ostringstream os;
        write(writeJSON(e, cx), os);
        assert(os.str() == i);
        const list<value> v = elementsToValues(e);
        const list<value> r = valuesToElements(v);
        assert(r == e);
    }
    {
        const list<value> r = mklist<value>(mklist<value>("id", 1), mklist<value>("result", mklist<value>(std::string("Service.get"), std::string("Service.getTotal"))));
        const list<value> e = valuesToElements(r);
        std::ostringstream os;
        write(writeJSON(e, cx), os);
        assert(os.str() == "{\"id\":1,\"result\":[\"Service.get\",\"Service.getTotal\"]}");
    }
    {
        const std::string f("{\"id\":1,\"result\":[\"Sample Feed\",\"123456789\",[\"Item\",\"111\",{\"javaClass\":\"services.Item\",\"name\":\"Apple\",\"currencyCode\":\"USD\",\"currencySymbol\":\"$\",\"price\":2.99}],[\"Item\",\"222\",{\"javaClass\":\"services.Item\",\"name\":\"Orange\",\"currencyCode\":\"USD\",\"currencySymbol\":\"$\",\"price\":3.55}],[\"Item\",\"333\",{\"javaClass\":\"services.Item\",\"name\":\"Pear\",\"currencyCode\":\"USD\",\"currencySymbol\":\"$\",\"price\":1.55}]]}");
        const list<value> r = readJSON(mklist(f), cx);
        const list<value> v = elementsToValues(r);
        const list<value> e = valuesToElements(v);
        std::ostringstream os;
        write(writeJSON(e, cx), os);
        assert(os.str() == f);
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
