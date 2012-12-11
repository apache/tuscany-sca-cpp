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

#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "parallel.hpp"

namespace tuscany {
namespace scheme {

const value primitiveSymbol("primitive");
const value quoteSymbol("'");
const value lambdaSymbol("lambda");

#ifdef WANT_THREADS
const perthread_ptr<ostream> displayOutStream;
#else
ostream* displayOutStream = NULL;
#endif

#ifdef WANT_THREADS
perthread_ptr<ostream> logOutStream;
#else
ostream* logOutStream = NULL;
#endif

inline const bool setupDisplay(ostream& out) {
    displayOutStream = &out;
    return true;
}

inline ostream& displayStream() {
    if (displayOutStream == NULL)
        return cout;
    return *displayOutStream;
}

inline const bool setupLog(ostream& out) {
    logOutStream = &out;
    return true;
}

inline ostream& logStream() {
    if (logOutStream == NULL)
        return cerr;
    return *logOutStream;
}

inline const value carProc(const list<value>& args) {
    return car((list<value> )car(args));
}

inline const value cdrProc(const list<value>& args) {
    return cdr((list<value> )car(args));
}

inline const value consProc(const list<value>& args) {
    return cons(car(args), (list<value> )cadr(args));
}

inline const value listProc(const list<value>& args) {
    return args;
}

inline const value assocProc(const list<value>& args) {
    return assoc(car(args), (list<list<value> >)cadr(args));
}

inline const value nulProc(const list<value>& args) {
    const value v(car(args));
    if (isNil(v))
        return true;
    if (isList(v))
        return isNil(list<value>(v));
    return false;
}

inline const value equalProc(const list<value>& args) {
    return (bool)(car(args) == cadr(args));
}

inline const value addProc(const list<value>& args) {
    if (isNil(cdr(args)))
        return (double)car(args);
    return (double)car(args) + (double)cadr(args);
}

inline const value subProc(const list<value>& args) {
    if (isNil(cdr(args)))
        return (double)0 - (double)car(args);
    return (double)car(args) - (double)cadr(args);
}

inline const value mulProc(const list<value>& args) {
    return (double)car(args) * (double)cadr(args);
}

inline const value divProc(const list<value>& args) {
    return (double)car(args) / (double)cadr(args);
}

inline const value displayProc(const list<value>& args) {
    if (isNil(args)) {
        displayStream() << endl;
        return true;
    }
    displayStream() << car(args);
    return displayProc(cdr(args));
}

inline const value logProc(const list<value>& args) {
    if (isNil(args)) {
        logStream() << endl;
        return true;
    }
    logStream() << car(args);
    return logProc(cdr(args));
}

inline const value uuidProc(unused const list<value>& args) {
    return mkuuid();
}

inline const value cadrProc(const list<value>& args) {
    return cadr((list<value> )car(args));
}

inline const value caddrProc(const list<value>& args) {
    return caddr((list<value> )car(args));
}

inline const value cadddrProc(const list<value>& args) {
    return cadddr((list<value> )car(args));
}

inline const value cddrProc(const list<value>& args) {
    return cddr((list<value> )car(args));
}

inline const value cdddrProc(const list<value>& args) {
    return cdddr((list<value> )car(args));
}

inline const value appendProc(const list<value>& args) {
    return append((list<value> )car(args), (list<value>)cadr(args));
}

inline const value startProc(unused const list<value>& args) {
    return lvvlambda();
}

inline const value stopProc(unused const list<value>& args) {
    return lvvlambda();
}

inline const value applyPrimitiveProcedure(const value& proc, list<value>& args) {
    const lvvlambda func(cadr((list<value>)proc));
    return func(args);
}

inline const bool isPrimitiveProcedure(const value& proc) {
    return isTaggedList(proc, primitiveSymbol);
}

inline const bool isSelfEvaluating(const value& exp) {
    if(isNil(exp))
        return true;
    if(isNumber(exp))
        return true;
    if(isString(exp))
        return true;
    if(isBool(exp))
        return true;
    if(isLambda(exp))
        return true;
    return false;
}

inline const value primitiveImplementation(const list<value>& proc) {
    return car(cdr(proc));
}

template<typename F> inline const value primitiveProcedure(const F& f) {
    return mklist<value>(primitiveSymbol, (lvvlambda)f);
}

inline const list<value> primitiveProcedureNames() {
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
    + "append"
    + "display"
    + "log"
    + "uuid"
    + "start"
    + "stop";
}

inline const list<value> primitiveProcedureObjects() {
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
    + primitiveProcedure(appendProc)
    + primitiveProcedure(displayProc)
    + primitiveProcedure(logProc)
    + primitiveProcedure(uuidProc)
    + primitiveProcedure(startProc)
    + primitiveProcedure(stopProc);
}

inline const bool isFalse(const value& exp) {
    return (bool)exp == false;
}

inline const bool isTrue(const value& exp) {
    return (bool)exp == true;
}

inline const bool isQuoted(const value& exp) {
    return isTaggedList(exp, quoteSymbol);
}

inline const value textOfQuotation(const value& exp) {
    return car(cdr((list<value> )exp));
}

inline const value makeLambda(const list<value>& parameters, const list<value>& body) {
    return cons(lambdaSymbol, cons<value>(parameters, body));
}

}
}
#endif /* tuscany_scheme_primitive_hpp */
