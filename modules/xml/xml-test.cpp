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
 * Test XML handling functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "perf.hpp"
#include "xml.hpp"
#include "xml-test.hpp"

namespace tuscany {
namespace xml {

const string currencyXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<composite xmlns=\"http://docs.oasis-open.org/ns/opencsa/sca/200912\" targetNamespace=\"http://services\" name=\"currency\">"
 "<component name=\"CurrencyConverterWebService\">"
  "<implementation.java class=\"services.CurrencyConverterImpl\"/>"
  "<service name=\"CurrencyConverter\">"
   "<binding.ws/>"
  "</service>"
 "</component>"
 "<component name=\"CurrencyConverterWebService2\">"
  "<implementation.java class=\"services.CurrencyConverterImpl2\"/>"
  "<service name=\"CurrencyConverter2\">"
    "<binding.atom/>"
  "</service>"
  "<property name=\"currency\">US</property>"
 "</component>"
"</composite>\n";

const string customerXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<customer>"
 "<name>jdoe</name>"
 "<address>"
  "<city>san francisco</city>"
  "<state>ca</state>"
 "</address>"
 "<account>"
  "<id>1234</id>"
  "<balance>1000</balance>"
 "</account>"
 "<account>"
  "<id>6789</id>"
  "<balance>2000</balance>"
 "</account>"
 "<account>"
  "<id>4567</id>"
  "<balance>3000</balance>"
 "</account>"
"</customer>\n";

const string abcXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<a>"
 "<m>abc</m>"
 "<m>def</m>"
 "<m>xyz</m>"
 "<m>tuv</m>"
"</a>\n";

const string xyzXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<a>"
 "<m>"
  "<id>123</id>"
  "<name>abc</name>"
 "</m>"
 "<m>"
  "<id>234</id>"
  "<name>def</name>"
 "</m>"
 "<m>"
  "<id>345</id>"
  "<name>xyz</name>"
 "</m>"
 "<m>"
  "<id>456</id>"
  "<name>tuv</name>"
 "</m>"
"</a>\n";

const string boolXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<x>true</x>\n";

const string numberXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<x>123.4</x>\n";

const bool isName(const value& token) {
    return isTaggedList(token, attribute) && attributeName(token) == "name";
}

const bool testReadXML() {
    {
        istringstream is(customerXML);
        const list<value> c = content(readElements(streamList(is)));
    }
    {
        istringstream is(currencyXML);
        const list<value> c = content(readElements(streamList(is)));

        const value composite = car(c);
        assert(isTaggedList(composite, element));
        assert(elementName(composite) == "composite");
        assert(attributeValue(car(filter<value>(isName, elementChildren(composite)))) == string("currency"));
    }
    {
        istringstream is(boolXML);
        const list<value> c = content(readElements(streamList(is)));
        assert(caddr<value>(car(c)) == value(true));
    }
    {
        istringstream is(numberXML);
        const list<value> c = content(readElements(streamList(is)));
        assert(caddr<value>(car(c)) == value(123.4));
    }
    return true;
}

ostream* xmlWriter(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const bool testWriteXML() {
    {
        istringstream is(customerXML);
        const list<value> c = content(readElements(streamList(is)));
        ostringstream os;
        writeElements<ostream*>(xmlWriter, &os, c);
        assert(str(os) == customerXML);
    }
    {
        istringstream is(currencyXML);
        const list<value> c = content(readElements(streamList(is)));
        ostringstream os;
        writeElements<ostream*>(xmlWriter, &os, c);
        assert(str(os) == currencyXML);
    }
    return true;
}

const bool testElements() {
    {
        const list<value> ad = mklist<value>(mklist<value>("city", string("san francisco")), mklist<value>("state", string("ca")));
        const list<value> ac1 = mklist<value>(mklist<value>("id", string("1234")), mklist<value>("balance", 1000));
        const list<value> ac2 = mklist<value>(mklist<value>("id", string("6789")), mklist<value>("balance", 2000));
        const list<value> ac3 = mklist<value>(mklist<value>("id", string("4567")), mklist<value>("balance", 3000));
        {
            const list<value> c = mklist<value>(mklist<value>("customer", mklist<value>("name", string("jdoe")), cons<value>("address", ad), mklist<value>("account", mklist<value>(ac1, ac2, ac3))));
            const list<value> e = valuesToElements(c);
            const list<value> v = elementsToValues(e);
            assert(v == c);

            ostringstream os;
            writeElements<ostream*>(xmlWriter, &os, e);
            assert(str(os) == customerXML);
        }
        {
            const list<value> c = mklist<value>(mklist<value>("customer", mklist<value>("name", string("jdoe")), cons<value>("address", ad), cons<value>("account", ac1), cons<value>("account", ac2), cons<value>("account", ac3)));
            const list<value> e = valuesToElements(c);
            const list<value> v = elementsToValues(e);

            ostringstream os;
            writeElements<ostream*>(xmlWriter, &os, e);
            assert(str(os) == customerXML);
        }
    }
    {
        istringstream is(abcXML);
        const list<value> c = content(readElements(streamList(is)));
        const list<value> v = elementsToValues(c);
        const list<value> e = valuesToElements(v);
        ostringstream os;
        writeElements<ostream*>(xmlWriter, &os, e);
        assert(str(os) == abcXML);
    }
    {
        istringstream is(xyzXML);
        const list<value> c = content(readElements(streamList(is)));
        const list<value> v = elementsToValues(c);
        const list<value> e = valuesToElements(v);
        ostringstream os;
        writeElements<ostream*>(xmlWriter, &os, e);
        assert(str(os) == xyzXML);
    }
    {
        istringstream is(customerXML);
        const list<value> c = content(readElements(streamList(is)));
        const list<value> v = elementsToValues(c);
        const list<value> e = valuesToElements(v);
        ostringstream os;
        writeElements<ostream*>(xmlWriter, &os, e);
        assert(str(os) == customerXML);
    }
    return true;
}

const bool testValues() {
    {
        const list<value> l = mklist<value>(nilListValue + "ns1:echoString" + (nilListValue + "@xmlns:ns1" + string("http://ws.apache.org/axis2/services/echo")) + (nilListValue + "text" + string("Hello World!")));
        const list<value> e = valuesToElements(l);
        const failable<list<string> > lx = writeElements(e);
        ostringstream os;
        write(content(lx), os);
        istringstream is(str(os));
        const list<value> x = content(readElements(streamList(is)));
        const list<value> v = elementsToValues(x);
        assert(v == l);
    }
    return true;
}

const bool testReadWrite() {
    const gc_scoped_pool pool;

    istringstream is(customerXML);
    const list<string> il = streamList(is);
    const list<value> r = elementsToValues(content(readElements(il)));

    ostringstream os;
    writeElements<ostream*>(xmlWriter, &os, valuesToElements(r));
    //assert(str(os) == customerXML);
    return true;
}

const bool testReadWritePerf() {
    const gc_scoped_pool pool;

    const blambda rwl = blambda(testReadWrite);
    cout << "XML read + write test " << time(rwl, 5, 200) << " ms" << endl;

    return true;
}

const bool testReadWriteBigDoc() {
    const gc_scoped_pool pool;

    istringstream is(testBigDoc);
    const list<string> il = streamList(is);
    const list<value> r = elementsToValues(content(readElements(il)));

    ostringstream os;
    writeElements<ostream*>(xmlWriter, &os, valuesToElements(r));
    //assert(str(os) == testBigDoc);
    return true;
}

const bool testReadWriteBigDocPerf() {
    const gc_scoped_pool pool;

    const blambda rwl = blambda(testReadWriteBigDoc);
    cout << "XML big doc read + write test " << time(rwl, 5, 200) << " ms" << endl;

    return true;
}

const string customersXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<customers>"
 "<customer>"
  "<name>jane doe</name>"
  "<address>"
   "<city>san francisco</city>"
   "<state>ca</state>"
  "</address>"
  "<account>"
   "<id>1234</id>"
   "<balance>1000</balance>"
  "</account>"
  "<account>"
   "<id>6789</id>"
   "<balance>4000</balance>"
  "</account>"
 "</customer>"
 "<customer>"
  "<name>john doe</name>"
  "<address>"
   "<city>new york</city>"
   "<state>ny</state>"
  "</address>"
  "<account>"
   "<id>5678</id>"
   "<balance>2000</balance>"
  "</account>"
  "<account>"
   "<id>4321</id>"
   "<balance>1000</balance>"
  "</account>"
 "</customer>"
"</customers>\n";

const bool testQuery() {
    {
        const gc_scoped_pool pool;

        // Read XML doc containing customers
        istringstream is(customersXML);
        const list<value> c = content(readElements(streamList(is)));
        //cout << c << endl;

        // Map list of customers to a list of (customer name, customer balance) pairs
        const list<value> nb = map<value, value>([](value e) -> const value {

            // Map list of accounts to a list of balances
            list<value> bals = map<value, value>([](value e) -> const value { 
                return (double)elementValue(elementChild("balance", e));
            }, elementChildren("account", e));

            // Reduce list of balances to compute customer balance
            value total = reduce<value, value>([](value accum, value v) -> const value {
                return (double)accum + (double)v;
            }, value(0), bals);

            // Return (customer name, customer balance) pair
            return mklist<value>(elementValue(elementChild("name", e)), total);

        }, elementChildren("customer", elementChild("customers", c)));

        // The resulting list of (customer name, customer balance) pairs
        //cout << nb << endl;
        assert(nb == mklist<value>(mklist<value>("jane doe", 5000), mklist<value>("john doe", 3000)));
    }
    return true;
}

}
}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::xml::testReadXML();
    tuscany::xml::testWriteXML();
    tuscany::xml::testElements();
    tuscany::xml::testValues();
    tuscany::xml::testReadWritePerf();
    tuscany::xml::testReadWriteBigDocPerf();
    tuscany::xml::testQuery();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
