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

#ifndef tuscany_eval_environment_hpp
#define tuscany_eval_environment_hpp

/**
 * Script evaluator environment implementation.
 */

#include <string>
#include "list.hpp"
#include "value.hpp"
#include "primitive.hpp"

namespace tuscany {
namespace eval {

typedef value Frame;
typedef list<value> Env;

const value trueSymbol("true");
const value falseSymbol("false");
const value defineSymbol("define");
const value setSymbol("set!");
const value dotSymbol(".");

const Env theEmptyEnvironment() {
    return list<value>();
}

const bool isDefinition(const value& exp) {
    return isTaggedList(exp, defineSymbol);
}

const bool isAssignment(const value& exp) {
    return isTaggedList(exp, setSymbol);
}

const bool isVariable(const value& exp) {
    return isSymbol(exp);
}

const Env enclosingEnvironment(const Env& env) {
    return cdr(env);
}

const gc_pool_ptr<Frame> firstFrame(const Env& env) {
    return car(env);
}

list<value> frameVariables(const Frame& frame) {
    return car((list<value> )frame);
}

list<value> frameValues(const Frame& frame) {
    return cdr((list<value> )frame);
}

const bool isDotVariable(const value& var) {
    return var == dotSymbol;
}

const Frame makeBinding(const Frame& frameSoFar, const list<value>& variables, const list<value> values) {
    if (isNil(variables)) {
        if (!isNil(values))
            logStream() << "Too many arguments supplied " << values << std::endl;
        return frameSoFar;
    }
    if (isDotVariable(car(variables)))
        return makeBinding(frameSoFar, cdr(variables), mklist<value>(values));

    if (isNil(values)) {
        if (!isNil(variables))
            logStream() << "Too few arguments supplied " << variables << std::endl;
        return frameSoFar;
    }

    const list<value> vars = cons(car(variables), frameVariables(frameSoFar));
    const list<value> vals = cons(car(values), frameValues(frameSoFar));
    const Frame newFrame = cons(value(vars), vals);

    return makeBinding(newFrame, cdr(variables), cdr(values));
}

const gc_pool_ptr<Frame> makeFrame(const list<value>& variables, const list<value> values, const gc_pool& pool) {
    gc_pool_ptr<Frame> frame = gc_pool_new<Frame>(pool);
    *frame = value(makeBinding(cons(value(list<value>()), list<value>()), variables, values));
    return frame;
}

const value definitionVariable(const value& exp) {
    const list<value> exps(exp);
    if(isSymbol(car(cdr(exps))))
        return car(cdr(exps));
    const list<value> lexps(car(cdr(exps)));
    return car(lexps);
}

const value definitionValue(const value& exp) {
    const list<value> exps(exp);
    if(isSymbol(car(cdr(exps))))
        return car(cdr(cdr(exps)));
    const list<value> lexps(car(cdr(exps)));
    return makeLambda(cdr(lexps), cdr(cdr(exps)));
}

const value assignmentVariable(const value& exp) {
    return car(cdr((list<value> )exp));
}

const value assignmentValue(const value& exp) {
    return car(cdr(cdr((list<value> )exp)));
}

const Frame addBindingToFrame(const value& var, const value& val, const Frame& frame) {
    return cons(value(cons(var, frameVariables(frame))), cons(val, frameValues(frame)));
}

const bool defineVariable(const value& var, const value& val, Env& env) {
    *firstFrame(env) = addBindingToFrame(var, val, *firstFrame(env));
    return true;
}

const Env extendEnvironment(const list<value>& vars, const list<value>& vals, const Env& baseEnv, const gc_pool& pool) {
    return cons<value>(makeFrame(vars, vals, pool), baseEnv);
}

const Env setupEnvironment(const gc_pool& pool) {
    Env env = extendEnvironment(primitiveProcedureNames(), primitiveProcedureObjects(), theEmptyEnvironment(), pool);
    defineVariable(trueSymbol, true, env);
    defineVariable(falseSymbol, false, env);
    return env;
}

const value lookupEnvLoop(const value& var, const Env& env);

const value lookupEnvScan(const value& var, const list<value>& vars, const list<value>& vals, const Env& env) {
    if(isNil(vars))
        return lookupEnvLoop(var, enclosingEnvironment(env));
    if(var == car(vars))
        return car(vals);
    return lookupEnvScan(var, cdr(vars), cdr(vals), env);
}

const value lookupEnvLoop(const value& var, const Env& env) {
    if(env == theEmptyEnvironment()) {
        logStream() << "Unbound variable " << var << std::endl;
        return value();
    }
    return lookupEnvScan(var, frameVariables(*firstFrame(env)), frameValues(*firstFrame(env)), env);
}

const value lookupVariableValue(const value& var, const Env& env) {
    return lookupEnvLoop(var, env);
}

}
}
#endif /* tuscany_eval_environment_hpp */
