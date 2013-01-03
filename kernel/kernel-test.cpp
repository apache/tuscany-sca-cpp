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

#include <functional>
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

const bool ptrPerf() {
    int x = 1;
    for (int i = 0; i < 10000; i++) {
        int* const y = &x;
        int z = *y + *y;
        z = z + 1;
    }
    return true;
}

const bool gcptrPerf() {
    int x = 1;
    for (int i = 0; i < 10000; i++) {
        const gc_ptr<int> y = &x;
        int z = *y + *y;
        z = z + 1;
    }
    return true;
}

const bool testPtrPerf() {
    {
        const gc_scoped_pool gcp;
        const blambda pp = ptrPerf;
        cout << "Pointer test " << (time(pp, 5, 10000) / 10000) << " ms" << endl;
    }
    {
        const gc_scoped_pool gcp;
        const blambda gpp = gcptrPerf;
        cout << "GC Pointer test " << (time(gpp, 5, 10000) / 10000) << " ms" << endl;
    }
    return true;
}

class incFunctor {
public:
    incFunctor(const int i) :
        i(i) {
    }
    const int operator()(const int x) const {
        return x + i;
    }
private:
    const int i;
};

const int square(const int x) {
    return x * x;
}

const int mapLambda(lambda<const int(const int)> f, int v) {
    return f(v);
}

const lambda<const int(const int)> mksquare() {
    return square;
}

const bool testSizes() {
    const gc_ptr<int> p(NULL);
    cout << "sizeof gc_ptr " << sizeof(p) << endl;
    const lambda<const int(const int)> sq(square);
    cout << "sizeof C function lambda " << sizeof(sq) << endl;
    const lambda<const int(const int)> incl(incFunctor(10));
    cout << "sizeof functor lambda " << sizeof(incl) << endl;
    const std::function<int(const int)> sqf(square);
    cout << "sizeof C function std::function " << sizeof(sqf) << endl;
    const std::function<int(const int)> incf(incFunctor(10));
    cout << "sizeof functor std::function " << sizeof(incf) << endl;
    const string s("abcd");
    cout << "sizeof string " << sizeof(s) << endl;
    const string v(s);
    cout << "sizeof val " << sizeof(v) << endl;
    const list<int> li = cons<int>(2, mklist<int>(3, 4));
    cout << "sizeof list<int> " << sizeof(li) << endl;
    const list<string> ls = cons<string>("a", mklist<string>("b", "c"));
    cout << "sizeof list<string> " << sizeof(ls) << endl;
    const list<value> lv = cons<value>("a", mklist<value>("b", "c"));
    cout << "sizeof list<value> " << sizeof(lv) << endl;
    return true;
}

const bool testLambda() {
    const lambda<const int(const int)> sq(square);
    assert(sq(2) == 4);

    const lambda<const lambda<const int(const int)>()> mksq(mksquare);
    assert(mksq()(2) == 4);

    assert(mapLambda(sq, 2) == 4);
    assert(mapLambda(square, 2) == 4);

    const lambda<const int(const int)> incf(incFunctor(10));
    assert(incf(1) == 11);
    assert(mapLambda(incf, 1) == 11);
    assert(mapLambda(incFunctor(10), 1) == 11);

    const int base = 10;
    auto incl11 = [base](const int i) { return base + i; };
    const lambda<const int(const int)> incl(incl11);
    assert(incl(1) == 11);
    assert(mapLambda(incl, 1) == 11);
    assert(mapLambda(incl11, 1) == 11);

    const lambda<const int(const int)> il(incf);
    assert(il(1) == 11);
    const lambda<const int(const int)> sl(square);
    assert(sl(2) == 4);
    return true;
}

int constructCopiable = 0;
int copyCopiable = 0;

class Copiable {
public:
    Copiable() {
        assert(false);
    }

    Copiable(const string& s) : s(s) {
        constructCopiable++;
    }

    Copiable(const Copiable& c) : s(c.s) {
        copyCopiable++;
    }

    Copiable& operator=(const Copiable& c) = delete;

    ~Copiable() {
    }

    void doit() const {
    }

private:
    const string s;
};

bool testLambdaRef(const Copiable& c) {
    [&c] { c.doit(); }();
    return true;
}

bool testLambdaCopy(const Copiable& c) {
    [c] { c.doit(); }();
    return true;
}

const bool testCopiable() {
    const Copiable ac = Copiable("assigned");
    ac.doit();
    assert(constructCopiable = 1);
    assert(copyCopiable = 1);

    const Copiable ac2 = []{ return Copiable("returned"); }();
    ac2.doit();
    assert(constructCopiable = 2);
    assert(copyCopiable = 2);

    const Copiable rc = Copiable("captured by ref");
    testLambdaRef(rc);
    assert(constructCopiable = 3);
    assert(copyCopiable = 3);

    const Copiable cc = Copiable("captured by value");
    testLambdaCopy(cc);
    assert(constructCopiable = 4);
    assert(copyCopiable = 5);

    return true;
}

const bool testMutable() {
    {
        gc_mutable_ref<string> s = string("aaa");
        s = "bbb";
        assert(s == "bbb");
    }
    {
        gc_mutable_ref<string> s;
        assert(s == "");
        s = "bbb";
        assert(s == "bbb");
    }
    {
        gc_mutable_ref<value> v;
        assert(isNil((value)v));
        v = 1;
        assert(v == 1);
    }
    return true;
}

const bool testLambdaGC() {
    resetLambdaCounters();
    {
        const gc_scoped_pool gcp;
        testLambda();
    }
    assert(checkLambdaCounters());
    return true;
}

class sint {
public:
    sint(const int i) : i(i) {
    }
    const int i;
    char b[4];
};

const int mult(const sint& a, const sint& b) {
    return a.i * b.i;
}

const bool testCurry() {
    const lambda<const int(const sint)> mult2 = curry((lambda<const int(const sint, const sint)>)mult, sint(2));
    assert(6 == mult2(sint(3)));
    return true;
}

const bool curryPerf() {
    const sint a(2);
    const sint b(3);
    const lambda<const int(const sint)> mult2 = curry((lambda<const int(const sint, const sint)>)mult, a);
    for(int i = 0; i < 1000; i++)
        mult2(b);
    return true;
}

class multFunctor {
public:
    multFunctor(const sint& a) : a(a) {
    }
    const int operator()(const sint& b) const {
        return a.i * b.i;
    }
private:
    const sint a;
};

const bool testFunctor() {
    const lambda<const int(const sint)> mult2 = multFunctor(sint(2));
    assert(6 == mult2(sint(3)));
    return true;
}

const bool functorPerf() {
    const sint a(2);
    const sint b(3);
    const lambda<const int(const sint)> mult2 = lambda<const int(const sint)>(multFunctor(a));
    for(int i = 0; i < 1000; i++)
        mult2(b);
    return true;
}

const bool test11Lambda() {
    const sint a(2);
    assert(6 == [a](const sint& b) { return a.i * b.i; } (sint(3)));
    return true;
}

const lambda<const sint(const sint)> multlambda11(const sint& a) {
    return [a](const sint& b) { return a.i * b.i; };
}

const bool lambda11Perf() {
    const sint a(2);
    const sint b(3);
    const lambda<const sint(const sint)> mult2 = multlambda11(a);
    for(int i = 0; i < 1000; i++)
        mult2(b);
    return true;
}

const std::function<sint(const sint&)> multfunction11(const sint& a) {
    return [a](const sint& b) { return a.i * b.i; };
}

const bool function11Perf() {
    const sint a(2);
    const sint b(3);
    const std::function<sint(const sint&)> mult2 = multfunction11(a);
    for(int i = 0; i < 1000; i++)
        mult2(b);
    return true;
}

const bool testFuncPerf() {
    {
        const gc_scoped_pool gcp;
        const blambda cp = curryPerf;
        cout << "Curry test " << (time(cp, 5, 1000) / 1000) << " ms" << endl;
    }
    {
        const gc_scoped_pool gcp;
        const blambda fp = functorPerf;
        cout << "Functor test " << (time(fp, 5, 1000) / 1000) << " ms" << endl;
    }
    {
        const gc_scoped_pool gcp;
        const blambda lp = lambda11Perf;
        cout << "Lambda11 test " << (time(lp, 5, 1000) / 1000) << " ms" << endl;
    }
    {
        const gc_scoped_pool gcp;
        const blambda lp = function11Perf;
        cout << "Function11 test " << (time(lp, 5, 1000) / 1000) << " ms" << endl;
    }
    return true;
}

int countElements = 0;

class Element {
public:
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

    int i;
};

ostream& operator<<(ostream& out, const Element& v) {
    out << v.i ;
    return out;
}

const bool testCons() {
    assert(car(cons(2, mklist(3))) == 2);
    assert(car(cdr(cons(2, mklist(3)))) == 3);
    assert(isNil(cdr(cdr(cons(2, mklist(3))))));

    assert(cons(Element(1), mklist(Element(2))) == mklist(Element(1), Element(2)));
    return true;
}

const bool testListGC() {
    resetLambdaCounters();
    resetListCounters();
    countElements = 0;
    {
        const gc_scoped_pool gcp;
        testCons();
    }
    assert(checkLambdaCounters());
    assert(checkListCounters());
    assert(countElements == 0);
    return true;
}

template<typename T> const list<T> listPerf(const T& v, const long int i) {
    if (i == 0)
        return list<T>();
    return cons<T>(v, listPerf<T>(v, i -1));
}

const bool testListPerf() {
    {
        const gc_scoped_pool gcp;
        const blambda lp = []() -> const bool {
            listPerf<int>(0, 1000);
            return true;
        };
        cout << "List<int> test " << (time(lp, 5, 1000) / 1000) << " ms" << endl;
    }
    {
        const gc_scoped_pool gcp;
        const blambda lp = []() -> const bool {
            listPerf<string>(string("x"), 1000);
            return true;
        };
        cout << "List<string> test " << (time(lp, 5, 1000) / 1000) << " ms" << endl;
    }
    {
        const gc_scoped_pool gcp;
        const blambda lp = []() -> const bool {
            listPerf<string>(value("x"), 1000);
            return true;
        };
        cout << "List<value> test " << (time(lp, 5, 1000) / 1000) << " ms" << endl;
    }
    return true;
}

const bool testOut() {
    ostringstream os1;
    os1 << list<int> ();
    assert(str(os1) == "()");

    ostringstream os2;
    os2 << mklist(1, 2, 3);
    assert(str(os2) == "(1 2 3)");
    return true;
}

const bool testEquals() {
    assert(list<int>() == list<int>());
    assert(mklist(1, 2) == mklist(1, 2));
    assert(list<int>() != mklist(1, 2));
    assert(mklist(1, 2, 3) == mklist(1, 2, 3));
    assert(mklist(1, 2) != mklist(1, 2, 3));
    return true;
}

const bool testLength() {
    assert(0 == length(list<int>()));
    assert(1 == length(mklist(1)));
    assert(2 == length(cons(1, mklist(2))));
    return true;
}

const bool testAppend() {
    assert(car(append(mklist(1), mklist(2))) == 1);
    assert(car(cdr(append(mklist(1), mklist(2)))) == 2);
    assert(car(cdr(cdr(append(mklist(1), mklist(2, 3))))) == 3);
    assert(isNil(cdr(cdr(cdr(append(mklist(1), mklist(2, 3)))))));

    assert(list<int>() + 1 + 2 + 3 == mklist(1, 2, 3));
    return true;
}

class Complex {
public:
    Complex() : x(0), y(0) {
    }
    Complex(const int x, const int y) : x(x), y(y) {
    }

    const int x;
    const int y;
};

ostream& operator<<(ostream& out, const Complex& v) {
    out << "[" << v.x << ":" << v.y << "]";
    return out;
}

const bool testComplex() {
    const list<Complex> p = mklist(Complex(1, 2), Complex(3, 4));
    assert(car(p).x == 1);
    assert(car(cdr(p)).x == 3);
    assert(isNil(cdr(cdr(p))));
    return true;
}

const bool testMap() {
    assert(isNil(map<int, int>(square, list<int>())));

    const list<int> m = map<int, int>(square, mklist(2, 3));
    assert(car(m) == 4);
    assert(car(cdr(m)) == 9);

    return true;
}

const int add(const int x, const int y) {
    return x + y;
}

const bool testReduce() {
    const lambda<const int(const int, const int)> r(add);
    assert(reduce(r, 0, mklist(1, 2, 3)) == 6);
    return true;
}

const bool isPositive(const int x) {
    if(x >= 0)
        return true;
    else
        return false;
}

const bool testFilter() {
    assert(car(filter<int>(isPositive, mklist(1, -1, 2, -2))) == 1);
    assert(cadr(filter<int>(isPositive, mklist(1, -1, 2, -2))) == 2);
    return true;
}

const bool testMember() {
    assert(isNil(member(4, mklist(1, 2, 3))));
    assert(car(member(1, mklist(1, 2, 3))) == 1);
    assert(car(member(2, mklist(1, 2, 3))) == 2);
    assert(car(member(3, mklist(1, 2, 3))) == 3);
    return true;
}

const bool testReverse() {
    assert(isNil(reverse(list<int>())));
    assert(car(reverse(mklist(1, 2, 3))) == 3);
    assert(cadr(reverse(mklist(1, 2, 3))) == 2);
    return true;
}

const bool testListRef() {
    assert(listRef(mklist(1), 0) == 1);
    assert(listRef(mklist(1, 2, 3), 0) == 1);
    assert(listRef(mklist(1, 2, 3), 1) == 2);
    assert(listRef(mklist(1, 2, 3), 2) == 3);
    return true;
}

const bool testSubst() {
    assert(subst(1, 9, mklist(1, 2, 1, 3, 1)) == mklist(9, 2, 9, 3, 9));
    assert(subst<value>(1, 9, mklist<value>(1, 2, 1, 3, 1)) == mklist<value>(9, 2, 9, 3, 9));
    return true;
}

const bool testAssoc() {
    const list<list<string> > l = mklist(mklist<string>("x", "X"), mklist<string>("a", "A"), mklist<string>("y", "Y"), mklist<string>("a", "AA"));
    assert(assoc<string>("a", l) == mklist<string>("a", "A"));
    assert(isNil(assoc<string>("z", l)));

    const list<list<string> > l3 = mklist(mklist<string>("x", "X"), mklist<string>("a", "A"), mklist<string>("a", "AA"));
    assert(delAssoc<string>("y", l) == l3);

    const list<list<string> > l4 = mklist(mklist<string>("x", "X"), mklist<string>("y", "Y"));
    assert(delAssoc<string>("a", l) == l4);

    const list<list<string> > l5 = mklist(mklist<string>("x", "X"), mklist<string>("a", "A"), mklist<string>("y", "YY"), mklist<string>("a", "AA"));
    assert(substAssoc<string>("y", mklist<string>("y", "YY"), l) == l5);

    const list<list<string> > l6 = mklist(mklist<string>("x", "X"), mklist<string>("a", "AAA"), mklist<string>("y", "Y"), mklist<string>("a", "AAA"));
    assert(substAssoc<string>("a", mklist<string>("a", "AAA"), l) == l6);

    const list<list<string> > l7 = mklist(mklist<string>("x", "X"), mklist<string>("a", "A"), mklist<string>("y", "Y"), mklist<string>("a", "AA"), mklist<string>("z", "ZZ"));
    assert(substAssoc<string>("z", mklist<string>("z", "ZZ"), l) == l);
    assert(substAssoc<string>("z", mklist<string>("z", "ZZ"), l, true) == l7);

    return true;
}

const bool testValueAssoc() {
    const list<list<value> > u = mklist(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"), mklist<value>("a", "AA"));
    assert(assoc<value>("a", u) == mklist<value>("a", "A"));

    const list<value> v = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"), mklist<value>("a", "AA"));
    assert(assoc<value>("a", v) == mklist<value>("a", "A"));

    const list<value> v2 = mklist<value>(mklist<value>("x", "X"), "a", mklist<value>("a", "A"), mklist<value>("y", "Y"), mklist<value>("a", "AA"));
    assert(assoc<value>("a", v2) == mklist<value>("a", "A"));

    const list<value> v3 = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("a", "AA"));
    assert(delAssoc<value>("y", v) == v3);

    const list<value> v4 = mklist<value>(mklist<value>("x", "X"), mklist<value>("y", "Y"));
    assert(delAssoc<value>("a", v) == v4);

    const list<value> v5 = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "YY"), mklist<value>("a", "AA"));
    assert(substAssoc<value>("y", mklist<value>("y", "YY"), v) == v5);

    const list<value> v6 = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "AAA"), mklist<value>("y", "Y"), mklist<value>("a", "AAA"));
    assert(substAssoc<value>("a", mklist<value>("a", "AAA"), v) == v6);

    const list<value> v7 = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"), mklist<value>("a", "AA"), mklist<value>("z", "ZZ"));
    assert(substAssoc<value>("z", mklist<value>("z", "ZZ"), v) == v);
    assert(substAssoc<value>("z", mklist<value>("z", "ZZ"), v, true) == v7);
    return true;
}

const bool testTreeAssoc() {
    const list<value> tv = mklist<value>(mklist<value>("a", "A"), mklist<value>("b", mklist<value>("ba", "BA"), mklist<value>("bc", "BC"), mklist<value>("bd", mklist<value>("z", "ZZ"), mklist<value>("bda", "BDA"))), mklist<value>("z", "Z"));
    assert(treeSelectAssoc<value>(mklist<value>("a"), tv) == mklist<value>(mklist<value>("a", "A")));
    assert(treeSelectAssoc<value>(mklist<value>("b", "bc"), tv) == mklist<value>(mklist<value>("bc", "BC")));
    assert(treeSelectAssoc<value>(mklist<value>("b", "bx"), tv) == list<value>());
    assert(treeSelectAssoc<value>(mklist<value>("b", "bd", "bda"), tv) == mklist<value>(mklist<value>("bda", "BDA")));
    assert(treeSelectAssoc<value>(mklist<value>("bc"), tv) == mklist<value>(mklist<value>("bc", "BC")));
    assert(treeSelectAssoc<value>(mklist<value>("bda"), tv) == mklist<value>(mklist<value>("bda", "BDA")));
    assert(treeSelectAssoc<value>(mklist<value>("bd", "bda"), tv) == mklist<value>(mklist<value>("bda", "BDA")));
    assert(treeSelectAssoc<value>(mklist<value>("bd", "bda", "bdax"), tv) == list<value>());
    assert(treeSelectAssoc<value>(mklist<value>("z"), tv) == mklist<value>(mklist<value>("z", "ZZ"), mklist<value>("z", "Z")));

    const list<value> tv2 = mklist<value>(mklist<value>("a", "A"), mklist<value>("b", mklist<value>("ba", "BA"), mklist<value>("bc", "BC"), mklist<value>("bd", mklist<value>("bda", "BDA"))));
    const list<value> tv3 = mklist<value>(mklist<value>("a", "A"), mklist<value>("b", mklist<value>("ba", "BA"), mklist<value>("bc", "BC"), mklist<value>("bd", mklist<value>("z", "ZZ"))), mklist<value>("z", "Z"));
    assert(treeDelAssoc<value>(mklist<value>("z"), tv) == tv2);
    assert(treeDelAssoc<value>(mklist<value>("bda"), tv) == tv3);
    assert(treeDelAssoc<value>(mklist<value>("bd", "bda"), tv) == tv3);
    assert(treeDelAssoc<value>(mklist<value>("bd", "bda", "bdax"), tv) == tv);

    const list<value> tv4 = mklist<value>(mklist<value>("a", "A"), mklist<value>("b", mklist<value>("ba", "BA"), mklist<value>("bc", "BC"), mklist<value>("bd", mklist<value>("z", "ZZZ"), mklist<value>("bda", "BDA"))), mklist<value>("z", "ZZZ"));
    const list<value> tv5 = mklist<value>(mklist<value>("a", "A"), mklist<value>("b", mklist<value>("ba", "BA"), mklist<value>("bc", "BC"), mklist<value>("bd", mklist<value>("z", "ZZ"), mklist<value>("bda", "BDAX"))), mklist<value>("z", "Z"));
    assert(treeSubstAssoc<value>(mklist<value>("z"), mklist<value>("z", "ZZZ"), tv) == tv4);
    assert(treeSubstAssoc<value>(mklist<value>("bda"), mklist<value>("bda", "BDAX"), tv) == tv5);
    assert(treeSubstAssoc<value>(mklist<value>("bd", "bda"), mklist<value>("bda", "BDAX"), tv) == tv5);
    assert(treeSubstAssoc<value>(mklist<value>("bd", "bda", "bdax"), mklist<value>("bda", "BDAX"), tv) == tv);
    return true;
}

const bool testZip() {
    const list<string> k = mklist<string>("x", "a", "y", "a");
    const list<string> v = mklist<string>("X", "A", "Y", "AA");
    const list<list<string> > z = mklist(k, v);
    const list<list<string> > u = mklist(mklist<string>("x", "X"), mklist<string>("a", "A"), mklist<string>("y", "Y"), mklist<string>("a", "AA"));
    assert(zip(k, v) == u);
    assert(unzip(u) == z);
    return true;
}

const bool testTokenize() {
    assert(tokenize("/", "") == list<string>());
    assert(tokenize("/", "aaa") == mklist<string>("aaa"));
    assert(tokenize("/", "aaa/bbb/ccc/ddd") == mklist<string>("aaa", "bbb", "ccc", "ddd"));
    assert(tokenize("/", "/bbb/ccc/ddd") == mklist<string>("", "bbb", "ccc", "ddd"));
    assert(tokenize("/", "/bbb/ccc/") == mklist<string>("", "bbb", "ccc"));
    assert(tokenize("/", "/bbb//ccc/") == mklist<string>("", "bbb", "", "ccc"));
    assert(tokenize("/", "abc/def/") == mklist<string>("abc", "def"));

    assert(join("/", list<string>()) == "");
    assert(join("/", mklist<string>("aaa")) == "aaa");
    assert(join("/", mklist<string>("aaa", "bbb", "ccc", "ddd")) == "aaa/bbb/ccc/ddd");
    assert(join("/", mklist<string>("", "bbb", "ccc", "ddd")) == "/bbb/ccc/ddd");
    assert(join("/", mklist<string>("bbb", "ccc", "")) == "bbb/ccc/");
    assert(join("/", mklist<string>("bbb", "", "ccc")) == "bbb//ccc");
    return true;
}

const double testSeqMap(const double x) {
    return x;
}

double testSeqReduce(unused const double v, const double accum) {
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

const value valueSquare(const list<value>& x) {
    return (int)car(x) * (int)car(x);
}

const bool testValue() {
    assert(value(true) == trueValue);
    assert(value(1) == value(1));
    assert(value("abcd") == value("abcd"));
    lvvlambda vl(valueSquare);
    //assert(value(vl) == value(vl));
    assert(value(mklist<value>(1, 2)) == value(mklist<value>(1, 2)));

    const list<value> v = mklist<value>(mklist<value>("x", "X"), mklist<value>("a", "A"), mklist<value>("y", "Y"));
    assert(cadr<value>(value(v)) == mklist<value>("a", "A"));

    const value pv(gc_ptr<value>(new (gc_new<value>()) value(1)));
    assert(*(gc_ptr<value>)pv == value(1));

    const list<value> lpv = mklist<value>(gc_ptr<value>(new (gc_new<value>()) value(1)), gc_ptr<value>(new (gc_new<value>()) value(2)));
    assert(*(gc_ptr<value>)car(lpv) == value(1));
    return true;
}

const bool testValueGC() {
    resetLambdaCounters();
    resetListCounters();
    resetValueCounters();
    {
        const gc_scoped_pool gcp;
        testValue();
    }
    assert(checkValueCounters());
    assert(checkLambdaCounters());
    assert(checkListCounters());
    return true;
}

const bool testBinaryTree() {
    const list<value> t = mkrbtree<value>("a", nilListValue, nilListValue);
    const list<value> ct = rbtreeCons<value>("d", rbtreeCons<value>("f", rbtreeCons<value>("c", rbtreeCons<value>("e", rbtreeCons<value>("b", t)))));
    const list<value> mt = mkrbtree(mklist<value>("d", "f", "c", "e", "b", "a"));
    assert(mt == ct);
    const list<value> l = flatten<value>(mt);
    assert(length(l) == 6);
    assert(car(l) == "a");
    assert(car(reverse(l)) == "f");
    const list<value> bt = mkbrbtree<value>(l);
    assert(car(bt) == "c");
    return true;
}

const list<value> lta(const string& x) {
    return mklist<value>(c_str(x), c_str(x + x));
}

const bool testBinaryTreeAssoc() {
    const list<value> t = mkrbtree<value>(lta("a"), nilListValue, nilListValue);
    const list<value> at = rbtreeCons<value>(lta("d"), rbtreeCons<value>(lta("f"), rbtreeCons<value>(lta("c"), rbtreeCons<value>(lta("e"), rbtreeCons<value>(lta("b"), t)))));
    const list<value> l = flatten<value>(at);
    assert(length(l) == 6);
    assert(car(l) == mklist<value>("a", "aa"));
    assert(car(reverse(l)) == mklist<value>("f", "ff"));
    const list<value> bt = mkbrbtree<value>(l);
    assert(car(bt) == mklist<value>("c", "cc"));
    assert(rbtreeAssoc<value>("a", bt) == mklist<value>("a", "aa"));
    assert(rbtreeAssoc<value>("b", bt) == mklist<value>("b", "bb"));
    assert(rbtreeAssoc<value>("f", bt) == mklist<value>("f", "ff"));
    assert(isNil(rbtreeAssoc<value>("x", bt)));
    return true;
}

const double fib_aux(const double n, const double a, const double b) {
    if(n == 0.0)
        return a;
    return fib_aux(n - 1.0, b, a + b);
}

const double fib(const double n) {
    return fib_aux(n, 0.0, 1.0);
}

const bool fibMapPerf() {
    const list<double> s = seq(0.0, 999.0);
    const list<double> r = map<double, double>(fib, s);
    assert(1000 == length(r));
    return true;
}

const bool testCppPerf() {
    {
        const gc_scoped_pool gcp;
        const blambda fml = fibMapPerf;
        cout << "Fibonacci map test " << (time(fml, 1, 1) / 1000) << " ms" << endl;
    }

    {
        const lambda<const double(const double n)> fib = [](const double n) -> const double {
            const lambda<const double(const double, const double, const double)> fib_aux = [&fib_aux](double n, double a, double b) -> const double {
                if(n == 0.0)
                    return a;
                return fib_aux(n - 1.0, b, a + b);
            };
            return fib_aux(n, 0.0, 1.0);
        };

        const blambda nfml = [fib]() -> const bool {
            const list<double> s = seq(0.0, 999.0);
            const list<double> r = map<double, double>(fib, s);
            assert(1000 == length(r));
            return true;
        };
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

const bool testIdMonad() {
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

const bool testMaybeMonad() {
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

const bool testFailableMonad() {
    const failable<int> m(2);
    assert(m >> failableF == failableF(2));
    assert((m >> success<int, string, int>()) == m);
    assert(m >> failableF >> failableG == m >> failableH);

    cout << "Failable monad test... " << endl;
    const failable<int> ooops = mkfailure<int>("test", 500);
    assert(reason(ooops) == "test");
    assert(rcode(ooops) == 500);
    assert(ooops >> failableF >> failableG == ooops);

    const failable<value> vooops = mkfailure<value>(ooops);
    assert(reason(vooops) == "test");
    assert(rcode(vooops) == 500);

    const value v = value(vooops);
    assert(car<value>(v) == nilValue);
    assert(cadr<value>(v) == string("test"));
    assert(caddr<value>(v) == value((double)500));
    return true;
}

const state<int, double> tick(const double v) {
    return transformer<int, double>([v](const int s) {
        return scp<int, double>(s + 1, v);
    });
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

const bool testStateMonad() {
    const lambda<const state<int, double>(const double)> r(result<int, double>);

    state<int, double> m = result<int, double>(2.0);
    assert((m >> stateF)(0) == stateF(2.0)(0));
    assert(1 == (int)(m >> stateF)(0));
    assert((m >> r)(0) == m(0));
    assert((m >> stateF >> stateG)(0) == (m >> stateH)(0));

    return true;
}

const bool testDynLib() {
    const lib dl(string("./libdynlib-test") + dynlibExt);
    const failable<lambda<const int(const int)> > sq(dynlambda<const int(const int)>("csquare", dl));
    assert(hasContent(sq));
    lambda<const int(const int)> l(content(sq));
    assert(l(2) == 4);

    const failable<lambda<const lambda<const int(const int)>()> > sql(dynlambda<const lambda<const int(const int)>()>("csquarel", dl));
    assert(hasContent(sql));
    lambda<const lambda<const int(const int)>()> ll(content(sql));
    assert(ll()(3) == 9);
    return true;
}


}

int main() {
    const tuscany::gc_scoped_pool p;
    tuscany::cout << "Testing..." << tuscany::endl;

    tuscany::testSizes();
    tuscany::testCopiable();
    tuscany::testMutable();
    tuscany::testPtrPerf();
    tuscany::testLambda();
    tuscany::testLambdaGC();
    tuscany::testCurry();
    tuscany::testFunctor();
    tuscany::test11Lambda();
    tuscany::testFuncPerf();
    tuscany::testCons();
    tuscany::testListGC();
    tuscany::testListPerf();
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
    tuscany::testSubst();
    tuscany::testAssoc();
    tuscany::testValueAssoc();
    tuscany::testTreeAssoc();
    tuscany::testZip();
    tuscany::testTokenize();
    tuscany::testSeq();
    tuscany::testValue();
    tuscany::testValueGC();
    tuscany::testBinaryTree();
    tuscany::testBinaryTreeAssoc();
    tuscany::testCppPerf();
    tuscany::testIdMonad();
    tuscany::testMaybeMonad();
    tuscany::testFailableMonad();
    tuscany::testStateMonad();
    tuscany::testDynLib();

    tuscany::cout << "OK" << tuscany::endl;

    return 0;
}
