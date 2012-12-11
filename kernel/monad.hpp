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

#ifdef WANT_MAINTAINER_BACKTRACE
#include <execinfo.h>
#include <cxxabi.h>
#endif

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
    inline id(const V& v) : v(v) {
    }

    id<V>& operator=(const id<V>& m) = delete;

    inline const bool operator!=(const id<V>& m) const {
        return !this->operator==(m);
    }

    inline const bool operator==(const id<V>& m) const {
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
template<typename V> inline ostream& operator<<(ostream& out, const id<V>& m) {
    out << content(m);
    return out;
}

/**
 * Returns the content of an identity monad.
 */
template<typename V> inline const V content(const id<V>& m) {
    return m.v;
}

/**
 * Return an identity monad from a value.
 */
template<typename V> inline const id<V> mkunit(const V& v) {
    return id<V>(v);
}

template<typename V> inline const lambda<const id<V>(const V)> unit() {
    return mkunit<V>;
}

/**
 * Bind a function to an identity monad. Pass the value in the monad to the function.
 */
template<typename R, typename V> inline const id<R> operator>>(const id<V>& m, const lambda<const id<R>(const V)>& f) {
    return f(content(m));
}

template<typename R, typename V> inline const id<R> operator>>(const id<V>& m, const id<R> (* const f)(const V)) {
    return f(content(m));
}

/**
 * Maybe monad. Used to represent an optional value, which may be there or not.
 * To get the value in the monad, just cast it to the value type.
 */
template<typename V> class maybe {
public:
    inline maybe(const V& v) : hasv(true), v(v) {
    }

    inline maybe() : hasv(false), v() {
    }

    inline maybe(const maybe<V>& m) : hasv(m.hasv), v(m.v) {
    }

    maybe<V>& operator=(const maybe<V>& m) = delete;

    inline const bool operator!=(const maybe<V>& m) const {
        return !this->operator==(m);
    }

    inline const bool operator==(const maybe<V>& m) const {
        if (this == &m)
            return true;
        if (!hasv)
            return !m.hasv;
        return m.hasv && v == m.v;
    }

private:
    const bool hasv;
    const V v;

    template<typename X> friend const bool hasContent(const maybe<X>& m);
    template<typename X> friend const X content(const maybe<X>& m);
};

/**
 * Write a maybe monad to a stream.
 */
template<typename V> inline ostream& operator<<(ostream& out, const maybe<V>& m) {
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
template<typename V> inline const maybe<V> mkjust(const V& v) {
    return maybe<V>(v);
}

template<typename V> inline const lambda<const maybe<V>(const V)> just() {
    return mkjust<V>;
}

/**
 * Returns true if a maybe monad contains a content.
 */
template<typename V> inline const bool hasContent(const maybe<V>& m) {
    return m.hasv;
}

/**
 * Returns the content of a maybe monad.
 */
template<typename V> inline const V content(const maybe<V>& m) {
    return m.v;
}

/**
 * Bind a function to a maybe monad. Passes the value in the monad to the function
 * if present, or does nothing if there's no value.
 */
template<typename R, typename V> inline const maybe<R> operator>>(const maybe<V>& m, const lambda<const maybe<R>(const V)>& f) {
    if (!hasContent(m))
        return m;
    return f(content(m));
}

template<typename R, typename V> inline const maybe<R> operator>>(const maybe<V>& m, const maybe<R> (* const f)(const V)) {
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
    inline failable() : hasv(false), v(), c(-1) {
    }

    inline failable(const V& v) : hasv(true), v(v), c(-1) {
    }

    inline failable(const failable<V, F, C>& m) : hasv(m.hasv), v(m.v), f(m.f), c(m.c) {
    }

    failable<V, F, C>& operator=(const failable<V, F, C>& m) = delete;

    inline const bool operator!=(const failable<V, F, C>& m) const {
        return !this->operator==(m);
    }

    inline const bool operator==(const failable<V, F, C>& m) const {
        if (this == &m)
            return true;
        if (!hasv)
            return !m.hasv && f == m.f && c == m.c;
        return m.hasv && v == m.v;
    }

private:
    inline failable(const bool hasv, const F& f, const C& c) : hasv(hasv), v(), f(f), c(c) {
    }

    template<typename A, typename B, typename R> friend const bool hasContent(const failable<A, B, R>& m);
    template<typename A, typename B, typename R> friend const A content(const failable<A, B, R>& m);
    template<typename A, typename B, typename R> friend const B reason(const failable<A, B, R>& m);
    template<typename A, typename B, typename R> friend const R rcode(const failable<A, B, R>& m);
    template<typename A, typename B, typename R> friend const failable<A, B, R> mkfailure(const B& f, const R& c, const bool log);
    template<typename A, typename B> friend const failable<A, B> mkfailure(const B& f, const int c, const bool log);
    template<typename A> friend const failable<A> mkfailure();

    const bool hasv;
    const V v;
    const F f;
    const C c;
};

/**
 * Write a failable monad to a stream.
 */
template<typename V, typename F, typename C> inline ostream& operator<<(ostream& out, const failable<V, F, C>& m) {
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
template<typename V, typename F, typename C> inline const failable<V, F, C> mksuccess(const V& v) {
    return failable<V, F, C>(v);
}

template<typename V, typename F, typename C> inline const lambda<const failable<V, F, C>(const V)> success() {
    return mksuccess<V, F, C>;
}

#ifdef WANT_MAINTAINER_BACKTRACE

/**
 * Demangle a C++ function name.
 */
inline const string demangleFrame(const char* fun) {
    int status;
    char* const name = abi::__cxa_demangle(fun, 0, 0, &status);
    if (name == NULL)
        return fun;
    const string s = name;
    free(name);
    return s;
}

/**
 * Format a backtrace frame.
 */
inline const char* const formatFrameFile(const char* const file) {
    const char* const s = strrchr(file, '/');
    return s == NULL? file : s + 1;
}

inline const string formatFrame(const char* const symbol) {
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
inline const bool logFrames(char** const symbols, const int frames, const bool log) {
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
inline const bool logBacktrace(void** const callstack, const int frames, const bool log) {
    char** symbols = backtrace_symbols(callstack, frames);
    logFrames(symbols, frames, log);
    free(symbols);
    return true;
}

#endif

/**
 * Returns a failable monad with a failure in it.
 */
template<typename V, typename F, typename C> inline const failable<V, F, C> mkfailure(const F& f, const C& c, const bool log = true) {
#ifdef WANT_MAINTAINER_LOG
    if (!log) {
        // Log the failure
        debug(f, "failable::mkfailure");

#ifdef WANT_MAINTAINER_BACKTRACE
        // Log the call stack
        void* callstack[16];
        const int frames = backtrace(callstack, 16);
        logBacktrace(callstack, frames, log);
#endif
    }
#endif
    if (log) {
        ostringstream os;
        os << f;
        if (length(str(os)) != 0) {
            // Log the failure
            cfailure << "failable::mkfailure: " << f << " : " << c << endl;

#ifdef WANT_MAINTAINER_BACKTRACE
            // Print the call stack
            void* callstack[16];
            const int frames = backtrace(callstack, 16);
            logBacktrace(callstack, frames, log);
#endif
        }
    }
    return failable<V, F, C>(false, f, c);
}

template<typename V, typename F> inline const failable<V, F> mkfailure(const F& f, const int c = -1, const bool log = true) {
    return mkfailure<V, F, int>(f, c, log);
}

template<typename V> inline const failable<V> mkfailure(const char* f, const int c = -1, const bool log = true) {
    return mkfailure<V, string, int>(string(f), c, log);
}

template<typename V> inline const failable<V> mkfailure() {
    return failable<V, string>(false, string(), -1);
}

template<typename V, typename F, typename C> inline const lambda<const failable<V, F, C>(const V)> failure() {
    return mkfailure<V, F, C>;
}

/**
 * Convert a failable of a given type to a failable of another type.
 */
template<typename V, typename F, typename C, typename X> inline const failable<V, F, C> mkfailure(const failable<X, F, C>& f, const bool log = false) {
    return mkfailure<V, F, C>(reason(f), rcode(f), log);
}

/**
 * Returns true if the monad contains a content.
 */
template<typename V, typename F, typename C> inline const bool hasContent(const failable<V, F, C>& m) {
    return m.hasv;
}

/**
 * Returns the content of a failable monad.
 */
template<typename V, typename F, typename C> inline const V content(const failable<V, F, C>& m) {
    return m.v;
}

/**
 * Returns the reason for failure of a failable monad.
 */
template<typename V, typename F, typename C> inline const F reason(const failable<V, F, C>& m) {
    return m.f;
}

/**
 * Returns the reason code for failure of a failable monad.
 */
template<typename V, typename F, typename C> inline const C rcode(const failable<V, F, C>& m) {
    return m.c;
}

/**
 * Bind a function to a failable monad. Passes the success value in the monad to the function
 * if present, or does nothing if there's no value and a failure instead.
 */
template<typename R, typename FR, typename XR, typename V, typename FV, typename XV>
inline const failable<R, FR, XR> operator>>(const failable<V, FV, XV>& m, const lambda<const failable<R, FR, XR>(const V)>& f) {
    if (!hasContent(m))
        return m;
    return f(content(m));
}

template<typename R, typename FR, typename XR, typename V, typename FV, typename XV>
inline const failable<R, FR, XR> operator>>(const failable<V, FV, XV>& m, const failable<R, FR, XR> (* const f)(const V)) {
    if (!hasContent(m))
        return m;
    return f(content(m));
}

/**
 * State + content pair data type used by the state monad.
 */
template<typename S, typename V> class scp {
public:
    inline scp(const S& s, const V& v) : s(s), v(v) {
    }

    inline operator const S() const {
        return s;
    }

    inline operator const V() const {
        return v;
    }

    inline scp(const scp<S, V>& p) : s(p.s), v(p.v) {
    }

    scp<S, V>& operator=(const scp<S, V>& p) = delete;

    inline const bool operator!=(const scp<S, V>& p) const {
        return !this->operator==(p);
    }

    inline const bool operator==(const scp<S, V>& p) const {
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
template<typename S, typename V> inline const S scpstate(const scp<S, V>& m) {
    return m.s;
}

/**
 * Returns the content of a state-content pair.
 */
template<typename S, typename V> inline const S content(const scp<S, V>& m) {
    return m.v;
}

/**
 * State monad. Used to represent the combination of a state and a content.
 */
template<typename S, typename V> class state {
public:
    inline state(const lambda<const scp<S, V>(const S)>& f) : f(f) {
    }

    inline const scp<S, V> operator()(const S& s) const {
        return f(s);
    }

    inline state(const state<S, V>& m) : f(m.f) {
    }

    state<S, V>& operator=(const state<S, V>& m) = delete;

    inline const bool operator!=(const state<S, V>& m) const {
        return !this->operator==(m);
    }

    inline const bool operator==(const state<S, V>& m) const {
        if (this == &m)
            return true;
        return f == m.f;
    }

private:
    const lambda<const scp<S, V>(const S)> f;
};

/**
 * Write a state monad to a stream.
 */
template<typename S, typename V> inline ostream& operator<<(ostream& out, const state<S, V>& m) {
    const S s = m;
    const V v = m;
    out << '(' << s << ' ' << v << ')';
    return out;
}

/**
 * Return a state monad carrying a result content.
 */
template<typename S, typename V> inline const state<S, V> result(const V& v) {
    return state<S, V>([v](const S& s) -> const scp<S, V> {
        return scp<S, V>(s, v);
    });
}

/**
 * Return a state monad with a transformer function.
 * A transformer function takes a state and returns an scp pair carrying a content and a
 * new (transformed) state.
 */
template<typename S, typename V> inline const state<S, V> transformer(const lambda<const scp<S, V>(const S)>& f) {
    return state<S, V>(f);
}

/**
 * Bind a function to a state monad. The function takes a content and returns a state
 * monad carrying a return content.
 */
template<typename S, typename A, typename B>
inline const state<S, B> operator>>(const state<S, A>& st, const lambda<const state<S, B>(const A)>& f) {
    const lambda<const scp<S, B>(const S&)> stateBind = [st, f](const S& is) -> const scp<S, B> {
        const scp<S, A> iscp = st(is);
        const state<S, B> m = f((A)iscp);
        return m((S)iscp);
    };
    return state<S, B>(stateBind);
}

template<typename S, typename A, typename B>
inline const state<S, B> operator>>(const state<S, A>& st, const state<S, B> (* const f)(const A)) {
    return st >> lambda<const state<S, B>(const A)>(f);
}

}
#endif /* tuscany_monad_hpp */
