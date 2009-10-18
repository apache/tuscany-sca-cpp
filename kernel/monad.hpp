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

#ifndef tuscany_monad_hpp
#define tuscany_monad_hpp

/**
 * Simple monad implementations.
 */

#include <string>
#include <iostream>
#include "function.hpp"

namespace tuscany
{

/**
 * Identity monad. Just wraps a value.
 * To get the value in the monad, just cast it to the value type.
 */
template<typename V> class id {
public:
    id(const V& v) : v(v) {
    }

    operator const V() const {
        return v;
    }

    const id<V>& operator=(const id<V>& m) {
        if(this == &m)
            return *this;
        v = m.v;
        return *this;
    }

    const bool operator!=(const id<V>& m) const {
        return !this->operator==(m);
    }

    const bool operator==(const id<V>& m) const {
        if (&m == this)
            return true;
        return v == m.v;
    }

private:
    const V v;
};

/**
 * Return an identity monad from a value.
 */
template<typename V> const id<V> mkunit(const V& v) {
    return id<V>(v);
}

template<typename V> const lambda<id<V>(V)> unit() {
    return mkunit<V>;
}

/**
 * Bind a function to an identity monad. Pass the value in the monad to the function.
 */
template<typename R, typename V> const id<R> operator>>(const id<V>& m, const lambda<id<R>(V)>& f) {
    return f(m);
}

template<typename R, typename V> const id<R> operator>>(const id<V>& m, const id<R> (* const f)(const V)) {
    return f(m);
}

/**
 * Maybe monad. Used to represent an optional value, which may be there or not.
 * To get the value in the monad, just cast it to the value type.
 */
template<typename V> class maybe {
public:
    maybe(const V& v) : hasv(true), v(v) {
    }

    maybe() : hasv(false) {
    }

    operator const V() const {
        return v;
    }

    const maybe<V>& operator=(const maybe<V>& m) {
        if(this == &m)
            return *this;
        hasv = m.hasv;
        if (hasv)
            v = m.v;
        return *this;
    }

    const bool operator!=(const maybe<V>& m) const {
        return !this->operator==(m);
    }

    const bool operator==(const maybe<V>& m) const {
        if (this == &m)
            return true;
        if (!hasv)
            return !m.hasv;
        return m.hasv && v == m.v;
    }

private:
    const bool hasv;
    V v;

    template<typename A> friend const bool hasValue(const maybe<A>& m);
};

/**
 * Return a maybe monad with a value in it.
 */
template<typename V> const maybe<V> mkjust(const V& v) {
    return maybe<V>(v);
}

template<typename V> const lambda<maybe<V>(V)> just() {
    return mkjust<V>;
}

/**
 * Returns true if the monad contains a value.
 */
template<typename V> const bool hasValue(const maybe<V>& m) {
    return m.hasv;
}

/**
 * Bind a function to a maybe monad. Passes the value in the monad to the function
 * if present, or does nothing if there's no value.
 */
template<typename R, typename V> const maybe<R> operator>>(const maybe<V>& m, const lambda<maybe<R>(V)>& f) {
    if (!hasValue(m))
        return m;
    return f(m);
}

template<typename R, typename V> const maybe<R> operator>>(const maybe<V>& m, const maybe<R> (* const f)(const V)) {
    if (!hasValue(m))
        return m;
    return f(m);
}

/**
 * Failable monad. Used to represent either a success value or a failure.
 * To get the value in the monad, just cast it to the value type.
 * To get the failure in the monad, cast it to the failure type.
 */
template<typename V, typename F> class failable {
public:
    failable(const V& v) : hasv(true), v(v) {
    }

    failable(const F& f) : hasv(false), f(f) {
    }

    operator const V() const {
        return v;
    }

    operator const F() const {
        return f;
    }

    const failable<V, F>& operator=(const failable<V, F>& m) {
        if(this == &m)
            return *this;
        hasv = m.hasv;
        if (hasv)
            v = m.v;
        else
            f = m.f;
        return *this;
    }

    const bool operator!=(const failable<V, F>& m) const {
        return !this->operator==(m);
    }

    const bool operator==(const failable<V, F>& m) const {
        if (this == &m)
            return true;
        if (!hasv)
            return !m.hasv && f == m.f;
        return m.hasv && v == m.v;
    }

private:
    const bool hasv;
    V v;
    F f;

    template<typename A, typename B> friend const bool hasValue(const failable<A, B>& m);
};

/**
 * Returns a failable monad with a success value in it.
 */
template<typename V, typename F> const failable<V, F> mksuccess(const V& v) {
    return failable<V, F>(v);
}

template<typename V, typename F> const lambda<failable<V, F>(V)> success() {
    return mksuccess<V, F>;
}

/**
 * Returns true if the monad contains a value.
 */
template<typename V, typename F> const bool hasValue(const failable<V, F>& m) {
    return m.hasv;
}

/**
 * Bind a function to a failable monad. Passes the success value in the monad to the function
 * if present, or does nothing if there's no value and a failure instead.
 */
template<typename R, typename FR, typename V, typename FV>
const failable<R, FR> operator>>(const failable<V, FV>& m, const lambda<failable<R, FR>(V)>& f) {
    if (!hasValue(m))
        return m;
    return f(m);
}

template<typename R, typename FR, typename V, typename FV>
const failable<R, FR> operator>>(const failable<V, FV>& m, const failable<R, FR> (* const f)(const V)) {
    if (!hasValue(m))
        return m;
    return f(m);
}

/**
 * State + value pair data type used by the state monad.
 */
template<typename S, typename V> class svp {
public:
    svp(const S& s, const V& v) : s(s), v(v) {
    }

    operator const S() const {
        return s;
    }

    operator const V() const {
        return v;
    }

    const svp<S, V>& operator=(const svp<S, V>& p) {
        if(this == &p)
            return *this;
        s = p.s;
        v = p.v;
        return *this;
    }

    const bool operator!=(const svp<S, V>& p) const {
        return !this->operator==(p);
    }

    const bool operator==(const svp<S, V>& p) const {
        if (this == &p)
            return true;
        return s == p.s && v == p.v;
    }

private:
    const S s;
    const V v;
};

/**
 * State monad. Used to represent the combination of a state and a value.
 * To get the state in the monad, just cast it to the state type.
 * To get the value in the monad, just cast it to the value type.
 */
template<typename S, typename V> class state {
public:
    state(const lambda<svp<S, V>(S)>& f) : f(f) {
    }

    const svp<S, V> operator()(const S& s) const {
        return f(s);
    }

    const state<S, V>& operator=(const state<S, V>& m) {
        if(this == &m)
            return *this;
        f = m.f;
        return *this;
    }

    const bool operator!=(const state<S, V>& m) const {
        return !this->operator==(m);
    }

    const bool operator==(const state<S, V>& m) const {
        if (this == &m)
            return true;
        return f == m.f;
    }

private:
    const lambda<svp<S, V>(S)> f;
};

/**
 * Return a state monad carrying a result value.
 */
template<typename S, typename V> struct returnState {
    const V v;
    returnState(const V& v) : v(v) {
    }
    const svp<S, V> operator()(const S& s) const {
        return svp<S, V>(s, v);
    }
};

template<typename S, typename V> const state<S, V> result(const V& v) {
    return state<S, V>(returnState<S, V>(v));
}

/**
 * Return a state monad with a transformer function.
 * A transformer function takes a state and returns an svp pair carrying a value and a
 * new (transformed) state.
 */
template<typename S, typename V> const state<S, V> transformer(const lambda<svp<S, V>(S)>& f) {
    return state<S, V>(f);
}

/**
 * Bind a function to a state monad. The function takes a value and returns a state
 * monad carrying a return value.
 */
template<typename S, typename A, typename B> struct stateBind {
    const state<S, A> st;
    const lambda<state<S, B>(A)>f;

    stateBind(const state<S, A>& st, const lambda<state<S, B>(A)>& f) : st(st), f(f) {
    }

    const svp<S, B> operator()(const S& is) const {
        const svp<S, A> iscp = st(is);
        const state<S, B> m = f((A)iscp);
        return m((S)iscp);
    }
};

template<typename S, typename A, typename B>
const state<S, B> operator>>(const state<S, A>& st, const lambda<state<S, B>(A)>& f) {
    return state<S, B>(stateBind<S, A , B>(st, f));
}

template<typename S, typename A, typename B>
const state<S, B> operator>>(const state<S, A>& st, const state<S, B> (* const f)(const A)) {
    return state<S, B>(stateBind<S, A , B>(st, f));
}

}
#endif /* tuscany_monad_hpp */
