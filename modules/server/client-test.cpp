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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "parallel.hpp"
#include "perf.hpp"
#include "../http/curl.hpp"

namespace tuscany {
namespace server {

ostream* curlWriter(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const bool testGet() {
    http::CURLSession ch;
    {
        ostringstream os;
        const failable<list<ostream*> > r = http::get<ostream*>(curlWriter, &os, "http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(str(os), "HTTP/1.1 200 OK"));
        assert(contains(str(os), "It works"));
    }
    {
        const failable<value> r = http::getcontent("http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(car(reverse(list<value>(content(r)))), "It works"));
    }
    return true;
}

struct getLoop {
    http::CURLSession ch;
    getLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const failable<value> r = http::getcontent("http://localhost:8090", ch);
        assert(hasContent(r));
        assert(contains(car(reverse(list<value>(content(r)))), "It works"));
        return true;
    }
};

const bool testGetPerf() {
    http::CURLSession ch;
    const lambda<bool()> gl = getLoop(ch);
    cout << "Static GET test " << time(gl, 5, 200) << " ms" << endl;
    return true;
}

const bool testEval(const string& uri) {
    http::CURLSession ch;
    const value val = content(http::evalExpr(mklist<value>(string("echo"), string("Hello")), uri, ch));
    assert(val == string("Hello"));
    return true;
}

const bool testEval() {
    testEval("http://localhost:8090/test");
    testEval("http://localhost:8090/cpp");
    testEval("http://localhost:8090/python");
    return true;
}

struct evalLoop {
    const string uri;
    http::CURLSession ch;
    evalLoop(const string& uri, http::CURLSession& ch) : uri(uri), ch(ch) {
    }
    const bool operator()() const {
        const value val = content(http::evalExpr(mklist<value>(string("echo"), string("Hello")), uri, ch));
        assert(val == string("Hello"));
        return true;
    }
};

const value blob(string(3000, 'A'));
const list<value> blobs = mklist(blob, blob, blob, blob, blob);

struct blobEvalLoop {
    const string uri;
    http::CURLSession ch;
    blobEvalLoop(const string& uri, http::CURLSession& ch) : uri(uri), ch(ch) {
    }
    const bool operator()() const {
        const value val = content(http::evalExpr(mklist<value>(string("echo"), blobs), uri, ch));
        assert(val == blobs);
        return true;
    }
};

const bool testEvalPerf(const string& type, const string& uri) {
    http::CURLSession ch;
    const lambda<bool()> el = evalLoop(uri, ch);
    cout << type << " JSON-RPC eval echo test " << time(el, 5, 200) << " ms" << endl;
    const lambda<bool()> bel = blobEvalLoop(uri, ch);
    cout << type << " JSON-RPC eval blob test " << time(bel, 5, 200) << " ms" << endl;
    return true;
}

const bool testEvalPerf() {
    testEvalPerf("Scheme", "http://localhost:8090/test");
    testEvalPerf("C++", "http://localhost:8090/cpp");
    testEvalPerf("Python", "http://localhost:8090/python");
    return true;
}

bool testPost(const string& uri) {
    const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99"));
    const list<value> a = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
    http::CURLSession ch;
    const failable<value> id = http::post(a, uri, ch);
    assert(hasContent(id));
    return true;
}

const bool testPost() {
    testPost("http://localhost:8090/test");
    testPost("http://localhost:8090/cpp");
    testPost("http://localhost:8090/python");
    return true;
}

struct postLoop {
    const string uri;
    const value val;
    http::CURLSession ch;
    postLoop(const string& uri, const value& val, http::CURLSession& ch) : uri(uri), val(val), ch(ch) {
    }
    const bool operator()() const {
        const failable<value> id = http::post(val, uri, ch);
        assert(hasContent(id));
        return true;
    }
};

const bool testPostPerf(const string& type, const string& uri) {
    http::CURLSession ch;
    {
        const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99"));
        const list<value> val = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
        const lambda<bool()> pl = postLoop(uri, val, ch);
        cout << type << " ATOMPub POST small test " << time(pl, 5, 200) << " ms" << endl;
    }
    {
        const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "blob1" + blob)
            + (list<value>() + "blob2" + blob)
            + (list<value>() + "blob3" + blob)
            + (list<value>() + "blob4" + blob)
            + (list<value>() + "blob5" + blob)
            + (list<value>() + "price" + string("$2.99"));
        const list<value> val = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
        const lambda<bool()> pl = postLoop(uri, val, ch);
        cout << type << " ATOMPub POST blob test  " << time(pl, 5, 200) << " ms" << endl;
    }
    return true;
}

const bool testPostPerf() {
    testPostPerf("Scheme", "http://localhost:8090/test");
    testPostPerf("C++", "http://localhost:8090/cpp");
    testPostPerf("Python", "http://localhost:8090/python");
    return true;
}

#ifdef _REENTRANT

const bool postThread(const string& uri, const int count, const value& val) {
    http::CURLSession ch;
    const lambda<bool()> pl = postLoop(uri, val, ch);
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
    postThreadLoop(const lambda<bool()>& l, const int threads) : l(l), threads(threads), w(new (gc_new<worker>()) worker(threads)) {
    }
    const bool operator()() const {
        list<future<bool> > r = startPost(*w, threads, l);
        checkPost(r);
        return true;
    }
};

const bool testPostThreadPerf(const string& type, const string& uri) {
    const int count = 50;
    const int threads = 10;

    const list<value> i = list<value>()
        + (list<value>() + "name" + string("Apple"))
        + (list<value>() + "price" + string("$2.99"));
    const value val = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    const lambda<bool()> pl= curry(lambda<bool(const string, const int, const value)>(postThread), uri, count, val);
    const lambda<bool()> ptl = postThreadLoop(pl, threads);
    double t = time(ptl, 0, 1) / (threads * count);
    cout << type << " ATOMPub POST thread test " << t << " ms" << endl;

    return true;
}

const bool testPostThreadPerf() {
    testPostThreadPerf("Scheme", "http://localhost:8090/test");
    testPostThreadPerf("C++", "http://localhost:8090/cpp");
    testPostThreadPerf("Python", "http://localhost:8090/python");
    return true;
}

#else

const bool postProc(const string& uri, const int count, const value& val) {
    http::CURLSession ch;
    const lambda<bool()> pl = postLoop(uri, val, ch);
    time(pl, 0, count);
    return true;
}

const list<pid_t> startPost(const int procs, const lambda<bool()>& l) {
    if (procs == 0)
        return list<pid_t>();
    pid_t pid = fork();
    if (pid == 0) {
        assert(l() == true);
        exit(0);
    }
    return cons(pid, startPost(procs - 1, l));
}

const bool checkPost(const list<pid_t>& r) {
    if (isNil(r))
        return true;
    int status;
    waitpid(car(r), &status, 0);
    assert(status == 0);
    return checkPost(cdr(r));
}

struct postForkLoop {
    const lambda<bool()> l;
    const int procs;
    postForkLoop(const lambda<bool()>& l, const int procs) : l(l), procs(procs) {
    }
    const bool operator()() const {
        list<pid_t> r = startPost(procs, l);
        checkPost(r);
        return true;
    }
};

const bool testPostForkPerf(const string& type, const string& uri) {
    const int count = 50;
    const int procs = 10;

    const list<value> i = list<value>()
        + (list<value>() + "name" + string("Apple"))
        + (list<value>() + "price" + string("$2.99"));
    const value val = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    const lambda<bool()> pl= curry(lambda<bool(const string, const int, const value)>(postProc), uri, count, val);
    const lambda<bool()> ptl = postForkLoop(pl, procs);
    double t = time(ptl, 0, 1) / (procs * count);
    cout << "ATOMPub POST fork test " << t << " ms" << endl;

    return true;
}

const bool testPostForkPerf() {
    testPostForkPerf("Scheme", "http://localhost:8090/test");
    //testPostForkPerf("C++", "http://localhost:8090/cpp");
    testPostForkPerf("Python", "http://localhost:8090/python");
    return true;
}

#endif

const bool testPut(const string& uri) {
    const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99"));
    const list<value> a = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
    http::CURLSession ch;
    value rc = content(http::put(a, uri, ch));
    assert(rc == value(true));
    return true;
}

const bool testPut() {
    testPut("http://localhost:8090/test/111");
    testPut("http://localhost:8090/cpp/111");
    testPut("http://localhost:8090/python/111");
    return true;
}

const bool testDel(const string& uri) {
    http::CURLSession ch;
    value rc = content(http::del(uri, ch));
    assert(rc == value(true));
    return true;
}

const bool testDel() {
    testDel("http://localhost:8090/test/123456789");
    testDel("http://localhost:8090/cpp/123456789");
    testDel("http://localhost:8090/python/123456789");
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::server::testGet();
    tuscany::server::testPost();
    tuscany::server::testPut();
    tuscany::server::testDel();
    tuscany::server::testEval();
    tuscany::server::testGetPerf();
    tuscany::server::testPostPerf();
#ifdef _REENTRANT
    tuscany::server::testPostThreadPerf();
#else
    tuscany::server::testPostForkPerf();
#endif
    tuscany::server::testEvalPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
