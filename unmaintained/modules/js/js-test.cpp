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
 * Test JavaScript evaluation functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "parallel.hpp"
#include "perf.hpp"
#include "eval.hpp"

namespace tuscany {
namespace js {

bool testEval() {
    gc_scoped_pool pool;
    failable<value> v = evalScript("(function testJSON(n){ return JSON.parse(JSON.stringify(n)) })(5)");
    assert(hasContent(v));
    assert(content(v) == value(5));
    return true;
}

struct evalLoop {
    evalLoop() {
    }
    const bool operator()() const {
        failable<value> v = evalScript("(function testJSON(n){ return JSON.parse(JSON.stringify(n)) })(5)");
        assert(hasContent(v));
        assert(content(v) == value(5));
        return true;
    }
};

const bool testEvalPerf() {
    gc_scoped_pool pool;
    const lambda<bool()> el = evalLoop();
    cout << "JS eval test " << time(el, 5, 5) << " ms" << endl;
    return true;
}

}
}

int main() {
    tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::js::testEval();
    tuscany::js::testEvalPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
