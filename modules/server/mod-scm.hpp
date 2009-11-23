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
#include "monad.hpp"
#include "cache.hpp"
#include "../eval/driver.hpp"
#include "../http/httpd.hpp"
#include "mod-eval.hpp"

namespace tuscany {
namespace server {
namespace modeval {
namespace scm {

/**
 * Evaluate a script component implementation function.
 */
struct evalImplementation {
    const value impl;
    evalImplementation(const value& impl) : impl(impl) {
    }
    const failable<value, std::string> operator()(const value& func, const list<value>& params) const {
        const value expr = cons<value>(func, eval::quotedParameters(params));
        httpd::logValue(expr, "expr");
        gc_pool pool;
        eval::Env globalEnv = eval::setupEnvironment(pool);
        const value val = eval::evalScript(expr, impl, globalEnv, pool);
        httpd::logValue(val, "val");
        if (isNil(val))
            return mkfailure<value, std::string>("Could not evaluate expression");
        return val;
    }
};

/**
 * Read a script component implementation.
 */
const failable<ilambda, std::string> readLatestImplementation(const std::string path) {
    std::ifstream is(path.c_str(), std::ios_base::in);
    if (is.fail() || is.bad())
        return mkfailure<ilambda, std::string>("Could not read implementation: " + path);
    const value impl = eval::readScript(is);
    if (isNil(impl))
        return mkfailure<ilambda, std::string>("Could not read implementation: " + path);
    return ilambda(evalImplementation(impl));
}

const cached<failable<ilambda, std::string> > readImplementation(const std::string& path) {
    const lambda<failable<ilambda, std::string>(std::string)> ri(readLatestImplementation);
    const lambda<unsigned long(std::string)> ft(latestFileTime);
    return cached<failable<ilambda, std::string> >(curry(ri, path), curry(ft, path));
}

}
}
}
}

#endif /* tuscany_modscm_hpp */
