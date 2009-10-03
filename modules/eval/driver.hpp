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

namespace tuscany
{

const std::string evalOutputPrompt("; ");
const std::string evalInputPrompt("=> ");

const bool promptForInput(std::ostream& out, const std::string str) {
    out << "\n\n" << str;
    return true;
}

const bool announceOutput(std::ostream& out, const std::string str) {
    out << "\n" << str;
    return true;
}

const bool userPrint(std::ostream& out, const value object) {
    if(isCompoundProcedure(object))
        out << makeList(compoundProcedureSymbol, value(procedureParameters(object)), value(procedureBody(object)), value(
                "<procedure-env>"));
    out << object;
    return true;
}

const value evalDriverLoop(std::istream& in, std::ostream& out, Env& globalEnv) {
    promptForInput(out, evalInputPrompt);
    value input = read(in);
    if (isNil(input))
        return input;
    const value output = eval(input, globalEnv);
    announceOutput(out, evalOutputPrompt);
    userPrint(out, output);
    return evalDriverLoop(in, out, globalEnv);
}

const bool evalDriverRun(std::istream& in, std::ostream& out) {
    setupEvalOut(out);
    Env globalEnv = setupEnvironment();
    evalDriverLoop(in, out, globalEnv);
    return true;
}

}
#endif /* tuscany_eval_driver_hpp */
