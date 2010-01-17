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

#ifndef tuscany_modscheme_hpp
#define tuscany_modscheme_hpp

/**
 * Evaluation functions used by mod-eval to evaluate Scheme
 * component implementations.
 */

#include "string.hpp"
#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../scheme/eval.hpp"
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
 * Apply a Scheme component implementation function.
 */
struct applyImplementation {
    const value impl;
    const list<value> px;
    applyImplementation(const value& impl, const list<value>& px) : impl(impl), px(scheme::quotedParameters(primitiveProcedures(px))) {
    }
    const value operator()(const list<value>& params) const {
        const value expr = cons<value>(car(params), append(scheme::quotedParameters(cdr(params)), px));
        debug(expr, "modeval::scheme::applyImplementation::input");
        scheme::Env env = scheme::setupEnvironment();
        const value val = scheme::evalScript(expr, impl, env);
        debug(val, "modeval::scheme::applyImplementation::result");
        if (isNil(val))
            return mklist<value>(value(), string("Could not evaluate expression"));
        return mklist<value>(val);
    }
};

/**
 * Evaluate a Scheme component implementation and convert it to an
 * applicable lambda function.
 */
const failable<lambda<value(const list<value>&)> > evalImplementation(const string& path, const value& impl, const list<value>& px) {
    const string fpath(path + attributeValue("script", impl));
    ifstream is(fpath);
    if (fail(is))
        return mkfailure<lambda<value(const list<value>&)> >(string("Could not read implementation: ") + fpath);
    const value script = scheme::readScript(is);
    if (isNil(script))
        return mkfailure<lambda<value(const list<value>&)> >(string("Could not read implementation: ") + fpath);
    return lambda<value(const list<value>&)>(applyImplementation(script, px));
}

}
}
}

#endif /* tuscany_modscheme_hpp */
