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
 * HTTPD module for Tuscany Open authentication.
 *
 * This module allows multiple authentication mechanisms to co-exist in a
 * single Web site:
 * - OAuth1 using Tuscany's mod-tuscany-oauth1
 * - OAuth2 using Tuscany's mod-tuscany-oauth2
 * - OpenID using mod_auth_openid
 * - Form-based using HTTPD's mod_auth_form
 * - SSL certificate using SSLFakeBasicAuth and mod_auth_basic
 */

#include <sys/stat.h>

#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "httpd.hpp"
#include "http.hpp"
#include "openauth.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_openauth;
}

namespace tuscany {
namespace openauth {

/**
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf(apr_pool_t* p, server_rec* s) : p(p), server(s) {
    }

    const gc_pool p;
    server_rec* server;
};

/**
 * Directory configuration.
 */
class DirConf {
public:
    DirConf(apr_pool_t* p, char* d) : p(p), dir(d), enabled(false), login("") {
    }

    const gc_pool p;
    const char* dir;
    bool enabled;
    string login;
};

#ifdef WANT_MAINTAINER_MODE

/**
 * Log a session entry.
 */
int debugSession(unused void* r, const char* key, const char* value) {
    cdebug << "  session key: " << key << ", value: " << value << endl;
    return 1;
}

#endif

/**
 * Return the user info from a form auth encrypted session cookie.
 */
static int (*ap_session_load_fn) (request_rec * r, session_rec ** z) = NULL;
static void (*ap_session_get_fn) (request_rec * r, session_rec * z, const char *key, const char **value) = NULL;

const failable<value> userInfoFromSession(const string& realm, request_rec* r) {
    debug("modopenauth::userInfoFromSession");
    if (ap_session_load_fn == NULL)
        ap_session_load_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_load);
    session_rec *z = NULL;
    ap_session_load_fn(r, &z);
    if (z == NULL)
        return mkfailure<value>("Couldn't retrieve user session");
#ifdef WANT_MAINTAINER_MODE
    apr_table_do(debugSession, r, z->entries, NULL);
#endif

    if (ap_session_get_fn == NULL)
        ap_session_get_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_get);
    const char* user = NULL;
    ap_session_get_fn(r, z, c_str(realm + "-user"), &user);
    if (user == NULL)
        return mkfailure<value>("Couldn't retrieve user id");
    const char* pw = NULL;
    ap_session_get_fn(r, z, c_str(realm + "-pw"), &pw);
    if (pw == NULL)
        return mkfailure<value>("Couldn't retrieve password");
    return value(mklist<value>(mklist<value>("realm", realm), mklist<value>("id", string(user)), mklist<value>("password", string(pw))));
}

/**
 * Return the user info from a form auth session cookie.
 */
const failable<value> userInfoFromCookie(const value& sid, const string& realm, request_rec* r) {
    const list<list<value>> info = httpd::queryArgs(sid);
    debug(info, "modopenauth::userInfoFromCookie::info");
    const list<value> user = assoc<value>(realm + "-user", info);
    if (isNil(user))
        return userInfoFromSession(realm, r);
    const list<value> pw = assoc<value>(realm + "-pw", info);
    if (isNil(pw))
        return mkfailure<value>("Couldn't retrieve password");
    return value(mklist<value>(mklist<value>("realm", realm), mklist<value>("id", cadr(user)), mklist<value>("password", cadr(pw))));
}

/**
 * Return the user info from a basic auth header.
 */
const failable<value> userInfoFromHeader(const char* header, const string& realm, request_rec* r) {
    debug(header, "modopenauth::userInfoFromHeader::header");
    if (strcasecmp(ap_getword(r->pool, &header, ' '), "Basic"))
        return mkfailure<value>("Wrong authentication scheme");

    while (apr_isspace(*header))
        header++;
    char *decoded_line = (char*)apr_palloc(r->pool, apr_base64_decode_len(header) + 1);
    int length = apr_base64_decode(decoded_line, header);
    decoded_line[length] = '\0';

    const string user(ap_getword_nulls(r->pool, const_cast<const char**>(&decoded_line), ':'));
    const string pw(decoded_line);

    return value(mklist<value>(mklist<value>("realm", realm), mklist<value>("id", user), mklist<value>("password", pw)));
}

/**
 * Handle an authenticated request.
 */
const failable<int> authenticated(const list<list<value> >& info, request_rec* r) {
    debug(info, "modopenauth::authenticated::info");

    // Store user info in the request
    const list<value> realm = assoc<value>("realm", info);
    if (isNil(realm) || isNil(cdr(realm)))
        return mkfailure<int>("Couldn't retrieve realm");
    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "REALM"), apr_pstrdup(r->pool, c_str(cadr(realm))));

    const list<value> id = assoc<value>("id", info);
    if (isNil(id) || isNil(cdr(id)))
        return mkfailure<int>("Couldn't retrieve user id");
    r->user = apr_pstrdup(r->pool, c_str(cadr(id)));

    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "NICKNAME"), apr_pstrdup(r->pool, c_str(cadr(id))));
    return OK;
}

/**
 * Run the authnz hooks to try to authenticate a request.
 */
const failable<int> checkAuthnz(const string& user, const string& pw, request_rec* r) {
    const authn_provider* provider = (const authn_provider*)ap_lookup_provider(AUTHN_PROVIDER_GROUP, AUTHN_DEFAULT_PROVIDER, AUTHN_PROVIDER_VERSION);
    if (!provider || !provider->check_password)
        return mkfailure<int>("No Authn provider configured");
    apr_table_setn(r->notes, AUTHN_PROVIDER_NAME_NOTE, AUTHN_DEFAULT_PROVIDER);
    const authn_status auth_result = provider->check_password(r, c_str(user), c_str(pw));
    apr_table_unset(r->notes, AUTHN_PROVIDER_NAME_NOTE);
    if (auth_result != AUTH_GRANTED)
        return mkfailure<int>("Authentication failure for: " + user);
    return OK;
}

/**
 * Check user authentication.
 */
static int checkAuthn(request_rec *r) {
    // Decline if we're not enabled or AuthType is not set to Open
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_openauth);
    if (!dc.enabled)
        return DECLINED;
    const char* atype = ap_auth_type(r);
    debug(atype, "modopenauth::checkAuthn::auth_type");
    if (atype == NULL || strcasecmp(atype, "Open"))
        return DECLINED;

    // Create a scoped memory pool
    gc_scoped_pool pool(r->pool);
    httpdDebugRequest(r, "modopenauth::checkAuthn::input");

    // Get session id from the request
    const maybe<string> sid = sessionID(r);
    if (hasContent(sid)) {
        // Decline if the session id was not created by this module
        const string stype = substr(content(sid), 0, 7);
        if (stype == "OAuth2_" || stype == "OAuth1_" || stype == "OpenID_")
            return DECLINED;

        // Retrieve the auth realm
        const char* aname = ap_auth_name(r);
        if (aname == NULL)
            return httpd::reportStatus(mkfailure<int>("Missing AuthName"));

        // Extract user info from the session id
        const failable<value> info = userInfoFromCookie(content(sid), aname, r);
        if (hasContent(info)) {

            // Try to authenticate the request
            const value cinfo = content(info);
            const failable<int> authz = checkAuthnz(cadr(assoc<value>("id", cinfo)), cadr(assoc<value>("password", cinfo)), r);
            if (!hasContent(authz)) {

                // Authentication failed, redirect to login page
                r->ap_auth_type = const_cast<char*>(atype);
                return httpd::reportStatus(login(dc.login, r));
            }

            // Successfully authenticated, store the user info in the request
            r->ap_auth_type = const_cast<char*>(atype);
            return httpd::reportStatus(authenticated(cinfo, r));
        }
    }

    // Get basic auth header from the request
    const char* header = apr_table_get(r->headers_in, (PROXYREQ_PROXY == r->proxyreq) ? "Proxy-Authorization" : "Authorization");
    if (header != NULL) {

        // Retrieve the auth realm
        const char* aname = ap_auth_name(r);
        if (aname == NULL)
            return httpd::reportStatus(mkfailure<int>("Missing AuthName"));

        // Extract user info from the session id
        const failable<value> info = userInfoFromHeader(header, aname, r);
        if (hasContent(info)) {

            // Try to authenticate the request
            const value cinfo = content(info);
            const failable<int> authz = checkAuthnz(cadr(assoc<value>("id", cinfo)), cadr(assoc<value>("password", cinfo)), r);
            if (!hasContent(authz)) {

                // Authentication failed, redirect to login page
                r->ap_auth_type = const_cast<char*>(atype);
                return httpd::reportStatus(login(dc.login, r));
            }

            // Successfully authenticated, store the user info in the request
            r->ap_auth_type = const_cast<char*>(atype);
            return httpd::reportStatus(authenticated(cinfo, r));
        }
    }

    // Get the request args
    const list<list<value> > args = httpd::queryArgs(r);

    // Decline if the request is for another authentication provider
    if (!isNil(assoc<value>("openid_identifier", args)))
        return DECLINED;
    if (!isNil(assoc<value>("mod_oauth1_step", args)))
        return DECLINED;
    if (!isNil(assoc<value>("mod_oauth2_step", args)))
        return DECLINED;

    // Redirect to the login page
    r->ap_auth_type = const_cast<char*>(atype);
    return httpd::reportStatus(login(dc.login, r));
}

/**
 * Fixup cache control.
 */
bool filterCacheControl(const string& tok) {
    return tok != "no-cache";
}

static apr_status_t outputFilter(ap_filter_t * f, apr_bucket_brigade * in) {
    request_rec *r = f->r->main;
    if (!r)
        r = f->r;
    for (; r != NULL; r = r->next) {
        if (r->status != HTTP_OK && r->status != HTTP_NOT_MODIFIED) {

            // Don't cache errors and redirects
            debug("no-cache", "modopenauth::outputFilter::nokCacheControl");
            apr_table_set(r->headers_out, "Cache-Control", "no-cache");
            continue;
        }

        // Cache OK content
        const char* cc = apr_table_get(r->headers_out, "Cache-Control");
        if (cc == NULL) {
            debug("modopenauth::outputFilter::noCacheControl");
            continue;
        }
        debug(cc, "modopenauth::outputFilter::cacheControl");
        const string ncc = join(", ", filter<string>(filterCacheControl, tokenize(", ", cc)));
        if (length(ncc) == 0) {
            debug("modopenauth::outputFilter::noCacheControl");
            apr_table_unset(r->headers_out, "Cache-Control");
            continue;
        }

        debug(ncc, "modopenauth::outputFilter::okCacheControl");
        apr_table_set(r->headers_out, "Cache-Control", c_str(ncc));
    }

    ap_remove_output_filter(f);
    return ap_pass_brigade(f->next, in);
}

/**
 * Insert our cache control output filter.
 */
static void insertOutputFilter(request_rec * r) {
    ap_add_output_filter("mod_openauth", NULL, r, r->connection);
}

/**
 * Process the module configuration.
 */
int postConfigMerge(ServerConf& mainsc, server_rec* s) {
    if (s == NULL)
        return OK;
    debug(httpd::serverName(s), "modopenauth::postConfigMerge::serverName");

    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t* p, unused apr_pool_t* plog, unused apr_pool_t* ptemp, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_openauth);
    debug(httpd::serverName(s), "modopenauth::postConfig::serverName");

    // Merge server configurations
    return postConfigMerge(sc, s);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* p, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf* psc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_openauth);
    if(psc == NULL) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_openauth loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
    ServerConf& sc = *psc;

    // Merge the updated configuration into the virtual hosts
    postConfigMerge(sc, s->next);
}

/**
 * Configuration commands.
 */
const char* confEnabled(cmd_parms *cmd, void *c, const int arg) {
    gc_scoped_pool pool(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.enabled = (bool)arg;
    return NULL;
}
const char* confLogin(cmd_parms *cmd, void *c, const char* arg) {
    gc_scoped_pool pool(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.login = arg;
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_FLAG("AuthOpenAuth", (const char*(*)())confEnabled, NULL, OR_AUTHCFG, "Tuscany Open Auth authentication On | Off"),
    AP_INIT_TAKE1("AuthOpenAuthLoginPage", (const char*(*)())confLogin, NULL, OR_AUTHCFG, "Tuscany Open Auth login page"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};

void registerHooks(unused apr_pool_t *p) {
    ap_hook_post_config(postConfig, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(childInit, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_check_authn(checkAuthn, NULL, NULL, APR_HOOK_MIDDLE, AP_AUTH_INTERNAL_PER_CONF);
    ap_register_output_filter("mod_openauth", outputFilter, NULL, AP_FTYPE_CONTENT_SET);
    ap_hook_insert_filter(insertOutputFilter, NULL, NULL, APR_HOOK_LAST);
}

}
}

extern "C" {

module AP_MODULE_DECLARE_DATA mod_tuscany_openauth = {
    STANDARD20_MODULE_STUFF,
    // dir config and merger
    tuscany::httpd::makeDirConf<tuscany::openauth::DirConf>, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::openauth::ServerConf>, NULL,
    // commands and hooks
    tuscany::openauth::commands, tuscany::openauth::registerHooks
};

}
