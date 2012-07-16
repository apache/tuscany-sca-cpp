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
#include "stream.hpp"
#include "string.hpp"
#include "json.hpp"
#include "perf.hpp"

namespace tuscany {
namespace json {

ostream* jsonWriter(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const string jscustomer("{\n"
    " \"customer\": {\n"
    "  \"@name\": \"jdoe\",\n"
    "  \"address\": {\n"
    "   \"@city\": \"san francisco\",\n"
    "   \"@state\": \"ca\"\n"
    "  },\n"
    "  \"account\": {\n"
    "   \"id\": \"1234\",\n"
    "   \"@balance\": 1000\n"
    "  }\n"
    " }\n"
    "}");

const string jsphones("{\n"
    " \"phones\": [\n"
    "  \"408-1234\",\n"
    "  \"650-1234\"\n"
    " ],\n"
    " \"lastName\": \"test\\u0009tab\",\n"
    " \"@firstName\": \"test1\"\n"
    "}");

const string jsecho("{\n"
    " \"ns1:echoString\": {\n"
    "  \"@xmlns:ns1\": \"http://ws.apache.org/axis2/services/echo\",\n"
    "  \"text\": \"Hello World!\"\n"
    " }\n"
    "}");

const string jsarray("{\n"
    " \"fruit\": [\n"
    "  \"Apple\",\n"
    "  \"Orange\"\n"
    " ]\n"
    "}");

bool testJSON() {
    gc_scoped_pool pool;
    const js::JSContext cx;

    {
        const list<value> ad = mklist<value>(mklist<value>(attribute, "city", string("san francisco")), mklist<value>(attribute, "state", string("ca")));
        const list<value> ac = mklist<value>(mklist<value>(element, "id", string("1234")), mklist<value>(attribute, "balance", 1000));
        const list<value> cr = mklist<value>(mklist<value> (attribute, "name", string("jdoe")), cons<value>(element, cons<value>("address", ad)), cons<value>(element, cons<value>("account", ac)));
        const list<value> c = mklist<value>(cons<value>(element, cons<value>("customer", cr)));

        ostringstream os;
        writeJSON<ostream*>(jsonWriter, &os, c, cx);
        assert(str(os) == jscustomer);
    }
    {
        const list<value> phones = mklist<value> (string("408-1234"), string("650-1234"));
        const list<value> l = mklist<value> (mklist<value> (element, "phones", phones), mklist<value> (element, "lastName", string("test\ttab")), mklist<value> (attribute, "firstName", string("test1")));

        ostringstream os;
        writeJSON<ostream*>(jsonWriter, &os, l, cx);
        assert(str(os) == jsphones);

        istringstream is(str(os));
        const list<string> il = streamList(is);
        const list<value> r = content(readJSON(il, cx));
        assert(r == l);

        ostringstream wos;
        write(content(writeJSON(r, cx)), wos);
        assert(str(wos) == str(os));
    }
    {
        const list<value> l = mklist<value>(list<value>() + "ns1:echoString" + (list<value>() + "@xmlns:ns1" + string("http://ws.apache.org/axis2/services/echo")) + (list<value>() + "text" + string("Hello World!")));
        ostringstream wos;
        write(content(writeJSON(valuesToElements(l), cx)), wos);
        assert(str(wos) == jsecho);

        istringstream is(str(wos));
        const list<string> il = streamList(is);
        const list<value> r = elementsToValues(content(readJSON(il, cx)));
        assert(r == l);
    }
    {
        const list<value> l = mklist<value>((list<value>() + "fruit" + string("Apple")), (list<value>() + "fruit" + string("Orange")));
        ostringstream wos;
        write(content(writeJSON(valuesToElements(l), cx)), wos);
        assert(str(wos) == jsarray);

        istringstream is(str(wos));
        const list<string> il = streamList(is);
        const list<value> r = elementsToValues(content(readJSON(il, cx)));
        const list<value> l2 = mklist<value>(list<value>() + "fruit" + (list<value>() + string("Apple") + string("Orange")));
        assert(r == l2);
    }
    return true;
}

const string jsitem("{\n"
    " \"id\": 3,\n"
    " \"result\": [\n"
    "  {\n"
    "   \"price\": \"$2.99\",\n"
    "   \"name\": \"Apple\"\n"
    "  },\n"
    "  {\n"
    "   \"price\": \"$3.55\",\n"
    "   \"name\": \"Orange\"\n"
    "  },\n"
    "  {\n"
    "   \"price\": \"$1.55\",\n"
    "   \"name\": \"Pear\"\n"
    "  }\n"
    " ]\n"
    "}");

const string jsresult("{\n"
    " \"id\": 1,\n"
    " \"result\": [\n"
    "  \"Service.get\",\n"
    "  \"Service.getTotal\"\n"
    " ]\n"
    "}");

const string jsfeed("{\n"
    " \"id\": 1,\n"
    " \"result\": [\n"
    "  \"Sample Feed\",\n"
    "  \"123456789\",\n"
    "  [\n"
    "   \"Item\",\n"
    "   \"111\",\n"
    "   {\n"
    "    \"name\": \"Apple\",\n"
    "    \"currencyCode\": \"USD\",\n"
    "    \"currencySymbol\": \"$\",\n"
    "    \"price\": 2.99\n"
    "   }\n"
    "  ],\n"
    "  [\n"
    "   \"Item\",\n"
    "   \"222\",\n"
    "   {\n"
    "    \"name\": \"Orange\",\n"
    "    \"currencyCode\": \"USD\",\n"
    "    \"currencySymbol\": \"$\",\n"
    "    \"price\": 3.55\n"
    "   }\n"
    "  ],\n"
    "  [\n"
    "   \"Item\",\n"
    "   \"333\",\n"
    "   {\n"
    "    \"name\": \"Pear\",\n"
    "    \"currencyCode\": \"USD\",\n"
    "    \"currencySymbol\": \"$\",\n"
    "    \"price\": 1.55\n"
    "   }\n"
    "  ]\n"
    " ]\n"
    "}");

const string jsechoreq("{\n"
    " \"id\": 1,\n"
    " \"method\": \"echo\",\n"
    " \"params\": [\n"
    "  {\n"
    "   \"ns1:echoString\": {\n"
    "    \"@xmlns:ns1\": \"http://ws.apache.org/axis2/services/echo\",\n"
    "    \"text\": \"Hello World!\"\n"
    "   }\n"
    "  }\n"
    " ]\n"
    "}");

const string jsechores("{\n"
    " \"id\": 1,\n"
    " \"result\": {\n"
    "  \"ns1:echoString\": {\n"
    "   \"@xmlns:ns1\": \"http://ws.apache.org/axis2/c/samples\",\n"
    "   \"text\": \"Hello World!\"\n"
    "  }\n"
    " }\n"
    "}");

bool testJSONRPC() {
    gc_scoped_pool pool;
    js::JSContext cx;
    {
        const string lm("{\"id\": 1, \"method\": \"test\", \"params\": []}");
        const list<value> e = content(readJSON(mklist(lm), cx));
        const list<value> v = elementsToValues(e);
        assert(assoc<value>("id", v) == mklist<value>("id", 1));
        assert(assoc<value>("method", v) == mklist<value>("method", string("test")));
        assert(assoc<value>("params", v) == mklist<value>("params", list<value>()));
    }
    {
        const string i2 = "{\"id\": 3,\"result\": {\"0\": {\"price\": \"$2.99\",\"name\": \"Apple\"},\"1\": {\"price\": \"$3.55\",\"name\": \"Orange\"},\"2\": {\"price\": \"$1.55\",\"name\": \"Pear\"}}}";
        const list<value> e = content(readJSON(mklist(jsitem), cx));
        const list<value> e2 = content(readJSON(mklist(i2), cx));
        assert(e == e2);
    }
    {
        const list<value> e = content(readJSON(mklist(jsitem), cx));
        ostringstream os;
        write(content(writeJSON(e, cx)), os);
        assert(str(os) == jsitem);
        const list<value> v = elementsToValues(e);
        const list<value> r = valuesToElements(v);
        assert(r == e);
    }
    {
        const list<value> r = mklist<value>(mklist<value>("id", 1), mklist<value>("result", mklist<value>(string("Service.get"), string("Service.getTotal"))));
        const list<value> e = valuesToElements(r);
        ostringstream os;
        write(content(writeJSON(e, cx)), os);
        assert(str(os) == jsresult);
    }
    {
        const list<value> r = content(readJSON(mklist(jsfeed), cx));
        const list<value> v = elementsToValues(r);
        const list<value> e = valuesToElements(v);
        ostringstream os;
        write(content(writeJSON(e, cx)), os);
        assert(str(os) == jsfeed);
    }
    {
        const list<value> arg = mklist<value>(list<value>() + "ns1:echoString" + (list<value>() + "@xmlns:ns1" + string("http://ws.apache.org/axis2/services/echo")) + (list<value>() + "text" + string("Hello World!")));
        const failable<list<string> > r = jsonRequest(1, "echo", mklist<value>(arg), cx);
        ostringstream os;
        write(content(r), os);
        assert(str(os) == jsechoreq);

        istringstream is(str(os));
        const list<string> il = streamList(is);
        const list<value> ir = elementsToValues(content(readJSON(il, cx)));
        assert(car<value>(cadr<value>(caddr<value>(ir))) == arg);
    }
    {
        const list<value> res = mklist<value>(list<value>() + "ns1:echoString" + (list<value>() + "@xmlns:ns1" + string("http://ws.apache.org/axis2/c/samples")) + (list<value>() + "text" + string("Hello World!")));
        const failable<list<string> > r = jsonResult(1, res, cx);
        ostringstream os;
        write(content(r), os);
        assert(str(os) == jsechores);

        istringstream is(str(os));
        const list<string> il = streamList(is);
        const list<value> ir = elementsToValues(content(readJSON(il, cx)));
        assert(cdr<value>(cadr<value>(ir)) == res);
    }
    return true;
}

struct testReadWrite {
    testReadWrite() {
    }
    const bool operator()() const {
        gc_scoped_pool pool;
        js::JSContext cx;

        const list<value> ad = mklist<value>(mklist<value>(attribute, "city", string("san francisco")), mklist<value>(attribute, "state", string("ca")));
        const list<value> ac = mklist<value>(mklist<value>(element, "id", string("1234")), mklist<value>(attribute, "balance", 1000));
        const list<value> cr = mklist<value>(mklist<value> (attribute, "name", string("jdoe")), cons<value>(element, cons<value>("address", ad)), cons<value>(element, cons<value>("account", ac)));
        const list<value> c = mklist<value>(cons<value>(element, cons<value>("customer", cr)));

        ostringstream os;
        writeJSON<ostream*>(jsonWriter, &os, c, cx);
        assert(str(os) == jscustomer);

        istringstream is(jscustomer);
        const list<string> il = streamList(is);
        const list<value> r = content(readJSON(il, cx));
        assert(r == c);
        return true;
    }
};

bool testJSONPerf() {
    gc_scoped_pool pool;

    const lambda<bool()> rwl = lambda<bool()>(testReadWrite());
    cout << "JSON read + write test " << time(rwl, 5, 200) << " ms" << endl;

    return true;
}

}
}

int main() {
    tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::json::testJSON();
    tuscany::json::testJSONRPC();
    tuscany::json::testJSONPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
