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

const bool testFileDB(const string& dbname, const string& format) {
    const FileDB db(dbname, format);
    const value k = mklist<value>("a", "b");

    const list<value> a = mklist<value>(nilListValue + "ns1:a" + (nilListValue + "@xmlns:ns1" + string("http://aaa")) + (nilListValue + "text" + string("Hey!")));
    const list<value> b = mklist<value>(nilListValue + "ns1:b" + (nilListValue + "@xmlns:ns1" + string("http://bbb")) + (nilListValue + "text" + string("Hey!")));
    const list<value> c = mklist<value>(nilListValue + "ns1:c" + (nilListValue + "@xmlns:ns1" + string("http://ccc")) + (nilListValue + "text" + string("Hey!")));

    assert(hasContent(post(k, a, db)));
    assert((get(k, db)) == value(a));
    assert(hasContent(put(k, b, db)));
    assert((get(k, db)) == value(b));
    assert(hasContent(patch(k, c, db)));
    assert((get(k, db)) == value(c));
    assert(hasContent(del(k, db)));
    assert(!hasContent(get(k, db)));
    assert(hasContent(post(k, a, db)));

    return true;
}

const bool testGetPerf(const string& dbname, const string& format) {
    const FileDB db(dbname, format);

    const value k = mklist<value>("c");
    const list<value> c = mklist<value>(nilListValue + "ns1:c" + (nilListValue + "@xmlns:ns1" + string("http://ccc")) + (nilListValue + "text" + string("Hey!")));
    assert(hasContent(post(k, c, db)));

    const blambda gl = [k, c, db]() -> const bool {
        assert((get(k, db)) == value(c));
        return true;
    };
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
