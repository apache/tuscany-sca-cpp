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
 * Test PostgreSQL hot standby support.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "perf.hpp"
#include "pgsql.hpp"

namespace tuscany {
namespace pgsql {

bool testPGSql() {
    PGSql wpg("host=localhost port=6432 dbname=db", "test");
    PGSql rpg("host=localhost port=6433 dbname=db", "test");
    const value k = mklist<value>("a");

    assert(hasContent(post(k, string("AAA"), wpg)));
    sleep(1);
    assert((get(k, rpg)) == value(string("AAA")));
    assert(hasContent(put(k, string("aaa"), wpg)));
    sleep(1);
    assert((get(k, rpg)) == value(string("aaa")));
    assert(hasContent(del(k, wpg)));
    sleep(1);
    assert(!hasContent(get(k, rpg)));

    return true;
}

struct getLoop {
    const value k;
    PGSql& pg;
    getLoop(const value& k, PGSql& pg) : k(k), pg(pg) {
    }
    const bool operator()() const {
        assert((get(k, pg)) == value(string("CCC")));
        return true;
    }
};

bool testGetPerf() {
    const value k = mklist<value>("c");
    PGSql wpg("host=localhost port=6432 dbname=db", "test");
    PGSql rpg("host=localhost port=6433 dbname=db", "test");
    assert(hasContent(post(k, string("CCC"), wpg)));
    sleep(1);

    const lambda<bool()> gl = getLoop(k, rpg);
    cout << "PGSql get test " << time(gl, 5, 200) << " ms" << endl;
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::pgsql::testPGSql();
    tuscany::pgsql::testGetPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
