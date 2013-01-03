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
#include "../http/http.hpp"

namespace tuscany {
namespace server {

gc_mutable_ref<string> testURI = string("http://localhost:8090/scheme");

bool testBlobs = true;

ostream* curlWriter(const string& s, ostream* os) {
    (*os) << s;
    return os;
}

const bool testGet() {
    const gc_scoped_pool pool;
    const http::CURLSession ch("", "", "", "", 0);
    {
        ostringstream os;
        const failable<list<ostream*> > r = http::get<ostream*>(curlWriter, &os, "http://localhost:8090/index.html", ch);
        assert(hasContent(r));
        assert(contains(str(os), "HTTP/1.1 200") || contains(str(os), "HTTP/1.0 200"));
        assert(contains(str(os), "It works"));
    }
    {
        const failable<value> r = http::getcontent("http://localhost:8090/index.html", ch);
        assert(hasContent(r));
        assert(contains(car(reverse(list<value>(content(r)))), "It works"));
    }
    return true;
}

const bool testGetPerf() {
    const gc_scoped_pool pool;
    const http::CURLSession ch("", "", "", "", 0);
    const blambda gl = [ch]() -> const bool {
        const failable<value> r = http::getcontent("http://localhost:8090/index.html", ch);
        assert(hasContent(r));
        assert(contains(car(reverse(list<value>(content(r)))), "It works"));
        return true;
    };
    cout << "Static GET test " << time(gl, 5, 200) << " ms" << endl;
    return true;
}

const bool testEval() {
    const gc_scoped_pool pool;
    const http::CURLSession ch("", "", "", "", 0);
    const failable<value> r = http::evalExpr(mklist<value>(string("echo"), string("Hello")), testURI, ch);
    assert(hasContent(r));
    assert(content(r) == string("Hello"));
    return true;
}

const value blob(string(2048, 'A'));
const list<value> blobs = mklist(blob, blob);

const bool testEvalPerf() {
    const gc_scoped_pool pool;
    const http::CURLSession ch("", "", "", "", 0);
    const blambda el = [ch]() -> const bool {
        const failable<value> r = http::evalExpr(mklist<value>(string("echo"), string("Hello")), testURI, ch);
        assert(hasContent(r));
        assert(content(r) == string("Hello"));
        return true;
    };
    cout << "JSON-RPC eval echo test " << time(el, 5, 200) << " ms" << endl;

    if (testBlobs) {
        const blambda bel = [ch]() -> const bool {
            const failable<value> r = content(http::evalExpr(mklist<value>(string("echo"), blobs), testURI, ch));
            assert(hasContent(r));
            assert(content(r) == blobs);
            return true;
        };
        cout << "JSON-RPC eval blob test " << time(bel, 5, 200) << " ms" << endl;
    }
    return true;
}

const bool testPost() {
    const gc_scoped_pool pool;
    const list<value> i = nilListValue + "content" + (nilListValue + "item"
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
    const list<value> a = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
    const http::CURLSession ch("", "", "", "", 0);
    const failable<value> id = http::post(a, testURI, ch);
    assert(hasContent(id));
    return true;
}

const bool testPostPerf() {
    const gc_scoped_pool pool;
    const http::CURLSession ch("", "", "", "", 0);
    {
        const list<value> i = nilListValue + "content" + (nilListValue + "item" 
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
        const list<value> val = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
        const blambda pl = [val, ch]() -> const bool {
            const failable<value> id = http::post(val, testURI, ch);
            assert(hasContent(id));
            return true;
        };
        cout << "ATOMPub POST small test " << time(pl, 5, 200) << " ms" << endl;
    }
    if (testBlobs) {
        const list<value> i = nilListValue + "content" + (nilListValue + "item"
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "blob1" + blob)
            + (nilListValue + "blob2" + blob)
            + (nilListValue + "price" + string("$2.99")));
        const list<value> val = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
        const blambda pl = [val, ch]() -> const bool {
            const gc_scoped_pool pool;
            const failable<value> id = http::post(val, testURI, ch);
            assert(hasContent(id));
            return true;
        };
        cout << "ATOMPub POST blob test  " << time(pl, 5, 200) << " ms" << endl;
    }
    return true;
}

const blambda mkpostLoop(const string& uri, const value& val, const http::CURLSession& ch) {
    return [uri, val, ch]() -> const bool {
        const failable<value> id = http::post(val, uri, ch);
        assert(hasContent(id));
        return true;
    };
}

#ifdef WANT_THREADS

const bool postThread(const string& uri, const int count, const value& val) {
    const gc_scoped_pool pool;
    const http::CURLSession ch("", "", "", "", 0);
    const blambda pl = mkpostLoop(uri, val, ch);
    time(pl, 0, count);
    return true;
}

const list<future<bool> > startPost(worker& w, const int threads, const blambda& l) {
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

const bool testPostThreadPerf() {
    const gc_scoped_pool pool;
    const int count = 50;
    const int threads = 10;
    worker w(threads);

    const list<value> i = nilListValue + "content" + (nilListValue + "item"
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
    const value val = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);

    const blambda pl= curry(lambda<const bool(const string, const int, const value)>(postThread), (const string)testURI, count, val);
    const blambda ptl = [pl, &w, threads]() -> const bool {
        list<future<bool> > r = startPost(w, threads, pl);
        checkPost(r);
        return true;
    };
    const double t = time(ptl, 0, 1) / (threads * count);
    cout << "ATOMPub POST thread test " << t << " ms" << endl;

    return true;
}

#else

const bool postProc(const string& uri, const int count, const value& val) {
    const gc_scoped_pool pool;
    const http::CURLSession ch("", "", "", "", 0);
    const blambda pl = mkpostLoop(uri, val, ch);
    time(pl, 0, count);
    return true;
}

const list<pid_t> startPost(const int procs, const blambda& l) {
    if (procs == 0)
        return list<pid_t>();
    const pid_t pid = fork();
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

const bool testPostForkPerf() {
    const gc_scoped_pool pool;
    const int count = 50;
    const int procs = 10;

    const list<value> i = nilListValue + "content" + (nilListValue + "item"
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
    const value val = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);

    const blambda pl= curry(lambda<const bool(const string, const int, const value)>(postProc), testURI, count, val);
    const blambda ptl = [pl, procs]() -> const bool {
        list<pid_t> r = startPost(procs, l);
        checkPost(r);
        return true;
    };
    const double t = time(ptl, 0, 1) / (procs * count);
    cout << "ATOMPub POST fork test " << t << " ms" << endl;

    return true;
}

#endif

const bool testPut() {
    const gc_scoped_pool pool;
    const list<value> i = nilListValue + "content" + (nilListValue + "item"
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
    const list<value> a = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
    const http::CURLSession ch("", "", "", "", 0);
    const value rc = content(http::put(a, testURI + "/111", ch));
    assert(rc == trueValue);
    return true;
}

const bool testPatch() {
    const gc_scoped_pool pool;
    const list<value> i = nilListValue + "content" + (nilListValue + "item"
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
    const list<value> a = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);
    const http::CURLSession ch("", "", "", "", 0);
    const value rc = content(http::patch(a, testURI + "/111", ch));
    assert(rc == trueValue);
    return true;
}

const bool testDel() {
    const gc_scoped_pool pool;
    const http::CURLSession ch("", "", "", "", 0);
    const value rc = content(http::del(testURI + "/111", ch));
    assert(rc == trueValue);
    return true;
}

const bool testServer() {
    tuscany::server::testGet();
    tuscany::server::testPost();
    tuscany::server::testPut();
    tuscany::server::testPatch();
    tuscany::server::testDel();
    tuscany::server::testEval();
    tuscany::server::testGetPerf();
    tuscany::server::testPostPerf();
#ifdef WANT_THREADS
    tuscany::server::testPostThreadPerf();
#else
    tuscany::server::testPostForkPerf();
#endif
    tuscany::server::testEvalPerf();
    return true;
}

}
}
