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

#ifndef tuscany_scheme_primitive_hpp
#define tuscany_scheme_primitive_hpp

/**
 * Script evaluator primitive functions.
 */

#include <apr_general.h>
#include <apr_uuid.h>
#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"

namespace tuscany {
namespace scheme {

const value primitiveSymbol("primitive");
const value quoteSymbol("'");
const value lambdaSymbol("lambda");

#ifdef _REENTRANT
__thread
#endif
ostream* displayOutStream = NULL;

#ifdef _REENTRANT
__thread
#endif
ostream* logOutStream = NULL;

const bool setupDisplay(ostream& out) {
    displayOutStream = &out;
    return true;
}

ostream& displayStream() {
    if (displayOutStream == NULL)
        return cout;
    return *displayOutStream;
}

const bool setupLog(ostream& out) {
    logOutStream = &out;
    return true;
}

ostream& logStream() {
    if (logOutStream == NULL)
        return cerr;
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
        displayStream() << endl;
        return true;
    }
    displayStream() << car(args);
    return displayProc(cdr(args));
}

const value logProc(const list<value>& args) {
    if (isNil(args)) {
        logStream() << endl;
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
    return string(buf, APR_UUID_FORMATTED_LENGTH);
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
    return mklist<value>("car")
    + "cdr"
    + "cons"
    + "list"
    + "nul"
    + "="
    + "equal?"
    + "+"
    + "-"
    + "*"
    + "/"
    + "assoc"
    + "cadr"
    + "caddr"
    + "cadddr"
    + "cddr"
    + "cdddr"
    + "display"
    + "log"
    + "uuid";
}

const list<value> primitiveProcedureObjects() {
    return mklist(primitiveProcedure(carProc))
    + primitiveProcedure(cdrProc)
    + primitiveProcedure(consProc)
    + primitiveProcedure(listProc)
    + primitiveProcedure(nulProc)
    + primitiveProcedure(equalProc)
    + primitiveProcedure(equalProc)
    + primitiveProcedure(addProc)
    + primitiveProcedure(subProc)
    + primitiveProcedure(mulProc)
    + primitiveProcedure(divProc)
    + primitiveProcedure(assocProc)
    + primitiveProcedure(cadrProc)
    + primitiveProcedure(caddrProc)
    + primitiveProcedure(cadddrProc)
    + primitiveProcedure(cddrProc)
    + primitiveProcedure(cdddrProc)
    + primitiveProcedure(displayProc)
    + primitiveProcedure(logProc)
    + primitiveProcedure(uuidProc);
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
#endif /* tuscany_scheme_primitive_hpp */
