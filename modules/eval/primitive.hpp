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

#ifndef tuscany_eval_primitive_hpp
#define tuscany_eval_primitive_hpp

/**
 * Script evaluator primitive functions.
 */

#include <iostream>
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"

namespace tuscany
{

const value primitiveSymbol("primitive");
const value quoteSymbol("'");
const value lambdaSymbol("lambda");

std::ostream* evalOut = &std::cout;

const bool setupEvalOut(std::ostream& out) {
    evalOut = &out;
    return true;
}

const value valueCar(list<value>& args) {
    return car((list<value> )car(args));
}

const value valueCdr(list<value>& args) {
    return cdr((list<value> )car(args));
}

const value valueCons(list<value>& args) {
    return cons(car(args), (list<value> )cadr(args));
}

const value valueList(list<value>& args) {
    return args;
}

const value valueNul(list<value>& args) {
    return (bool)isNil(car(args));
}

const value valueEqual(list<value>& args) {
    return (bool)(car(args) == cadr(args));
}

const value valueAdd(list<value>& args) {
    if (isNil(cdr(args)))
        return (double)car(args);
    return (double)car(args) + (double)cadr(args);
}

const value valueSub(list<value>& args) {
    if (isNil(cdr(args)))
        return (double)0 - (double)car(args);
    return (double)car(args) - (double)cadr(args);
}

const value valueMul(list<value>& args) {
    return (double)car(args) * (double)cadr(args);
}

const value valueDiv(list<value>& args) {
    return (double)car(args) / (double)cadr(args);
}

const value valueDisplay(list<value>& args) {
    *evalOut << car(args);
    return true;
}

const value valueComment(list<value>& args) {
    *evalOut << "; " << car(args);
    return true;
}

const value valueError(list<value>& args) {
    std::cerr << (std::string)car(args);
    return true;
}

const value applyPrimitiveProcedure(const value& proc, list<value>& args) {
    const lambda<value(list<value>&)> func(cadr((list<value>)proc));
    return func(args);
}

const bool isPrimitiveProcedure(const value& proc) {
    return isTaggedList(proc, primitiveSymbol);
}

const bool isSelfEvaluating(const value& exp) {
    if(isNil(exp))
        return true;
    if(isNumber(exp))
        return true;
    if(isString(exp))
        return true;
    if(isBoolean(exp))
        return true;
    if(isCharacter(exp))
        return true;
    return false;
}

const value primitiveImplementation(const list<value>& proc) {
    return car(cdr(proc));
}

template<typename F> const value primitiveProcedure(const F& f) {
    return mklist<value>(primitiveSymbol, (lambda<value(list<value>&)>)f);
}

const list<value> primitiveProcedureNames() {
    list<value> l = mklist<value>("car");
    l = cons<value>("cdr", l);
    l = cons<value>("cons", l);
    l = cons<value>("list", l);
    l = cons<value>("nul", l);
    l = cons<value>("=", l);
    l = cons<value>("+", l);
    l = cons<value>("-", l);
    l = cons<value>("*", l);
    l = cons<value>("/", l);
    l = cons<value>("equal?", l);
    l = cons<value>("display", l);
    l = cons<value>(";", l);
    return l;
}

const list<value> primitiveProcedureObjects() {
    list<value> l = mklist(primitiveProcedure(valueCar));
    l = cons(primitiveProcedure(valueCdr), l);
    l = cons(primitiveProcedure(valueCons), l);
    l = cons(primitiveProcedure(valueList), l);
    l = cons(primitiveProcedure(valueNul), l);
    l = cons(primitiveProcedure(valueEqual), l);
    l = cons(primitiveProcedure(valueAdd), l);
    l = cons(primitiveProcedure(valueSub), l);
    l = cons(primitiveProcedure(valueMul), l);
    l = cons(primitiveProcedure(valueDiv), l);
    l = cons(primitiveProcedure(valueEqual), l);
    l = cons(primitiveProcedure(valueDisplay), l);
    l = cons(primitiveProcedure(valueComment), l);
    return l;
}

const bool isFalse(const value& exp) {
    return (bool)exp == false;
}

const bool isTrue(const value& exp) {
    return (bool)exp == true;
}

const bool isQuoted(const value& exp) {
    return isTaggedList(exp, quoteSymbol);
}

const value textOfQuotation(const value& exp) {
    return car(cdr((list<value> )exp));
}

const value makeLambda(const list<value>& parameters, const list<value>& body) {
    return cons(lambdaSymbol, cons<value>(parameters, body));
}

}
#endif /* tuscany_eval_primitive_hpp */
