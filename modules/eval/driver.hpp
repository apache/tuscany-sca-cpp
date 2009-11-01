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

#ifndef tuscany_eval_driver_hpp
#define tuscany_eval_driver_hpp

/**
 * Script evaluator main driver loop.
 */

#include <string>
#include <iostream>
#include "eval.hpp"

namespace tuscany {
namespace eval {

const std::string evalOutputPrompt("; ");
const std::string evalInputPrompt("=> ");

const bool promptForInput(const std::string str, std::ostream& out) {
    out << "\n\n" << str;
    return true;
}

const bool announceOutput(const std::string str, std::ostream& out) {
    out << "\n" << str;
    return true;
}

const bool userPrint(const value val, std::ostream& out) {
    if(isCompoundProcedure(val))
        writeValue(mklist<value>(compoundProcedureSymbol, procedureParameters(val), procedureBody(val), "<procedure-env>"), out);
    writeValue(val, out);
    return true;
}

const value evalDriverLoop(std::istream& in, std::ostream& out, Env& globalEnv, const gc_pool& pool) {
    promptForInput(evalInputPrompt, out);
    value input = readValue(in);
    if (isNil(input))
        return input;
    const value output = evalExpr(input, globalEnv, pool);
    announceOutput(evalOutputPrompt, out);
    userPrint(output, out);
    return evalDriverLoop(in, out, globalEnv, pool);
}

const bool evalDriverRun(std::istream& in, std::ostream& out) {
    gc_pool pool;
    setupDisplay(out);
    Env globalEnv = setupEnvironment(pool);
    evalDriverLoop(in, out, globalEnv, pool);
    return true;
}

}
}
#endif /* tuscany_eval_driver_hpp */
