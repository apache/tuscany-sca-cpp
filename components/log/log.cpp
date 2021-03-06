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
 * Scribe-based log component implementation.
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/http/http.hpp"
#include "scribe.hpp"

namespace tuscany {
namespace log {

/**
 * Post an item to the Scribe log.
 */
const failable<value> post(const list<value>& params, const value& host, const value& category, const scribe::Scribe& sc) {
    debug(cadr(params), "log::post::value");
    const failable<bool> val = scribe::log(cadr(params), host, category, sc);
    if (!hasContent(val))
        return mkfailure<value>(val);
    return value(mklist<value>(true));
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Connect to Scribe
    const scribe::Scribe& sc = *(new (gc_new<scribe::Scribe>()) scribe::Scribe("localhost", 1464));

    // Extract the configured category
    const value host = ((lvvlambda)car(params))(nilListValue);
    const value category = ((lvvlambda)cadr(params))(nilListValue);
    debug(host, "log::start::host");
    debug(category, "log::start::category");

    // Return the component implementation lambda function
    const lvvlambda applyLog = [host, category, sc](const list<value>& params) -> const value {
        const value func(car(params));
        if (func == "post")
            return post(cdr(params), host, category, sc);
        return mkfailure<value>();
    };
    return value(applyLog);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::log::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
