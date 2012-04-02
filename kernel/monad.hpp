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

#include "function.hpp"
#include "string.hpp"
#include "stream.hpp"
#include "sstream.hpp"
#include "fstream.hpp"

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

    template<typename X> friend const X content(const id<X>& m);
};

/**
 * Write an identity monad to a stream.
 */
template<typename V> ostream& operator<<(ostream& out, const id<V>& m) {
    out << content(m);
    return out;
}

/**
 * Returns the content of an identity monad.
 */
template<typename V> const V content(const id<V>& m) {
    return m.v;
}

/**
 * Return an identity monad from a value.
 */
template<typename V> const id<V> mkunit(const V& v) {
    return id<V>(v);
}

template<typename V> const lambda<id<V>(const V)> unit() {
    return mkunit<V>;
}

/**
 * Bind a function to an identity monad. Pass the value in the monad to the function.
 */
template<typename R, typename V> const id<R> operator>>(const id<V>& m, const lambda<id<R>(const V)>& f) {
    return f(content(m));
}

template<typename R, typename V> const id<R> operator>>(const id<V>& m, const id<R> (* const f)(const V)) {
    return f(content(m));
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

    template<typename X> friend const bool hasContent(const maybe<X>& m);
    template<typename X> friend const X content(const maybe<X>& m);
};

/**
 * Write a maybe monad to a stream.
 */
template<typename V> ostream& operator<<(ostream& out, const maybe<V>& m) {
    if (!hasContent(m)) {
        out << "nothing";
        return out;
    }
    out << content(m);
    return out;
}

/**
 * Return a maybe monad with a value in it.
 */
template<typename V> const maybe<V> mkjust(const V& v) {
    return maybe<V>(v);
}

template<typename V> const lambda<maybe<V>(const V)> just() {
    return mkjust<V>;
}

/**
 * Returns true if a maybe monad contains a content.
 */
template<typename V> const bool hasContent(const maybe<V>& m) {
    return m.hasv;
}

/**
 * Returns the content of a maybe monad.
 */
template<typename V> const V content(const maybe<V>& m) {
    return m.v;
}

/**
 * Bind a function to a maybe monad. Passes the value in the monad to the function
 * if present, or does nothing if there's no value.
 */
template<typename R, typename V> const maybe<R> operator>>(const maybe<V>& m, const lambda<maybe<R>(const V)>& f) {
    if (!hasContent(m))
        return m;
    return f(content(m));
}

template<typename R, typename V> const maybe<R> operator>>(const maybe<V>& m, const maybe<R> (* const f)(const V)) {
    if (!hasContent(m))
        return m;
    return f(content(m));
}

/**
 * Failable monad. Used to represent either a success value or a failure.
 * To get the value in the monad, just cast it to the value type.
 * To get the failure in the monad, cast it to the failure type.
 */
template<typename V, typename F = string> class failable {
public:
    failable() : hasv(false) {
    }

    failable(const V& v) : hasv(true), v(v) {
    }

    failable(const failable<V, F>& m) : hasv(m.hasv), v(m.v), f(m.f) {
    }

    const failable<V, F>& operator=(const failable<V, F>& m) {
        if (&m == this)
            return *this;
        hasv = m.hasv;
        v = m.v;
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
    failable(const bool hasv, const F& f) : hasv(hasv), f(f) {
    }

    template<typename A, typename B> friend const bool hasContent(const failable<A, B>& m);
    template<typename A, typename B> friend const A content(const failable<A, B>& m);
    template<typename A, typename B> friend const B reason(const failable<A, B>& m);
    template<typename A, typename B> friend const failable<A, B> mkfailure(const B& f, const bool log);
    template<typename A> friend const failable<A, string> mkfailure();

    bool hasv;
    V v;
    F f;
};

/**
 * Write a failable monad to a stream.
 */
template<typename V, typename F> ostream& operator<<(ostream& out, const failable<V, F>& m) {
    if (!hasContent(m)) {
        out << reason(m);
        return out;
    }
    out << content(m);
    return out;
}

/**
 * Returns a failable monad with a success value in it.
 */
template<typename V, typename F> const failable<V, F> mksuccess(const V& v) {
    return failable<V, F>(v);
}

template<typename V, typename F> const lambda<failable<V, F>(const V)> success() {
    return mksuccess<V, F>;
}

/**
 * Returns a failable monad with a failure in it.
 */
template<typename V, typename F> const failable<V, F> mkfailure(const F& f, const bool log = true) {
#ifdef WANT_MAINTAINER_LOG
    if (!log)
        debug(f, "failable::mkfailure");
#endif
    if (log) {
        ostringstream os;
        os << f;
        if (length(str(os)) != 0)
            cfailure << "failable::mkfailure" << ": " << f << endl;
    }
    return failable<V, F>(false, f);
}

template<typename V> const failable<V> mkfailure(const char* f, const bool log = true) {
    return mkfailure<V, string>(string(f), log);
}

template<typename V> const failable<V> mkfailure() {
    return failable<V, string>(false, string());
}

template<typename V, typename F> const lambda<failable<V, F>(const V)> failure() {
    return mkfailure<V, F>;
}

/**
 * Returns true if the monad contains a content.
 */
template<typename V, typename F> const bool hasContent(const failable<V, F>& m) {
    return m.hasv;
}

/**
 * Returns the content of a failable monad.
 */
template<typename V, typename F> const V content(const failable<V, F>& m) {
    return m.v;
}

/**
 * Returns the reason for failure of a failable monad.
 */
template<typename V, typename F> const F reason(const failable<V, F>& m) {
    return m.f;
}

/**
 * Bind a function to a failable monad. Passes the success value in the monad to the function
 * if present, or does nothing if there's no value and a failure instead.
 */
template<typename R, typename FR, typename V, typename FV>
const failable<R, FR> operator>>(const failable<V, FV>& m, const lambda<failable<R, FR>(const V)>& f) {
    if (!hasContent(m))
        return m;
    return f(content(m));
}

template<typename R, typename FR, typename V, typename FV>
const failable<R, FR> operator>>(const failable<V, FV>& m, const failable<R, FR> (* const f)(const V)) {
    if (!hasContent(m))
        return m;
    return f(content(m));
}

/**
 * State + content pair data type used by the state monad.
 */
template<typename S, typename V> class scp {
public:
    scp(const S& s, const V& v) : s(s), v(v) {
    }

    operator const S() const {
        return s;
    }

    operator const V() const {
        return v;
    }

    const scp<S, V>& operator=(const scp<S, V>& p) {
        if(this == &p)
            return *this;
        s = p.s;
        v = p.v;
        return *this;
    }

    const bool operator!=(const scp<S, V>& p) const {
        return !this->operator==(p);
    }

    const bool operator==(const scp<S, V>& p) const {
        if (this == &p)
            return true;
        return s == p.s && v == p.v;
    }

private:
    const S s;
    const V v;

    template<typename A, typename B> friend const A scpstate(const scp<A, B>& m);
    template<typename A, typename B> friend const B content(const scp<A, B>& m);
};

/**
 * Returns the state of a state-content pair.
 */
template<typename S, typename V> const S scpstate(const scp<S, V>& m) {
    return m.s;
}

/**
 * Returns the content of a state-content pair.
 */
template<typename S, typename V> const S content(const scp<S, V>& m) {
    return m.v;
}

/**
 * State monad. Used to represent the combination of a state and a content.
 */
template<typename S, typename V> class state {
public:
    state(const lambda<scp<S, V>(const S)>& f) : f(f) {
    }

    const scp<S, V> operator()(const S& s) const {
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
    const lambda<scp<S, V>(const S)> f;
};

/**
 * Write a state monad to a stream.
 */
template<typename S, typename V> ostream& operator<<(ostream& out, const state<S, V>& m) {
    const S s = m;
    const V v = m;
    out << '(' << s << ' ' << v << ')';
    return out;
}

/**
 * Return a state monad carrying a result content.
 */
template<typename S, typename V> struct returnState {
    const V v;
    returnState(const V& v) : v(v) {
    }
    const scp<S, V> operator()(const S& s) const {
        return scp<S, V>(s, v);
    }
};

template<typename S, typename V> const state<S, V> result(const V& v) {
    return state<S, V>(returnState<S, V>(v));
}

/**
 * Return a state monad with a transformer function.
 * A transformer function takes a state and returns an scp pair carrying a content and a
 * new (transformed) state.
 */
template<typename S, typename V> const state<S, V> transformer(const lambda<scp<S, V>(const S)>& f) {
    return state<S, V>(f);
}

/**
 * Bind a function to a state monad. The function takes a content and returns a state
 * monad carrying a return content.
 */
template<typename S, typename A, typename B> struct stateBind {
    const state<S, A> st;
    const lambda<state<S, B>(const A)>f;

    stateBind(const state<S, A>& st, const lambda<state<S, B>(const A)>& f) : st(st), f(f) {
    }

    const scp<S, B> operator()(const S& is) const {
        const scp<S, A> iscp = st(is);
        const state<S, B> m = f((A)iscp);
        return m((S)iscp);
    }
};

template<typename S, typename A, typename B>
const state<S, B> operator>>(const state<S, A>& st, const lambda<state<S, B>(const A)>& f) {
    return state<S, B>(stateBind<S, A , B>(st, f));
}

template<typename S, typename A, typename B>
const state<S, B> operator>>(const state<S, A>& st, const state<S, B> (* const f)(const A)) {
    return state<S, B>(stateBind<S, A , B>(st, f));
}

}
#endif /* tuscany_monad_hpp */
