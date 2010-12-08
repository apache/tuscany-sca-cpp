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
 * Test FileDB access functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "perf.hpp"
#include "filedb.hpp"

namespace tuscany {
namespace filedb {

bool testFileDB() {
    FileDB db("tmp/testdb");
    const value k = mklist<value>("a");

    assert(hasContent(post(k, string("AAA"), db)));
    assert((get(k, db)) == value(string("AAA")));
    assert(hasContent(put(k, string("aaa"), db)));
    assert((get(k, db)) == value(string("aaa")));
    assert(hasContent(del(k, db)));
    assert(!hasContent(get(k, db)));

    return true;
}

struct getLoop {
    const value k;
    FileDB& db;
    getLoop(const value& k, FileDB& db) : k(k), db(db) {
    }
    const bool operator()() const {
        assert((get(k, db)) == value(string("CCC")));
        return true;
    }
};

bool testGetPerf() {
    const value k = mklist<value>("c");
    FileDB db("tmp/testdb");
    assert(hasContent(post(k, string("CCC"), db)));

    const lambda<bool()> gl = getLoop(k, db);
    cout << "FileDB get test " << time(gl, 5, 10000) << " ms" << endl;
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::filedb::testFileDB();
    tuscany::filedb::testGetPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
