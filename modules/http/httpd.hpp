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
 * HTTPD module utility functions.
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
 * Converts the args received in a POST to a list of key value pairs.
 */
const list<list<value> > postArgs(const list<value>& a) {
    if (isNil(a))
        return list<list<value> >();
    const list<value> l = car(a);
    return cons(l, postArgs(cdr(a)));
}

}
}

#endif /* tuscany_httpd_hpp */
