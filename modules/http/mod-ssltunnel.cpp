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
 * HTTPD module used to tunnel traffic over an HTTPS connection.
 */

#include <sys/stat.h>

#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "httpd.hpp"
#include "http.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_ssltunnel;
}

namespace tuscany {
namespace httpd {
namespace modssltunnel {

/**
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf(apr_pool_t* p, server_rec* s) : p(p), server(s) {
    }

    const gc_pool p;
    server_rec* server;
    string pass;
    string host;
    string path;
    string ca;
    string cert;
    string key;
};

extern "C" {
extern module AP_DECLARE_DATA core_module;
}

/**
 * Process the module configuration.
 */
int M_SSLTUNNEL;
int postConfigMerge(ServerConf& mainsc, apr_pool_t* p, server_rec* s) {
    if (s == NULL)
        return OK;
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_ssltunnel);
    debug(httpd::serverName(s), "modwiring::postConfigMerge::serverName");

    // Merge configuration from main server
    if (length(sc.ca) == 0 && length(mainsc.ca) !=0)
        sc.ca = mainsc.ca;
    if (length(sc.cert) == 0 && length(mainsc.cert) !=0)
        sc.cert = mainsc.cert;
    if (length(sc.key) == 0 && length(mainsc.key) !=0)
        sc.key = mainsc.key;

    // Parse the configured TunnelPass URI
    if (length(sc.pass) != 0) {
        apr_uri_t uri;
        apr_status_t prc = apr_uri_parse(p, c_str(sc.pass), &uri);
        if (prc != APR_SUCCESS) {
            mkfailure<int>("Couldn't parse TunnelPass: " + sc.pass + ", " + http::apreason(prc));
            return prc;
        }
        sc.host = uri.hostname;
        sc.path = uri.path;
    }
    return postConfigMerge(mainsc, p, s->next);
}

int postConfig(apr_pool_t* p, unused apr_pool_t* plog, unused apr_pool_t* ptemp, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_ssltunnel);
    debug(httpd::serverName(s), "modwiring::postConfig::serverName");

    // Register the SSLTUNNEL method
    M_SSLTUNNEL = ap_method_register(p, "SSLTUNNEL");

    // Merge and process server configurations
    return postConfigMerge(sc, p, s);
}

/**
 * Close a connection.
 */
const int close(conn_rec* conn, apr_socket_t* csock) {
    debug("modssltunnel::close");
    apr_socket_close(csock);
    conn->aborted = 1;
    return OK;
}

/**
 * Abort a connection.
 */
const int abort(conn_rec* conn, apr_socket_t* csock, const string& reason) {
    debug("modssltunnel::abort");
    apr_socket_close(csock);
    conn->aborted = 1;
    return httpd::reportStatus(mkfailure<int>(reason));
}

/**
 * Tunnel traffic from a client connection to a target URL.
 */
int tunnel(conn_rec* conn, const string& ca, const string& cert, const string& key, const string& url, const string& preamble, const gc_pool& p, unused ap_filter_t* ifilter, ap_filter_t* ofilter) {

    // Create input/output bucket brigades
    apr_bucket_brigade* ib = apr_brigade_create(pool(p), conn->bucket_alloc);
    apr_bucket_brigade* ob = apr_brigade_create(pool(p), conn->bucket_alloc);

    // Get client connection socket
    apr_socket_t* csock = (apr_socket_t*)ap_get_module_config(conn->conn_config, &core_module);

    // Open connection to target
    http::CURLSession cs(ca, cert, key);
    const failable<bool> crc = http::connect(url, cs);
    if (!hasContent(crc))
        return abort(conn, csock, reason(crc));
    apr_socket_t* tsock = http::sock(cs);

    // Send preamble
    if (length(preamble) != 0) {
        debug(preamble, "modssltunnel::tunnel::sendPreambleToTarget");
        const failable<bool> src = http::send(c_str(preamble), length(preamble), cs);
        if (!hasContent(src))
            return abort(conn, csock, string("Couldn't send to target: ") + reason(src));
    }

    // Create a pollset for the client and target sockets
    apr_pollset_t* pollset;
    apr_status_t cprc = apr_pollset_create(&pollset, 2, pool(p), 0);
    if (cprc != APR_SUCCESS)
        return abort(conn, csock, http::apreason(cprc));
    const apr_pollfd_t* cpollfd = http::pollfd(csock, APR_POLLIN, p);
    apr_pollset_add(pollset, cpollfd);
    const apr_pollfd_t* tpollfd = http::pollfd(tsock, APR_POLLIN, p);
    apr_pollset_add(pollset, tpollfd);

    // Relay traffic in both directions until end of stream
    const apr_pollfd_t* pollfds = cpollfd;
    apr_int32_t pollcount = 1;
    for(;;) {
        for (; pollcount > 0; pollcount--, pollfds++) {
            if (pollfds->rtnevents & APR_POLLIN) {
                if (pollfds->desc.s == csock) {

                    // Receive buckets from client
                    const apr_status_t getrc = ap_get_brigade(conn->input_filters, ib, AP_MODE_READBYTES, APR_BLOCK_READ, HUGE_STRING_LEN);
                    if (getrc != APR_SUCCESS)
                        return abort(conn, csock, string("Couldn't receive from client"));

                    for (apr_bucket* bucket = APR_BRIGADE_FIRST(ib); bucket != APR_BRIGADE_SENTINEL(ib); bucket = APR_BUCKET_NEXT(bucket)) {
                        if (APR_BUCKET_IS_FLUSH(bucket))
                            continue;

                        // Client connection closed
                        if (APR_BUCKET_IS_EOS(bucket))
                            return close(conn, csock);

                        const char *data;
                        apr_size_t rl;
                        apr_bucket_read(bucket, &data, &rl, APR_BLOCK_READ);
                        if (rl > 0) {
                            debug(string(data, rl), "modssltunnel::tunnel::sendToTarget");

                            // Send to target
                            const failable<bool> src = http::send(data, rl, cs);
                            if (!hasContent(src))
                                return abort(conn, csock, string("Couldn't send to target: ") + reason(src));
                        }
                    }
                    apr_brigade_cleanup(ib);
                } else {

                    // Receive from target
                    char data[8192];
                    const failable<size_t> frl = http::recv(data, sizeof(data), cs);
                    if (!hasContent(frl))
                        return abort(conn, csock, string("Couldn't receive from target") + reason(frl));
                    const size_t rl = content(frl);

                    // Target connection closed
                    if (rl == 0)
                        return close(conn, csock);

                    // Send bucket to client
                    debug(string(data, rl), "modssltunnel::tunnel::sendToClient");
                    APR_BRIGADE_INSERT_TAIL(ob, apr_bucket_transient_create(data, rl, conn->bucket_alloc));
                    APR_BRIGADE_INSERT_TAIL(ob, apr_bucket_flush_create(conn->bucket_alloc));
                    if (ap_pass_brigade(ofilter, ob) != APR_SUCCESS)
                        return abort(conn, csock, "Couldn't send data bucket to client");
                    apr_brigade_cleanup(ob);
                }
            }

            // Error
            if (pollfds->rtnevents & (APR_POLLERR | APR_POLLHUP | APR_POLLNVAL)) {
                if (pollfds->desc.s == csock) 
                    return abort(conn, csock, "Couldn't receive from client");
                else
                    return abort(conn, csock, "Couldn't receive from target");
            }
        }

        // Poll the client and target sockets
        debug("modssltunnel::tunnel::poll");
        apr_status_t pollrc = apr_pollset_poll(pollset, -1, &pollcount, &pollfds);
        if (pollrc != APR_SUCCESS)
            return abort(conn, csock, "Couldn't poll sockets");
        debug(pollcount, "modssltunnel::tunnel::pollfds");
    }

    // Close client connection
    return close(conn, csock);
}

/**
 * Return the first connection filter in a list of filters.
 */
ap_filter_t* connectionFilter(ap_filter_t* f) {
    if (f == NULL)
        return f;
    if (f->frec->ftype < AP_FTYPE_CONNECTION)
        return connectionFilter(f->next);
    return f;
}

/**
 * Process a client connection and relay it to a tunnel.
 */
int processConnection(conn_rec *conn) {
    // Only allow configured virtual hosts
    if (!conn->base_server->is_virtual)
        return DECLINED;
    if (ap_get_module_config(conn->base_server->module_config, &mod_tuscany_ssltunnel) == NULL)
        return DECLINED;

    // Create a scoped memory pool
    gc_scoped_pool pool;

    const ServerConf& sc = httpd::serverConf<ServerConf>(conn->base_server, &mod_tuscany_ssltunnel);
    if (length(sc.pass) == 0)
        return DECLINED;
    debug(sc.pass, "modssltunnel::processConnection::pass");

    // Run the tunnel
    const string preamble = string("SSLTUNNEL ") + sc.path + string(" HTTP/1.1\r\nHost: ") + sc.host + string("\r\n\r\n");
    debug(preamble, "modssltunnel::processConnection::preamble");
    return tunnel(conn, sc.ca, sc.cert, sc.key, sc.pass, preamble, gc_pool(conn->pool), connectionFilter(conn->input_filters), connectionFilter(conn->output_filters));
}

/**
 * Tunnel a SSLTUNNEL request to a target host/port.
 */
int handler(request_rec* r) {
    if (r->method_number != M_SSLTUNNEL)
        return DECLINED;

    // Only allow HTTPS
    if (strcmp(r->server->server_scheme, "https"))
        return DECLINED;

    // Create a scoped memory pool
    gc_scoped_pool pool(r->pool);

    // Build the target URL
    debug(r->uri, "modssltunnel::handler::uri");
    const list<value> path(pathValues(r->uri));
    const string url = string(cadr(path)) + ":" + caddr(path);
    debug(url, "modssltunnel::handler::target");

    // Run the tunnel
    return tunnel(r->connection, "", "", "", url, "", gc_pool(r->pool), connectionFilter(r->proto_input_filters), connectionFilter(r->proto_output_filters));
}

/**
 * Configuration commands.
 */
const char* confTunnelPass(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_ssltunnel);
    sc.pass = arg;
    return NULL;
}
const char* confCAFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_ssltunnel);
    sc.ca = arg;
    return NULL;
}
const char* confCertFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_ssltunnel);
    sc.cert = arg;
    return NULL;
}
const char* confCertKeyFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_ssltunnel);
    sc.key = arg;
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE1("TunnelPass", (const char*(*)())confTunnelPass, NULL, RSRC_CONF, "Tunnel server name"),
    AP_INIT_TAKE1("TunnelSSLCACertificateFile", (const char*(*)())confCAFile, NULL, RSRC_CONF, "Tunnel SSL CA certificate file"),
    AP_INIT_TAKE1("TunnelSSLCertificateFile", (const char*(*)())confCertFile, NULL, RSRC_CONF, "Tunnel SSL certificate file"),
    AP_INIT_TAKE1("TunnelSSLCertificateKeyFile", (const char*(*)())confCertKeyFile, NULL, RSRC_CONF, "Tunnel SSL certificate key file"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};

void registerHooks(unused apr_pool_t *p) {
    ap_hook_post_config(postConfig, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_process_connection(processConnection, NULL, NULL, APR_HOOK_MIDDLE);
}

}
}
}

extern "C" {

module AP_MODULE_DECLARE_DATA mod_tuscany_ssltunnel = {
    STANDARD20_MODULE_STUFF,
    // dir config and merger
    NULL, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::httpd::modssltunnel::ServerConf>, NULL,
    // commands and hooks
    tuscany::httpd::modssltunnel::commands, tuscany::httpd::modssltunnel::registerHooks
};

}
