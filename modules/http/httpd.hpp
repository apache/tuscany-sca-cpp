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

#include <string>
#include <iostream>

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
#include "value.hpp"


namespace tuscany {
namespace httpd {

/**
 * Set to true to log requests and content.
 */
bool logRequests = false;
bool logContent = false;

/**
 * Returns a server-scoped module configuration.
 */
template<typename C> void* makeServerConf(apr_pool_t *p, server_rec *s) {
    C* c = new (apr_palloc(p, sizeof(C))) C(s);
    apr_pool_cleanup_register(p, c, gc_pool_cleanupCallback<C>, apr_pool_cleanup_null) ;
    return c;
}

template<typename C> const C& serverConf(const request_rec* r, const module* mod) {
    return *(C*)ap_get_module_config(r->server->module_config, mod);
}

/**
 * Returns a directory-scoped module configuration.
 */
template<typename C> void *makeDirConf(apr_pool_t *p, char *dirspec) {
    C* c = new (apr_palloc(p, sizeof(C))) C(dirspec);
    apr_pool_cleanup_register(p, c, gc_pool_cleanupCallback<C>, apr_pool_cleanup_null) ;
    return c;
}

template<typename C> C& dirConf(const request_rec* r, const module* mod) {
    return *(C*)ap_get_module_config(r->per_dir_config, mod);
}

/**
 * Convert a path string to a list of values.
 */
const list<std::string> pathTokens(const char* p) {
    if (p == NULL || p[0] == '\0')
        return list<std::string>();
    return tokenize("/", p + 1);
}

const list<value> pathValues(const list<std::string>& l) {
    if (isNil(l))
        return list<value>();
    return cons<value>(car(l), pathValues(cdr(l)));
}

const list<value> path(const char* p) {
    return pathValues(pathTokens(p));
}

/**
 * Convert a path represented as a list of values to a string.
 */
const std::string path(const list<value>& p) {
    if (isNil(p))
        return "";
    return "/" + car(p) + path(cdr(p));
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

const bool logRequest(request_rec* r, const std::string& msg) {
    if (!logRequests)
        return true;
    std::cout << msg << std::endl;
    std::cout << "protocol: " << optional(r->protocol) << std::endl;
    std::cout << "method: " << optional(r->method) << std::endl;
    std::cout << "method number: " << r->method_number << std::endl;
    std::cout << "content type: " << contentType(r) << std::endl;
    std::cout << "content encoding: " << optional(r->content_encoding) << std::endl;
    apr_table_do(logHeader, r, r->headers_in, NULL);
    std::cout << "uri: " << optional(r->unparsed_uri) << std::endl;
    std::cout << "path: " << optional(r->uri) << std::endl;
    std::cout << "path info: " << optional(r->path_info) << std::endl;
    std::cout << "filename: " << optional(r->filename) << std::endl;
    std::cout << "path tokens: " << pathTokens(r->uri) << std::endl;
    std::cout << "args: " << optional(r->args) << std::endl;
    std::cout.flush();
    return true;
}

const bool logValue(const value& v, const std::string& msg) {
    if (!logContent)
        return true;
    std::cout<< msg << ": " << v << std::endl;
    std::cout.flush();
    return true;
}

const bool logValue(const failable<value, std::string>& v, const std::string& msg) {
    if (!logContent)
        return true;
    std::cout<< msg << ": " << v << std::endl;
    std::cout.flush();
    return true;
}

const bool logStrings(const list<std::string>& ls, const std::string& msg) {
    if (!logContent)
        return true;
    std::cout<< msg << ": " << std::endl;
    write(ls, std::cout);
    std::cout<< std::endl;
    std::cout.flush();
    return true;
}

/**
 * Returns a list of key value pairs from the args in a query string.
 */
const list<value> queryArg(const std::string& s) {
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
const list<std::string> read(request_rec* r) {
    char b[2048];
    const int n = ap_get_client_block(r, b, 2048);
    if (n <= 0)
        return list<std::string>();
    return cons(std::string(b, n), read(r));
}

/**
 * Convert a URI value to an absolute URL.
 */
const char* url(const value& v, request_rec* r) {
    std::string u = r->uri;
    u.append("/");
    u.append(v);
    return ap_construct_url(r->pool, u.c_str(), r);
}

/**
 * Convert an ATOM entry to a value.
 */
const value feedEntry(const list<value>& e) {
    const list<value> v = elementsToValues(mklist<value>(caddr(e)));
    return cons(car(e), mklist<value>(cadr(e), cdr<value>(car(v))));
}

/**
 * Write an HTTP result.
 */
const failable<int, std::string> writeResult(const failable<list<std::string>, std::string>& ls, const std::string& ct, request_rec* r) {
    if (!hasContent(ls))
        return mkfailure<int, std::string>(reason(ls));
    std::ostringstream os;
    write(content(ls), os);
    if (logContent) {
        std::cout<< "content: " << std::endl << os.str() << std::endl;
        std::cout.flush();
    }

    const std::string etag(ap_md5(r->pool, (const unsigned char*)std::string(os.str()).c_str()));
    const char* match = apr_table_get(r->headers_in, "If-None-Match");
    apr_table_setn(r->headers_out, "ETag", apr_pstrdup(r->pool, etag.c_str()));
    if (match != NULL  && etag == match) {
        r->status = HTTP_NOT_MODIFIED;
        return OK;
    }
    ap_set_content_type(r, ct.c_str());
    ap_rputs(std::string(os.str()).c_str(), r);
    return OK;
}

/**
 * Report request execution status.
 */
const int reportStatus(const failable<int, std::string>& rc) {
    if (!hasContent(rc))
        return HTTP_INTERNAL_SERVER_ERROR;
    return content(rc);
}

}
}

#endif /* tuscany_httpd_hpp */
