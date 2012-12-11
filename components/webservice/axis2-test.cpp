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
 * Test WebService Axis2 client support functions.
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

const bool testEval() {
    const Axis2Context ax;

    const value func = "http://ws.apache.org/axis2/c/samples/echoString";
    const list<value> arg = mklist<value>(
            nilListValue + "ns1:echoString"
            + (nilListValue + "@xmlns:ns1" + string("http://ws.apache.org/axis2/services/echo"))
            + (nilListValue + "text" + string("Hello World!")));

    const failable<value> rval = evalExpr(mklist<value>(func, arg, string("http://localhost:9090/axis2/services/echo")), ax);
    assert(hasContent(rval));

    const list<value> r = mklist<value>(
            nilListValue + "ns1:echoString"
            + (nilListValue + "@xmlns:ns1" + string("http://ws.apache.org/axis2/c/samples"))
            + (nilListValue + "text" + string("Hello World!")));
    cerr << content(rval) << endl;
    assert(content(rval) == r);

    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::webservice::testEval();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
