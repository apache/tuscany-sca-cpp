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

#ifndef tuscany_modjava_hpp
#define tuscany_modjava_hpp

/**
 * Evaluation functions used by mod-eval to evaluate Java
 * component implementations.
 */

#include "string.hpp"
#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "eval.hpp"

namespace tuscany {
namespace server {
namespace modjava {

/**
 * Evaluate a Java component implementation and convert it to an applicable
 * lambda function.
 */
const failable<lvvlambda > evalImplementation(const string& path, const value& impl, const list<value>& px, const java::JavaRuntime& jr) {
    const string cn(attributeValue("class", impl));
    const failable<java::JavaClass> fjc = java::readClass(jr, path, cn);
    if (!hasContent(fjc))
        return mkfailure<lvvlambda >(fjc);

    // Return Java component implementation lambda function
    const java::JavaClass jc = content(fjc);
    const lvvlambda applyImplementation = [jc, px, jr](const list<value>& params) -> const value {
        const value expr = append<value>(params, px);
        debug(expr, "modeval::java::applyImplementation::expr");
        const failable<value> res = java::evalClass(jr, expr, jc);
        const value val = !hasContent(res)? mklist<value>(nilValue, reason(res), rcode(res)) : mklist<value>(content(res));
        debug(val, "modeval::java::applyImplementation::result");
        return val;
    };
    return applyImplementation;
}

}
}
}

#endif /* tuscany_modjava_hpp */
