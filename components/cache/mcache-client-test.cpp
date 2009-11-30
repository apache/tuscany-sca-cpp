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
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <string>

#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
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

const double duration(struct timeval start, struct timeval end, int count) {
    long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
    return (double)t / (double)count;
}

bool testGetLoop(const int count, const value& id, const value& entry, http::CURLSession& cs) {
    if (count == 0)
        return true;
    const failable<value, std::string> val = http::get(url + "/" + std::string(id), cs);
    assert(hasContent(val));
    assert(content(val) == entry);
    return testGetLoop(count - 1, id, entry, cs);
}

bool testGetPerf() {
    const int count = 50;
    struct timeval start;
    struct timeval end;
    {
        const list<value> i = list<value>()
                << (list<value>() << "name" << std::string("Apple"))
                << (list<value>() << "price" << std::string("$4.55"));
        const list<value> a = mklist<value>(std::string("item"), std::string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"), i);

        http::CURLSession cs;
        const failable<value, std::string> id = http::post(a, url, cs);
        assert(hasContent(id));

        testGetLoop(5, content(id), a, cs);

        gettimeofday(&start, NULL);

        testGetLoop(count, content(id), a, cs);

        gettimeofday(&end, NULL);
        std::cout << "Cache get test " << duration(start, end, count) << " ms" << std::endl;
    }
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
