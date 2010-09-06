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
#include "../../components/cache/memcache.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_oauth;
}

namespace tuscany {
namespace oauth {

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
    list<list<value> > apps;
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
 * Check user authentication.
 */
static int checkUserID(request_rec *r) {
    // Decline if we're not enabled or AuthType is not set to Open
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_oauth);
    if (!dc.enabled)
        return DECLINED;
    const char* atype = ap_auth_type(r);
    if (atype == NULL || strcasecmp(atype, "Open"))
        return DECLINED;

    gc_scoped_pool pool(r->pool);
    httpdDebugRequest(r, "modoauth::checkUserID::input");
    return OK;
}

/**
 * Return the session id from a request.
 */
const maybe<string> sessionID(const list<string> c) {
    if (isNil(c))
        return maybe<string>();
    const list<string> kv = tokenize("=", car(c));
    if (!isNil(kv) && !isNil(cdr(kv))) {
        if (car(kv) == "TuscanyOpenAuth")
            return cadr(kv);
    }
    return sessionID(cdr(c));
}

const maybe<string> sessionID(const request_rec* r) {
    const char* c = apr_table_get(r->headers_in, "Cookie");
    debug(c, "modoauth::sessionid::cookies");
    if (c == NULL)
        return maybe<string>();
    return sessionID(tokenize(";", c));
}

/**
 * Return the user info for a session.
 */
const failable<value> userInfo(const value& sid, const ServerConf& sc) {
    return memcache::get(mklist<value>("tuscanyOpenAuth", sid), sc.mc);
}

/**
 * Handle an authenticated request.
 */
const failable<int> authenticated(const list<list<value> >& info, request_rec* r) {
    debug(info, "modoauth::authenticated::info");

    const list<value> id = assoc<value>("id", info);
    r->user = apr_pstrdup(r->pool, c_str(cadr(id)));

    const list<value> email = assoc<value>("email", info);
    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "EMAIL"), apr_pstrdup(r->pool, c_str(cadr(email))));

    const list<value> fullname = assoc<value>("name", info);
    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "NICKNAME"), apr_pstrdup(r->pool, c_str(cadr(fullname))));
    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "FULLNAME"), apr_pstrdup(r->pool, c_str(cadr(fullname))));

    const list<value> firstname = assoc<value>("first_name", info);
    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "FIRSTNAME"), apr_pstrdup(r->pool, c_str(cadr(firstname))));

    const list<value> lastname = assoc<value>("last_name", info);
    apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, "LASTNAME"), apr_pstrdup(r->pool, c_str(cadr(lastname))));

    if(r->ap_auth_type == NULL)
        r->ap_auth_type = const_cast<char*>("OAuth");
    return DECLINED;
}

/**
 * Redirect to the configured login page.
 */
const failable<int> login(const string& page, request_rec* r) {
    const list<list<value> > largs = mklist<list<value> >(mklist<value>("openauth_referrer", httpd::escape(httpd::url(r->uri, r))));
    const string loc = httpd::url(page, r) + string("?") + httpd::queryString(largs);
    debug(loc, "modoauth::login::uri");
    return httpd::externalRedirect(loc, r);
}

/**
 * Handle an authorize request.
 */
const failable<int> authorize(const list<list<value> >& args, request_rec* r) {
    // Extract authorize URI, access_token URI and client ID
    const list<value> auth = assoc<value>("mod_oauth_authorize", args);
    const list<value> tok = assoc<value>("mod_oauth_access_token", args);
    const list<value> cid = assoc<value>("mod_oauth_client_id", args);
    const list<value> info = assoc<value>("mod_oauth_info", args);

    // Build the redirect URI
    const list<list<value> > rargs = mklist<list<value> >(mklist<value>("mod_oauth_step", "access_token"), tok, cid, info);
    const string redir = httpd::url(r->uri, r) + string("?") + httpd::queryString(rargs);
    debug(redir, "modoauth::authorize::redir");

    // Redirect to the authorize URI
    const list<list<value> > aargs = mklist<list<value> >(mklist<value>("client_id", cadr(cid)), mklist<value>("scope", "email"), mklist<value>("redirect_uri", httpd::escape(redir)));
    const string uri = httpd::unescape(cadr(auth)) + string("?") + httpd::queryString(aargs);
    debug(uri, "modoauth::authorize::uri");
    return httpd::externalRedirect(uri, r);
}

/**
 * Convert a session id to a cookie string.
 */
const string cookie(const string& sid) {
    const time_t t = time(NULL) + 86400;
    char exp[32];
    strftime(exp, 32, "%a, %d-%b-%Y %H:%M:%S GMT", gmtime(&t));
    const string c = string("TuscanyOpenAuth=") + sid + string(";path=/;expires=" + string(exp)) + ";secure=TRUE";
    debug(c, "modoauth::cookie");
    return c;
}

/**
 * Handle an access_token request.
 */
const failable<int> access_token(const list<list<value> >& args, request_rec* r, const ServerConf& sc) {
    // Extract access_token URI, client ID and authorization code
    const list<value> tok = assoc<value>("mod_oauth_access_token", args);
    const list<value> cid = assoc<value>("mod_oauth_client_id", args);
    const list<value> info = assoc<value>("mod_oauth_info", args);
    const list<value> code = assoc<value>("code", args);

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), sc.apps);

    // Build the redirect URI
    const list<list<value> > rargs = mklist<list<value> >(mklist<value>("mod_oauth_step", "access_token"), tok, cid, info);
    const string redir = httpd::url(r->uri, r) + string("?") + httpd::queryString(rargs);
    debug(redir, "modoauth::access_token::redir");

    // Request access token
    const list<list<value> > targs = mklist<list<value> >(mklist<value>("client_id", cadr(cid)), mklist<value>("redirect_uri", httpd::escape(redir)), mklist<value>("client_secret", cadr(app)), code);
    const string turi = httpd::unescape(cadr(tok)) + string("?") + httpd::queryString(targs);
    debug(turi, "modoauth::access_token::tokenuri");
    const failable<value> tr = http::get(turi, sc.cs);
    debug(tr, "modoauth::access_token::response");
    const list<value> tv = assoc<value>("access_token", httpd::queryArgs(join("", convertValues<string>(content(tr)))));
    debug(tv, "modoauth::access_token::token");

    // Request user info
    // TODO Make this step configurable
    const list<list<value> > iargs = mklist<list<value> >(tv);
    const string iuri = httpd::unescape(cadr(info)) + string("?") + httpd::queryString(iargs);
    debug(iuri, "modoauth::access_token::infouri");
    const failable<value> iv = http::get(iuri, sc.cs);
    debug(iv, "modoauth::access_token::info");

    // Store user info in memcached keyed by session ID
    const value sid = string("OAuth_") + mkrand();
    memcache::put(mklist<value>("tuscanyOpenAuth", sid), content(iv), sc.mc);

    // Send session ID to the client in a cookie
    apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie(sid)));
    return httpd::externalRedirect(httpd::url(r->uri, r), r);
}

/**
 * Handle a request.
 */
int handler(request_rec* r) {
    // Decline if we're not enabled or if the user is already 
    // authenticated by another module
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_oauth);
    if(!dc.enabled)
        return DECLINED;
    if (r->user != NULL || apr_table_get(r->subprocess_env, "SSL_REMOTE_USER") != NULL)
        return DECLINED;

    gc_scoped_pool pool(r->pool);
    httpdDebugRequest(r, "modoauth::handler::input");
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_oauth);

    // Get session id from the request
    const maybe<string> sid = sessionID(r);
    if (hasContent(sid)) {
        // Decline if the session id was not created by this module
        if (substr(content(sid), 0, 6) != "OAuth_")
            return DECLINED;

        // If we're authenticated store the user info in the request
        const failable<value> info = userInfo(content(sid), sc);
        if (hasContent(info))
            return httpd::reportStatus(authenticated(content(info), r));
    }

    // Get the request args
    const list<list<value> > args = httpd::queryArgs(r);

    // Decline if the request is for OpenID authentication
    if (!isNil(assoc<value>("openid_identifier", args)))
        return DECLINED;

    // Determine the OAuth protocol flow step, conveniently passed
    // around in a request arg
    const list<value> sl = assoc<value>("mod_oauth_step", args);
    const value step = !isNil(sl) && !isNil(cdr(sl))? cadr(sl) : "";

    // Handle OAuth authorize request step
    if (step == "authorize")
        return httpd::reportStatus(authorize(args, r));

    // Handle OAuth access_token request step
    if (step == "access_token")
        return httpd::reportStatus(access_token(args, r, sc));

    // Redirect to the login page
    return httpd::reportStatus(login(dc.login, r));
}

/**
 * Process the module configuration.
 */
int postConfigMerge(ServerConf& mainsc, server_rec* s) {
    if (s == NULL)
        return OK;
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_oauth);
    debug(httpd::serverName(s), "modoauth::postConfigMerge::serverName");

    // Merge configuration from main server
    if (isNil(sc.apps))
        sc.apps = mainsc.apps;
    sc.mc = mainsc.mc;
    sc.cs = mainsc.cs;

    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t* p, unused apr_pool_t* plog, unused apr_pool_t* ptemp, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_oauth);
    debug(httpd::serverName(s), "modoauth::postConfig::serverName");

    // Merge server configurations
    return postConfigMerge(sc, s);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* p, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf* psc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_oauth);
    if(psc == NULL) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_oauth loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
    ServerConf& sc = *psc;

    // Connect to Memcached
    if (isNil(sc.mcaddrs))
        sc.mc = *(new (gc_new<memcache::MemCached>()) memcache::MemCached("localhost", 11211));
    else
        sc.mc = *(new (gc_new<memcache::MemCached>()) memcache::MemCached(sc.mcaddrs));

    // Setup a CURL session
    sc.cs = *(new (gc_new<http::CURLSession>()) http::CURLSession(sc.ca, sc.cert, sc.key));

    // Merge the updated configuration into the virtual hosts
    postConfigMerge(sc, s->next);
}

/**
 * Configuration commands.
 */
const char* confApp(cmd_parms *cmd, unused void *c, const char *arg1, const char* arg2) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth);
    sc.apps = cons<list<value> >(mklist<value>(arg1, arg2), sc.apps);
    return NULL;
}
const char* confMemcached(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth);
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
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth);
    sc.ca = arg;
    return NULL;
}
const char* confCertFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth);
    sc.cert = arg;
    return NULL;
}
const char* confCertKeyFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth);
    sc.key = arg;
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_ITERATE2("AddAuthOAuthApp", (const char*(*)())confApp, NULL, RSRC_CONF, "OAuth app-id app-secret"),
    AP_INIT_ITERATE("AddAuthOAuthMemcached", (const char*(*)())confMemcached, NULL, RSRC_CONF, "Memcached server host:port"),
    AP_INIT_FLAG("AuthOAuth", (const char*(*)())confEnabled, NULL, OR_AUTHCFG, "OAuth authentication On | Off"),
    AP_INIT_TAKE1("AuthOAuthLoginPage", (const char*(*)())confLogin, NULL, OR_AUTHCFG, "OAuth login page"),
    AP_INIT_TAKE1("AuthOAuthSSLCACertificateFile", (const char*(*)())confCAFile, NULL, RSRC_CONF, "OAUth SSL CA certificate file"),
    AP_INIT_TAKE1("AuthOAuthSSLCertificateFile", (const char*(*)())confCertFile, NULL, RSRC_CONF, "OAuth SSL certificate file"),
    AP_INIT_TAKE1("AuthOAuthSSLCertificateKeyFile", (const char*(*)())confCertKeyFile, NULL, RSRC_CONF, "OAuth SSL certificate key file"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};

void registerHooks(unused apr_pool_t *p) {
    ap_hook_post_config(postConfig, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(childInit, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_check_user_id(checkUserID, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(handler, NULL, NULL, APR_HOOK_FIRST);
}

}
}

extern "C" {

module AP_MODULE_DECLARE_DATA mod_tuscany_oauth = {
    STANDARD20_MODULE_STUFF,
    // dir config and merger
    tuscany::httpd::makeDirConf<tuscany::oauth::DirConf>, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::oauth::ServerConf>, NULL,
    // commands and hooks
    tuscany::oauth::commands, tuscany::oauth::registerHooks
};

}
