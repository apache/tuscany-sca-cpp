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

#include <string>
#include <iostream>
#include <fstream>

#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "debug.hpp"
#include "monad.hpp"
#include "cache.hpp"
#include "../eval/primitive.hpp"
#include "../eval/driver.hpp"
#include "../http/httpd.hpp"
#include "mod-eval.hpp"

namespace tuscany {
namespace server {
namespace modeval {
namespace scm {

/**
 * Convert proxy lambdas to evaluator primitive procedures.
 */
const list<value> primitiveProcedures(const list<value>& l) {
    if (isNil(l))
        return l;
    return cons<value>(mklist<value>(eval::primitiveSymbol, car(l)), primitiveProcedures(cdr(l)));
}

/**
 * Evaluate a script component implementation function.
 */
struct evalImplementation {
    const value impl;
    const list<value> px;
    evalImplementation(const value& impl, const list<value>& px) : impl(impl), px(eval::quotedParameters(primitiveProcedures(px))) {
    }
    const failable<value, std::string> operator()(const value& func, const list<value>& params) const {
        const value expr = cons<value>(func, append(eval::quotedParameters(params), px));
        debug(expr, "modeval::scm::evalImplementation::input");
        gc_pool pool;
        eval::Env globalEnv = eval::setupEnvironment(pool);
        const value val = eval::evalScript(expr, impl, globalEnv, pool);
        debug(val, "modeval::scm::evalImplementation::result");
        if (isNil(val))
            return mkfailure<value, std::string>("Could not evaluate expression");
        return val;
    }
};

/**
 * Read a script component implementation.
 */
const failable<ilambda, std::string> readLatestImplementation(const std::string path, const list<value>& px) {
    std::ifstream is(path.c_str(), std::ios_base::in);
    if (is.fail() || is.bad())
        return mkfailure<ilambda, std::string>("Could not read implementation: " + path);
    const value impl = eval::readScript(is);
    if (isNil(impl))
        return mkfailure<ilambda, std::string>("Could not read implementation: " + path);
    return ilambda(evalImplementation(impl, px));
}

const cached<failable<ilambda, std::string> > readImplementation(const std::string& path, const list<value>& px) {
    const lambda<failable<ilambda, std::string>(std::string, list<value>)> ri(readLatestImplementation);
    const lambda<unsigned long(std::string)> ft(latestFileTime);
    return cached<failable<ilambda, std::string> >(curry(ri, path, px), curry(ft, path));
}

}
}
}
}

#endif /* tuscany_modscm_hpp */
