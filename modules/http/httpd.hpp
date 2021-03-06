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

extern "C" {
#include <apr_strings.h>
#include <apr_fnmatch.h>
#include <apr_lib.h>
#define APR_WANT_STRFUNC
#include <apr_want.h>
#include <apr_base64.h>

#include <httpd.h>
// Hack to workaround compile error with CLang/LLVM
#undef strtoul
// Hack to workaround compile error with HTTPD 2.3.8
#define new new_
#include <http_config.h>
#undef new
#include <http_core.h>
#include <http_connection.h>
#include <http_request.h>
// Ignore conversion warnings in HTTPD 2.3.15 header
#ifdef WANT_MAINTAINER_WARNINGS
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#endif
#include <http_protocol.h>
// Re-enable conversion warnings
#ifdef WANT_MAINTAINER_WARNINGS
#ifndef __clang__
#pragma GCC diagnostic warning "-Wconversion"
#endif
#endif
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
}

#include "string.hpp"
#include "stream.hpp"
#include "sstream.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "http.hpp"


namespace tuscany {
namespace httpd {

/**
 * Returns a server-scoped module configuration.
 */
template<typename C> void* makeServerConf(apr_pool_t* p, server_rec* s) {
    return new (gc_new<C>(p)) C(p, s);
}

template<typename C> const C& serverConf(const request_rec* const r, const module* const mod) {
    return *(C*)ap_get_module_config(r->server->module_config, mod);
}

template<typename C> C& serverConf(const server_rec* const s, const module* const mod) {
    return *(C*)ap_get_module_config(s->module_config, mod);
}

template<typename C> C& serverConf(const cmd_parms* const cmd, const module* const mod) {
    return *(C*)ap_get_module_config(cmd->server->module_config, mod);
}

/**
 * Returns a directory-scoped module configuration.
 */
template<typename C> void* makeDirConf(apr_pool_t *p, char* d) {
    return new (gc_new<C>(p)) C(p, d);
}

template<typename C> const C& dirConf(const request_rec* const r, const module* const mod) {
    return *(C*)ap_get_module_config(r->per_dir_config, mod);
}

template<typename C> C& dirConf(const void* const c) {
    return *(C*)c;
}

/**
 * Returns a request-scoped module configuration.
 */
template<typename C> C& makeRequestConf(const request_rec* const r, const module* const mod) {
    C* const c = new (gc_new<C>(r->pool)) C(r->pool, r);
    ap_set_module_config(r->request_config, mod, c);
    return *c;
}

template<typename C> C& requestConf(const request_rec* const r, const module* const mod) {
    C* const c = (C*)ap_get_module_config(r->request_config, mod);
    if (c == NULL)
        return makeRequestConf<C>(r, mod);
    return *c;
}

/**
 * Return the host name for a server.
 */
const string hostName(const server_rec* const s, const string& def = "localhost") {
    return s->server_hostname != NULL? s->server_hostname : def;
}

/**
 * Return the host name from an HTTP request.
 */
const string hostName(request_rec* const r, const string& def = "localhost") {
    const char* const fh = apr_table_get(r->headers_in, "X-Forwarded-Server");
    if (fh != NULL)
        return fh;
    const char* const h = ap_get_server_name(r);
    return h != NULL? h : (r->server->server_hostname != NULL? r->server->server_hostname : def);
}

/**
 * Convert a host name to a realm.
 */
const string realm(const string& host) {
    const string pre = substr(host, 0, 4);
    return pre == "www." || pre == "ww1." || pre == "ww2." || pre == "ww3."? substr(host, 4) : host;
}

/**
 * Return the protocol scheme for a server.
 */
const string scheme(const server_rec* const s, const string& def = "http") {
    return s->server_scheme != NULL? s->server_scheme : def;
}

/**
 * Return the protocol scheme from an HTTP request.
 */
const string scheme(const request_rec* const r, const string& def = "http") {
    const char* const fs = apr_table_get(r->headers_in, "X-Forwarded-HTTPS");
    if (fs != NULL)
        return !strcmp(fs, "on")? "https" : "http";
    return r->server->server_scheme != NULL? r->server->server_scheme : def;
}

/**
 * Return the port number for a server.
 */
const int port(const server_rec* const s, const int def = 80) {
    return s->port != 0? s->port : def;
}

/**
 * Return the port number from an HTTP request.
 */
const int port(const request_rec* const r, const int def = 80) {
    const char* const fp = apr_table_get(r->headers_in, "X-Forwarded-Port");
    if (fp != NULL)
        return atoi(fp);
    const int p = ap_get_server_port(r);
    return p != 0? p : def;
}

/**
 * Return the name of a server.
 */
const string serverName(server_rec* const s, const string& def = "localhost") {
    ostringstream n;
    const string sc = scheme(s);
    const string h = hostName(s, def);
    const int p = port(s, sc == "https"? 443 : 80);
    n << sc << "://" << h;
    if (!((sc == "http" && p == 80) || (sc == "https" && p == 443)))
        n << ":" << p;
    n << (s->path != NULL? string(s->path, s->pathlen) : emptyString);
    return str(n);
}

/**
 * Determine the name of a server from an HTTP request.
 */
const string serverName(request_rec* const r, const string& def = "localhost") {
    ostringstream n;
    const string s = scheme(r);
    const string h = hostName(r, def);
    const int p = port(r, s == "https"? 443 : 80);
    n << s << "://" << h;
    if (!((s == "http" && p == 80) || (s == "https" && p == 443)))
        n << ":" << p;
    n << (r->server->path != NULL? string(r->server->path, r->server->pathlen) : emptyString);
    return str(n);
}

/**
 * Return true if a request is targeting a virtual host.
 */
const bool isVhostRequest(const server_rec* const s, const string& d, request_rec* const r) {
    const string rh = hostName(r);
    return rh != hostName(s) && http::topDomain(rh) == d;
}

/**
 * Return the content type of a request.
 */
const string contentType(const request_rec* const r) {
    const char* ct = apr_table_get(r->headers_in, "Content-Type");
    if (ct == NULL)
        return emptyString;
    return ct;
}

/**
 * Return the cookie header of a request.
 */
const string cookie(const request_rec* const r) {
    const char* c = apr_table_get(r->headers_in, "Cookie");
    if (c == NULL)
        return emptyString;
    return c;
}

/**
 * Return the remaining part of a uri after the given path (aka the path info.)
 */
const list<value> pathInfo(const list<value>& uri, const list<value>& path) {
    if (isNull(path))
        return uri;
    return pathInfo(cdr(uri), cdr(path));
}

/**
 * Convert a URI to an absolute URL.
 */
const string url(const string& uri, request_rec* const r) {
    if (contains(uri, "://"))
        return uri;
    ostringstream n;
    const string s = scheme(r);
    const string h = hostName(r, "localhost");
    const int p = port(r, s == "https"? 443 : 80);
    n << s << "://" << h;
    if (!((s == "http" && p == 80) || (s == "https" && p == 443)))
        n << ":" << p;
    n << uri;
    return str(n);
}

/**
 * Convert a URI and a path to an absolute URL.
 */
const string url(const string& uri, const list<value>& p, request_rec* const r) {
    return url(uri + (string)path(p), r);
}

/**
 * Escape a URI.
 */
const char escape_c2x[] = "0123456789ABCDEF";
const string escape(const string& uri) {
    debug(uri, "httpd::escape::uri");
    char* const copy = (char*)apr_palloc(gc_current_pool(), 3 * length(uri) + 3);
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
    char* const b = const_cast<char*>(c_str(string(c_str(uri))));
    ap_unescape_url(b);
    debug(b, "httpd::unescape::result");
    return b;
}

/**
 * Unescape a list of key of value pairs representing query args.
 */
const value unescapeArg(const value& a) {
    return mklist<value>(car<value>(a), unescape(cadr<value>(a)));
}

const list<value> unescapeArgs(const list<value>& args) {
    debug(args, "httpd::unescape::args");
    const list<value> uargs = map<value, value>(unescapeArg, args);
    debug(uargs, "httpd::unescape::result");
    return uargs;
}

/**
 * Returns a list of key value pairs from the args in a query string.
 */
const value queryArg(const string& s) {
    debug(s, "httpd::queryArg::string");
    const list<string> t = tokenize("=", s);
    if (isNull(cdr(t)))
        return mklist<value>(c_str(car(t)), emptyString);
    return mklist<value>(c_str(car(t)), cadr(t));
}

const string fixupQueryArgs(const string& a) {
    const list<string> t = tokenize("?", a);
    if (isNull(t) || isNull(cdr(t)))
        return a;
    return join("&", t);
}

const list<value> queryArgs(const string& a) {
    return map<string, value>(queryArg, tokenize("&", fixupQueryArgs(a)));
}

/**
 * Returns a list of key value pairs from the args in an HTTP request.
 */
const list<value> queryArgs(const request_rec* const r) {
    if (r->args == NULL)
        return nilListValue;
    return queryArgs(r->args);
}

/**
 * Converts the args received in a POST to a list of key value pairs.
 */
const list<value> postArgs(const list<value>& a) {
    if (isNull(a))
        return nilListValue;
    const list<value> l = car(a);
    return cons<value>(l, postArgs(cdr(a)));
}

/**
 * Setup the HTTP read policy.
 */
const int setupReadPolicy(request_rec* const r) {
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
 * Read the content of an HTTP request.
 */
const list<string> read(request_rec* const r) {
    char b[1024];
    const size_t n = ap_get_client_block(r, b, sizeof(b));
    if (n <= 0)
        return list<string>();
    return cons(string(b, n), read(r));
}

/**
 * Write an HTTP result.
 */
const failable<int> writeResult(const failable<list<string> >& ls, const string& ct, request_rec* const r) {
    if (!hasContent(ls))
        return mkfailure<int>(ls);
    ostringstream os;
    write(content(ls), os);
    const string ob(str(os));

    // Make sure browsers come back and check for updated dynamic content
    apr_table_setn(r->headers_out, "Cache-Control", "must-revalidate, max-age=0");

    // Compute and return an Etag for the returned content
    const string etag(ap_md5_binary(r->pool, (const unsigned char*)c_str(ob), (int)length(ob)));

    // Check for an If-None-Match header and just return a 304 not-modified status
    // if the Etag matches the Etag presented by the client, to save bandwith
    const char* match = apr_table_get(r->headers_in, "If-None-Match");
    apr_table_setn(r->headers_out, "ETag", apr_pstrdup(r->pool, c_str(etag)));
    if (match != NULL  && etag == match) {
        r->status = HTTP_NOT_MODIFIED;
        debug(r->status, "httpd::writeResult::status");
        return OK;
    }

    debug(r->status, "httpd::writeResult::status");
    debug(ct, "httpd::writeResult::contentType");
    debug(ob, "httpd::writeResult::content");
    ap_set_content_type(r, apr_pstrdup(r->pool, c_str(ct)));
    ap_rwrite(c_str(ob), (int)length(ob), r);
    return OK;
}

/**
 * Report a request execution status.
 */
const int reportStatus(const failable<int>& rc) {
    debug(rc, "httpd::reportStatus::rc");
    if (!hasContent(rc)) {
        const int r = rcode(rc);
        return r == -1 ? HTTP_INTERNAL_SERVER_ERROR : r;
    }
    return content(rc);
}

/**
 * Convert a value to an HTTPD request struct.
 */
request_rec* request(const value& v) {
    return (request_rec*)(long)(double)v;
}

/**
 * Convert an HTTPD request struct to a value.
 */
const value requestValue(const request_rec* const r) {
    return value((double)(long)r);
}

/**
 * Update request filters  in an HTTPD redirect request.
 * Similar to httpd/modules/http/http_request.c::update_r_in_filters.
 */
const bool internalUpdateFilters(ap_filter_t *f, const request_rec* const from,  request_rec* const to) {
    while(f) {
        if(f->r == from)
            f->r = to;
        f = f->next;
    }
    return true;
}

/**
 * Rename original env in an HTTPD redirect request.
 * Similar to httpd/modules/http/http_request.c::rename_original_env.
 */
apr_table_t* const internalRenameOriginalEnv(apr_pool_t* const p, apr_table_t* const t) {
    const apr_array_header_t *env_arr = apr_table_elts(t);
    const apr_table_entry_t *elts = (const apr_table_entry_t *) (void*)env_arr->elts;
    apr_table_t *nt = apr_table_make(p, env_arr->nalloc);
    int i;
    for(i = 0; i < env_arr->nelts; ++i) {
        if (!elts[i].key)
            continue;
        apr_table_setn(nt, apr_pstrcat(p, "REDIRECT_", elts[i].key, NULL), elts[i].val);
    }
    return nt;
}

/**
 * Create an HTTPD internal redirect request.
 * Similar to httpd/modules/http/http_request.c::internal_internal_redirect.
 */
const failable<request_rec*> internalRedirectRequest(const string& nr_uri, request_rec* const r) {
    if (ap_is_recursion_limit_exceeded(r))
        return mkfailure<request_rec*>("Redirect recursion limit exceeded", HTTP_INTERNAL_SERVER_ERROR);

    // Create a new request
    request_rec* nr = (request_rec*)apr_pcalloc(r->pool, sizeof(request_rec));
    nr->connection = r->connection;
    nr->server = r->server;
    nr->pool = r->pool;
    nr->method = r->method;
    nr->method_number = r->method_number;
    nr->allowed_methods = ap_make_method_list(nr->pool, 2);
    ap_parse_uri(nr, apr_pstrdup(nr->pool, c_str(nr_uri)));
    nr->parsed_uri.port_str = r->parsed_uri.port_str;
    nr->parsed_uri.port = r->parsed_uri.port;
    nr->filename = apr_pstrdup(nr->pool, c_str(string("/redirected:") + nr_uri));
    nr->request_config = ap_create_request_config(r->pool);
    nr->per_dir_config = r->server->lookup_defaults;
    nr->prev = r;
    r->next = nr;
    nr->useragent_addr = r->useragent_addr;
    nr->useragent_ip = r->useragent_ip;

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
    if (ap_is_HTTP_REDIRECT(nr->status)) {
        const char *location = apr_table_get(r->headers_out, "Location");
        if (location)
            apr_table_setn(nr->headers_out, "Location", location);
    }
    nr->err_headers_out = r->err_headers_out;
    nr->subprocess_env = r->subprocess_env;
    nr->subprocess_env  = internalRenameOriginalEnv(r->pool, r->subprocess_env);
    nr->notes = apr_table_make(r->pool, 5);
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
    if (nr->main) {
        ap_filter_t *f, *nextf;
        nr->output_filters = r->output_filters;
        f = nr->output_filters;
        do {
            nextf = f->next;
            if (f->r == r && f->frec != ap_subreq_core_filter_handle) {
                f->r = nr;
                ap_remove_output_filter(f);
            }
            f = nextf;
        } while(f && f != nr->proto_output_filters);
    }
    else {
        nr->output_filters  = nr->proto_output_filters;
    }
    internalUpdateFilters(nr->input_filters, r, nr);
    internalUpdateFilters(nr->output_filters, r, nr);

    apr_table_setn(nr->subprocess_env, "REDIRECT_STATUS", apr_itoa(r->pool, r->status));
    nr->used_path_info = AP_REQ_DEFAULT_PATH_INFO;

    const int rrc = ap_run_post_read_request(nr);
    if (rrc != OK && rrc != DECLINED)
        return mkfailure<request_rec*>("Error handling internal redirect", rrc);

    return nr;
}

/**
 * Process an HTTPD internal redirect request.
 * Similar to httpd/modules/http/http_request.c::ap_internal_redirect.
 */
const int internalRedirect(request_rec* const nr) {
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
const int internalRedirect(const string& uri, request_rec* const r) {
    debug(uri, "httpd::internalRedirect");
    //ap_internal_redirect(c_str(uri), r);
    //return OK;
    const failable<request_rec*> nr = httpd::internalRedirectRequest(uri, r);
    if (!hasContent(nr))
        return rcode(nr);
    return httpd::internalRedirect(content(nr));
}

/**
 * Create an HTTPD sub request.
 * Similar to httpd/server/request.c::make_sub_request
 */
const failable<request_rec*> internalSubRequest(const string& nr_uri, request_rec* const r) {
    if (ap_is_recursion_limit_exceeded(r))
        return mkfailure<request_rec*>("Redirect recursion limit exceeded", HTTP_INTERNAL_SERVER_ERROR);

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
const int externalRedirect(const string& uri, request_rec* const r) {
    debug(uri, "httpd::externalRedirect");
    r->status = HTTP_MOVED_TEMPORARILY;
    apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, c_str(uri)));
    apr_table_setn(r->headers_out, "Cache-Control", "no-cache, no-store, must-revalidate, max-age=0");
    apr_table_setn(r->err_headers_out, "Cache-Control", "no-cache, no-store, must-revalidate, max-age=0");
    r->filename = apr_pstrdup(r->pool, c_str(string("/redirect:/") + uri));
    return HTTP_MOVED_TEMPORARILY;
}

/**
 * Put a value in the process user data.
 */
const bool putUserData(const string& k, const void* const v, const server_rec* const s) {
    apr_pool_userdata_set((const void *)v, c_str(k), apr_pool_cleanup_null, s->process->pool);
    return true;
}

/**
 * Return a user data value.
 */
const void* const userData(const string& k, const server_rec* const s) {
    void* v = NULL;
    apr_pool_userdata_get(&v, c_str(k), s->process->pool);
    return v;
}

#ifdef WANT_MAINTAINER_LOG

/**
 * Debug log.
 */

/**
 * Log an optional value.
 */
const char* const debugOptional(const char* const s) {
    if (s == NULL)
        return "";
    return s;
}

/**
 * Log a header
 */
int debugHeader(unused void* r, const char* const key, const char* const value) {
    cdebug << "  header key: " << key << ", value: " << value << endl;
    return 1;
}

/**
 * Log an environment variable
 */
int debugEnv(unused void* r, const char* const key, const char* const value) {
    cdebug << "  var key: " << key << ", value: " << value << endl;
    return 1;
}

/**
 * Log a note.
 */
int debugNote(unused void* r, const char* const key, const char* const value) {
    cdebug << "  note key: " << key << ", value: " << value << endl;
    return 1;
}

/**
 * Log a request.
 */
const bool debugRequest(request_rec* const r, const string& msg) {
    const gc_scoped_pool pool;
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

#define debug_httpdRequest(r, msg) do { if (debug_islogging()) httpd::debugRequest(r, msg); } while(0)

#else

#define debug_httpdRequest(r, msg)

#endif

}
}

#endif /* tuscany_httpd_hpp */
