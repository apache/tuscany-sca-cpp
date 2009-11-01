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
#include <iostream>
#include <string>
#include "cache.hpp"

namespace tuscany {
namespace cache {

bool testCache() {
    Cache cache;
    addServer("localhost", 11311, cache);

    assert(hasValue(post("a", "AAA", cache)));
    assert(get("a", cache) == value(std::string("AAA")));
    assert(hasValue(put("a", "aaa", cache)));
    assert(get("a", cache) == value(std::string("aaa")));
    assert(hasValue(del("a", cache)));
    assert(!hasValue(get("a", cache)));

    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::cache::testCache();

    std::cout << "OK" << std::endl;

    return 0;
}
