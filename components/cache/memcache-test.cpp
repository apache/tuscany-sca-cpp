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
#include "stream.hpp"
#include "string.hpp"
#include "perf.hpp"
#include "memcache.hpp"

namespace tuscany {
namespace memcache {

bool testMemCached() {
    MemCached ch(mklist<string>("localhost:11211", "localhost:11212", "localhost:11213"));
    const value k = mklist<value>("a");

    assert(hasContent(post(k, string("AAA"), ch)));
    assert(get(k, ch) == value(string("AAA")));
    assert(hasContent(put(k, string("aaa"), ch)));
    assert(get(k, ch) == value(string("aaa")));
    assert(hasContent(del(k, ch)));
    assert(!hasContent(get(k, ch)));

    return true;
}

struct getLoop {
    const value k;
    MemCached& ch;
    getLoop(const value& k, MemCached& ch) : k(k), ch(ch) {
    }
    const bool operator()() const {
        gc_scoped_pool p;
        assert(get(k, ch) == value(string("CCC")));
        return true;
    }
};

bool testGetPerf() {
    const value k = mklist<value>("c");
    MemCached ch(mklist<string>("localhost:11211", "localhost:11212", "localhost:11213"));
    assert(hasContent(post(k, string("CCC"), ch)));

    const lambda<bool()> gl = getLoop(k, ch);
    cout << "Memcached get test " << time(gl, 5, 200) << " ms" << endl;
    return true;
}

}
}

int main() {
    tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::memcache::testMemCached();
    tuscany::memcache::testGetPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
