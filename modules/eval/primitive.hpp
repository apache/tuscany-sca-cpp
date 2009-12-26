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

std::ostream* displayOutStream = &std::cout;
std::ostream* logOutStream = &std::cerr;

const bool setupDisplay(std::ostream& out) {
    displayOutStream = &out;
    return true;
}

std::ostream& displayStream() {
    return *displayOutStream;
}

const bool setupLog(std::ostream& out) {
    logOutStream = &out;
    return true;
}

std::ostream& logStream() {
    return *logOutStream;
}

const value carProc(const list<value>& args) {
    return car((list<value> )car(args));
}

const value cdrProc(const list<value>& args) {
    return cdr((list<value> )car(args));
}

const value consProc(const list<value>& args) {
    return cons(car(args), (list<value> )cadr(args));
}

const value listProc(const list<value>& args) {
    return args;
}

const value assocProc(const list<value>& args) {
    return assoc(car(args), (list<list<value> >)cadr(args));
}

const value nulProc(const list<value>& args) {
    const value v(car(args));
    if (isNil(v))
        return true;
    if (isList(v))
        return isNil(list<value>(v));
    return false;
}

const value equalProc(const list<value>& args) {
    return (bool)(car(args) == cadr(args));
}

const value addProc(const list<value>& args) {
    if (isNil(cdr(args)))
        return (double)car(args);
    return (double)car(args) + (double)cadr(args);
}

const value subProc(const list<value>& args) {
    if (isNil(cdr(args)))
        return (double)0 - (double)car(args);
    return (double)car(args) - (double)cadr(args);
}

const value mulProc(const list<value>& args) {
    return (double)car(args) * (double)cadr(args);
}

const value divProc(const list<value>& args) {
    return (double)car(args) / (double)cadr(args);
}

const value displayProc(const list<value>& args) {
    if (isNil(args)) {
        displayStream() << std::endl;
        return true;
    }
    displayStream() << car(args);
    return displayProc(cdr(args));
}

const value logProc(const list<value>& args) {
    if (isNil(args)) {
        logStream() << std::endl;
        return true;
    }
    logStream() << car(args);
    return logProc(cdr(args));
}

const value uuidProc(unused const list<value>& args) {
    apr_uuid_t uuid;
    apr_uuid_get(&uuid);
    char buf[APR_UUID_FORMATTED_LENGTH];
    apr_uuid_format(buf, &uuid);
    return std::string(buf, APR_UUID_FORMATTED_LENGTH);
}

const value cadrProc(unused const list<value>& args) {
    return cadr((list<value> )car(args));
}

const value caddrProc(unused const list<value>& args) {
    return caddr((list<value> )car(args));
}

const value cadddrProc(unused const list<value>& args) {
    return cadddr((list<value> )car(args));
}

const value cddrProc(unused const list<value>& args) {
    return cddr((list<value> )car(args));
}

const value cdddrProc(unused const list<value>& args) {
    return cdddr((list<value> )car(args));
}

const value applyPrimitiveProcedure(const value& proc, list<value>& args) {
    const lambda<value(const list<value>&)> func(cadr((list<value>)proc));
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
    if(isLambda(exp))
        return true;
    return false;
}

const value primitiveImplementation(const list<value>& proc) {
    return car(cdr(proc));
}

template<typename F> const value primitiveProcedure(const F& f) {
    return mklist<value>(primitiveSymbol, (lambda<value(const list<value>&)>)f);
}

const list<value> primitiveProcedureNames() {
    list<value> l = mklist<value>("car");
    l = cons<value>("cdr", l);
    l = cons<value>("cons", l);
    l = cons<value>("list", l);
    l = cons<value>("nul", l);
    l = cons<value>("=", l);
    l = cons<value>("equal?", l);
    l = cons<value>("+", l);
    l = cons<value>("-", l);
    l = cons<value>("*", l);
    l = cons<value>("/", l);
    l = cons<value>("assoc", l);
    l = cons<value>("cadr", l);
    l = cons<value>("caddr", l);
    l = cons<value>("cadddr", l);
    l = cons<value>("cddr", l);
    l = cons<value>("cdddr", l);
    l = cons<value>("display", l);
    l = cons<value>("log", l);
    l = cons<value>("uuid", l);
    return l;
}

const list<value> primitiveProcedureObjects() {
    list<value> l = mklist(primitiveProcedure(carProc));
    l = cons(primitiveProcedure(cdrProc), l);
    l = cons(primitiveProcedure(consProc), l);
    l = cons(primitiveProcedure(listProc), l);
    l = cons(primitiveProcedure(nulProc), l);
    l = cons(primitiveProcedure(equalProc), l);
    l = cons(primitiveProcedure(equalProc), l);
    l = cons(primitiveProcedure(addProc), l);
    l = cons(primitiveProcedure(subProc), l);
    l = cons(primitiveProcedure(mulProc), l);
    l = cons(primitiveProcedure(divProc), l);
    l = cons(primitiveProcedure(assocProc), l);
    l = cons(primitiveProcedure(cadrProc), l);
    l = cons(primitiveProcedure(caddrProc), l);
    l = cons(primitiveProcedure(cadddrProc), l);
    l = cons(primitiveProcedure(cddrProc), l);
    l = cons(primitiveProcedure(cdddrProc), l);
    l = cons(primitiveProcedure(displayProc), l);
    l = cons(primitiveProcedure(logProc), l);
    l = cons(primitiveProcedure(uuidProc), l);
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
