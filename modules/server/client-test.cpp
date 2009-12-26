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
#include "parallel.hpp"
#include "perf.hpp"
#include "../http/curl.hpp"

namespace tuscany {
namespace server {

const bool contains(const std::string& str, const std::string& pattern) {
    return str.find(pattern) != str.npos;
}

std::ostringstream* curlWriter(const std::string& s, std::ostringstream* os) {
    (*os) << s;
    return os;
}

const bool testGet() {
    http::CURLSession ch;
    {
        std::ostringstream os;
        const failable<list<std::ostringstream*>, std::string> r = http::get<std::ostringstream*>(curlWriter, &os, "http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(os.str(), "HTTP/1.1 200 OK"));
        assert(contains(os.str(), "It works"));
    }
    {
        const failable<value, std::string> r = http::get("http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(content(r), "It works"));
    }
    return true;
}

struct getLoop {
    http::CURLSession ch;
    getLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const failable<value, std::string> r = get("http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(content(r), "It works"));
        return true;
    }
};

const bool testGetPerf() {
    http::CURLSession ch;
    const lambda<bool()> gl = getLoop(ch);
    std::cout << "Static GET test " << time(gl, 5, 200) << " ms" << std::endl;
    return true;
}

const bool testEval() {
    http::CURLSession ch;
    const value val = content(http::evalExpr(mklist<value>(std::string("echo"), std::string("Hello")), "http://localhost:8090/test", ch));
    assert(val == std::string("Hello"));
    return true;
}

struct evalLoop {
    http::CURLSession ch;
    evalLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const value val = content(http::evalExpr(mklist<value>(std::string("echo"), std::string("Hello")), "http://localhost:8090/test", ch));
        assert(val == std::string("Hello"));
        return true;
    }
};

const value blob(std::string(3000, 'A'));
const list<value> blobs = mklist(blob, blob, blob, blob, blob);

struct blobEvalLoop {
    http::CURLSession ch;
    blobEvalLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const value val = content(http::evalExpr(mklist<value>(std::string("echo"), blobs), "http://localhost:8090/test", ch));
        assert(val == blobs);
        return true;
    }
};

const bool testEvalPerf() {
    http::CURLSession ch;
    const lambda<bool()> el = evalLoop(ch);
    std::cout << "JSON-RPC eval echo test " << time(el, 5, 200) << " ms" << std::endl;
    const lambda<bool()> bel = blobEvalLoop(ch);
    std::cout << "JSON-RPC eval blob test " << time(bel, 5, 200) << " ms" << std::endl;
    return true;
}

bool testPost() {
    const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$2.99"));
    const list<value> a = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
    http::CURLSession ch;
    const failable<value, std::string> id = http::post(a, "http://localhost:8090/test", ch);
    assert(hasContent(id));
    return true;
}

struct postLoop {
    const value val;
    http::CURLSession ch;
    postLoop(const value& val, http::CURLSession& ch) : val(val), ch(ch) {
    }
    const bool operator()() const {
        const failable<value, std::string> id = http::post(val, "http://localhost:8090/test", ch);
        assert(hasContent(id));
        return true;
    }
};

const bool testPostPerf() {
    http::CURLSession ch;
    {
        const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$2.99"));
        const list<value> val = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
        const lambda<bool()> pl = postLoop(val, ch);
        std::cout << "ATOMPub POST small test " << time(pl, 5, 200) << " ms" << std::endl;
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
        const lambda<bool()> pl = postLoop(val, ch);
        std::cout << "ATOMPub POST blob test  " << time(pl, 5, 200) << " ms" << std::endl;
    }
    return true;
}

const bool postThread(const int count, const value& val) {
    http::CURLSession ch;
    const lambda<bool()> pl = postLoop(val, ch);
    time(pl, 0, count);
    return true;
}

const list<future<bool> > startPost(worker& w, const int threads, const lambda<bool()>& l) {
    if (threads == 0)
        return list<future<bool> >();
    return cons(submit(w, l), startPost(w, threads - 1, l));
}

const bool checkPost(const list<future<bool> >& r) {
    if (isNil(r))
        return true;
    assert(car(r) == true);
    return checkPost(cdr(r));
}

struct postThreadLoop {
    const lambda<bool()> l;
    const int threads;
    const gc_ptr<worker> w;
    postThreadLoop(const lambda<bool()>& l, const int threads) : l(l), threads(threads), w(new worker(threads)) {
    }
    const bool operator()() const {
        list<future<bool> > r = startPost(*w, threads, l);
        checkPost(r);
        return true;
    }
};

const bool testPostThreadPerf() {
    const int count = 50;
    const int threads = 10;

    const list<value> i = list<value>()
        << (list<value>() << "name" << std::string("Apple"))
        << (list<value>() << "price" << std::string("$2.99"));
    const value val = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    const lambda<bool()> pl= curry(lambda<bool(const int, const value)>(postThread), count, val);
    const lambda<bool()> ptl = postThreadLoop(pl, threads);
    double t = time(ptl, 0, 1) / (threads * count);
    std::cout << "ATOMPub POST thread test " << t << " ms" << std::endl;

    return true;
}

const bool testPut() {
    const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$2.99"));
    const list<value> a = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
    http::CURLSession ch;
    value rc = content(http::put(a, "http://localhost:8090/test/111", ch));
    assert(rc == value(true));
    return true;
}

const bool testDel() {
    http::CURLSession ch;
    value rc = content(http::del("http://localhost:8090/test/123456789", ch));
    assert(rc == value(true));
    return true;
}

const bool testEvalCpp() {
    http::CURLSession ch;
    const value val = content(http::evalExpr(mklist<value>(std::string("hello"), std::string("world")), "http://localhost:8090/cpp", ch));
    assert(val == std::string("hello world"));
    return true;
}

struct evalCppLoop {
    http::CURLSession ch;
    evalCppLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const value val = content(http::evalExpr(mklist<value>(std::string("hello"), std::string("world")), "http://localhost:8090/cpp", ch));
        assert(val == std::string("hello world"));
        return true;
    }
};

const bool testEvalCppPerf() {
    http::CURLSession ch;
    const lambda<bool()> el = evalCppLoop(ch);
    std::cout << "JSON-RPC C++ eval test " << time(el, 5, 200) << " ms" << std::endl;
    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::server::testGet();
    tuscany::server::testGetPerf();
    tuscany::server::testPost();
    tuscany::server::testPostPerf();
    tuscany::server::testPostThreadPerf();
    tuscany::server::testEval();
    tuscany::server::testEvalPerf();
    tuscany::server::testPut();
    tuscany::server::testDel();
    tuscany::server::testEvalCpp();
    tuscany::server::testEvalCppPerf();

    std::cout << "OK" << std::endl;

    return 0;
}
