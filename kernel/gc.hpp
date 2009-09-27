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

#include <iostream>

namespace tuscany
{

template<typename T> class gc_ptr {
public:
    explicit gc_ptr(T* p = 0) throw() : countingRef(p == 0? 0 : new CountingRef(p)) {
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
            __sync_add_and_fetch(&ref->count, 1);
    }

    void release() throw() {
        if(countingRef) {
            unsigned rc = __sync_sub_and_fetch(&countingRef->count, 1);
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
    explicit gc_aptr(T* p = 0) throw() : countingRef(p == 0? 0 : new CountingRef(p)) {
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
            __sync_add_and_fetch(&ref->count, 1);
    }

    void release() throw() {
        if(countingRef) {
            unsigned rc = __sync_sub_and_fetch(&countingRef->count, 1);
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
    explicit gc_counting_ptr(T* p = 0) throw() : ptr(p) {
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

}
#endif /* tuscany_gc_hpp */
