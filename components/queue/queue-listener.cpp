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
 * AMQP queue listener component implementation.
 */

#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "qpid.hpp"

// Ignore conversion issues and redundant declarations in Qpid headers
#ifdef WANT_MAINTAINER_MODE
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif

namespace tuscany {
namespace queue {

QpidConnection qc;

/**
 * Initialize the component.
 */
const failable<value> init(const list<value>& params) {
    QpidSession qs(qc);

    // Declare the configured AMQP key / queue pair
    const value key = ((lambda<value(list<value>)>)caddr(params))(list<value>());
    const value qname = ((lambda<value(list<value>)>)cadddr(params))(list<value>());
    declareQueue(key, qname, qs);

    //TODO create a subscription and mark the current server instance busy

    return value(true);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "init")
        return tuscany::queue::init(cdr(params));
    return tuscany::mkfailure<tuscany::value>(tuscany::string("Function not supported: ") + func);
}

}
