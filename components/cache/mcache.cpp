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
 * Memcached-based cache component implementation.
 */

#include <apr_uuid.h>

#include "string.hpp"

#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "mcache.hpp"

namespace tuscany {
namespace mcache {

/**
 * Get an item from the cache.
 */
const failable<value> get(const list<value>& params, cache::MemCached& ch) {
    return cache::get(car(params), ch);
}

/**
 * Post an item to the cache.
 */
const value uuidValue() {
    apr_uuid_t uuid;
    apr_uuid_get(&uuid);
    char buf[APR_UUID_FORMATTED_LENGTH];
    apr_uuid_format(buf, &uuid);
    return value(string(buf, APR_UUID_FORMATTED_LENGTH));
}

const failable<value> post(const list<value>& params, cache::MemCached& ch) {
    const value id = append<value>(car(params), mklist(uuidValue()));
    const failable<bool> val = cache::post(id, cadr(params), ch);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return id;
}

/**
 * Put an item into the cache.
 */
const failable<value> put(const list<value>& params, cache::MemCached& ch) {
    const failable<bool> val = cache::put(car(params), cadr(params), ch);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(content(val));
}

/**
 * Delete an item from the cache.
 */
const failable<value> del(const list<value>& params, cache::MemCached& ch) {
    const failable<bool> val = cache::del(car(params), ch);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(content(val));
}

/**
 * Component implementation lambda function.
 */
class applyCache {
public:
    applyCache(cache::MemCached& ch) : ch(ch) {
    }

    const value operator()(const list<value>& params) const {
        const value func(car(params));
        if (func == "get")
            return get(cdr(params), ch);
        if (func == "post")
            return post(cdr(params), ch);
        if (func == "put")
            return put(cdr(params), ch);
        if (func == "delete")
            return del(cdr(params), ch);
        return tuscany::mkfailure<tuscany::value>();
    }

private:
    cache::MemCached& ch;
};

/**
 * Start the component.
 */
const failable<value> start(unused const list<value>& params) {
    // Connect to memcached
    cache::MemCached& ch = *(new (gc_new<cache::MemCached>()) cache::MemCached("127.0.0.1", 11211));

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applyCache(ch)));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::mcache::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
