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
 * Evaluate a Scheme component implementation and convert it to an
 * applicable lambda function.
 */
const failable<lvvlambda > evalImplementation(const string& path, const value& impl, const list<value>& px) {
    const string fpath(path + (string)attributeValue("script", impl));
    ifstream is(fpath);
    if (fail(is))
        return mkfailure<lvvlambda >(string("Could not read implementation: ") + fpath);
    const value script = scheme::readScript(is);
    if (isNil(script))
        return mkfailure<lvvlambda >(string("Could not read implementation: ") + fpath);
    const list<value> pxproc = scheme::quotedParameters(primitiveProcedures(px));

    const lvvlambda applyImplementation = [script, pxproc](const list<value>& params) -> const value {
        // Apply a Scheme component implementation function
        const value expr = cons<value>(car(params), append(scheme::quotedParameters(cdr(params)), pxproc));
        debug(expr, "modeval::scheme::applyImplementation::input");
        scheme::Env env = scheme::setupEnvironment();
        const value res = scheme::evalScript(expr, script, env);
        const value val = isNil(res)? mklist<value>(nilValue, string("Could not evaluate expression")) : mklist<value>(res);
        debug(val, "modeval::scheme::applyImplementation::result");
        return val;
    };
    return applyImplementation;
}

}
}
}

#endif /* tuscany_modscheme_hpp */
