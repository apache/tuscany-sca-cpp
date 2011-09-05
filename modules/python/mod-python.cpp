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

/**
 * HTTPD module used to eval Python component implementations.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../server/mod-cpp.hpp"
#include "../server/mod-eval.hpp"
#include "mod-python.hpp"

namespace tuscany {
namespace server {
namespace modeval {

/**
 * Apply a lifecycle start or restart event.
 */
struct pythonLifecycle {
    python::PythonRuntime& py;
    pythonLifecycle(python::PythonRuntime& py) : py(py) {
    }
    const value operator()(const list<value>& params) const {
        const value func = car(params);
        if (func == "pythonRuntime")
            return (gc_ptr<value>)(value*)(void*)&py;
        return lambda<value(const list<value>&)>();
    }
};

const value applyLifecycle(unused const list<value>& params) {

    // Create a Python runtime
    python::PythonRuntime& py = *(new (gc_new<python::PythonRuntime>()) python::PythonRuntime());

    // Return the function to invoke on subsequent events
    return failable<value>(lambda<value(const list<value>&)>(pythonLifecycle(py)));
}

/**
 * Evaluate a Python component implementation and convert it to an applicable
 * lambda function.
 */
const failable<lambda<value(const list<value>&)> > evalImplementation(const string& path, const value& impl, const list<value>& px, const lambda<value(const list<value>&)>& lifecycle) {
    const string itype(elementName(impl));
    if (contains(itype, ".python")) {
        const void* p = (gc_ptr<value>)lifecycle(mklist<value>("pythonRuntime"));
        return modpython::evalImplementation(path, impl, px, *(python::PythonRuntime*)p);
    }
    if (contains(itype, ".cpp"))
        return modcpp::evalImplementation(path, impl, px);
    return mkfailure<lambda<value(const list<value>&)> >(string("Unsupported implementation type: ") + itype);
}

}
}
}
