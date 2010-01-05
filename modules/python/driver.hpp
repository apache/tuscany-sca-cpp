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

#ifndef tuscany_python_pydriver_hpp
#define tuscany_python_pydriver_hpp

/**
 * Python evaluator main driver loop.
 */

#include "string.hpp"
#include "stream.hpp"
#include "monad.hpp"
#include "eval.hpp"

namespace tuscany {
namespace python {

const string evalOutputPrompt("; ");
const string evalInputPrompt("=> ");

const bool promptForInput(const string& str, ostream& out) {
    out << endl << endl << str;
    return true;
}

const bool announceOutput(const string str, ostream& out) {
    out << endl << str;
    return true;
}

const bool userPrint(const value val, ostream& out) {
    writeValue(val, out);
    return true;
}

const value evalDriverLoop(PyObject* script, istream& in, ostream& out, const gc_pool& pool) {
    promptForInput(evalInputPrompt, out);
    value input = readValue(in);
    if (isNil(input))
        return input;
    const value output = evalScript(input, script, pool);
    announceOutput(evalOutputPrompt, out);
    userPrint(output, out);
    return evalDriverLoop(script, in, out, pool);
}

const bool evalDriverRun(istream& in, ostream& out, const gc_pool& pool) {
    setupDisplay(out);
    evalDriverLoop(builtin(pythonRuntime), in, out, pool);
    return true;
}

const bool evalDriverRun(const char* path, istream& in, ostream& out, const gc_pool& pool) {
    setupDisplay(out);
    ifstream is(path);
    failable<PyObject*> script = readScript(path, is);
    if (!hasContent(script))
        return true;
    evalDriverLoop(content(script), in, out, pool);
    Py_DECREF(content(script));
    return true;
}

}
}
#endif /* tuscany_scheme_pydriver_hpp */
