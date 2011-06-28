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
#include <apr_strings.h>
#include <assert.h>
#include <new>
#include "config.hpp"

namespace tuscany
{

#ifdef WANT_MAINTAINER_MODE

/**
 * Force a core dump on assertion violation.
 */
bool assertOrFail(const bool expr) {
    if (!expr)
        *(char*)NULL = '\0';
    return true;
}

#else

#define assertOrFail(expr)

#endif

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
    gc_pool() : apr_pool(NULL) {
    }

    gc_pool(apr_pool_t* p) : apr_pool(p) {
    }

    gc_pool(const gc_pool& pool) : apr_pool(pool.apr_pool) {
    }

    gc_pool& operator=(const gc_pool& pool) {
        if (this == &pool)
            return *this;
        apr_pool = pool.apr_pool;
        return *this;
    }

private:
    friend apr_pool_t* pool(const gc_pool& pool);
    friend class gc_global_pool_t;
    friend class gc_scoped_pool;

    apr_pool_t* apr_pool;
};

/**
 * Make a new APR pool.
 */
apr_pool_t* mkpool() {
    apr_pool_t* p = NULL;
    apr_pool_create(&p, NULL);
    assertOrFail(p != NULL);
    return p;
}

/**
 * Return the APR pool used by a gc_pool.
 */
apr_pool_t* pool(const gc_pool& pool) {
    return pool.apr_pool;
}

/**
 * Destroy a memory pool.
 */
const bool destroy(const gc_pool& p) {
    apr_pool_destroy(pool(p));
    return true;
}

/**
 * Initialize APR.
 */
class gc_apr_context_t {
public:
    gc_apr_context_t() {
        apr_initialize();
    }
} gc_apr_context;

/**
 * Maintain a stack of memory pools.
 */
#ifdef WANT_THREADS
__thread
#endif
apr_pool_t* gc_pool_stack = NULL;

/**
 * Return the current memory pool.
 */
apr_pool_t* gc_current_pool() {
    apr_pool_t* apr_pool = gc_pool_stack;
    if (apr_pool != NULL)
        return apr_pool;

    // Create a parent pool for the current thread
    apr_pool_create(&apr_pool, NULL);
    assertOrFail(apr_pool != NULL);
    gc_pool_stack = apr_pool;
    return apr_pool;
}

/**
 * Push a pool onto the stack.
 */
apr_pool_t* gc_push_pool(apr_pool_t* pool) {
    apr_pool_t* p = gc_pool_stack;
    gc_pool_stack = pool;
    return p;
}

/**
 * Pop a pool from the stack.
 */
apr_pool_t* gc_pop_pool(apr_pool_t* pool) {
    apr_pool_t* p = gc_pool_stack;
    gc_pool_stack = pool;
    return p;
}

/**
 * A memory pool scope, used to setup a scope in which a particular pool
 * will be used for all allocations.
 */
class gc_scoped_pool : public gc_pool {
public:

    gc_scoped_pool() : gc_pool(NULL), prev(gc_current_pool()), owner(true) {
        apr_pool_create(&apr_pool, NULL);
        assertOrFail(apr_pool != NULL);
        gc_push_pool(apr_pool);
    }

    gc_scoped_pool(apr_pool_t* pool) : gc_pool(pool), prev(gc_current_pool()), owner(false) {
        gc_push_pool(apr_pool);
    }

    ~gc_scoped_pool() {
        if (owner)
            apr_pool_destroy(apr_pool);
        gc_pop_pool(prev);
    }

private:
    gc_scoped_pool(const unused gc_scoped_pool& pool) : gc_pool(pool.apr_pool), prev(NULL), owner(false) {
    }

    apr_pool_t* prev;
    bool owner;
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
    void* gc_new_ptr = apr_palloc(p, sizeof(T));
    assertOrFail(gc_new_ptr != NULL);
    apr_pool_cleanup_register(p, gc_new_ptr, gc_pool_cleanup<T>, apr_pool_cleanup_null) ;
    return static_cast<T*>(gc_new_ptr);
}

template<typename T> T* gc_new(const gc_pool& p) {
    return gc_new<T>(pool(p));
}

template<typename T> T* gc_new() {
    return gc_new<T>(gc_current_pool());
}

template<typename T> apr_status_t gc_pool_acleanup(void* v) {
    size_t* m = static_cast<size_t*>(v);
    size_t n = *m;
    T* t = (T*)(m + 1);
    for (size_t i = 0; i < n; i++, t++)
        t->~T();
    return APR_SUCCESS;
}

template<typename T> T* gc_anew(apr_pool_t* p, size_t n) {
    size_t* gc_anew_ptr = static_cast<size_t*>(apr_palloc(p, sizeof(size_t) + sizeof(T[n])));
    assertOrFail(gc_anew_ptr != NULL);
    *gc_anew_ptr = n;
    apr_pool_cleanup_register(p, gc_anew_ptr, gc_pool_acleanup<T>, apr_pool_cleanup_null) ;
    return (T*)(gc_anew_ptr + 1);
}

template<typename T> T* gc_anew(const gc_pool& p, size_t n) {
    return gc_anew<T>(pool(p), n);
}

template<typename T> T* gc_anew(size_t n) {
    return gc_anew<T>(gc_current_pool(), n);
}

/**
 * Allocate an array of chars.
 */
char* gc_cnew(apr_pool_t* p, size_t n) {
    char* gc_cnew_ptr = static_cast<char*>(apr_palloc(p, n));
    assertOrFail(gc_cnew_ptr != NULL);
    return gc_cnew_ptr;
}

char* gc_cnew(size_t n) {
    return gc_cnew(gc_current_pool(), n);
}

/**
 * Pool based equivalent of the standard malloc function.
 */
void* gc_malloc(size_t n) {
    size_t* gc_malloc_ptr = static_cast<size_t*>(apr_palloc(gc_current_pool(), sizeof(size_t) + n));
    assertOrFail(gc_malloc_ptr != NULL);
    *gc_malloc_ptr = n;
    return gc_malloc_ptr + 1;
}

/**
 * Pool based equivalent of the standard realloc function.
 */
void* gc_realloc(void* ptr, size_t n) {
    size_t size = *(static_cast<size_t*>(ptr) - 1);
    size_t* gc_realloc_ptr = static_cast<size_t*>(apr_palloc(gc_current_pool(), sizeof(size_t) + n));
    assertOrFail(gc_realloc_ptr != NULL);
    *gc_realloc_ptr = n;
    memcpy(gc_realloc_ptr + 1, ptr, size < n? size : n);
    return gc_realloc_ptr + 1;
}

/**
 * Pool based equivalent of the standard free function.
 */
void gc_free(unused void* ptr) {
    // Memory allocated from a pool is freed when the pool is freed
}

/**
 * Pool based equivalent of the standard strdup function.
 */
char* gc_strdup(const char* str) {
    char* gc_strdup_ptr = static_cast<char*>(gc_malloc(strlen(str) + 1));
    assertOrFail(gc_strdup_ptr != NULL);
    strcpy(gc_strdup_ptr, str);
    return gc_strdup_ptr;
}

}

#endif /* tuscany_gc_hpp */
