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

#ifndef tuscany_function_hpp
#define tuscany_function_hpp

/**
 * Lambda function type that enforces pass by ref and constant parameters,
 * allocates memory only as needed and using an APR memory pool, is about
 * 2 times faster than std::function and between 1/4 and 1/6 of its size.
 */

#include <utility>
#include "fstream.hpp"
#include "gc.hpp"
#include "config.hpp"

namespace tuscany {

#ifdef WANT_MAINTAINER_COUNTERS

/**
 * Debug counters.
 */
long int countProxies;
long int countFProxies = 0;
long int countCProxies = 0;
long int countLambdas = 0;
long int countELambdas = 0;
long int countCLambdas = 0;
long int countFLambdas = 0;

inline const bool resetLambdaCounters() {
    countLambdas = countELambdas = countCLambdas = countFLambdas = countProxies = countFProxies = countCProxies = 0;
    return true;
}

inline const bool checkLambdaCounters() {
    return countLambdas == 0;
}

inline const bool printLambdaCounters() {
    cout << "countLambdas " << countLambdas << endl;
    cout << "countELambdas " << countELambdas << endl;
    cout << "countFLambdas " << countFLambdas << endl;
    cout << "countCLambdas " << countCLambdas << endl;
    cout << "countProxies " << countProxies << endl;
    cout << "countFProxies " << countFProxies << endl;
    cout << "countCProxies " << countCProxies << endl;
    return true;
}

#else

#define resetLambdaCounters()
#define checkLambdaCounters() true
#define printLambdaCounters()

#endif

/**
 * Lambda function type.
 */

template<typename R, typename... P> class Callable {
public:
    inline Callable() noexcept {
    }

    virtual const R operator()(const P&&... p) const noexcept = 0;

    inline virtual ~Callable() noexcept {
    }

    template<typename F> class Proxy: public Callable<R, P...> {
    public:
        inline Proxy(const F& f) noexcept : function(f) {
            debug_inc(countProxies);
            debug_inc(countFProxies);
        }

        inline Proxy(const Proxy& p) noexcept : function(p.function) {
            debug_inc(countProxies);
            debug_inc(countCProxies);
        }

        inline ~Proxy() noexcept {
            debug_dec(countProxies);
        }

        virtual const R operator() (const P&&... p) const noexcept {
            return function(std::forward<const P&&>(p)...);
        }

    private:
        const F function;
    };
};

template<typename S> class lambda;

template<typename R, typename... P> class lambda<R(P...)> {
public:
    inline lambda() noexcept : callable(NULL) {
        debug_inc(countLambdas);
        debug_inc(countELambdas);
    }

    template<typename F> inline lambda(const F f) noexcept : callable(mkproxy<F>(f)) {
        debug_inc(countLambdas);
        debug_inc(countFLambdas);
    }

    template<typename F> inline lambda(const gc_mutable_ref<F>& r) noexcept : callable(mkproxy<F>(*(F*)r)) {
        debug_inc(countLambdas);
        debug_inc(countFLambdas);
    }

    inline lambda(const lambda& l) noexcept : callable(l.callable) {
        debug_inc(countLambdas);
        debug_inc(countCLambdas);
    }

    lambda& operator=(const lambda& l) = delete;

    inline ~lambda() noexcept {
        debug_dec(countLambdas);
    }

    inline const bool operator==(const lambda& l) const noexcept {
        if (this == &l)
            return true;
        return false;
    }

    inline const bool operator!=(const lambda& l) const noexcept {
        return !this->operator==(l);
    }

    inline const R operator()(const P&... p) const noexcept {
        return (*callable)(std::forward<const P&&>(p)...);
    }

    template<typename S> friend ostream& operator<<(ostream&, const lambda<S>&);
    template<typename S> friend const bool isNil(const lambda<S>& l) noexcept;

private:
    typedef Callable<R,P...> CallableType;
    const gc_ptr<CallableType> callable;

    template<typename F> inline gc_ptr<CallableType> mkproxy(const F& f) noexcept {
        typedef typename CallableType::template Proxy<F> ProxyType;
        return gc_ptr<CallableType>(new (gc_new<ProxyType>()) ProxyType(f));
    }
};

template<typename S> inline ostream& operator<<(ostream& out, const lambda<S>& l) {
    return out << "lambda::" << l.callable;
}

/**
 * Return true if a lambda is nil.
 */
template<typename S> inline const bool isNil(const lambda<S>& l) noexcept {
    return (const void*)l.callable == NULL;
}

/**
 * Curry a lambda function.
 */
template<typename R, typename T, typename... P> class curried {
public:
    inline curried(const lambda<R(T, P...)>& f, const T& v) noexcept: v(v), f(f) {
    }

    inline const R operator()(const P&... p) const noexcept {
        return f(v, std::forward<const P&>(p)...);
    }

private:
    const T v;
    const lambda<R(T, P...)>f;
};

template<typename R, typename T, typename... P> inline const lambda<const R(P...)> curry(const lambda<R(T, P...)>& f, const T& t) noexcept {
    return curried<R, T, P...>(f, t);
}

template<typename R, typename T, typename U, typename... P> inline const lambda<R(P...)> curry(const lambda<R(T, U, P...)>& f, const T& t, const U& u) noexcept {
    return curry(curry(f, t), u);
}

template<typename R, typename T, typename U, typename V, typename... P> inline const lambda<R(P...)> curry(const lambda<R(T, U, V, P...)>& f, const T& t, const U& u, const V& v) noexcept {
    return curry(curry(curry(f, t), u), v);
}

/**
 * A lambda function that returns the given value.
 */
template<typename T> inline const lambda<const T()> result(const T& v) {
    return [v]()->T { return v; };
}

/**
 * Commonly used lambda types.
 */
typedef lambda<const bool()> blambda;

}
#endif /* tuscany_function_hpp */
