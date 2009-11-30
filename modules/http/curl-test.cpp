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
 * Test HTTP client functions.
 */

#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <string>
#include "slist.hpp"
#include "curl.hpp"

namespace tuscany {
namespace http {

const bool contains(const std::string& str, const std::string& pattern) {
    return str.find(pattern) != str.npos;
}

const double duration(struct timeval start, struct timeval end, int count) {
    long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
    return (double)t / (double)count;
}

std::ostringstream* curlWriter(const std::string& s, std::ostringstream* os) {
    (*os) << s;
    return os;
}

const bool testGet() {
    CURLSession ch;
    {
        std::ostringstream os;
        const failable<list<std::ostringstream*>, std::string> r = get<std::ostringstream*>(curlWriter, &os, "http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(os.str(), "HTTP/1.1 200 OK"));
        assert(contains(os.str(), "It works"));
    }
    {
        const failable<value, std::string> r = get("http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(content(r), "It works"));
    }
    return true;
}

const bool testGetLoop(const int count, CURLSession& ch) {
    if (count == 0)
        return true;
    const failable<value, std::string> r = get("http://localhost:8090", ch);
    assert(hasContent(r));
    assert(contains(content(r), "It works"));
    return testGetLoop(count - 1, ch);
}

const bool testGetPerf() {
    const int count = 50;
    CURLSession ch;
    struct timeval start;
    struct timeval end;
    {
        testGetLoop(5, ch);

        gettimeofday(&start, NULL);

        testGetLoop(count, ch);

        gettimeofday(&end, NULL);
        std::cout << "Static GET test " << duration(start, end, count) << " ms" << std::endl;
    }
    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::http::testGet();
    tuscany::http::testGetPerf();

    std::cout << "OK" << std::endl;

    return 0;
}
