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

#ifndef tuscany_mcache_hpp
#define tuscany_mcache_hpp

/**
 * Memcached access functions.
 */

#include "apr.h"
#include "apu.h"
#include "apr_general.h"
#include "apr_strings.h"
#include "apr_hash.h"
#include "apr_memcache.h"
#include "apr_network_io.h"

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace cache {

/**
 * Represents a memcached context.
 */
class MemCached {
public:
    MemCached() {
        apr_pool_create(&pool, NULL);
        apr_memcache_create(pool, 1, 0, &mc);
        init("localhost", 11211);
    }

    MemCached(const string host, const int port) {
        apr_pool_create(&pool, NULL);
        apr_memcache_create(pool, 1, 0, &mc);
        init(host, port);
    }

    ~MemCached() {
        apr_pool_destroy(pool);
    }

private:
    apr_pool_t* pool;
    apr_memcache_t* mc;

    friend const failable<bool> post(const value& key, const value& val, const MemCached& cache);
    friend const failable<bool> put(const value& key, const value& val, const MemCached& cache);
    friend const failable<value> get(const value& key, const MemCached& cache);
    friend const failable<bool> del(const value& key, const MemCached& cache);

    /**
     * Initialize the memcached context.
     */
    const failable<bool> init(const string& host, const int port) {
        apr_memcache_server_t *server;
        const apr_status_t sc = apr_memcache_server_create(pool, c_str(host), (apr_port_t)port, 0, 1, 1, 60, &server);
        if (sc != APR_SUCCESS)
            return mkfailure<bool>("Could not create server");
        const apr_status_t as = apr_memcache_add_server(mc, server);
        if (as != APR_SUCCESS)
            return mkfailure<bool>("Could not add server");
        return true;
    }

};

/**
 * Post a new item to the cache.
 */
const failable<bool> post(const value& key, const value& val, const MemCached& cache) {
    debug(key, "cache::post::key");
    debug(val, "cache::post::value");

    const string ks(scheme::writeValue(key));
    const string vs(scheme::writeValue(val));
    const apr_status_t rc = apr_memcache_add(cache.mc, c_str(ks), const_cast<char*>(c_str(vs)), length(vs), 0, 27);
    if (rc != APR_SUCCESS)
        return mkfailure<bool>("Could not add entry");

    debug(true, "cache::post::result");
    return true;
}

/**
 * Update an item in the cache. If the item doesn't exist it is added.
 */
const failable<bool> put(const value& key, const value& val, const MemCached& cache) {
    debug(key, "cache::put::key");
    debug(val, "cache::put::value");

    const string ks(scheme::writeValue(key));
    const string vs(scheme::writeValue(val));
    const apr_status_t rc = apr_memcache_set(cache.mc, c_str(ks), const_cast<char*>(c_str(vs)), length(vs), 0, 27);
    if (rc != APR_SUCCESS)
        return mkfailure<bool>("Could not add entry");

    debug(true, "cache::put::result");
    return true;
}

/**
 * Get an item from the cache.
 */
const failable<value> get(const value& key, const MemCached& cache) {
    debug(key, "cache::get::key");

    const string ks(scheme::writeValue(key));
    apr_pool_t* vpool;
    const apr_status_t pc = apr_pool_create(&vpool, cache.pool);
    if (pc != APR_SUCCESS)
        return mkfailure<value>("Could not allocate memory");

    char *data;
    apr_size_t size;
    const apr_status_t rc = apr_memcache_getp(cache.mc, cache.pool, c_str(ks), &data, &size, NULL);
    if (rc != APR_SUCCESS) {
        apr_pool_destroy(vpool);
        return mkfailure<value>("Could not get entry");
    }

    const value val(scheme::readValue(string(data, size)));
    apr_pool_destroy(vpool);

    debug(val, "cache::get::result");
    return val;
}

/**
 * Delete an item from the cache
 */
const failable<bool> del(const value& key, const MemCached& cache) {
    debug(key, "cache::delete::key");

    const string ks(scheme::writeValue(key));
    const apr_status_t rc = apr_memcache_delete(cache.mc, c_str(ks), 0);
    if (rc != APR_SUCCESS)
        return mkfailure<bool>("Could not delete entry");

    debug(true, "cache::delete::result");
    return true;
}

}
}

#endif /* tuscany_mcache_hpp */
