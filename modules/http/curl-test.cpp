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
#include <sys/time.h>
#include <time.h>
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

std::ostringstream* curlWriter(const std::string& s, std::ostringstream* os) {
    (*os) << s;
    return os;
}

const bool testEval() {
    CURLHandle ch;
    const value val = evalExpr(mklist<value>(std::string("echo"), std::string("Hello")), "http://localhost:8091/test", ch);
    assert(val == std::string("Hello"));
    return true;
}

const bool testEvalLoop(const int count, CURLHandle& ch) {
    if (count == 0)
        return true;
    const value val = evalExpr(mklist<value>(std::string("echo"), std::string("Hello")), "http://localhost:8091/test", ch);
    assert(val == std::string("Hello"));
    return testEvalLoop(count - 1, ch);
}

const value blob(std::string(3000, 'A'));
const list<value> blobs = mklist(blob, blob, blob, blob, blob);

const bool testBlobEvalLoop(const int count, CURLHandle& ch) {
    if (count == 0)
        return true;
    const value val = evalExpr(mklist<value>(std::string("echo"), blobs), "http://localhost:8091/test", ch);
    assert(val == blobs);
    return testBlobEvalLoop(count - 1, ch);
}

const bool testEvalPerf() {
    const int count = 50;
    CURLHandle ch;
    struct timeval start;
    struct timeval end;
    {
        testEvalLoop(5, ch);

        gettimeofday(&start, NULL);

        testEvalLoop(count, ch);

        gettimeofday(&end, NULL);
        long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        std::cout << "JSON-RPC eval echo test " << (t / count) << " ms" << std::endl;
    }
    {
        testBlobEvalLoop(5, ch);

        gettimeofday(&start, NULL);

        testBlobEvalLoop(count, ch);

        gettimeofday(&end, NULL);
        long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        std::cout << "JSON-RPC eval blob test " << (t / count) << " ms" << std::endl;
    }
}

const bool testGet() {
    CURLHandle ch;
    {
        std::ostringstream os;
        const failable<list<std::ostringstream*>, std::string> r = get<std::ostringstream*>(curlWriter, &os, "http://localhost:8091", ch);
        assert(hasValue(r));
        assert(contains(os.str(), "HTTP/1.1 200 OK"));
        assert(contains(os.str(), "It works"));
    }
    {
        const failable<value, std::string> r = get("http://localhost:8091", ch);
        assert(hasValue(r));
        const value val = r;
        assert(contains(val, "It works"));
    }
    return true;
}

const bool testFeed() {
    return true;
}

bool testPost() {
    const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$2.99"));
    const list<value> a = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
    CURLHandle ch;
    value rc = post(a, "http://localhost:8091/test", ch);
    assert(rc == value(true));
    return true;
}

const bool testPostLoop(const int count, const value& val, CURLHandle& ch) {
    if (count == 0)
        return true;
    const value rc = post(val, "http://localhost:8091/test", ch);
    assert(rc == value(true));
    return testPostLoop(count - 1, val, ch);
}

const bool testPostPerf() {
    const int count = 50;
    CURLHandle ch;
    struct timeval start;
    struct timeval end;
    {
        const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$2.99"));
        const list<value> val = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
        testPostLoop(5, val, ch);

        gettimeofday(&start, NULL);

        testPostLoop(count, val, ch);

        gettimeofday(&end, NULL);
        long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        std::cout << "ATOMPub POST small test " << (t / count) << " ms" << std::endl;
    }
    {
        const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "blob1" << blob)
            << (list<value>() << "blob2" << blob)
            << (list<value>() << "blob3" << blob)
            << (list<value>() << "blob4" << blob)
            << (list<value>() << "blob5" << blob)
            << (list<value>() << "price" << std::string("$2.99"));
        const list<value> val = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
        testPostLoop(5, val, ch);

        gettimeofday(&start, NULL);

        testPostLoop(count, val, ch);

        gettimeofday(&end, NULL);
        long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        std::cout << "ATOMPub POST blob test  " << (t / count) << " ms" << std::endl;
    }
}

const bool testPut() {
    const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$2.99"));
    const list<value> a = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
    CURLHandle ch;
    value rc = put(a, "http://localhost:8091/test", ch);
    assert(rc == value(true));
    return true;
}

const bool testDel() {
    CURLHandle ch;
    value rc = del("http://localhost:8091/test/123456789", ch);
    assert(rc == value(true));
    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::http::testGet();
    tuscany::http::testPost();
    //tuscany::http::testPostPerf();
    tuscany::http::testEval();
    //tuscany::http::testEvalPerf();
    tuscany::http::testFeed();
    tuscany::http::testPut();
    tuscany::http::testDel();

    std::cout << "OK" << std::endl;

    return 0;
}
