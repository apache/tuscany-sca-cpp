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
 * Lambda function type.
 */

#include <iostream>
#include "gc.hpp"
#include "debug.hpp"

namespace tuscany {

#ifdef _DEBUG

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

bool resetLambdaCounters() {
    countLambdas = countELambdas = countCLambdas = countFLambdas = countProxies = countFProxies = countCProxies = 0;
    return true;
}

bool checkLambdaCounters() {
    return countLambdas == 0;
}

bool printLambdaCounters() {
    std::cout << "countLambdas " << countLambdas << std::endl;
    std::cout << "countELambdas " << countELambdas << std::endl;
    std::cout << "countFLambdas " << countFLambdas << std::endl;
    std::cout << "countCLambdas " << countCLambdas << std::endl;
    std::cout << "countProxies " << countProxies << std::endl;
    std::cout << "countFProxies " << countFProxies << std::endl;
    std::cout << "countCProxies " << countCProxies << std::endl;
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
    Callable() : refCount(0) {
    }

    virtual const int size() const = 0;

    virtual const R operator()(P... p) const = 0;

    virtual ~Callable() {
    }

    template<typename F> class Proxy: public Callable {
    public:
        Proxy(const F& f) : function(f) {
            debug_inc(countProxies);
            debug_inc(countFProxies);
        }

        Proxy(const Proxy& p) : function(p.function) {
            debug_inc(countProxies);
            debug_inc(countCProxies);
        }

        ~Proxy() {
            debug_dec(countProxies);
        }

        virtual const R operator() (P... p) const {
            return function(std::forward<P>(p)...);
        }

        virtual const int size() const {
            return sizeof(function);
        }

    private:
        const F function;
    };

private:
    friend class gc_counting_ptr<Callable>;

    unsigned int refCount;

    unsigned int acquire() {
        return gc_add_and_fetch(refCount, (unsigned int)1);
    }

    unsigned int release() {
        return gc_sub_and_fetch(refCount, (unsigned int)1);
    }
};

template<typename S> class lambda;

template<typename R, typename... P> class lambda<R(P...)> {
public:
    lambda() : callable(0) {
        debug_inc(countLambdas);
        debug_inc(countELambdas);
    }

    template<typename F> lambda(const F f) {
        debug_inc(countLambdas);
        debug_inc(countFLambdas);

        typedef typename CallableType::template Proxy<F> ProxyType;
        callable = gc_counting_ptr<CallableType>(new ProxyType(f));
    }

    lambda(const lambda& l) {
        debug_inc(countLambdas);
        debug_inc(countCLambdas);
        callable = l.callable;
    }

    const lambda& operator=(const lambda& l) {
        if (this == &l)
            return *this;
        callable = l.callable;
        return *this;
    }

    ~lambda() {
        debug_dec(countLambdas);
    }

    const bool operator==(const lambda& l) const {
        if (this == &l)
            return true;
        return callable == l.callable;
    }

    const bool operator!=(const lambda& l) const {
        return !this->operator==(l);
    }

    const R operator()(P... p) const {
        return (*callable)(std::forward<P>(p)...);
    }

    template<typename S> friend std::ostream& operator<<(std::ostream&, const lambda<S>&);
    template<typename S> friend const bool isNil(const lambda<S>& l);

private:
    typedef Callable<R,P...> CallableType;
    gc_counting_ptr<CallableType> callable;
};

template<typename S> std::ostream& operator<<(std::ostream& out, const lambda<S>& l) {
    return out << "lambda::" << l.callable;
}

/**
 * Return true if a lambda is nil.
 */
template<typename S> const bool isNil(const lambda<S>& l) {
    return ((void*)l.callable) == 0;
}

/**
 * Curry a lambda function.
 */
template<typename R, typename T, typename... P> class curried {
public:
    curried(const lambda<R(T, P...)>& f, const T& v): v(v), f(f) {
    }

    const R operator()(P... p) const {
        return f(v, std::forward<P>(p)...);
    }

private:
    const T v;
    const lambda<R(T, P...)>f;
};

template<typename R, typename T, typename... P> const lambda<R(P...)> curry(const lambda<R(T, P...)>& f, const T& t) {
    return curried<R, T, P...>(f, t);
}

template<typename R, typename T, typename U, typename... P> const lambda<R(P...)> curry(const lambda<R(T, U, P...)>& f, const T& t, const U& u) {
    return curry(curry(f, t), u);
}

template<typename R, typename T, typename U, typename V, typename... P> const lambda<R(P...)> curry(const lambda<R(T, U, P...)>& f, const T& t, const U& u, const V& v) {
    return curry(curry(curry(f, t), u), v);
}

/**
 * A lambda function that returns the given value.
 */
template<typename T> class returnResult {
public:
    returnResult(const T& v) :
        v(v) {
    }
    const T operator()() const {
        return v;
    }
private:
    const T v;
};

template<typename T> const lambda<T()> result(const T& v) {
    return returnResult<T> (v);
}

}
#endif /* tuscany_function_hpp */
