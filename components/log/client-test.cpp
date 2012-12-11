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
 * Test log component.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"

#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "perf.hpp"
#include "../../modules/http/http.hpp"

namespace tuscany {
namespace log {

const string uri("http://localhost:8090/log");

const bool testLog() {
    const http::CURLSession cs("", "", "", "", 0);

    const list<value> i = nilListValue + "content" + (nilListValue + "item" 
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
    const list<value> a = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);

    const failable<value> id = http::post(a, uri, cs);
    assert(hasContent(id));

    return true;
}

const bool testLogPerf() {
    const list<value> i = nilListValue + "content" + (nilListValue + "item" 
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
    const list<value> a = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);

    const http::CURLSession cs("", "", "", "", 0);
    const failable<value> id = http::post(a, uri, cs);
    assert(hasContent(id));

    const blambda ll = [a, uri, cs]() -> const bool {
        const failable<value> id = http::post(a, uri, cs);
        assert(hasContent(id));
        return true;
    };
    cout << "Log test " << time(ll, 5, 200) << " ms" << endl;

    return true;
}

const bool testLogger() {
    const http::CURLSession cs("", "", "", "", 0);

    const failable<value> res = http::evalExpr(mklist<value>(string("sum"), 33, 22), string("http://localhost:8090/client"), cs);
    assert(hasContent(res));
    assert((int)content(res) == 55);

    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::log::testLog();
    tuscany::log::testLogPerf();
    tuscany::log::testLogger();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
