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
#include <fstream>

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
#include "value.hpp"
#include "monad.hpp"
#include "../json/json.hpp"
#include "../eval/driver.hpp"

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
    const char* contribution;
    const char* component;
    const char* implementation;
};

/**
 * Returns the server conf for a request.
 */
const ServerConf& serverConf(const request_rec* r) {
    return *(ServerConf*)ap_get_module_config(r->server->module_config, &mod_tuscany);
}
const std::string home(request_rec* r) {
    return serverConf(r).home;
}

/**
 * Returns the dir conf for a request.
 */
const DirConf& dirConf(const request_rec* r) {
    return *(DirConf*)ap_get_module_config(r->per_dir_config, &mod_tuscany);
}
const std::string contribution(request_rec* r) {
    return dirConf(r).contribution;
}
const std::string component(request_rec* r) {
    return dirConf(r).component;
}
const std::string implementation(request_rec* r) {
    return dirConf(r).implementation;
}

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

const bool logRequest(request_rec* r) {
    std::cout << "mod-tuscany..." << std::endl;
    std::cout << "tuscany home: " << home(r) << std::endl;
    std::cout << "contribution: " << contribution(r) << std::endl;
    std::cout << "component: " << component(r) << std::endl;
    std::cout << "protocol: " << optional(r->protocol) << std::endl;
    std::cout << "method: " << optional(r->method) << std::endl;
    std::cout << "method number: " << r->method_number << std::endl;
    std::cout << "content type: " << optional(apr_table_get(r->headers_in, "Content-Type")) << std::endl;
    std::cout << "content encoding: " << optional(r->content_encoding) << std::endl;
    apr_table_do(logHeader, r, r->headers_in, NULL);
    std::cout << "uri: " << optional(r->uri) << std::endl;
    std::cout << "path info: " << optional(r->path_info) << std::endl;
    std::cout << "path: " << path(r) << std::endl;
    std::cout << "args: " << optional(r->args) << std::endl;
    std::cout.flush();
    return true;
}

const value evalLoop(std::istream& is, const value& req, Env& globalEnv) {
    value in = read(is);
    if (isNil(in))
        return eval(req, globalEnv);
    eval(in, globalEnv);
    return evalLoop(is, req, globalEnv);
}

/**
 * Returns a list of key value pairs from the args in a query string.
 */
const list<value> queryArg(std::string s) {
    const list<std::string> t = tokenize("=", s);
    return makeList<value>(car(t).c_str(), cadr(t));
}

const list<list<value> > queryArgs(const request_rec* r) {
    const char* a = r->args;
    if (a == NULL)
        return list<list<value> >();
    return map<std::string, list<value>>(queryArg, tokenize("&", a));
}

/**
 * Returns a list of param values other than the id and method args from a list
 * of key value pairs.
 */
const list<value> queryParams(list<list<value> > a) {
    if (isNil(a))
        return list<value>();
    if (car(a) == value("id") || car(a) == value("method"))
        return queryParams(cdr(a));
    return cons(cadr(car(a)), queryParams(cdr(a)));
}

/**
 * Handle an HTTP GET request.
 */
const int get(request_rec* r) {

    // Setup the script evaluator
    Env globalEnv = setupEnvironment();
    std::ostringstream nullos;
    setupEvalOut(nullos);

    // Open the component implementation
    const std::string impl = contribution(r) + implementation(r);
    std::ifstream is(impl.c_str(), std::ios_base::in);
    if (is.fail() || is.bad())
        return HTTP_NOT_FOUND;

    // Extract the request id, method and params from the query string
    const list<list<value> > args = queryArgs(r);
    const value id = cadr(assoc(value("id"), args));
    const value method = std::string(cadr(assoc(value("method"), args))).c_str();
    const list<value> params = queryParams(args);

    // Build expr to evaluate
    const value expr = cons<value>(method, params);
    std::cout<< "expr: " << expr << std::endl;
    std::cout.flush();

    // Evaluate the expr
    const tuscany::value val = evalLoop(is, expr, globalEnv);
    if (isNil(val))
        return HTTP_INTERNAL_SERVER_ERROR;
    std::cout<< "val: " << val << std::endl;
    std::cout.flush();

    // Convert the expr value to JSON
    const JSONContext cx;
    failable<list<std::string>, std::string> jsval = writeJSON(cx, makeList<value>(makeList<value>("id", id), makeList<value>("result", val)));
    if (!hasValue(jsval))
        return HTTP_INTERNAL_SERVER_ERROR;

    // Send the response
    ap_set_content_type(r, "application/json-rpc");
    std::ostringstream os;
    write(jsval, os);
    std::string sval = os.str();
    std::string etag(ap_md5(r->pool, (const unsigned char*)sval.c_str()));
    const char* match = apr_table_get(r->headers_in, "If-None-Match");
    if (match != NULL  && etag == match)
        r->status = HTTP_NOT_MODIFIED;
    apr_table_setn(r->headers_out, "ETag", etag.c_str());
    ap_rputs(sval.c_str(), r);

    return OK;
}

/**
 * Read the content of a POST.
 */
const list<std::string> read(request_rec* r) {
    char b[2048];
    const int n = ap_get_client_block(r, b, 2048);
    if (n <= 0)
        return list<std::string>();
    return cons(std::string(b, n), read(r));
}

/**
 * Converts the args received in a POST to a list of key value pairs.
 */
const list<list<value> > postArgs(list<value> a) {
    if (isNil(a))
        return list<list<value> >();
    const list<value> l = car(a);
    return cons(l, postArgs(cdr(a)));
}

/**
 * Handle an HTTP POST request.
 */
const int post(request_rec* r) {

    // Setup the script evaluator
    Env globalEnv = setupEnvironment();
    std::ostringstream nullos;
    setupEvalOut(nullos);

    // Open the component implementation
    const std::string impl = contribution(r) + implementation(r);
    std::ifstream is(impl.c_str(), std::ios_base::in);
    if (is.fail() || is.bad())
        return HTTP_NOT_FOUND;

    // Read the JSON request
    const list<std::string> req = read(r);
    JSONContext cx;
    const list<value> json = readJSON(cx, req);
    const list<list<value> > args = postArgs(json);

    // Extract the request id, method and params
    const value id = cadr(assoc(value("id"), args));
    const value method = std::string(cadr(assoc(value("method"), args))).c_str();
    const list<value> params = (list<value>)cadr(assoc(value("params"), args));

    // Build expr to evaluate
    const value expr = cons<value>(method, params);
    std::cout<< "expr: " << expr << std::endl;
    std::cout.flush();

    // Evaluate the expr
    const tuscany::value val = evalLoop(is, expr, globalEnv);
    if (isNil(val))
        return HTTP_INTERNAL_SERVER_ERROR;
    std::cout<< "val: " << val << std::endl;
    std::cout.flush();

    // Convert the expr value to JSON
    failable<list<std::string>, std::string> jsval = writeJSON(cx, makeList<value>(makeList<value>("id", id), makeList<value>("result", val)));
    if (!hasValue(jsval))
        return HTTP_INTERNAL_SERVER_ERROR;

    // Send the JSON response
    ap_set_content_type(r, "application/json-rpc");
    std::ostringstream os;
    write(jsval, os);
    ap_rputs(os.str().c_str(), r);

    return OK;
}

/**
 * Handle an HTTP PUT request.
 */
const int put(request_rec* r) {
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

    // Log the request
    if(logRequests)
        logRequest(r);

    // Set up the read policy
    const int rc = ap_setup_client_block(r, REQUEST_CHUNKED_DECHUNK);
    if(rc != OK)
        return rc;
    ap_should_client_block(r);
    if(r->read_chunked == true && r->remaining == 0)
        r->chunked = true;
    apr_table_setn(r->headers_out, "Connection", "close");

    // Handle HTTP method
    if (r->header_only)
         return OK;
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
const char *confContribution(cmd_parms *cmd, void *c, const char *arg) {
    DirConf *conf = (DirConf*)c;
    conf->contribution = apr_pstrdup(cmd->pool, arg);
    return NULL;
}
const char *confComponent(cmd_parms *cmd, void *c, const char *arg) {
    DirConf *conf = (DirConf*)c;
    conf->component = apr_pstrdup(cmd->pool, arg);
    return NULL;
}
const char *confImplementation(cmd_parms *cmd, void *c, const char *arg) {
    DirConf *conf = (DirConf*)c;
    conf->implementation = apr_pstrdup(cmd->pool, arg);
    return NULL;
}
void *makeDirConf(apr_pool_t *p, char *dirspec) {
    DirConf* conf = (DirConf*)apr_palloc(p, sizeof(*conf));
    conf->contribution = "";
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
    AP_INIT_TAKE1("TuscanyHome", (const char*(*)())confHome, NULL, RSRC_CONF, "Tuscany home directory"),
    AP_INIT_TAKE1("SCAContribution", (const char*(*)())confContribution, NULL, ACCESS_CONF, "SCA contribution location"),
    AP_INIT_TAKE1("SCAComponent", (const char*(*)())confComponent, NULL, ACCESS_CONF, "SCA component name"),
    AP_INIT_TAKE1("SCAImplementation", (const char*(*)())confImplementation, NULL, ACCESS_CONF, "SCA component implementation"),
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
