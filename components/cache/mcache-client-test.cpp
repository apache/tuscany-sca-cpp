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
 * Test Memcached access functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"

#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "perf.hpp"
#include "../../modules/http/curl.hpp"

namespace tuscany {
namespace cache {

const string url("http://localhost:8090/mcache");

bool testCache() {
    http::CURLSession cs;

    const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$2.99"));
    const list<value> a = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    const failable<value> id = http::post(a, url, cs);
    assert(hasContent(id));
    {
        const failable<value> val = http::get(url + "/" + content(id), cs);
        assert(hasContent(val));
        assert(content(val) == a);
    }

    const list<value> j = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$3.55"));
    const list<value> b = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), j);

    {
        const failable<value> r = http::put(b, url + "/" + content(id), cs);
        assert(hasContent(r));
        assert(content(r) == value(true));
    }
    {
        const failable<value> val = http::get(url + "/" + content(id), cs);
        assert(hasContent(val));
        assert(content(val) == b);
    }
    {
        const failable<value> r = http::del(url + "/" + content(id), cs);
        assert(hasContent(r));
        assert(content(r) == value(true));
    }
    {
        const failable<value> val = http::get(url + "/" + content(id), cs);
        assert(!hasContent(val));
    }

    return true;
}

struct getLoop {
    const value id;
    const value entry;
    http::CURLSession cs;
    getLoop(const value& id, const value& entry, http::CURLSession cs) : id(id), entry(entry), cs(cs) {
    }
    const bool operator()() const {
        const failable<value> val = http::get(url + "/" + id, cs);
        assert(hasContent(val));
        assert(content(val) == entry);
        return true;
    }
};

bool testGetPerf() {
    const list<value> i = list<value>()
            + (list<value>() + "name" + string("Apple"))
            + (list<value>() + "price" + string("$4.55"));
    const value a = mklist<value>(string("item"), string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    http::CURLSession cs;
    const failable<value> id = http::post(a, url, cs);
    assert(hasContent(id));

    const lambda<bool()> gl = getLoop(content(id), a, cs);
    cout << "Cache get test " << time(gl, 5, 200) << " ms" << endl;

    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::cache::testCache();
    tuscany::cache::testGetPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
