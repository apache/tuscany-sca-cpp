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
 * This sender gets its configuration from a single property and its
 * input data from component references instead of function parameters.
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
const failable<value> post(const lvvlambda& jid, const lvvlambda& pass, const lvvlambda& to, const lvvlambda& msg, const list<value>& params) {

    const value vjid = jid(mklist<value>("get", params));
    const value vpass = pass(mklist<value>("get", params));
    const value vto = to(mklist<value>("get", params));
    const value vmsg = msg(mklist<value>("get", params));
    debug(vjid, "chat::post::from");
    debug(vto, "chat::post::to");
    debug(vmsg, "chat::post::value");

    // Create an XMPP client session
    XMPPClient xc(vjid, vpass);
    const failable<bool> c = connect(xc);
    if (!hasContent(c))
        return mkfailure<value>(c);

    // Post the message
    const failable<bool> r = post(vto, vmsg, xc);
    if (!hasContent(r))
        return mkfailure<value>(r);
    return value(mklist<value>(vto));
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {

    // Return the chat sender component lambda function
    const lvvlambda jid = car(params);
    const lvvlambda pass = cadr(params);
    const lvvlambda to = caddr(params);
    const lvvlambda msg = cadddr(params);
    const lvvlambda sender = [jid, pass, to, msg](const list<value>& params) -> const value {
        const tuscany::value func(car(params));
        if (func == "get")
            return post(jid, pass, to, msg, cdr(params));

        // Stop the chat sender component
        if (func != "stop")
            return mkfailure<value>();
        debug("chat::sender::stop");
        return failable<value>(value(lvvlambda()));
    };
    return value(sender);
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
