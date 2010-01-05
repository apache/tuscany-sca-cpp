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

#ifndef tuscany_modcpp_hpp
#define tuscany_modcpp_hpp

/**
 * Evaluation functions used by mod-eval to evaluate implementation.cpp
 * component implementations.
 */

#include "string.hpp"
#include "stream.hpp"

#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "debug.hpp"
#include "monad.hpp"
#include "dynlib.hpp"
#include "../scheme/driver.hpp"
#include "../http/httpd.hpp"

namespace tuscany {
namespace server {
namespace modcpp {

/**
 * Evaluate a C++ component implementation function.
 */
struct evalImplementation {
    const lib ilib;
    const lambda<value(const list<value>&)> impl;
    const list<value> px;
    evalImplementation(const lib& ilib, const lambda<value(const list<value>&)>& impl, const list<value>& px) : ilib(ilib), impl(impl), px(px) {
    }
    const value operator()(const list<value>& params) const {
        debug(params, "modeval::cpp::evalImplementation::input");
        const value val = impl(append(params, px));
        debug(val, "modeval::cpp::evalImplementation::result");
        return val;
    }
};

/**
 * Read a C++ component implementation.
 */
const failable<lambda<value(const list<value>&)> > readImplementation(const string& path, const list<value>& px) {
    const lib ilib(*(new (gc_new<lib>()) lib(path + dynlibExt)));
    const failable<lambda<value(const list<value>&)> > impl(dynlambda<value(const list<value>&)>("eval", ilib));
    if (!hasContent(impl))
        return impl;
    return lambda<value(const list<value>&)>(evalImplementation(ilib, content(impl), px));
}

}
}
}

#endif /* tuscany_modcpp_hpp */
