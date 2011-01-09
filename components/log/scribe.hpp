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

#ifndef tuscany_scribe_hpp
#define tuscany_scribe_hpp

/**
 * Scribe logging functions.
 */

// Work around redundant defines in Scribe headers
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef VERSION

// Ignore integer conversion issues in Thrift and Scribe headers
#ifdef WANT_MAINTAINER_MODE
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "gen-cpp/scribe.h"

#ifdef WANT_MAINTAINER_MODE
#pragma GCC diagnostic warning "-Wconversion"
#endif

#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace scribe {

/**
 * Represents a Scribe connection.
 */
class Scribe {
public:
    Scribe() : owner(false) {
        debug("scribe::scribe");
    }

    Scribe(const string host, const int port) : owner(true) {
        debug(host, "scribe::scribe::host");
        debug(port, "scribe::scribe::port");
        init(host, port);
    }

    Scribe(const Scribe& c) : owner(false) {
        debug("scribe::scribe::copy");
        client = c.client;
        transport = c.transport;
    }

    ~Scribe() {
        debug("scribe::~scribe");
        if (!owner)
            return;
        try {
            transport->close();
            delete client;
        } catch (const std::exception& e) {
            mkfailure<bool>(e.what());
        }
    }

private:
    bool owner;
    ::scribe::thrift::scribeClient* client;
    boost::shared_ptr<apache::thrift::transport::TTransport> transport;

    friend const failable<bool> log(const value& val, const value& category, const Scribe& sc);

    /**
     * Initialize the Scribe connection.
     */
    const failable<bool> init(const string& host, const int port) {
        try {
            boost::shared_ptr<apache::thrift::transport::TTransport> sock(new apache::thrift::transport::TSocket(c_str(host), port));
            boost::shared_ptr<apache::thrift::transport::TFramedTransport> framedSock(new apache::thrift::transport::TFramedTransport(sock));
            transport = framedSock;
            boost::shared_ptr<apache::thrift::protocol::TProtocol> proto(new apache::thrift::protocol::TBinaryProtocol(transport));
            client = new ::scribe::thrift::scribeClient(proto);
            transport->open();
            return true;
        } catch (const std::exception& e) {
            return mkfailure<bool>(e.what());
        }
    }
};

/**
 * Log an item.
 */
const failable<bool> log(const value& val, const value& category, const Scribe& sc) {
    debug(val, "scribe::log::value");
    debug(category, "scribe::log::category");

    const value cat = isString(category)? value(c_str(category)):category;
    const string cs(scheme::writeValue(cat));
    const string vs(scheme::writeValue(val));

    try {
        ::scribe::thrift::LogEntry entry;
        entry.category = c_str(cs);
        entry.message = c_str(vs);
        std::vector< ::scribe::thrift::LogEntry> msgs;
        msgs.push_back(entry);

        int result = sc.client->Log(msgs);
        if (result != ::scribe::thrift::OK)
            return mkfailure<bool>("Could not log value, retry later");
    } catch (const std::exception& e) {
        return mkfailure<bool>(e.what());
    }

    debug(true, "scribe::log::result");
    return true;
}

}
}

#endif /* tuscany_scribe_hpp */
