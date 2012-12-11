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

int inci = 0;

const gc_ptr<int> tlsic() {
    const gc_ptr<int> i = new (gc_new<int>()) int();
    *i = 0;
    return i;
}
const perthread_ptr<int> tlsi(tlsic);

#ifdef WANT_THREADS

int addi = 0;
int muxi = 0;

#endif

const bool testAtomicPerf() {
    const int count = 100000;
    {
        const blambda l = []() -> const bool {
            inci = inci + 1;
            return true;
        };
        cout << "Non-atomic inc test " << time(l, 1000, count) << " ms" << endl;
        assert(inci == count + 1000);
    }
#ifdef WANT_THREADS
    {
        const blambda l = []() -> const bool {
            __sync_add_and_fetch(&addi, 1);
            return true;
        };
        cout << "Atomic inc test " << time(l, 1000, count) << " ms" << endl;
        assert(addi == count + 1000);
    }
    {
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        const blambda l = [&mutex]() -> const bool {
            pthread_mutex_lock(&mutex);
            muxi = muxi + 1;
            pthread_mutex_unlock(&mutex);
            return true;
        };
        cout << "Locked inc test " << time(l, 1000, count) << " ms" << endl;
        assert(muxi == count + 1000);
        pthread_mutex_destroy(&mutex);
    }
#endif
    {
        const blambda l = []() -> const bool {
            *tlsi = *tlsi + 1;
            return true;
        };
        cout << "Thread local inc test " << time(l, 1000, count) << " ms" << endl;
        assert(*tlsi == count + 1000);
    }
    return true;
}

#ifdef WANT_THREADS

const int mtsquare(const int x) {
    for(int i = 0; i < 10000000; i++)
        ;
    return x * x;
}

const list<future<int> > submitSquares(worker& w, const int max, const int i) {
    if (i == max)
        return list<future<int> >();
    const lambda<const int()> func = curry(lambda<const int(const int)> (mtsquare), i);
    return cons(submit(w, func), submitSquares(w, max, i + 1));
}

bool checkSquareResults(const list<future<int> > r, int i) {
    if (isNil(r))
        return true;
    assert(car(r) == i * i);
    checkSquareResults(cdr(r), i + 1);
    return true;
}

const gc_ptr<unsigned long> tlsvc() {
    const gc_ptr<unsigned long> i = new (gc_new<unsigned long>()) unsigned long();
    *i = 0l;
    return i;
}
const perthread_ptr<unsigned long> tlsv(tlsvc);

const long int tlsset(gc_ptr<wqueue<bool>> wq, gc_ptr<wqueue<bool>> xq) {
    const unsigned long v = *tlsv;
    *tlsv = threadId();
    enqueue(*xq, true);
    dequeue(*wq);
    return v;
}

const bool tlscheck(gc_ptr<wqueue<bool>> wq, gc_ptr<wqueue<bool>> xq) {
    const bool r = *tlsv == threadId();
    enqueue(*xq, true);
    dequeue(*wq);
    return r;
}

const bool waitForWorkers(wqueue<bool>& xq, const int n) {
    if (n == 0)
        return true;
    dequeue(xq);
    return waitForWorkers(xq, n - 1);
}

const bool unblockWorkers(wqueue<bool>& wq, const int n) {
    if (n == 0)
        return true;
    enqueue(wq, true);
    return unblockWorkers(wq, n - 1);
}

const list<future<long int> > submitTLSSets(worker& w, wqueue<bool>& wq, wqueue<bool>& xq, const int max, const int i) {
    if (i == max)
        return list<future<long int> >();
    const lambda<const long int()> func = curry(lambda<const long int(gc_ptr<wqueue<bool>>, gc_ptr<wqueue<bool>>)>(tlsset), (gc_ptr<wqueue<bool>>)&wq, (gc_ptr<wqueue<bool>>)&xq);
    return cons(submit(w, func), submitTLSSets(w, wq, xq, max, i + 1));
}

const bool checkTLSSets(const list<future<long int> > s) {
    if (isNil(s))
        return true;
    assert(car(s) == 0);
    return checkTLSSets(cdr(s));
}

const list<future<bool> > submitTLSChecks(worker& w, wqueue<bool>& wq, wqueue<bool>& xq, const int max, const int i) {
    if (i == max)
        return list<future<bool> >();
    const blambda func = curry(lambda<const bool(gc_ptr<wqueue<bool>>, gc_ptr<wqueue<bool>>)>(tlscheck), (gc_ptr<wqueue<bool>>)&wq, (gc_ptr<wqueue<bool>>)&xq);
    return cons(submit(w, func), submitTLSChecks(w, wq, xq, max, i + 1));
}

const bool checkTLSResults(const list<future<bool> > r) {
    if (isNil(r))
        return true;
    assert(car(r) == true);
    return checkTLSResults(cdr(r));
}

const bool testWorker() {
    const int max = 100;
    worker w(max);
    {
        const lambda<const int()> func = curry(lambda<const int(const int)> (mtsquare), 2);
        assert(submit(w, func) == 4);
    }
    {
        const list<future<int> > r(submitSquares(w, max, 0));
        checkSquareResults(r, 0);
    }
    {
        wqueue<bool> wq(max);
        unblockWorkers(wq, max);
        waitForWorkers(wq, max);
        unblockWorkers(wq, max);
        waitForWorkers(wq, max);
    }
    {
        wqueue<bool> wq(max);
        wqueue<bool> xq(max);
        const list<future<long int> > s(submitTLSSets(w, wq, xq, max, 0));
        waitForWorkers(xq, max);
        unblockWorkers(wq, max);
        checkTLSSets(s);
        const list<future<bool> > r(submitTLSChecks(w, wq, xq, max, 0));
        waitForWorkers(xq, max);
        unblockWorkers(wq, max);
        checkTLSResults(r);
    }
    shutdown(w);
    return true;
}

#endif

}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testAtomicPerf();
#ifdef WANT_THREADS
    tuscany::testWorker();
#else
    tuscany::cout << "Skipped multi-thread tests" << tuscany::endl;
#endif

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
