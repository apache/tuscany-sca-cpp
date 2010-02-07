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

#ifndef tuscany_python_driver_hpp
#define tuscany_python_driver_hpp

/**
 * Python evaluator main driver loop.
 */

#include "string.hpp"
#include "stream.hpp"
#include "monad.hpp"
#include "../scheme/driver.hpp"
#include "eval.hpp"

namespace tuscany {
namespace python {

const value evalDriverLoop(PyObject* script, istream& in, ostream& out) {
    scheme::promptForInput(scheme::evalInputPrompt, out);
    value input = scheme::readValue(in);
    if (isNil(input))
        return input;
    const failable<value> output = evalScript(input, script);
    scheme::announceOutput(scheme::evalOutputPrompt, out);
    scheme::userPrint(content(output), out);
    return evalDriverLoop(script, in, out);
}

const bool evalDriverRun(const char* path, istream& in, ostream& out) {
    PythonRuntime py;
    scheme::setupDisplay(out);
    ifstream is(path);
    failable<PyObject*> script = readScript(path, is);
    if (!hasContent(script))
        return true;
    evalDriverLoop(content(script), in, out);
    Py_DECREF(content(script));
    return true;
}

}
}
#endif /* tuscany_python_driver_hpp */
