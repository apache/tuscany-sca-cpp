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
#include "element.hpp"
#include "monad.hpp"
#include "../atom/atom.hpp"
#include "../json/json.hpp"
#include "../eval/driver.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany;
}

namespace tuscany {
namespace httpd {

/**
 * Set to true to log requests and content.
 */
bool logRequests = false;
bool logContent = false;

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
const list<std::string> pathTokens(const request_rec* r) {
    const char* p = r->path_info;
    if (p == NULL || p[0] == '\0')
        return list<std::string>();
    return tokenize("/", p + 1);
}

const list<value> pathValues(const list<std::string>& l) {
    if (isNil(l))
        return list<value>();
    return cons<value>(car(l), pathValues(cdr(l)));
}

const list<value> path(const request_rec* r) {
    return pathValues(pathTokens(r));
}

/**
 * Return the content type of a request.
 */
const char* optional(const char* s) {
    if (s == NULL)
        return "(null)";
    return s;
}

const std::string contentType(const request_rec* r) {
    return optional(apr_table_get(r->headers_in, "Content-Type"));
}

/**
 * Log HTTP request info.
 */
int logHeader(void* r, const char* key, const char* value) {
    std::cout << "header key: " << key << ", value: " << value << std::endl;
    return 1;
}

const bool logRequest(request_rec* r) {
    std::cout << "mod-tuscany..." << std::endl;
    std::cout << "tuscany home: " << home(r) << std::endl;
    std::cout << "contribution: " << contribution(r) << std::endl;
    std::cout << "component: " << component(r) << std::endl;
    std::cout << "protocol: " << optional(r->protocol) << std::endl;
    std::cout << "method: " << optional(r->method) << std::endl;
    std::cout << "method number: " << r->method_number << std::endl;
    std::cout << "content type: " << contentType(r) << std::endl;
    std::cout << "content encoding: " << optional(r->content_encoding) << std::endl;
    apr_table_do(logHeader, r, r->headers_in, NULL);
    std::cout << "uri: " << optional(r->uri) << std::endl;
    std::cout << "path info: " << optional(r->path_info) << std::endl;
    std::cout << "filename: " << optional(r->filename) << std::endl;
    std::cout << "path: " << pathTokens(r) << std::endl;
    std::cout << "args: " << optional(r->args) << std::endl;
    std::cout.flush();
    return true;
}

/**
 * Evaluate an expression against a component implementation.
 */
const value evalExprLoop(std::istream& is, const value& expr, eval::Env& globalEnv, const gc_pool& pool) {
    value in = eval::readValue(is);
    if (isNil(in))
        return eval::evalExpr(expr, globalEnv, pool);
    eval::evalExpr(in, globalEnv, pool);
    return evalExprLoop(is, expr, globalEnv, pool);
}

const failable<value, std::string> evalExpr(const value& expr, const std::string& contrib, const std::string& impl) {
    // Retrieve the component implementation
    const std::string path = contrib + impl;
    std::ifstream is(path.c_str(), std::ios_base::in);
    if (is.fail() || is.bad())
        return std::string("HTTP_NOT_FOUND");

    // Evaluate the expr
    gc_pool pool;
    eval::Env globalEnv = eval::setupEnvironment(pool);
    std::cout<< "expr: " << expr << std::endl;
    std::cout.flush();
    const value val = evalExprLoop(is, expr, globalEnv, pool);
    std::cout<< "val: " << val << std::endl;
    std::cout.flush();

    if (isNil(val))
        return std::string("Could not evaluate expression");
    return val;
}

/**
 * Returns a list of key value pairs from the args in a query string.
 */
const list<value> queryArg(std::string s) {
    const list<std::string> t = tokenize("=", s);
    return mklist<value>(car(t).c_str(), cadr(t));
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
    const list<value> p = car(a);
    if (car(p) == value("id") || car(p) == value("method"))
        return queryParams(cdr(a));
    return cons(cadr(p), queryParams(cdr(a)));
}

/**
 * Write an HTTP result.
 */
const failable<int, std::string> writeResult(const failable<list<std::string>, std::string> ls, const std::string& ct, request_rec* r) {
    if (!hasValue(ls))
        return std::string(ls);
    std::ostringstream os;
    write(ls, os);
    if (logContent) {
        std::cout<< "content: " << std::endl << os.str() << std::endl;
        std::cout.flush();
    }

    std::string etag(ap_md5(r->pool, (const unsigned char*)std::string(os.str()).c_str()));
    const char* match = apr_table_get(r->headers_in, "If-None-Match");
    apr_table_setn(r->headers_out, "ETag", etag.c_str());
    if (match != NULL  && etag == match) {
        r->status = HTTP_NOT_MODIFIED;
        return OK;
    }
    ap_set_content_type(r, ct.c_str());
    ap_rputs(std::string(os.str()).c_str(), r);
    return OK;
}

/**
 * Handle an HTTP GET.
 */
const failable<int, std::string> get(request_rec* r) {

    // Inspect the query string
    const list<list<value> > args = queryArgs(r);
    const list<value> ia = assoc(value("id"), args);
    const list<value> ma = assoc(value("method"), args);

    // Evaluate a JSON-RPC request and return a JSON result
    if (!isNil(ia) && !isNil(ma)) {

        // Extract the request id, method and params
        const value id = cadr(ia);
        const value func = std::string(cadr(ma)).c_str();
        const list<value> params = queryParams(args);

        // Evaluate the request expression
        const failable<value, std::string> val = evalExpr(cons(func, eval::quotedParameters(params)), contribution(r), implementation(r));
        if (!hasValue(val))
            return std::string(val);

        // Return JSON result
        json::JSONContext cx;
        return writeResult(json::jsonResult(id, val, cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM GET request and return an ATOM feed
    if (isNil(path(r))) {
        const failable<value, std::string> val = evalExpr(mklist<value>("getall"), contribution(r), implementation(r));
        if (!hasValue(val))
            return std::string(val);
        const value feed = val;
        return writeResult(atom::writeATOMFeed(atom::feedValuesToElements(feed)), "application/atom+xml;type=feed", r);
    }

    // Evaluate an ATOM GET and return an ATOM entry
    const failable<value, std::string> val = evalExpr(cons<value>("get", path(r)), contribution(r), implementation(r));
    if (!hasValue(val))
        return std::string(val);
    const value entry = val;
    return writeResult(atom::writeATOMEntry(atom::entryValuesToElements(entry)), "application/atom+xml;type=entry", r);
    
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

const char* url(const value& v, request_rec* r) {
    std::string u = r->uri;
    u.append("/");
    u.append(v);
    return ap_construct_url(r->pool, u.c_str(), r);
}

/**
 * Convert an ATOM entry to a value.
 */
const value feedEntry(const list<value> e) {
    const list<value> v = elementsToValues(mklist<value>(caddr(e)));
    return cons(car(e), mklist<value>(cadr(e), cdr<value>(car(v))));
}

/**
 * Handle an HTTP POST.
 */
const failable<int, std::string> post(request_rec* r) {
    const list<std::string> ls = read(r);
    if (logContent) {
        std::cout<< "content: " << std::endl;
        write(ls, std::cout);
        std::cout<< std::endl;
        std::cout.flush();
    }

    // Evaluate a JSON-RPC request and return a JSON result
    const std::string ct = contentType(r);
    if (ct.find("application/json-rpc") != std::string::npos || ct.find("text/plain") != std::string::npos) {
        json::JSONContext cx;
        const list<value> json = elementsToValues(json::readJSON(ls, cx));
        const list<list<value> > args = postArgs(json);

        // Extract the request id, method and params
        const value id = cadr(assoc(value("id"), args));
        const value func = std::string(cadr(assoc(value("method"), args))).c_str();
        const list<value> params = (list<value>)cadr(assoc(value("params"), args));

        // Evaluate the request expression
        const failable<value, std::string> val = evalExpr(cons(func, eval::quotedParameters(params)), contribution(r), implementation(r));
        if (!hasValue(val))
            return std::string(val);

        // Return JSON result
        return writeResult(json::jsonResult(id, val, cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM POST request and return the created resource location
    if (ct.find("application/atom+xml") != std::string::npos) {

        // Evaluate the request expression
        const value entry = feedEntry(atom::readEntry(ls));
        const failable<value, std::string> val = evalExpr(cons<value>("post", eval::quotedParameters(mklist<value>(entry))), contribution(r), implementation(r));
        if (!hasValue(val))
            return std::string(val);

        // Return the created resource location
        apr_table_setn(r->headers_out, "Location", url(val, r));
        r->status = HTTP_CREATED;
        return OK;
    }

    return HTTP_NOT_IMPLEMENTED;
}

/**
 * Handle an HTTP PUT.
 */
const failable<int, std::string> put(request_rec* r) {
    const list<std::string> ls = read(r);
    std::cout<< "content: " << std::endl;
    write(ls, std::cout);
    std::cout<< std::endl;
    std::cout.flush();

    // Evaluate an ATOM PUT request
    const value entry = feedEntry(atom::readEntry(ls));
    const failable<value, std::string> val = evalExpr(cons<value>("put", eval::quotedParameters(mklist<value>(entry))), contribution(r), implementation(r));
    if (!hasValue(val))
        return std::string(val);
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Handle an HTTP DELETE.
 */
const failable<int, std::string> del(request_rec* r) {

    // Evaluate an ATOM delete request
    const failable<value, std::string> val = evalExpr(cons<value>("delete", path(r)), contribution(r), implementation(r));
    if (!hasValue(val))
        return std::string(val);
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Report request execution status.
 */
const int reportStatus(const failable<int, std::string> rc) {
    if (!hasValue(rc))
        return HTTP_INTERNAL_SERVER_ERROR;
    return rc;
}

/**
 * HTTP request handler entry point.
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
    //apr_table_setn(r->headers_out, "Connection", "close");

    // Handle HTTP method
    if (r->header_only)
         return OK;
    if(r->method_number == M_GET)
        return reportStatus(get(r));
    if(r->method_number == M_POST)
        return reportStatus(post(r));
    if(r->method_number == M_PUT)
        return reportStatus(put(r));
    if(r->method_number == M_DELETE)
        return reportStatus(del(r));
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
}

extern "C" {

module AP_MODULE_DECLARE_DATA mod_tuscany = {
    STANDARD20_MODULE_STUFF,
    // dir config
    tuscany::httpd::makeDirConf,
    // dir merger, default is to override
    NULL,
    // server config
    tuscany::httpd::makeServerConf,
    // merge server config
    NULL,
    // command table
    tuscany::httpd::commands,
    // register hooks
    tuscany::httpd::registerHooks
};

}
