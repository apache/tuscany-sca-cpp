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
 * A data cache component implementation which coordinates access to two
 * levels of backend read/write caches or stores. Each cache or store is
 * accessed through two references: a writer reference and a reader reference.
 *
 * This is useful if your level2 store is made of a master and slave
 * replicated databases, you can then wire the writer reference to the master
 * database and the reader reference to one your slave databases (assuming
 * that the updates eventually get replicated to the slave database, in the
 * meantime the updates will be retrieved from the level1 cache).
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"

namespace tuscany {
namespace datacache {

/**
 * Get an item from the cache.
 */
const failable<value> get(const value& key, const lvvlambda& rcache1, const lvvlambda& wcache1, const lvvlambda& rcache2, unused const lvvlambda& wcache2) {

    // Lookup level1 cache
    const value val1 = rcache1(mklist<value>("get", key));
    if (!isNull(val1))
        return val1;

    // Lookup level2 cache
    const value val2 = rcache2(mklist<value>("get", key));
    if (isNull(val2)) {
        ostringstream os;
        os << "Couldn't get cache entry: " << key;
        return mkfailure<value>(str(os), 404, false);
    }

    // Update level1 cache
    wcache1(mklist<value>("put", key, val2));

    return val2;
}

/**
 * Post an item to the cache.
 */
const failable<value> post(const value& key, const value& val, unused const lvvlambda& rcache1, const lvvlambda& wcache1, unused const lvvlambda& rcache2, const lvvlambda& wcache2) {
    const value id = append<value>(key, mklist(mkuuid()));

    // Update level1 cache
    wcache1(mklist<value>("put", id, val));

    // Update level2 cache
    wcache2(mklist<value>("put", id, val));

    return id;
}

/**
 * Put an item into the cache.
 */
const failable<value> put(const value& key, const value& val, unused const lvvlambda& rcache1, const lvvlambda& wcache1, unused const lvvlambda& rcache2, const lvvlambda& wcache2) {

    // Update level1 cache
    wcache1(mklist<value>("put", key, val));

    // Update level2 cache
    wcache2(mklist<value>("put", key, val));

    return trueValue;
}

/**
 * Patch an item in the cache.
 */
const failable<value> patch(const value& key, const value& val, unused const lvvlambda& rcache1, const lvvlambda& wcache1, unused const lvvlambda& rcache2, const lvvlambda& wcache2) {

    // Update level1 cache
    wcache1(mklist<value>("patch", key, val));

    // Update level2 cache
    wcache2(mklist<value>("patch", key, val));

    return trueValue;
}

/**
 * Delete an item from the cache.
 */
const failable<value> del(const value& key, unused const lvvlambda& rcache1, const lvvlambda& wcache1, unused const lvvlambda& rcache2, const lvvlambda& wcache2) {

    // Delete from level1 cache
    wcache1(mklist<value>("delete", key));

    // Delete from level2 cache
    wcache2(mklist<value>("delete", key));

    return trueValue;
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "get")
        return tuscany::datacache::get(cadr(params), caddr(params), cadddr(params), caddddr(params), cadddddr(params));
    if (func == "post")
        return tuscany::datacache::post(cadr(params), caddr(params), cadddr(params), caddddr(params), cadddddr(params), caddddddr(params));
    if (func == "put")
        return tuscany::datacache::put(cadr(params), caddr(params), cadddr(params), caddddr(params), cadddddr(params), caddddddr(params));
    if (func == "patch")
        return tuscany::datacache::patch(cadr(params), caddr(params), cadddr(params), caddddr(params), cadddddr(params), caddddddr(params));
    if (func == "delete")
        return tuscany::datacache::del(cadr(params), caddr(params), cadddr(params), caddddr(params), cadddddr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
