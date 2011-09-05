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
unsigned long processId() {
    return (unsigned long)getpid();
}

#ifdef WANT_THREADS

/**
 * Returns the current thread id.
 */
unsigned long threadId() {
    return (unsigned long)pthread_self();
}

/**
 * Represents a value which will be know in the future.
 */
template<typename T> class future {

private:
    template<typename X> class futureValue {
    public:
        futureValue() : hasValue(false) {
            pthread_mutex_init(&valueMutex, NULL);
            pthread_cond_init(&valueCond, NULL);
        }

        futureValue(const futureValue& fv) : valueMutex(fv.valueMutex), valueCond(fv.valueCond), hasValue(fv.hasValue), value(fv.value) {
        }

        ~futureValue() {
            //pthread_mutex_destroy(&valueMutex);
            //pthread_cond_destroy(&valueCond);
        }

        bool set(const T& v) {
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

        const T get() {
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
        X value;
    };

    gc_ptr<futureValue<T> > fvalue;

    template<typename X> friend const X get(const future<X>& f);
    template<typename X> friend bool set(const future<X>& f, const X& v);

public:
    future() : fvalue(new (gc_new<futureValue<T> >()) futureValue<T>()) {
    }

    ~future() {
    }

    future(const future& f) : fvalue(f.fvalue) {
    }

    const future& operator=(const future& f) {
        if (&f == this)
            return *this;
        fvalue = f.fvalue;
        return *this;
    }

    const future& operator=(const T& v) const {
        fvalue->set(v);
        return *this;
    }

    operator const T() const {
        return fvalue->get();
    }
};

/**
 * A bounded thread safe queue.
 */
template<typename T> class wqueue {
public:
    wqueue(size_t max) : max(max), size(0), tail(0), head(0), values(new (gc_anew<T>(max)) T[max]) {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&full, NULL);
        pthread_cond_init(&empty, NULL);
    }

    wqueue(const wqueue& wq) : max(wq.max), size(wq.size), tail(wq.tail), head(wq.head), mutex(wq.mutex), full(wq.full), empty(wq.empty), values(wq.values) {
    }

    ~wqueue() {
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
    gc_ptr<T> values;

    template<typename X> friend const size_t enqueue(wqueue<X>& q, const X& v);
    template<typename X> friend const X dequeue(wqueue<X>& q);
};

/**
 * Adds an element to the tail of the queue.
 */
template<typename T> const size_t enqueue(wqueue<T>&q, const T& v) {
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
template<typename T> const T dequeue(wqueue<T>& q) {
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
void *workerThreadFunc(void *arg) {
    int ost;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &ost);
    int ot;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &ot);

    wqueue<lambda<bool()> >* work = reinterpret_cast<wqueue<lambda<bool()> >*>(arg);
    while(dequeue(*work)())
        ;
    return NULL;
}

/**
 * Returns a list of worker threads.
 */
const list<pthread_t> workerThreads(wqueue<lambda<bool()> >& wqueue, const size_t count) {
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
        sharedWorker(size_t max) : work(wqueue<lambda<bool()> >(max)), threads(workerThreads(work, max)) {
        }

        wqueue<lambda<bool()> > work;
        const list<pthread_t> threads;
    };

public:
    worker(size_t max) : w(*(new (gc_new<sharedWorker>()) sharedWorker(max))) {
    }

    worker(const worker& wk) : w(wk.w) {
    }

private:
    sharedWorker& w;

    template<typename X> friend const future<X> submit(worker& w, const lambda<X()>& func);
    friend const bool shutdown(worker& w);
    friend const bool cancel(worker& w);
};

/**
 * Function used to wrap work submitted to a worker.
 */
template<typename R> bool submitFunc(const lambda<R()>& func, const future<R>& fut) {
    fut = func();
    return true;
}

/**
 * Submits work to a worker.
 */
template<typename R> const future<R> submit(worker& w, const lambda<R()>& func) {
    const future<R> fut;
    const lambda<bool()> f = curry(lambda<bool(const lambda<R()>, future<R>)>(submitFunc<R>), func, fut);
    enqueue(w.w.work, f);
    return fut;
}

/**
 * Enqueues shutdown requests.
 */
const bool shutdownEnqueue(const list<pthread_t>& threads, wqueue<lambda<bool()> >& work) {
    if (isNil(threads))
        return true;
    enqueue(work, result(false));
    return shutdownEnqueue(cdr(threads), work);
}

/**
 * Waits for shut down threads to terminate.
 */
const bool shutdownJoin(const list<pthread_t>& threads) {
    if (isNil(threads))
        return true;
    pthread_join(car(threads), NULL);
    return shutdownJoin(cdr(threads));
}

/**
 * Shutdown a worker.
 */
const bool shutdown(worker& w) {
    shutdownEnqueue(w.w.threads, w.w.work);
    shutdownJoin(w.w.threads);
    return true;
}

/**
 * Cancel a worker.
 */
const bool cancel(const list<pthread_t>& threads) {
    if (isNil(threads))
        return true;
    pthread_cancel(car(threads));
    return cancel(cdr(threads));
}

const bool cancel(worker& w) {
    cancel(w.w.threads);
    return true;
}

#else

/**
 * Returns the current thread id.
 */
unsigned long threadId() {
    return 0;
}

#endif

/**
 * Represents a per-thread value.
 */
template<typename T> class perthread_ptr {
public:
    perthread_ptr() : key(createkey()), owner(true), cl(lambda<gc_ptr<T>()>()), managed(false) {
    }

    perthread_ptr(const lambda<gc_ptr<T>()>& cl) : key(createkey()), owner(true), cl(cl), managed(true) {
    }

    ~perthread_ptr() {
        if (owner)
            deletekey(key);
    }

    perthread_ptr(const perthread_ptr& c) : key(c.key), owner(false), cl(c.cl), managed(c.managed) {
    }

    perthread_ptr& operator=(const perthread_ptr& r) throw() {
        if(this == &r)
            return *this;
        key = r.key;
        owner = false;
        cl = r.cl;
        managed = r.managed;
        return *this;
    }

    const perthread_ptr& operator=(const gc_ptr<T>& v) {
        set(v);
        return *this;
    }

    const perthread_ptr& operator=(T* v) {
        set(v);
        return *this;
    }

    const bool operator==(const gc_ptr<T>& r) const throw() {
        return get() == r;
    }

    const bool operator==(T* p) const throw() {
        return get() == p;
    }

    const bool operator!=(const gc_ptr<T>& r) const throw() {
        return !this->operator==(r);
    }

    const bool operator!=(T* p) const throw() {
        return !this->operator==(p);
    }

    T& operator*() const throw() {
        return *get();
    }

    T* operator->() const throw() {
        return get();
    }

    operator gc_ptr<T>() const {
        return get();
    }

    operator T*() const {
        return get();
    }

private:
#ifdef WANT_THREADS
    pthread_key_t createkey() {
        pthread_key_t k;
        pthread_key_create(&k, NULL);
        return k;
    }

    bool deletekey(pthread_key_t k) {
        pthread_key_delete(k);
        return true;
    }

    bool set(const gc_ptr<T>& v) {
        pthread_setspecific(key, (T*)v);
        return true;
    }

    gc_ptr<T> get() const {
        const gc_ptr<T> v = static_cast<T*>(pthread_getspecific(key));
        if (v != NULL || !managed)
            return v;
        const gc_ptr<T> nv = cl();
        pthread_setspecific(key, nv);
        return nv;
    }

#else
    int createkey() {
        return 0;
    }

    bool deletekey(unused int k) {
        return true;
    }

    bool set(const gc_ptr<T>& v) {
        val = v;
        return true;
    }

    gc_ptr<T> get() const {
        if (val != NULL || !managed)
            return val;
        val = cl();
        return val;
    }

#endif

#ifdef WANT_THREADS
    pthread_key_t key;
#else
    int key;
    gc_ptr<T> val;
#endif

    bool owner;
    lambda<const gc_ptr<T>()> cl;
    bool managed;
};

}
#endif /* tuscany_parallel_hpp */
