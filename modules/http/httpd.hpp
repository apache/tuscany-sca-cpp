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

#ifndef tuscany_httpd_hpp
#define tuscany_httpd_hpp

/**
 * HTTPD module implementation functions.
 */

#include "apr_strings.h"
#include "apr_fnmatch.h"
#include "apr_lib.h"
#define APR_WANT_STRFUNC
#include "apr_want.h"

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

#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "value.hpp"


namespace tuscany {
namespace httpd {

/**
 * Returns a server-scoped module configuration.
 */
template<typename C> void* makeServerConf(apr_pool_t *p, server_rec *s) {
    return new (gc_new<C>(p)) C(s);
}

template<typename C> const C& serverConf(const request_rec* r, const module* mod) {
    return *(C*)ap_get_module_config(r->server->module_config, mod);
}

template<typename C> C& serverConf(const cmd_parms *cmd, const module* mod) {
    return *(C*)ap_get_module_config(cmd->server->module_config, mod);
}


/**
 * Returns a directory-scoped module configuration.
 */
template<typename C> void *makeDirConf(apr_pool_t *p, char *dirspec) {
    return new (gc_new<C>(p)) C(dirspec);
}

template<typename C> C& dirConf(const request_rec* r, const module* mod) {
    return *(C*)ap_get_module_config(r->per_dir_config, mod);
}

/**
 * Convert a path string to a list of values.
 */
const list<string> pathTokens(const char* p) {
    if (p == NULL || p[0] == '\0')
        return list<string>();
    if (p[0] == '/')
        return tokenize("/", p + 1);
    return tokenize("/", p);
}

const list<value> pathValues(const char* p) {
    return mkvalues(pathTokens(p));
}

/**
 * Convert a path represented as a list of values to a string.
 */
const string path(const list<value>& p) {
    if (isNil(p))
        return "";
    return string("/") + car(p) + path(cdr(p));
}

/**
 * Return the content type of a request.
 */
const char* optional(const char* s) {
    if (s == NULL)
        return "";
    return s;
}

const string contentType(const request_rec* r) {
    return optional(apr_table_get(r->headers_in, "Content-Type"));
}

#ifdef WANT_MAINTAINER_MODE

/**
 * Debug log.
 */
int debugHeader(unused void* r, const char* key, const char* value) {
    cerr << "  header key: " << key << ", value: " << value << endl;
    return 1;
}

const bool debugRequest(request_rec* r, const string& msg) {
    cerr << msg << ":" << endl;
    cerr << "  protocol: " << optional(r->protocol) << endl;
    cerr << "  method: " << optional(r->method) << endl;
    cerr << "  method number: " << r->method_number << endl;
    cerr << "  content type: " << contentType(r) << endl;
    cerr << "  content encoding: " << optional(r->content_encoding) << endl;
    apr_table_do(debugHeader, r, r->headers_in, NULL);
    cerr << "  unparsed uri: " << optional(r->unparsed_uri) << endl;
    cerr << "  uri: " << optional(r->uri) << endl;
    cerr << "  path info: " << optional(r->path_info) << endl;
    cerr << "  filename: " << optional(r->filename) << endl;
    cerr << "  uri tokens: " << pathTokens(r->uri) << endl;
    cerr << "  args: " << optional(r->args) << endl;
    return true;
}

#define httpdDebugRequest(r, msg) httpd::debugRequest(r, msg)

#else

#define httpdDebugRequest(r, msg)

#endif

/**
 * Return the remaining part of a uri after the given path (aka the path info.)
 */
const list<value> pathInfo(const list<value>& uri, const list<value>& path) {
    if (isNil(path))
        return uri;
    return pathInfo(cdr(uri), cdr(path));
}

/**
 * Returns a list of key value pairs from the args in a query string.
 */
const list<value> queryArg(const string& s) {
    const list<string> t = tokenize("=", s);
    return mklist<value>(c_str(car(t)), cadr(t));
}

const list<list<value> > queryArgs(const request_rec* r) {
    const char* a = r->args;
    if (a == NULL)
        return list<list<value> >();
    return map<string, list<value>>(queryArg, tokenize("&", a));
}

/**
 * Returns a list of param values other than the id and method args from a list
 * of key value pairs.
 */
const list<value> queryParams(const list<list<value> >& a) {
    if (isNil(a))
        return list<value>();
    const list<value> p = car(a);
    if (car(p) == value("id") || car(p) == value("method"))
        return queryParams(cdr(a));
    return cons(cadr(p), queryParams(cdr(a)));
}

/**
 * Converts the args received in a POST to a list of key value pairs.
 */
const list<list<value> > postArgs(const list<value>& a) {
    if (isNil(a))
        return list<list<value> >();
    const list<value> l = car(a);
    return cons(l, postArgs(cdr(a)));
}

/**
 * Setup the HTTP read policy.
 */
const int setupReadPolicy(request_rec* r) {
    const int rc = ap_setup_client_block(r, REQUEST_CHUNKED_DECHUNK);
    if(rc != OK)
        return rc;
    ap_should_client_block(r);
    if(r->read_chunked == true && r->remaining == 0)
        r->chunked = true;
    //apr_table_setn(r->headers_out, "Connection", "close");
    return OK;
}

/**
 * Read the content of a POST or PUT.
 */
const list<string> read(request_rec* r) {
    char b[1024];
    const int n = ap_get_client_block(r, b, sizeof(b));
    if (n <= 0)
        return list<string>();
    return cons(string(b, n), read(r));
}

/**
 * Convert a URI value to an absolute URL.
 */
const char* url(const value& v, request_rec* r) {
    const string u = string(r->uri) + "/" + v;
    return ap_construct_url(r->pool, c_str(u), r);
}

/**
 * Write an HTTP result.
 */
const failable<int> writeResult(const failable<list<string> >& ls, const string& ct, request_rec* r) {
    if (!hasContent(ls))
        return mkfailure<int>(reason(ls));
    ostringstream os;
    write(content(ls), os);
    const string ob(str(os));
    debug(ob, "httpd::result");

    const string etag(ap_md5(r->pool, (const unsigned char*)c_str(ob)));
    const char* match = apr_table_get(r->headers_in, "If-None-Match");
    apr_table_setn(r->headers_out, "ETag", apr_pstrdup(r->pool, c_str(etag)));
    if (match != NULL  && etag == match) {
        r->status = HTTP_NOT_MODIFIED;
        return OK;
    }
    ap_set_content_type(r, apr_pstrdup(r->pool, c_str(ct)));
    ap_rputs(c_str(ob), r);
    return OK;
}

/**
 * Report request execution status.
 */
const int reportStatus(const failable<int>& rc) {
    if (!hasContent(rc))
        return HTTP_INTERNAL_SERVER_ERROR;
    return content(rc);
}

}
}

#endif /* tuscany_httpd_hpp */
