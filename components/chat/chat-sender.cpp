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
 * XMPP chat sender component implementation.
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "xmpp.hpp"

namespace tuscany {
namespace chat {
namespace sender {

/**
 * Post an item to an XMPP JID.
 */
const failable<value> post(const list<value>& params, XMPPClient& xc) {
    const value to = car<value>(car(params));
    const value val = cadr(params);
    debug(to, "chat::post::jid");
    debug(val, "chat::post::value");
    const failable<bool> r = post(to, val, xc);
    if (!hasContent(r))
        return mkfailure<value>(r);
    return value(mklist<value>(to));
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Extract the the XMPP JID and password
    const list<value> props = params;
    const value jid = ((lvvlambda)car(props))(nilListValue);
    const value pass = ((lvvlambda)cadr(props))(nilListValue);

    // Create an XMPP client session
    XMPPClient xc(jid, pass, false);
    const failable<bool> r = connect(xc);
    if (!hasContent(r))
        return mkfailure<value>(r);

    // Subscribe and listen to XMPP session
    const lambda<const failable<bool>()> subscribe = [xc]() -> const failable<bool> {
        const gc_pool pool;
        debug("chat::subscribe::listen");
        const lambda<const failable<bool>(const value&, const value&, XMPPClient&)> noop = [](unused const value& jid, unused const value& val, unused XMPPClient& xc) -> const failable<bool> {
            return true;
        };
        const failable<bool> r = listen(noop, const_cast<XMPPClient&>(xc));
        debug("chat::subscribe::stopped");
        return r;
    };

    // Listen and relay messages in a worker thread
    worker w(3);
    submit<failable<bool> >(w, subscribe);

    // Return the chat sender component lambda function
    const lvvlambda chatSender = [xc, w](const list<value>& params) -> const value {
        const tuscany::value func(car(params));
        if (func == "post")
            return post(cdr(params), const_cast<XMPPClient&>(xc));

        // Stop the chat sender component
        if (func != "stop")
            return mkfailure<value>();
        debug("chat::sender::stop");

        // Disconnect and shutdown the worker thread
        disconnect(const_cast<XMPPClient&>(xc));
        cancel(const_cast<worker&>(w));
        debug("chat::sender::stopped");

        return failable<value>(value(lvvlambda()));
    };
    return value(chatSender);
}

}
}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::chat::sender::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
