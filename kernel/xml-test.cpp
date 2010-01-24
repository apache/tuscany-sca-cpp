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
#include "xml.hpp"

namespace tuscany {

const string currencyXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<composite xmlns=\"http://docs.oasis-open.org/ns/opencsa/sca/200912\" "
"xmlns:t=\"http://tuscany.apache.org/xmlns/sca/1.1\" "
"targetNamespace=\"http://services\" "
"name=\"currency\">"
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
"</composite>"
"\n";

const string customerXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<customer>"
"<name>jdoe</name>"
"<address><city>san francisco</city><state>ca</state></address>"
"<account><id>1234</id><balance>1000</balance></account>"
"<account><id>6789</id><balance>2000</balance></account>"
"<account><id>4567</id><balance>3000</balance></account>"
"</customer>"
"\n";


const bool isName(const value& token) {
    return isTaggedList(token, attribute) && attributeName(token) == "name";
}

bool testReadXML() {
    {
        istringstream is(customerXML);
        const list<value> c = readXML(streamList(is));
    }
    {
        istringstream is(currencyXML);
        const list<value> c = readXML(streamList(is));

        const value composite = car(c);
        assert(isTaggedList(composite, element));
        assert(elementName(composite) == "composite");
        assert(attributeValue(car(filter<value>(isName, elementChildren(composite)))) == string("currency"));
    }
    return true;
}

ostream* xmlWriter(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

bool testWriteXML() {
    {
        istringstream is(customerXML);
        const list<value> c = readXML(streamList(is));
        ostringstream os;
        writeXML<ostream*>(xmlWriter, &os, c);
        assert(str(os) == customerXML);
    }
    {
        istringstream is(currencyXML);
        const list<value> c = readXML(streamList(is));
        ostringstream os;
        writeXML<ostream*>(xmlWriter, &os, c);
        assert(str(os) == currencyXML);
    }
    return true;
}

bool testElements() {
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
            writeXML<ostream*>(xmlWriter, &os, e);
            assert(str(os) == customerXML);
        }
        {
            const list<value> c = mklist<value>(mklist<value>("customer", mklist<value>("name", string("jdoe")), cons<value>("address", ad), cons<value>("account", ac1), cons<value>("account", ac2), cons<value>("account", ac3)));
            const list<value> e = valuesToElements(c);
            const list<value> v = elementsToValues(e);

            ostringstream os;
            writeXML<ostream*>(xmlWriter, &os, e);
            assert(str(os) == customerXML);
        }
    }
    {
        istringstream is(customerXML);
        const list<value> c = readXML(streamList(is));
        const list<value> v = elementsToValues(c);
        const list<value> e = valuesToElements(v);
        ostringstream os;
        writeXML<ostream*>(xmlWriter, &os, e);
        assert(str(os) == customerXML);
    }
    return true;
}

bool testValues() {
    {
        const list<value> l = mklist<value>(list<value>() + "ns1:echoString" + (list<value>() + "@xmlns:ns1" + string("http://ws.apache.org/axis2/services/echo")) + (list<value>() + "text" + string("Hello World!")));
        const list<value> e = valuesToElements(l);
        const failable<list<string> > lx = writeXML(e);
        ostringstream os;
        write(content(lx), os);
        istringstream is(str(os));
        const list<value> x = readXML(streamList(is));
        const list<value> v = elementsToValues(x);
        assert(v == l);
    }
    return true;
}

}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testReadXML();
    tuscany::testWriteXML();
    tuscany::testElements();
    tuscany::testValues();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
