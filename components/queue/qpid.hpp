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

#ifndef tuscany_queue_hpp
#define tuscany_queue_hpp

/**
 * AMQP queue access functions.
 */

// Ignore conversion issues and redundant declarations in Qpid headers
#ifdef WANT_MAINTAINER_MODE
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif

#include <qpid/client/Connection.h>
#include <qpid/client/Session.h>

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
        c.open("localhost", 5672);
    }

    QpidConnection(const QpidConnection& qc) : owner(false), c(qc.c) {
    }

    ~QpidConnection() {
        if (!owner)
            return;
        c.close();
    }

private:
    friend class QpidSession;

    const bool owner;
    qpid::client::Connection c;

};

/**
 * Represents a Qpid session.
 */
class QpidSession {
public:
    QpidSession(QpidConnection& qc) : owner(true), s(qc.c.newSession()) {
    }

    QpidSession(const QpidSession& qs) : owner(false), s(qs.s) {
    }

    ~QpidSession() {
        if (!owner)
            return;
        s.close();
    }

private:
    friend qpid::client::Session session(const QpidSession& qs);

    const bool owner;
    qpid::client::Session s;
};

qpid::client::Session session(const QpidSession& qs) {
    return qs.s;
}

/**
 * Declare a key / AMQP queue pair.
 */
const failable<bool> declareQueue(const string& key, const string& name, QpidSession& qs) {
    session(qs).queueDeclare(qpid::client::arg::queue=c_str(name));
    session(qs).exchangeBind(qpid::client::arg::exchange="amq.direct", qpid::client::arg::queue=c_str(name), qpid::client::arg::bindingKey=c_str(key));
    return true;
}

/**
 * Post a key / value pair in message to an AMQP broker.
 */
const failable<bool> post(const string& key, const value& val, QpidSession& qs) {

    // Convert the value to a string
    const string vs(scheme::writeValue(val));

    // Send in a message with the given key.
    qpid::client::Message message;
    message.getDeliveryProperties().setRoutingKey(c_str(key));
    message.setData(c_str(vs));
    session(qs).messageTransfer(qpid::client::arg::content=message,  qpid::client::arg::destination="amq.direct");
    return true;
}

}
}

// Re-enable conversion and redundant declarations warnings
#ifdef WANT_MAINTAINER_MODE
#pragma GCC diagnostic warning "-Wconversion"
#pragma GCC diagnostic warning "-Wredundant-decls"
#endif

#endif /* tuscany_qpid_hpp */
