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
const failable<value> post(const lambda<value(const list<value>&)> jid, const lambda<value(const list<value>&)> pass, const lambda<value(const list<value>&)> to, const lambda<value(const list<value>&)> msg, const list<value>& params) {

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
        return mkfailure<value>(reason(c));

    // Post the message
    const failable<bool> r = post(vto, vmsg, xc);
    if (!hasContent(r))
        return mkfailure<value>(reason(r));
    return value(mklist<value>(vto));
}

/**
 * Chat sender component lambda function
 */
class chatSender {
public:
    chatSender(const lambda<value(const list<value>&)> jid, const lambda<value(const list<value>&)> pass, const lambda<value(const list<value>&)> to, const lambda<value(const list<value>&)> msg) : jid(jid), pass(pass), to(to), msg(msg) {
    }

    const value operator()(const list<value>& params) const {
        const tuscany::value func(car(params));
        if (func == "get")
            return post(jid, pass, to, msg, cdr(params));

        // Stop the chat sender component
        if (func != "stop")
            return tuscany::mkfailure<tuscany::value>();
        debug("chat::sender::stop");
        return failable<value>(value(lambda<value(const list<value>&)>()));
    }

private:
    const lambda<value(const list<value>&)> jid;
    const lambda<value(const list<value>&)> pass;
    const lambda<value(const list<value>&)> to;
    const lambda<value(const list<value>&)> msg;
};

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {

    // Return the chat sender component lambda function
    return value(lambda<value(const list<value>&)>(chatSender(car(params), cadr(params), caddr(params), cadddr(params))));
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
