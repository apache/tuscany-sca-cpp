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
 * Start the module.
 */
const failable<bool> start(unused ServerConf& sc) {
    // Start a Python runtime
    sc.moduleConf =  new (gc_new<python::PythonRuntime>()) python::PythonRuntime();
    return true;
}

/**
 * Stop the module.
 */
const failable<bool> stop(unused ServerConf& sc) {
    return true;
}

/**
 * Restart the module.
 */
const failable<bool> restart(unused ServerConf& sc) {
    // Start a Python runtime
    sc.moduleConf =  new (gc_new<python::PythonRuntime>()) python::PythonRuntime();
    return true;
}

/**
 * Evaluate a Python component implementation and convert it to an applicable
 * lambda function.
 */
const failable<lambda<value(const list<value>&)> > evalImplementation(const string& path, const value& impl, const list<value>& px, modeval::ServerConf& sc) {
    const string itype(elementName(impl));
    if (contains(itype, ".python"))
        return modpython::evalImplementation(path, impl, px, sc);
    if (contains(itype, ".cpp"))
        return modcpp::evalImplementation(path, impl, px, sc);
    return mkfailure<lambda<value(const list<value>&)> >(string("Unsupported implementation type: ") + itype);
}

}
}
}
