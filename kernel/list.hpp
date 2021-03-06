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

#include <assert.h>
#include "string.hpp"
#include "fstream.hpp"
#include "function.hpp"

namespace tuscany {

#ifdef WANT_MAINTAINER_COUNTERS

/**
 * Debug utilities. Counters used to track instances of lists.
 */
long countLists = 0;
long countILists = 0;
long countCLists = 0;
long countELists = 0;

inline const bool resetListCounters() noexcept {
    countLists = countILists = countCLists = countELists = 0;
    return true;
}

inline const bool checkListCounters() noexcept {
    return countLists == 0;
}

inline const bool printListCounters() noexcept {
    cout << "countLists " << countLists << endl;
    cout << "countELists " << countELists << endl;
    cout << "countILists " << countILists << endl;
    cout << "countCLists " << countCLists << endl;
    return true;
}

#else

#define resetListCounters()
#define checkListCounters() true
#define printListCounters()

#endif

#ifdef WANT_MAINTAINER_WATCH

/**
 * Debug utilities. Macro used to write the contents of a list to
 * a string, easier to watch in a debugger than the list itself.
 */
#define debug_watchList() do { \
        this->watch = watchList(*this); \
    } while (0)

#else

#define debug_watchList();

#endif

/**
 * A car/cdr lisp-like pair, base structure to construct lists.
 */

template<typename T> class list {
public:

    inline list() noexcept : car() {
        debug_inc(countLists);
        debug_inc(countELists);
        debug_watchList();
    }

    inline list(const T& car, const lambda<list<T>()>& cdr) noexcept : car(car), cdr(cdr) {
        debug_inc(countLists);
        debug_inc(countILists);
        debug_watchList();
    }

    inline list(const list& p) noexcept : car(p.car), cdr(p.cdr) {
        debug_inc(countLists);
        debug_inc(countCLists);
#ifdef WANT_MAINTAINER_WATCH
        watch = p.watch;
#endif
    }

    list<T>& operator=(const list<T>& p) = delete;

    inline ~list() noexcept {
        debug_dec(countLists);
    }

    inline const bool operator==(const list<T>& p) const noexcept {
        if(this == &p)
            return true;
        if(isNull(cdr))
            return isNull(p.cdr);
        if(isNull(p.cdr))
            return false;
        if(!(car == p.car))
            return false;
        if(cdr == p.cdr)
            return true;
        return cdr() == p.cdr();
    }

    inline const bool operator<(const list<T>& p) const noexcept {
        if(this == &p)
            return false;
        if (isNull(cdr))
            return !isNull(p.cdr);
        if (isNull(p.cdr))
            return false;
        if (car < p.car)
            return true;
        if (car != p.car)
            return false;
        return cdr() < p.cdr();
    }

    inline const bool operator>(const list<T>& p) const noexcept {
        if(this == &p)
            return false;
        if (isNull(cdr))
            return false;
        if (isNull(p.cdr))
            return true;
        if (car > p.car)
            return true;
        if (car != p.car)
            return false;
        return cdr() > p.cdr();
    }

    inline const bool operator!=(const list<T>& p) const noexcept {
        return !this->operator==(p);
    }

    inline operator const list<list<T> >() const noexcept {
        return (list<list<T> >)T(*this);
    }

private:
#ifdef WANT_MAINTAINER_WATCH
    template<typename X> friend const string watchList(const list<X>& p) noexcept;
    string watch;
#endif

    template<typename X> friend const bool isNull(const list<X>& p) noexcept;
    template<typename X> friend const X car(const list<X>& p) noexcept;
    template<typename X> friend const list<X> cdr(const list<X>& p) noexcept;

    const T car;
    const lambda<list<T>()> cdr;
};

#ifdef WANT_MAINTAINER_WATCH

/**
 * Debug utility used to write the contents of a list to a string, easier
 * to watch than the list itself in a debugger.
 */
template<typename T> inline const string watchList(const list<T>& p) noexcept {
    if(isNull(p))
        return "()";
    odebugstream os;
    os << "(" << car(p) << " ...)";
    return str(os);
}

#endif

/**
 * Returns true if the given list is nil.
 */
template<typename T> inline const bool isNull(const list<T>& p) noexcept {
    return isNull(p.cdr);
}

/**
 * Write a list to an output stream.
 */
template<typename T> inline ostream& writeHelper(ostream& out, const list<T>& l) noexcept {
    if (isNull(l))
        return out;
    out << " " << car(l);
    return writeHelper(out, cdr(l));
}

template<typename T> inline ostream& operator<<(ostream& out, const list<T>& l) noexcept {
    if(isNull(l))
        return out << "()";
    out << "(" << car(l);
    writeHelper<T>(out, cdr(l));
    return out << ")";
}

/**
 * Construct a (lazy) list from a value and a lambda function that returns the cdr.
 */
template<typename T> inline const list<T> cons(const T& car, const lambda<const list<T>()>& cdr) noexcept {
    return list<T> (car, cdr);
}

/**
 * Construct a list from a value and a cdr list.
 */
template<typename T> inline const list<T> cons(const T& car, const list<T>& cdr) noexcept {
    return list<T> (car, result(cdr));
}

/**
 * Cons variations for use with the reduce and reduceRight functions.
 */
template<typename T> inline const list<T> lcons(const list<T>& cdr, const T& car) noexcept {
    return cons<T>(car, cdr);
}

template<typename T> inline const list<T> rcons(const T& car, const list<T>& cdr) noexcept {
    return cons<T>(car, cdr);
}

/**
 * Construct a list of one value.
 */
template<typename T> inline const list<T> mklist(const T& car) noexcept {
    return list<T> (car, result(list<T> ()));
}

/**
 * Construct a list of two values.
 */
template<typename T> inline const list<T> mklist(const T& a, const T& b) noexcept {
    return cons(a, mklist(b));
}

/**
 * Construct a list of three values.
 */
template<typename T> inline const list<T> mklist(const T& a, const T& b, const T& c) noexcept {
    return cons(a, cons(b, mklist(c)));
}

/**
 * Construct a list of four values.
 */
template<typename T> inline const list<T> mklist(const T& a, const T& b, const T& c, const T& d) noexcept {
    return cons(a, cons(b, cons(c, mklist(d))));
}

/**
 * Construct a list of five values.
 */
template<typename T> inline const list<T> mklist(const T& a, const T& b, const T& c, const T& d, const T& e) noexcept {
    return cons(a, cons(b, cons(c, cons(d, mklist(e)))));
}

/**
 * Construct a list of six values.
 */
template<typename T> inline const list<T> mklist(const T& a, const T& b, const T& c, const T& d, const T& e, const T& f) noexcept {
    return cons(a, cons(b, cons(c, cons(d, cons(e, mklist(f))))));
}

/**
 * Returns the car of a list.
 */
template<typename T> inline const T car(const list<T>& p) noexcept {
    // Abort if trying to access the car of a nil list
    assertOrFail(!isNull(p.cdr));
    return p.car;
}

/**
 * Returns the cdr of a list.
 */
template<typename T> inline const list<T> cdr(const list<T>& p) noexcept {
    return p.cdr();
}

/**
 * Returns the car of the cdr (the 2nd element) of a list.
 */
template<typename T> inline const T cadr(const list<T>& p) noexcept {
    return car(cdr(p));
}

/**
 * Returns the 3rd element of a list.
 */
template<typename T> inline const T caddr(const list<T>& p) noexcept {
    return car(cdr(cdr(p)));
}

/**
 * Returns the 4th element of a list.
 */
template<typename T> inline const T cadddr(const list<T>& p) noexcept {
    return car(cdr(cdr(cdr(p))));
}

/**
 * Returns the 5th element of a list.
 */
template<typename T> inline const T caddddr(const list<T>& p) noexcept {
    return car(cdr(cdr(cdr(cdr(p)))));
}

/**
 * Returns the 6th element of a list.
 */
template<typename T> inline const T cadddddr(const list<T>& p) noexcept {
    return car(cdr(cdr(cdr(cdr(cdr(p))))));
}

/**
 * Returns the 7th element of a list.
 */
template<typename T> inline const T caddddddr(const list<T>& p) noexcept {
    return car(cdr(cdr(cdr(cdr(cdr(cdr(p)))))));
}

/**
 * Returns the 8th element of a list.
 */
template<typename T> inline const T cadddddddr(const list<T>& p) noexcept {
    return car(cdr(cdr(cdr(cdr(cdr(cdr(cdr(p))))))));
}

/**
 * Returns a list of elements from the 3rd to the end of a list.
 */
template<typename T> inline const list<T> cddr(const list<T>& p) noexcept {
    return cdr(cdr(p));
}

/**
 * Returns a list of elements from the 4th to the end of a list.
 */
template<typename T> inline const list<T> cdddr(const list<T>& p) noexcept {
    return cdr(cdr(cdr(p)));
}

/**
 * Returns a list of elements from the 5th to the end of a list.
 */
template<typename T> inline const list<T> cddddr(const list<T>& p) noexcept {
    return cdr(cdr(cdr(cdr(p))));
}

/**
 * Returns a list of elements from the 6th to the end of a list.
 */
template<typename T> inline const list<T> cdddddr(const list<T>& p) noexcept {
    return cdr(cdr(cdr(cdr(cdr(p)))));
}

/**
 * Returns a list of elements from the 7th to the end of a list.
 */
template<typename T> inline const list<T> cddddddr(const list<T>& p) noexcept {
    return cdr(cdr(cdr(cdr(cdr(cdr(p))))));
}

/**
 * Returns a list of elements from the 8th to the end of a list.
 */
template<typename T> inline const list<T> cdddddddr(const list<T>& p) noexcept {
    return cdr(cdr(cdr(cdr(cdr(cdr(cdr(p)))))));
}

/**
 * Returns the length of a list.
 */
template<typename T> inline const size_t length(const list<T>& p) noexcept {
    const lambda<size_t(const size_t, const list<T>&)> lengthRef = [&lengthRef](const size_t c, const list<T>& p) -> const size_t {
        if(isNull(p))
            return c;
        return lengthRef(c + 1, cdr(p));
    };
    return lengthRef(0, p);
}

/**
 * Appends a list and a lambda function returning a list.
 */
template<typename T> inline const list<T> append(const list<T>&a, const lambda<const list<T>()>& fb) noexcept {
    if(isNull(a))
        return fb();
    return cons<T>(car(a), [a, fb]() { return append(cdr(a), fb); });
}

/**
 * Appends two lists.
 */
template<typename T> inline const list<T> append(const list<T>&a, const list<T>& b) noexcept {
    return append(a, result(b));
}

/**
 * Append a value to a list.
 */
template<typename T> inline const list<T> operator+(const list<T>& l, const T& v) noexcept {
    return append(l, mklist(v));
}

template<typename T, typename V> const list<T> inline operator+(const list<T>& l, const V& v) noexcept {
    return append(l, mklist<T>(v));
}

/**
 * Run a map lambda function on a list.
 */
template<typename T, typename R> inline const list<R> map(const lambda<const R(const T)>& f, const list<T>& p) noexcept {
    if(isNull(p))
        return list<R> ();
    return cons(f(car(p)), map(f, cdr(p)));
}

/**
 * Run a reduce lambda function on a list.
 */
template<typename T, typename R> inline const R reduce(const lambda<const R(const R, const T)>& f, const R& initial, const list<T>& p) noexcept {
    const lambda<const R(const R&, const list<T>&p)> reduceAccumulate = [f, &reduceAccumulate](const R& acc, const list<T>& p) -> R {
        if(isNull(p))
            return acc;
        return reduceAccumulate(f(acc, car(p)), cdr(p));
    };
    return reduceAccumulate(initial, p);
}

template<typename T, typename R> inline const R reduceRight(const lambda<const R(const T, const R)>& f, const R& initial, const list<T>& p) noexcept {
    const lambda<const R(const list<T>&p, const R&)> reduceRightAccumulate = [f, &reduceRightAccumulate](const list<T>& p, const R& acc) -> R {
        if(isNull(p))
            return acc;
        return reduceRightAccumulate(cdr(p), f(car(p), acc));
    };
    return reduceRightAccumulate(p, initial);
}

/**
 * Run a filter lambda function on a list.
 */
template<typename T> inline const list<T> filter(const lambda<const bool(const T)>& f, const list<T>& p) noexcept {
    if(isNull(p))
        return list<T> ();
    if(f(car(p))) {
        const lambda<const list<T>(const lambda<const bool(const T)>, const list<T>)> ff(filter<T>);
        return cons(car(p), curry(ff, f, cdr(p)));
    }
    return filter(f, cdr(p));
}

/**
 * Returns a list pointing to a member of a list.
 */
template<typename T> inline const list<T> member(const T& t, const list<T>& p) noexcept {
    if(isNull(p))
        return list<T> ();
    if(t == car(p))
        return p;
    return member(t, cdr(p));
}

/**
 * Reverse a list.
 */
template<typename T> inline const list<T> reverseIter(const list<T>& acc, const list<T>& p) noexcept {
    if(isNull(p))
        return acc;
    return reverseIter(cons(car(p), acc), cdr(p));
}

template<typename T> inline const list<T> reverse(const list<T>& p) noexcept {
    return reverseIter(list<T> (), p);
}

/**
 * Returns a sequence of values between the given bounds.
 */
template<typename T> inline const list<T> seq(const T& start, const T& end) noexcept {
    if(start == end)
        return mklist(start);
    if(start < end)
        return cons<T>(start, [start, end] { return seq<T> (start + 1, end); });
    return cons<T>(start, [start, end] { return seq<T> (start - 1, end); });
}

/**
 * Returns the i-th element of a list.
 */
template<typename T> inline const T listRef(const list<T>& l, const size_t i) noexcept {
    if(i == 0)
        return car(l);
    return listRef(cdr(l), i - 1);
}

/**
 * Returns a new list consisting of the first k elements of a list.
 */
template<typename T> inline const list<T> listHead(const list<T>& l, const size_t k) noexcept {
    if(k == 0)
        return list<T>();
    if(isNull(l))
        return l;
    return cons<T>(car(l), listHead(cdr(l), k - 1));
}

/**
 * Returns the tail of a list, ommiting the first k elements.
 */
template<typename T> inline const list<T> listTail(const list<T>& l, const size_t k) noexcept {
    if(k == 0)
        return l;
    if(isNull(l))
        return l;
    return listTail(cdr(l), k - 1);
}

/**
 * Substitute elements in a list.
 */
template<typename T> inline const list<T> subst(const T& o, const T& n, const list<T>& p) noexcept {
    if(isNull(p))
        return p;
    if(o == car(p))
        return cons<T>(n, subst(o, n, cdr(p)));
    return cons<T>(car(p), subst(o, n, cdr(p)));
}

/**
 * Returns the first pair matching a key from a list of key value pairs.
 */
template<typename T> inline const list<T> assoc(const T& k, const list<list<T> >& p) noexcept {
    if(isNull(p))
        return list<T>();
    if(k == car(car(p)))
        return car(p);
    return assoc(k, cdr(p));
}

/**
 * Returns the first pair matching a key from a list of key value pairs.
 * Requires T to support isList and cast to list<T>.
 */
template<typename T> inline const list<T> assoc(const T& k, const list<T>& p) noexcept {
    if(isNull(p))
        return list<T>();
    const T c = car(p);
    if(isList(c) && k == car<T>(c))
        return c;
    return assoc(k, cdr(p));
}

/**
 * Returns a list of lists containing elements from two input lists.
 */
template<typename T> inline const list<list<T> > zip(const list<T>& a, const list<T>& b) noexcept {
    if (isNull(a) || isNull(b))
        return list<list<T> >();
    return cons<list<T> >(mklist<T>(car(a), car(b)), zip(cdr(a), cdr(b)));
}

/**
 * Converts a list of key value pairs to a list containing the list of keys and the list of values.
 */
template<typename T> inline const list<T> unzipKeys(const list<list<T> >& l) noexcept {
    if (isNull(l))
        return list<T>();
    return cons(car(car(l)), unzipKeys(cdr(l)));
}

template<typename T> inline const list<T> unzipValues(const list<list<T> >& l) noexcept {
    if (isNull(l))
        return list<T>();
    return cons(cadr(car(l)), unzipValues(cdr(l)));
}

template<typename T> inline const list<list<T> > unzip(const list<list<T> >& l) noexcept {
    return mklist<list<T> >(unzipKeys(l), unzipValues(l));
}

/**
 * Delete assocs matching a key from a list of assocs.
 */
template<typename T> inline const list<list<T> > delAssoc(const T& k, const list<list<T> >& p) noexcept {
    if(isNull(p))
        return p;
    if(k == car(car(p)))
        return delAssoc(k, cdr(p));
    return cons<list<T> >(car(p), delAssoc(k, cdr(p)));
}

/**
 * Delete assocs matching a key from a list of assocs.
 * Requires T to support isList, isAssoc, and cast to list<T>.
 */
template<typename T> inline const list<T> delAssoc(const T& k, const list<T>& p) noexcept {
    if(isNull(p))
        return p;
    const T c = car(p);
    if(isList(c) && k == car<T>(c))
        return delAssoc(k, cdr(p));
    return cons<T>(c, delAssoc(k, cdr(p)));
}

/**
 * Substitute assocs with matching keys in a list of assocs.
 */
template<typename T> inline const list<list<T> > substAssoc(const T& k, const list<T>& n, const list<list<T> >& p, const bool add = false) noexcept {
    if(isNull(p))
        return add? mklist<list<T> >(n) : p;
    if(k == car(car(p)))
        return cons<list<T> >(n, substAssoc(k, n, cdr(p), false));
    return cons<list<T> >(car(p), substAssoc(k, n, cdr(p), add));
}

/**
 * Substitute assocs with matching keys in a list of assocs.
 * Requires T to support isList, isAssoc, and cast to list<T>.
 */
template<typename T> inline const list<T> substAssoc(const T& k, const list<T>& n, const list<T>& p, const bool add = false) noexcept {
    if(isNull(p))
        return add? mklist<T>(n) : p;
    const T c = car(p);
    if(isList(c) && k == car<T>(c))
        return cons<T>(n, substAssoc(k, n, cdr(p), false));
    return cons<T>(c, substAssoc(k, n, cdr(p), add));
}

}

#endif /* tuscany_list_hpp */
