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
 * AMQP queue sender component implementation.
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "qpid.hpp"

// Ignore conversion issues and redundant declarations in Qpid headers
#ifdef WANT_MAINTAINER_WARNINGS
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif

namespace tuscany {
namespace queue {

/**
 * Post an item to a queue.
 */
const failable<value> post(const list<value>& params) {
    QpidConnection qc;
    QpidSession qs(qc);

    // Post the item
    const value pk = ((lambda<value(const list<value>&)>)caddr(params))(list<value>());
    const value key = isList(pk)? append<value>(pk, (list<value>)car(params)) : cons<value>(pk, (list<value>)car(params));
    debug(key, "queue::post::key");
    debug(cadr(params), "queue::post::value");
    const failable<bool> r = post(key, cadr(params), qs);
    if (!hasContent(r))
        return mkfailure<value>(r);
    return key;
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "post")
        return tuscany::queue::post(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
