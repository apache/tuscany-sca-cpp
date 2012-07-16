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
 * Scribe-based logger component implementation, used to intercept
 * and log service invocations.
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
namespace logger {

/**
 * Component implementation lambda function.
 */
class applyLog {
public:
    applyLog(const lambda<value(const list<value>&)>& relay, const value& host, const value& category, scribe::Scribe& sc) : relay(relay), host(host), category(category), sc(sc) {
    }

    const value operator()(const list<value>& params) const {
        // Log the function params
        debug(params, "logger::apply::params");
        scribe::log(params, host, category, sc);

        // Relay the function call
        const failable<value> res = relay(params);

        // Log the result
        scribe::log(res, host, category, sc);
        return res;
    }

private:
    const lambda<value(const list<value>&)> relay;
    const value host;
    const value category;
    scribe::Scribe& sc;
};

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Connect to Scribe
    scribe::Scribe& sc = *(new (gc_new<scribe::Scribe>()) scribe::Scribe("localhost", 1464));

    // Extract the configured relay service and category
    const value rel = car(params);
    const value host = ((lambda<value(const list<value>&)>)cadr(params))(list<value>());
    const value category = ((lambda<value(const list<value>&)>)caddr(params))(list<value>());
    debug(host, "logger::start::host");
    debug(category, "logger::start::category");

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applyLog(rel, host, category, sc)));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::logger::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
