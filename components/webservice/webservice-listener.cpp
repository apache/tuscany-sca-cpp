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
 * Web service listener component implementation.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/http/httpd.hpp"
#include "axis2.hpp"

namespace tuscany {
namespace webservice {

/**
 * Handle an HTTP request.
 */
const failable<value> handle(const list<value>& params) {

    // Extract HTTPD request and relay reference
    unused request_rec* r = httpd::request(car(params));
    const lambda<value(const list<value>&)> relay = cadr(params);

    //TODO Hook Axis2/C server module here

    return value(true);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "handle")
        return tuscany::webservice::handle(cdr(params));
    return tuscany::mkfailure<tuscany::value>(tuscany::string("Function not supported: ") + func);
}

}
