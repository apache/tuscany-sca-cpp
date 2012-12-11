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

#ifndef tuscany_memcache_hpp
#define tuscany_memcache_hpp

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
namespace memcache {

/**
 * Represents a memcached context.
 */
class MemCached {
public:
    MemCached() : owner(false), mc(NULL) {
    }

    MemCached(const string& host, const int port) : p(), owner(true), mc(mkmc(host, port)) {
    }

    MemCached(const list<string>& servers) : p(), owner(true), mc(mkmc(servers)) {
    }

    MemCached(const MemCached& c) : p(c.p), owner(false), mc(c.mc) {
        debug("memcache::memcached::copy");
    }

    MemCached& operator=(const MemCached& c) = delete;

    ~MemCached() {
    }

private:
    const gc_child_pool p;
    const bool owner;
    apr_memcache_t* const mc;

    friend const failable<bool> post(const value& key, const value& val, const MemCached& cache);
    friend const failable<bool> put(const value& key, const value& val, const MemCached& cache);
    friend const failable<value> get(const value& key, const MemCached& cache);
    friend const failable<bool> del(const value& key, const MemCached& cache);

    /**
     * Add servers to the memcached context.
     */
    const failable<bool> addServer(apr_memcache_t* const m, const string& host, const int port) {
        apr_memcache_server_t *server;
        const apr_status_t sc = apr_memcache_server_create(pool(p), c_str(host), (apr_port_t)port, 1, 1, 1, 600, &server);
        if (sc != APR_SUCCESS) {
            ostringstream os;
            os << "Couldn't connect to memcached server: " << host << ":" << port;
            return mkfailure<bool>(str(os));
        }
        const apr_status_t as = apr_memcache_add_server(m, server);
        if (as != APR_SUCCESS)
            return mkfailure<bool>("Couldn't add memcached server");
        return true;
    }

    const failable<bool> addServers(apr_memcache_t* const m, const list<string>& servers) {
        if (isNil(servers))
            return true;
        const list<string> toks = tokenize(":", car(servers));
        const failable<bool> r = addServer(m, car(toks), isNil(cdr(toks))? 11211 : atoi(c_str(cadr(toks))));
        if (!hasContent(r))
            return r;
        return addServers(m, cdr(servers));
    }

    /**
     * Create memcached handle.
     */
    apr_memcache_t* const mkmc(const string& host, const int port) {
        debug(host, "memcache::memcached::host");
        debug(port, "memcache::memcached::port");
        apr_memcache_t* m;
        apr_memcache_create(pool(p), 1, 0, &m);
        addServer(m, host, port);
        return m;
    }

    apr_memcache_t* const mkmc(const list<string>& servers) {
        debug(servers, "memcache::memcached::servers");
        apr_memcache_t* m;
        apr_memcache_create(pool(p), (apr_uint16_t)length(servers), 0, &m);
        addServers(m, servers);
        return m;
    }

};

/**
 * Replace spaces by tabs (as spaces are not allowed in memcached keys).
 */
const char* const nospaces(const char* const s) {
    char* c = const_cast<char*>(s);
    for (; *c; c++)
        if (*c == ' ')
            *c = '\t';
    return s;
}

/**
 * Post a new item to the cache.
 */
const failable<bool> post(const value& key, const value& val, const MemCached& cache) {
    debug(key, "memcache::post::key");
    debug(val, "memcache::post::value");

    const string ks(write(content(scheme::writeValue(key))));
    const string vs(write(content(scheme::writeValue(val))));
    const apr_status_t rc = apr_memcache_add(cache.mc, nospaces(c_str(ks)), const_cast<char*>(c_str(vs)), length(vs), 0, 27);
    if (rc != APR_SUCCESS) {
        ostringstream os;
        os << "Couldn't add memcached entry: " << key;
        return mkfailure<bool>(str(os));
    }

    debug(true, "memcache::post::result");
    return true;
}

/**
 * Update an item in the cache. If the item doesn't exist it is added.
 */
const failable<bool> put(const value& key, const value& val, const MemCached& cache) {
    debug(key, "memcache::put::key");
    debug(val, "memcache::put::value");

    const string ks(write(content(scheme::writeValue(key))));
    const string vs(write(content(scheme::writeValue(val))));
    const apr_status_t rc = apr_memcache_set(cache.mc, nospaces(c_str(ks)), const_cast<char*>(c_str(vs)), length(vs), 0, 27);
    if (rc != APR_SUCCESS) {
        ostringstream os;
        os << "Couldn't set memcached entry: " << key;
        return mkfailure<bool>(str(os));
    }

    debug(true, "memcache::put::result");
    return true;
}

/**
 * Get an item from the cache.
 */
const failable<value> get(const value& key, const MemCached& cache) {
    debug(key, "memcache::get::key");

    const string ks(write(content(scheme::writeValue(key))));
    char *data;
    apr_size_t size;
    const gc_local_pool lp;
    const apr_status_t rc = apr_memcache_getp(cache.mc, pool(lp), nospaces(c_str(ks)), &data, &size, NULL);
    if (rc != APR_SUCCESS) {
        ostringstream os;
        os << "Couldn't get memcached entry: " << key;
        return mkfailure<value>(str(os), 404, false);
    }
    const value val(content(scheme::readValue(string(data, size))));

    debug(val, "memcache::get::result");
    return val;
}

/**
 * Delete an item from the cache
 */
const failable<bool> del(const value& key, const MemCached& cache) {
    debug(key, "memcache::delete::key");

    const string ks(write(content(scheme::writeValue(key))));
    const apr_status_t rc = apr_memcache_delete(cache.mc, nospaces(c_str(ks)), 0);
    if (rc != APR_SUCCESS) {
        ostringstream os;
        os << "Couldn't delete memcached entry: " << key;
        return mkfailure<bool>(str(os));
    }

    debug(true, "memcache::delete::result");
    return true;
}

}
}

#endif /* tuscany_memcache_hpp */
