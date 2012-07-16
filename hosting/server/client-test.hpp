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
#include "../../modules/http/http.hpp"

namespace tuscany {
namespace server {

string testURI = "http://localhost:8090";
bool testBlobs = true;

ostream* curlWriter(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const bool testGetDoc() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    {
        ostringstream os;
        const failable<list<ostream*> > r = http::get<ostream*>(curlWriter, &os, testURI + "/", ch);
        assert(hasContent(r));
        assert(contains(str(os), "HTTP/1.1 200") || contains(str(os), "HTTP/1.0 200"));
        assert(contains(str(os), "<base href=\"/\"/>"));
    }
    {
        const failable<value> r = http::getcontent(testURI + "/", ch);
        assert(hasContent(r));
        assert(contains(car(list<value>(content(r))), "<base href=\"/\"/>"));
    }
    return true;
}

struct getDocLoop {
    http::CURLSession& ch;
    getDocLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const failable<value> r = http::getcontent(testURI + "/", ch);
        assert(hasContent(r));
        assert(contains(car(list<value>(content(r))), "<base href=\"/\"/>"));
        return true;
    }
};

const bool testGetDocPerf() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    const lambda<bool()> gl = getDocLoop(ch);
    cout << "GET doc test " << time(gl, 10, 50) << " ms" << endl;
    return true;
}

struct getCompositeLoop {
    http::CURLSession& ch;
    getCompositeLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const failable<value> r = http::getcontent(testURI + "/r/Editor/composites/test", ch);
        assert(hasContent(r));
        return true;
    }
};

const bool testGetCompositePerf() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    const lambda<bool()> gl = getCompositeLoop(ch);
    cout << "GET composite test " << time(gl, 10, 50) << " ms" << endl;
    return true;
}

struct getPageLoop {
    http::CURLSession& ch;
    getPageLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const failable<value> r = http::getcontent(testURI + "/r/Editor/pages/test", ch);
        assert(hasContent(r));
        return true;
    }
};

const bool testGetPagePerf() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    const lambda<bool()> gl = getPageLoop(ch);
    cout << "GET page test " << time(gl, 10, 50) << " ms" << endl;
    return true;
}

struct getAppLoop {
    http::CURLSession& ch;
    getAppLoop(http::CURLSession& ch) : ch(ch) {
    }
    const bool operator()() const {
        const failable<value> r = http::getcontent(testURI + "/r/Editor/apps/test", ch);
        assert(hasContent(r));
        return true;
    }
};

const bool testGetAppPerf() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    const lambda<bool()> gl = getAppLoop(ch);
    cout << "GET app test " << time(gl, 10, 50) << " ms" << endl;
    return true;
}

const bool testEval() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    const failable<value> r = http::evalExpr(mklist<value>(string("echo"), string("Hello")), testURI, ch);
    assert(hasContent(r));
    assert(content(r) == string("Hello"));
    return true;
}

struct evalLoop {
    const string uri;
    http::CURLSession& ch;
    evalLoop(const string& uri, http::CURLSession& ch) : uri(uri), ch(ch) {
    }
    const bool operator()() const {
        const failable<value> r = http::evalExpr(mklist<value>(string("echo"), string("Hello")), uri, ch);
        assert(hasContent(r));
        assert(content(r) == string("Hello"));
        return true;
    }
};

const value blob(string(2048, 'A'));
const list<value> blobs = mklist(blob, blob);

struct blobEvalLoop {
    const string uri;
    http::CURLSession& ch;
    blobEvalLoop(const string& uri, http::CURLSession& ch) : uri(uri), ch(ch) {
    }
    const bool operator()() const {
        const failable<value> r = content(http::evalExpr(mklist<value>(string("echo"), blobs), uri, ch));
        assert(hasContent(r));
        assert(content(r) == blobs);
        return true;
    }
};

const bool testEvalPerf() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    const lambda<bool()> el = evalLoop(testURI, ch);
    cout << "JSON-RPC eval echo test " << time(el, 5, 200) << " ms" << endl;

    if (testBlobs) {
        const lambda<bool()> bel = blobEvalLoop(testURI, ch);
        cout << "JSON-RPC eval blob test " << time(bel, 5, 200) << " ms" << endl;
    }
    return true;
}

bool testPost() {
    gc_scoped_pool pool;
    const list<value> i = list<value>() + "content" + (list<value>() + "item"
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99")));
    const list<value> a = list<value>() + (list<value>() + "entry" 
            + (list<value>() + "title" + string("item"))
            + (list<value>() + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
    http::CURLSession ch("", "", "", "", 0);
    const failable<value> id = http::post(a, testURI, ch);
    assert(hasContent(id));
    return true;
}

struct postLoop {
    const string uri;
    const value val;
    http::CURLSession& ch;
    postLoop(const string& uri, const value& val, http::CURLSession& ch) : uri(uri), val(val), ch(ch) {
    }
    const bool operator()() const {
        const failable<value> id = http::post(val, uri, ch);
        assert(hasContent(id));
        return true;
    }
};

struct postBlobLoop {
    const string uri;
    const value val;
    http::CURLSession& ch;
    postBlobLoop(const string& uri, const value& val, http::CURLSession& ch) : uri(uri), val(val), ch(ch) {
    }
    const bool operator()() const {
        gc_scoped_pool pool;
        const failable<value> id = http::post(val, uri, ch);
        assert(hasContent(id));
        return true;
    }
};

const bool testPostPerf() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    {
        const list<value> i = list<value>() + "content" + (list<value>() + "item" 
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99")));
        const list<value> val = list<value>() + (list<value>() + "entry" 
            + (list<value>() + "title" + string("item"))
            + (list<value>() + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
        const lambda<bool()> pl = postLoop(testURI, val, ch);
        cout << "ATOMPub POST small test " << time(pl, 5, 200) << " ms" << endl;
    }
    if (testBlobs) {
        const list<value> i = list<value>() + "content" + (list<value>() + "item"
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "blob1" + blob)
            + (list<value>() + "blob2" + blob)
            + (list<value>() + "price" + string("$2.99")));
        const list<value> val = list<value>() + (list<value>() + "entry" 
            + (list<value>() + "title" + string("item"))
            + (list<value>() + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
        const lambda<bool()> pl = postBlobLoop(testURI, val, ch);
        cout << "ATOMPub POST blob test  " << time(pl, 5, 200) << " ms" << endl;
    }
    return true;
}

#ifdef WANT_THREADS

const bool postThread(const string& uri, const int count, const value& val) {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
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
    worker& w;
    const int threads;
    postThreadLoop(const lambda<bool()>& l, worker& w, const int threads) : l(l), w(w), threads(threads) {
    }
    const bool operator()() const {
        list<future<bool> > r = startPost(w, threads, l);
        checkPost(r);
        return true;
    }
};

const bool testPostThreadPerf() {
    gc_scoped_pool pool;
    const int count = 50;
    const int threads = 10;
    worker w(threads);

    const list<value> i = list<value>() + "content" + (list<value>() + "item"
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99")));
    const value val = list<value>() + (list<value>() + "entry" 
            + (list<value>() + "title" + string("item"))
            + (list<value>() + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);

    const lambda<bool()> pl= curry(lambda<bool(const string, const int, const value)>(postThread), testURI, count, val);
    const lambda<bool()> ptl = postThreadLoop(pl, w, threads);
    double t = time(ptl, 0, 1) / (threads * count);
    cout << "ATOMPub POST thread test " << t << " ms" << endl;

    return true;
}

#else

const bool postProc(const string& uri, const int count, const value& val) {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
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

const bool testPostForkPerf() {
    gc_scoped_pool pool;
    const int count = 50;
    const int procs = 10;

    const list<value> i = list<value>() + "content" + (list<value>() + "item"
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99")));
    const value val = list<value>() + (list<value>() + "entry" 
            + (list<value>() + "title" + string("item"))
            + (list<value>() + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);

    const lambda<bool()> pl= curry(lambda<bool(const string, const int, const value)>(postProc), testURI, count, val);
    const lambda<bool()> ptl = postForkLoop(pl, procs);
    double t = time(ptl, 0, 1) / (procs * count);
    cout << "ATOMPub POST fork test " << t << " ms" << endl;

    return true;
}

#endif

const bool testPut() {
    gc_scoped_pool pool;
    const list<value> i = list<value>() + "content" + (list<value>() + "item"
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99")));
    const list<value> a = list<value>() + (list<value>() + "entry" 
            + (list<value>() + "title" + string("item"))
            + (list<value>() + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
    http::CURLSession ch("", "", "", "", 0);
    value rc = content(http::put(a, testURI + "/111", ch));
    assert(rc == value(true));
    return true;
}

const bool testDel() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "", 0);
    value rc = content(http::del(testURI + "/111", ch));
    assert(rc == value(true));
    return true;
}

const bool testServer() {
    tuscany::server::testGetDoc();
#ifdef FOO
    tuscany::server::testPost();
    tuscany::server::testPut();
    tuscany::server::testDel();
    tuscany::server::testEval();
#endif
    tuscany::server::testGetDocPerf();
    tuscany::server::testGetCompositePerf();
    tuscany::server::testGetPagePerf();
    tuscany::server::testGetAppPerf();
#ifdef FOO
    tuscany::server::testPostPerf();
#ifdef WANT_THREADS
    tuscany::server::testPostThreadPerf();
#else
    tuscany::server::testPostForkPerf();
#endif
    tuscany::server::testEvalPerf();
#endif
    return true;
}

}
}
