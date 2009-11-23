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
#include "slist.hpp"
#include "parallel.hpp"
#include "value.hpp"
#include "element.hpp"
#include "xml.hpp"
#include "monad.hpp"
#include "dynlib.hpp"

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

int mapLambda(lambda<int(int)> f, int v) {
    return f(v);
}

bool testLambda() {
    const lambda<int(int)> sq(square);
    assert(sq(2) == 4);
    assert(mapLambda(sq, 2) == 4);
    assert(mapLambda(square, 2) == 4);

    const lambda<int(int)> incf(inc(10));
    assert(incf(1) == 11);
    assert(mapLambda(incf, 1) == 11);
    assert(mapLambda(inc(10), 1) == 11);

    lambda<int(int)> l;
    l = incf;
    assert(l(1) == 11);
    l = square;
    assert(l(2) == 4);
    return true;
}

bool testLambdaGC() {
    resetLambdaCounters();
    testLambda();
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
    assert(car(cons(2, mklist(3))) == 2);
    assert(car(cdr(cons(2, mklist(3)))) == 3);
    assert(isNil(cdr(cdr(cons(2, mklist(3))))));

    assert(cons(Element(1), mklist(Element(2))) == mklist(Element(1), Element(2)));
    return true;
}

bool testSet() {
    list<int> l = mklist(1, 2, 3);
    setCar(l, 4);
    setCdr(l, mklist(5, 6));
    assert(car(l) == 4);
    assert(cadr(l) == 5);
    assert(caddr(l) == 6);
    assert(isNil(cdddr(l)));
    return true;
}

bool testListGC() {
    resetLambdaCounters();
    resetListCounters();
    countElements = 0;
    testCons();
    testSet();
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
    os2 << mklist(1, 2, 3);
    assert(os2.str() == "(1 2 3)");
    return true;
}

bool testEquals() {
    assert(list<int>() == list<int>());
    assert(mklist(1, 2) == mklist(1, 2));
    assert(list<int>() != mklist(1, 2));
    assert(mklist(1, 2, 3) == mklist(1, 2, 3));
    assert(mklist(1, 2) != mklist(1, 2, 3));
    return true;
}

bool testLength() {
    assert(0 == length(list<int>()));
    assert(1 == length(mklist(1)));
    assert(2 == length(cons(1, mklist(2))));
    return true;
}

bool testAppend() {
    assert(car(append(mklist(1), mklist(2))) == 1);
    assert(car(cdr(append(mklist(1), mklist(2)))) == 2);
    assert(car(cdr(cdr(append(mklist(1), mklist(2, 3))))) == 3);
    assert(isNil(cdr(cdr(cdr(append(mklist(1), mklist(2, 3)))))));

    list<int> l;
    l << 1 << 2 << 3;
    assert(l == mklist(1, 2, 3));
    assert(list<int>() << 1 << 2 << 3 == mklist(1, 2, 3));
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
    const list<Complex> p = mklist(Complex(1, 2), Complex(3, 4));
    assert(car(p).x == 1);
    assert(car(cdr(p)).x == 3);
    assert(isNil(cdr(cdr(p))));
    return true;
}

bool testMap() {
    assert(isNil(map<int, int>(square, list<int>())));

    const list<int> m = map<int, int>(square, mklist(2, 3));
    assert(car(m) == 4);
    assert(car(cdr(m)) == 9);

    return true;
}

const int add(const int x, const int y) {
    return x + y;
}

bool testReduce() {
    const lambda<int(int, int)> r(add);
    assert(reduce(r, 0, mklist(1, 2, 3)) == 6);
    return true;
}

bool isPositive(int x) {
    if(x >= 0)
        return true;
    else
        return false;
}

bool testFilter() {
    assert(car(filter<int>(isPositive, mklist(1, -1, 2, -2))) == 1);
    assert(cadr(filter<int>(isPositive, mklist(1, -1, 2, -2))) == 2);
    return true;
}

bool testMember() {
    assert(isNil(member(4, mklist(1, 2, 3))));
    assert(car(member(1, mklist(1, 2, 3))) == 1);
    assert(car(member(2, mklist(1, 2, 3))) == 2);
    assert(car(member(3, mklist(1, 2, 3))) == 3);
    return true;
}

bool testReverse() {
    assert(isNil(reverse(list<int>())));
    assert(car(reverse(mklist(1, 2, 3))) == 3);
    assert(cadr(reverse(mklist(1, 2, 3))) == 2);
    return true;
}

bool testListRef() {
    assert(listRef(mklist(1), 0) == 1);
    assert(listRef(mklist(1, 2, 3), 0) == 1);
    assert(listRef(mklist(1, 2, 3), 1) == 2);
    assert(listRef(mklist(1, 2, 3), 2) == 3);
    return true;
}

bool testAssoc() {
    const list<list<std::string> > l = mklist(mklist<std::string>("x", "X"), mklist<std::string>("a", "A"), mklist<std::string>("y", "Y"), mklist<std::string>("a", "AA"));
    assert(assoc<std::string>("a", l) == mklist<std::string>("a", "A"));
    assert(isNil(assoc<std::string>("z", l)));

    const list<list<value> > u = mklist(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"), mklist<value>("a", "AA"));
    assert(assoc<value>("a", u) == mklist<value>("a", "A"));

    const list<value> v = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"), mklist<value>("a", "AA"));
    assert(assoc<value>("a", v) == mklist<value>("a", "A"));
    return true;
}

bool testZip() {
    const list<std::string> k = mklist<std::string>("x", "a", "y", "a");
    const list<std::string> v = mklist<std::string>("X", "A", "Y", "AA");
    const list<list<std::string> > z = mklist(k, v);
    const list<list<std::string> > u = mklist(mklist<std::string>("x", "X"), mklist<std::string>("a", "A"), mklist<std::string>("y", "Y"), mklist<std::string>("a", "AA"));
    assert(zip(k, v) == u);
    assert(unzip(u) == z);
    return true;
}

bool testTokenize() {
    assert(tokenize("/", "aaa/bbb/ccc/ddd") == mklist<std::string>("aaa", "bbb", "ccc", "ddd"));
    assert(tokenize("/", "/bbb/ccc/ddd") == mklist<std::string>("", "bbb", "ccc", "ddd"));
    assert(tokenize("/", "/bbb/ccc/") == mklist<std::string>("", "bbb", "ccc"));
    assert(tokenize("/", "/bbb//ccc/") == mklist<std::string>("", "bbb", "", "ccc"));
    assert(tokenize("/", "abc/def/") == mklist<std::string>("abc", "def"));
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
    //printLambdaCounters();
    //printListCounters();

    assert(1001 == length(map<double, double>(testSeqMap, s)));

    assert(801 == length(member(200.0, s)));
    assert(201 == length(member(200.0, reverse(s))));

    assert(1001 == (reduce<double, double>(testSeqReduce, 0.0, s)));
    //printLambdaCounters();
    //printListCounters();

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
    assert(value(mklist<value>(1, 2)) == value(mklist<value>(1, 2)));

    const list<value> v = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"));
    assert(cadr((list<list<value> >)value(v)) == mklist<value>("a", "A"));

    const value pv(gc_ptr<value>(new value(1)));
    assert(*(gc_ptr<value>)pv == value(1));

    const list<value> lpv = mklist<value>(gc_ptr<value>(new value(1)), gc_ptr<value>(new value(2)));
    assert(*(gc_ptr<value>)car(lpv) == value(1));
    *(gc_ptr<value>)cadr(lpv) = value(3);
    assert(*(gc_ptr<value>)cadr(lpv) == value(3));
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
    {
        gettimeofday(&start, NULL);

        list<double> s = seq(0.0, 999.0);
        list<double> r = map<double, double>(fib, s);
        assert(1000 == length(r));

        gettimeofday(&end, NULL);
        //long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        //std::cout << "Fib cpp function map perf test " << t << " ms" << std::endl;
    }

    {
        struct nested {
            static double fib(double n) {
                struct nested {
                    static double fib_aux(double n, double a, double b) {
                        if(n == 0.0)
                            return a;
                        return fib_aux(n - 1.0, b, a + b);
                    }
                };
                return nested::fib_aux(n, 0.0, 1.0);
            }
        };

        gettimeofday(&start, NULL);

        list<double> s = seq(0.0, 999.0);
        list<double> r = map(lambda<double(double)>(nested::fib), s);
        assert(1000 == length(r));

        gettimeofday(&end, NULL);
        //long t = (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
        //std::cout << "Fib cpp nested function map perf test " << t << " ms" << std::endl;
    }
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
"<property name=\"currency\">US</property>"
"</component>"
"</composite>"
"\n";

const std::string customerXML =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<customer>"
"<name>jdoe</name>"
"<address><city>san francisco</city><state>ca</state></address>"
"<account><id>1234</id><balance>1000</balance></account>"
"<account><id>6789</id><balance>2000</balance></account>"
"<account><id>4567</id><balance>3000</balance></account>"
"</customer>"
"\n";


const bool isName(const value& token) {
    return isTaggedList(token, attribute) && attributeName(token) == "name";
}

bool testReadXML() {
    {
        std::istringstream is(customerXML);
        const list<value> c = readXML(streamList(is));
    }
    {
        std::istringstream is(currencyXML);
        const list<value> c = readXML(streamList(is));

        const value composite = car(c);
        assert(isTaggedList(composite, element));
        assert(elementName(composite) == "composite");
        assert(attributeValue(car(filter<value>(isName, elementChildren(composite)))) == std::string("currency"));
    }
    return true;
}

std::ostringstream* xmlWriter(const std::string& s, std::ostringstream* os) {
    (*os) << s;
    return os;
}

bool testWriteXML() {
    {
        std::istringstream is(customerXML);
        const list<value> c = readXML(streamList(is));
        std::ostringstream os;
        writeXML<std::ostringstream*>(xmlWriter, &os, c);
        assert(os.str() == customerXML);
    }
    {
        std::istringstream is(currencyXML);
        const list<value> c = readXML(streamList(is));
        std::ostringstream os;
        writeXML<std::ostringstream*>(xmlWriter, &os, c);
        assert(os.str() == currencyXML);
    }
    return true;
}

bool testElement() {
    {
        const list<value> ad = mklist<value>(mklist<value>("city", std::string("san francisco")), mklist<value>("state", std::string("ca")));
        const list<value> ac1 = mklist<value>(mklist<value>("id", std::string("1234")), mklist<value>("balance", 1000));
        const list<value> ac2 = mklist<value>(mklist<value>("id", std::string("6789")), mklist<value>("balance", 2000));
        const list<value> ac3 = mklist<value>(mklist<value>("id", std::string("4567")), mklist<value>("balance", 3000));
        {
            const list<value> c = mklist<value>(mklist<value>("customer", mklist<value>("name", std::string("jdoe")), cons<value>("address", ad), mklist<value>("account", mklist<value>(ac1, ac2, ac3))));
            const list<value> e = valuesToElements(c);
            const list<value> v = elementsToValues(e);
            assert(v == c);

            std::ostringstream os;
            writeXML<std::ostringstream*>(xmlWriter, &os, e);
            assert(os.str() == customerXML);
        }
        {
            const list<value> c = mklist<value>(mklist<value>("customer", mklist<value>("name", std::string("jdoe")), cons<value>("address", ad), cons<value>("account", ac1), cons<value>("account", ac2), cons<value>("account", ac3)));
            const list<value> e = valuesToElements(c);
            const list<value> v = elementsToValues(e);

            std::ostringstream os;
            writeXML<std::ostringstream*>(xmlWriter, &os, e);
            assert(os.str() == customerXML);
        }
    }
    {
        std::istringstream is(customerXML);
        const list<value> c = readXML(streamList(is));
        const list<value> v = elementsToValues(c);
        const list<value> e = valuesToElements(v);
        std::ostringstream os;
        writeXML<std::ostringstream*>(xmlWriter, &os, e);
        assert(os.str() == customerXML);
    }
    return true;
}

const id<int> idF(const int v) {
    return v * 2;
}

const id<int> idG(const int v) {
    return v * 3;
}

const id<int> idH(const int v) {
    return idF(v) >> idG;
}

bool testIdMonad() {
    const id<int> m(2);
    assert(m >> idF == idF(2));
    assert(m >> unit<int>() == m);
    assert(m >> idF >> idG == m >> idH);
    return true;
}

const maybe<int> maybeF(const int v) {
    return v * 2;
}

const maybe<int> maybeG(const int v) {
    return v * 3;
}

const maybe<int> maybeH(const int v) {
    return maybeF(v) >> maybeG;
}

bool testMaybeMonad() {
    const maybe<int> m(2);
    assert(m >> maybeF == maybeF(2));
    assert((m >> just<int>()) == m);
    assert(m >> maybeF >> maybeG == m >> maybeH);

    assert(maybe<int>() >> maybeF >> maybeG == maybe<int>());
    return true;
}

const failable<int, std::string> failableF(const int v) {
    return v * 2;
}

const failable<int, std::string> failableG(const int v) {
    return v * 3;
}

const failable<int, std::string> failableH(const int v) {
    return failableF(v) >> failableG;
}

bool testFailableMonad() {
    const failable<int, std::string> m(2);
    assert(m >> failableF == failableF(2));
    assert((m >> success<int, std::string>()) == m);
    assert(m >> failableF >> failableG == m >> failableH);

    failable<int, std::string> ooops = mkfailure<int, std::string>("ooops");
    assert(reason(ooops) == "ooops");
    assert(ooops >> failableF >> failableG == ooops);
    return true;
}

struct tickInc {
    const double v;
    tickInc(const double v) : v(v) {
    }
    const scp<int, double> operator()(int s) const {
        return scp<int, double>(s + 1, v);
    }
};

const state<int, double> tick(const double v) {
    return transformer<int, double>(tickInc(v));
}

const state<int, double> stateF(const double v) {
    return result<int, double>(v * 2.0) >> tick;
}

const state<int, double> stateG(const double v) {
    return result<int, double>(v + 5);
}

const state<int, double> stateH(const double v) {
    return stateF(v) >> stateG;
}

bool testStateMonad() {
    const lambda<state<int, double>(double)> r(result<int, double>);

    state<int, double> m = result<int, double>(2.0);
    assert((m >> stateF)(0) == stateF(2.0)(0));
    assert(1 == (int)(m >> stateF)(0));
    assert((m >> r)(0) == m(0));
    assert((m >> stateF >> stateG)(0) == (m >> stateH)(0));

    return true;
}

bool testDynLib() {
    const failable<lib, std::string> dl(dynlib(".libs/libdynlib-test" + dynlibExt));
    assert(hasContent(dl));
    const failable<lambda<int(int)>, std::string> sq(dynlambda<int(int)>("csquare", content(dl)));
    assert(hasContent(sq));
    lambda<int(int)> l(content(sq));
    assert(l(2) == 4);

    const failable<lambda<lambda<int(int)>()>, std::string> sql(dynlambda<lambda<int(int)>()>("csquarel", content(dl)));
    assert(hasContent(sql));
    lambda<lambda<int(int)>()> ll(content(sql));
    assert(ll()(3) == 9);
    return true;
}

}

int main() {
    std::cout << "Testing..." << std::endl;

    tuscany::testLambda();
    tuscany::testLambdaGC();
    tuscany::testCons();
    tuscany::testSet();
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
    tuscany::testListRef();
    tuscany::testAssoc();
    tuscany::testZip();
    tuscany::testTokenize();
    tuscany::testSeq();
    tuscany::testValue();
    tuscany::testValueGC();
    tuscany::testElement();
    tuscany::testCppPerf();
    tuscany::testAtomicPerf();
    tuscany::testWorker();
    tuscany::testReadXML();
    tuscany::testWriteXML();
    tuscany::testIdMonad();
    tuscany::testMaybeMonad();
    tuscany::testFailableMonad();
    tuscany::testStateMonad();
    tuscany::testDynLib();

    std::cout << "OK" << std::endl;

    return 0;
}
