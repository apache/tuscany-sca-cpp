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
 * Test cache component.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"

#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "perf.hpp"
#include "../../modules/http/http.hpp"

namespace tuscany {
namespace cache {

const string memcacheuri("http://localhost:8090/memcache");
const string datacacheuri("http://localhost:8090/datacache");
const string memocacheuri("http://localhost:8090/memocache");
const string partition1uri("http://localhost:8090/partitioner/a");
const string partition2uri("http://localhost:8090/partitioner/b");

const bool testCache(const string& uri) {
    const http::CURLSession cs("", "", "", "", 0);

    const list<value> i = nilListValue + "content" + (nilListValue + "item" 
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$2.99")));
    const list<value> a = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);

    const failable<value> id = http::post(a, uri, cs);
    assert(hasContent(id));

    const string p = path(content(id));
    {
        const failable<value> val = http::get(uri + p, cs);
        assert(hasContent(val));
        assert(content(val) == a);
    }

    const list<value> j = nilListValue + "content" + (nilListValue + "item" 
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$3.55")));
    const list<value> b = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + j);

    {
        const failable<value> r = http::put(b, uri + p, cs);
        assert(hasContent(r));
        assert(content(r) == trueValue);
    }
    {
        const failable<value> val = http::get(uri + p, cs);
        assert(hasContent(val));
        assert(content(val) == b);
    }

    const list<value> k = nilListValue + "content" + (nilListValue + "item" 
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$3.99")));
    const list<value> c = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + k);

    {
        const list<value> s = nilListValue + "content" +
            (nilListValue + "patch" + string("(define (patch id e) (tree-subst-assoc '(price) '(price \"$3.99\") e))"));
        const list<value> ps = nilListValue + (nilListValue + "entry" 
                + (nilListValue + "title" + string("item"))
                + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
                + s);

        const failable<value> r = http::patch(ps, uri + p, cs);
        assert(hasContent(r));
        assert(content(r) == trueValue);
    }
    {
        const failable<value> val = http::get(uri + p, cs);
        assert(hasContent(val));
        assert(content(val) == c);
    }
    {
        const failable<value> r = http::del(uri + p, cs);
        assert(hasContent(r));
        assert(content(r) == trueValue);
    }
    {
        const failable<value> val = http::get(uri + p, cs);
        assert(!hasContent(val));
    }

    return true;
}

const bool testMemcache() {
    return testCache(memcacheuri);
}

const bool testDatacache() {
    return testCache(datacacheuri);
}

const bool testMemocache() {
    const http::CURLSession cs("", "", "", "", 0);

    const failable<value> res = http::evalExpr(mklist<value>(string("add"), 33, 22), memocacheuri, cs);
    assert(hasContent(res));
    assert((int)content(res) == 55);

    const failable<value> res2 = http::evalExpr(mklist<value>(string("add"), 33, 22), memocacheuri, cs);
    assert(hasContent(res2));
    assert((int)content(res2) == 55);

    return true;
}

const bool testPartitioner() {
    const http::CURLSession cs("", "", "", "", 0);

    const failable<value> res1 = http::get(partition1uri, cs);
    assert(hasContent(res1));
    assert(cadr<value>(content(res1)) == string("1"));

    const failable<value> res2 = http::get(partition2uri, cs);
    assert(hasContent(res2));
    assert(cadr<value>(content(res2)) == string("2"));

    return true;
}

const bool testGetPerf() {
    const list<value> i = nilListValue + "content" + (nilListValue + "item" 
            + (nilListValue + "name" + string("Apple"))
            + (nilListValue + "price" + string("$4.55")));
    const list<value> a = nilListValue + (nilListValue + "entry" 
            + (nilListValue + "title" + string("item"))
            + (nilListValue + "id" + string("cart-53d67a61-aa5e-4e5e-8401-39edeba8b83b"))
            + i);

    const http::CURLSession cs("", "", "", "", 0);
    const failable<value> id = http::post(a, memcacheuri, cs);
    assert(hasContent(id));
    const string p = path(content(id));

    const blambda gl = [p, a, cs]() -> const bool {
        const failable<value> val = http::get(memcacheuri + p, cs);
        assert(hasContent(val));
        assert(content(val) == a);
        return true;
    };
    cout << "Cache get test " << time(gl, 5, 200) << " ms" << endl;

    return true;
}

}
}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::cache::testMemcache();
    tuscany::cache::testDatacache();
    tuscany::cache::testMemocache();
    tuscany::cache::testPartitioner();
    tuscany::cache::testGetPerf();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
