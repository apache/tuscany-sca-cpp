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

#define WANT_HTTPD_LOG 1
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
const value applyLifecycle(unused const list<value>& params) {

    // Create a Python runtime
    python::PythonRuntime& py = *(new (gc_new<python::PythonRuntime>()) python::PythonRuntime());

    // Return the function to invoke on subsequent events
    return failable<value>(lvvlambda([&py](const list<value>& params) -> const value {
        const value func = car(params);
        if (func == "pythonRuntime")
            return (gc_ptr<value>)(value*)&py;
        return lvvlambda();
    }));
}

/**
 * Evaluate a Python component implementation and convert it to an applicable
 * lambda function.
 */
const failable<lvvlambda > evalImplementation(const string& path, const value& impl, const list<value>& px, const lvvlambda& lifecycle) {
    const string itype(elementName(impl));
    if (contains(itype, ".python")) {
        const value* const p = (gc_ptr<value>)lifecycle(mklist<value>("pythonRuntime"));
        return modpython::evalImplementation(path, impl, px, *(python::PythonRuntime*)p);
    }
    if (contains(itype, ".cpp"))
        return modcpp::evalImplementation(path, impl, px);
    if (contains(itype, ".widget"))
        return mkfailure<lvvlambda >(string("Unsupported implementation type: ") + itype, -1, false);
    return mkfailure<lvvlambda >(string("Unsupported implementation type: ") + itype);
}

}
}
}
