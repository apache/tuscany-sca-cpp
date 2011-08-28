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
#include "parallel.hpp"
#include "../../modules/http/http.hpp"

namespace tuscany {
namespace httppost {

/**
 * Evaluate an HTTP post.
 */
const failable<value> get(const lambda<value(const list<value>&)> url, const lambda<value(const list<value>&)> val, http::CURLSession& ch) {
    debug("httppost::get");
    const value u = url(mklist<value>("get", list<value>()));
    const value v = val(mklist<value>("get", list<value>()));
    debug(u, "httppost::get::url");
    debug(v, "httppost::get::val");
    return http::post(v, u, ch);
}

/**
 * Component implementation lambda function.
 */
class applyhttp {
public:
    applyhttp(const lambda<value(const list<value>&)> url, const lambda<value(const list<value>&)> val, const perthread_ptr<http::CURLSession>& ch) : url(url), val(val), ch(ch) {
    }

    const value operator()(const list<value>& params) const {
        debug(params, "httppost::applyhttp::params");
        const value func(car(params));
        if (func == "get")
            return get(url, val, *ch);
        return tuscany::mkfailure<tuscany::value>();
    }

private:
    const lambda<value(const list<value>&)> url;
    const lambda<value(const list<value>&)> val;
    perthread_ptr<http::CURLSession> ch;
};

/**
 * Create a new CURL session.
 */
const gc_ptr<http::CURLSession> newsession() {
    return new (gc_new<http::CURLSession>()) http::CURLSession("", "", "", "");
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Create a CURL session
    const perthread_ptr<http::CURLSession> ch = perthread_ptr<http::CURLSession>(lambda<gc_ptr<http::CURLSession>()>(newsession));

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applyhttp(car(params), cadr(params), ch)));
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
