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

bool testCrc32hash() {
    const string key("This is a test key");
    unsigned int h = crc32hash(c_str(key), length(key));
    assert(h != 0);
    return true;
}

bool testTimes33hash() {
    const string key("This is a test key");
    unsigned int h = times33hash(c_str(key), length(key));
    assert(h != 0);
    return true;
}

bool testMurmurhash() {
    const string key("This is a test key");
    unsigned int h = murmurhash(c_str(key), length(key));
    assert(h != 0);
    return true;
}

bool testPortablemurmurhash() {
    const string key("This is a test key");
    unsigned int h = portablemurmurhash(c_str(key), length(key));
    assert(h != 0);
    return true;
}

struct crc32hashTest {
    const string key;
    crc32hashTest(const string& key) : key(key) {
    }
    bool operator()() const {
        unsigned int h = crc32hash(c_str(key), length(key));
        assert(h != 0);
        return true;
    }
};

struct times33hashTest {
    const string key;
    times33hashTest(const string& key) : key(key) {
    }
    bool operator()() const {
        unsigned int h = times33hash(c_str(key), length(key));
        assert(h != 0);
        return true;
    }
};

struct murmurhashTest {
    const string key;
    murmurhashTest(const string& key) : key(key) {
    }
    bool operator()() const {
        unsigned int h = murmurhash(c_str(key), length(key));
        assert(h != 0);
        return true;
    }
};

struct portablemurmurhashTest {
    const string key;
    portablemurmurhashTest(const string& key) : key(key) {
    }
    bool operator()() const {
        unsigned int h = portablemurmurhash(c_str(key), length(key));
        assert(h != 0);
        return true;
    }
};

bool testHashPerf() {
    const string key("This is a test key");
    const int count = 100000;

    const lambda<bool()> crc32 = crc32hashTest(key);
    cout << "crc32hash test " << time(crc32, 5, count) << " ms" << endl;
    const lambda<bool()> times33 = times33hashTest(key);
    cout << "times33hash test " << time(times33, 5, count) << " ms" << endl;
    const lambda<bool()> murmur = murmurhashTest(key);
    cout << "murmurhash test " << time(murmur, 5, count) << " ms" << endl;
    const lambda<bool()> portablemurmur = portablemurmurhashTest(key);
    cout << "portable murmurhash test " << time(portablemurmur, 5, count) << " ms" << endl;

    return true;
}

}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testCrc32hash();
    tuscany::testTimes33hash();
    tuscany::testMurmurhash();
    tuscany::testPortablemurmurhash();
    tuscany::testHashPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
