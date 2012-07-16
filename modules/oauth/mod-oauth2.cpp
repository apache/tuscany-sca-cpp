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

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "parallel.hpp"
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
    perthread_ptr<http::CURLSession> cs;
};

/**
 * Authentication provider configuration.
 */
class AuthnProviderConf {
public:
    AuthnProviderConf() : name(), provider(NULL) {
    }
    AuthnProviderConf(const string name, const authn_provider* provider) : name(name), provider(provider) {
    }

    string name;
    const authn_provider* provider;
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
    list<list<value> > scopeattrs;
    list<AuthnProviderConf> apcs;
};

/**
 * Run the authnz hooks to authenticate a request.
 */
const failable<int> checkAuthnzProviders(const string& user, request_rec* r, const list<AuthnProviderConf>& apcs) {
    if (isNil(apcs))
        return mkfailure<int>("Authentication failure for: " + user, HTTP_UNAUTHORIZED);
    const AuthnProviderConf apc = car<AuthnProviderConf>(apcs);
    if (apc.provider == NULL || !apc.provider->check_password)
        return checkAuthnzProviders(user, r, cdr(apcs));

    apr_table_setn(r->notes, AUTHN_PROVIDER_NAME_NOTE, c_str(apc.name));
    const authn_status auth_result = apc.provider->check_password(r, c_str(string("/oauth2/") + user), "password");
    apr_table_unset(r->notes, AUTHN_PROVIDER_NAME_NOTE);
    if (auth_result != AUTH_GRANTED)
        return checkAuthnzProviders(user, r, cdr(apcs));
    return OK;
}

const failable<int> checkAuthnz(const string& user, request_rec* r, const list<AuthnProviderConf>& apcs) {
    if (substr(user, 0, 1) == "/")
        return mkfailure<int>(string("Encountered FakeBasicAuth spoof: ") + user, HTTP_UNAUTHORIZED);

    if (isNil(apcs)) {
        const authn_provider* provider = (const authn_provider*)ap_lookup_provider(AUTHN_PROVIDER_GROUP, AUTHN_DEFAULT_PROVIDER, AUTHN_PROVIDER_VERSION);
        return checkAuthnzProviders(user, r, mklist<AuthnProviderConf>(AuthnProviderConf(AUTHN_DEFAULT_PROVIDER, provider)));
    }
    return checkAuthnzProviders(user, r, apcs);
}

/**
 * Return the user info for a session.
 */
const failable<value> userInfo(const value& sid, const memcache::MemCached& mc) {
    return memcache::get(mklist<value>("tuscanyOAuth2", sid), mc);
}

/**
 * Handle an authenticated request.
 */
const failable<int> authenticated(const list<list<value> >& userinfo, const bool check, request_rec* r, const list<list<value> >& scopeattrs, const list<AuthnProviderConf>& apcs) {
    debug(userinfo, "modoauth2::authenticated::userinfo");

    if (isNil(scopeattrs)) {

        // Store user id in an environment variable
        const list<value> id = assoc<value>("id", userinfo);
        if (isNil(id) || isNil(cdr(id)))
            return mkfailure<int>("Couldn't retrieve user id");
        apr_table_set(r->subprocess_env, "OAUTH2_ID", apr_pstrdup(r->pool, c_str(cadr(id))));

        // If the request user field has not been mapped to another attribute, map the
        // OAuth id attribute to it
        if (r->user == NULL || r->user[0] == '\0')
            r->user = apr_pstrdup(r->pool, c_str(cadr(id)));

        // Run the authnz hooks to check the authenticated user
        if (check)
            return checkAuthnz(r->user == NULL? "" : r->user, r, apcs);
        return OK;
    }

    // Store each configured OAuth scope attribute in an environment variable
    const list<value> a = car(scopeattrs);
    const list<value> v = assoc<value>(cadr(a), userinfo);
    if (!isNil(v) && !isNil(cdr(v))) {

        // Map the REMOTE_USER attribute to the request user field
        if (string(car(a)) == "REMOTE_USER")
            r->user = apr_pstrdup(r->pool, c_str(cadr(v)));
        else
            apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, c_str(car(a))), apr_pstrdup(r->pool, c_str(cadr(v))));
    }
    return authenticated(userinfo, check, r, cdr(scopeattrs), apcs);
}

/**
 * Handle an authorize request.
 */
const failable<int> authorize(const list<list<value> >& args, request_rec* r, const list<list<value> >& appkeys) {
    // Extract authorize, access_token, client ID and info URIs
    const list<value> ref = assoc<value>("openauth_referrer", args);
    if (isNil(ref) || isNil(cdr(ref)))
        return mkfailure<int>("Missing openauth_referrer parameter");
    const list<value> auth = assoc<value>("oauth2_authorize", args);
    if (isNil(auth) || isNil(cdr(auth)))
        return mkfailure<int>("Missing oauth2_authorize parameter");
    const list<value> tok = assoc<value>("oauth2_access_token", args);
    if (isNil(tok) || isNil(cdr(tok)))
        return mkfailure<int>("Missing oauth2_access_token parameter");
    const list<value> cid = assoc<value>("oauth2_client_id", args);
    if (isNil(cid) || isNil(cdr(cid)))
        return mkfailure<int>("Missing oauth2_client_id parameter");
    const list<value> info = assoc<value>("oauth2_info", args);
    if (isNil(info) || isNil(cdr(info)))
        return mkfailure<int>("Missing oauth2_info parameter");
    const list<value> scope = assoc<value>("oauth2_scope", args);
    if (isNil(scope) || isNil(cdr(scope)))
        return mkfailure<int>("Missing oauth2_scope parameter");
    const list<value> display = assoc<value>("oauth2_display", args);

    // Build the redirect URI
    const string redir = httpd::url("/oauth2/access_token/", r);
    debug(redir, "modoauth2::authorize::redir");

    // Build the state URI
    const list<list<value> > stargs = mklist<list<value> >(tok, cid, info, ref);
    const string state = http::queryString(stargs);
    debug(state, "modoauth2::authorize::state");

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), appkeys);
    if (isNil(app) || isNil(cdr(app)))
        return mkfailure<int>(string("client id not found: ") + cadr(cid));
    list<value> appkey = cadr(app);

    // Redirect to the authorize URI
    const list<value> adisplay = (isNil(display) || isNil(cdr(display)))? list<value>() : mklist<value>("display", cadr(display));
    const list<list<value> > aargs = mklist<list<value> >(mklist<value>("response_type", "code"), mklist<value>("client_id", car(appkey)), mklist<value>("scope", cadr(scope)), adisplay, mklist<value>("redirect_uri", httpd::escape(redir)), mklist<value>("state", httpd::escape(state)));
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
const failable<int> accessToken(const list<list<value> >& args, request_rec* r, const list<list<value> >& appkeys, const perthread_ptr<http::CURLSession>& cs, const list<list<value> >& scopeattrs, const list<AuthnProviderConf>& apcs, const memcache::MemCached& mc) {

    // Extract access_token URI, client ID and authorization code parameters
    const list<value> state = assoc<value>("state", args);
    if (isNil(state) || isNil(cdr(state)))
        return mkfailure<int>("Missing state parameter");
    const list<list<value> >& stargs = httpd::queryArgs(httpd::unescape(cadr(state)));
    const list<value> ref = assoc<value>("openauth_referrer", stargs);
    if (isNil(ref) || isNil(cdr(ref)))
        return mkfailure<int>("Missing openauth_referrer parameter");
    const list<value> tok = assoc<value>("oauth2_access_token", stargs);
    if (isNil(tok) || isNil(cdr(tok)))
        return mkfailure<int>("Missing oauth2_access_token parameter");
    const list<value> cid = assoc<value>("oauth2_client_id", stargs);
    if (isNil(cid) || isNil(cdr(cid)))
        return mkfailure<int>("Missing oauth2_client_id parameter");
    const list<value> info = assoc<value>("oauth2_info", stargs);
    if (isNil(info) || isNil(cdr(info)))
        return mkfailure<int>("Missing oauth2_info parameter");
    const list<value> code = assoc<value>("code", args);
    if (isNil(code) || isNil(cdr(code)))
        return mkfailure<int>("Missing code parameter");

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), appkeys);
    if (isNil(app) || isNil(cdr(app)))
        return mkfailure<int>(string("client id not found: ") + cadr(cid));
    list<value> appkey = cadr(app);

    // Build the redirect URI
    const string redir = httpd::url("/oauth2/access_token/", r);
    debug(redir, "modoauth2::access_token::redir");

    // Request access token
    const list<list<value> > targs = mklist<list<value> >(mklist<value>("client_id", car(appkey)), mklist<value>("redirect_uri", httpd::escape(redir)), mklist<value>("client_secret", cadr(appkey)), code, mklist<value>("grant_type", "authorization_code"));
    const string tqs = http::queryString(targs);
    debug(tqs, "modoauth2::access_token::tokenqs");
    const string turi = httpd::unescape(cadr(tok));
    debug(turi, "modoauth2::access_token::tokenuri");
    const value tval = mklist<value>(string("application/x-www-form-urlencoded;charset=UTF-8"), mklist<value>(tqs));
    const failable<value> ftr = http::post(tval, turi, *(cs));
    if (!hasContent(ftr))
        return mkfailure<int>(ftr);
    const value tr = content(ftr);
    debug(tr, "modoauth2::access_token::response");
    if (!isList(tr) || isNil(tr))
        return mkfailure<int>("Empty access token");
    const list<value> tv = isString(car<value>(tr)) ?
        assoc<value>("access_token", httpd::queryArgs(join("", convertValues<string>(cadr<value>(tr))))) :
        assoc<value>("access_token", tr);
    if (isNil(tv) || isNil(cdr(tv)))
        return mkfailure<int>("Couldn't retrieve access_token");
    debug(tv, "modoauth2::access_token::token");

    // Request user info
    // TODO Make this step configurable
    const list<list<value> > iargs = mklist<list<value> >(tv);
    const string iuri = httpd::unescape(cadr(info)) + string("?") + http::queryString(iargs);
    debug(iuri, "modoauth2::access_token::infouri");
    const failable<value> profres = http::get(iuri, *(cs));
    if (!hasContent(profres))
        return mkfailure<int>("Couldn't retrieve user info");
    debug(content(profres), "modoauth2::access_token::info");

    // Retrieve the user info from the profile
    const failable<list<value> > userinfo = profileUserInfo(cadr(cid), content(profres));
    if (!hasContent(userinfo))
        return mkfailure<int>(userinfo);

    // Validate the authenticated user
    const failable<int> authrc = authenticated(content(userinfo), true, r, scopeattrs, apcs);
    if (!hasContent(authrc))
        return authrc;

    // Store user info in memcached keyed by a session ID
    const value sid = string("OAuth2_") + mkrand();
    const failable<bool> prc = memcache::put(mklist<value>("tuscanyOAuth2", sid), content(userinfo), mc);
    if (!hasContent(prc))
        return mkfailure<int>(prc);

    // Send the session ID to the client in a cookie
    debug(c_str(openauth::cookie("TuscanyOAuth2", sid, httpd::hostName(r))), "modoauth2::access_token::setcookie");
    apr_table_set(r->err_headers_out, "Set-Cookie", c_str(openauth::cookie("TuscanyOAuth2", sid, httpd::hostName(r))));
    return httpd::externalRedirect(httpd::url(httpd::unescape(cadr(ref)), r), r);
}

/**
 * Check user authentication.
 */
static int checkAuthn(request_rec *r) {
    gc_scoped_pool pool(r->pool);

    // Decline if we're not enabled or AuthType is not set to Open
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_oauth2);
    if (!dc.enabled)
        return DECLINED;
    const char* atype = ap_auth_type(r);
    if (atype == NULL || strcasecmp(atype, "Open"))
        return DECLINED;
    debug_httpdRequest(r, "modoauth2::checkAuthn::input");
    debug(atype, "modopenauth::checkAuthn::auth_type");

    // Get the server configuration
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_oauth2);

    // Get session id from the request
    const maybe<string> sid = openauth::sessionID(r, "TuscanyOAuth2");
    if (hasContent(sid)) {
        // Decline if the session id was not created by this module
        if (substr(content(sid), 0, 7) != "OAuth2_")
            return DECLINED;

        // Extract the user info from the auth session
        const failable<value> userinfo = userInfo(content(sid), sc.mc);
        if (!hasContent(userinfo))
            return httpd::reportStatus(mkfailure<int>(userinfo));
        r->ap_auth_type = const_cast<char*>(atype);
        return httpd::reportStatus(authenticated(content(userinfo), false, r, dc.scopeattrs, dc.apcs));
    }

    // Get the request args
    const list<list<value> > args = httpd::queryArgs(r);

    // Handle OAuth authorize request step
    if (string(r->uri) == "/oauth2/authorize/") {
        r->ap_auth_type = const_cast<char*>(atype);
        return httpd::reportStatus(authorize(args, r, sc.appkeys));
    }

    // Handle OAuth access_token request step
    if (string(r->uri) == "/oauth2/access_token/") {
        r->ap_auth_type = const_cast<char*>(atype);
        const failable<int> authrc = accessToken(args, r, sc.appkeys, sc.cs, dc.scopeattrs, dc.apcs, sc.mc);

        // Redirect to the login page if user is not authorized
        if (!hasContent(authrc) && rcode(authrc) == HTTP_UNAUTHORIZED)
            return httpd::reportStatus(openauth::login(dc.login, string("/"), 1, r));

        return httpd::reportStatus(authrc);
    }

    // Redirect to the login page, unless we have a session id or an authorization
    // header from another module
    if (apr_table_get(r->headers_in, (PROXYREQ_PROXY == r->proxyreq) ? "Proxy-Authorization" : "Authorization") != NULL)
        return DECLINED;
    if (hasContent(openauth::sessionID(r, "TuscanyOpenIDAuth")) ||
        hasContent(openauth::sessionID(r, "TuscanyOpenAuth")) ||
        hasContent(openauth::sessionID(r, "TuscanyOAuth1")))
        return DECLINED;
    if ((substr(string(r->uri), 0, 8) == "/oauth1/") || !isNil(assoc<value>("openid_identifier", args)))
        return DECLINED;

    r->ap_auth_type = const_cast<char*>(atype);
    return httpd::reportStatus(openauth::login(dc.login, value(), value(), r));
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
    if (isNil(sc.mcaddrs))
        sc.mcaddrs = mainsc.mcaddrs;
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
 * Lambda function that creates a new CURL session.
 */
class newsession {
public:
    newsession(const string& ca, const string& cert, const string& key) : ca(ca), cert(cert), key(key) {
    }

    const gc_ptr<http::CURLSession> operator()() const {
        return new (gc_new<http::CURLSession>()) http::CURLSession(ca, cert, key, "", 0);
    }

private:
    const string ca;
    const string cert;
    const string key;
};

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
    sc.cs = perthread_ptr<http::CURLSession>(lambda<gc_ptr<http::CURLSession>()>(newsession(sc.ca, sc.cert, sc.key)));

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
const char* confScopeAttr(cmd_parms *cmd, void* c, const char* arg1, const char* arg2) {
    gc_scoped_pool pool(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.scopeattrs = cons<list<value> >(mklist<value>(arg1, arg2), dc.scopeattrs);
    return NULL;
}
const char* confAuthnProvider(cmd_parms *cmd, void *c, const char* arg) {
    gc_scoped_pool pool(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);

    // Lookup and cache the Authn provider
    const authn_provider* provider = (authn_provider*)ap_lookup_provider(AUTHN_PROVIDER_GROUP, arg, AUTHN_PROVIDER_VERSION);
    if (provider == NULL)
        return apr_psprintf(cmd->pool, "Unknown Authn provider: %s", arg);
    if (!provider->check_password)
        return apr_psprintf(cmd->pool, "The '%s' Authn provider doesn't support password authentication", arg);
    dc.apcs = append<AuthnProviderConf>(dc.apcs, mklist<AuthnProviderConf>(AuthnProviderConf(arg, provider)));
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_ITERATE("AuthOAuthProvider", (const char*(*)())confAuthnProvider, NULL, OR_AUTHCFG, "Auth providers for a directory or location"),
    AP_INIT_TAKE3("AddAuthOAuth2AppKey", (const char*(*)())confAppKey, NULL, RSRC_CONF, "OAuth 2.0 name app-id app-key"),
    AP_INIT_ITERATE("AddAuthOAuthMemcached", (const char*(*)())confMemcached, NULL, RSRC_CONF, "Memcached server host:port"),
    AP_INIT_FLAG("AuthOAuth", (const char*(*)())confEnabled, NULL, OR_AUTHCFG, "OAuth 2.0 authentication On | Off"),
    AP_INIT_TAKE1("AuthOAuthLoginPage", (const char*(*)())confLogin, NULL, OR_AUTHCFG, "OAuth 2.0 login page"),
    AP_INIT_TAKE1("AuthOAuthSSLCACertificateFile", (const char*(*)())confCAFile, NULL, RSRC_CONF, "OAUth 2.0 SSL CA certificate file"),
    AP_INIT_TAKE1("AuthOAuthSSLCertificateFile", (const char*(*)())confCertFile, NULL, RSRC_CONF, "OAuth 2.0 SSL certificate file"),
    AP_INIT_TAKE1("AuthOAuthSSLCertificateKeyFile", (const char*(*)())confCertKeyFile, NULL, RSRC_CONF, "OAuth 2.0 SSL certificate key file"),
    AP_INIT_TAKE2("AddAuthOAuth2ScopeAttr", (const char*(*)())confScopeAttr, NULL, OR_AUTHCFG, "OAuth 2.0 scope attribute"),
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
