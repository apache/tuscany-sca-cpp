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
#include "http_config.h"
#include "ap_mpm.h"
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

template<typename C> C& serverConf(const server_rec* s, const module* mod) {
    return *(C*)ap_get_module_config(s->module_config, mod);
}

template<typename C> C& serverConf(const cmd_parms *cmd, const module* mod) {
    return *(C*)ap_get_module_config(cmd->server->module_config, mod);
}


/**
 * Return the name of a server.
 */
const string serverName(const server_rec* s) {
    ostringstream n;
    n << (s->server_scheme != NULL? s->server_scheme : "http") << "://"
        << (s->server_hostname != NULL? s->server_hostname : "localhost") << ":"
        << (s->port != 0? s->port : 80)
        << (s->path != NULL? string(s->path, s->pathlen) : "");
    return str(n);
}

/**
 * Determine the name of a server from an HTTP request.
 */
const string serverName(request_rec* r) {
    ostringstream n;
    const char* hn = ap_get_server_name(r);
    n << (r->server->server_scheme != NULL? r->server->server_scheme : "http") << "://"
        << (hn != NULL? hn : (r->server->server_hostname != NULL? r->server->server_hostname : "localhost")) << ":"
        << (r->server->port != 0? r->server->port : 80)
        << (r->server->path != NULL? string(r->server->path, r->server->pathlen) : "");
    return str(n);
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

int debugEnv(unused void* r, const char* key, const char* value) {
    cerr << "  var key: " << key << ", value: " << value << endl;
    return 1;
}

int debugNote(unused void* r, const char* key, const char* value) {
    cerr << "  note key: " << key << ", value: " << value << endl;
    return 1;
}

const bool debugRequest(request_rec* r, const string& msg) {
    cerr << msg << ":" << endl;
    cerr << "  server: " << optional(r->server->server_hostname) << endl;
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
    cerr << "  user: " << optional(r->user) << endl;
    cerr << "  auth type: " << optional(r->ap_auth_type) << endl;
    apr_table_do(debugEnv, r, r->subprocess_env, NULL);
    apr_table_do(debugEnv, r, r->notes, NULL);
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
 * Convert a URI represented as a list to an absolute URL.
 */
const char* url(const list<value>& v, request_rec* r) {
    const string u = string(r->uri) + path(v);
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

    // Make sure browsers come back and check for updated dynamic content
    apr_table_setn(r->headers_out, "Expires", "Tue, 01 Jan 1980 00:00:00 GMT");

    // Compute and return an Etag for the returned content
    const string etag(ap_md5(r->pool, (const unsigned char*)c_str(ob)));

    // Check for an If-None-Match header and just return a 304 not-modified status
    // if the Etag matches the Etag presented by the client, to save bandwith
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

/**
 * Construct a redirect URI.
 */
const string redirectURI(const string& file, const string& pi) {
    return file + pi;
}

const string redirectURI(const string& file, const string& pi, const string& args) {
    return file + pi + "?" + args;
}

/**
 * Convert a value to an HTTPD request struc
 */
request_rec* request(const value& v) {
    return (request_rec*)(long)(double)v;
}

/**
 * Convert an HTTPD request struct to a value
 */
const value requestValue(request_rec* r) {
    return value((double)(long)r);
}

/**
 * Update filters in an HTTPD redirect request.
 * Similar to httpd/modules/http/http_request.c::update_r_in_filters.
 */
const bool redirectFilters(ap_filter_t* f, request_rec* from, request_rec* to) {
    if (f == NULL)
        return true;
    if (f->r == from)
        f->r = to;
    return redirectFilters(f->next, from, to);
}

/**
 * Create an HTTPD internal redirect request.
 * Similar to httpd/modules/http/http_request.c::internal_internal_redirect.
 */
extern "C" {
    AP_DECLARE(ap_conf_vector_t*) ap_create_request_config(apr_pool_t *p);
}

const failable<request_rec*, int> internalRedirectRequest(const string& nr_uri, request_rec* r) {
    if (ap_is_recursion_limit_exceeded(r))
        return mkfailure<request_rec*, int>(HTTP_INTERNAL_SERVER_ERROR);

    // Create a new request
    request_rec* nr = (request_rec*)apr_pcalloc(r->pool, sizeof(request_rec));
    nr->connection = r->connection;
    nr->server = r->server;
    nr->pool = r->pool;
    nr->method = r->method;
    nr->method_number = r->method_number;
    nr->allowed_methods = ap_make_method_list(nr->pool, 2);
    ap_parse_uri(nr, apr_pstrdup(nr->pool, c_str(nr_uri)));
    nr->request_config = ap_create_request_config(r->pool);
    nr->per_dir_config = r->server->lookup_defaults;
    nr->prev = r;
    r->next   = nr;

    // Run create request hook
    ap_run_create_request(nr);

    // Inherit protocol info from the original request
    nr->the_request = r->the_request;
    nr->allowed = r->allowed;
    nr->status = r->status;
    nr->assbackwards = r->assbackwards;
    nr->header_only = r->header_only;
    nr->protocol = r->protocol;
    nr->proto_num = r->proto_num;
    nr->hostname = r->hostname;
    nr->request_time = r->request_time;
    nr->main = r->main;
    nr->headers_in = r->headers_in;
    nr->headers_out = apr_table_make(r->pool, 12);
    nr->err_headers_out = r->err_headers_out;
    nr->subprocess_env = r->subprocess_env;
    nr->notes = apr_table_make(r->pool, 5);
    nr->allowed_methods = ap_make_method_list(nr->pool, 2);
    nr->htaccess = r->htaccess;
    nr->no_cache = r->no_cache;
    nr->expecting_100 = r->expecting_100;
    nr->no_local_copy = r->no_local_copy;
    nr->read_length = r->read_length;
    nr->vlist_validator = r->vlist_validator;
    nr->user = r->user;

    // Setup input and output filters
    nr->proto_output_filters  = r->proto_output_filters;
    nr->proto_input_filters   = r->proto_input_filters;
    nr->output_filters  = nr->proto_output_filters;
    nr->input_filters   = nr->proto_input_filters;
    if (nr->main)
        ap_add_output_filter_handle(ap_subreq_core_filter_handle, NULL, nr, nr->connection);
    redirectFilters(nr->input_filters, r, nr);
    redirectFilters(nr->output_filters, r, nr);
    const int rrc = ap_run_post_read_request(nr);
    if (rrc != OK && rrc != DECLINED)
        return mkfailure<request_rec*, int>(rrc);

    return nr;
}

/**
 * Process an HTTPD internal redirect request.
 * Similar to httpd/modules/http/http_request.c::ap_internal_redirect.
 */
extern "C" {
    AP_DECLARE(int) ap_invoke_handler(request_rec *r);
}

const int internalRedirect(request_rec* nr) {
    int status = ap_run_quick_handler(nr, 0);
    if (status == DECLINED) {
        status = ap_process_request_internal(nr);
        if (status == OK)
            status = ap_invoke_handler(nr);
    }
    if (status != OK) {
        nr->status = status;
        return OK;
    }
    ap_finalize_request_protocol(nr);
    return OK;
}

/**
 * Create and process an HTTPD internal redirect request.
 */
const int internalRedirect(const string& uri, request_rec* r) {
    const failable<request_rec*, int> nr = httpd::internalRedirectRequest(uri, r);
    if (!hasContent(nr))
        return reason(nr);
    return httpd::internalRedirect(content(nr));
}

/**
 * Put a value in the process user data.
 */
const bool putUserData(const string& k, const void* v, const server_rec* s) {
    apr_pool_userdata_set((const void *)v, c_str(k), apr_pool_cleanup_null, s->process->pool);
    return true;
}

/**
 * Return a user data value.
 */
const void* userData(const string& k, const server_rec* s) {
    void* v = NULL;
    apr_pool_userdata_get(&v, c_str(k), s->process->pool);
    return v;
}

}
}

#endif /* tuscany_httpd_hpp */
