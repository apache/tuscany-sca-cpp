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

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "scribe.hpp"

namespace tuscany {
namespace log {

/**
 * Post an item to the Scribe log.
 */
const failable<value> post(const list<value>& params, const value& category, scribe::Scribe& sc) {
    debug(cadr(params), "log::post::value");
    const failable<bool> val = scribe::log(cadr(params), category, sc);
    if (!hasContent(val))
        return mkfailure<value>(reason(val));
    return value(mklist<value>(true));
}

/**
 * Component implementation lambda function.
 */
class applyLog {
public:
    applyLog(const value& category, scribe::Scribe& sc) : category(category), sc(sc) {
    }

    const value operator()(const list<value>& params) const {
        const value func(car(params));
        if (func == "post")
            return post(cdr(params), category, sc);
        return tuscany::mkfailure<tuscany::value>();
    }

private:
    const value category;
    scribe::Scribe& sc;
};

/**
 * Start the component.
 */
const failable<value> start(unused const list<value>& params) {
    // Connect to Scribe
    scribe::Scribe& sc = *(new (gc_new<scribe::Scribe>()) scribe::Scribe("localhost", 1464));

    // Extract the configured category
    const value category = ((lambda<value(list<value>)>)car(params))(list<value>());
    debug(category, "log::start::category");

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applyLog(category, sc)));
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
