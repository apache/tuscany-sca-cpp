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
 * HTTPD module used to eval C++ and Scheme component implementations.
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "mod-eval.hpp"
#include "mod-scheme.hpp"
#include "mod-cpp.hpp"

namespace tuscany {
namespace server {
namespace modeval {

/**
 * Apply a lifecycle start or restart event.
 */
const value applyLifecycle(unused const list<value>& params) {
    // Return a nil function as we don't need to handle any subsequent events
    return failable<value>(lambda<value(const list<value>&)>());
}

/**
 * Evaluate a Scheme or C++ component implementation and convert it to an
 * applicable lambda function.
 */
const failable<lambda<value(const list<value>&)> > evalImplementation(const string& path, const value& impl, const list<value>& px, unused const lambda<value(const list<value>&)>& lifecycle) {
    const string itype(elementName(impl));
    if (contains(itype, ".scheme"))
        return modscheme::evalImplementation(path, impl, px);
    if (contains(itype, ".cpp"))
        return modcpp::evalImplementation(path, impl, px);
    if (contains(itype, ".widget"))
        return mkfailure<lambda<value(const list<value>&)> >(string("Unsupported implementation type: ") + itype, -1, false);
    return mkfailure<lambda<value(const list<value>&)> >(string("Unsupported implementation type: ") + itype);
}

}
}
}
