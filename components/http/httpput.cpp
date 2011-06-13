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
 * HTTP client component implementation.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/http/http.hpp"

namespace tuscany {
namespace httpput {

/**
 * Evaluate an HTTP put.
 */
const failable<value> get(const lambda<value(const list<value>&)> url, const lambda<value(const list<value>&)> val, http::CURLSession& ch) {
    debug("httpput::get");
    const value u = url(mklist<value>("get", list<value>()));
    const value v = val(mklist<value>("get", list<value>()));
    debug(u, "httpput::get::url");
    debug(v, "httpput::get::val");
    return http::put(v, u, ch);
}

/**
 * Component implementation lambda function.
 */
class applyhttp {
public:
    applyhttp(const lambda<value(const list<value>&)> url, const lambda<value(const list<value>&)> val, http::CURLSession& ch) : url(url), val(val), ch(ch) {
    }

    const value operator()(const list<value>& params) const {
        debug(params, "httpput::applyhttp::params");
        const value func(car(params));
        if (func == "get")
            return get(url, val, ch);
        return tuscany::mkfailure<tuscany::value>();
    }

private:
    const lambda<value(const list<value>&)> url;
    const lambda<value(const list<value>&)> val;
    http::CURLSession& ch;
};

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Create a CURL session
    http::CURLSession& ch = *(new (gc_new<http::CURLSession>()) http::CURLSession("", "", "", ""));

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applyhttp(car(params), cadr(params), ch)));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::httpput::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
