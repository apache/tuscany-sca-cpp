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

#include <execinfo.h>
#include <cxxabi.h>
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
template<typename V, typename F = string, typename C = int> class failable {
public:
    failable() : hasv(false), c(-1) {
    }

    failable(const V& v) : hasv(true), v(v), c(-1) {
    }

    failable(const failable<V, F, C>& m) : hasv(m.hasv), v(m.v), f(m.f), c(m.c) {
    }

    const failable<V, F, C>& operator=(const failable<V, F, C>& m) {
        if (&m == this)
            return *this;
        hasv = m.hasv;
        v = m.v;
        f = m.f;
        c = m.c;
        return *this;
    }

    const bool operator!=(const failable<V, F, C>& m) const {
        return !this->operator==(m);
    }

    const bool operator==(const failable<V, F, C>& m) const {
        if (this == &m)
            return true;
        if (!hasv)
            return !m.hasv && f == m.f && c == m.c;
        return m.hasv && v == m.v;
    }

private:
    failable(const bool hasv, const F& f, const C& c) : hasv(hasv), f(f), c(c) {
    }

    template<typename A, typename B, typename R> friend const bool hasContent(const failable<A, B, R>& m);
    template<typename A, typename B, typename R> friend const A content(const failable<A, B, R>& m);
    template<typename A, typename B, typename R> friend const B reason(const failable<A, B, R>& m);
    template<typename A, typename B, typename R> friend const R rcode(const failable<A, B, R>& m);
    template<typename A, typename B, typename R> friend const failable<A, B, R> mkfailure(const B& f, const R& c, const bool log);
    template<typename A, typename B> friend const failable<A, B> mkfailure(const B& f, const int c, const bool log);
    template<typename A> friend const failable<A> mkfailure();

    bool hasv;
    V v;
    F f;
    C c;
};

/**
 * Write a failable monad to a stream.
 */
template<typename V, typename F, typename C> ostream& operator<<(ostream& out, const failable<V, F, C>& m) {
    if (!hasContent(m)) {
        out << reason(m) << " : " << rcode(m);
        return out;
    }
    out << content(m);
    return out;
}

/**
 * Returns a failable monad with a success value in it.
 */
template<typename V, typename F, typename C> const failable<V, F, C> mksuccess(const V& v) {
    return failable<V, F, C>(v);
}

template<typename V, typename F, typename C> const lambda<failable<V, F, C>(const V)> success() {
    return mksuccess<V, F, C>;
}

/**
 * Demangle a C++ function name.
 */
const string demangleFrame(const char* fun) {
    int status;
    char* name = abi::__cxa_demangle(fun, 0, 0, &status);
    if (name == NULL)
        return fun;
    const string s = name;
    free(name);
    return s;
}

/**
 * Format a backtrace frame.
 */
const char* formatFrameFile(const char* file) {
    const char* s = strrchr(file, '/');
    return s == NULL? file : s + 1;
}

const string formatFrame(const char* symbol) {
#ifdef __clang__
    // Mac OS X CLang/LLVM stack frame format
    // 0 kernel-test 0x000000010d440179 _ZN7tuscany9mkfailureINS_5valueENS_6stringEiEEKNS_8failableIT_T0_T1_EERKS5_RKS6_b + 265
    char nb[3];
    char file[256];
    char addr[32];
    char fun[256];
    char offset[16];
    if (sscanf(symbol, "%2s %255s %31s %255s %*[+] %15s", nb, file, addr, fun, offset) == 5) {
        char buf[1024];
        if (debug_islogging())
            sprintf(buf, "%.255s %.31s %.511s + %.15s", formatFrameFile(file), addr, c_str(demangleFrame(fun)), offset);
        else
            sprintf(buf, "%.255s %.31s", formatFrameFile(file), addr);
        return buf;
    }
#else
    // Linux GCC stack frame format
    // ./kernel-test(_ZN7tuscany9mkfailureINS_5valueENS_6stringEiEEKNS_8failableIT_T0_T1_EERKS5_RKS6_b+0x23d) [0xb7197afd]
    char file[256];
    char fun[256];
    char offset[16];
    char addr[32];
    if (sscanf(symbol, "%[^(]%*[(]%[^+]%*[+]%[^)]%*[)] %*[[]%[^]]%*[]]", file, fun, offset, addr) == 4) {
        char buf[1024];
        if (debug_islogging())
            sprintf(buf, "%.255s %.31s %.511s + %.15s", formatFrameFile(file), addr, c_str(demangleFrame(fun)), offset);
        else
            sprintf(buf, "%.255s %.31s", formatFrameFile(file), addr);
        return buf;
    }
    if (sscanf(symbol, "%[^(]%*[(]%*[^)]%*[)] %*[[]%[^]]%*[]]", file, addr) == 2) {
        char buf[512];
        sprintf(buf, "%.255s %.31s", formatFrameFile(file), addr);
        return buf;
    }
    if (sscanf(symbol, "%[^(]%*[(]%*[)] %*[[]%[^]]%*[]]", file, addr) == 2) {
        char buf[512];
        sprintf(buf, "%.255s %.31s", formatFrameFile(file), addr);
        return buf;
    }
#endif
    return symbol;
}

/**
 * Log backtrace frames.
 */
const bool logFrames(char** symbols, const int frames, const bool log) {
    if (frames == 0)
        return true;
#ifdef WANT_MAINTAINER_LOG
    if (!log)
        debug(formatFrame(*symbols), "failable::backtrace");
#endif
    if (log)
        cfailure << "failable::backtrace: " << formatFrame(*symbols) << endl;
    return logFrames(symbols + 1, frames - 1, log);
}

/**
 * Log a backtrace.
 */
const bool logBacktrace(void** callstack, const int frames, const bool log) {
    char** symbols = backtrace_symbols(callstack, frames);
    logFrames(symbols, frames, log);
    free(symbols);
    return true;
}

/**
 * Returns a failable monad with a failure in it.
 */
template<typename V, typename F, typename C> const failable<V, F, C> mkfailure(const F& f, const C& c, const bool log = true) {
#ifdef WANT_MAINTAINER_LOG
    if (!log) {
        // Log the failure
        debug(f, "failable::mkfailure");

        // Log the call stack
        void* callstack[16];
        const int frames = backtrace(callstack, 16);
        logBacktrace(callstack, frames, log);
    }
#endif
    if (log) {
        ostringstream os;
        os << f;
        if (length(str(os)) != 0) {
            // Log the failure
            cfailure << "failable::mkfailure: " << f << " : " << c << endl;

            // Print the call stack
            void* callstack[16];
            const int frames = backtrace(callstack, 16);
            logBacktrace(callstack, frames, log);
        }
    }
    return failable<V, F, C>(false, f, c);
}

template<typename V, typename F> const failable<V, F> mkfailure(const F& f, const int c = -1, const bool log = true) {
    return mkfailure<V, F, int>(f, c, log);
}

template<typename V> const failable<V> mkfailure(const char* f, const int c = -1, const bool log = true) {
    return mkfailure<V, string, int>(string(f), c, log);
}

template<typename V> const failable<V> mkfailure() {
    return failable<V, string>(false, string(), -1);
}

template<typename V, typename F, typename C> const lambda<failable<V, F, C>(const V)> failure() {
    return mkfailure<V, F, C>;
}

/**
 * Convert a failable of a given type to a failable of another type.
 */
template<typename V, typename F, typename C, typename X> const failable<V, F, C> mkfailure(const failable<X, F, C>& f, const bool log = true) {
    return mkfailure<V, F, C>(reason(f), rcode(f), log);
}

/**
 * Returns true if the monad contains a content.
 */
template<typename V, typename F, typename C> const bool hasContent(const failable<V, F, C>& m) {
    return m.hasv;
}

/**
 * Returns the content of a failable monad.
 */
template<typename V, typename F, typename C> const V content(const failable<V, F, C>& m) {
    return m.v;
}

/**
 * Returns the reason for failure of a failable monad.
 */
template<typename V, typename F, typename C> const F reason(const failable<V, F, C>& m) {
    return m.f;
}

/**
 * Returns the reason code for failure of a failable monad.
 */
template<typename V, typename F, typename C> const C rcode(const failable<V, F, C>& m) {
    return m.c;
}

/**
 * Bind a function to a failable monad. Passes the success value in the monad to the function
 * if present, or does nothing if there's no value and a failure instead.
 */
template<typename R, typename FR, typename XR, typename V, typename FV, typename XV>
const failable<R, FR, XR> operator>>(const failable<V, FV, XV>& m, const lambda<failable<R, FR, XR>(const V)>& f) {
    if (!hasContent(m))
        return m;
    return f(content(m));
}

template<typename R, typename FR, typename XR, typename V, typename FV, typename XV>
const failable<R, FR, XR> operator>>(const failable<V, FV, XV>& m, const failable<R, FR, XR> (* const f)(const V)) {
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
