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

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "qpid.hpp"

// Ignore conversion issues and redundant declarations in Qpid headers
#ifdef WANT_MAINTAINER_WARNINGS
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif

namespace tuscany {
namespace queue {

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Extract the relay reference and the AMQP key and queue name
    const value rel = car(params);
    const value pk = ((lvvlambda)cadr(params))(nilListValue);
    const value key = isList(pk)? (list<value>)pk : mklist<value>(pk);
    const value qname = ((lvvlambda)caddr(params))(nilListValue);

    // Create an AMQP session
    QpidConnection qc(false);
    QpidSession qs(qc, false);

    // Declare the configured AMQP key / queue pair
    declareQueue(key, qname, qs);

    // Listen and relay messages in a worker thread
    QpidSubscription qsub(qs, false);
    const worker w(3);
    const lambda<const bool(const value&, const value&)> rl = [rel](const value& k, const value& v) -> const bool {
        // A relay function that posts the AMQP messages it receives to a relay component reference.
        debug(k, "queue::relay::key");
        debug(v, "queue::relay::value");
        const value res = rel(mklist<value>("post", isList(k)? (list<value>)k : mklist<value>(k), v));
        return true;
    };

    // Subscribe and listen to the AMQP queue.
    const lambda<const failable<bool>()> subscribe = [qname, rl, qsub]() -> const failable<bool> {
        const gc_pool pool;
        debug(qname, "queue::subscribe::listen");
        const failable<bool> r =  listen(qname, rl, const_cast<QpidSubscription&>(qsub));
        debug(qname, "queue::subscribe::stopped");
        return r;
    };
    submit<failable<bool> >(w, subscribe);

    // Return the listener component lambda function
    const lvvlambda listener = [qc, qs, qsub, w](const list<value>& params) -> const value {
        const tuscany::value func(car(params));

        // Stop the component
        if (func != "stop")
            return mkfailure<value>();
        debug("queue::listener::stop");

        // TODO check why stop() and close() hang in child processes
        stop(const_cast<QpidSubscription&>(qsub));
        close(const_cast<QpidSession&>(qs));
        close(const_cast<QpidConnection&>(qc));
        cancel(const_cast<worker&>(w));

        debug("queue::listener::stopped");
        return failable<value>(value(lvvlambda()));
    };
    return value(listener);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::queue::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
