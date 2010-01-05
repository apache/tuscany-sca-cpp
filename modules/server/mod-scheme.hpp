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

#ifndef tuscany_modscm_hpp
#define tuscany_modscm_hpp

/**
 * Evaluation functions used by mod-eval to evaluate implementation.scheme
 * component implementations.
 */

#include "string.hpp"
#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "debug.hpp"
#include "monad.hpp"
#include "../scheme/primitive.hpp"
#include "../scheme/driver.hpp"
#include "../http/httpd.hpp"

namespace tuscany {
namespace server {
namespace modscheme {

/**
 * Convert proxy lambdas to evaluator primitive procedures.
 */
const list<value> primitiveProcedures(const list<value>& l) {
    if (isNil(l))
        return l;
    return cons<value>(mklist<value>(scheme::primitiveSymbol, car(l)), primitiveProcedures(cdr(l)));
}

/**
 * Evaluate a script component implementation function.
 */
struct evalImplementation {
    const value impl;
    const list<value> px;
    evalImplementation(const value& impl, const list<value>& px) : impl(impl), px(scheme::quotedParameters(primitiveProcedures(px))) {
    }
    const value operator()(const list<value>& params) const {
        const value expr = cons<value>(car(params), append(scheme::quotedParameters(cdr(params)), px));
        debug(expr, "modeval::scm::evalImplementation::input");
        gc_pool pool(gc_current_pool());
        scheme::Env globalEnv = scheme::setupEnvironment(pool);
        const value val = scheme::evalScript(expr, impl, globalEnv, pool);
        debug(val, "modeval::scm::evalImplementation::result");
        if (isNil(val))
            return mklist<value>(value(), string("Could not evaluate expression"));
        return mklist<value>(val);
    }
};

/**
 * Read a script component implementation.
 */
const failable<lambda<value(const list<value>&)> > readImplementation(const string& path, const list<value>& px) {
    ifstream is(path);
    if (fail(is))
        return mkfailure<lambda<value(const list<value>&)> >(string("Could not read implementation: ") + path);
    const value impl = scheme::readScript(is);
    if (isNil(impl))
        return mkfailure<lambda<value(const list<value>&)> >(string("Could not read implementation: ") + path);
    return lambda<value(const list<value>&)>(evalImplementation(impl, px));
}

}
}
}

#endif /* tuscany_modscm_hpp */
