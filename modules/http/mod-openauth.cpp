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

#define WANT_HTTPD_LOG 1
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
    ServerConf(apr_pool_t* const p, server_rec* const s) : p(p), server(s) {
    }

    const gc_pool p;
    server_rec* const server;
};

/**
 * Authentication provider configuration.
 */
class AuthnProviderConf {
public:
    AuthnProviderConf() : name(), provider(NULL) {
    }
    AuthnProviderConf(const string name, const authn_provider* const provider) : name(name), provider(provider) {
    }

    const string name;
    const authn_provider* const provider;
};

/**
 * Directory configuration.
 */
class DirConf {
public:
    DirConf(apr_pool_t* const p, const char* d) : p(p), dir(d), enabled(false), login(emptyString) {
    }

    const gc_pool p;
    const char* const dir;
    bool enabled;
    gc_mutable_ref<string> login;
    gc_mutable_ref<list<AuthnProviderConf> > apcs;
};

#ifdef WANT_MAINTAINER_LOG

/**
 * Log session entries.
 */
int debugSessionEntry(unused void* r, const char* const key, const char* const value) {
    cdebug << "  session key: " << key << ", value: " << value << endl;
    return 1;
}

const bool debugSession(request_rec* const r, const session_rec* const z) {
    apr_table_do(debugSessionEntry, r, z->entries, NULL);
    return true;
}

#define debug_authSession(r, z) if(debug_islogging()) openauth::debugSession(r, z)

#else

#define debug_authSession(r, z)

#endif

/**
 * Session hook functions.
 */
static int (*ap_session_load_fn) (request_rec * r, session_rec ** z) = NULL;
static apr_status_t (*ap_session_get_fn) (request_rec * r, session_rec * z, const char *key, const char **value) = NULL;
static apr_status_t (*ap_session_set_fn)(request_rec * r, session_rec * z, const char *key, const char *value) = NULL;

/**
 * Run the authnz hooks to authenticate a request.
 */
const failable<int> checkAuthnzProviders(const string& user, const string& pw, request_rec* const r, const list<AuthnProviderConf>& apcs) {
    if(isNull(apcs))
        return mkfailure<int>("Authentication failure for: " + user, HTTP_UNAUTHORIZED);
    const AuthnProviderConf apc = car<AuthnProviderConf>(apcs);
    if(apc.provider == NULL || !apc.provider->check_password)
        return checkAuthnzProviders(user, pw, r, cdr(apcs));

    apr_table_setn(r->notes, AUTHN_PROVIDER_NAME_NOTE, c_str(apc.name));
    const authn_status auth_result = apc.provider->check_password(r, c_str(user), c_str(pw));
    apr_table_unset(r->notes, AUTHN_PROVIDER_NAME_NOTE);
    if(auth_result != AUTH_GRANTED)
        return checkAuthnzProviders(user, pw, r, cdr(apcs));
    return OK;
}

const failable<int> checkAuthnz(const string& user, const string& pw, request_rec* const r, const DirConf& dc) {
    if(substr(user, 0, 1) == "/" && pw == "password")
        return mkfailure<int>(string("Encountered FakeBasicAuth spoof: ") + user, HTTP_UNAUTHORIZED);

    if(isNull((const list<AuthnProviderConf>)dc.apcs)) {
        const authn_provider* provider = (const authn_provider*)ap_lookup_provider(AUTHN_PROVIDER_GROUP, AUTHN_DEFAULT_PROVIDER, AUTHN_PROVIDER_VERSION);
        return checkAuthnzProviders(user, pw, r, mklist<AuthnProviderConf>(AuthnProviderConf(AUTHN_DEFAULT_PROVIDER, provider)));
    }
    return checkAuthnzProviders(user, pw, r, dc.apcs);
}

/**
 * Return the user info from a form auth encrypted session cookie.
 */
const failable<value> userInfoFromSession(const string& realm, request_rec* const r) {
    debug("modopenauth::userInfoFromSession");
    session_rec *z = NULL;
    ap_session_load_fn(r, &z);
    if(z == NULL)
        return mkfailure<value>("Couldn't retrieve user session", HTTP_UNAUTHORIZED);
    debug_authSession(r, z);

    const char* user = NULL;
    ap_session_get_fn(r, z, c_str(realm + "-user"), &user);
    if(user == NULL)
        return mkfailure<value>("Couldn't retrieve user id", HTTP_UNAUTHORIZED);
    const char* pw = NULL;
    ap_session_get_fn(r, z, c_str(realm + "-pw"), &pw);
    if(pw == NULL)
        return mkfailure<value>("Couldn't retrieve password", HTTP_UNAUTHORIZED);
    return value(mklist<value>(mklist<value>("realm", realm), mklist<value>("id", string(user)), mklist<value>("password", string(pw))));
}

/**
 * Return the user info from a form auth session cookie.
 */
const failable<value> userInfoFromCookie(const value& sid, const string& realm, request_rec* const r) {
    const list<value> info = httpd::queryArgs(sid);
    debug(info, "modopenauth::userInfoFromCookie::info");
    const list<value> user = assoc<value>(realm + "-user", info);
    if(isNull(user))
        return userInfoFromSession(realm, r);
    const list<value> pw = assoc<value>(realm + "-pw", info);
    if(isNull(pw))
        return mkfailure<value>("Couldn't retrieve password", HTTP_UNAUTHORIZED);
    return value(mklist<value>(mklist<value>("realm", realm), mklist<value>("id", cadr(user)), mklist<value>("password", cadr(pw))));
}

/**
 * Return the user info from a basic auth header.
 */
const failable<value> userInfoFromHeader(const char* header, const string& realm, request_rec* const r) {
    debug(header, "modopenauth::userInfoFromHeader::header");
    if(strcasecmp(ap_getword(r->pool, &header, ' '), "Basic"))
        return mkfailure<value>("Wrong authentication scheme", HTTP_UNAUTHORIZED);

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
const failable<int> authenticated(const list<value>& info, request_rec* const r) {
    debug(info, "modopenauth::authenticated::info");

    // Store user info in the request
    const list<value> realm = assoc<value>("realm", info);
    if(isNull(realm) || isNull(cdr(realm)))
        return mkfailure<int>("Couldn't retrieve realm", HTTP_UNAUTHORIZED);
    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "REALM"), apr_pstrdup(r->pool, c_str(cadr(realm))));

    const list<value> id = assoc<value>("id", info);
    if(isNull(id) || isNull(cdr(id)))
        return mkfailure<int>("Couldn't retrieve user id", HTTP_UNAUTHORIZED);
    r->user = apr_pstrdup(r->pool, c_str(cadr(id)));

    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "NICKNAME"), apr_pstrdup(r->pool, c_str(cadr(id))));
    return OK;
}

/**
 * Check user authentication.
 */
static int checkAuthn(request_rec* const r) {
    const gc_scoped_pool sp(r->pool);

    // Decline if we're not enabled or AuthType is not set to Open
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_openauth);
    if(!dc.enabled)
        return DECLINED;
    const char* atype = ap_auth_type(r);
    debug_httpdRequest(r, "modopenauth::checkAuthn::input");
    debug(atype, "modopenauth::checkAuthn::auth_type");
    if(atype == NULL || strcasecmp(atype, "Open"))
        return DECLINED;
    debug(atype, "modopenauth::checkAuthn::auth_type");

    // Get the request args
    const list<value> args = httpd::queryArgs(r);

    // Get session id from the request
    const maybe<string> sid = sessionID(r, "TuscanyOpenAuth");
    if(hasContent(sid)) {
        // Decline if the session id was not created by this module
        const string stype = substr(content(sid), 0, 7);
        if(stype == "OAuth2_" || stype == "OAuth1_" || stype == "OpenID_")
            return DECLINED;

        // Retrieve the auth realm
        const char* aname = ap_auth_name(r);
        if(aname == NULL)
            return reportStatus(mkfailure<int>("Missing AuthName", HTTP_UNAUTHORIZED), dc.login, nilValue, r);

        // Extract user info from the session id
        const failable<value> userinfo = userInfoFromCookie(content(sid), aname, r);
        if(hasContent(userinfo)) {

            // Try to authenticate the request
            const value uinfo = content(userinfo);
            const failable<int> authz = checkAuthnz(cadr(assoc<value>("id", uinfo)), cadr(assoc<value>("password", uinfo)), r, dc);
            if(!hasContent(authz)) {

                // Authentication failed, redirect to login page
                r->ap_auth_type = const_cast<char*>(atype);
                return reportStatus(authz, dc.login, 1, r);
            }

            // Successfully authenticated, store the user info in the request
            r->ap_auth_type = const_cast<char*>(atype);
            return reportStatus(authenticated(uinfo, r), dc.login, nilValue, r);
        }
    }

    // Get basic auth header from the request
    const char* const header = apr_table_get(r->headers_in, (PROXYREQ_PROXY == r->proxyreq) ? "Proxy-Authorization" : "Authorization");
    if(header != NULL) {

        // Retrieve the auth realm
        const char* const aname = ap_auth_name(r);
        if(aname == NULL)
            return reportStatus(mkfailure<int>("Missing AuthName", HTTP_UNAUTHORIZED), dc.login, nilValue, r);

        // Extract user info from the session id
        const failable<value> info = userInfoFromHeader(header, aname, r);
        if(hasContent(info)) {

            // Try to authenticate the request
            const value uinfo = content(info);
            const failable<int> authz = checkAuthnz(cadr(assoc<value>("id", uinfo)), cadr(assoc<value>("password", uinfo)), r, dc);
            if(!hasContent(authz)) {

                // Authentication failed, redirect to login page
                r->ap_auth_type = const_cast<char*>(atype);
                return reportStatus(authz, dc.login, 1, r);
            }

            // Successfully authenticated, store the user info in the request
            r->ap_auth_type = const_cast<char*>(atype);
            return reportStatus(authenticated(uinfo, r), dc.login, nilValue, r);
        }
    }

    // Decline if the request is for another authentication provider
    if(!isNull(assoc<value>("openid_identifier", args)))
        return DECLINED;

    // Redirect to the login page, unless we have a session id from another module
    if(hasContent(sessionID(r, "TuscanyOpenIDAuth")) ||
        hasContent(sessionID(r, "TuscanyOAuth1")) ||
        hasContent(sessionID(r, "TuscanyOAuth2")))
        return DECLINED;

    r->ap_auth_type = const_cast<char*>(atype);
    return httpd::reportStatus(login(dc.login, nilValue, nilValue, r));
}

/**
 * Load the auth session cookie from the request.
 */
static int sessionCookieLoad(request_rec* const r, session_rec** const z) {
    const gc_scoped_pool sp(r->pool);

    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_openauth);
    if(!dc.enabled)
        return DECLINED;

    // First look in the notes
    const char* const note = apr_pstrcat(r->pool, "mod_openauth", "TuscanyOpenAuth", NULL);
    session_rec* zz = (session_rec*)(void*)apr_table_get(r->notes, note);
    if(zz != NULL) {
        *z = zz;
        return OK;
    }

    // Parse the cookie
    const maybe<string> sid = openauth::sessionID(r, "TuscanyOpenAuth");

    // Create a new session
    zz = (session_rec*)apr_pcalloc(r->pool, sizeof(session_rec));
    zz->pool = r->pool;
    zz->entries = apr_table_make(r->pool, 10);
    zz->encoded = hasContent(sid)? c_str(content(sid)) : NULL;
    zz->uuid = (apr_uuid_t *) apr_pcalloc(r->pool, sizeof(apr_uuid_t));
    *z = zz;

    // Store it in the notes
    apr_table_setn(r->notes, note, (char*)zz);

    return OK;
}

/**
 * Save the auth session cookie in the response.
 */
static int sessionCookieSave(request_rec* const r, session_rec* const z) {
    const gc_scoped_pool sp(r->pool);

    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_openauth);
    if(!dc.enabled)
        return DECLINED;
    if(z->encoded == NULL || *(z->encoded) == '\0') {
        const maybe<string> sid = sessionID(r, "TuscanyOpenAuth");
        if(!hasContent(sid))
            return OK;
    }

    debug(c_str(cookie("TuscanyOpenAuth", z->encoded, httpd::hostName(r))), "modopenauth::sessioncookiesave::setcookie");
    apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOpenAuth", z->encoded, httpd::hostName(r))));
    return OK;
}

/**
 * Logout request handler.
 */
int logoutHandler(request_rec* const r) {
    if(r->handler == NULL || strcmp(r->handler, "mod_tuscany_openauth_logout"))
        return DECLINED;

    const gc_scoped_pool sp(r->pool);
    debug_httpdRequest(r, "modopenauth::handler::input");
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_openauth);
    if(!dc.enabled)
        return DECLINED;

    // Clear the current session
    if(hasContent(sessionID(r, "TuscanyOpenAuth"))) {
        const char* const authname = ap_auth_name(r);
        session_rec* z = NULL;
        ap_session_load_fn(r, &z);
        if(z != NULL && authname != NULL) {
            ap_session_set_fn(r, z, apr_pstrcat(r->pool, authname, "-user", NULL), NULL);
            ap_session_set_fn(r, z, apr_pstrcat(r->pool, authname, "-pw", NULL), NULL);
            ap_session_set_fn(r, z, apr_pstrcat(r->pool, authname, "-site", NULL), NULL);
        } else
            apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOpenAuth", emptyString, httpd::hostName(r))));
    }
    if(hasContent(sessionID(r, "TuscanyOpenIDAuth")))
        apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOpenIDAuth", emptyString, httpd::hostName(r))));
    if(hasContent(sessionID(r, "TuscanyOAuth1")))
        apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOAuth1", emptyString, httpd::hostName(r))));
    if(hasContent(sessionID(r, "TuscanyOAuth2")))
        apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOAuth2", emptyString, httpd::hostName(r))));

    // Redirect to the login page
    return httpd::reportStatus(login(dc.login, "/", nilValue, r));
}

/**
 * Process the module configuration.
 */
int postConfigMerge(const ServerConf& mainsc, server_rec* const s) {
    if(s == NULL)
        return OK;
    debug(httpd::serverName(s), "modopenauth::postConfigMerge::serverName");

    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t* const p, unused apr_pool_t* const plog, unused apr_pool_t* const ptemp, server_rec* const s) {
    const gc_scoped_pool sp(p);

    const ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_openauth);
    debug(httpd::serverName(s), "modopenauth::postConfig::serverName");

    // Retrieve session hook functions
    if(ap_session_load_fn == NULL)
        ap_session_load_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_load);
    if(ap_session_get_fn == NULL)
        ap_session_get_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_get);
    if(ap_session_set_fn == NULL)
        ap_session_set_fn = APR_RETRIEVE_OPTIONAL_FN(ap_session_set);

    // Merge server configurations
    return postConfigMerge(sc, s);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* const p, server_rec* const s) {
    const gc_scoped_pool sp(p);

    const ServerConf* const psc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_openauth);
    if(psc == NULL) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_openauth loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
    const ServerConf& sc = *psc;

    // Merge the updated configuration into the virtual hosts
    postConfigMerge(sc, s->next);
}

/**
 * Configuration commands.
 */
char* confEnabled(cmd_parms* cmd, void *c, const int arg) {
    const gc_scoped_pool sp(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.enabled = (bool)arg;
    return NULL;
}
char* confLogin(cmd_parms *cmd, void *c, const char* arg) {
    const gc_scoped_pool sp(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.login = arg;
    return NULL;
}
char* confAuthnProvider(cmd_parms *cmd, void *c, const char* arg) {
    const gc_scoped_pool sp(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);

    // Lookup and cache the Authn provider
    const authn_provider* provider = (authn_provider*)ap_lookup_provider(AUTHN_PROVIDER_GROUP, arg, AUTHN_PROVIDER_VERSION);
    if(provider == NULL)
        return apr_psprintf(cmd->pool, "Unknown Authn provider: %s", arg);
    if(!provider->check_password)
        return apr_psprintf(cmd->pool, "The '%s' Authn provider doesn't support password authentication", arg);
    dc.apcs = append<AuthnProviderConf>(dc.apcs, mklist<AuthnProviderConf>(AuthnProviderConf(arg, provider)));
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_ITERATE("AuthOpenAuthProvider", (const char*(*)())confAuthnProvider, NULL, OR_AUTHCFG, "Auth providers for a directory or location"),
    AP_INIT_FLAG("AuthOpenAuth", (const char*(*)())confEnabled, NULL, OR_AUTHCFG, "Tuscany Open Auth authentication On | Off"),
    AP_INIT_TAKE1("AuthOpenAuthLoginPage", (const char*(*)())confLogin, NULL, OR_AUTHCFG, "Tuscany Open Auth login page"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};

void registerHooks(unused apr_pool_t *p) {
    ap_hook_post_config(postConfig, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(childInit, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_check_authn(checkAuthn, NULL, NULL, APR_HOOK_MIDDLE, AP_AUTH_INTERNAL_PER_CONF);
    ap_hook_session_load(sessionCookieLoad, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_session_save(sessionCookieSave, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(logoutHandler, NULL, NULL, APR_HOOK_MIDDLE);
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
