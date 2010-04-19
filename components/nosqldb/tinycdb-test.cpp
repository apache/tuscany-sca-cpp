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
 * Test TinyCDB access functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "perf.hpp"
#include "tinycdb.hpp"

namespace tuscany {
namespace tinycdb {

bool testTinyCDB() {
    TinyCDB cdb("tmp/test.cdb");
    const value k = mklist<value>("a");

    assert(hasContent(post(k, string("AAA"), cdb)));
    assert((get(k, cdb)) == value(string("AAA")));
    assert(hasContent(put(k, string("aaa"), cdb)));
    assert((get(k, cdb)) == value(string("aaa")));
    assert(hasContent(del(k, cdb)));
    assert(!hasContent(get(k, cdb)));

    return true;
}

struct getLoop {
    const value k;
    TinyCDB& cdb;
    getLoop(const value& k, TinyCDB& cdb) : k(k), cdb(cdb) {
    }
    const bool operator()() const {
        assert((get(k, cdb)) == value(string("CCC")));
        return true;
    }
};

bool testGetPerf() {
    const value k = mklist<value>("c");
    TinyCDB cdb("tmp/test.cdb");
    assert(hasContent(post(k, string("CCC"), cdb)));

    const lambda<bool()> gl = getLoop(k, cdb);
    cout << "TinyCDB get test " << time(gl, 5, 100000) << " ms" << endl;
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::tinycdb::testTinyCDB();
    tuscany::tinycdb::testGetPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
