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
 * Test queue component.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "list.hpp"
#include "element.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "perf.hpp"
#include "../../modules/http/http.hpp"
#include "qpid.hpp"

// Ignore conversion issues and redundant declarations in Qpid headers
#ifdef WANT_MAINTAINER_MODE
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif

namespace tuscany {
namespace queue {

const value key(mklist<value>(string("report")));
const string qname("reportq");

const list<value> item = list<value>() + "content" + (list<value>() + "item" 
    + (list<value>() + "name" + string("Apple"))
    + (list<value>() + "price" + string("$2.99")));
const list<value> entry = list<value>() + (list<value>() + "entry" 
    + (list<value>() + "title" + string("item"))
    + (list<value>() + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
    + item);

bool testDeclareQueue() {
    QpidConnection qc;
    QpidSession qs(qc);
    const failable<bool> r = declareQueue(key, qname, qs);
    assert(hasContent(r));
    return true;
}

const bool listener(const value& k, const value& v) {
    cerr << "k " << k << " v " << v << endl;
    assert(k == key);
    assert(v == entry);
    return false;
}

bool testListen() {
    QpidConnection qc;
    QpidSession qs(qc);
    QpidSubscription qsub(qs);
    const lambda<bool(const value&, const value&)> l(listener);
    listen(qname, l, qsub);
    return true;
}

bool testPost() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "", "");
    const failable<value> id = http::post(entry, "http://localhost:8090/print-sender", ch);
    assert(hasContent(id));
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::queue::testDeclareQueue();
    tuscany::queue::testPost();
    tuscany::queue::testListen();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
