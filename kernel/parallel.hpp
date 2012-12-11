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

#ifndef tuscany_parallel_hpp
#define tuscany_parallel_hpp

/**
 * Simple parallel work execution functions.
 */

#include <unistd.h>
#ifdef WANT_THREADS
#include <pthread.h>
#endif

#include "function.hpp"
#include "list.hpp"

namespace tuscany {

/**
 * Returns the current process id.
 */
inline const unsigned long processId() noexcept {
    return (unsigned long)getpid();
}

#ifdef WANT_THREADS

/**
 * Returns the current thread id.
 */
inline const unsigned long threadId()  noexcept{
    return (unsigned long)pthread_self();
}

/**
 * Represents a value which will be know in the future.
 */
template<typename T> class future {

private:
    template<typename X> class futureValue {
    public:
        inline futureValue() noexcept : hasValue(false) {
            pthread_mutex_init(&valueMutex, NULL);
            pthread_cond_init(&valueCond, NULL);
        }

        inline futureValue(const futureValue& fv) noexcept : valueMutex(fv.valueMutex), valueCond(fv.valueCond), hasValue(fv.hasValue), value(fv.value) {
        }

        inline ~futureValue() noexcept {
            //pthread_mutex_destroy(&valueMutex);
            //pthread_cond_destroy(&valueCond);
        }

        inline const bool set(const T& v) noexcept {
            pthread_mutex_lock(&valueMutex);
            if(hasValue) {
                pthread_mutex_unlock(&valueMutex);
                return false;
            }
            hasValue = true;
            value = v;
            pthread_mutex_unlock(&valueMutex);
            pthread_cond_broadcast(&valueCond);
            return true;
        }

        inline const T get() noexcept {
            pthread_mutex_lock(&valueMutex);
            while(!hasValue) {
                pthread_cond_wait(&valueCond, &valueMutex);
            }
            const T& v = value;
            pthread_mutex_unlock(&valueMutex);
            return v;
        }

    private:
        pthread_mutex_t valueMutex;
        pthread_cond_t valueCond;
        bool hasValue;
        gc_mutable_ref<X> value;
    };

    const gc_ptr<futureValue<T> > fvalue;

    template<typename X> friend const X get(const future<X>& f) noexcept;
    template<typename X> friend bool set(const future<X>& f, const X& v) noexcept;

public:
    inline future() noexcept : fvalue(new (gc_new<futureValue<T> >()) futureValue<T>()) {
    }

    inline ~future() noexcept {
    }

    inline future(const future& f) noexcept : fvalue(f.fvalue) {
    }

    const future& operator=(const future& f) = delete;

    inline const future& operator=(const T& v) const noexcept {
        fvalue->set(v);
        return *this;
    }

    inline operator const T() const noexcept {
        return fvalue->get();
    }
};

/**
 * A bounded thread safe queue.
 */
template<typename T> class wqueue {
public:
    inline wqueue(size_t max) noexcept : max(max), size(0), tail(0), head(0), values(new (gc_anew<gc_mutable_ref<T> >(max)) gc_mutable_ref<T>[max]) {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&full, NULL);
        pthread_cond_init(&empty, NULL);
    }

    inline wqueue(const wqueue& wq) noexcept : max(wq.max), size(wq.size), tail(wq.tail), head(wq.head), mutex(wq.mutex), full(wq.full), empty(wq.empty), values(wq.values) {
    }

    inline ~wqueue() {
        //pthread_mutex_destroy(&mutex);
        //pthread_cond_destroy(&full);
        //pthread_cond_destroy(&empty);
    }

private:
    const size_t max;
    size_t size;
    size_t tail;
    size_t head;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
    gc_ptr<gc_mutable_ref<T> > values;

    template<typename X> friend const size_t enqueue(wqueue<X>& q, const X& v) noexcept;
    template<typename X> friend const X dequeue(wqueue<X>& q) noexcept;
};

/**
 * Adds an element to the tail of the queue.
 */
template<typename T> inline const size_t enqueue(wqueue<T>&q, const T& v) noexcept {
    pthread_mutex_lock(&q.mutex);
    while(q.size == q.max)
        pthread_cond_wait(&q.full, &q.mutex);
    q.values[q.tail] = v;
    q.tail = (q.tail + 1) % q.max;
    q.size++;
    pthread_mutex_unlock(&q.mutex);
    pthread_cond_broadcast(&q.empty);
    return q.size;
}

/**
 * Returns the element at the head of the queue.
 */
template<typename T> inline const T dequeue(wqueue<T>& q) noexcept {
    pthread_mutex_lock(&q.mutex);
    while(q.size == 0)
        pthread_cond_wait(&q.empty, &q.mutex);
    const T v = q.values[q.head];
    q.head = (q.head + 1) % q.max;
    q.size--;
    pthread_mutex_unlock(&q.mutex);
    pthread_cond_broadcast(&q.full);
    return v;
}

/**
 * The worker thread function.
 */
inline void* workerThreadFunc(void* arg) noexcept {
    int ost;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &ost);
    int ot;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &ot);

    wqueue<blambda >* work = reinterpret_cast<wqueue<blambda >*>(arg);
    while(dequeue(*work)())
        ;
    return NULL;
}

/**
 * Returns a list of worker threads.
 */
inline const list<pthread_t> workerThreads(wqueue<blambda >& wqueue, const size_t count) noexcept {
    if (count == 0)
        return list<pthread_t>();
    pthread_t thread;
    pthread_create(&thread, NULL, workerThreadFunc, &wqueue);
    return cons(thread, workerThreads(wqueue, count - 1));
}

/**
 * A worker, implemented with a work queue and a pool of threads.
 */
class worker {
private:

    // The worker holds a reference to a sharedWorker, to avoid non-thread-safe
    // copies of the queue and thread pool when a worker is copied 
    class sharedWorker {
    public:
        inline sharedWorker(size_t max) noexcept : work(wqueue<blambda >(max)), threads(workerThreads(work, max)) {
        }

        wqueue<blambda > work;
        const list<pthread_t> threads;
    };

public:
    inline worker(size_t max) noexcept : w(*(new (gc_new<sharedWorker>()) sharedWorker(max))) {
    }

    inline worker(const worker& wk) noexcept : w(wk.w) {
    }

private:
    sharedWorker& w;

    template<typename X> friend const future<X> submit(const worker& w, const lambda<const X()>& func) noexcept;
    friend const bool shutdown(const worker& w) noexcept;
    friend const bool cancel(const worker& w) noexcept;
};

/**
 * Function used to wrap work submitted to a worker.
 */
template<typename R> inline const bool submitFunc(const lambda<const R()>& func, const future<R>& fut) noexcept {
    fut = func();
    return true;
}

/**
 * Submits work to a worker.
 */
template<typename R> inline const future<R> submit(const worker& w, const lambda<const R()>& func) noexcept {
    const future<R> fut;
    const blambda f = curry(lambda<const bool(const lambda<const R()>, future<R>)>(submitFunc<R>), func, fut);
    enqueue(w.w.work, f);
    return fut;
}

/**
 * Enqueues shutdown requests.
 */
inline const bool shutdownEnqueue(const list<pthread_t>& threads, wqueue<blambda>& work) noexcept {
    if (isNil(threads))
        return true;
    enqueue(work, result(false));
    return shutdownEnqueue(cdr(threads), work);
}

/**
 * Waits for shut down threads to terminate.
 */
inline const bool shutdownJoin(const list<pthread_t>& threads) noexcept {
    if (isNil(threads))
        return true;
    pthread_join(car(threads), NULL);
    return shutdownJoin(cdr(threads));
}

/**
 * Shutdown a worker.
 */
inline const bool shutdown(const worker& w) noexcept {
    shutdownEnqueue(w.w.threads, w.w.work);
    shutdownJoin(w.w.threads);
    return true;
}

/**
 * Cancel a worker.
 */
inline const bool cancel(const list<pthread_t>& threads) noexcept {
    if (isNil(threads))
        return true;
    pthread_cancel(car(threads));
    return cancel(cdr(threads));
}

inline const bool cancel(const worker& w) noexcept {
    cancel(w.w.threads);
    return true;
}

#else

/**
 * Returns the current thread id.
 */
inline const unsigned long threadId() noexcept {
    return 0;
}

#endif

/**
 * Represents a per-thread value.
 */
template<typename T> class perthread_ptr {
public:
    inline perthread_ptr() noexcept : key(createkey()), owner(true), cl(lambda<const gc_ptr<T>()>()), managed(false) {
    }

    inline perthread_ptr(const lambda<const gc_ptr<T>()>& cl) noexcept : key(createkey()), owner(true), cl(cl), managed(true) {
    }

    inline ~perthread_ptr() noexcept {
        if (owner)
            deletekey(key);
    }

    inline perthread_ptr(const perthread_ptr& c) noexcept : key(c.key), owner(false), cl(c.cl), managed(c.managed) {
    }

    perthread_ptr& operator=(const perthread_ptr& r) = delete;

    inline const perthread_ptr& operator=(const gc_ptr<T>& v) const noexcept {
        set(v);
        return *this;
    }

    inline const perthread_ptr& operator=(T* const v) const noexcept {
        set(v);
        return *this;
    }

    inline const bool operator==(const gc_ptr<T>& r) const noexcept {
        return get() == r;
    }

    inline const bool operator==(const T* const p) const noexcept {
        return get() == p;
    }

    inline const bool operator!=(const gc_ptr<T>& r) const noexcept {
        return !this->operator==(r);
    }

    inline const bool operator!=(const T* const p) const noexcept {
        return !this->operator==(p);
    }

    inline T& operator*() const noexcept {
        return *get();
    }

    inline T* const operator->() const noexcept {
        return get();
    }

    inline operator gc_ptr<T>() const {
        return get();
    }

    inline operator T* const () const {
        return get();
    }

private:
#ifdef WANT_THREADS
    inline const pthread_key_t createkey() noexcept {
        pthread_key_t k;
        pthread_key_create(&k, NULL);
        return k;
    }

    inline const bool deletekey(pthread_key_t k) noexcept {
        pthread_key_delete(k);
        return true;
    }

    inline const bool set(const gc_ptr<T>& v) const noexcept {
        pthread_setspecific(key, (T*)v);
        return true;
    }

    inline const gc_ptr<T> get() const noexcept {
        const gc_ptr<T> v = (T*)(pthread_getspecific(key));
        if (v != NULL || !managed)
            return v;
        const gc_ptr<T> nv = cl();
        pthread_setspecific(key, (T*)nv);
        return nv;
    }

#else
    inline const gc_ptr<gc_ptr<T> > createkey() noexcept {
        return new (gc_new<gc_ptr<T> >()) gc_ptr<T>();
    }

    inline const bool deletekey(unused gc_ptr<gc_ptr<T> > k) noexcept {
        return true;
    }

    inline const bool set(const gc_ptr<T>& v) const noexcept {
        *key = v;
        return true;
    }

    inline const gc_ptr<T> get() const noexcept {
        if (*key != NULL || !managed)
            return *key;
        *key = cl();
        return *key;
    }

#endif

#ifdef WANT_THREADS
    const pthread_key_t key;
#else
    const gc_ptr<gc_ptr<T> >key;
#endif

    const bool owner;
    const lambda<const gc_ptr<T>()> cl;
    const bool managed;
};

}
#endif /* tuscany_parallel_hpp */
