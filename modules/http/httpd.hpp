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

#include <apr_strings.h>
#include <apr_fnmatch.h>
#include <apr_lib.h>
#define APR_WANT_STRFUNC
#include <apr_want.h>
#include <apr_base64.h>

#include <httpd.h>
// Hack to workaround compile error with HTTPD 2.3.8
#define new new_
#include <http_config.h>
#undef new
#include <http_core.h>
#include <http_connection.h>
#include <http_request.h>
#include <http_protocol.h>
// Hack to workaround compile error with HTTPD 2.3.8
#define aplog_module_index aplog_module_index = 0
#include <http_log.h>
#undef aplog_module_index
#undef APLOG_MODULE_INDEX
#define APLOG_MODULE_INDEX (aplog_module_index ? *aplog_module_index : APLOG_NO_MODULE)
#include <http_main.h>
#include <util_script.h>
#include <util_md5.h>
#include <http_config.h>
#include <http_log.h>
#include <ap_mpm.h>
#include <mod_core.h>
#include <ap_provider.h>
#include <mod_auth.h>
#include <mod_session.h>

#include "string.hpp"
#include "stream.hpp"
#include "sstream.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"


namespace tuscany {
namespace httpd {

/**
 * Returns a server-scoped module configuration.
 */
template<typename C> void* makeServerConf(apr_pool_t* p, server_rec* s) {
    return new (gc_new<C>(p)) C(p, s);
}

template<typename C> const C& serverConf(const request_rec* r, const module* mod) {
    return *(C*)ap_get_module_config(r->server->module_config, mod);
}

template<typename C> C& serverConf(const server_rec* s, const module* mod) {
    return *(C*)ap_get_module_config(s->module_config, mod);
}

template<typename C> C& serverConf(const cmd_parms* cmd, const module* mod) {
    return *(C*)ap_get_module_config(cmd->server->module_config, mod);
}

/**
 * Returns a directory-scoped module configuration.
 */
template<typename C> void* makeDirConf(apr_pool_t *p, char* d) {
    return new (gc_new<C>(p)) C(p, d);
}

template<typename C> const C& dirConf(const request_rec* r, const module* mod) {
    return *(C*)ap_get_module_config(r->per_dir_config, mod);
}

template<typename C> C& dirConf(const void* c) {
    return *(C*)c;
}

/**
 * Return the name of a server.
 */
const string serverName(const server_rec* s, const string& def = "localhost") {
    ostringstream n;
    n << (s->server_scheme != NULL? s->server_scheme : "http") << "://"
        << (s->server_hostname != NULL? s->server_hostname : def) << ":"
        << (s->port != 0? s->port : 80)
        << (s->path != NULL? string(s->path, s->pathlen) : "");
    return str(n);
}

/**
 * Determine the name of a server from an HTTP request.
 */
const string serverName(request_rec* r, const string& def = "localhost") {
    ostringstream n;
    const char* hn = ap_get_server_name(r);
    n << (r->server->server_scheme != NULL? r->server->server_scheme : "http") << "://"
        << (hn != NULL? hn : (r->server->server_hostname != NULL? r->server->server_hostname : def)) << ":"
        << (r->server->port != 0? r->server->port : 80)
        << (r->server->path != NULL? string(r->server->path, r->server->pathlen) : "");
    return str(n);
}

/**
 * Return the host name for a server.
 */
const string hostName(const server_rec* s, const string& def = "localhost") {
    return s->server_hostname != NULL? s->server_hostname : def;
}

/**
 * Return the host name from an HTTP request.
 */
const string hostName(request_rec* r, const string& def = "localhost") {
    const char* hn = ap_get_server_name(r);
    return hn != NULL? hn : (r->server->server_hostname != NULL? r->server->server_hostname : def);
}

/**
 * Return the first subdomain name in a host name.
 */
const string subdomain(const string& host) {
    return substr(host, 0, find(host, '.'));
}

/**
 * Return true if a request is targeting a virtual host.
 */
const bool isVirtualHostRequest(const server_rec* s, request_rec* r) {
    return hostName(r) != hostName(s);
}

/**
 * Return true if a URI is absolute.
 */
const bool isAbsolute(const string& uri) {
    return contains(uri, "://");
}

/**
 * Return the protocol scheme for a server.
 */
const string scheme(const server_rec* s, const string& def = "http") {
    return s->server_scheme != NULL? s->server_scheme : def;
}

/**
 * Return the protocol scheme from an HTTP request.
 */
const string scheme(request_rec* r, const string& def = "http") {
    return r->server->server_scheme != NULL? r->server->server_scheme : def;
}

/**
 * Return the content type of a request.
 */
const string contentType(const request_rec* r) {
    const char* ct = apr_table_get(r->headers_in, "Content-Type");
    if (ct == NULL)
        return "";
    return ct;
}

/**
 * Return the remaining part of a uri after the given path (aka the path info.)
 */
const list<value> pathInfo(const list<value>& uri, const list<value>& path) {
    if (isNil(path))
        return uri;
    return pathInfo(cdr(uri), cdr(path));
}

/**
 * Convert a URI and a path to an absolute URL.
 */
const string url(const string& uri, const list<value>& p, request_rec* r) {
    const string u = uri + path(p);
    return ap_construct_url(r->pool, c_str(u), r);
}

/**
 * Convert a URI to an absolute URL.
 */
const string url(const string& uri, request_rec* r) {
    return ap_construct_url(r->pool, c_str(uri), r);
}

/**
 * Escape a URI.
 */
const char escape_c2x[] = "0123456789ABCDEF";
const string escape(const string& uri) {
    debug(uri, "httpd::escape::uri");
    char* copy = (char*)apr_palloc(gc_current_pool(), 3 * length(uri) + 3);
    const unsigned char* s = (const unsigned char *)c_str(uri);
    unsigned char* d = (unsigned char*)copy;
    unsigned c;
    while ((c = *s)) {
        if (apr_isalnum(c) || c == '_')
            *d++ = (unsigned char)c;
        else if (c == ' ')
            *d++ = '+';
        else {
            *d++ = '%';
            *d++ = escape_c2x[c >> 4];
            *d++ = escape_c2x[c & 0xf];
        }
        ++s;
    }
    *d = '\0';
    debug(copy, "httpd::escape::result");
    return copy;
}

/**
 * Unescape a URI.
 */
const string unescape(const string& uri) {
    debug(uri, "httpd::unescape::uri");
    char* b = const_cast<char*>(c_str(string(c_str(uri))));
    ap_unescape_url(b);
    debug(b, "httpd::unescape::result");
    return b;
}

/**
 * Returns a list of key value pairs from the args in a query string.
 */
const list<value> queryArg(const string& s) {
    debug(s, "httpd::queryArg::string");
    const list<string> t = tokenize("=", s);
    if (isNil(cdr(t)))
        return mklist<value>(c_str(car(t)), "");
    return mklist<value>(c_str(car(t)), cadr(t));
}

const string fixupQueryArgs(const string& a) {
    const list<string> t = tokenize("?", a);
    if (isNil(t) || isNil(cdr(t)))
        return a;
    return join("&", t);
}

const list<list<value> > queryArgs(const string& a) {
    return map<string, list<value>>(queryArg, tokenize("&", fixupQueryArgs(a)));
}

/**
 * Returns a list of key value pairs from the args in an HTTP request.
 */
const list<list<value> > queryArgs(const request_rec* r) {
    if (r->args == NULL)
        return list<list<value> >();
    return queryArgs(r->args);
}

/**
 * Converts a list of key value pairs to a query string.
 */
ostringstream& queryString(const list<list<value> > args, ostringstream& os) {
    if (isNil(args))
        return os;
    debug(car(args), "httpd::queryString::arg");
    os << car(car(args)) << "=" << c_str(cadr(car(args)));
    if (!isNil(cdr(args)))
        os << "&";
    return queryString(cdr(args), os);
}

const string queryString(const list<list<value> > args) {
    ostringstream os;
    return str(queryString(args, os));
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
    const size_t n = ap_get_client_block(r, b, sizeof(b));
    if (n <= 0)
        return list<string>();
    return cons(string(b, n), read(r));
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
    debug(ob, "httpd::writeResult");

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
 * Report a request execution status.
 */
const int reportStatus(const failable<int>& rc) {
    debug(rc, "httpd::reportStatus::rc");
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
 * Update request filters  in an HTTPD redirect request.
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
    nr->filename = apr_pstrdup(nr->pool, c_str(string("/redirected:") + nr_uri));
    nr->request_config = ap_create_request_config(r->pool);
    nr->per_dir_config = r->server->lookup_defaults;
    nr->prev = r;
    r->next = nr;

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
    debug(uri, "httpd::internalRedirect");
    const failable<request_rec*, int> nr = httpd::internalRedirectRequest(uri, r);
    if (!hasContent(nr))
        return reason(nr);
    return httpd::internalRedirect(content(nr));
}

/**
 * Create an HTTPD sub request.
 * Similar to httpd/server/request.c::make_sub_request
 */
const failable<request_rec*, int> internalSubRequest(const string& nr_uri, request_rec* r) {
    if (ap_is_recursion_limit_exceeded(r))
        return mkfailure<request_rec*, int>(HTTP_INTERNAL_SERVER_ERROR);

    // Create a new sub pool
    apr_pool_t *nrp;
    apr_pool_create(&nrp, r->pool);
    apr_pool_tag(nrp, "subrequest");

    // Create a new POST request
    request_rec* nr = (request_rec*)apr_pcalloc(nrp, sizeof(request_rec));
    nr->connection = r->connection;
    nr->server = r->server;
    nr->pool = nrp;
    nr->method = "POST";
    nr->method_number = M_POST;
    nr->allowed_methods = ap_make_method_list(nr->pool, 2);
    ap_parse_uri(nr, apr_pstrdup(nr->pool, c_str(nr_uri)));
    nr->filename = apr_pstrdup(nr->pool, c_str(string("/subreq:") + nr_uri));
    nr->request_config = ap_create_request_config(r->pool);
    nr->per_dir_config = r->server->lookup_defaults;

    // Inherit some of the protocol info from the parent request
    nr->the_request = r->the_request;
    nr->hostname = r->hostname;
    nr->request_time = r->request_time;
    nr->allowed = r->allowed;
    nr->status = HTTP_OK;
    nr->assbackwards = r->assbackwards;
    nr->header_only = r->header_only;
    nr->protocol = const_cast<char*>("INCLUDED");
    nr->hostname = r->hostname;
    nr->request_time = r->request_time;
    nr->main = r;
    nr->headers_in = apr_table_make(r->pool, 12);
    nr->headers_out = apr_table_make(r->pool, 12);
    nr->err_headers_out = apr_table_make(nr->pool, 5);
    nr->subprocess_env = r->subprocess_env;
    nr->subprocess_env  = apr_table_copy(nr->pool, r->subprocess_env);
    nr->notes = apr_table_make(r->pool, 5);
    nr->htaccess = r->htaccess;
    nr->no_cache = r->no_cache;
    nr->expecting_100 = r->expecting_100;
    nr->no_local_copy = r->no_local_copy;
    nr->read_length = 0;
    nr->vlist_validator = r->vlist_validator;
    nr->user = r->user;

    // Setup input and output filters
    nr->proto_output_filters = r->proto_output_filters;
    nr->proto_input_filters = r->proto_input_filters;
    nr->output_filters = nr->proto_output_filters;
    nr->input_filters = nr->proto_input_filters;
    ap_add_output_filter_handle(ap_subreq_core_filter_handle, NULL, nr, nr->connection);

    // Run create request hook
    ap_run_create_request(nr);
    nr->used_path_info = AP_REQ_DEFAULT_PATH_INFO;

    return nr;
}

/**
 * Return an HTTP external redirect request.
 */
const int externalRedirect(const string& uri, request_rec* r) {
    debug(uri, "httpd::externalRedirect");
    r->status = HTTP_MOVED_TEMPORARILY;
    apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, c_str(uri)));
    r->filename = apr_pstrdup(r->pool, c_str(string("/redirect:/") + uri));
    return HTTP_MOVED_TEMPORARILY;
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

#ifdef WANT_MAINTAINER_MODE

/**
 * Debug log.
 */

/**
 * Log an optional value.
 */
const char* debugOptional(const char* s) {
    if (s == NULL)
        return "";
    return s;
}

/**
 * Log a header
 */
int debugHeader(unused void* r, const char* key, const char* value) {
    gc_scoped_pool();
    cdebug << "  header key: " << key << ", value: " << value << endl;
    return 1;
}

/**
 * Log an environment variable
 */
int debugEnv(unused void* r, const char* key, const char* value) {
    gc_scoped_pool();
    cdebug << "  var key: " << key << ", value: " << value << endl;
    return 1;
}

/**
 * Log a note.
 */
int debugNote(unused void* r, const char* key, const char* value) {
    gc_scoped_pool();
    cdebug << "  note key: " << key << ", value: " << value << endl;
    return 1;
}

/**
 * Log a request.
 */
const bool debugRequest(request_rec* r, const string& msg) {
    gc_scoped_pool();
    cdebug << msg << ":" << endl;
    cdebug << "  unparsed uri: " << debugOptional(r->unparsed_uri) << endl;
    cdebug << "  uri: " << debugOptional(r->uri) << endl;
    cdebug << "  path info: " << debugOptional(r->path_info) << endl;
    cdebug << "  filename: " << debugOptional(r->filename) << endl;
    cdebug << "  uri tokens: " << pathTokens(r->uri) << endl;
    cdebug << "  args: " << debugOptional(r->args) << endl;
    cdebug << "  server: " << debugOptional(r->server->server_hostname) << endl;
    cdebug << "  protocol: " << debugOptional(r->protocol) << endl;
    cdebug << "  method: " << debugOptional(r->method) << endl;
    cdebug << "  method number: " << r->method_number << endl;
    cdebug << "  content type: " << contentType(r) << endl;
    cdebug << "  content encoding: " << debugOptional(r->content_encoding) << endl;
    apr_table_do(debugHeader, r, r->headers_in, NULL);
    cdebug << "  user: " << debugOptional(r->user) << endl;
    cdebug << "  auth type: " << debugOptional(r->ap_auth_type) << endl;
    apr_table_do(debugEnv, r, r->subprocess_env, NULL);
    apr_table_do(debugNote, r, r->notes, NULL);
    return true;
}

#define httpdDebugRequest(r, msg) httpd::debugRequest(r, msg)

#else

#define httpdDebugRequest(r, msg)

#endif

}
}

#endif /* tuscany_httpd_hpp */
