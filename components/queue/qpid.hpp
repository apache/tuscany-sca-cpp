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

#ifndef tuscany_qpid_hpp
#define tuscany_qpid_hpp

/**
 * AMQP queue access functions.
 */

// Ignore conversion issues and redundant declarations in Qpid headers
#ifdef WANT_MAINTAINER_WARNINGS
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif

#include <qpid/client/Connection.h>
#include <qpid/client/Session.h>
#include <qpid/client/MessageListener.h>
#include <qpid/client/SubscriptionManager.h>

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace queue {

/**
 * Represents a Qpid connection.
 */
class QpidConnection {
public:
    QpidConnection() : owner(true) {
        debug("queue::qpidonnection");
        c.open("localhost", 5672);
    }

    QpidConnection(const bool owner) : owner(owner) {
        debug("queue::qpidonnection");
        c.open("localhost", 5672);
    }

    QpidConnection(const QpidConnection& qc) : owner(false), c(qc.c) {
        debug("queue::qpidonnection::copy");
    }

    QpidConnection& operator=(const QpidConnection& qc) = delete;

    ~QpidConnection() {
        debug("queue::~qpidonnection");
        if (!owner)
            return;
        c.close();
    }

private:
    friend const failable<bool> close(QpidConnection& qc);
    friend class QpidSession;

    const bool owner;
    qpid::client::Connection c;

};

/**
 * Close a Qpid connection.
 */
const failable<bool> close(QpidConnection& qc) {
    qc.c.close();
    return true;
}

/**
 * Represents a Qpid session.
 */
class QpidSession {
public:
    QpidSession(QpidConnection& qc) : owner(true), s(qc.c.newSession()) {
        debug("queue::qpidsession");
    }

    QpidSession(QpidConnection& qc, const bool owner) : owner(owner), s(qc.c.newSession()) {
        debug("queue::qpidsession");
    }

    QpidSession(const QpidSession& qs) : owner(false), s(qs.s) {
        debug("queue::qpidsession::copy");
    }

    ~QpidSession() {
        debug("queue::~qpidsession");
        if (!owner)
            return;
        s.close();
    }

private:
    friend const failable<bool> close(QpidSession& qs);
    friend const failable<bool> declareQueue(const value& key, const string& name, QpidSession& qs);
    friend const failable<bool> post(const value& key, const value& val, QpidSession& qs);
    friend class QpidSubscription;

    const bool owner;
    qpid::client::Session s;
};

/**
 * Close a Qpid session.
 */
const failable<bool> close(QpidSession& qs) {
    try {
        qs.s.close();
    } catch (const qpid::Exception& e) {
        return mkfailure<bool>(string("Qpid failure: ") + e.what());
    }
    return true;
}

/**
 * Declare a key / AMQP queue pair.
 */
const failable<bool> declareQueue(const value& key, const string& name, QpidSession& qs) {
    const string ks(write(content(scheme::writeValue(key))));
    try {
        qs.s.queueDeclare(qpid::client::arg::queue=c_str(name));
        qs.s.exchangeBind(qpid::client::arg::exchange="amq.direct", qpid::client::arg::queue=c_str(name), qpid::client::arg::bindingKey=c_str(ks));
    } catch (const qpid::Exception& e) {
        return mkfailure<bool>(string("Qpid failure: ") + e.what());
    }
    return true;
}

/**
 * Post a key / value pair message to an AMQP broker.
 */
const failable<bool> post(const value& key, const value& val, QpidSession& qs) {

    // Send in a message with the given key.
    const string ks(write(content(scheme::writeValue(key))));
    const string vs(write(content(scheme::writeValue(val))));
    try {
        qpid::client::Message message;
        message.getDeliveryProperties().setRoutingKey(c_str(ks));
        message.setData(c_str(vs));
        qs.s.messageTransfer(qpid::client::arg::content=message,  qpid::client::arg::destination="amq.direct");
    } catch (const qpid::Exception& e) {
        return mkfailure<bool>(string("Qpid failure: ") + e.what());
    }
    return true;
}

/**
 * Represents a Qpid subscription.
 */
class QpidSubscription {
public:
    QpidSubscription(QpidSession& qs) : owner(true), subs(qs.s) {
    }

    QpidSubscription(QpidSession& qs, const bool owner) : owner(owner), subs(qs.s) {
    }

    QpidSubscription(const QpidSubscription& qsub) : owner(false), subs(qsub.subs) {
    }

    ~QpidSubscription() {
        if (!owner)
            return;
        try {
            subs.stop();
        } catch (const qpid::Exception& e) {
            mkfailure<bool>(string("Qpid failure: ") + e.what());
        }
    }

private:
    friend const failable<bool> listen(const string& name, const lambda<const bool(const value&, const value&)>& l, QpidSubscription& qsub);
    friend const failable<bool> stop(QpidSubscription& qsub);

    const bool owner;
    qpid::client::SubscriptionManager subs;
};

/**
 * Register a listener function with an AMQP queue.
 */
class Listener : public qpid::client::MessageListener {
public:
    Listener(const lambda<const bool(const value&, const value&)> l, qpid::client::SubscriptionManager& subs) : l(l), subs(subs) {
    }

    virtual void received(qpid::client::Message& msg) {

        // Call the listener function
        const value k(content(scheme::readValue(msg.getDeliveryProperties().getRoutingKey().c_str())));
        const value v(content(scheme::readValue(msg.getData().c_str())));
        const bool r = l(k, v);
        if (!r) {
            try {
                subs.cancel(msg.getDestination());
            } catch (const qpid::Exception& e) {
                mkfailure<bool>(string("Qpid failure: ") + e.what());
            }
        }
    }

private:
    const lambda<const bool(const value&, const value&)> l;
    qpid::client::SubscriptionManager& subs;
};


const failable<bool> listen(const string& name, const lambda<const bool(const value&, const value&)>& l, QpidSubscription& qsub) {
    debug("queue::listen");
    Listener listener(l, qsub.subs);
    try {
        qsub.subs.subscribe(listener, c_str(name));
        qsub.subs.run();
    } catch (const qpid::Exception& e) {
        return mkfailure<bool>(string("Qpid failure: ") + e.what());
    }
    debug("queue::listen::stopped");
    return true;
}

/**
 * Stop an AMQP subscription.
 */
const failable<bool> stop(QpidSubscription& qsub) {
    debug("queue::stop");
    try {
        qsub.subs.stop();
    } catch (const qpid::Exception& e) {
        return mkfailure<bool>(string("Qpid failure: ") + e.what());
    }
    debug("queue::stopped");
    return true;
}

}
}

// Re-enable conversion and redundant declarations warnings
#ifdef WANT_MAINTAINER_WARNINGS
#pragma GCC diagnostic warning "-Wconversion"
#pragma GCC diagnostic warning "-Wredundant-decls"
#endif

#endif /* tuscany_qpid_hpp */
