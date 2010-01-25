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
 * Web service client component implementation.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "axis2.hpp"

namespace tuscany {
namespace webservice {

/**
 * Apply a function provided by a remote Web service using Axis2.
 */
const failable<value> apply(const value& func, const list<value>& params) {
    const Axis2Context ax;

    // Extract parameters
    const value doc = car<value>(params);
    const lambda<value(const list<value>&)> l = cadr<value>(params);

    // Call the URI property lambda function to get the configured URI
    const value uri = l(list<value>());

    // Evaluate using Axis2
    return evalExpr(mklist<value>(func, doc, uri), ax);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    return tuscany::webservice::apply(car(params), cdr(params));
}

}
