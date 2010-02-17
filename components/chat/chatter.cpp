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
 * XMPP chatter component implementation.
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
        return mkfailure<value>(reason(r));
    return value(mklist<value>(to));
}

/**
 * A relay function that posts the XMPP messages it receives to a relay component reference.
 */
class relay {
public:
    relay(const lambda<value(const list<value>&)>& rel) : rel(rel) {
    }

    const failable<bool> operator()(const value& jid, const value& val, unused XMPPClient& xc) const {
        if (isNil(rel))
            return true;
        debug(jid, "chat::relay::jid");
        debug(val, "chat::relay::value");
        const value res = rel(mklist<value>("post", mklist<value>(jid), val));
        return true;
    }

private:
    const lambda<value(const list<value>&)> rel;
};

/**
 * Subscribe and listen to an XMPP session.
 */
class subscribe {
public:
    subscribe(const lambda<failable<bool>(const value&, const value&, XMPPClient&)>& l, XMPPClient& xc) : l(l), xc(xc) {
    }

    const failable<bool> operator()() const {
        gc_pool pool;
        debug("chat::subscribe::listen");
        const failable<bool> r = listen(l, const_cast<XMPPClient&>(xc));
        debug("chat::subscribe::stopped");
        return r;
    }

private:
    const lambda<failable<bool>(const value&, const value&, XMPPClient&)> l;
    XMPPClient xc;
};

/**
 * Chatter component lambda function
 */
class chatter {
public:
    chatter(XMPPClient& xc, worker& w) : xc(xc), w(w) {
    }

    const value operator()(const list<value>& params) const {
        const tuscany::value func(car(params));
        if (func == "post")
            return post(cdr(params), const_cast<XMPPClient&>(xc));

        // Stop the chatter component
        if (func != "stop")
            return tuscany::mkfailure<tuscany::value>();
        debug("chat::chatter::stop");

        // Disconnect and shutdown the worker thread
        disconnect(const_cast<XMPPClient&>(xc));
        shutdown(const_cast<worker&>(w));
        debug("chat::chatter::stopped");

        return failable<value>(value(lambda<value(const list<value>&)>()));
    }

private:
    const XMPPClient xc;
    worker w;
};

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Extract the relay reference and the XMPP JID and password
    const bool hasRelay = !isNil(cddr(params));
    const value rel = hasRelay? car(params) : value(lambda<value(const list<value>&)>());
    const list<value> props = hasRelay? cdr(params) : params;
    const value jid = ((lambda<value(list<value>)>)car(props))(list<value>());
    const value pass = ((lambda<value(list<value>)>)cadr(props))(list<value>());

    // Create an XMPP client session
    XMPPClient xc(jid, pass, false);
    const failable<bool> r = connect(xc);
    if (!hasContent(r))
        return mkfailure<value>(reason(r));

    // Listen and relay messages in a worker thread
    worker w(3);
    const lambda<failable<bool>(const value&, const value&, XMPPClient&)> rl = relay(rel);
    submit<failable<bool> >(w, lambda<failable<bool>()>(subscribe(rl, xc)));

    // Return the chatter component lambda function
    return value(lambda<value(const list<value>&)>(chatter(xc, w)));
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start" || func == "restart")
        return tuscany::chat::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
