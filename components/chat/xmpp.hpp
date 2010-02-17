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
 * XMPP support functions.
 */

#include <apr_uuid.h>

#include "strophe.h"
extern "C" {
#include "common.h"
}
#include "string.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../../modules/scheme/eval.hpp"

namespace tuscany {
namespace chat {

/**
 * XMPP runtime, one per process.
 */
class XMPPRuntime {
public:
    XMPPRuntime() {
        xmpp_initialize();
        log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG);
    }

    ~XMPPRuntime() {
        xmpp_shutdown();
    }

private:
    friend class XMPPClient;
    xmpp_log_t* log;

} xmppRuntime;

/**
 * Represents an XMPP client.
 */
const string resourceUUID() {
    apr_uuid_t uuid;
    apr_uuid_get(&uuid);
    char buf[APR_UUID_FORMATTED_LENGTH];
    apr_uuid_format(buf, &uuid);
    return string(buf, APR_UUID_FORMATTED_LENGTH);
}

class XMPPClient {
public:
    XMPPClient(const string& jid, const string& pass, bool owner = true) : owner(owner), ctx(xmpp_ctx_new(NULL, xmppRuntime.log)), conn(xmpp_conn_new(ctx)), connecting(false), connected(false), disconnecting(false) {
        xmpp_conn_set_jid(conn, c_str(jid + "/" + resourceUUID()));
        xmpp_conn_set_pass(conn, c_str(pass));
    }

    XMPPClient(const XMPPClient& xc) : owner(false), ctx(xc.ctx), conn(xc.conn), listener(xc.listener),  connecting(xc.connecting), connected(xc.connected), disconnecting(xc.disconnecting) {
    }

    ~XMPPClient() {
        extern const failable<bool> disconnect(XMPPClient& xc);
        if (!owner)
            return;
        if (!disconnecting)
            disconnect(*this);
        xmpp_conn_release(conn);
        xmpp_ctx_free(ctx);
    }

private:
    friend int versionHandler(xmpp_conn_t* const conn, xmpp_stanza_t* const stanza, void* const udata);
    friend void connHandler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, const int err, xmpp_stream_error_t* const errstream, void *const udata);
    friend int messageHandler(xmpp_conn_t* const conn, xmpp_stanza_t* const stanza, void* const udata);
    friend const failable<bool> connect(XMPPClient& xc);
    friend const failable<int> send(const char* data, const int len, XMPPClient& xc);
    friend const failable<int> send(xmpp_stanza_t* const stanza, XMPPClient& xc);
    friend const failable<bool> post(const value& to, const value& val, XMPPClient& xc);
    friend const failable<bool> disconnect(XMPPClient& xc);
    friend const failable<bool> listen(const lambda<failable<bool>(const value&, const value&, XMPPClient&)>& listener, XMPPClient& xc);

    const bool owner;
    xmpp_ctx_t* ctx;
    xmpp_conn_t* conn;
    lambda<failable<bool>(const value&, const value&, XMPPClient&)> listener;
    bool connecting;
    bool connected;
    bool disconnecting;
};

/**
 * Make a text stanza.
 */
xmpp_stanza_t* textStanza(const char* text, xmpp_ctx_t* ctx) {
    xmpp_stanza_t* stanza = xmpp_stanza_new(ctx);
    xmpp_stanza_set_text(stanza, text);
    return stanza;
}

/**
 * Make a named stanza.
 */
xmpp_stanza_t* namedStanza(const char* ns, const char* name, xmpp_ctx_t* ctx) {
    xmpp_stanza_t* stanza = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(stanza, name);
    if (ns != NULL)
        xmpp_stanza_set_ns(stanza, ns);
    return stanza;
}

/**
 * Make a named stanza using a qualified name.
 */
xmpp_stanza_t* namedStanza(const char* name, xmpp_ctx_t* ctx) {
    xmpp_stanza_t* stanza = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(stanza, name);
    return stanza;
}

/**
 * XMPP version handler.
 */
int versionHandler(xmpp_conn_t* const conn, xmpp_stanza_t* const stanza, void* const udata) {
    XMPPClient& xc = *(XMPPClient*)udata;

    // Build version reply stanza
    xmpp_stanza_t* reply = namedStanza("iq", xc.ctx);
    xmpp_stanza_set_type(reply, "result");
    xmpp_stanza_set_id(reply, xmpp_stanza_get_id(stanza));
    xmpp_stanza_set_attribute(reply, "to", xmpp_stanza_get_attribute(stanza, "from"));
    xmpp_stanza_t* query = namedStanza(xmpp_stanza_get_ns(xmpp_stanza_get_children(stanza)), "query", xc.ctx);
    xmpp_stanza_add_child(reply, query);
    xmpp_stanza_t* name = namedStanza("name", xc.ctx);
    xmpp_stanza_add_child(query, name);
    xmpp_stanza_add_child(name, textStanza("Apache Tuscany", xc.ctx));
    xmpp_stanza_t* version = namedStanza("version", xc.ctx);
    xmpp_stanza_add_child(query, version);
    xmpp_stanza_add_child(version, textStanza("1.0", xc.ctx));

    // Send it
    xmpp_send(conn, reply);
    xmpp_stanza_release(reply);
    return 1;
}

/**
 * XMPP message handler
 */
int messageHandler(unused xmpp_conn_t* const conn, xmpp_stanza_t* const stanza, void* const udata) {
    // Ignore noise
    if(xmpp_stanza_get_child_by_name(stanza, "body") == NULL)
        return 1;
    if(!strcmp(xmpp_stanza_get_attribute(stanza, "type"), "error"))
        return 1;

    // Call the client listener function
    XMPPClient& xc = *(XMPPClient*)udata;
    const char* from = xmpp_stanza_get_attribute(stanza, "from");
    const char* text = xmpp_stanza_get_text(xmpp_stanza_get_child_by_name(stanza, "body"));
    if (isNil(xc.listener))
        return 1;
    const value val(scheme::readValue(text));
    debug(from, "chat::messageHandler::from");
    debug(val, "chat::messageHandler::body");
    const failable<bool> r = xc.listener(value(string(from)), val, xc);
    if (!hasContent(r) || !content(r)) {
        // Stop listening
        xc.listener = lambda<failable<bool>(const value&, const value&, XMPPClient&)>();
        return 0;
    }
    return 1;
}

/**
 * XMPP connection handler.
 */
void connHandler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, unused const int err, unused xmpp_stream_error_t* const errstream, void *const udata) {
    XMPPClient& xc = *(XMPPClient*)udata;
    xc.connecting = false;

    if (status == XMPP_CONN_CONNECT) {
        debug("chat::connHandler::connected");
        xmpp_handler_add(conn, versionHandler, "jabber:iq:version", "iq", NULL, &xc);

        // Send a <presence/> stanza so that we appear online to contacts
        xmpp_stanza_t* pres = xmpp_stanza_new(xc.ctx);
        xmpp_stanza_set_name(pres, "presence");
        xmpp_send(conn, pres);
        xmpp_stanza_release(pres);
        xc.connected = true;
        return;
    }

    debug("chat::connHandler::disconnected");
    xc.connected = false;
    if (xc.ctx->loop_status == XMPP_LOOP_RUNNING)
        xc.ctx->loop_status = XMPP_LOOP_QUIT;
}

/**
 * Connect to an XMPP server.
 */
const failable<bool> connect(XMPPClient& xc) {
    xc.connecting = true;
    xmpp_connect_client(xc.conn, NULL, 0, connHandler, &xc);
    while(xc.connecting)
        xmpp_run_once(xc.ctx, 20L);
    if (!xc.connected)
        return mkfailure<bool>("Couldn't connect to XMPP server");
    return true;
}

/**
 * Send a buffer on an XMPP session.
 */
const failable<int> send(const char* data, const int len, XMPPClient& xc) {
    if (len == 0)
        return 0;
    const int written = xc.conn->tls? tls_write(xc.conn->tls, data, len) : sock_write(xc.conn->sock, data, len);
    if (written < 0) {
        xc.conn->error = xc.conn->tls? tls_error(xc.conn->tls) : sock_error();
        return mkfailure<int>("Couldn't send stanza to XMPP server");
    }
    return send(data + written, len - written, xc);
}

/**
 * Send a string on an XMPP session.
 */
const failable<int> send(const string& data, XMPPClient& xc) {
    return send(c_str(data), length(data), xc);
}

/**
 * Send a stanza on an XMPP session.
 */
const failable<int> send(xmpp_stanza_t* const stanza, XMPPClient& xc) {
    char *buf;
    size_t len;
    const int rc = xmpp_stanza_to_text(stanza, &buf, &len);
     if (rc != 0)
         return mkfailure<int>("Couldn't convert stanza to text");
     const failable<int> r = send(buf, len, xc);
     if (!hasContent(r)) {
         xmpp_free(xc.conn->ctx, buf);
         return r;
     }
     xmpp_debug(xc.conn->ctx, "conn", "SENT: %s", buf);
     xmpp_free(xc.conn->ctx, buf);
     return content(r);
}

/**
 * Post a message to an XMPP jid.
 */
const failable<bool> post(const value& to, const value& val, XMPPClient& xc) {
    debug(to, "chat::post::to");
    debug(val, "chat::post::body");

    // Convert the value to a string
    const string vs(scheme::writeValue(val));

    // Build message stanza
    xmpp_stanza_t* stanza = namedStanza("message", xc.ctx);
    xmpp_stanza_set_type(stanza, "chat");
    xmpp_stanza_set_attribute(stanza, "to", c_str(string(to)));
    xmpp_stanza_t* body = namedStanza("body", xc.ctx);
    xmpp_stanza_add_child(stanza, body);
    xmpp_stanza_add_child(body, textStanza(c_str(vs), xc.ctx));

    // Send it
    const failable<int> r = send(stanza, xc);
    xmpp_stanza_release(stanza);
    if (!hasContent(r))
        return mkfailure<bool>(reason(r));
    return true;
}

/**
 * Disconnect an XMPP session.
 */
const failable<bool> disconnect(XMPPClient& xc) {
    xc.disconnecting = true;
    const failable<int> r = send("</stream:stream>", xc);
    if (!hasContent(r))
        return mkfailure<bool>(reason(r));
    return true;
}

/**
 * Listen to messages received by an XMPP client.
 */
const failable<bool> listen(const lambda<failable<bool>(const value&, const value&, XMPPClient&)>& listener, XMPPClient& xc) {
    debug("chat::listen");
    xc.listener = listener;
    xmpp_handler_add(xc.conn, messageHandler, NULL, "message", NULL, &xc);
    while(xc.connected && !isNil(xc.listener))
        xmpp_run_once(xc.ctx, 1000L);
    return true;
}

}
}

#endif /* tuscany_xmpp_hpp */
