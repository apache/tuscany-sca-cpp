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
 * Test file database component.
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
namespace http {

const string uri("http://localhost:8090/httpget");

bool testGet() {
    http::CURLSession cs("", "", "", "");

    const failable<value> val = http::get(uri, cs);
    assert(hasContent(val));
    assert(contains(string(car<value>(cadr<value>(content(val)))), "It works"));

    return true;
}

struct getLoop {
    http::CURLSession cs;
    getLoop(http::CURLSession cs) : cs(cs) {
    }
    const bool operator()() const {
        const failable<value> val = http::get(uri, cs);
        assert(hasContent(val));
        assert(contains(string(car<value>(cadr<value>(content(val)))), "It works"));
        return true;
    }
};

bool testGetPerf() {
    http::CURLSession cs("", "", "", "");

    const lambda<bool()> gl = getLoop(cs);
    cout << "HTTP get test " << time(gl, 5, 200) << " ms" << endl;

    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::http::testGet();
    tuscany::http::testGetPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
