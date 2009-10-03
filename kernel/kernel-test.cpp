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
 * Test core utils.
 */

#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include "function.hpp"
#include "list.hpp"
#include "parallel.hpp"
#include "value.hpp"
#include "xml.hpp"

namespace tuscany {

struct inc {
    int i;
    inc(int i) :
        i(i) {
    }
    const int operator()(const int x) const {
        return x + i;
    }
};

const int square(const int x) {
    return x * x;
}

bool testFunction() {
    const lambda<int(int)> sq(square);
    assert(sq(2) == 4);

    const lambda<int(int i)> incf(inc(10));
    assert(incf(1) == 11);
    return true;
}

bool testFunctionGC() {
    resetLambdaCounters();
    testFunction();
    assert(countLambdas == 0);
    return true;
}

int countElements = 0;

struct Element {
    int i;

    Element() :
        i(0) {
        countElements++;
    }

    Element(int i) :
        i(i) {
        countElements++;
    }

    Element(const Element& o) :
        i(o.i) {
        countElements++;
    }

    ~Element() {
        countElements--;
    }

    const bool operator==(const Element& o) const {
        return o.i == i;
    }
};

bool testCons() {
    assert(car(cons(2, makeList(3))) == 2);
    assert(car(cdr(cons(2, makeList(3)))) == 3);
    assert(isNil(cdr(cdr(cons(2, makeList(3))))));

    assert(cons(Element(1), makeList(Element(2))) == makeList(Element(1), Element(2)));
    return true;
}

bool testListGC() {
    resetLambdaCounters();
    resetListCounters();
    countElements = 0;
    testCons();
    assert(countLambdas == 0);
    assert(countlists == 0);
    assert(countElements == 0);
    return true;
}

bool testOut() {
    std::ostringstream os1;
    os1 << list<int> ();
    assert(os1.str() == "()");

    std::ostringstream os2;
    os2 << makeList(1, 2, 3);
    assert(os2.str() == "(1, (2, (3, ())))");
    return true;
}

bool testEquals() {
    assert(list<int>() == list<int>());
    assert(makeList(1, 2) == makeList(1, 2));
    assert(list<int>() != makeList(1, 2));
    assert(makeList(1, 2, 3) == makeList(1, 2, 3));
    assert(makeList(1, 2) != makeList(1, 2, 3));
    return true;
}

bool testLength() {
    assert(0 == length(list<int>()));
    assert(1 == length(makeList(1)));
    assert(2 == length(cons(1, makeList(2))));
    return true;
}

bool testAppend() {
    assert(car(append(makeList(1), makeList(2))) == 1);
    assert(car(cdr(append(makeList(1), makeList(2)))) == 2);
    assert(car(cdr(cdr(append(makeList(1), makeList(2, 3))))) == 3);
    assert(isNil(cdr(cdr(cdr(append(makeList(1), makeList(2, 3)))))));
    return true;
}

struct Complex {
    int x;
    int y;
    Complex() {
    }
    Complex(int x, int y) :
        x(x), y(y) {
    }
};

bool testComplex() {
    const list<Complex> p = makeList(Complex(1, 2), Complex(3, 4));
    assert(car(p).x == 1);
    assert(car(cdr(p)).x == 3);
    assert(isNil(cdr(cdr(p))));
    return true;
}

bool testMap() {
    assert(isNil(map((lambda<int(int)>)square, list<int>())));

    const list<int> m = map((lambda<int(int)> )square, makeList(2, 3));
    assert(car(m) == 4);
    assert(car(cdr(m)) == 9);

    return true;
}

const int add(const int x, const int y) {
    return x + y;
}

bool testReduce() {
    const lambda<int(int, int)> r(add);
    assert(reduce(r, 0, makeList(1, 2, 3)) == 6);
    return true;
}

bool isPositive(int x) {
    if(x >= 0)
        return true;
    else
        return false;
}

bool testFilter() {
    lambda<bool(int)> f(isPositive);
    assert(car(filter(f, makeList(1, -1, 2, -2))) == 1);
    assert(cadr(filter(f, makeList(1, -1, 2, -2))) == 2);
    return true;
}

bool testMember() {
    assert(isNil(member(4, makeList(1, 2, 3))));
    assert(car(member(1, makeList(1, 2, 3))) == 1);
    assert(car(member(2, makeList(1, 2, 3))) == 2);
    assert(car(member(3, makeList(1, 2, 3))) == 3);
    return true;
}

bool testReverse() {
    assert(isNil(reverse(list<int>())));
    assert(car(reverse(makeList(1, 2, 3))) == 3);
    assert(cadr(reverse(makeList(1, 2, 3))) == 2);
    return true;
}

double testSeqMap(double x) {
    return x;
}

double testSeqReduce(double v, double accum) {
    return accum + 1.0;
}

bool testSeq() {
    resetLambdaCounters();
    resetListCounters();

    list<double> s = seq(0.0, 1000.0);
    assert(1001 == length(s));
    printLambdaCounters();
    printListCounters();

    assert(1001 == length(map(lambda<double(double)>(testSeqMap), s)));

    assert(801 == length(member(200.0, s)));
    assert(201 == length(member(200.0, reverse(s))));

    assert(1001 == reduce(lambda<double(double, double)>(testSeqReduce), 0.0, s));
    printLambdaCounters();
    printListCounters();

    return true;
}

value valueSquare(list<value> x) {
    return (int)car(x) * (int)car(x);
}

bool testValue() {
    assert(value(true) == value(true));
    assert(value(1) == value(1));
    assert(value("abcd") == value("abcd"));
    lambda<value(list<value>&)> vl(valueSquare);
    assert(value(vl) == value(vl));
    assert(value(makeList<value>(1, 2)) == value(makeList<value>(1, 2)));
    return true;
}

bool testValueGC() {
    resetLambdaCounters();
    resetListCounters();
    resetValueCounters();
    testValue();
    assert(countValues == 0);
    assert(countLambdas == 0);
    assert(countlists == 0);
    return true;
}

double fib_aux(double n, double a, double b) {
    if(n == 0.0)
        return a;
    return fib_aux(n - 1.0, b, a + b);
}

double fib(double n) {
    return fib_aux(n, 0.0, 1.0);
}

bool testCppPerf() {
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);

    list<double> s = seq(0.0, 999.0);
    list<double> r = map((lambda<double(double)> )fib, s);
    assert(1000 == length(r));

    gettimeofday(&end, NULL);
    //long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
    //std::cout << "Fib cpp perf test " << t << " ms" << std::endl;
    return true;
}

bool testAtomicPerf() {
    struct timeval start;
    struct timeval end;
    {
        gettimeofday(&start, NULL);
        for(int i = 0; i < 10000000;)
            i = i + 1;
        gettimeofday(&end, NULL);
        //long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        //std::cout << "Loop test " << t << " ms" << std::endl;
    }
    {
        gettimeofday(&start, NULL);
        for(int i = 0; i < 10000000;)
            __sync_add_and_fetch(&i, 1);
        gettimeofday(&end, NULL);
        //long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        //std::cout << "Loop atomic test " << t << " ms" << std::endl;
    }
    {
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        gettimeofday(&start, NULL);
        for(int i = 0; i < 10000000;) {
            pthread_mutex_lock(&mutex);
            i = i + 1;
            pthread_mutex_unlock(&mutex);
        }
        gettimeofday(&end, NULL);
        pthread_mutex_destroy(&mutex);
        //long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        //std::cout << "Loop mutex test " << t << " ms" << std::endl;
    }
    return true;
}

const int mtsquare(const int x) {
    //std::cout << "thread " << threadId() << " mtsquare(" << x << ")\n";
    for(int i = 0; i < 10000000; i++)
        ;
    return x * x;
}

bool testWorker() {
    worker w(10);
    {
        const lambda<int()> func = curry(lambda<int(int)> (mtsquare), 2);
        assert(submit(w, func) == 4);
    }
    {
        const int max = 10;

        list<future<int> > r;
        for(int i = 0; i < max; i++) {
            const lambda<int()> func = curry(lambda<int(int)> (mtsquare), i);
            r = cons(submit(w, func), r);
        }
        for(int i = max - 1; i >= 0; i--) {
            assert(car(r) == i * i);
            r = cdr(r);
        }
    }
    shutdown(w);
    return true;
}

const std::string currencyXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<composite xmlns=\"http://docs.oasis-open.org/ns/opencsa/sca/200903\" "
"xmlns:t=\"http://tuscany.apache.org/xmlns/sca/1.1\" "
"targetNamespace=\"http://services\" "
"name=\"currency\">"
"<component name=\"CurrencyConverterWebService\">"
"<implementation.java class=\"services.CurrencyConverterImpl\"/>"
"<service name=\"CurrencyConverter\">"
"<binding.ws/>"
"</service>"
"</component>"
"<component name=\"CurrencyConverterWebService2\">"
"<implementation.java class=\"services.CurrencyConverterImpl2\"/>"
"<service name=\"CurrencyConverter2\">"
"<binding.atom/>"
"</service>"
"</component>"
"</composite>"
"\n";

const bool isName(const value& token) {
    return isAttribute(token) && attributeName(token) == "name";
}

bool testReadXML() {
    std::istringstream is(currencyXML);
    const list<value> currency = readXML(is);

    const value composite = car(currency);
    assert(isElement(composite));
    assert(elementName(composite) == "composite");
    assert(!elementHasText(composite));

    assert(attributeText(car(filter(lambda<bool(value)>(isName), elementChildren(composite)))) == "currency");
    return true;
}

bool testWriteXML() {
    std::istringstream is(currencyXML);
    const list<value> currency = readXML(is);
    std::ostringstream os;
    writeXML(currency, os);
    assert(os.str() == currencyXML);
    return true;
}

}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::testFunction();
    tuscany::testFunctionGC();
    tuscany::testCons();
    tuscany::testListGC();
    tuscany::testOut();
    tuscany::testEquals();
    tuscany::testLength();
    tuscany::testAppend();
    tuscany::testComplex();
    tuscany::testMap();
    tuscany::testReduce();
    tuscany::testFilter();
    tuscany::testMember();
    tuscany::testReverse();
    tuscany::testSeq();
    tuscany::testValue();
    tuscany::testValueGC();
    tuscany::testCppPerf();
    tuscany::testAtomicPerf();
    tuscany::testWorker();
    tuscany::testReadXML();
    tuscany::testWriteXML();

    std::cout << "OK" << std::endl;

    return 0;
}
