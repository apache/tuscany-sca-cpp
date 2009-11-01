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

#include <apr_general.h>
#include <apr_uuid.h>
#include <iostream>
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"

namespace tuscany {
namespace eval {

const value primitiveSymbol("primitive");
const value quoteSymbol("'");
const value lambdaSymbol("lambda");

std::ostream* displayOut = &std::cout;

const bool setupDisplay(std::ostream& out) {
    displayOut = &out;
    return true;
}

const value carProc(list<value>& args) {
    return car((list<value> )car(args));
}

const value cdrProc(list<value>& args) {
    return cdr((list<value> )car(args));
}

const value consProc(list<value>& args) {
    return cons(car(args), (list<value> )cadr(args));
}

const value listProc(list<value>& args) {
    return args;
}

const value nulProc(list<value>& args) {
    return (bool)isNil(car(args));
}

const value equalProc(list<value>& args) {
    return (bool)(car(args) == cadr(args));
}

const value addProc(list<value>& args) {
    if (isNil(cdr(args)))
        return (double)car(args);
    return (double)car(args) + (double)cadr(args);
}

const value subProc(list<value>& args) {
    if (isNil(cdr(args)))
        return (double)0 - (double)car(args);
    return (double)car(args) - (double)cadr(args);
}

const value mulProc(list<value>& args) {
    return (double)car(args) * (double)cadr(args);
}

const value divProc(list<value>& args) {
    return (double)car(args) / (double)cadr(args);
}

const value displayProc(list<value>& args) {
    *displayOut << car(args);
    return true;
}

const value commentProc(list<value>& args) {
    return true;
}

const value uuidProc(list<value>& args) {
    apr_uuid_t uuid;
    apr_uuid_get(&uuid);
    char buf[APR_UUID_FORMATTED_LENGTH];
    apr_uuid_format(buf, &uuid);
    return std::string(buf, APR_UUID_FORMATTED_LENGTH);
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
    if(isBool(exp))
        return true;
    if(isChar(exp))
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
    l = cons<value>("uuid", l);
    l = cons<value>(";", l);
    return l;
}

const list<value> primitiveProcedureObjects() {
    list<value> l = mklist(primitiveProcedure(carProc));
    l = cons(primitiveProcedure(cdrProc), l);
    l = cons(primitiveProcedure(consProc), l);
    l = cons(primitiveProcedure(listProc), l);
    l = cons(primitiveProcedure(nulProc), l);
    l = cons(primitiveProcedure(equalProc), l);
    l = cons(primitiveProcedure(addProc), l);
    l = cons(primitiveProcedure(subProc), l);
    l = cons(primitiveProcedure(mulProc), l);
    l = cons(primitiveProcedure(divProc), l);
    l = cons(primitiveProcedure(equalProc), l);
    l = cons(primitiveProcedure(displayProc), l);
    l = cons(primitiveProcedure(uuidProc), l);
    l = cons(primitiveProcedure(commentProc), l);
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
}
#endif /* tuscany_eval_primitive_hpp */
