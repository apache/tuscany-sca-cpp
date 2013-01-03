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

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "memcache.hpp"

namespace tuscany {
namespace cache {

/**
 * Get an item from the cache.
 */
const failable<value> get(const list<value>& params, const memcache::MemCached& ch) {
    return memcache::get(car(params), ch);
}

/**
 * Post an item to the cache.
 */
const failable<value> post(const list<value>& params, const memcache::MemCached& ch) {
    const value id = append<value>(car(params), mklist(mkuuid()));
    const failable<bool> val = memcache::post(id, cadr(params), ch);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return id;
}

/**
 * Put an item into the cache.
 */
const failable<value> put(const list<value>& params, const memcache::MemCached& ch) {
    const failable<bool> val = memcache::put(car(params), cadr(params), ch);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Patch an item in the cache.
 */
const failable<value> patch(const list<value>& params, const memcache::MemCached& ch) {
    // Read patch
    value p = assoc<value>("patch", assoc<value>("content", car<value>(cadr(params))));
    if (isNil(p))
        return mkfailure<value>("Couldn't read patch script");
    const string script = cadr<value>(p);
    debug(script, "memcache::patch::script");
    istringstream is(script);

    // Get existing value from cache
    const failable<value> ival = memcache::get(car(params), ch);
    if (!hasContent(ival) && rcode(ival) != 404)
        return mkfailure<value>(ival);

    // Apply patch
    scheme::Env env = scheme::setupEnvironment();
    const value pval = scheme::evalScript(cons<value>("patch", scheme::quotedParameters(mklist<value>(car(params), hasContent(ival)? content(ival) : (value)list<value>()))), is, env);
    if (isNil(pval)) {
        ostringstream os;
        os << "Couldn't patch memcached entry: " << car(params);
        return mkfailure<value>(str(os), 404, false);
    }

    // Push patched value to cache
    const failable<bool> val = memcache::patch(car(params), pval, ch);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Delete an item from the cache.
 */
const failable<value> del(const list<value>& params, const memcache::MemCached& ch) {
    const failable<bool> val = memcache::del(car(params), ch);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(content(val));
}

/**
 * Convert a list of properties to a list of server addresses.
 */
const list<string> servers(const list<value>& params) {
    if (isNil(params))
        return list<string>();
    const value s = ((lvvlambda)car(params))(nilListValue);
    return cons<string>(s, servers(cdr(params)));
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Connect to memcached
    const memcache::MemCached& ch = *(new (gc_new<memcache::MemCached>()) memcache::MemCached(servers(params)));

    // Return the component implementation lambda function
    const lvvlambda applyCache = [ch](const list<value>& params) -> const value {
        const value func(car(params));
        if (func == "get")
            return get(cdr(params), ch);
        if (func == "post")
            return post(cdr(params), ch);
        if (func == "put")
            return put(cdr(params), ch);
        if (func == "patch")
            return patch(cdr(params), ch);
        if (func == "delete")
            return del(cdr(params), ch);
        return mkfailure<value>();
    };
    return value(applyCache);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::cache::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
