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
 * Tuscany HTTPD server module.
 */

#include <string>
#include <iostream>
#include <sstream>

#include "apr_strings.h"
#include "apr_fnmatch.h"
#include "apr_lib.h"
#define APR_WANT_STRFUNC
#include "apr_want.h"
#include "ap_config.h"

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_request.h"
#include "http_protocol.h"
#include "http_log.h"
#include "http_main.h"
#include "util_script.h"
#include "util_md5.h"

#include "mod_core.h"

#include "list.hpp"
#include "slist.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany;
}

namespace tuscany {

/**
 * Server configuration.
 */
struct ServerConf {
    const char* home;
};

/**
 * Directory configuration.
 */
struct DirConf {
    const char* root;
    const char* path;
    const char* uri;
    const char* component;
};

/**
 * Returns an HTTP request path as a list of strings.
 */
const list<std::string> path(const request_rec* r) {
    const char* p = r->path_info;
    if (p == NULL || p[0] == '\0')
        return list<std::string>();
    return tokenize("/", p + 1);
}

/**
 * Returns an HTTP query string as a list of lists of strings.
 */
const list<list<std::string> > args(const request_rec* r) {
    const char* a = r->args;
    if (a == NULL)
        return list<list<std::string> >();
    const lambda<list<std::string>(std::string, std::string)> tok(tokenize);
    return map(curry(tok, std::string("=")), tokenize("&", a));
}

/**
 * Log HTTP request info to standard out for now, for debugging purposes.
 */
bool logRequests = true;

int logHeader(void* r, const char* key, const char* value) {
    std::cout << "header key: " << key << ", value: " << value << std::endl;
    return 1;
}

const char* optional(const char* s) {
    if (s == NULL)
        return "(null)";
    return s;
}

const bool logRequest(request_rec* r, const ServerConf& sc, const DirConf& dc) {
    std::cout << "mod-tuscany..." << std::endl;
    std::cout << "tuscany home: " << sc.home << std::endl;
    std::cout << "tuscany root: " << dc.root << std::endl;
    std::cout << "tuscany path: " << dc.path << std::endl;
    std::cout << "component: " << dc.component << std::endl;
    std::cout << "protocol: " << optional(r->protocol) << std::endl;
    std::cout << "method: " << optional(r->method) << std::endl;
    std::cout << "method number: " << r->method_number << std::endl;
    std::cout << "content type: " << optional(apr_table_get(r->headers_in, "Content-Type")) << std::endl;
    std::cout << "content encoding: " << optional(r->content_encoding) << std::endl;
    apr_table_do(logHeader, r, r->headers_in, NULL);
    std::cout << "uri: " << optional(r->uri) << std::endl;
    std::cout << "path info: " << optional(r->path_info) << std::endl;
    std::cout << "path: " << path(r) << std::endl;
    std::cout << "args info: " << optional(r->args) << std::endl;
    std::cout << "args: " << args(r) << std::endl;
    return true;
}

/**
 * Handle an HTTP GET request.
 */
const int get(request_rec* r) {
    std::string str("<result>OK</result>");
    if (false) {
        r->status = HTTP_NOT_FOUND;
        return OK;
    }

    // Handle a conditional GET
    std::string etag(ap_md5(r->pool, (const unsigned char*)str.c_str()));
    const char* match = apr_table_get(r->headers_in, "If-None-Match");
    if (match != NULL && etag == match) {
        r->status = HTTP_NOT_MODIFIED;
        return OK;
    }

    // Send response
    ap_set_content_type(r, "text/xml");
    apr_table_setn(r->headers_out, "ETag", etag.c_str());
    ap_rputs(str.c_str(), r);

    return OK;
}

/**
 * Handle an HTTP POST request.
 */
const int post(request_rec* r) {
    return OK;
}

/**
 * Handle an HTTP PUT request.
 */
const int put(request_rec* r) {
    std::ostringstream sos;
    char buffer[2049];
    for ( ; ; )
    {
        int size = ap_get_client_block(r, buffer, 2048);
        if (size > 0)
        {
            buffer[size] = '\0';
            sos << buffer;
        }
        else if (size == 0)
        {
            break;
        }
        else if (size < 0)
        {
            return HTTP_INTERNAL_SERVER_ERROR;
        }
    }
    std::string input = sos.str();
    return OK;
}

/**
 * Handle an HTTP DELETE request.
 */
const int del(request_rec* r) {
    return OK;
}

/**
 * HTTP handler entry point.
 */
int handler(request_rec *r) {
    if(strcmp(r->handler, "mod_tuscany"))
        return DECLINED;

    // Get the server and dir config
    ServerConf& serverConf = *(ServerConf*)ap_get_module_config(r->server->module_config, &mod_tuscany);
    DirConf& dirConf = *(DirConf*)ap_get_module_config(r->per_dir_config, &mod_tuscany);

    // Log the request
    if(logRequests)
        logRequest(r, serverConf, dirConf);

    // Set up the read policy
    const int rc = ap_setup_client_block(r, REQUEST_CHUNKED_DECHUNK);
    if(rc != OK)
        return rc;
    ap_should_client_block(r);
    if(r->read_chunked == true && r->remaining == 0)
        r->chunked = true;
    apr_table_setn(r->headers_out, "Connection", "close");

    if (r->header_only)
         return OK;

    // Handle HTTP method
    if(r->method_number == M_GET)
        return get(r);
    if(r->method_number == M_POST)
        return post(r);
    if(r->method_number == M_PUT)
        return put(r);
    if(r->method_number == M_DELETE)
        return del(r);
    return HTTP_NOT_IMPLEMENTED;
}

/**
 * Configuration commands.
 */
const char *confHome(cmd_parms *cmd, void *dummy, const char *arg) {
    ServerConf *conf = (ServerConf*)ap_get_module_config(cmd->server->module_config, &mod_tuscany);
    conf->home = apr_pstrdup(cmd->pool, arg);
    return NULL;
}
const char *confPath(cmd_parms *cmd, void *c, const char *arg) {
    DirConf *conf = (DirConf*)c;
    conf->path = apr_pstrdup(cmd->pool, arg);
    return NULL;
}
const char *confRoot(cmd_parms *cmd, void *c, const char *arg) {
    DirConf *conf = (DirConf*)c;
    conf->root = apr_pstrdup(cmd->pool, arg);
    return NULL;
}
const char *confURI(cmd_parms *cmd, void *c, const char *arg) {
    DirConf *conf = (DirConf*)c;
    conf->uri = apr_pstrdup(cmd->pool, arg);
    return NULL;
}
const char *confComponent(cmd_parms *cmd, void *c, const char *arg) {
    DirConf *conf = (DirConf*)c;
    conf->component = apr_pstrdup(cmd->pool, arg);
    return NULL;
}
void *makeDirConf(apr_pool_t *p, char *dirspec) {
    DirConf* conf = (DirConf*)apr_palloc(p, sizeof(*conf));
    conf->path = "";
    conf->root = "";
    conf->uri = "";
    conf->component = "";
    return conf;
}
void* makeServerConf(apr_pool_t *p, server_rec *s) {
    ServerConf* conf = (ServerConf* )apr_palloc(p, sizeof(*conf));
    conf->home = "";
    return conf;
}

/**
 * HTTP server module declarations.
 */
const command_rec commands[] = {
    AP_INIT_TAKE1("home", (const char*(*)())confHome, NULL, RSRC_CONF, "Tuscany home directory"),
    AP_INIT_TAKE1("path", (const char*(*)())confPath, NULL, ACCESS_CONF, "Tuscany SCA composite search path"),
    AP_INIT_TAKE1("root", (const char*(*)())confRoot, NULL, ACCESS_CONF, "Tuscany root SCA configuration path"),
    AP_INIT_TAKE1("uri", (const char*(*)())confURI, NULL, ACCESS_CONF, "Tuscany SCA system base URI"),
    AP_INIT_TAKE1("component", (const char*(*)())confComponent, NULL, ACCESS_CONF, "SCA component name"),
    {NULL}
};

int init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *s) {
    return OK;
}

void childInit(apr_pool_t* p, server_rec* svr_rec) {
    ServerConf *conf = (ServerConf*)ap_get_module_config(svr_rec->module_config, &mod_tuscany);
    if(conf == NULL) {
        std::cerr << "[Tuscany] Due to one or more errors mod_rest loading failed. Causing apache2 to stop loading" << std::endl;
        exit(APEXIT_CHILDFATAL);
    }
}

void registerHooks(apr_pool_t *p) {
    ap_hook_handler(handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_post_config(init, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(childInit, NULL, NULL, APR_HOOK_MIDDLE);
}

}

extern "C" {

module AP_MODULE_DECLARE_DATA mod_tuscany = {
    STANDARD20_MODULE_STUFF,
    // dir config
    tuscany::makeDirConf,
    // dir merger, default is to override
    NULL,
    // server config
    tuscany::makeServerConf,
    // merge server config
    NULL,
    // command table
    tuscany::commands,
    // register hooks
    tuscany::registerHooks
};

}
