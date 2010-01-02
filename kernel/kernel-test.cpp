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
 * Test kernel functions.
 */

#include <assert.h>
#include "string.hpp"
#include "sstream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "dynlib.hpp"
#include "perf.hpp"

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

int mapLambda(lambda<int(const int)> f, int v) {
    return f(v);
}

bool testLambda() {
    const lambda<int(const int)> sq(square);
    assert(sq(2) == 4);
    assert(mapLambda(sq, 2) == 4);
    assert(mapLambda(square, 2) == 4);

    const lambda<int(const int)> incf(inc(10));
    assert(incf(1) == 11);
    assert(mapLambda(incf, 1) == 11);
    assert(mapLambda(inc(10), 1) == 11);

    lambda<int(const int)> l;
    l = incf;
    assert(l(1) == 11);
    l = square;
    assert(l(2) == 4);
    return true;
}

bool testLambdaGC() {
    resetLambdaCounters();
    {
        gc_scoped_pool gc;
        testLambda();
    }
    assert(checkLambdaCounters());
    return true;
}

int countElements = 0;

struct Element {
    int i;

    Element() : i(0) {
        countElements++;
    }

    Element(int i) : i(i) {
        countElements++;
    }

    Element(const Element& o) : i(o.i) {
        countElements++;
    }

    ~Element() {
        countElements--;
    }

    const bool operator==(const Element& o) const {
        return o.i == i;
    }
};
ostream& operator<<(ostream& out, const Element& v) {
    out << v.i ;
    return out;
}

bool testCons() {
    assert(car(cons(2, mklist(3))) == 2);
    assert(car(cdr(cons(2, mklist(3)))) == 3);
    assert(isNil(cdr(cdr(cons(2, mklist(3))))));

    assert(cons(Element(1), mklist(Element(2))) == mklist(Element(1), Element(2)));
    return true;
}

bool testListGC() {
    resetLambdaCounters();
    resetListCounters();
    countElements = 0;
    {
        gc_scoped_pool gc;
        testCons();
    }
    assert(checkLambdaCounters());
    assert(checkListCounters());
    assert(countElements == 0);
    return true;
}

bool testOut() {
    ostringstream os1;
    os1 << list<int> ();
    assert(str(os1) == "()");

    ostringstream os2;
    os2 << mklist(1, 2, 3);
    assert(str(os2) == "(1 2 3)");
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

    assert(list<int>() + 1 + 2 + 3 == mklist(1, 2, 3));
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
ostream& operator<<(ostream& out, const Complex& v) {
    out << "[" << v.x << ":" << v.y << "]";
    return out;
}

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
    const lambda<int(const int, const int)> r(add);
    assert(reduce(r, 0, mklist(1, 2, 3)) == 6);
    return true;
}

bool isPositive(const int x) {
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
    const list<list<string> > l = mklist(mklist<string>("x", "X"), mklist<string>("a", "A"), mklist<string>("y", "Y"), mklist<string>("a", "AA"));
    assert(assoc<string>("a", l) == mklist<string>("a", "A"));
    assert(isNil(assoc<string>("z", l)));

    const list<list<value> > u = mklist(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"), mklist<value>("a", "AA"));
    assert(assoc<value>("a", u) == mklist<value>("a", "A"));

    const list<value> v = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"), mklist<value>("a", "AA"));
    assert(assoc<value>("a", v) == mklist<value>("a", "A"));
    return true;
}

bool testZip() {
    const list<string> k = mklist<string>("x", "a", "y", "a");
    const list<string> v = mklist<string>("X", "A", "Y", "AA");
    const list<list<string> > z = mklist(k, v);
    const list<list<string> > u = mklist(mklist<string>("x", "X"), mklist<string>("a", "A"), mklist<string>("y", "Y"), mklist<string>("a", "AA"));
    assert(zip(k, v) == u);
    assert(unzip(u) == z);
    return true;
}

bool testTokenize() {
    assert(tokenize("/", "aaa/bbb/ccc/ddd") == mklist<string>("aaa", "bbb", "ccc", "ddd"));
    assert(tokenize("/", "/bbb/ccc/ddd") == mklist<string>("", "bbb", "ccc", "ddd"));
    assert(tokenize("/", "/bbb/ccc/") == mklist<string>("", "bbb", "ccc"));
    assert(tokenize("/", "/bbb//ccc/") == mklist<string>("", "bbb", "", "ccc"));
    assert(tokenize("/", "abc/def/") == mklist<string>("abc", "def"));
    return true;
}

double testSeqMap(double x) {
    return x;
}

double testSeqReduce(unused double v, double accum) {
    return accum + 1.0;
}

bool testSeq() {
    resetLambdaCounters();
    resetListCounters();

    list<double> s = seq(0.0, 1000.0);
    assert(1001 == length(s));

    assert(1001 == length(map<double, double>(testSeqMap, s)));

    assert(801 == length(member(200.0, s)));
    assert(201 == length(member(200.0, reverse(s))));

    assert(1001 == (reduce<double, double>(testSeqReduce, 0.0, s)));
    return true;
}

value valueSquare(list<value> x) {
    return (int)car(x) * (int)car(x);
}

bool testValue() {
    assert(value(true) == value(true));
    assert(value(1) == value(1));
    assert(value("abcd") == value("abcd"));
    lambda<value(const list<value>&)> vl(valueSquare);
    assert(value(vl) == value(vl));
    assert(value(mklist<value>(1, 2)) == value(mklist<value>(1, 2)));

    const list<value> v = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"));
    assert(cadr((list<list<value> >)value(v)) == mklist<value>("a", "A"));

    const value pv(gc_ptr<value>(new (gc_new<value>()) value(1)));
    assert(*(gc_ptr<value>)pv == value(1));

    const list<value> lpv = mklist<value>(gc_ptr<value>(new (gc_new<value>()) value(1)), gc_ptr<value>(new (gc_new<value>()) value(2)));
    assert(*(gc_ptr<value>)car(lpv) == value(1));
    return true;
}

bool testValueGC() {
    resetLambdaCounters();
    resetListCounters();
    resetValueCounters();
    {
        gc_scoped_pool gc;
        testValue();
    }
    assert(checkValueCounters());
    assert(checkLambdaCounters());
    assert(checkListCounters());
    return true;
}

bool testTree() {
    const list<value> t = mktree<value>("a", list<value>(), list<value>());
    const list<value> ct = constree<value>("d", constree<value>("f", constree<value>("c", constree<value>("e", constree<value>("b", t)))));
    const list<value> mt = mktree(mklist<value>("d", "f", "c", "e", "b", "a"));
    assert(mt == ct);
    const list<value> l = flatten<value>(mt);
    assert(length(l) == 6);
    assert(car(l) == "a");
    assert(car(reverse(l)) == "f");
    const list<value> bt = mkbtree<value>(l);
    assert(car(bt) == "c");
    return true;
}

const list<value> lta(const string& x) {
    return mklist<value>(c_str(x), c_str(x + x));
}

bool testTreeAssoc() {
    const list<value> t = mktree<value>(lta("a"), list<value>(), list<value>());
    const list<value> at = constree<value>(lta("d"), constree<value>(lta("f"), constree<value>(lta("c"), constree<value>(lta("e"), constree<value>(lta("b"), t)))));
    const list<value> l = flatten<value>(at);
    assert(length(l) == 6);
    assert(car(l) == mklist<value>("a", "aa"));
    assert(car(reverse(l)) == mklist<value>("f", "ff"));
    const list<value> bt = mkbtree<value>(l);
    assert(car(bt) == mklist<value>("c", "cc"));
    assert(assoctree<value>("a", bt) == mklist<value>("a", "aa"));
    assert(assoctree<value>("b", bt) == mklist<value>("b", "bb"));
    assert(assoctree<value>("f", bt) == mklist<value>("f", "ff"));
    assert(isNil(assoctree<value>("x", bt)));
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

struct fibMapPerf {
    const bool operator()() const {
        list<double> s = seq(0.0, 999.0);
        list<double> r = map<double, double>(fib, s);
        assert(1000 == length(r));
        return true;
    }
};

struct nestedFibMapPerf {
    const lambda<double(const double)> fib;
    nestedFibMapPerf(const lambda<double(const double)>& fib) : fib(fib) {
    }
    const bool operator()() const {
        list<double> s = seq(0.0, 999.0);
        list<double> r = map<double, double>(fib, s);
        assert(1000 == length(r));
        return true;
    }
};

bool testCppPerf() {
    {
        const lambda<bool()> fml = fibMapPerf();
        cout << "Fibonacci map test " << (time(fml, 1, 1) / 1000) << " ms" << endl;
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

        const lambda<bool()> nfml = nestedFibMapPerf(lambda<double(const double)>(nested::fib));
        cout << "Nested Fibonacci map test " << (time(nfml, 1, 1) / 1000) << " ms" << endl;
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

const failable<int> failableF(const int v) {
    return v * 2;
}

const failable<int> failableG(const int v) {
    return v * 3;
}

const failable<int> failableH(const int v) {
    return failableF(v) >> failableG;
}

bool testFailableMonad() {
    const failable<int> m(2);
    assert(m >> failableF == failableF(2));
    assert((m >> success<int, string>()) == m);
    assert(m >> failableF >> failableG == m >> failableH);

    cout << "Failable monad test... " << endl;
    failable<int> ooops = mkfailure<int>("test");
    assert(reason(ooops) == "test");
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
    const lambda<state<int, double>(const double)> r(result<int, double>);

    state<int, double> m = result<int, double>(2.0);
    assert((m >> stateF)(0) == stateF(2.0)(0));
    assert(1 == (int)(m >> stateF)(0));
    assert((m >> r)(0) == m(0));
    assert((m >> stateF >> stateG)(0) == (m >> stateH)(0));

    return true;
}

bool testDynLib() {
    const lib dl(string(".libs/libdynlib-test") + dynlibExt);
    const failable<lambda<int(const int)> > sq(dynlambda<int(const int)>("csquare", dl));
    assert(hasContent(sq));
    lambda<int(const int)> l(content(sq));
    assert(l(2) == 4);

    const failable<lambda<lambda<int(const int)>()> > sql(dynlambda<lambda<int(const int)>()>("csquarel", dl));
    assert(hasContent(sql));
    lambda<lambda<int(const int)>()> ll(content(sql));
    assert(ll()(3) == 9);
    return true;
}

}

int main() {
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testLambda();
    tuscany::testLambdaGC();
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
    tuscany::testListRef();
    tuscany::testAssoc();
    tuscany::testZip();
    tuscany::testTokenize();
    tuscany::testSeq();
    tuscany::testValue();
    tuscany::testValueGC();
    tuscany::testTree();
    tuscany::testTreeAssoc();
    tuscany::testCppPerf();
    tuscany::testIdMonad();
    tuscany::testMaybeMonad();
    tuscany::testFailableMonad();
    tuscany::testStateMonad();
    tuscany::testDynLib();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
