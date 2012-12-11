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

#ifndef tuscany_modpython_hpp
#define tuscany_modpython_hpp

/**
 * Evaluation functions used by mod-eval to evaluate Python
 * component implementations.
 */

#include "string.hpp"
#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "eval.hpp"

namespace tuscany {
namespace server {
namespace modpython {

/**
 * Evaluate a Python component implementation and convert it to an applicable
 * lambda function.
 */
const failable<lvvlambda > evalImplementation(const string& path, const value& impl, const list<value>& px, python::PythonRuntime& py) {
    const string spath(attributeValue("script", impl));
    const string fpath(path + spath);
    ifstream is(fpath);
    if (fail(is))
        return mkfailure<lvvlambda >(string("Could not read implementation: ") + fpath);
    const failable<PyObject*> fscript = python::readScript(python::moduleName(spath), fpath, is, py);
    if (!hasContent(fscript))
        return mkfailure<lvvlambda >(fscript);
    PyObject* const script = content(fscript);

    const lvvlambda applyImplementation = [script, px, &py](const list<value>& params) -> const value {
        // Apply a Python component implementation function
        const value expr = append<value>(params, px);
        debug(expr, "modeval::python::applyImplementation::input");
        const failable<value> res = python::evalScript(expr, script, py);
        const value val = !hasContent(res)? mklist<value>(nilValue, reason(res), rcode(res)) : mklist<value>(content(res));
        debug(val, "modeval::python::applyImplementation::result");
        return val;
    };
    return applyImplementation;
}

}
}
}

#endif /* tuscany_modpython_hpp */
