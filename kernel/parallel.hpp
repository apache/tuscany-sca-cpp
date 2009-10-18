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

#include <pthread.h>
#include <sys/syscall.h>
#include "function.hpp"

namespace tuscany {

/**
 * Returns the current thread id.
 */
unsigned int threadId() {
    return syscall(__NR_gettid);
}

/**
 * Represents a value which will be know in the future.
 */
template<typename T> class future {

private:
    template<typename X> class futureValue {
    public:
        futureValue() :
            refCount(0), hasValue(false) {
            pthread_mutex_init(&valueMutex, NULL);
            pthread_cond_init(&valueCond, NULL);
        }

        ~futureValue() {
            pthread_mutex_destroy(&valueMutex);
            pthread_cond_destroy(&valueCond);
        }

        unsigned int acquire() {
            return __sync_add_and_fetch(&refCount, 1);
        }

        unsigned int release() {
            return __sync_sub_and_fetch(&refCount, 1);
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
        unsigned refCount;
        pthread_mutex_t valueMutex;
        pthread_cond_t valueCond;
        bool hasValue;
        X value;
    };

    gc_counting_ptr<futureValue<T> > fvalue;

    template<typename X> friend const X get(const future<X>& f);
    template<typename X> friend bool set(const future<X>& f, const X& v);

public:
    future() : fvalue(new futureValue<T>()) {
        //std::cout << "future() threadId " << threadId() << "\n";
    }

    ~future() {
        //std::cout << "~future() threadId " << threadId() << "\n";
    }

    future(const future& f) : fvalue(f.fvalue) {
        //std::cout << "future(const future& f) threadId " << threadId() << "\n";
    }

    const future& operator=(const future& f) {
        //std::cout << "future::operator=(const future& f) threadId " << threadId() << "\n";
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
template<typename T> class queue {
public:
    queue(int max) : max(max), size(0), tail(0), head(0), values(new T[max]) {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&full, NULL);
        pthread_cond_init(&empty, NULL);
    }

    ~queue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&full);
        pthread_cond_destroy(&empty);
    }

private:
    const int max;
    int size;
    int tail;
    int head;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
    gc_aptr<T> values;

    template<typename X> friend const int enqueue(queue<X>& q, const X& v);
    template<typename X> friend const X dequeue(queue<X>& q);
};

/**
 * Adds an element to the tail of the queue.
 */
template<typename T> const int enqueue(queue<T>&q, const T& v) {
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
template<typename T> const T dequeue(queue<T>& q) {
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
    queue<lambda<bool()> >* work = reinterpret_cast<queue<lambda<bool()> >*>(arg);
    while(dequeue(*work)())
        ;
    return NULL;
}

/**
 * Returns a list of worker threads.
 */
const list<pthread_t> workerThreads(queue<lambda<bool()> >& queue, const int count) {
    if (count == 0)
        return list<pthread_t>();
    pthread_t thread;
    pthread_create(&thread, NULL, workerThreadFunc, &queue);
    return cons(thread, workerThreads(queue, count - 1));
}

/**
 * A worker, implemented with a work queue and a pool of threads.
 */
class worker {
public:
    worker(int max) : work(queue<lambda<bool()> >(max)), threads(workerThreads(work, max)) {
    }

private:
    queue<lambda<bool()> > work;
    const list<pthread_t> threads;

    template<typename X> friend const future<X> submit(worker& w, const lambda<X()>& func);
    friend const bool shutdown(worker& w);
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
    const lambda<bool()> f = curry(lambda<bool(lambda<R()>, future<R>)>(submitFunc<R>), func, fut);
    enqueue(w.work, f);
    return fut;
}

/**
 * Enqueues shutdown requests.
 */
const bool shutdownEnqueue(const list<pthread_t>& threads, queue<lambda<bool()> >& work) {
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
    shutdownEnqueue(w.threads, w.work);
    shutdownJoin(w.threads);
    return true;
}

}
#endif /* tuscany_parallel_hpp */
