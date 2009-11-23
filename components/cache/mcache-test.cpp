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
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include "mcache.hpp"

namespace tuscany {
namespace cache {

bool testMemCached() {
    MemCached ch("localhost", 11311);

    assert(hasContent(post("a", "AAA", ch)));
    assert(get("a", ch) == value(std::string("AAA")));
    assert(hasContent(put("a", "aaa", ch)));
    assert(get("a", ch) == value(std::string("aaa")));
    assert(hasContent(del("a", ch)));
    assert(!hasContent(get("a", ch)));

    return true;
}

const double duration(struct timeval start, struct timeval end, int count) {
    long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
    return (double)t / (double)count;
}

bool testGetLoop(const int count, MemCached& ch) {
    if (count == 0)
        return true;
    assert(get("c", ch) == value(std::string("CCC")));
    return testGetLoop(count - 1, ch);
}

bool testGetPerf() {
    const int count = 50;
    struct timeval start;
    struct timeval end;
    {
        MemCached ch("localhost", 11311);
        assert(hasContent(post("c", "CCC", ch)));

        testGetLoop(5, ch);

        gettimeofday(&start, NULL);

        testGetLoop(count, ch);

        gettimeofday(&end, NULL);
        std::cout << "Memcached get test " << duration(start, end, count) << " ms" << std::endl;
    }
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
