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
 * HTTPD module for OAuth 2.0 authentication.
 */

#include <sys/stat.h>

#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../http/httpd.hpp"
#include "../http/http.hpp"
#include "../http/openauth.hpp"
#include "../../components/cache/memcache.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_oauth2;
}

namespace tuscany {
namespace oauth2 {

/**
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf(apr_pool_t* p, server_rec* s) : p(p), server(s) {
    }

    const gc_pool p;
    server_rec* server;
    string ca;
    string cert;
    string key;
    list<list<value> > appkeys;
    list<string> mcaddrs;
    memcache::MemCached mc;
    http::CURLSession cs;
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

/**
 * Return the user info for a session.
 */
const failable<value> userInfo(const value& sid, const memcache::MemCached& mc) {
    return memcache::get(mklist<value>("tuscanyOpenAuth", sid), mc);
}

/**
 * Handle an authenticated request.
 */
const failable<int> authenticated(const list<list<value> >& info, request_rec* r) {
    debug(info, "modoauth2::authenticated::info");

    // Store user info in the request
    const list<value> realm = assoc<value>("realm", info);
    if (isNil(realm) || isNil(cdr(realm)))
        return mkfailure<int>("Couldn't retrieve realm");
    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "REALM"), apr_pstrdup(r->pool, c_str(cadr(realm))));

    const list<value> id = assoc<value>("id", info);
    if (isNil(id) || isNil(cdr(id)))
        return mkfailure<int>("Couldn't retrieve user id");
    r->user = apr_pstrdup(r->pool, c_str(cadr(id)));

    const list<value> email = assoc<value>("email", info);
    if (!isNil(email) && !isNil(cdr(email)))
        apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "EMAIL"), apr_pstrdup(r->pool, c_str(cadr(email))));

    const list<value> fullname = assoc<value>("name", info);
    if (!isNil(fullname) && !isNil(cdr(fullname))) {
        apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "NICKNAME"), apr_pstrdup(r->pool, c_str(cadr(fullname))));
        apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "FULLNAME"), apr_pstrdup(r->pool, c_str(cadr(fullname))));
    }

    const list<value> firstname = assoc<value>("first_name", info);
    if (!isNil(firstname) && !isNil(cdr(firstname)))
        apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "FIRSTNAME"), apr_pstrdup(r->pool, c_str(cadr(firstname))));

    const list<value> lastname = assoc<value>("last_name", info);
    if (!isNil(lastname) && !isNil(cdr(lastname)))
        apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "LASTNAME"), apr_pstrdup(r->pool, c_str(cadr(lastname))));

    return OK;
}

/**
 * Handle an authorize request.
 */
const failable<int> authorize(const list<list<value> >& args, request_rec* r, const ServerConf& sc) {
    // Extract authorize, access_token, client ID and info URIs
    const list<value> auth = assoc<value>("mod_oauth2_authorize", args);
    if (isNil(auth) || isNil(cdr(auth)))
        return mkfailure<int>("Missing mod_oauth2_authorize parameter");
    const list<value> tok = assoc<value>("mod_oauth2_access_token", args);
    if (isNil(tok) || isNil(cdr(tok)))
        return mkfailure<int>("Missing mod_oauth2_access_token parameter");
    const list<value> cid = assoc<value>("mod_oauth2_client_id", args);
    if (isNil(cid) || isNil(cdr(cid)))
        return mkfailure<int>("Missing mod_oauth2_client_id parameter");
    const list<value> info = assoc<value>("mod_oauth2_info", args);
    if (isNil(info) || isNil(cdr(info)))
        return mkfailure<int>("Missing mod_oauth2_info parameter");

    // Build the redirect URI
    const list<list<value> > rargs = mklist<list<value> >(mklist<value>("mod_oauth2_step", "access_token"), tok, cid, info);
    const string redir = httpd::url(r->uri, r) + string("?") + http::queryString(rargs);
    debug(redir, "modoauth2::authorize::redir");

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), sc.appkeys);
    if (isNil(app) || isNil(cdr(app)))
        return mkfailure<int>(string("client id not found: ") + cadr(cid));
    list<value> appkey = cadr(app);

    // Redirect to the authorize URI
    const list<list<value> > aargs = mklist<list<value> >(mklist<value>("client_id", car(appkey)), mklist<value>("scope", "email"), mklist<value>("redirect_uri", httpd::escape(redir)));
    const string uri = httpd::unescape(cadr(auth)) + string("?") + http::queryString(aargs);
    debug(uri, "modoauth2::authorize::uri");
    return httpd::externalRedirect(uri, r);
}

/**
 * Extract user info from a profile/info response.
 * TODO This currently only works for Facebook and Gowalla.
 * User profile parsing needs to be made configurable.
 */
const failable<list<value> > profileUserInfo(const value& cid, const list<value>& info) {
    return cons<value>(mklist<value>("realm", cid), info);
}

/**
 * Handle an access_token request.
 */
const failable<int> access_token(const list<list<value> >& args, request_rec* r, const ServerConf& sc) {
    // Extract access_token URI, client ID and authorization code
    const list<value> tok = assoc<value>("mod_oauth2_access_token", args);
    if (isNil(tok) || isNil(cdr(tok)))
        return mkfailure<int>("Missing mod_oauth2_access_token parameter");
    const list<value> cid = assoc<value>("mod_oauth2_client_id", args);
    if (isNil(cid) || isNil(cdr(cid)))
        return mkfailure<int>("Missing mod_oauth2_client_id parameter");
    const list<value> info = assoc<value>("mod_oauth2_info", args);
    if (isNil(info) || isNil(cdr(info)))
        return mkfailure<int>("Missing mod_oauth2_info parameter");
    const list<value> code = assoc<value>("code", args);
    if (isNil(code) || isNil(cdr(code)))
        return mkfailure<int>("Missing code parameter");

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), sc.appkeys);
    if (isNil(app) || isNil(cdr(app)))
        return mkfailure<int>(string("client id not found: ") + cadr(cid));
    list<value> appkey = cadr(app);

    // Build the redirect URI
    const list<list<value> > rargs = mklist<list<value> >(mklist<value>("mod_oauth2_step", "access_token"), tok, cid, info);
    const string redir = httpd::url(r->uri, r) + string("?") + http::queryString(rargs);
    debug(redir, "modoauth2::access_token::redir");

    // Request access token
    const list<list<value> > targs = mklist<list<value> >(mklist<value>("client_id", car(appkey)), mklist<value>("redirect_uri", httpd::escape(redir)), mklist<value>("client_secret", cadr(appkey)), code);
    const string turi = httpd::unescape(cadr(tok)) + string("?") + http::queryString(targs);
    debug(turi, "modoauth2::access_token::tokenuri");
    const failable<value> tr = http::get(turi, sc.cs);
    if (!hasContent(tr))
        return mkfailure<int>(reason(tr));
    debug(tr, "modoauth2::access_token::response");
    const list<value> tv = assoc<value>("access_token", httpd::queryArgs(join("", convertValues<string>(content(tr)))));
    if (isNil(tv) || isNil(cdr(tv)))
        return mkfailure<int>("Couldn't retrieve access_token");
    debug(tv, "modoauth2::access_token::token");

    // Request user info
    // TODO Make this step configurable
    const list<list<value> > iargs = mklist<list<value> >(tv);
    const string iuri = httpd::unescape(cadr(info)) + string("?") + http::queryString(iargs);
    debug(iuri, "modoauth2::access_token::infouri");
    const failable<value> profres = http::get(iuri, sc.cs);
    if (!hasContent(profres))
        return mkfailure<int>("Couldn't retrieve user info");
    debug(content(profres), "modoauth2::access_token::info");

    // Retrieve the user info from the profile
    const failable<list<value> > iv = profileUserInfo(cadr(cid), content(profres));
    if (!hasContent(iv))
        return mkfailure<int>(reason(iv));

    // Store user info in memcached keyed by session ID
    const value sid = string("OAuth2_") + mkrand();
    const failable<bool> prc = memcache::put(mklist<value>("tuscanyOpenAuth", sid), content(iv), sc.mc);
    if (!hasContent(prc))
        return mkfailure<int>(reason(prc));

    // Send session ID to the client in a cookie
    apr_table_set(r->err_headers_out, "Set-Cookie", c_str(openauth::cookie(sid, httpd::hostName(sc.server))));
    return httpd::externalRedirect(httpd::url(r->uri, r), r);
}

/**
 * Check user authentication.
 */
static int checkAuthn(request_rec *r) {
    // Decline if we're not enabled or AuthType is not set to Open
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_oauth2);
    if (!dc.enabled)
        return DECLINED;
    const char* atype = ap_auth_type(r);
    if (atype == NULL || strcasecmp(atype, "Open"))
        return DECLINED;

    // Create a scoped memory pool
    gc_scoped_pool pool(r->pool);

    // Get the server configuration
    httpdDebugRequest(r, "modoauth2::checkAuthn::input");
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_oauth2);

    // Get session id from the request
    const maybe<string> sid = openauth::sessionID(r);
    if (hasContent(sid)) {
        // Decline if the session id was not created by this module
        if (substr(content(sid), 0, 7) != "OAuth2_")
            return DECLINED;

        // If we're authenticated store the user info in the request
        const failable<value> info = userInfo(content(sid), sc.mc);
        if (hasContent(info)) {
            r->ap_auth_type = const_cast<char*>(atype);
            return httpd::reportStatus(authenticated(content(info), r));
        }
    }

    // Get the request args
    const list<list<value> > args = httpd::queryArgs(r);

    // Decline if the request is for another authentication provider
    if (!isNil(assoc<value>("openid_identifier", args)))
        return DECLINED;
    if (!isNil(assoc<value>("mod_oauth1_step", args)))
        return DECLINED;

    // Determine the OAuth protocol flow step, conveniently passed
    // around in a request arg
    const list<value> sl = assoc<value>("mod_oauth2_step", args);
    const value step = !isNil(sl) && !isNil(cdr(sl))? cadr(sl) : "";

    // Handle OAuth authorize request step
    if (step == "authorize") {
        r->ap_auth_type = const_cast<char*>(atype);
        return httpd::reportStatus(authorize(args, r, sc));
    }

    // Handle OAuth access_token request step
    if (step == "access_token") {
        r->ap_auth_type = const_cast<char*>(atype);
        return httpd::reportStatus(access_token(args, r, sc));
    }

    // Redirect to the login page
    r->ap_auth_type = const_cast<char*>(atype);
    return httpd::reportStatus(openauth::login(dc.login, r));
}

/**
 * Process the module configuration.
 */
int postConfigMerge(ServerConf& mainsc, server_rec* s) {
    if (s == NULL)
        return OK;
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_oauth2);
    debug(httpd::serverName(s), "modoauth2::postConfigMerge::serverName");

    // Merge configuration from main server
    if (isNil(sc.appkeys))
        sc.appkeys = mainsc.appkeys;
    sc.mc = mainsc.mc;
    sc.cs = mainsc.cs;

    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t* p, unused apr_pool_t* plog, unused apr_pool_t* ptemp, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_oauth2);
    debug(httpd::serverName(s), "modoauth2::postConfig::serverName");

    // Merge server configurations
    return postConfigMerge(sc, s);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* p, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf* psc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_oauth2);
    if(psc == NULL) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_oauth2 loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
    ServerConf& sc = *psc;

    // Connect to Memcached
    if (isNil(sc.mcaddrs))
        sc.mc = *(new (gc_new<memcache::MemCached>()) memcache::MemCached("localhost", 11211));
    else
        sc.mc = *(new (gc_new<memcache::MemCached>()) memcache::MemCached(sc.mcaddrs));

    // Setup a CURL session
    sc.cs = *(new (gc_new<http::CURLSession>()) http::CURLSession(sc.ca, sc.cert, sc.key, ""));

    // Merge the updated configuration into the virtual hosts
    postConfigMerge(sc, s->next);
}

/**
 * Configuration commands.
 */
const char* confAppKey(cmd_parms *cmd, unused void *c, const char *arg1, const char* arg2, const char* arg3) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.appkeys = cons<list<value> >(mklist<value>(arg1, mklist<value>(arg2, arg3)), sc.appkeys);
    return NULL;
}
const char* confMemcached(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.mcaddrs = cons<string>(arg, sc.mcaddrs);
    return NULL;
}
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
const char* confCAFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.ca = arg;
    return NULL;
}
const char* confCertFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.cert = arg;
    return NULL;
}
const char* confCertKeyFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.key = arg;
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE3("AddAuthOAuth2AppKey", (const char*(*)())confAppKey, NULL, RSRC_CONF, "OAuth 2.0 name app-id app-key"),
    AP_INIT_ITERATE("AddAuthOAuthMemcached", (const char*(*)())confMemcached, NULL, RSRC_CONF, "Memcached server host:port"),
    AP_INIT_FLAG("AuthOAuth", (const char*(*)())confEnabled, NULL, OR_AUTHCFG, "OAuth 2.0 authentication On | Off"),
    AP_INIT_TAKE1("AuthOAuthLoginPage", (const char*(*)())confLogin, NULL, OR_AUTHCFG, "OAuth 2.0 login page"),
    AP_INIT_TAKE1("AuthOAuthSSLCACertificateFile", (const char*(*)())confCAFile, NULL, RSRC_CONF, "OAUth 2.0 SSL CA certificate file"),
    AP_INIT_TAKE1("AuthOAuthSSLCertificateFile", (const char*(*)())confCertFile, NULL, RSRC_CONF, "OAuth 2.0 SSL certificate file"),
    AP_INIT_TAKE1("AuthOAuthSSLCertificateKeyFile", (const char*(*)())confCertKeyFile, NULL, RSRC_CONF, "OAuth 2.0 SSL certificate key file"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};

void registerHooks(unused apr_pool_t *p) {
    ap_hook_post_config(postConfig, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(childInit, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_check_authn(checkAuthn, NULL, NULL, APR_HOOK_MIDDLE, AP_AUTH_INTERNAL_PER_CONF);
}

}
}

extern "C" {

module AP_MODULE_DECLARE_DATA mod_tuscany_oauth2 = {
    STANDARD20_MODULE_STUFF,
    // dir config and merger
    tuscany::httpd::makeDirConf<tuscany::oauth2::DirConf>, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::oauth2::ServerConf>, NULL,
    // commands and hooks
    tuscany::oauth2::commands, tuscany::oauth2::registerHooks
};

}
