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

#ifndef tuscany_cache_hpp
#define tuscany_cache_hpp

/**
 * Simple local cache monad implementation.
 */

#ifdef _REENTRANT
#include <pthread.h>
#endif
#include <sys/stat.h>

#include <string>
#include "value.hpp"
#include "monad.hpp"

namespace tuscany {

/**
 * Cached monad. Used to represent a value that can be cached.
 * To get the value in the monad, just cast it to the value type.
 */
template<typename V> class cached {
public:
    cached() : mutex(mutex_init()), mtime(0) {
    }

    cached(const lambda<V()>& lvalue, const lambda<unsigned long()> ltime) : mutex(mutex_init()), lvalue(lvalue), ltime(ltime), mtime(0) {
    }

    cached(const lambda<V()>& lvalue, const lambda<unsigned long()> ltime, const unsigned long mtime, const V& v) : mutex(mutex_init()), lvalue(lvalue), ltime(ltime), mtime(mtime), v(v) {
    }

    cached(const cached<V>& c) : mutex(mutex_init()), lvalue(c.lvalue), ltime(c.ltime), mtime(c.mtime), v(c.v) {
    }

    ~cached() {
#ifdef _REENTRANT
        pthread_mutex_destroy(mutex);
#endif
    }

    const cached<V>& operator=(const cached<V>& c) {
        if(this == &c)
            return *this;
#ifdef _REENTRANT
        pthread_mutex_lock(mutex);
#endif
        this->lvalue = c.lvalue;
        this->ltime = c.ltime;
        this->mtime = c.mtime;
        this->v = c.v;
#ifdef _REENTRANT
        pthread_mutex_unlock(mutex);
#endif
        return *this;
    }

    const bool operator!=(const cached<V>& m) const {
        return !this->operator==(m);
    }

    const bool operator==(const cached<V>& m) const {
        if (this == &m)
            return true;
#ifdef _REENTRANT
        pthread_mutex_lock(mutex);
#endif
        const bool r = mtime == m.mtime && v == m.v;
#ifdef _REENTRANT
        pthread_mutex_unlock(mutex);
#endif
        return r;
    }

private:
#ifdef _REENTRANT
    pthread_mutex_t* mutex;
    pthread_mutex_t* mutex_init() {
        pthread_mutex_t* mx = new pthread_mutex_t();
        pthread_mutex_init(mx, NULL);
        return mx;
    }
#else
    void* mutex;
    void* mutex_init() {
        return NULL;
    }
#endif

    lambda<V()> lvalue;
    lambda<time_t()> ltime;
    unsigned long mtime;
    V v;

    template<typename X> friend const cached<X> latest(const cached<X>& c);
    template<typename X> friend const X content(const cached<X>& c);
    template<typename X> friend std::ostream& operator<<(std::ostream& out, const cached<X>& c);
};

/**
 * Write a cached monad to a stream.
 */
template<typename V> std::ostream& operator<<(std::ostream& out, const cached<V>& c) {
    out << c.v;
    return out;
}

/**
 * Returns the latest value of a cached monad.
 */
template<typename V> const cached<V> latest(const cached<V>& c) {
    unsigned long nt = c.ltime();
    if (nt == c.mtime)
        return c;
    return cached<V>(c.lvalue, c.ltime, nt, c.lvalue());
}

/**
 * Returns the content of a cached monad.
 */
template<typename V> const V content(const cached<V>& c) {
    return c.v;
}

/**
 * Returns the latest modification time of a file.
 */
const unsigned long latestFileTime(const std::string& path) {
    struct stat st;
    int rc = stat(path.c_str(), &st);
    if (rc < 0)
        return 0;
    return st.st_mtim.tv_nsec;
}

}

#endif /* tuscany_cache_hpp */
