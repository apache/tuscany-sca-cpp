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
    ServerConf(apr_pool_t* const p, server_rec* const s) : p(p), server(s) {
    }

    const gc_pool p;
    server_rec* const server;
    gc_mutable_ref<string> ca;
    gc_mutable_ref<string> cert;
    gc_mutable_ref<string> key;
    gc_mutable_ref<list<value> > appkeys;
    gc_mutable_ref<list<string> > mcaddrs;
    gc_mutable_ref<memcache::MemCached> mc;
    gc_mutable_ref<perthread_ptr<http::CURLSession> > cs;
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

    const string name;
    const authn_provider* provider;
};

/**
 * Directory configuration.
 */
class DirConf {
public:
    DirConf(apr_pool_t* const p, const char* const d) : p(p), dir(d), enabled(false), login(emptyString) {
    }

    const gc_pool p;
    const char* const dir;
    bool enabled;
    gc_mutable_ref<string> login;
    gc_mutable_ref<list<value> > scopeattrs;
    gc_mutable_ref<list<AuthnProviderConf> > apcs;
};

/**
 * Run the authnz hooks to authenticate a request.
 */
const failable<int> checkAuthnzProviders(const string& user, request_rec* const r, const list<AuthnProviderConf>& apcs) {
    if (isNull(apcs))
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

const failable<int> checkAuthnz(const string& user, request_rec* const r, const list<AuthnProviderConf>& apcs) {
    if (substr(user, 0, 1) == "/")
        return mkfailure<int>(string("Encountered FakeBasicAuth spoof: ") + user, HTTP_UNAUTHORIZED);

    if (isNull(apcs)) {
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
const failable<int> authenticated(const list<value>& userinfo, request_rec* const r, const list<value>& scopeattrs, const list<AuthnProviderConf>& apcs) {
    debug(userinfo, "modoauth2::authenticated::userinfo");

    if (isNull(scopeattrs)) {

        // Store user id in an environment variable
        const list<value> id = assoc<value>("id", userinfo);
        if (isNull(id) || isNull(cdr(id)))
            return mkfailure<int>("Couldn't retrieve user id", HTTP_UNAUTHORIZED);
        apr_table_set(r->subprocess_env, "OAUTH2_ID", apr_pstrdup(r->pool, c_str(cadr(id))));

        // If the request user field has not been mapped to another attribute, map the
        // OAuth id attribute to it
        if (r->user == NULL || r->user[0] == '\0')
            r->user = apr_pstrdup(r->pool, c_str(cadr(id)));

        // Run the authnz hooks to check the authenticated user
        const failable<int> arc = checkAuthnz(r->user == NULL? emptyString : r->user, r, apcs);
        if (!hasContent(arc))
            return arc;

        // Update the request user field with the authorized user id returned by the authnz hooks
        const char* auser = apr_table_get(r->subprocess_env, "AUTHZ_USER");
        if (auser != NULL)
            r->user = apr_pstrdup(r->pool, auser);

        return OK;
    }

    // Store each configured OAuth scope attribute in an environment variable
    const list<value> a = car(scopeattrs);
    const list<value> v = assoc<value>(cadr(a), userinfo);
    if (!isNull(v) && !isNull(cdr(v))) {

        // Map the REMOTE_USER attribute to the request user field
        if (string(car(a)) == "REMOTE_USER")
            r->user = apr_pstrdup(r->pool, c_str(cadr(v)));
        else
            apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, c_str(car(a))), apr_pstrdup(r->pool, c_str(cadr(v))));
    }
    return authenticated(userinfo, r, cdr(scopeattrs), apcs);
}

/**
 * Handle an authorize request.
 */
const failable<int> authorize(const list<value>& args, request_rec* const r, const list<value>& appkeys) {
    // Extract authorize, access_token, client ID and info URIs
    const list<value> ref = assoc<value>("openauth_referrer", args);
    if (isNull(ref) || isNull(cdr(ref)))
        return mkfailure<int>("Missing openauth_referrer parameter");
    const list<value> auth = assoc<value>("oauth2_authorize", args);
    if (isNull(auth) || isNull(cdr(auth)))
        return mkfailure<int>("Missing oauth2_authorize parameter");
    const list<value> tok = assoc<value>("oauth2_access_token", args);
    if (isNull(tok) || isNull(cdr(tok)))
        return mkfailure<int>("Missing oauth2_access_token parameter");
    const list<value> cid = assoc<value>("oauth2_client_id", args);
    if (isNull(cid) || isNull(cdr(cid)))
        return mkfailure<int>("Missing oauth2_client_id parameter");
    const list<value> info = assoc<value>("oauth2_info", args);
    if (isNull(info) || isNull(cdr(info)))
        return mkfailure<int>("Missing oauth2_info parameter");
    const list<value> scope = assoc<value>("oauth2_scope", args);
    if (isNull(scope) || isNull(cdr(scope)))
        return mkfailure<int>("Missing oauth2_scope parameter");
    const list<value> display = assoc<value>("oauth2_display", args);

    // Build the redirect URI
    const string redir = httpd::url("/oauth2/access_token/", r);
    debug(redir, "modoauth2::authorize::redir");

    // Build the state URI
    const list<value> stargs = mklist<value>(tok, cid, info, ref);
    const string state = http::queryString(stargs);
    debug(state, "modoauth2::authorize::state");

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), appkeys);
    if (isNull(app) || isNull(cdr(app)))
        return mkfailure<int>(string("client id not found: ") + (string)cadr(cid));
    list<value> appkey = cadr(app);

    // Redirect to the authorize URI
    const list<value> adisplay = (isNull(display) || isNull(cdr(display)))? nilListValue : mklist<value>("display", cadr(display));
    const list<value> aargs = mklist<value>(mklist<value>("response_type", "code"), mklist<value>("client_id", car(appkey)), mklist<value>("scope", cadr(scope)), adisplay, mklist<value>("redirect_uri", httpd::escape(redir)), mklist<value>("state", httpd::escape(state)));
    const string uri = httpd::unescape(cadr(auth)) + string("?") + http::queryString(aargs);
    debug(uri, "modoauth2::authorize::uri");
    return httpd::externalRedirect(uri, r);
}

/**
 * Extract user info from a profile/info response.
 * TODO Make this configurable.
 */
const failable<list<value> > profileUserInfo(const value& cid, const list<value>& info) {
    return cons<value>(mklist<value>("realm", cid), info);
}

/**
 * Handle an access_token request.
 */
const failable<int> accessToken(const list<value>& args, request_rec* r, const list<value>& appkeys, const http::CURLSession& cs, const list<value>& scopeattrs, const list<AuthnProviderConf>& apcs, const memcache::MemCached& mc) {

    // Extract access_token URI, client ID and authorization code parameters
    const list<value> state = assoc<value>("state", args);
    if (isNull(state) || isNull(cdr(state)))
        return mkfailure<int>("Missing state parameter");
    const list<value>& stargs = httpd::queryArgs(httpd::unescape(cadr(state)));
    const list<value> ref = assoc<value>("openauth_referrer", stargs);
    if (isNull(ref) || isNull(cdr(ref)))
        return mkfailure<int>("Missing openauth_referrer parameter");
    const list<value> tok = assoc<value>("oauth2_access_token", stargs);
    if (isNull(tok) || isNull(cdr(tok)))
        return mkfailure<int>("Missing oauth2_access_token parameter");
    const list<value> cid = assoc<value>("oauth2_client_id", stargs);
    if (isNull(cid) || isNull(cdr(cid)))
        return mkfailure<int>("Missing oauth2_client_id parameter");
    const list<value> info = assoc<value>("oauth2_info", stargs);
    if (isNull(info) || isNull(cdr(info)))
        return mkfailure<int>("Missing oauth2_info parameter");
    const list<value> code = assoc<value>("code", args);
    if (isNull(code) || isNull(cdr(code)))
        return mkfailure<int>("Missing code parameter");

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), appkeys);
    if (isNull(app) || isNull(cdr(app)))
        return mkfailure<int>(string("client id not found: ") + (string)cadr(cid));
    list<value> appkey = cadr(app);

    // Build the redirect URI
    const string redir = httpd::url("/oauth2/access_token/", r);
    debug(redir, "modoauth2::access_token::redir");

    // Request access token
    const list<value> targs = mklist<value>(mklist<value>("client_id", car(appkey)), mklist<value>("redirect_uri", httpd::escape(redir)), mklist<value>("client_secret", cadr(appkey)), code, mklist<value>("grant_type", "authorization_code"));
    const string tqs = http::queryString(targs);
    debug(tqs, "modoauth2::access_token::tokenqs");
    const string turi = httpd::unescape(cadr(tok));
    debug(turi, "modoauth2::access_token::tokenuri");
    const value tval = mklist<value>(string("application/x-www-form-urlencoded;charset=UTF-8"), mklist<value>(tqs));
    const failable<value> ftr = http::post(tval, turi, cs);
    if (!hasContent(ftr))
        return mkfailure<int>(ftr);
    const value tr = content(ftr);
    debug(tr, "modoauth2::access_token::response");
    if (!isList(tr) || isNull(tr))
        return mkfailure<int>("Empty access token");
    const list<value> tv = isString(car<value>(tr)) ?
        assoc<value>("access_token", httpd::queryArgs(join("", convertValues<string>(cadr<value>(tr))))) :
        assoc<value>("access_token", tr);
    if (isNull(tv) || isNull(cdr(tv)))
        return mkfailure<int>("Couldn't retrieve access_token");
    debug(tv, "modoauth2::access_token::token");

    // Request user info
    // TODO Make this step configurable
    const list<value> iargs = mklist<value>(tv);
    const string iuri = httpd::unescape(cadr(info)) + string("?") + http::queryString(iargs);
    debug(iuri, "modoauth2::access_token::infouri");
    const failable<value> profres = http::get(iuri, cs);
    if (!hasContent(profres))
        return mkfailure<int>("Couldn't retrieve user info");
    debug(content(profres), "modoauth2::access_token::info");

    // Retrieve the user info from the profile
    const failable<list<value> > userinfo = profileUserInfo(cadr(cid), content(profres));
    if (!hasContent(userinfo))
        return mkfailure<int>(userinfo);

    // Validate the authenticated user
    const failable<int> authrc = authenticated(content(userinfo), r, scopeattrs, apcs);
    if (!hasContent(authrc))
        return authrc;

    // Store user info in memcached keyed by a session ID
    const value sid = string("OAuth2_") + (string)mkrand();
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
    const gc_scoped_pool sp(r->pool);

    // Decline if we're not enabled or AuthType is not set to Open
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_oauth2);
    if (!dc.enabled)
        return DECLINED;
    const char* atype = ap_auth_type(r);
    if (atype == NULL || strcasecmp(atype, "Open"))
        return DECLINED;
    debug_httpdRequest(r, "modoauth2::checkAuthn::input");
    debug(atype, "modoauth2::checkAuthn::auth_type");

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
            return openauth::reportStatus(mkfailure<int>(reason(userinfo), HTTP_UNAUTHORIZED), dc.login, nilValue, r);
        r->ap_auth_type = const_cast<char*>(atype);
        return openauth::reportStatus(authenticated(content(userinfo), r, dc.scopeattrs, dc.apcs), dc.login, nilValue, r);
    }

    // Get the request args
    const list<value> args = httpd::queryArgs(r);

    // Handle OAuth authorize request step
    if (string(r->uri) == "/oauth2/authorize/") {
        r->ap_auth_type = const_cast<char*>(atype);
        return openauth::reportStatus(authorize(args, r, sc.appkeys), dc.login, 1, r);
    }

    // Handle OAuth access_token request step
    if (string(r->uri) == "/oauth2/access_token/") {
        r->ap_auth_type = const_cast<char*>(atype);
        const failable<int> authrc = accessToken(args, r, sc.appkeys, *(*(perthread_ptr<http::CURLSession>*)sc.cs), dc.scopeattrs, dc.apcs, sc.mc);
        return openauth::reportStatus(authrc, dc.login, 1, r);
    }

    // Redirect to the login page, unless we have a session id or an authorization
    // header from another module
    if (apr_table_get(r->headers_in, (PROXYREQ_PROXY == r->proxyreq) ? "Proxy-Authorization" : "Authorization") != NULL)
        return DECLINED;
    if (hasContent(openauth::sessionID(r, "TuscanyOpenIDAuth")) ||
        hasContent(openauth::sessionID(r, "TuscanyOpenAuth")) ||
        hasContent(openauth::sessionID(r, "TuscanyOAuth1")))
        return DECLINED;
    if ((substr(string(r->uri), 0, 8) == "/oauth1/") || !isNull(assoc<value>("openid_identifier", args)))
        return DECLINED;

    r->ap_auth_type = const_cast<char*>(atype);
    return httpd::reportStatus(openauth::login(dc.login, nilValue, nilValue, r));
}

/**
 * Process the module configuration.
 */
int postConfigMerge(const ServerConf& mainsc, server_rec* const s) {
    if (s == NULL)
        return OK;
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_oauth2);
    debug(httpd::serverName(s), "modoauth2::postConfigMerge::serverName");

    // Merge configuration from main server
    if (isNull((list<value>)sc.appkeys))
        sc.appkeys = mainsc.appkeys;
    if (isNull((list<string>)sc.mcaddrs))
        sc.mcaddrs = mainsc.mcaddrs;
    sc.mc = mainsc.mc;
    sc.cs = mainsc.cs;

    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t* const p, unused apr_pool_t* const plog, unused apr_pool_t* const ptemp, server_rec* const s) {
    const gc_scoped_pool sp(p);

    const ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_oauth2);
    debug(httpd::serverName(s), "modoauth2::postConfig::serverName");

    // Merge server configurations
    return postConfigMerge(sc, s);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* const p, server_rec* const s) {
    const gc_scoped_pool sp(p);

    ServerConf* const psc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_oauth2);
    if(psc == NULL) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_oauth2 loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
    ServerConf& sc = *psc;

    // Connect to Memcached
    if (isNull((list<string>)sc.mcaddrs))
        sc.mc = *(new (gc_new<memcache::MemCached>()) memcache::MemCached("localhost", 11211));
    else
        sc.mc = *(new (gc_new<memcache::MemCached>()) memcache::MemCached(sc.mcaddrs));

    // Setup a CURL session
    const string ca = sc.ca;
    const string cert = sc.cert;
    const string key = sc.key;
    const gc_pool cp = gc_current_pool();
    const lambda<const gc_ptr<http::CURLSession>()> newsession = [ca, cert, key, cp]() -> const gc_ptr<http::CURLSession> {
        const gc_scoped_pool sp(pool(cp));
        return new (gc_new<http::CURLSession>()) http::CURLSession(ca, cert, key, emptyString, 0);
    };
    sc.cs = *(new (gc_new<perthread_ptr<http::CURLSession> >()) perthread_ptr<http::CURLSession>(newsession));

    // Merge the updated configuration into the virtual hosts
    postConfigMerge(sc, s->next);
}

/**
 * Configuration commands.
 */
char* confAppKey(cmd_parms *cmd, unused void *c, char *arg1, char* arg2, char* arg3) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.appkeys = cons<value>(mklist<value>(arg1, mklist<value>(arg2, arg3)), (list<value>)sc.appkeys);
    return NULL;
}
char* confMemcached(cmd_parms *cmd, unused void *c, char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.mcaddrs = cons<string>(arg, (list<string>)sc.mcaddrs);
    return NULL;
}
char* confEnabled(cmd_parms *cmd, void *c, int arg) {
    const gc_scoped_pool sp(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.enabled = (bool)arg;
    return NULL;
}
char* confLogin(cmd_parms *cmd, void *c, char* arg) {
    const gc_scoped_pool sp(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.login = arg;
    return NULL;
}
char* confCAFile(cmd_parms *cmd, unused void *c, char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.ca = arg;
    return NULL;
}
char* confCertFile(cmd_parms *cmd, unused void *c, char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.cert = arg;
    return NULL;
}
char* confCertKeyFile(cmd_parms *cmd, unused void *c, char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth2);
    sc.key = arg;
    return NULL;
}
char* confScopeAttr(cmd_parms *cmd, void* c, char* arg1, char* arg2) {
    const gc_scoped_pool sp(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.scopeattrs = cons<value>(mklist<value>(arg1, arg2), (list<value>)dc.scopeattrs);
    return NULL;
}
char* confAuthnProvider(cmd_parms *cmd, void *c, char* arg) {
    const gc_scoped_pool sp(cmd->pool);
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
