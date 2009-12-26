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
#include <iostream>
#include <sstream>
#include <string>
#include "slist.hpp"
#include "perf.hpp"
#include "curl.hpp"

namespace tuscany {
namespace http {

const bool contains(const std::string& str, const std::string& pattern) {
    return str.find(pattern) != str.npos;
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

struct getLoop {
    CURLSession& ch;
    getLoop(CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const failable<value, std::string> r = get("http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(content(r), "It works"));
        return true;
    }
};

const bool testGetPerf() {
    CURLSession ch;
    lambda<bool()> gl = getLoop(ch);
    std::cout << "Static GET test " << time(gl, 5, 200) << " ms" << std::endl;
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
