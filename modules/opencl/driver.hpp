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

#ifndef tuscany_opencl_driver_hpp
#define tuscany_opencl_driver_hpp

/**
 * OpenCL evaluator main driver loop.
 */

#include "string.hpp"
#include "stream.hpp"
#include "monad.hpp"
#include "../scheme/driver.hpp"
#include "eval.hpp"

namespace tuscany {
namespace opencl {

const value evalDriverLoop(const OpenCLProgram& clprog, istream& in, ostream& out, const OpenCLContext& cl) {
    scheme::promptForInput(scheme::evalInputPrompt, out);
    value input = scheme::readValue(in);
    if (isNil(input))
        return input;
    const failable<value> output = evalKernel(createKernel(input, clprog), input, 1, value::String, 512, cl);
    scheme::announceOutput(scheme::evalOutputPrompt, out);
    scheme::userPrint(content(output), out);
    return evalDriverLoop(clprog, in, out, cl);
}

const bool evalDriverRun(const char* path, istream& in, ostream& out) {
    OpenCLContext cl(OpenCLContext::DEFAULT);
    scheme::setupDisplay(out);
    ifstream is(path);
    failable<OpenCLProgram> clprog = readProgram(path, is, cl);
    if (!hasContent(clprog))
        return true;
    evalDriverLoop(content(clprog), in, out, cl);
    return true;
}

}
}
#endif /* tuscany_opencl_driver_hpp */
