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
#include <iostream>
#include <sstream>
#include "gc.hpp"
#include "function.hpp"
#include "list.hpp"

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
    std::cout << "countValues " << countValues << std::endl;
    std::cout << "countEValues " << countEValues << std::endl;
    std::cout << "countCValues " << countCValues << std::endl;
    std::cout << "countVValues " << countVValues << std::endl;
    return true;
}

class value;

class value {
public:

    enum ValueType {
        Undefined, Symbol, String, List, Number, Boolean, Character, Lambda
    };

    value() :
        type(value::Undefined) {
        countValues++;
        countEValues++;
    }

    value(const value& v) {
        countValues++;
        countCValues++;
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
        case value::Boolean:
            boo() = v.boo();
        case value::Character:
            chr() = v.chr();
        default:
            break;
        }
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
        case value::Boolean:
            boo() = v.boo();
        case value::Character:
            chr() = v.chr();
        default:
            break;
        }
        return *this;
    }

    virtual ~value() {
        countValues--;
    }

    value(const lambda<value(list<value>&)>& func) :
        type(value::Lambda), data(vdata(func)) {
        countValues++;
        countVValues++;
    }

    value(const std::string& str) :
        type(value::String), data(vdata(result(str))) {
        countValues++;
        countVValues++;
    }

    value(const char* str) :
        type(value::Symbol), data(vdata(result(std::string(str)))) {
        countValues++;
        countVValues++;
    }

    value(const list<value>& lst) :
        type(value::List), data(vdata(result(lst))) {
        countValues++;
        countVValues++;
    }

    value(const list<list<value> >& l) :
        type(value::List), data(vdata(result(listOfValues(l)))) {
        countValues++;
        countVValues++;
    }

    value(const double num) :
        type(value::Number), data(vdata(result(num))) {
        countValues++;
        countVValues++;
    }

    value(const int num) :
        type(value::Number), data(vdata(result((double)num))) {
        countValues++;
        countVValues++;
    }

    value(const bool boo) :
        type(value::Boolean), data(vdata(result(boo))) {
        countValues++;
        countVValues++;
    }

    value(const char chr) :
        type(value::Character), data(vdata(result(chr))) {
        countValues++;
        countVValues++;
    }

    const bool operator!=(const value& v) const {
        return !this->operator==(v);
    }

    const bool operator==(const value& v) const {
        if(this == &v)
            return true;
        if(type != v.type)
            return false;
        switch(type) {
        case value::Undefined:
            return true;
        case value::List:
            return lst()() == v.lst()();
        case value::Lambda:
            return func() == v.func();
        case value::Symbol:
            return str()() == v.str()();
        case value::String:
            return str()() == v.str()();
        case value::Number:
            return num()() == v.num()();
        case value::Boolean:
            return boo()() == v.boo()();
        case value::Character:
            return chr()() == v.chr()();
        default:
            return false;
        }
    }

    const value operator()(list<value>& args) const {
        return func()(args);
    }

    operator const std::string() const {
        switch(type) {
        case value::List:
        case value::Lambda:
            return "";
        case value::Symbol:
        case value::String:
            return str()();
        case value::Number: {
            std::ostringstream sos;
            sos << num()();
            return sos.str();
        }
        case value::Boolean: {
            if(boo()())
                return "true";
            else
                return "false";
        }
        case value::Character: {
            std::ostringstream sos;
            sos << chr()();
            return sos.str();
        }
        default:
            return "";
        }
    }

    operator const double() const {
        return num()();
    }

    operator const int() const {
        return num()();
    }

    operator const bool() const {
        return boo()();
    }

    operator const char() const {
        return chr()();
    }

    operator const list<value>() const {
        return lst()();
    }

    operator const list<list<value> >() const {
        return listOfListOfValues(lst()());
    }

    operator const lambda<value(list<value>&)>() const {
        return func();
    }

    friend std::ostream& operator<<(std::ostream&, const value&);

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

    lambda<std::string()>& str() const {
        return vdata<std::string()> ();
    }

    lambda<list<value>()>& lst() const {
        return vdata<list<value>()> ();
    }

    lambda<value(list<value>&)>& func() const {
        return vdata<value(list<value>&)> ();
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

};


std::ostream& operator<<(std::ostream& out, const value& v) {
    switch(v.type) {
    case value::List:
        return out << v.lst()();
    case value::Lambda:
        return out << "lambda::" << v.func();
    case value::Symbol:
        return out << "symbol::" << v.str()();
    case value::String:
        return out << "string::" << '\"' << v.str()() << '\"';
    case value::Number:
        return out << "number::" << v.num()();
    case value::Boolean:
        if(v.boo()())
            return out << "bool::" << "true";
        else
            return out << "bool::" << "false";
    case value::Character:
        return out << "char::" << v.chr()();
    default:
        return out << "undefined";
    }
}

const value::ValueType type(const value& v) {
    return v.type;
}

const bool isNil(const value& value) {
    return value.type == value::Undefined;
}

const bool isString(const value& value) {
    return value.type == value::String;
}

const bool isSymbol(const value& value) {
    return value.type == value::Symbol;
}

const bool isList(const value& value) {
    return value.type == value::List;
}

const bool isNumber(const value& value) {
    return value.type == value::Number;
}

const bool isBoolean(const value& value) {
    return value.type == value::Boolean;
}

const bool isCharacter(const value& value) {
    return value.type == value::Character;
}

const bool isTaggedList(const value& exp, value tag) {
    if(isList(exp) && !isNil((list<value>)exp))
        return car((list<value>)exp) == tag;
    return false;
}

}
#endif /* tuscany_value_hpp */
