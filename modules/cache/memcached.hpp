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

#ifndef tuscany_memcached_hpp
#define tuscany_memcached_hpp

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

#include <string>
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"

namespace tuscany {
namespace memcached {

/**
 * Represents a memcached context.
 */
class Cache {
public:
    Cache() {
        apr_pool_create(&pool, NULL);
        apr_memcache_create(pool, 1, 0, &mc);
    }
    ~Cache() {
        apr_pool_destroy(pool);
    }

    operator apr_memcache_t*() const {
        return mc;
    }

    operator apr_pool_t*() const {
        return pool;
    }

private:
    apr_pool_t* pool;
    apr_memcache_t* mc;

};

/**
 * Add a server to the memcached context.
 */
const failable<bool, std::string> addServer(const std::string& host, const int port, const Cache& cache) {
    apr_memcache_server_t *server;
    const apr_status_t sc = apr_memcache_server_create(cache, host.c_str(), port, 0, 1, 1, 60, &server);
    if (sc != APR_SUCCESS)
        return mkfailure<bool, std::string>("Could not create server");
    const apr_status_t as = apr_memcache_add_server(cache, server);
    if (as != APR_SUCCESS)
        return mkfailure<bool, std::string>("Could not add server");
    return true;
}

/**
 * Post a new item to the cache.
 */
const failable<bool, std::string> post(const value& key, const value& val, const Cache& cache) {
    const std::string v(val);
    const apr_status_t rc = apr_memcache_add(cache, std::string(key).c_str(), const_cast<char*>(v.c_str()), v.size(), 0, 27);
    if (rc != APR_SUCCESS)
        return mkfailure<bool, std::string>("Could not add entry");
    return true;
}

/**
 * Update an item in the cache.
 */
const failable<bool, std::string> put(const value& key, const value& val, const Cache& cache) {
    const std::string v(val);
    const apr_status_t rc = apr_memcache_replace(cache, std::string(key).c_str(), const_cast<char*>(v.c_str()), v.size(), 0, 27);
    if (rc != APR_SUCCESS)
        return mkfailure<bool, std::string>("Could not add entry");
    return true;
}

/**
 * Get an item from the cache.
 */
const failable<value, std::string> get(const value& key, const Cache& cache) {
    apr_pool_t* vpool;
    const apr_status_t pc = apr_pool_create(&vpool, cache);
    if (pc != APR_SUCCESS)
        return mkfailure<value, std::string>("Could not allocate memory");

    char *data;
    apr_size_t size;
    const apr_status_t rc = apr_memcache_getp(cache, cache, std::string(key).c_str(), &data, &size, NULL);
    if (rc != APR_SUCCESS) {
        apr_pool_destroy(vpool);
        return mkfailure<value, std::string>("Could not get entry");
    }

    const value val(std::string(data, size));
    apr_pool_destroy(vpool);
    return val;
}

/**
 * Delete an item from the cache
 */
const failable<bool, std::string> del(const value& key, const Cache& cache) {
    const apr_status_t rc = apr_memcache_delete(cache, std::string(key).c_str(), 0);
    if (rc != APR_SUCCESS)
        return mkfailure<bool, std::string>("Could not add entry");
    return true;
}

}
}

#endif /* tuscany_memcached_hpp */
