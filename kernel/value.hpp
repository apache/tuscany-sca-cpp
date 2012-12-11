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
#include <apr_uuid.h>
#include <apr_time.h>

#include "string.hpp"
#include "sstream.hpp"
#include "gc.hpp"
#include "function.hpp"
#include "list.hpp"
#include "monad.hpp"

namespace tuscany
{

#ifdef WANT_MAINTAINER_COUNTERS

/**
 * Debug utilities. Counters used to track instances of values
 */
long int countValues = 0;
long int countEValues = 0;
long int countCValues = 0;
long int countVValues = 0;

inline const bool resetValueCounters() {
    countValues = countEValues = countCValues = countVValues = 0;
    return true;
}

inline const bool checkValueCounters() {
    return countValues == 0;
}

inline const bool printValueCounters() {
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

/**
 * Debug utilities. Macro used to write the contents of a value to
 * a string, easier to watch in a debugger than the value itself.
 */

#define debug_watchValue() do { \
        this->watch = watchValue(*this); \
    } while (0)

#else

#define debug_watchValue()

#endif

/**
 * Common value constants.
 */
class value;
extern const value nilValue;
extern const list<value> nilListValue;
extern const list<value> nilPairValue;
extern const value emptyStringValue;
extern const value trueValue;
extern const value falseValue;

/**
 * Common value-based lambda types.
 */
typedef lambda<const value(const list<value>&)> lvvlambda;
typedef lambda<const value(const value&)> vvlambda;
typedef lambda<const bool(const value&)> vblambda;
typedef lambda<const value()> vlambda;

/**
 * Generic value type class.
 */

class value {
public:

    enum ValueType {
        Nil, Symbol, String, List, Number, Bool, Lambda, Ptr
    };

    typedef union {
        const void* mix;
        const gc_ptr<list<value> > lst;
        const string str;
        const lvvlambda func;
        const gc_ptr<value> ptr;
        const double num;
        const bool boo;
    } ValueMix;

    inline value() noexcept : type(value::Nil) {
        debug_inc(countValues);
        debug_inc(countEValues);
        debug_watchValue();
    }

    inline value(const value& v) noexcept : type(v.type) {
        debug_inc(countValues);
        debug_inc(countCValues);
        memcpy((void*)&mix, (void*)&v.mix, sizeof(ValueMix));
#ifdef WANT_MAINTAINER_WATCH
        watch = v.watch;
#endif
    }

    inline value(const gc_mutable_ref<value>& r) noexcept : type(((value*)r)->type) {
        debug_inc(countValues);
        debug_inc(countCValues);
        memcpy((void*)&mix, (void*)&(((value*)r)->mix), sizeof(ValueMix));
#ifdef WANT_MAINTAINER_WATCH
        watch = v.watch;
#endif
    }

    inline virtual ~value() noexcept {
        debug_dec(countValues);
    }

    inline value(const lvvlambda& f) noexcept : type(value::Lambda), func(f) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const string& s) noexcept : type(value::String), str(s) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const char* s) noexcept : type(value::Symbol), str(s) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const list<value>& l) noexcept : type(value::List), lst(new (gc_new<list<value> >()) list<value>(l)) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const list<list<value> >& l) noexcept : type(value::List), lst(new (gc_new<list<value> >()) list<value>(listOfValues(l))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const double d) noexcept : type(value::Number), num(d) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const int i) noexcept : type(value::Number), num((double)i) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const bool b) noexcept : type(value::Bool), boo(b) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const gc_ptr<value> p) noexcept : type(value::Ptr), ptr(p) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const failable<value>& m) noexcept : type(value::List),
        lst(new (gc_new<list<value> >()) list<value>(hasContent(m)? mklist<value>(content(m)) :
            rcode(m) == 1? mklist<value>(nilValue, reason(m)) : mklist<value>(nilValue, reason(m), rcode(m)))) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    inline value(const maybe<value>& m) noexcept : type(value::List),
        lst(new (gc_new<list<value> >()) list<value>(hasContent(m)? mklist<value>(content(m)) : nilListValue)) {
        debug_inc(countValues);
        debug_inc(countVValues);
        debug_watchValue();
    }

    value& operator=(const value& v) = delete;

    inline const bool operator!=(const value& v) const noexcept {
        return !this->operator==(v);
    }

    inline const bool operator==(const value& v) const noexcept {
        if(this == &v)
            return true;
        switch(type) {
        case value::Nil:
            return v.type == value::Nil;
        case value::List:
            return v.type == value::List && *lst == *v.lst;
        case value::Lambda:
            return v.type == value::Lambda && func == v.func;
        case value::Symbol:
        case value::String:
            return (v.type == value::Symbol || v.type == value::String) && str == v.str;
        case value::Number:
            return v.type == value::Number && num == v.num;
        case value::Bool:
            return v.type == value::Bool && boo == v.boo;
        case value::Ptr:
            return v.type == value::Ptr && ptr == v.ptr;
        default:
            return false;
        }
    }

    inline const bool operator<(const value& v) const noexcept {
        if(this == &v)
            return false;
        switch(type) {
        case value::List:
            return v.type == value::List && *lst < *v.lst;
        case value::Symbol:
        case value::String:
            return (v.type == value::Symbol || v.type == value::String) && str < v.str;
        case value::Bool:
            return v.type == value::Bool && boo < v.boo;
        case value::Number:
            return v.type == value::Number && num < v.num;
        default:
            return false;
        }
    }

    inline const bool operator>(const value& v) const noexcept {
        if(this == &v)
            return false;
        switch(type) {
        case value::List:
            return v.type == value::List && *lst > *v.lst;
        case value::Symbol:
        case value::String:
            return (v.type == value::Symbol || v.type == value::String) && str > v.str;
        case value::Bool:
            return v.type == value::Bool && boo > v.boo;
        case value::Number:
            return v.type == value::Number && num > v.num;
        default:
            return false;
        }
    }

    inline const value operator()(const list<value>& args) const noexcept {
        return func(args);
    }

    inline operator const string() const noexcept {
        switch(type) {
        case value::Symbol:
        case value::String:
            return str;
        case value::Number: {
            ostringstream os;
            os << num;
            return tuscany::str(os);
        }
        case value::Bool:
            return boo? trueString : falseString;
        default:
            return emptyString;
        }
    }

    inline operator const double() const noexcept {
        switch(type) {
        case value::Symbol:
        case value::String:
            return atof(c_str(str));
        case value::Number:
            return (double)num;
        case value::Bool:
            return boo? 1.0 : 0.0;
        default:
            return 0.0;
        }
    }

    inline operator const int() const noexcept {
        switch(type) {
        case value::Symbol:
        case value::String:
            return atoi(c_str(str));
        case value::Number:
            return (int)num;
        case value::Bool:
            return boo? 1 : 0;
        default:
            return 0;
        }
    }

    inline operator const bool() const noexcept {
        switch(type) {
        case value::Symbol:
        case value::String:
            return str == trueString;
        case value::Number:
            return num != 0.0;
        case value::Bool:
            return boo;
        default:
            return false;
        }
    }

    inline operator const gc_ptr<value>() const noexcept {
        return ptr;
    }

    inline operator const list<value>() const noexcept {
        return *lst;
    }

    inline operator const list<list<value> >() const noexcept {
        return listOfListOfValues(*lst);
    }

    inline operator const lvvlambda() const noexcept {
        return func;
    }

private:
    inline const list<value> listOfValues(const list<list<value> >& l) const noexcept {
        if (isNil(l))
            return nilListValue;
        return cons<value>(car(l), listOfValues(cdr(l)));
    }

    inline const list<list<value> > listOfListOfValues(const list<value>& l) const noexcept {
        if (isNil(l))
            return list<list<value> >();
        return cons<list<value> >(car(l).type == value::List? list<value>(car(l)) : nilPairValue, listOfListOfValues(cdr(l)));
    }

    friend ostream& operator<<(ostream&, const value&) noexcept;
    friend const value::ValueType type(const value& v) noexcept;
    friend const bool setvalue(value& target, const value& v);

#ifdef WANT_MAINTAINER_WATCH
    friend const string watchValue(const value& v);
    string watch;
#endif

    const ValueType type;
    union {
        void* mix;
        const gc_ptr<list<value> > lst;
        const string str;
        const lvvlambda func;
        const gc_ptr<value> ptr;
        const double num;
        const bool boo;
    };
};

/**
 * Common value constants.
 */
const value nilValue;
const list<value> nilListValue = list<value>();
const list<value> nilPairValue = mklist<value>(nilValue, nilValue);
const value emptyStringValue(emptyString);
const value trueValue(true);
const value falseValue(false);

#ifdef WANT_MAINTAINER_WATCH

/**
 * Debug utility used to write the contents of a value to a string, easier
 * to watch than the value itself in a debugger.
 */
inline const string watchValue(const value& v) {
    if (v.type == value::List)
        return watchList<value>(v);
    odebugstream os;
    os << v;
    return str(os);
}

#endif

/**
 * Write an escape string to a buffer.
 */
inline const char* escapestr(const char* const s, char* const buf) noexcept {
    if (*s == '\0') {
        *buf = '\0';
        return buf;
    }
    if (*s == '\\' || *s == '"') {
        *buf = '\\';
        *(buf + 1) = *s;
        return escapestr(s + 1, buf + 2);
    }
    *buf = *s;
    return escapestr(s + 1, buf + 1);
}

/**
 * Write an escaped string value to a stream.
 */
inline ostream& escvwrite(const string& str, ostream& out) noexcept {
    char* buf = gc_cnew(length(str) * 2 + 1);
    escapestr(c_str(str), buf);
    out << buf;
    return out;
}

/**
 * Write a value to a stream.
 */
inline ostream& operator<<(ostream& out, const value& v) noexcept {
    switch(v.type) {
    case value::List:
        return out << *v.lst;
    case value::Lambda:
        return out << "lambda::" << v.func;
    case value::Symbol:
        return out << v.str;
    case value::String:
        out << '\"';
        escvwrite(v.str, out);
        return out << '\"';
    case value::Number:
        return out << v.num;
    case value::Bool:
        if(v.boo)
            return out << trueString;
        else
            return out << falseString;
    case value::Ptr: {
        if (v.ptr == gc_ptr<value>(NULL))
            return out << "gc_ptr::null";
        return out << "gc_ptr::" << v.ptr;
    }
    default:
        return out << "nil";
    }
}

/**
 * Returns the type of a value.
 */
inline const value::ValueType type(const value& v) noexcept {
    return v.type;
}

/**
 * Returns true if a value is nil.
 */
inline const bool isNil(const value& v) noexcept {
    return type(v) == value::Nil;
}

/**
 * Returns true if a value is a lambda.
 */
inline const bool isLambda(const value& v) noexcept {
    return type(v) == value::Lambda;
}

/**
 * Returns true if a value is a string.
 */
inline const bool isString(const value& v) noexcept {
    return type(v) == value::String;
}

/**
 * Returns true if a value is a symbol.
 */
inline const bool isSymbol(const value& v) noexcept {
    return type(v) == value::Symbol;
}

/**
 * Returns true if a value is a list.
 */
inline const bool isList(const value& v) noexcept {
    return type(v) == value::List;
}

/**
 * Returns true if a value is a number.
 */
inline const bool isNumber(const value& v) noexcept {
    return type(v) == value::Number;
}

/**
 * Returns true if a value is a boolean.
 */
inline const bool isBool(const value& v) noexcept {
    return type(v) == value::Bool;
}

/**
 * Returns true if a value is a pointer.
 */
inline const bool isPtr(const value& v) noexcept {
    return type(v) == value::Ptr;
}

/**
 * Returns true if a value is a tagged list.
 */
inline const bool isTaggedList(const value& exp, const value& tag) noexcept {
    if(isList(exp) && !isNil((list<value>)exp))
        return car((list<value>)exp) == tag;
    return false;
}

/**
 * Make a list of values from a list of other things.
 */
template<typename T> inline const list<value> mkvalues(const list<T>& l) noexcept {
    if (isNil(l))
        return nilListValue;
    return cons<value>(car(l), mkvalues(cdr(l)));
}

/**
 * Convert a list of values to a list of other things.
 */
template<typename T> inline const list<T> convertValues(const list<value>& l) noexcept {
    if (isNil(l))
        return list<T>();
    return cons<T>(car(l), convertValues<T>(cdr(l)));
}

/**
 * Convert a path string value to a list of values.
 */
inline const list<string> pathTokens(const char* const p) noexcept {
    if (p == NULL || p[0] == '\0')
        return list<string>();
    if (p[0] == '/')
        return tokenize("/", p + 1);
    return tokenize("/", p);
}

inline const list<value> pathValues(const value& p) noexcept {
    return mkvalues(pathTokens(c_str(p)));
}

/**
 * Convert a path represented as a list of values to a string value.
 */
inline const value path(const list<value>& p) noexcept {
    if (isNil(p))
        return emptyString;
    return string("/") + (string)car(p) + (string)path(cdr(p));
}

/**
 * Make a uuid value.
 */
inline const value mkuuid() noexcept {
    apr_uuid_t id;
    apr_uuid_get(&id);
    char buf[APR_UUID_FORMATTED_LENGTH];
    apr_uuid_format(buf, &id);
    return value(string(buf, APR_UUID_FORMATTED_LENGTH));
}

/**
 * Make a random alphanumeric value.
 */
inline const int intrand() noexcept {
    const apr_uint64_t now = apr_time_now();
    srand((unsigned int)(((now >> 32) ^ now) & 0xffffffff));
    return rand() & 0x0FFFF;
}       

inline const value mkrand() noexcept {
    char buf[32];
    const char* const an = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i =0; i < 32; i++)
        buf[i] = an[intrand() % 62];
    return value(string(buf, 32));
}

/**
 * Set a value. Use with moderation.
 */
inline const bool setvalue(value& target, const value& v) {
    if (&target == &v)
        return true;
#ifdef WANT_MAINTAINER_WATCH
    memcpy(&target.watch, &v.watch, sizeof(string));
#endif
    memcpy((void*)&target.type, (void*)&v.type, sizeof(value::ValueType));
    memcpy((void*)&target.mix, (void*)&v.mix, sizeof(value::ValueMix));
    return true;
}

}
#endif /* tuscany_value_hpp */
