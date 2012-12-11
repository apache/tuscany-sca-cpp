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

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "../../modules/http/http.hpp"

namespace tuscany {
namespace httppost {

/**
 * Evaluate an HTTP post.
 */
const failable<value> get(const lvvlambda& url, const lvvlambda& val, const http::CURLSession& ch) {
    debug("httppost::get");
    const value u = url(mklist<value>("get", nilListValue));
    const value v = val(mklist<value>("get", nilListValue));
    debug(u, "httppost::get::url");
    debug(v, "httppost::get::val");
    return http::post(v, u, ch);
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Create a CURL session
    const lvvlambda timeout = caddr(params);
    const gc_pool cp(gc_current_pool());
    const lambda<const gc_ptr<http::CURLSession>()> newsession = [timeout, cp]() -> const gc_ptr<http::CURLSession> {
        const gc_scoped_pool sp(pool(cp));
        const int t = atoi(c_str((string)timeout(nilListValue)));
        return new (gc_new<http::CURLSession>()) http::CURLSession(emptyString, emptyString, emptyString, emptyString, t);
    };
    const perthread_ptr<http::CURLSession> ch = *(new (gc_new<perthread_ptr<http::CURLSession> >()) perthread_ptr<http::CURLSession>(newsession));

    // Return the component implementation lambda function
    const lvvlambda url = car(params);
    const lvvlambda val = cadr(params);
    const lvvlambda applyhttp = [url, val, ch](const list<value>& params) -> const value {
        debug(params, "httppost::applyhttp::params");
        const value func(car(params));
        if (func == "get")
            return get(url, val, *ch);
        return mkfailure<value>();
    };
    return value(applyhttp);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::httppost::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
