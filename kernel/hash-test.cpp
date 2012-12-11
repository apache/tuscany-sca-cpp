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
 * Test hash functions.
 */

#include <assert.h>
#include "string.hpp"
#include "hash.hpp"
#include "perf.hpp"

namespace tuscany {

const bool testCrc32hash() {
    const string key("This is a test key");
    const unsigned int h = crc32hash(c_str(key), length(key));
    assert(h != 0);
    return true;
}

const bool testTimes33hash() {
    const string key("This is a test key");
    const unsigned int h = times33hash(c_str(key), length(key));
    assert(h != 0);
    return true;
}

const bool testMurmurhash() {
    const string key("This is a test key");
    const unsigned int h = murmurhash(c_str(key), length(key));
    assert(h != 0);
    return true;
}

const bool testPortablemurmurhash() {
    const string key("This is a test key");
    const unsigned int h = portablemurmurhash(c_str(key), length(key));
    assert(h != 0);
    return true;
}

const bool testHashPerf() {
    const string key("This is a test key");
    const int count = 100000;

    const blambda crc32 = [key]() -> const bool {
        const unsigned int h = crc32hash(c_str(key), length(key));
        assert(h != 0);
        return true;
    };
    cout << "crc32hash test " << time(crc32, 5, count) << " ms" << endl;

    const blambda times33 = [key]() -> const bool {
        const unsigned int h = times33hash(c_str(key), length(key));
        assert(h != 0);
        return true;
    };
    cout << "times33hash test " << time(times33, 5, count) << " ms" << endl;

    const blambda murmur = [key]() -> const bool {
        const unsigned int h = murmurhash(c_str(key), length(key));
        assert(h != 0);
        return true;
    };
    cout << "murmurhash test " << time(murmur, 5, count) << " ms" << endl;

    const blambda portablemurmur = [key]() -> const bool {
        const unsigned int h = portablemurmurhash(c_str(key), length(key));
        assert(h != 0);
        return true;
    };
    cout << "portable murmurhash test " << time(portablemurmur, 5, count) << " ms" << endl;

    return true;
}

}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testCrc32hash();
    tuscany::testTimes33hash();
    tuscany::testMurmurhash();
    tuscany::testPortablemurmurhash();
    tuscany::testHashPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
