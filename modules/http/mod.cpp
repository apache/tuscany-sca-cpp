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
 * Log HTTP request info to standard out for now, for debugging purposes.
 */
bool logRequests = true;

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
    std::cout << "path: " << pathTokens(r) << std::endl;
    std::cout << "args: " << optional(r->args) << std::endl;
    std::cout.flush();
    return true;
}

/**
 * Evaluate an expression against a component implementation.
 */
const value evalExprLoop(std::istream& is, const value& req, eval::Env& globalEnv) {
    value in = eval::read(is);
    if (isNil(in))
        return eval::evalApply(req, globalEnv);
    eval::evalApply(in, globalEnv);
    return evalExprLoop(is, req, globalEnv);
}

const failable<value, int> evalExpr(const value& expr, const std::string& contrib, const std::string& impl) {
    // Setup the evaluator
    eval::Env globalEnv = eval::setupEnvironment();
    std::ostringstream nullos;
    eval::setupEvalOut(nullos);

    // Retrieve the component implementation
    const std::string path = contrib + impl;
    std::ifstream is(path.c_str(), std::ios_base::in);
    if (is.fail() || is.bad())
        return HTTP_NOT_FOUND;

    // Evaluate the expr
    std::cout<< "expr: " << expr << std::endl;
    std::cout.flush();
    const value val = evalExprLoop(is, expr, globalEnv);
    std::cout<< "val: " << val << std::endl;
    std::cout.flush();
    if (isNil(val))
        return HTTP_INTERNAL_SERVER_ERROR;
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
 * Convert a value to a JSON result.
 */
const failable<list<std::string>, int> jsonResult(json::JSONContext& cx, const value& id, const failable<value, int>& val) {
    if (!hasValue(val))
        return int(val);
    const list<value> r = mklist<value>(mklist<value>("id", id), mklist<value>("result", val));
    failable<list<std::string>, std::string> ls = json::write(cx, valuesToElements(r));
    if (!hasValue(ls))
        return HTTP_INTERNAL_SERVER_ERROR;
    std::cout<< "content: " << std::endl;
    write(ls, std::cout);
    std::cout<< std::endl;
    std::cout.flush();
    return list<std::string>(ls);
}

/**
 * Convert a value to an ATOM entry.
 */
const list<value> feedEntryResult(const list<value> e) {
    return cons(car(e), cons(cadr(e), valuesToElements(mklist<value>(cons<value>("item", (list<value>)caddr(e))))));
}

/**
 * Convert a value to an ATOM feed.
 */
const list<value> feedEntriesResults(const list<value> e) {
    if (isNil(e))
        return list<value>();
    return cons<value>(feedEntryResult(car(e)), feedEntriesResults(cdr(e)));
}

const failable<list<std::string>, int> feedResult(const failable<value, int>& val) {
    if (!hasValue(val))
        return int(val);
    const value v = val;
    list<value> f = cons(car<value>(v), cons<value>(cadr<value>(v), feedEntriesResults(cddr<value>(v))));
    failable<list<std::string>, std::string> ls = atom::writeFeed(f);
    if (!hasValue(ls))
        return HTTP_INTERNAL_SERVER_ERROR;
    return list<std::string>(ls);
}

/**
 * Convert a value to an ATOM entry result.
 */
const failable<list<std::string>, int> entryResult(const failable<value, int>& val) {
    if (!hasValue(val))
        return int(val);
    const value v = val;
    list<value> e = feedEntryResult(v);
    std::cout<< "entry: " << e << std::endl;
    failable<list<std::string>, std::string> ls = atom::writeEntry(e);
    if (!hasValue(ls))
        return HTTP_INTERNAL_SERVER_ERROR;
    return list<std::string>(ls);
}

/**
 * Write an HTTP result.
 */
const int writeResult(const failable<list<std::string>, int> ls, const std::string& ct, request_rec* r) {
    if (!hasValue(ls))
        return ls;
    std::ostringstream os;
    write(ls, os);
    std::cout<< "content: " << os.str() << std::endl;
    std::cout.flush();

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
const int get(request_rec* r) {

    // Inspect the query string
    const list<list<value> > args = queryArgs(r);
    const list<value> ia = assoc(value("id"), args);
    const list<value> ma = assoc(value("method"), args);

    // Evaluate a JSON-RPC request and return a JSON result
    if (!isNil(ia) && !isNil(ma)) {

        // Extract the request id, method and params
        const value id = cadr(ia);
        const value method = std::string(cadr(ma)).c_str();
        const list<value> params = queryParams(args);

        // Evaluate the request expression
        const failable<value, int> val = evalExpr(cons(method, params), contribution(r), implementation(r));

        // Return JSON result
        json::JSONContext cx;
        return writeResult(jsonResult(cx, id, val), "application/json-rpc", r);
    }

    // Evaluate an ATOM GET request and return an ATOM feed
    if (length(path(r)) < 2) {
        const failable<value, int> val = evalExpr(cons<value>("getall"), contribution(r), implementation(r));
        return writeResult(feedResult(val), "application/atom+xml", r);
    }

    // Evaluate an ATOM GET and return an ATOM entry
    const failable<value, int> val = evalExpr(cons<value>("get", cdr(path(r))), contribution(r), implementation(r));
    return writeResult(entryResult(val), "application/atom+xml", r);
    
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

const char* url(const std::string& loc, request_rec* r) {
    std::string u = r->uri;
    u.append("/");
    u.append(loc);
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
const int post(request_rec* r) {
    const std::string ct = contentType(r);

    // Evaluate a JSON-RPC request and return a JSON result
    if (ct.find("application/json-rpc") != std::string::npos || ct.find("text/plain") != std::string::npos) {
        json::JSONContext cx;
        const list<value> json = elementsToValues(json::read(cx, read(r)));
        const list<list<value> > args = postArgs(json);

        // Extract the request id, method and params
        const value id = cadr(assoc(value("id"), args));
        const value method = std::string(cadr(assoc(value("method"), args))).c_str();
        const list<value> params = (list<value>)cadr(assoc(value("params"), args));

        // Evaluate the request expression
        const failable<value, int> val = evalExpr(cons(method, params), contribution(r), implementation(r));

        // Return JSON result
        return writeResult(jsonResult(cx, id, val), "application/json-rpc", r);
    }

    // Evaluate an ATOM POST request and return the created resource location
    if (ct.find("application/atom+xml") != std::string::npos) {
        const list<std::string> c = read(r);
        std::cout << "POST content: " << c << std::endl;
        const list<value> e = atom::readEntry(c);
        std::cout << "POST entry: " << e << std::endl;
        const value v = feedEntry(e);
        std::cout << "POST param: " << v << std::endl;

        // Evaluate the request expression
        const failable<value, int> val = evalExpr(mklist<value>("post", mklist(v)), contribution(r), implementation(r));

        const char* u = url("abcd", r);
        apr_table_setn(r->headers_out, "Location", u);
        apr_table_setn(r->headers_out, "Content-Location", u);
        return HTTP_CREATED;
    }

    return HTTP_NOT_IMPLEMENTED;
}

/**
 * Handle an HTTP PUT.
 */
const int put(request_rec* r) {
    // TODO later
    return OK;
}

/**
 * Handle an HTTP DELETE.
 */
const int del(request_rec* r) {

    // Evaluate an ATOM delete request
    const failable<value, int> val = evalExpr(cons<value>("delete", cdr(path(r))), contribution(r), implementation(r));
    if (!hasValue(val))
        return val;
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
