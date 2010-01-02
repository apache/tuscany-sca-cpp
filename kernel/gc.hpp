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

#ifndef tuscany_gc_hpp
#define tuscany_gc_hpp

/**
 * Garbage collected memory management, using APR memory pools.
 */

#include <stdlib.h>
#include <apr_general.h>
#include <apr_pools.h>
#include <assert.h>
#include <new>
#include "debug.hpp"

namespace tuscany
{

/**
 * Pointer to a value.
 */
template<typename T> class gc_ptr {
public:
    gc_ptr(T* ptr = NULL) throw() : ptr(ptr) {
    }

    ~gc_ptr() throw() {
    }

    gc_ptr(const gc_ptr& r) throw() : ptr(r.ptr) {
    }

    gc_ptr& operator=(const gc_ptr& r) throw() {
        if(this == &r)
            return *this;
        ptr = r.ptr;
        return *this;
    }

    const bool operator==(const gc_ptr& r) const throw() {
        if (this == &r)
            return true;
        return ptr == r.ptr;
    }

    const bool operator!=(const gc_ptr& r) const throw() {
        return !this->operator==(r);
    }

    T& operator*() const throw() {
        return *ptr;
    }

    T* operator->() const throw() {
        return ptr;
    }

    operator T*() const throw() {
        return ptr;
    }

    T* ptr;
};

/**
 * Garbage collected APR memory pool.
 */
class gc_pool {
public:
    gc_pool() : p(NULL) {
    }

    gc_pool(apr_pool_t* p) : p(p) {
    }

    gc_pool(const gc_pool& pool) : p(pool.p) {
    }

    gc_pool& operator=(const gc_pool& pool) {
        if (this == &pool)
            return *this;
        p = pool.p;
        return *this;
    }

    operator apr_pool_t*() const {
        return p;
    }

private:
    friend const bool destroy(gc_pool& pool);
    friend class gc_global_pool_t;
    friend class gc_scoped_pool;

    apr_pool_t* p;
};

/**
 * Destroy a memory pool.
 */
const bool destroy(gc_pool& pool) {
    apr_pool_destroy(pool.p);
    return true;
}

/**
 * Default global memory pool.
 */
class gc_apr_context_t {
public:
    gc_apr_context_t() {
        apr_initialize();
    }
    ~gc_apr_context_t() {
        //apr_terminate();
    }
};

gc_apr_context_t gc_apr_context;

class gc_global_pool_t : public gc_pool {
public:
    gc_global_pool_t() {
        apr_pool_create(&p, NULL);
    }

    ~gc_global_pool_t() {
        //apr_pool_destroy(p);
    }
};

gc_global_pool_t gc_global_pool;

/**
 * Maintain a stack of memory pools.
 */
#ifdef _REENTRANT
__thread
#endif
apr_pool_t* gc_pool_stack = NULL;

/**
 * Return the current memory pool.
 */
apr_pool_t* gc_current_pool() {
    apr_pool_t* p = gc_pool_stack;
    if (p != NULL)
        return p;
    apr_pool_t* g = gc_global_pool;
    gc_pool_stack = g;
    return g;
}

/**
 * A memory pool scope, used to setup a scope in which a particular pool
 * will be used for all allocations.
 */
class gc_scoped_pool : public gc_pool {
public:

    gc_scoped_pool() : gc_pool(NULL), prev(gc_current_pool()), owned(true) {
        apr_pool_create(&p, NULL);
        gc_pool_stack = p;
    }

    gc_scoped_pool(apr_pool_t* pool) : gc_pool(pool), prev(gc_current_pool()), owned(false) {
        gc_pool_stack = p;
    }

    ~gc_scoped_pool() {
        if (owned)
            apr_pool_destroy(p);
        if (prev != NULL)
            gc_pool_stack = prev;
    }

private:
    gc_scoped_pool(const unused gc_scoped_pool& pool) : gc_pool(pool.p), prev(NULL), owned(false) {
    }

    apr_pool_t* prev;
    bool owned;
};

/**
 * Allocates a pointer to an object allocated from a memory pool and
 * register a cleanup callback for it.
 */
template<typename T> apr_status_t gc_pool_cleanup(void* v) {
    T* t = static_cast<T*>(v);
    t->~T();
    return APR_SUCCESS;
}

template<typename T> T* gc_new(apr_pool_t* p) {
    void* m = apr_palloc(p, sizeof(T));
    apr_pool_cleanup_register(p, m, gc_pool_cleanup<T>, apr_pool_cleanup_null) ;
    return static_cast<T*>(m);
}

template<typename T> T* gc_new() {
    return gc_new<T>(gc_current_pool());
}

template<typename T> apr_status_t gc_pool_acleanup(void* v) {
    int* m = static_cast<int*>(v);
    int n = *m;
    T* t = static_cast<T*>(m + 1);
    for (int i = 0; i < n; i++, t++)
        t->~T();
    return APR_SUCCESS;
}

template<typename T> T* gc_anew(apr_pool_t* p, int n) {
    int* m = static_cast<int*>(apr_palloc(p, sizeof(int) + sizeof(T[n])));
    *m = n;
    apr_pool_cleanup_register(p, m, gc_pool_acleanup<T>, apr_pool_cleanup_null) ;
    return static_cast<T*>(m + 1);
}

template<typename T> T* gc_anew(int n) {
    return gc_anew<T>(gc_current_pool(), n);
}

/**
 * Allocate an array of chars.
 */
char* gc_cnew(apr_pool_t* p, int n) {
    return static_cast<char*>(apr_palloc(p, n));
}

char* gc_cnew(int n) {
    return gc_cnew(gc_current_pool(), n);
}

}

#endif /* tuscany_gc_hpp */
