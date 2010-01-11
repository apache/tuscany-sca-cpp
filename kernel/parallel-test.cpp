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

/**
 * Test parallel functions.
 */

#include <assert.h>
#include "stream.hpp"
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "perf.hpp"
#include "parallel.hpp"

namespace tuscany {

#ifdef WANT_THREADS

int inci = 0;

struct incPerf {
    incPerf() {
    }
    const bool operator()() const {
        inci = inci + 1;
        return true;
    }
};

int addi = 0;

struct addAndFetchPerf {
    addAndFetchPerf() {
    }
    const bool operator()() const {
        __sync_add_and_fetch(&addi, 1);
        return true;
    }
};

int muxi = 0;

struct mutexPerf {
    pthread_mutex_t* mutex;
    mutexPerf(pthread_mutex_t* mutex) : mutex(mutex) {
    }
    const bool operator()() const {
        pthread_mutex_lock(mutex);
        muxi = muxi + 1;
        pthread_mutex_unlock(mutex);
        return true;
    }
};

__thread int tlsi = 0;

struct tlsPerf {
    tlsPerf() {
    }
    const bool operator()() const {
        tlsi = tlsi + 1;
        return true;
    }
};

bool testAtomicPerf() {
    const int count = 100000;
    {
        const lambda<bool()> l = incPerf();
        cout << "Non-atomic inc test " << time(l, 1000, count) << " ms" << endl;
        assert(inci == count + 1000);
    }
    {
        const lambda<bool()> l = addAndFetchPerf();
        cout << "Atomic inc test " << time(l, 1000, count) << " ms" << endl;
        assert(addi == count + 1000);
    }
    {
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        const lambda<bool()> l = mutexPerf(&mutex);
        cout << "Locked inc test " << time(l, 1000, count) << " ms" << endl;
        assert(muxi == count + 1000);
        pthread_mutex_destroy(&mutex);
    }
    {
        const lambda<bool()> l = tlsPerf();
        cout << "Thread local inc test " << time(l, 1000, count) << " ms" << endl;
        assert(tlsi == count + 1000);
    }
    return true;
}

const int mtsquare(const int x) {
    for(int i = 0; i < 10000000; i++)
        ;
    return x * x;
}

bool checkResults(const list<future<int> > r, int i) {
    if (isNil(r))
        return true;
    assert(car(r) == i * i);
    checkResults(cdr(r), i + 1);
    return true;
}

const list<future<int> > submitSquares(worker& w, const int max, const int i) {
    if (i == max)
        return list<future<int> >();
    const lambda<int()> func = curry(lambda<int(const int)> (mtsquare), i);
    return cons(submit(w, func), submitSquares(w, max, i + 1));
}

bool testWorker() {
    worker w(20);
    {
        const lambda<int()> func = curry(lambda<int(const int)> (mtsquare), 2);
        assert(submit(w, func) == 4);
    }
    {
        const int max = 20;
        const list<future<int> > r(submitSquares(w, max, 0));
        checkResults(r, 0);
    }
    shutdown(w);
    return true;
}

#endif

}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

#ifdef WANT_THREADS
    tuscany::testAtomicPerf();
    tuscany::testWorker();
#else
    tuscany::cout << "Skipped multi-thread tests" << tuscany::endl;
#endif

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
