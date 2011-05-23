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
#include "stream.hpp"
#include "string.hpp"
#include "perf.hpp"
#include "http.hpp"

namespace tuscany {
namespace http {

string testURI = "http://localhost:8090";

ostream* curlWriter(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const bool testGet() {
    CURLSession ch("", "", "", "");
    {
        ostringstream os;
        const failable<list<ostream*> > r = get<ostream*>(curlWriter, &os, testURI, ch);
        assert(hasContent(r));
        assert(contains(str(os), "HTTP/1.1 200 OK"));
        assert(contains(str(os), "It works"));
    }
    {
        const failable<value> r = getcontent(testURI, ch);
        assert(hasContent(r));
        assert(contains(car(reverse(list<value>(content(r)))), "It works"));
    }
    return true;
}

struct getLoop {
    CURLSession& ch;
    getLoop(CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const failable<value> r = getcontent(testURI, ch);
        assert(hasContent(r));
        assert(contains(car(reverse(list<value>(content(r)))), "It works"));
        return true;
    }
};

const bool testGetPerf() {
    CURLSession ch("", "", "", "");
    lambda<bool()> gl = getLoop(ch);
    cout << "Static GET test " << time(gl, 5, 200) << " ms" << endl;
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;
    tuscany::http::testURI = tuscany::string("http://") + tuscany::http::hostName() + ":8090";

    tuscany::http::testGet();
    tuscany::http::testGetPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
