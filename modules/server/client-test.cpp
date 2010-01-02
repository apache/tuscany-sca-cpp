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

const bool testEval() {
    http::CURLSession ch;
    const value val = content(http::evalExpr(mklist<value>(string("echo"), string("Hello")), "http://localhost:8090/test", ch));
    assert(val == string("Hello"));
    return true;
}

struct evalLoop {
    http::CURLSession ch;
    evalLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const value val = content(http::evalExpr(mklist<value>(string("echo"), string("Hello")), "http://localhost:8090/test", ch));
        assert(val == string("Hello"));
        return true;
    }
};

const value blob(string(3000, 'A'));
const list<value> blobs = mklist(blob, blob, blob, blob, blob);

struct blobEvalLoop {
    http::CURLSession ch;
    blobEvalLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const value val = content(http::evalExpr(mklist<value>(string("echo"), blobs), "http://localhost:8090/test", ch));
        assert(val == blobs);
        return true;
    }
};

const bool testEvalPerf() {
    http::CURLSession ch;
    const lambda<bool()> el = evalLoop(ch);
    cout << "JSON-RPC eval echo test " << time(el, 5, 200) << " ms" << endl;
    const lambda<bool()> bel = blobEvalLoop(ch);
    cout << "JSON-RPC eval blob test " << time(bel, 5, 200) << " ms" << endl;
    return true;
}

bool testPost() {
    const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99"));
    const list<value> a = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
    http::CURLSession ch;
    const failable<value> id = http::post(a, "http://localhost:8090/test", ch);
    assert(hasContent(id));
    return true;
}

struct postLoop {
    const value val;
    http::CURLSession ch;
    postLoop(const value& val, http::CURLSession& ch) : val(val), ch(ch) {
    }
    const bool operator()() const {
        const failable<value> id = http::post(val, "http://localhost:8090/test", ch);
        assert(hasContent(id));
        return true;
    }
};

const bool testPostPerf() {
    http::CURLSession ch;
    {
        const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99"));
        const list<value> val = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
        const lambda<bool()> pl = postLoop(val, ch);
        cout << "ATOMPub POST small test " << time(pl, 5, 200) << " ms" << endl;
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
        const lambda<bool()> pl = postLoop(val, ch);
        cout << "ATOMPub POST blob test  " << time(pl, 5, 200) << " ms" << endl;
    }
    return true;
}

#ifdef _REENTRANT

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
    postThreadLoop(const lambda<bool()>& l, const int threads) : l(l), threads(threads), w(new (gc_new<worker>()) worker(threads)) {
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
        + (list<value>() + "name" + string("Apple"))
        + (list<value>() + "price" + string("$2.99"));
    const value val = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    const lambda<bool()> pl= curry(lambda<bool(const int, const value)>(postThread), count, val);
    const lambda<bool()> ptl = postThreadLoop(pl, threads);
    double t = time(ptl, 0, 1) / (threads * count);
    cout << "ATOMPub POST thread test " << t << " ms" << endl;

    return true;
}

#else

const bool postProc(const int count, const value& val) {
    http::CURLSession ch;
    const lambda<bool()> pl = postLoop(val, ch);
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

const bool testPostForkPerf() {
    const int count = 50;
    const int procs = 10;

    const list<value> i = list<value>()
        + (list<value>() + "name" + string("Apple"))
        + (list<value>() + "price" + string("$2.99"));
    const value val = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    const lambda<bool()> pl= curry(lambda<bool(const int, const value)>(postProc), count, val);
    const lambda<bool()> ptl = postForkLoop(pl, procs);
    double t = time(ptl, 0, 1) / (procs * count);
    cout << "ATOMPub POST fork test " << t << " ms" << endl;

    return true;
}

#endif

const bool testPut() {
    const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99"));
    const list<value> a = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);
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
    const value val = content(http::evalExpr(mklist<value>(string("hello"), string("world")), "http://localhost:8090/cpp", ch));
    assert(val == string("hello world"));
    return true;
}

struct evalCppLoop {
    http::CURLSession ch;
    evalCppLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const value val = content(http::evalExpr(mklist<value>(string("hello"), string("world")), "http://localhost:8090/cpp", ch));
        assert(val == string("hello world"));
        return true;
    }
};

const bool testEvalCppPerf() {
    http::CURLSession ch;
    const lambda<bool()> el = evalCppLoop(ch);
    cout << "JSON-RPC C++ eval test " << time(el, 5, 200) << " ms" << endl;
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::server::testGet();
    tuscany::server::testGetPerf();
    tuscany::server::testPost();
    tuscany::server::testPostPerf();
#ifdef _REENTRANT
    tuscany::server::testPostThreadPerf();
#else
    tuscany::server::testPostForkPerf();
#endif
    tuscany::server::testEval();
    tuscany::server::testEvalPerf();
    tuscany::server::testPut();
    tuscany::server::testDel();
    tuscany::server::testEvalCpp();
    tuscany::server::testEvalCppPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
