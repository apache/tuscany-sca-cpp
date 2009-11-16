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
#include "memcached.hpp"

namespace tuscany {
namespace cache {

bool testMemCached() {
    memcached::Cache cache;
    memcached::addServer("localhost", 11311, cache);

    assert(hasValue(memcached::post("a", "AAA", cache)));
    assert(memcached::get("a", cache) == value(std::string("AAA")));
    assert(hasValue(memcached::put("a", "aaa", cache)));
    assert(memcached::get("a", cache) == value(std::string("aaa")));
    assert(hasValue(memcached::del("a", cache)));
    assert(!hasValue(memcached::get("a", cache)));

    return true;
}

const double duration(struct timeval start, struct timeval end, int count) {
    long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
    return (double)t / (double)count;
}

bool testGetLoop(const int count, memcached::Cache& cache) {
    if (count == 0)
        return true;
    assert(memcached::get("c", cache) == value(std::string("CCC")));
    return testGetLoop(count - 1, cache);
}

bool testGetPerf() {
    const int count = 50;
    struct timeval start;
    struct timeval end;
    {
        memcached::Cache cache;
        memcached::addServer("localhost", 11311, cache);
        assert(hasValue(memcached::post("c", "CCC", cache)));

        testGetLoop(5, cache);

        gettimeofday(&start, NULL);

        testGetLoop(count, cache);

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
