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
#include "json-test.hpp"

namespace tuscany {
namespace json {

ostream* jsonWriter(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const string testCustomer("{"
    "\"customer\":{"
    "\"@name\":\"jdoe\","
    "\"address\":{"
    "\"@city\":\"san francisco\","
    "\"@state\":\"ca\""
    "},"
    "\"account\":{"
    "\"id\":\"1234\","
    "\"@balance\":1000"
    "}"
    "}"
    "}");

const string testCustomer2("{"
    "\"customer\":{"
    "\"@name\":\"jdoe\","
    "\"account\":{"
    "\"id\":\"1234\","
    "\"@balance\":1000"
    "},"
    "\"address\":{"
    "\"@city\":\"san francisco\","
    "\"@state\":\"ca\""
    "}"
    "}"
    "}");

const string testPhones("{"
    "\"phones\":["
    "\"408-1234\","
    "\"650-1234\""
    "],"
    "\"lastName\":\"test\\ttab\","
    "\"@firstName\":\"test1\""
    "}");

const string testEcho("{"
    "\"ns1:echoString\":{"
    "\"@xmlns:ns1\":\"http://ws.apache.org/axis2/services/echo\","
    "\"text\":\"Hello World!\""
    "}"
    "}");

const string testArray("{"
    "\"fruit\":["
    "\"Apple\","
    "\"Orange\""
    "]"
    "}");

const bool testJSON() {
    const gc_scoped_pool pool;
    {
        const list<value> ad = mklist<value>(mklist<value>(attribute, "city", string("san francisco")), mklist<value>(attribute, "state", string("ca")));
        const list<value> ac = mklist<value>(mklist<value>(element, "id", string("1234")), mklist<value>(attribute, "balance", 1000));
        const list<value> cr = mklist<value>(mklist<value> (attribute, "name", string("jdoe")), cons<value>(element, cons<value>("address", ad)), cons<value>(element, cons<value>("account", ac)));
        const list<value> c = mklist<value>(cons<value>(element, cons<value>("customer", cr)));

        ostringstream os;
        writeElements<ostream*>(jsonWriter, &os, c);
        assert(str(os) == testCustomer);
    }
    {
        const list<value> phones = mklist<value> (string("408-1234"), string("650-1234"));
        const list<value> l = mklist<value> (mklist<value> (element, "phones", phones), mklist<value> (element, "lastName", string("test\ttab")), mklist<value> (attribute, "firstName", string("test1")));

        ostringstream os;
        writeElements<ostream*>(jsonWriter, &os, l);
        assert(str(os) == testPhones);

        istringstream is(str(os));
        const list<string> il = streamList(is);
        const list<value> r = content(readElements(il));
        assert(r == l);

        ostringstream wos;
        write(content(writeElements(r)), wos);
        assert(str(wos) == str(os));
    }
    {
        const list<value> l = mklist<value>(nilListValue + "ns1:echoString" + (nilListValue + "@xmlns:ns1" + string("http://ws.apache.org/axis2/services/echo")) + (nilListValue + "text" + string("Hello World!")));
        ostringstream wos;
        write(content(writeElements(valuesToElements(l))), wos);
        assert(str(wos) == testEcho);

        istringstream is(str(wos));
        const list<string> il = streamList(is);
        const list<value> r = elementsToValues(content(readElements(il)));
        assert(r == l);
    }
    {
        const list<value> l = mklist<value>((nilListValue + "fruit" + string("Apple")), (nilListValue + "fruit" + string("Orange")));
        ostringstream wos;
        write(content(writeElements(valuesToElements(l))), wos);
        assert(str(wos) == testArray);

        istringstream is(str(wos));
        const list<string> il = streamList(is);
        const list<value> r = elementsToValues(content(readElements(il)));
        const list<value> l2 = mklist<value>(nilListValue + "fruit" + (nilListValue + string("Apple") + string("Orange")));
        assert(r == l2);
    }
    return true;
}

const string testItem("{"
    "\"id\":3,"
    "\"result\":["
    "{"
    "\"price\":\"$2.99\","
    "\"name\":\"Apple\""
    "},"
    "{"
    "\"price\":\"$3.55\","
    "\"name\":\"Orange\""
    "},"
    "{"
    "\"price\":\"$1.55\","
    "\"name\":\"Pear\""
    "}"
    "]"
    "}");

const string testResult("{"
    "\"id\":1,"
    "\"result\":["
    "\"Service.get\","
    "\"Service.getTotal\""
    "]"
    "}");

const string testFeed("{"
    "\"id\":1,"
    "\"result\":["
    "\"Sample Feed\","
    "\"123456789\","
    "["
    "\"Item\","
    "\"111\","
    "{"
    "\"name\":\"Apple\","
    "\"currencyCode\":\"USD\","
    "\"currencySymbol\":\"$\","
    "\"price\":2.99"
    "}"
    "],"
    "["
    "\"Item\","
    "\"222\","
    "{"
    "\"name\":\"Orange\","
    "\"currencyCode\":\"USD\","
    "\"currencySymbol\":\"$\","
    "\"price\":3.55"
    "}"
    "],"
    "["
    "\"Item\","
    "\"333\","
    "{"
    "\"name\":\"Pear\","
    "\"currencyCode\":\"USD\","
    "\"currencySymbol\":\"$\","
    "\"price\":1.55"
    "}"
    "]"
    "]"
    "}");

const string testFeed2("{"
    "\"id\":1,"
    "\"result\":["
    "\"Sample Feed\","
    "\"123456789\","
    "["
    "\"Item\","
    "\"111\","
    "{"
    "\"name\":\"Apple\","
    "\"price\":2.99,"
    "\"currencyCode\":\"USD\","
    "\"currencySymbol\":\"$\""
    "}"
    "],"
    "["
    "\"Item\","
    "\"222\","
    "{"
    "\"name\":\"Orange\","
    "\"price\":3.55,"
    "\"currencyCode\":\"USD\","
    "\"currencySymbol\":\"$\""
    "}"
    "],"
    "["
    "\"Item\","
    "\"333\","
    "{"
    "\"name\":\"Pear\","
    "\"price\":1.55,"
    "\"currencyCode\":\"USD\","
    "\"currencySymbol\":\"$\""
    "}"
    "]"
    "]"
    "}");

const string testEchoreq("{"
    "\"id\":1,"
    "\"method\":\"echo\","
    "\"params\":["
    "{"
    "\"ns1:echoString\":{"
    "\"@xmlns:ns1\":\"http://ws.apache.org/axis2/services/echo\","
    "\"text\":\"Hello World!\""
    "}"
    "}"
    "]"
    "}");

const string testEchores("{"
    "\"id\":1,"
    "\"result\":{"
    "\"ns1:echoString\":{"
    "\"@xmlns:ns1\":\"http://ws.apache.org/axis2/c/samples\","
    "\"text\":\"Hello World!\""
    "}"
    "}"
    "}");

const bool testJSONRPC() {
    const gc_scoped_pool pool;
    {
        const string lm("{\"id\":1,\"method\":\"test\",\"params\":[]}");
        const list<value> e = content(readElements(mklist(lm)));
        const list<value> v = elementsToValues(e);
        assert(assoc<value>("id", v) == mklist<value>("id", 1));
        assert(assoc<value>("method", v) == mklist<value>("method", string("test")));
        assert(assoc<value>("params", v) == mklist<value>("params", nilListValue));
    }
    {
        const list<value> e = content(readElements(mklist(testItem)));
        ostringstream os;
        write(content(writeElements(e)), os);
        assert(str(os) == testItem);
        const list<value> v = elementsToValues(e);
        const list<value> r = valuesToElements(v);
        assert(r == e);
    }
    {
        const list<value> r = mklist<value>(mklist<value>("id", 1), mklist<value>("result", mklist<value>(string("Service.get"), string("Service.getTotal"))));
        const list<value> e = valuesToElements(r);
        ostringstream os;
        write(content(writeElements(e)), os);
        assert(str(os) == testResult);
    }
    {
        const list<value> r = content(readElements(mklist(testFeed)));
        const list<value> v = elementsToValues(r);
        const list<value> e = valuesToElements(v);
        ostringstream os;
        write(content(writeElements(e)), os);
#ifndef __clang__
        assert(str(os) == testFeed2);
#endif
    }
    {
        const list<value> arg = mklist<value>(nilListValue + "ns1:echoString" + (nilListValue + "@xmlns:ns1" + string("http://ws.apache.org/axis2/services/echo")) + (nilListValue + "text" + string("Hello World!")));
        const failable<list<string> > r = jsonRequest(1, "echo", mklist<value>(arg));
        ostringstream os;
        write(content(r), os);
        assert(str(os) == testEchoreq);

        istringstream is(str(os));
        const list<string> il = streamList(is);
        const list<value> ir = elementsToValues(content(readElements(il)));
        assert(car<value>(cadr<value>(caddr<value>(ir))) == arg);
    }
    {
        const list<value> res = mklist<value>(nilListValue + "ns1:echoString" + (nilListValue + "@xmlns:ns1" + string("http://ws.apache.org/axis2/c/samples")) + (nilListValue + "text" + string("Hello World!")));
        const failable<list<string> > r = jsonResult(1, res);
        ostringstream os;
        write(content(r), os);
        assert(str(os) == testEchores);

        istringstream is(str(os));
        const list<string> il = streamList(is);
        const list<value> ir = elementsToValues(content(readElements(il)));
        assert(cdr<value>(cadr<value>(ir)) == res);
    }
    return true;
}

const bool readWrite() {
    const gc_scoped_pool pool;

    istringstream is(testCustomer);
    const list<string> il = streamList(is);
    const list<value> r = elementsToValues(content(readElements(il)));

    ostringstream os;
    writeElements<ostream*>(jsonWriter, &os, valuesToElements(r));
    //assert(str(os) == testCustomer2);
    return true;
}

const bool testReadWritePerf() {
    const gc_scoped_pool pool;

    const blambda rwl = blambda(readWrite);
    cout << "JSON read + write test " << time(rwl, 5, 200) << " ms" << endl;

    return true;
}

const bool readWriteBigDoc() {
    const gc_scoped_pool pool;

    istringstream is(testBigDoc);
    const list<string> il = streamList(is);
    const list<value> r = elementsToValues(content(readElements(il)));

    ostringstream os;
    writeElements<ostream*>(jsonWriter, &os, valuesToElements(r));
    //assert(str(os) == testBigDoc2);
    return true;
}

const bool testReadWriteBigDocPerf() {
    const gc_scoped_pool pool;

    const blambda rwl = blambda(readWriteBigDoc);
    cout << "JSON big doc read + write test " << time(rwl, 5, 200) << " ms" << endl;

    return true;
}

}
}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::json::testJSON();
    tuscany::json::testJSONRPC();
    tuscany::json::testReadWritePerf();
    tuscany::json::testReadWriteBigDocPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
