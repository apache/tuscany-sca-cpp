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
#include <iostream>
#include <string>

#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "perf.hpp"
#include "../../modules/http/curl.hpp"

namespace tuscany {
namespace cache {

const std::string url("http://localhost:8090/mcache");

bool testCache() {
    http::CURLSession cs;

    const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$2.99"));
    const list<value> a = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    const failable<value, std::string> id = http::post(a, url, cs);
    assert(hasContent(id));
    {
        const failable<value, std::string> val = http::get(url + "/" + std::string(content(id)), cs);
        assert(hasContent(val));
        assert(content(val) == a);
    }

    const list<value> j = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$3.55"));
    const list<value> b = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), j);

    {
        const failable<value, std::string> r = http::put(b, url + "/" + std::string(content(id)), cs);
        assert(hasContent(r));
        assert(content(r) == value(true));
    }
    {
        const failable<value, std::string> val = http::get(url + "/" + std::string(content(id)), cs);
        assert(hasContent(val));
        assert(content(val) == b);
    }
    {
        const failable<value, std::string> r = http::del(url + "/" + std::string(content(id)), cs);
        assert(hasContent(r));
        assert(content(r) == value(true));
    }
    {
        const failable<value, std::string> val = http::get(url + "/" + std::string(content(id)), cs);
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
        const failable<value, std::string> val = http::get(url + "/" + std::string(id), cs);
        assert(hasContent(val));
        assert(content(val) == entry);
        return true;
    }
};

bool testGetPerf() {
    const list<value> i = list<value>()
            << (list<value>() << "name" << std::string("Apple"))
            << (list<value>() << "price" << std::string("$4.55"));
    const value a = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

    http::CURLSession cs;
    const failable<value, std::string> id = http::post(a, url, cs);
    assert(hasContent(id));

    const lambda<bool()> gl = getLoop(content(id), a, cs);
    std::cout << "Cache get test " << time(gl, 5, 200) << " ms" << std::endl;

    return true;
}

}
}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::cache::testCache();
    tuscany::cache::testGetPerf();

    std::cout << "OK" << std::endl;

    return 0;
}
