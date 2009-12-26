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
 * Test Memcached access functions.
 */

#include <assert.h>
#include <iostream>
#include <string>
#include "perf.hpp"
#include "mcache.hpp"

namespace tuscany {
namespace cache {

bool testMemCached() {
    MemCached ch;

    assert(hasContent(post("a", std::string("AAA"), ch)));
    assert(get("a", ch) == value(std::string("AAA")));
    assert(hasContent(put("a", std::string("aaa"), ch)));
    assert(get("a", ch) == value(std::string("aaa")));
    assert(hasContent(del("a", ch)));
    assert(!hasContent(get("a", ch)));

    return true;
}

struct getLoop {
    MemCached& ch;
    getLoop(MemCached& ch) : ch(ch) {
    }
    const bool operator()() const {
        assert(get("c", ch) == value(std::string("CCC")));
        return true;
    }
};

bool testGetPerf() {
    MemCached ch;
    assert(hasContent(post("c", std::string("CCC"), ch)));

    const lambda<bool()> gl = getLoop(ch);
    std::cout << "Memcached get test " << time(gl, 5, 200) << " ms" << std::endl;

    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::cache::testMemCached();
    tuscany::cache::testGetPerf();

    std::cout << "OK" << std::endl;

    return 0;
}
