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

namespace tuscany
{

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

const value firstFrame(const Env& env) {
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
            std::cout << "Too many arguments supplied " << values << "\n";
        return frameSoFar;
    }
    if (isDotVariable(car(variables)))
        return makeBinding(frameSoFar, cdr(variables), makeList<value>(values));

    if (isNil(values)) {
        if (!isNil(variables))
            std::cout << "Too few arguments supplied " << variables << "\n";
        return frameSoFar;
    }

    const list<value> vars = cons(car(variables), frameVariables(frameSoFar));
    const list<value> vals = cons(car(values), frameValues(frameSoFar));
    const Frame newFrame = cons(value(vars), vals);

    return makeBinding(newFrame, cdr(variables), cdr(values));
}

const Frame makeFrame(const list<value>& variables, const list<value> values) {
    const Frame emptyFrame = cons(value(list<value>()), list<value>());
    return makeBinding(emptyFrame, variables, values);
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

const Env defineVariable(const value& var, const value& val, Env& env) {
    return cons(addBindingToFrame(var, val, firstFrame(env)), cdr(env));
}

const Env extendEnvironment(const list<value>& vars, const list<value>& vals, const Env& baseEnv) {
//    if(length(vars) == length(vals))
//    else if(length(vars) < length(vals))
//        std::cout << "Too many arguments supplied " << vars << " " << vals << "\n";
//    else
//        std::cout << "Too few arguments supplied " << vars << " " << vals << "\n";
//    return baseEnv;
    return cons(makeFrame(vars, vals), baseEnv);
}

const Env setupEnvironment() {
    Env env = extendEnvironment(primitiveProcedureNames(), primitiveProcedureObjects(), theEmptyEnvironment());
    env = defineVariable(trueSymbol, true, env);
    env = defineVariable(falseSymbol, false, env);
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
        std::cout << "Unbound variable " << var << "\n";
        return value();
    }
    return lookupEnvScan(var, frameVariables(firstFrame(env)), frameValues(firstFrame(env)), env);
}

const value lookupVariableValue(const value& var, const Env& env) {
    return lookupEnvLoop(var, env);
}

}
#endif /* tuscany_eval_environment_hpp */
