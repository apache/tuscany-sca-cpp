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
 * Test Web service Axiom support functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "list.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "value.hpp"
#include "perf.hpp"
#include "axis2.hpp"

namespace tuscany {
namespace webservice {

const string customerElement =
"<customer>"
"<name>jdoe</name>"
"<address><city>san francisco</city><state>ca</state></address>"
"<account><id>1234</id><balance>1000</balance></account>"
"<account><id>6789</id><balance>2000</balance></account>"
"<account><id>4567</id><balance>3000</balance></account>"
"</customer>";

bool testAxiom() {
    const Axis2Context ax;
    {
        const failable<axiom_node_t*> n = stringToAxiomNode(customerElement, ax);
        assert(hasContent(n));
        const failable<const string> c = axiomNodeToString(content(n), ax);
        assert(hasContent(c));
        assert(content(c) == customerElement);
    }
    {
        const list<value> arg = mklist<value>(
                list<value>() + "ns1:echoString"
                + (list<value>() + "@xmlns:ns1" + string("http://ws.apache.org/axis2/services/echo"))
                + (list<value>() + "text" + string("Hello World!")));
        const failable<axiom_node_t*> n = valuesToAxiomNode(arg, ax);
        assert(hasContent(n));
        const failable<const string> x = axiomNodeToString(content(n), ax);
        assert(hasContent(x));
        assert(content(x) == "<ns1:echoString xmlns:ns1=\"http://ws.apache.org/axis2/services/echo\"><text>Hello World!</text></ns1:echoString>");
        const failable<const list<value> > l = axiomNodeToValues(content(n), ax);
        assert(hasContent(l));
        assert(l == arg);
    }
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::webservice::testAxiom();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
