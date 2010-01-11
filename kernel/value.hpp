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

#ifndef tuscany_value_hpp
#define tuscany_value_hpp

/**
 * Generic value type.
 */

#include <stdlib.h>
#include "string.hpp"
#include "sstream.hpp"
#include "gc.hpp"
#include "function.hpp"
#include "list.hpp"
#include "monad.hpp"

namespace tuscany
{

#ifdef WANT_MAINTAINER_MODE

/**
 * Debug utilities. Counters used to track instances of values, and
 * macro used to write the contents of a value in a string, easier to
 * watch in a debugger than the value itself.
 */
long int countValues = 0;
long int countEValues = 0;
long int countCValues = 0;
long int countVValues = 0;

bool resetValueCounters() {
    countValues = countEValues = countCValues = countVValues = 0;
    return true;
}

bool checkValueCounters() {
    return countValues == 0;
}

bool printValueCounters() {
    cout << "countValues " << countValues << endl;
    cout << "countEValues " << countEValues << endl;
    cout << "countCValues " << countCValues << endl;
    cout << "countVValues " << countVValues << endl;
    return true;
}

#else

#define resetValueCounters()
#define checkValueCounters() true
#define printValueCounters()

#endif

#ifdef WANT_MAINTAINER_WATCH

#define debug_watchValue() do { \
        this->watch = watchValue(*this); \
    } while (0)

#else

#define debug_watchValue()

#endif

class value;

class value {
public:

    enum ValueType {
        Undefined, Symbol, String, List, Number, Bool, Lambda, Ptr
    };

    value() : type(value::Undefined) {
        debug_inc(countValues);
        debug_inc(countEValues);
        debug_watchValue();
    }

    value(const value& v) {
        debug_inc(countValues);
        debug_inc(countCValues);
        type = v.type;
        switch(type) {
        case value::List:
            lst() = v.lst();
        case value::Lambda:
            func() = v.func();
        case value::Symbol:
            str() = v.str();
        case value::String:
            str() = v.str();
        case value::Number:
            num() = v.num();
        case value::Bool:
            boo() = v.boo();
        case value::Ptr:
            ptr() = v.ptr();
        default:
            break;
        }
#ifdef WANT_MAINTAINER_WATCH
        watch = v.watch;
#endif
    }

    virtual ~value() {
        debug_dec(countValues);
    }

    value(const lambda<value(const list<value>&)>& func) : type(value::Lambda), data(vdata(func)) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const string& str) : type(value::String), data(vdata(result(str))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const char* str) : type(value::Symbol), data(vdata(result(string(str)))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const list<value>& lst) : type(value::List), data(vdata(result(lst))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const list<list<value> >& l) : type(value::List), data(vdata(result(listOfValues(l)))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const double num) : type(value::Number), data(vdata(result(num))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const int num) : type(value::Number), data(vdata(result((double)num))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const bool boo) : type(value::Bool), data(vdata(result(boo))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const gc_ptr<value> ptr) : type(value::Ptr), data(vdata(result(ptr))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const failable<value>& m) : type(value::List),
        data(vdata(result(hasContent(m)? mklist<value>(content(m)) : mklist<value>(value(), reason(m))))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value(const maybe<value>& m) : type(value::List),
        data(vdata(result(hasContent(m)? mklist<value>(content(m)) : list<value>()))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    const value& operator=(const value& v) {
        if(this == &v)
            return *this;
        type = v.type;
        switch(type) {
        case value::List:
            lst() = v.lst();
        case value::Lambda:
            func() = v.func();
        case value::Symbol:
            str() = v.str();
        case value::String:
            str() = v.str();
        case value::Number:
            num() = v.num();
        case value::Bool:
            boo() = v.boo();
        case value::Ptr:
            ptr() = v.ptr();
        default:
            break;
        }
#ifdef WANT_MAINTAINER_WATCH
        watch = v.watch;
#endif
        return *this;
    }

    const bool operator!=(const value& v) const {
        return !this->operator==(v);
    }

    const bool operator==(const value& v) const {
        if(this == &v)
            return true;
        switch(type) {
        case value::Undefined:
            return true;
        case value::List:
            return v.type == value::List && lst()() == v.lst()();
        case value::Lambda:
            return v.type == value::Lambda && func() == v.func();
        case value::Symbol:
        case value::String:
            return str()() == (string)v;
        case value::Number:
            return num()() == (double)v;
        case value::Bool:
            return boo()() == (bool)v;
        case value::Ptr:
            return v.type == value::Ptr && ptr()() == v.ptr()();
        default:
            return false;
        }
    }

    const bool operator<(const value& v) const {
        if(this == &v)
            return false;
        switch(type) {
        case value::List:
            return v.type == value::List && lst()() < v.lst()();
        case value::Symbol:
        case value::String:
            return str()() < (string)v;
        case value::Bool:
            return boo()() < (bool)v;
        case value::Number:
            return num()() < (double)v;
        default:
            return false;
        }
    }

    const bool operator>(const value& v) const {
        if(this == &v)
            return false;
        switch(type) {
        case value::List:
            return v.type == value::List && lst()() > v.lst()();
        case value::Symbol:
        case value::String:
            return str()() > (string)v;
        case value::Bool:
            return boo()() > (bool)v;
        case value::Number:
            return num()() > (double)v;
        default:
            return false;
        }
    }

    const value operator()(const list<value>& args) const {
        return func()(args);
    }

    operator const string() const {
        switch(type) {
        case value::Symbol:
        case value::String:
            return str()();
        case value::Number: {
            ostringstream os;
            os << num()();
            return tuscany::str(os);
        }
        case value::Bool:
            return boo()()? trueString : falseString;
        default:
            return emptyString;
        }
    }

    operator const double() const {
        switch(type) {
        case value::Symbol:
        case value::String:
            return atof(c_str(str()()));
        case value::Number:
            return (double)num()();
        case value::Bool:
            return boo()()? 1.0 : 0.0;
        default:
            return 0.0;
        }
    }

    operator const int() const {
        switch(type) {
        case value::Symbol:
        case value::String:
            return atoi(c_str(str()()));
        case value::Number:
            return (int)num()();
        case value::Bool:
            return boo()()? 1 : 0;
        default:
            return 0;
        }
    }

    operator const bool() const {
        switch(type) {
        case value::Symbol:
        case value::String:
            return str()() == string("true");
        case value::Number:
            return (int)num()() != 0;
        case value::Bool:
            return boo()();
        default:
            return 0;
        }
    }

    operator const gc_ptr<value>() const {
        return ptr()();
    }

    operator const list<value>() const {
        return lst()();
    }

    operator const list<list<value> >() const {
        return listOfListOfValues(lst()());
    }

    operator const lambda<value(const list<value>&)>() const {
        return func();
    }

private:
     template<typename T> lambda<T>& vdata() const {
        return *reinterpret_cast<lambda<T> *> (const_cast<lambda<char()> *> (&data));
    }

    template<typename T> const lambda<char()>& vdata(const T& v) const {
        return *reinterpret_cast<const lambda<char()> *> (&v);
    }

    lambda<double()>& num() const {
        return vdata<double()> ();
    }

    lambda<bool()>& boo() const {
        return vdata<bool()> ();
    }

    lambda<gc_ptr<value>()>& ptr() const {
        return vdata<gc_ptr<value>()> ();
    }

    lambda<string()>& str() const {
        return vdata<string()> ();
    }

    lambda<list<value>()>& lst() const {
        return vdata<list<value>()> ();
    }

    lambda<value(const list<value>&)>& func() const {
        return vdata<value(const list<value>&)> ();
    }

    const list<value> listOfValues(const list<list<value> >& l) const {
        if (isNil(l))
            return list<value>();
        return cons<value>(car(l), listOfValues(cdr(l)));
    }

    const list<list<value> > listOfListOfValues(const list<value>& l) const {
        if (isNil(l))
            return list<list<value> >();
        return cons<list<value> >(list<value>(car(l)), listOfListOfValues(cdr(l)));
    }

    friend ostream& operator<<(ostream&, const value&);
    friend const value::ValueType type(const value& v);

#ifdef WANT_MAINTAINER_WATCH
    friend const string watchValue(const value& v);
    string watch;
#endif

    ValueType type;
     lambda<char()> data;
};

#ifdef WANT_MAINTAINER_WATCH

/**
 * Debug utility used to write the contents of a value to a string, easier
 * to watch than the value itself in a debugger.
 */
const string watchValue(const value& v) {
    if (v.type == value::List)
        return watchList<value>(v);
    odebugstream os;
    os << v;
    return str(os);
}

#endif

/**
 * Write a value to a stream.
 */
ostream& operator<<(ostream& out, const value& v) {
    switch(v.type) {
    case value::List:
        return out << v.lst()();
    case value::Lambda:
        return out << "lambda::" << v.func();
    case value::Symbol:
        return out << v.str()();
    case value::String:
        return out << '\"' << v.str()() << '\"';
    case value::Number:
        return out << v.num()();
    case value::Bool:
        if(v.boo()())
            return out << "true";
        else
            return out << "false";
    case value::Ptr: {
        const gc_ptr<value> p =  v.ptr()();
        if (p == gc_ptr<value>(NULL))
            return out << "gc_ptr::null";
        return out << "gc_ptr::" << p;
    }
    default:
        return out << "undefined";
    }
}

/**
 * Returns the type of a value.
 */
const value::ValueType type(const value& v) {
    return v.type;
}

/**
 * Returns true if a value is nil.
 */
const bool isNil(const value& v) {
    return type(v) == value::Undefined;
}

/**
 * Returns true if a value is a lambda.
 */
const bool isLambda(const value& v) {
    return type(v) == value::Lambda;
}

/**
 * Returns true if a value is a string.
 */
const bool isString(const value& v) {
    return type(v) == value::String;
}

/**
 * Returns true if a value is a symbol.
 */
const bool isSymbol(const value& v) {
    return type(v) == value::Symbol;
}

/**
 * Returns true if a value is a list.
 */
const bool isList(const value& v) {
    return type(v) == value::List;
}

/**
 * Returns true if a value is a number.
 */
const bool isNumber(const value& v) {
    return type(v) == value::Number;
}

/**
 * Returns true if a value is a boolean.
 */
const bool isBool(const value& v) {
    return type(v) == value::Bool;
}

/**
 * Returns true if a value is a pointer.
 */
const bool isPtr(const value& v) {
    return type(v) == value::Ptr;
}

/**
 * Returns true if a value is a tagged list.
 */
const bool isTaggedList(const value& exp, value tag) {
    if(isList(exp) && !isNil((list<value>)exp))
        return car((list<value>)exp) == tag;
    return false;
}

/**
 * Make a list of values from a list of other things.
 */
template<typename T> const list<value> mkvalues(const list<T>& l) {
    if (isNil(l))
        return list<value>();
    return cons<value>(car(l), mkvalues(cdr(l)));
}

}
#endif /* tuscany_value_hpp */
