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
 * Evaluation functions used by mod-eval to evaluate implementation.python
 * component implementations.
 */

#include "string.hpp"
#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "debug.hpp"
#include "monad.hpp"
#include "../python/eval.hpp"
#include "../http/httpd.hpp"

namespace tuscany {
namespace server {
namespace modpython {

/**
 * Evaluate a script component implementation function.
 */
struct evalImplementation {
    PyObject* impl;
    const list<value> px;
    evalImplementation(PyObject* impl, const list<value>& px) : impl(impl), px(px) {
    }
    const value operator()(const list<value>& params) const {
        const value expr = append<value>(params, px);
        debug(expr, "modeval::python::evalImplementation::input");
        gc_pool pool(gc_current_pool());
        const failable<value> val = python::evalScript(expr, impl, pool);
        debug(val, "modeval::python::evalImplementation::result");
        if (!hasContent(val))
            return mklist<value>(value(), reason(val));
        return mklist<value>(content(val));
    }
};

/**
 * Read a script component implementation.
 */
const failable<lambda<value(const list<value>&)> > readImplementation(const string& path, const list<value>& px) {
    ifstream is(path);
    if (fail(is))
        return mkfailure<lambda<value(const list<value>&)> >(string("Could not read implementation: ") + path);
    const failable<PyObject*> impl = python::readScript(path, is);
    if (!hasContent(impl))
        return mkfailure<lambda<value(const list<value>&)> >(reason(impl));
    return lambda<value(const list<value>&)>(evalImplementation(content(impl), px));
}

}
}
}

#endif /* tuscany_modpython_hpp */
