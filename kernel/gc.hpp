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

#include "config.hpp"
#ifdef WANT_MALLOC_MMAP
#include <sys/mman.h>
#include <malloc.h>
#endif
#include <stdlib.h>
#include <apr_general.h>
#include <apr_pools.h>
#include <apr_strings.h>
#include <assert.h>
#include <new>
#ifdef WANT_THREADS
#include <pthread.h>
#endif

namespace tuscany
{

#ifdef WANT_MAINTAINER_ASSERT

/**
 * Force a core dump on assertion violation.
 */
inline const bool assertOrFail(const bool expr) {
    if (!expr)
        abort();
    return true;
}

#else

#define assertOrFail(expr)

#endif

/**
 * Pointer to a value.
 */
#ifdef WANT_RAW_PTR

template<typename T> using gc_ptr = T*;

#else

template<typename T> class gc_ptr {
public:
    inline gc_ptr(T* const ptr = NULL) noexcept : ptr(ptr) {
    }

    inline ~gc_ptr() noexcept {
    }

    inline gc_ptr(const gc_ptr& r) noexcept : ptr(r.ptr) {
    }

    gc_ptr& operator=(const gc_ptr& r) = delete;

    inline const bool operator==(const gc_ptr& r) const noexcept {
        if (this == &r)
            return true;
        return ptr == r.ptr;
    }

    inline const bool operator==(const T* const p) const noexcept {
        return ptr == p;
    }

    inline const bool operator!=(const gc_ptr& r) const noexcept {
        return !this->operator==(r);
    }

    inline const bool operator!=(const T* const p) const noexcept {
        return !this->operator==(p);
    }

    inline T& operator*() const noexcept {
        return *ptr;
    }

    inline T* const operator->() const noexcept {
        return ptr;
    }

    inline operator T* const () const noexcept {
        return ptr;
    }

private:
    T* const ptr;
};

#endif

/**
 * Mutable pointer to an immutable value.
 */
#ifdef WANT_RAW_PTR

template<typename T> using gc_mutable_ptr = T*;

#else

template<typename T> class gc_mutable_ptr {
public:
    inline gc_mutable_ptr(T* const ptr = NULL) noexcept : ptr(ptr) {
    }

    inline ~gc_mutable_ptr() noexcept {
    }

    inline gc_mutable_ptr(const gc_mutable_ptr& r) noexcept : ptr(r.ptr) {
    }

    inline gc_mutable_ptr& operator=(T* const p) noexcept {
        ptr = p;
        return *this;
    }

    inline gc_mutable_ptr& operator=(const gc_mutable_ptr& r) noexcept {
        if (this == &r)
            return *this;
        ptr = r.ptr;
        return *this;
    }

    inline const bool operator==(const gc_mutable_ptr& r) const noexcept {
        if (this == &r)
            return true;
        return ptr == r.ptr;
    }

    inline const bool operator==(T* const p) const noexcept {
        return ptr == p;
    }

    inline const bool operator!=(const gc_mutable_ptr& r) const noexcept {
        return !this->operator==(r);
    }

    inline const bool operator!=(T* const p) const noexcept {
        return !this->operator==(p);
    }

    inline T& operator*() const noexcept {
        return *ptr;
    }

    inline T* const operator->() const noexcept {
        return ptr;
    }

    inline operator T* const () const noexcept {
        return ptr;
    }

private:
    T* ptr;
};

#endif

/**
 * Initialize APR.
 */
class gc_apr_context_t {
public:
    inline gc_apr_context_t() {
        apr_initialize();
    }
} gc_apr_context;

/**
 * Garbage collected APR memory pool.
 */
class gc_pool {
public:
    inline gc_pool() noexcept : apr_pool(NULL) {
    }

    inline gc_pool(apr_pool_t* const p) noexcept : apr_pool(p) {
    }

    inline gc_pool(const gc_pool& pool) noexcept : apr_pool(pool.apr_pool) {
    }

    gc_pool& operator=(const gc_pool& pool) = delete;

private:
    friend apr_pool_t* const pool(const gc_pool& pool) noexcept;
    friend class gc_global_pool_t;
    friend class gc_child_pool;
    friend class gc_local_pool;
    friend class gc_scoped_pool;

    apr_pool_t* const apr_pool;
};

/**
 * Return the APR pool used by a gc_pool.
 */
inline apr_pool_t* const pool(const gc_pool& pool) noexcept {
    return pool.apr_pool;
}

/**
 * Maintain a stack of memory pools.
 */
#ifdef WANT_THREADS

#ifdef __clang__

class gc_pool_stack_t {
public:
    inline gc_pool_stack_t() noexcept : key(mkkey()) {
    }

    inline operator apr_pool_t* const () const noexcept {
        return (apr_pool_t* const )pthread_getspecific(key);
    }

    const gc_pool_stack_t& operator=(apr_pool_t* const p) noexcept {
        const int rc = pthread_setspecific(key, p);
        assertOrFail(rc == 0);
        return *this;
    }

private:
    const pthread_key_t key;

    inline const pthread_key_t mkkey() noexcept {
        pthread_key_t k;
        const int rc = pthread_key_create(&k, NULL);
        assertOrFail(rc == 0);
        return k;
    }

} gc_pool_stack;

#else

__thread apr_pool_t* gc_pool_stack = NULL;

#endif

#else

apr_pool_t* gc_pool_stack = NULL;

#endif

/**
 * Push a pool onto the stack.
 */
inline apr_pool_t* const gc_push_pool(apr_pool_t* const pool) noexcept {
    apr_pool_t* const p = gc_pool_stack;
    gc_pool_stack = pool;
    return p;
}

/**
 * Pop a pool from the stack.
 */
inline apr_pool_t* const gc_pop_pool(apr_pool_t* const pool) noexcept {
    apr_pool_t* const p = gc_pool_stack;
    gc_pool_stack = pool;
    return p;
}

/**
 * Return the current memory pool.
 */
inline apr_pool_t* const gc_current_pool() noexcept {
    apr_pool_t* const p = gc_pool_stack;
    if (p != NULL)
        return p;

    // Create a parent pool for the current thread
    apr_pool_t* pp;
    apr_pool_create(&pp, NULL);
    assertOrFail(pp != NULL);
    gc_push_pool(pp);
    return pp;
}

/**
 * A child memory pool, which will be destroyed when its parent pool is destroyed.
 */
class gc_child_pool : public gc_pool {
public:

    inline gc_child_pool() noexcept : gc_pool(mkpool()), owner(true) {
    }

    inline gc_child_pool(const gc_child_pool& p) noexcept : gc_pool(p.apr_pool), owner(false) {
    }

    gc_child_pool& operator=(const gc_child_pool& p) = delete;

private:
    const bool owner;

    inline apr_pool_t* const mkpool() noexcept {
        apr_pool_t* p;
        apr_pool_create(&p, gc_current_pool());
        assertOrFail(p != NULL);
        return p;
    }
};

/**
 * A local pool scope, which will be destroyed when exiting the current scope.
 */
class gc_local_pool : public gc_pool {
public:

    inline gc_local_pool() noexcept : gc_pool(mkpool()), owner(true) {
    }

    inline ~gc_local_pool() noexcept {
        if (owner)
            apr_pool_destroy(apr_pool);
    }

    inline gc_local_pool(const gc_local_pool& p) noexcept : gc_pool(p.apr_pool), owner(false) {
    }

    gc_local_pool& operator=(const gc_local_pool& p) = delete;

private:
    const bool owner;

    inline apr_pool_t* const mkpool() noexcept {
        apr_pool_t* p;
        apr_pool_create(&p, gc_current_pool());
        assertOrFail(p != NULL);
        return p;
    }
};

/**
 * A memory pool scope, used to setup a scope in which a particular pool will be
 * used for all allocations. Will be destroyed when existing the current scope.
 */
class gc_scoped_pool : public gc_pool {
public:

    inline gc_scoped_pool() noexcept : gc_pool(mkpool()), prev(gc_current_pool()), owner(true) {
        gc_push_pool(apr_pool);
    }

    inline gc_scoped_pool(apr_pool_t* const p) noexcept : gc_pool(p), prev(gc_current_pool()), owner(false) {
        gc_push_pool(apr_pool);
    }

    inline ~gc_scoped_pool() noexcept {
        if (owner)
            apr_pool_destroy(apr_pool);
        gc_pop_pool(prev);
    }

    inline gc_scoped_pool(const gc_scoped_pool& p) noexcept : gc_pool(p.apr_pool), prev(p.prev), owner(false) {
    }

    gc_scoped_pool& operator=(const gc_scoped_pool& p) = delete;

private:
    apr_pool_t* const prev;
    const bool owner;

    inline apr_pool_t* const mkpool() noexcept {
        apr_pool_t* p;
        apr_pool_create(&p, gc_current_pool());
        assertOrFail(p != NULL);
        return p;
    }
};

/**
 * Allocates a pointer to an object allocated from a memory pool and
 * register a cleanup callback for it.
 */
template<typename T> inline apr_status_t gc_pool_cleanup(void* v) {
    T* t = (T*)v;
    t->~T();
    return APR_SUCCESS;
}

template<typename T> inline T* const gc_new(apr_pool_t* const p) noexcept {
    void* gc_new_ptr = apr_palloc(p, sizeof(T));
    assertOrFail(gc_new_ptr != NULL);
    apr_pool_cleanup_register(p, gc_new_ptr, gc_pool_cleanup<T>, apr_pool_cleanup_null) ;
    return (T*)(gc_new_ptr);
}

template<typename T> inline T* const gc_new(const gc_pool& p) noexcept {
    return gc_new<T>(pool(p));
}

template<typename T> inline T* const gc_new() noexcept {
    return gc_new<T>(gc_current_pool());
}

template<typename T> inline apr_status_t gc_pool_acleanup(void* v) {
    size_t* m = (size_t*)v;
    size_t n = *m;
    T* t = (T*)(m + 1);
    for (size_t i = 0; i < n; i++, t++)
        t->~T();
    return APR_SUCCESS;
}

template<typename T> inline T* const gc_anew(apr_pool_t* const p, const size_t n) noexcept {
    size_t* const gc_anew_ptr = (size_t*)apr_palloc(p, sizeof(size_t) + sizeof(T) * n);
    assertOrFail(gc_anew_ptr != NULL);
    *gc_anew_ptr = n;
    apr_pool_cleanup_register(p, gc_anew_ptr, gc_pool_acleanup<T>, apr_pool_cleanup_null) ;
    return (T*)(gc_anew_ptr + 1);
}

template<typename T> inline T* const gc_anew(const gc_pool& p, const size_t n) noexcept {
    return gc_anew<T>(pool(p), n);
}

template<typename T> inline T* const gc_anew(const size_t n) noexcept {
    return gc_anew<T>(gc_current_pool(), n);
}

/**
 * Allocate an array of chars.
 */
inline char* const gc_cnew(apr_pool_t* const p, const size_t n) noexcept {
    char* const gc_cnew_ptr = (char*)apr_palloc(p, n);
    assertOrFail(gc_cnew_ptr != NULL);
    return gc_cnew_ptr;
}

inline char* const gc_cnew(const size_t n) noexcept {
    return gc_cnew(gc_current_pool(), n);
}

/**
 * Mutable reference to an immutable value.
 */
template<typename T> class gc_mutable_ref {
public:
    inline gc_mutable_ref() noexcept : ptr(new (gc_new<T>()) T()) {
    }

    inline ~gc_mutable_ref() noexcept {
    }

    inline gc_mutable_ref(const gc_mutable_ref& r) noexcept : ptr(r.ptr) {
    }

    inline gc_mutable_ref(const T& v) noexcept : ptr(new (gc_new<T>()) T(v)) {
    }

    inline gc_mutable_ref& operator=(const gc_mutable_ref& r) noexcept {
        if (this == &r)
            return *this;
        ptr = r.ptr;
        return *this;
    }

    inline gc_mutable_ref& operator=(const T& v) noexcept {
        ptr = new (gc_new<T>()) T(v);
        return *this;
    }

    inline const bool operator==(const gc_mutable_ref& r) const noexcept {
        if (this == &r)
            return true;
        if (ptr == r.ptr)
            return true;
        return *ptr == *r.ptr;
    }

    inline const bool operator==(const T& v) const noexcept {
        return *ptr == v;
    }

    inline const bool operator!=(const gc_mutable_ref& r) const noexcept {
        return !this->operator==(r);
    }

    inline const bool operator!=(const T& v) const noexcept {
        return !this->operator==(v);
    }

    inline operator T&() const noexcept {
        return *ptr;
    }

    inline operator T* const () const noexcept {
        return ptr;
    }

private:
    T* ptr;
};

/**
 * Pool based equivalent of the standard malloc function.
 */
inline void* gc_pool_malloc(size_t n) {
    size_t* ptr = (size_t*)apr_palloc(gc_current_pool(), sizeof(size_t) + n);
    assertOrFail(ptr != NULL);
    *ptr = n;
    return ptr + 1;
}

/**
 * Pool based equivalent of the standard realloc function.
 */
inline void* gc_pool_realloc(void* ptr, size_t n) {
    size_t size = *(((size_t*)ptr) - 1);
    size_t* rptr = (size_t*)apr_palloc(gc_current_pool(), sizeof(size_t) + n);
    assertOrFail(rptr != NULL);
    *rptr = n;
    memcpy(rptr + 1, ptr, size < n? size : n);
    return rptr + 1;
}

/**
 * Pool based equivalent of the standard free function.
 */
inline void gc_pool_free(unused void* ptr) {
    // Memory allocated from a pool is freed when the pool is freed
}

/**
 * Pool based equivalent of the standard strdup function.
 */
inline char* gc_pool_strdup(const char* str) {
    char* dptr = (char*)gc_pool_malloc(strlen(str) + 1);
    assertOrFail(dptr != NULL);
    strcpy(dptr, str);
    return dptr;
}

#ifdef WANT_MALLOC_MMAP

/**
 * Mmap based memory allocation functions.
 */

/**
 * Mmap based equivalent of the standard malloc function.
 */
inline void* gc_mmap_malloc(size_t n, unused const void* caller) {
    //printf("gc_mmap_malloc %d", n);
    size_t* ptr = (size_t*)mmap(NULL, sizeof(size_t) + n, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assertOrFail(ptr != NULL);
    *ptr = n;
    //printf(" %p\n", ptr + 1);
    return ptr + 1;
}

/**
 * Mmap based equivalent of the standard realloc function.
 */
inline void* gc_mmap_realloc(void* ptr, size_t n, const void* caller) {
    if (ptr == NULL)
        return gc_mmap_malloc(n, caller);;
    //printf("gc_mmap_realloc %p %d", ptr, n);
    size_t size = *(((size_t*)ptr) - 1);
    size_t* rptr = (size_t*)mremap(((size_t*)ptr) - 1, sizeof(size_t) + size, sizeof(size_t) + n, MREMAP_MAYMOVE, NULL);
    assertOrFail(rptr != NULL);
    *rptr = n;
    //printf(" %p\n", rptr + 1);
    return rptr + 1;
}

/**
 * Mmap based equivalent of the standard free function.
 */
inline void gc_mmap_free(void* ptr, unused const void* caller) {
    //printf("gc_mmap_free %p\n", ptr);
    if (ptr == NULL)
        return;
    size_t size = *(((size_t*)ptr) - 1);
    munmap(((size_t*)ptr) - 1, sizeof(size_t) + size);
}

/**
 * Mmap based equivalent of the standard memalign function.
 */
inline void* gc_mmap_memalign(unused size_t alignment, size_t n, unused const void* caller) {
    //printf("gc_mmap_memalign %d %d\n", alignment, n);
    return gc_mmap_malloc(n, caller);
}

/**
 * Install the mmap based memory allocation functions.
 */
inline void gc_mmap_init_hook(void) {
    __malloc_hook = gc_mmap_malloc;
    __realloc_hook = gc_mmap_realloc;
    __free_hook = gc_mmap_free;
    __memalign_hook = gc_mmap_memalign;
}
                                          
#endif

}

#ifdef WANT_MALLOC_MMAP

void (*__malloc_initialize_hook)(void) = tuscany::gc_mmap_init_hook;

#endif

#endif /* tuscany_gc_hpp */
