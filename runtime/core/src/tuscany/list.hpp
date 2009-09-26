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

#ifndef tuscany_list_hpp
#define tuscany_list_hpp

/**
 * Simple list functions.
 */

#include <iostream>
#include "function.hpp"

namespace tuscany {

long countlists = 0;
long countIlists = 0;
long countClists = 0;
long countElists = 0;

bool resetlistCounters() {
    countlists = countIlists = countClists = countElists = 0;
    return true;
}

bool printlistCounters() {
    std::cout << "countlists " << countlists << std::endl;
    std::cout << "countElists " << countElists << std::endl;
    std::cout << "countIlists " << countIlists << std::endl;
    std::cout << "countClists " << countClists << std::endl;
    return true;
}

/**
 * A car/cdr lisp-like pair, base structure to construct lists.
 */

template<typename T> class list {
public:

    list(const T car, const lambda<list<T> ()>& cdr) :
        nil(false), car(car), cdr(cdr) {
        countlists++;
        countIlists++;
    }

    list() :
        nil(true) {
        countlists++;
        countElists++;
    }

    list(const list& p) :
        nil(p.nil), car(p.car), cdr(p.cdr) {
        countlists++;
        countClists++;
    }

    const list& operator=(const list<T>& p) {
        if(this == &p)
            return *this;
        nil = p.nil;
        car = p.car;
        cdr = p.cdr;
        return *this;
    }

    ~list() {
        countlists--;
    }

    const bool operator==(const list<T>& p) const {
        if(this == &p)
            return true;
        if(nil)
            return p.nil;
        if(p.nil)
            return false;
        if(!(car == p.car))
            return false;
        if(cdr == p.cdr)
            return true;
        return cdr() == p.cdr();
    }

    const bool operator!=(const list<T>& p) const {
        return !this->operator==(p);
    }

    template<typename X> friend std::ostream& operator<<(std::ostream&, const list<X>&);

    bool nil;
    T car;
    lambda<list<T> ()> cdr;
};

/**
 * Returns true if the given list is nil.
 */
template<typename T> const bool isNil(const list<T>& p) {
    return p.nil;
}

/**
 * Write a list to an output stream.
 */
template<typename X> std::ostream& operator<<(std::ostream& out, const list<X>& l) {
    if(l == list<X> ())
        return out << "()";
    return out << "(" << car(l) << ", " << cdr(l) << ")";
}

/**
 * Construct a (lazy) list from a value and a lambda function that returns the cdr.
 */
template<typename T> const list<T> cons(const T& car, const lambda<list<T> ()>& cdr) {
    return list<T> (car, cdr);
}

/**
 * Construct a list from a value and a cdr list.
 */
template<typename T> const list<T> cons(const T& car, const list<T>& cdr) {
    return list<T> (car, unit(cdr));
}

/**
 * Construct a list of one value.
 */
template<typename T> const list<T> makeList(const T& car) {
    return list<T> (car, unit(list<T> ()));
}

/**
 * Construct a list of two values.
 */
template<typename T> const list<T> makeList(const T& a, const T& b) {
    return cons(a, makeList(b));
}

/**
 * Construct a list of three values.
 */
template<typename T> const list<T> makeList(const T& a, const T& b, const T& c) {
    return cons(a, cons(b, makeList(c)));
}

/**
 * Construct a list of four values.
 */
template<typename T> const list<T> makeList(const T& a, const T& b, const T& c, const T& d) {
    return cons(a, cons(b, cons(c, makeList(d))));
}

/**
 * Returns the car of a list.
 */
template<typename T> const T car(const list<T>& p) {
    return p.car;
}

/**
 * Returns the cdr of a list.
 */
template<typename T> list<T> const cdr(const list<T>& p) {
    return p.cdr();
}

/**
 * Returns the car of the cdr of a list.
 */
template<typename T> const T cadr(const list<T>& p) {
    return car(cdr(p));
}

/**
 * Returns the car of the cdr of the cdr of a list.
 */
template<typename T> const T caddr(const list<T>& p) {
    return car(cdr(cdr(p)));
}

/**
 * Returns the cdr of a cdr of a list.
 */
template<typename T> const list<T> cddr(const list<T>& p) {
    return cdr(cdr(p));
}

/**
 * Returns the cdr of a cdr of the cdr of a list.
 */
template<typename T> const list<T> cdddr(const list<T>& p) {
    return cdr(cdr(cdr(p)));
}

/**
 * Returns the length of a list.
 */
template<typename T> struct lengthRef {
    const int operator()(const int c, const list<T>& p) {
        if(isNil(p))
            return c;
        return (*this)(c + 1, cdr(p));
    }
};

template<typename T> const int length(const list<T>& p) {
    return lengthRef<T> ()(0, p);
}

/**
 * Appends a list and a lambda function returning a list.
 */
template<typename T> struct appendCdr {
    const list<T> a;
    const lambda<list<T> ()> fb;
    appendCdr(const list<T>& a, const lambda<list<T> ()>& fb) :
        a(a), fb(fb) {
    }
    const list<T> operator()() const {
        return append(a, fb);
    }
};

template<typename T> const list<T> append(const list<T>&a, const lambda<list<T> ()>& fb) {
    if(isNil(a))
        return fb();

    return cons(car(a), lambda<list<T> ()> (appendCdr<T> (cdr(a), fb)));
}

/**
 * Appends two lists.
 */
template<typename T> const list<T> append(const list<T>&a, const list<T>& b) {
    return append(a, unit(b));
}

/**
 * Map a lambda function on a list.
 */
template<typename T, typename R> const list<R> map(const lambda<R(T)>& f, const list<T>& p) {
    if(isNil(p))
        return list<R> ();
    return cons(f(car(p)), map(f, cdr(p)));
}

/**
 * Run a reduce lambda function on a list.
 */
template<typename T, typename R> struct reduceAccumulate {
    const lambda<R(R, T)> f;
    explicit reduceAccumulate(const lambda<R(R, T)>& f) :
        f(f) {
    }
    R operator()(const R& acc, const list<T>& p) const {
        if(isNil(p))
            return acc;
        return (*this)(f(acc, car(p)), cdr(p));
    }
};

template<typename T, typename R> const R reduce(const lambda<R(R, T)>& f, const R& initial, const list<T>& p) {
    return reduceAccumulate<T, R> (f)(initial, p);
}

/**
 * Run a filter lambda function on a list.
 */
template<typename T> const list<T> filter(const lambda<bool(T)>& f, const list<T>& p) {
    if(isNil(p))
        return list<T> ();
    if(f(car(p))) {
        const lambda<list<T> (lambda<bool(T)> , list<T> )> ff(filter<T> );
        return cons(car(p), curry(ff, f, cdr(p)));
    }
    return filter(f, cdr(p));
}

/**
 * Returns a list pointing to a member of a list.
 */
template<typename T> const list<T> member(const T& t, const list<T>& p) {
    if(isNil(p))
        return list<T> ();
    if(t == car(p))
        return p;
    return member(t, cdr(p));
}

/**
 * Reverse a list.
 */
template<typename T> const list<T> reverseIter(const list<T>& acc, const list<T>& p) {
    if(isNil(p))
        return acc;
    return reverseIter(cons(car(p), acc), cdr(p));
}

template<typename T> const list<T> reverse(const list<T>& p) {
    return reverseIter(list<T> (), p);
}

template<typename T> const list<T> seq(const T& start, const T& end);

template<typename T> struct seqGenerate {
    const T start;
    const T end;
    seqGenerate(const T& start, const T&end) :
        start(start), end(end) {
    }
    const list<T> operator()() const {
        return seq<T> (start, end);
    }
};

/**
 * Returns a sequence of values between the given bounds.
 */
template<typename T> const list<T> seq(const T& start, const T& end) {
    if(start == end)
        return makeList(start);
    if(start < end)
        return cons(start, lambda<list<T> ()> (seqGenerate<T> (start + 1, end)));
    return cons(start, lambda<list<T> ()> (seqGenerate<T> (start - 1, end)));
}

/**
 * Equivalent of the list assoc function.
 */
template<typename T> const list<T> assoc(const T& k, const list<list<T> >& p) {
    if(isNil(p))
        return list<T> ();
    if(k == car(car(p)))
        return car(p);
    return assoc(k, cdr(p));
}

/**
 * Pretty print a list.
 */
template<typename T> std::ostream& print(const list<T>& l, std::ostream& os) {
    os << "(";
    if (!isNil(l)) {
        list<T> ml = l;
        while(true) {
            os << car(ml);
            ml = cdr(ml);
            if (isNil(ml))
                break;
            os << ", ";
        }
    }
    os << ")";
    return os;
}

}

#endif /* tuscany_list_hpp */
