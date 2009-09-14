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

#include <string>
#include <ostream>
#include "gc.hpp"
#include "function.hpp"
#include "list.hpp"

using std::string;
using std::ostream;
using std::cout;
using std::endl;

namespace tuscany
{

long int countValues = 0;
long int countEValues = 0;
long int countCValues = 0;
long int countVValues = 0;

bool resetValueCounters() {
    countValues = countEValues = countCValues = countVValues = 0;
    return true;
}

bool printValueCounters() {
    using std::cout;
    using std::endl;
    cout << "countValues " << countValues << endl;
    cout << "countEValues " << countEValues << endl;
    cout << "countCValues " << countCValues << endl;
    cout << "countVValues " << countVValues << endl;
    return true;
}

class Value;

class Value {
public:

    enum ValueType {
        Undefined, Symbol, String, List, Number, Boolean, Character, Lambda
    };

    Value() :
        type(Value::Undefined) {
        countValues++;
        countEValues++;
    }

    Value(const Value& v) {
        countValues++;
        countCValues++;
        type = v.type;
        switch(type) {
        case Value::List:
            lst() = v.lst();
        case Value::Lambda:
            func() = v.func();
        case Value::Symbol:
            str() = v.str();
        case Value::String:
            str() = v.str();
        case Value::Number:
            num() = v.num();
        case Value::Boolean:
            boo() = v.boo();
        case Value::Character:
            chr() = v.chr();
        default:
            break;
        }
    }

    const Value& operator=(const Value& v) {
        if(this == &v)
            return *this;
        type = v.type;
        switch(type) {
        case Value::List:
            lst() = v.lst();
        case Value::Lambda:
            func() = v.func();
        case Value::Symbol:
            str() = v.str();
        case Value::String:
            str() = v.str();
        case Value::Number:
            num() = v.num();
        case Value::Boolean:
            boo() = v.boo();
        case Value::Character:
            chr() = v.chr();
        default:
            break;
        }
        return *this;
    }

    virtual ~Value() {
        countValues--;
    }

    Value(const lambda<Value(list<Value>&)>& func) :
        type(Value::Lambda), data(vdata(func)) {
        countValues++;
        countVValues++;
    }

    Value(const string& str) :
        type(Value::String), data(vdata(unit(str))) {
        countValues++;
        countVValues++;
    }

    Value(const char* str) :
        type(Value::Symbol), data(vdata(unit(string(str)))) {
        countValues++;
        countVValues++;
    }

    Value(const list<Value>& lst) :
        type(Value::List), data(vdata(unit(lst))) {
        countValues++;
        countVValues++;
    }

    Value(const double num) :
        type(Value::Number), data(vdata(unit(num))) {
        countValues++;
        countVValues++;
    }

    Value(const int num) :
        type(Value::Number), data(vdata(unit((double)num))) {
        countValues++;
        countVValues++;
    }

    Value(const bool boo) :
        type(Value::Boolean), data(vdata(unit(boo))) {
        countValues++;
        countVValues++;
    }

    Value(const char chr) :
        type(Value::Character), data(vdata(unit(chr))) {
        countValues++;
        countVValues++;
    }

    const bool operator!=(const Value& v) const {
        return !this->operator==(v);
    }

    const bool operator==(const Value& v) const {
        if(this == &v)
            return true;
        if(type != v.type)
            return false;
        switch(type) {
        case Value::Undefined:
            return true;
        case Value::List:
            return lst()() == v.lst()();
        case Value::Lambda:
            return func() == v.func();
        case Value::Symbol:
            return str()() == v.str()();
        case Value::String:
            return str()() == v.str()();
        case Value::Number:
            return num()() == v.num()();
        case Value::Boolean:
            return boo()() == v.boo()();
        case Value::Character:
            return chr()() == v.chr()();
        default:
            return false;
        }
    }

    const Value operator()(list<Value>& args) const {
        return func()(args);
    }

    operator string() const {
        return str()();
    }

    operator double() const {
        return num()();
    }

    operator int() const {
        return num()();
    }

    operator bool() const {
        return boo()();
    }

    operator char() const {
        return chr()();
    }

    operator list<Value>() const {
        return lst()();
    }

    operator lambda<Value(list<Value>&)>() const {
        return func();
    }

    friend ostream& operator<<(ostream&, const Value&);

    ValueType type;
    lambda<char()> data;

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

    lambda<char()>& chr() const {
        return vdata<char()> ();
    }

    lambda<string()>& str() const {
        return vdata<string()> ();
    }

    lambda<list<Value>()>& lst() const {
        return vdata<list<Value>()> ();
    }

    lambda<Value(list<Value>&)>& func() const {
        return vdata<Value(list<Value>&)> ();
    }

};

ostream& operator<<(ostream& out, const Value& v) {
    switch(v.type) {
    case Value::List:
        return out << "List::" << v.lst()();
    case Value::Lambda:
        return out << "Lambda::" << v.func();
    case Value::Symbol:
        return out << "Symbol::" << v.str()();
    case Value::String:
        return out << "String::" << '\'' << v.str()() << '\'';
    case Value::Number:
        return out << "Number::" << v.num()();
    case Value::Boolean:
        if(v.boo()())
            return out << "Boolean::" << "true";
        else
            return out << "Boolean::" << "false";
    case Value::Character:
        return out << "Character::" << v.chr()();
    default:
        return out << "Undefined";
    }
}

const bool isNull(const Value& value) {
    return value.type == Value::Undefined;
}

const bool isString(const Value& value) {
    return value.type == Value::String;
}

const bool isSymbol(const Value& value) {
    return value.type == Value::Symbol;
}

const bool isList(const Value& value) {
    return value.type == Value::List;
}

const bool isNumber(const Value& value) {
    return value.type == Value::Number;
}

const bool isBoolean(const Value& value) {
    return value.type == Value::Boolean;
}

const bool isCharacter(const Value& value) {
    return value.type == Value::Character;
}

const bool isTaggedList(const Value& exp, Value tag) {
    if(isList(exp))
        return car((list<Value> )exp) == tag;
    return false;
}

}
#endif /* tuscany_value_hpp */
