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

bool testFileDB(const string& dbname, const string& format) {
    FileDB db(dbname, format);
    const value k = mklist<value>("a", "b");

    const list<value> a = mklist<value>(list<value>() + "ns1:a" + (list<value>() + "@xmlns:ns1" + string("http://aaa")) + (list<value>() + "text" + string("Hey!")));
    const list<value> b = mklist<value>(list<value>() + "ns1:b" + (list<value>() + "@xmlns:ns1" + string("http://bbb")) + (list<value>() + "text" + string("Hey!")));

    assert(hasContent(post(k, a, db)));
    assert((get(k, db)) == value(a));
    assert(hasContent(put(k, b, db)));
    assert((get(k, db)) == value(b));
    assert(hasContent(del(k, db)));
    assert(!hasContent(get(k, db)));
    assert(hasContent(post(k, a, db)));

    return true;
}

struct getLoop {
    const value k;
    FileDB& db;
    const list<value> c;
    getLoop(const value& k, FileDB& db) : k(k), db(db),
        c(mklist<value>(list<value>() + "ns1:c" + (list<value>() + "@xmlns:ns1" + string("http://ccc")) + (list<value>() + "text" + string("Hey!")))) {
    }
    const bool operator()() const {
        assert((get(k, db)) == value(c));
        return true;
    }
};

bool testGetPerf(const string& dbname, const string& format) {
    FileDB db(dbname, format);

    const value k = mklist<value>("c");
    const list<value> c = mklist<value>(list<value>() + "ns1:c" + (list<value>() + "@xmlns:ns1" + string("http://ccc")) + (list<value>() + "text" + string("Hey!")));
    assert(hasContent(post(k, c, db)));

    const lambda<bool()> gl = getLoop(k, db);
    cout << "FileDB get test " << time(gl, 5, 5000) << " ms" << endl;
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::filedb::testFileDB("tmp/schemedb", "scheme");
    tuscany::filedb::testGetPerf("tmp/schemedb", "scheme");
    tuscany::filedb::testFileDB("tmp/xmldb", "xml");
    tuscany::filedb::testGetPerf("tmp/xmldb", "xml");
    tuscany::filedb::testFileDB("tmp/jsondb", "json");
    tuscany::filedb::testGetPerf("tmp/jsondb", "json");

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
