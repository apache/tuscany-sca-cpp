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
 * Test JSON data conversion functions.
 */

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include "slist.hpp"
#include "curl.hpp"

namespace tuscany {
namespace http {

bool contains(const std::string& str, const std::string& pattern) {
    return str.find(pattern) != str.npos;
}

std::ostringstream* curlWriter(std::ostringstream* os, const std::string& s) {
    (*os) << s;
    return os;
}

const bool testCURL() {
    {
        std::ostringstream os;
        const failable<std::ostringstream*, int> r = get<std::ostringstream*>(curlWriter, &os, "http://localhost:9091");
        assert(hasValue(r));
        assert(contains(os.str(), "It works"));
    }
    {
        std::ostringstream os;
        const failable<list<std::string>, int> r = get("http://localhost:9091");
        assert(hasValue(r));
        write(r, os);
        assert(contains(os.str(), "It works"));
    }
    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::http::testCURL();

    std::cout << "OK" << std::endl;

    return 0;
}
