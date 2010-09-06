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
 * Test chat component.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "list.hpp"
#include "element.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "perf.hpp"
#include "parallel.hpp"
#include "../../modules/http/http.hpp"
#include "xmpp.hpp"

namespace tuscany {
namespace chat {

const value jid1("sca1@localhost");
const value pass1("sca1");
const value jid2("sca2@localhost");
const value pass2("sca2");
const value jid3("sca3@localhost");
const value pass3("sca3");

const list<value> item = list<value>()
        + (list<value>() + "name" + string("Apple"))
        + (list<value>() + "price" + string("$2.99"));
const list<value> entry = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), item);

worker w(2);
bool received;

const failable<bool> listener(const value& from, const value& val, unused XMPPClient& xc) {
    assert(contains(from, "sca2@localhost"));
    assert(val == entry);
    received = true;
    return false;
}

struct subscribe {
    XMPPClient& xc;
    subscribe(XMPPClient& xc) : xc(xc) {
    }
    const failable<bool> operator()() const {
        const lambda<failable<bool>(const value&, const value&, XMPPClient&)> l(listener);
        listen(l, xc);
        return true;
    }
};

bool testListen() {
    received = false;
    XMPPClient& xc = *(new (gc_new<XMPPClient>()) XMPPClient(jid3, pass3));
    const failable<bool> c = connect(xc);
    assert(hasContent(c));
    const lambda<failable<bool>()> subs = subscribe(xc);
    submit(w, subs);
    return true;
}

bool testPost() {
    gc_scoped_pool pool;
    http::CURLSession ch("", "", "");
    const failable<value> id = http::post(entry, "http://localhost:8090/print-sender/sca2@localhost", ch);
    assert(hasContent(id));
    return true;
}

bool testReceived() {
    shutdown(w);
    assert(received == true);
    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::chat::testListen();
    tuscany::chat::testPost();
    tuscany::chat::testReceived();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
