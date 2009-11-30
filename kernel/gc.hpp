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
 * Garbage collected pointer.
 */

#include <apr_general.h>
#include <apr_pools.h>
#include <iostream>

namespace tuscany
{

/**
 * Macros used to add or subtract values to reference counters.
 * In a multithreaded environment, define MTHREAD_GC to use
 * the GCC __sync_add_and_fetch and __sync_sub_and_fetch built
 * in functions.
 */
#ifdef MTHREAD_GC

#define gc_add_and_fetch(t, v) __sync_add_and_fetch(&(t), v)
#define gc_sub_and_fetch(t, v) __sync_sub_and_fetch(&(t), v)

#else

#define gc_add_and_fetch(t, v) ((t) = (t) + (v))
#define gc_sub_and_fetch(t, v) ((t) = (t) - (v))

#endif

template<typename T> class gc_ptr {
public:
    gc_ptr(T* p = 0) throw() : countingRef(p == 0? 0 : new CountingRef(p)) {
    }

    ~gc_ptr() throw() {
        release();
    }

    gc_ptr(const gc_ptr& r) throw() : countingRef(r.countingRef) {
        acquire(r.countingRef);
    }

    gc_ptr& operator=(const gc_ptr& r) throw() {
        if(this == &r)
            return *this;
        acquire(r.countingRef);
        release();
        countingRef = r.countingRef;
        return *this;
    }

    const bool operator==(const gc_ptr& r) const throw() {
        if (this == &r)
            return true;
        if (countingRef == NULL)
            return r.countingRef == NULL;
        if (r.countingRef == NULL)
            return false;
        return countingRef-> ptr == r.countingRef->ptr;
    }

    const bool operator!=(const gc_ptr& r) const throw() {
        return !this->operator ==(r);
    }

    T& operator*() const throw() {
        return *countingRef->ptr;
    }

    T* operator->() const throw() {
        return countingRef->ptr;
    }

    operator T*() const throw() {
        return countingRef->ptr;
    }

    template<typename X> friend std::ostream& operator<<(std::ostream&, const gc_ptr<X>&);

private:
    struct CountingRef {
        T* ptr;
        unsigned count;

        CountingRef(T* p) throw() :
            ptr(p), count(1) {
        }
    }* countingRef;

    void acquire(CountingRef* ref) throw() {
        if(ref)
            gc_add_and_fetch(ref->count, (unsigned int)1);
    }

    void release() throw() {
        if(countingRef) {
            unsigned rc = gc_sub_and_fetch(countingRef->count, (unsigned int)1);
            if(rc == 0) {
                delete countingRef->ptr;
                delete countingRef;
            }
        }
    }
};

template<typename T> std::ostream& operator<<(std::ostream& out, const gc_ptr<T>& p) {
    return out << p.countingRef->ptr;
}

/**
 * Garbage collected pointer to an array.
 */
template<typename T> class gc_aptr {
public:
    gc_aptr(T* p = 0) throw() : countingRef(p == 0? 0 : new CountingRef(p)) {
    }

    ~gc_aptr() throw() {
        release();
    }

    gc_aptr(const gc_aptr& r) throw() : countingRef(r.countingRef) {
        acquire(r.countingRef);
    }

    gc_aptr& operator=(const gc_aptr& r) throw() {
        if(this == &r)
            return *this;
        acquire(r.countingRef);
        release();
        countingRef = r.countingRef;
        return *this;
    }

    const bool operator==(const gc_aptr& r) const throw() {
        if (this == &r)
            return true;
        if (countingRef == NULL)
            return r.countingRef == NULL;
        if (r.countingRef == NULL)
            return false;
        return countingRef-> ptr == r.countingRef->ptr;
    }

    const bool operator!=(const gc_aptr& r) const throw() {
        return !this->operator ==(r);
    }

    T& operator*() const throw() {
        return *countingRef->ptr;
    }

    T* operator->() const throw() {
        return countingRef->ptr;
    }

    operator T*() const throw() {
        return countingRef->ptr;
    }

    template<typename X> friend std::ostream& operator<<(std::ostream&, const gc_aptr<X>&);

private:
    struct CountingRef {
        T* ptr;
        unsigned count;

        CountingRef(T* p) throw() :
            ptr(p), count(1) {
        }
    }* countingRef;

    void acquire(CountingRef* ref) throw() {
        if(ref)
            gc_add_and_fetch(ref->count, (unsigned int)1);
    }

    void release() throw() {
        if(countingRef) {
            unsigned rc = gc_sub_and_fetch(countingRef->count, (unsigned int)1);
            if(rc == 0) {
                delete[] countingRef->ptr;
                delete countingRef;
            }
        }
    }
};

template<typename T> std::ostream& operator<<(std::ostream& out, const gc_aptr<T>& p) {
    return out << p.countingRef->ptr;
}

/**
 * Garbage collected pointer to a reference counting object.
 */
template<typename T> class gc_counting_ptr {
public:
    gc_counting_ptr(T* p = 0) throw() : ptr(p) {
        acquire(p);
    }

    ~gc_counting_ptr() throw() {
        release();
    }

    gc_counting_ptr(const gc_counting_ptr& r) throw() : ptr(r.ptr) {
        acquire(ptr);
    }

    gc_counting_ptr& operator=(const gc_counting_ptr& r) throw() {
        if(this == &r)
            return *this;
        acquire(r.ptr);
        release();
        ptr = r.ptr;
        return *this;
    }

    const bool operator==(const gc_counting_ptr& r) const throw() {
        if (this == &r)
            return true;
        return ptr == r.ptr;
    }

    const bool operator!=(const gc_counting_ptr& r) const throw() {
        return !this->operator ==(r);
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

    template<typename X> friend std::ostream& operator<<(std::ostream&, const gc_counting_ptr<X>&);

private:
    T* ptr;

    void acquire(T* p) throw() {
        if(p)
            p->acquire();
    }

    void release() throw() {
        if(ptr) {
            if(ptr->release() == 0) {
                delete ptr;
            }
        }
    }
};

template<typename T> std::ostream& operator<<(std::ostream& out, const gc_counting_ptr<T>& p) {
    return out << p.ptr;
}

/**
 * Apache Portable Runtime library context
 */
class APRContext {
public:
    APRContext() {
        apr_initialize();
    }
    ~APRContext() {
        apr_terminate();
    }
};

APRContext aprContext;

/**
 * Garbage collected memory pool, using an APR pool.
 */
class gc_pool {
public:
    gc_pool() : aprPool(new APRPool) {
    }

    operator apr_pool_t*() const {
        return aprPool->p;
    }

private:
    class APRPool {
        friend class gc_pool;
        friend class gc_counting_ptr<APRPool>;

        unsigned int refCount;
        apr_pool_t* p;

        APRPool() : refCount(0) {
            apr_pool_create(&p, NULL);
        }

        ~APRPool() {
            apr_pool_destroy(p);
        }

        unsigned int acquire() {
            return gc_add_and_fetch(refCount, (unsigned int)1);
        }

        unsigned int release() {
            return gc_sub_and_fetch(refCount, (unsigned int)1);
        }
    };

    const gc_counting_ptr<APRPool> aprPool;
};

/**
 * Garbage collected pointer to pooled memory.
 */
template<typename T> class gc_pool_ptr {
public:
    gc_pool_ptr(T* ptr = 0) throw() : ptr(ptr) {
    }

    ~gc_pool_ptr() throw() {
    }

    gc_pool_ptr(const gc_pool_ptr& r) throw() : ptr(r.ptr) {
    }

    gc_pool_ptr& operator=(const gc_pool_ptr& r) throw() {
        if(this == &r)
            return *this;
        ptr = r.ptr;
        return *this;
    }

    const bool operator==(const gc_pool_ptr& r) const throw() {
        if (this == &r)
            return true;
        return ptr == r.ptr;
    }

    const bool operator!=(const gc_pool_ptr& r) const throw() {
        return !this->operator ==(r);
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

    template<typename X> friend std::ostream& operator<<(std::ostream&, const gc_pool_ptr<X>&);

private:
    T* ptr;
};

template<typename T> std::ostream& operator<<(std::ostream& out, const gc_pool_ptr<T>& p) {
    return out << p.ptr;
}

/**
 * Cleanup function, called by the APR pool to cleanup registered resources.
 * Calls the allocated object's destructor.
 */
template<typename T> apr_status_t gc_pool_cleanupCallback(void* v) {
    T* t = static_cast<T*>(v);
    t->~T();
    return APR_SUCCESS;
}

/**
 * Returns a pointer to an object allocated from a memory pool.
 */
template<typename T> gc_pool_ptr<T> gc_pool_new(const gc_pool& mp) {

    // Allocate memory from the pool
    void* m = apr_palloc(mp, sizeof(T));

    // Register a cleanup callback
    apr_pool_cleanup_register(mp, m, gc_pool_cleanupCallback<T>, apr_pool_cleanup_null) ;

    // Run the requested type's constructor over the allocated memory
    return new (m) T();
}

}
#endif /* tuscany_gc_hpp */
