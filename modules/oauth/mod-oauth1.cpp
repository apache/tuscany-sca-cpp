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
 * HTTPD module for OAuth 1.0 authentication.
 */

#include <stdlib.h>
#include <sys/stat.h>

extern "C" {
#include <oauth.h>
}

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "../json/json.hpp"
#include "../http/httpd.hpp"
#include "../http/http.hpp"
#include "../http/openauth.hpp"
#include "../../components/cache/memcache.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_oauth1;
}

namespace tuscany {
namespace oauth1 {

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
};

/**
 * Return the user info for a session.
 */
const failable<value> userInfo(const value& sid, const memcache::MemCached& mc) {
    return memcache::get(mklist<value>("tuscanyOAuth1", sid), mc);
}

/**
 * Handle an authenticated request.
 */
const failable<int> authenticated(const list<list<value> >& attrs, const list<list<value> >& info, request_rec* r) {
    debug(info, "modoauth1::authenticated::info");

    if (isNil(attrs)) {

        // Store user id in an environment variable
        const list<value> id = assoc<value>("id", info);
        if (isNil(id) || isNil(cdr(id)))
            return mkfailure<int>("Couldn't retrieve user id");
        apr_table_set(r->subprocess_env, "OAUTH1_ID", apr_pstrdup(r->pool, c_str(cadr(id))));

        // If the request user field has not been mapped to another attribute, map the
        // OAuth id attribute to it
        if (r->user == NULL || r->user[0] == '\0')
            r->user = apr_pstrdup(r->pool, c_str(cadr(id)));
        return OK;
    }

    // Store each configure OAuth scope attribute in an environment variable
    const list<value> a = car(attrs);
    const list<value> v = assoc<value>(cadr(a), info);
    if (!isNil(v) && !isNil(cdr(v))) {

        // Map the REMOTE_USER attribute to the request user field
        if (string(car(a)) == "REMOTE_USER")
            r->user = apr_pstrdup(r->pool, c_str(cadr(v)));
        else
            apr_table_set(r->subprocess_env, apr_pstrdup(r->pool, c_str(car(a))), apr_pstrdup(r->pool, c_str(cadr(v))));
    }
    return authenticated(cdr(attrs), info, r);
}

/**
 * Convert a query string containing oauth args to an authorization header.
 */
const string header(const string& qs, const string& redir, const string& verif) {
    const list<list<value> > args = httpd::queryArgs(qs);
    ostringstream hdr;
    hdr << "Authorization: OAuth "
        << "oauth_nonce=\"" << string(cadr(assoc<value>("oauth_nonce", args))) << "\", ";

    if (length(redir) != 0)
        hdr << "oauth_callback=\"" << httpd::escape(redir) << "\", ";

    hdr << "oauth_signature_method=\"" << string(cadr(assoc<value>("oauth_signature_method", args))) << "\", "
        << "oauth_timestamp=\"" << string(cadr(assoc<value>("oauth_timestamp", args))) << "\", "
        << "oauth_consumer_key=\"" << string(cadr(assoc<value>("oauth_consumer_key", args))) << "\", ";

    const list<value> atok = assoc<value>("oauth_token", args);
    if (!isNil(atok) && !isNil(cdr(atok)))
        hdr << "oauth_token=\"" << string(cadr(atok)) << "\", ";

    if (length(verif) != 0)
        hdr << "oauth_verifier=\"" << verif << "\", ";

    hdr << "oauth_signature=\"" << string(cadr(assoc<value>("oauth_signature", args))) << "\", "
        << "oauth_version=\"" << string(cadr(assoc<value>("oauth_version", args))) << "\"";
    debug(str(hdr), "modoauth1::authheader");
    return str(hdr);
}


/**
 * Sign a request.
 */
const list<string> sign(const string& verb, const string& uri, const list<value> appkey, const string& tok, const string& sec) {
    char* qs = NULL;
    char* suri = oauth_sign_url2(c_str(uri), &qs, OA_HMAC, c_str(verb), c_str(car(appkey)), c_str(cadr(appkey)), length(tok) != 0? c_str(tok) : NULL, length(sec) != 0? c_str(sec) : NULL);
    const list<string> res = mklist<string>(suri, qs);
    free(suri);
    free(qs);
    return res;
}

/**
 * Handle an authorize request.
 */
const failable<int> authorize(const list<list<value> >& args, request_rec* r, const list<list<value> >& appkeys, const memcache::MemCached& mc) {
    // Extract authorize, access_token, client ID and info URIs
    const list<value> ref = assoc<value>("openauth_referrer", args);
    if (isNil(ref) || isNil(cdr(ref)))
        return mkfailure<int>("Missing openauth_referrer parameter");
    const list<value> req = assoc<value>("oauth1_request_token", args);
    if (isNil(req) || isNil(cdr(req)))
        return mkfailure<int>("Missing oauth1_request_token parameter");
    const list<value> auth = assoc<value>("oauth1_authorize", args);
    if (isNil(auth) || isNil(cdr(auth)))
        return mkfailure<int>("Missing oauth1_authorize parameter");
    const list<value> tok = assoc<value>("oauth1_access_token", args);
    if (isNil(tok) || isNil(cdr(tok)))
        return mkfailure<int>("Missing oauth1_access_token parameter");
    const list<value> cid = assoc<value>("oauth1_client_id", args);
    if (isNil(cid) || isNil(cdr(cid)))
        return mkfailure<int>("Missing oauth1_client_id parameter");
    const list<value> info = assoc<value>("oauth1_info", args);
    if (isNil(info) || isNil(cdr(info)))
        return mkfailure<int>("Missing oauth1_info parameter");

    // Build the redirect URI
    const list<list<value> > redirargs = mklist<list<value> >(tok, cid, info, ref);
    const string redir = httpd::url("/oauth1/access_token/", r) + string("?") + http::queryString(redirargs);
    debug(redir, "modoauth1::authorize::redir");

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), appkeys);
    if (isNil(app) || isNil(cdr(app)))
        return mkfailure<int>(string("client id not found: ") + cadr(cid));
    list<value> appkey = cadr(app);

    // Build and sign the request token URI
    const string requri = httpd::unescape(cadr(req)) + string("&") + http::queryString(mklist<list<value> >(mklist<value>("oauth_callback", httpd::escape(redir))));
    const list<string> srequri = sign("POST", requri, appkey, "", "");
    debug(srequri, "modoauth1::authorize::srequri");

    // Put the args into an oauth header
    const string reqhdr = header(cadr(srequri), redir, "");

    // Send the request token request
    char* pres = oauth_http_post2(c_str(car(srequri)), "", c_str(reqhdr));
    if (pres == NULL)
        return mkfailure<int>("Couldn't send request token request");
    const string res(pres);
    free(pres);
    debug(res, "modoauth1::authorize::res");
    const list<list<value> > resargs = httpd::queryArgs(res);

    // Retrieve the request token
    const list<value> conf = assoc<value>("oauth_callback_confirmed", resargs);
    if (isNil(conf) || isNil(cdr(conf)) || cadr(conf) != "true")
        return mkfailure<int>("Couldn't confirm oauth_callback");
    const list<value> tv = assoc<value>("oauth_token", resargs);
    if (isNil(tv) || isNil(cdr(tv)))
        return mkfailure<int>("Couldn't retrieve oauth_token");
    const list<value> sv = assoc<value>("oauth_token_secret", resargs);
    if (isNil(sv) || isNil(cdr(sv)))
        return mkfailure<int>("Couldn't retrieve oauth_token_secret");

    // Store the request token in memcached
    const failable<bool> prc = memcache::put(mklist<value>("tuscanyOAuth1Token", cadr(tv)), cadr(sv), mc);
    if (!hasContent(prc))
        return mkfailure<int>(prc);

    // Redirect to the authorize URI
    const string authuri = httpd::unescape(cadr(auth)) + string("?") + http::queryString(mklist<list<value> >(tv));
    debug(authuri, "modoauth1::authorize::authuri");
    return httpd::externalRedirect(authuri, r);
}

/**
 * Extract user info from a profile/info response.
 * TODO This currently only works for Twitter, Foursquare and LinkedIn.
 * User profile parsing needs to be made configurable.
 */
const failable<list<value> > profileUserInfo(const value& cid, const string& info) {
    string b = substr(info, 0, 1);
    if (b == "[") {
        // Twitter JSON profile
        js::JSContext cx;
        const list<value> infov(json::jsonValues(content(json::readJSON(mklist<string>(info), cx))));
        if (isNil(infov))
            return mkfailure<list<value> >("Couldn't retrieve user info");
        debug(infov, "modoauth1::access_token::info");
        const list<value> uv = assoc<value>("user", car(infov));
        debug(uv, "modoauth1::access_token::userInfo");
        if (isNil(uv) || isNil(cdr(uv)))
            return mkfailure<list<value> >("Couldn't retrieve user info");
        const list<value> iv = cdr(uv);
        return cons<value>(mklist<value>("realm", cid), iv);
    }
    if (b == "{") {
        // Foursquare JSON profile
        js::JSContext cx;
        const list<value> infov(json::jsonValues(content(json::readJSON(mklist<string>(info), cx))));
        if (isNil(infov))
            return mkfailure<list<value> >("Couldn't retrieve user info");
        debug(infov, "modoauth1::access_token::info");
        const list<value> uv = assoc<value>("user", infov);
        debug(uv, "modoauth1::access_token::userInfo");
        if (isNil(uv) || isNil(cdr(uv)))
            return mkfailure<list<value> >("Couldn't retrieve user info");
        const list<value> iv = cdr(uv);
        return cons<value>(mklist<value>("realm", cid), iv);
    }
    if (b == "<") {
        // XML profile
        const list<value> infov = elementsToValues(readXML(mklist<string>(info)));
        if (isNil(infov))
            return mkfailure<list<value> >("Couldn't retrieve user info");
        debug(infov, "modoauth1::access_token::info");
        const list<value> pv = car(infov);
        debug(pv, "modoauth1::access_token::userInfo");
        if (isNil(pv) || isNil(cdr(pv)))
            return mkfailure<list<value> >("Couldn't retrieve user info");
        const list<value> iv = cdr(pv);
        return cons<value>(mklist<value>("realm", cid), iv);
    }
    return mkfailure<list<value> >("Couldn't retrieve user info");
}

/**
 * Handle an access_token request.
 */
const failable<int> accessToken(const list<list<value> >& args, request_rec* r, const list<list<value> >& appkeys, const memcache::MemCached& mc) {
    // Extract access_token URI, client ID and verification code
    const list<value> ref = assoc<value>("openauth_referrer", args);
    if (isNil(ref) || isNil(cdr(ref)))
        return mkfailure<int>("Missing openauth_referrer parameter");
    const list<value> tok = assoc<value>("oauth1_access_token", args);
    if (isNil(tok) || isNil(cdr(tok)))
        return mkfailure<int>("Missing oauth1_access_token parameter");
    const list<value> cid = assoc<value>("oauth1_client_id", args);
    if (isNil(cid) || isNil(cdr(cid)))
        return mkfailure<int>("Missing oauth1_client_id parameter");
    const list<value> info = assoc<value>("oauth1_info", args);
    if (isNil(info) || isNil(cdr(info)))
        return mkfailure<int>("Missing oauth1_info parameter");
    const list<value> tv = assoc<value>("oauth_token", args);
    if (isNil(tv) || isNil(cdr(tv)))
        return mkfailure<int>("Missing oauth_token parameter");
    const list<value> vv = assoc<value>("oauth_verifier", args);
    if (isNil(vv) || isNil(cdr(vv)))
        return mkfailure<int>("Missing oauth_verifier parameter");

    // Lookup client app configuration
    const list<value> app = assoc<value>(cadr(cid), appkeys);
    if (isNil(app) || isNil(cdr(app)))
        return mkfailure<int>(string("client id not found: ") + cadr(cid));
    list<value> appkey = cadr(app);

    // Retrieve the request token from memcached
    const failable<value> sv = memcache::get(mklist<value>("tuscanyOAuth1Token", cadr(tv)), mc);
    if (!hasContent(sv))
        return mkfailure<int>(sv);

    // Build and sign access token request URI
    const string tokuri = httpd::unescape(cadr(tok)) + string("?") + http::queryString(mklist<list<value> >(vv));
    const list<string> stokuri = sign("POST", tokuri, appkey, cadr(tv), content(sv));
    debug(stokuri, "modoauth1::access_token::stokuri");

    // Put the args into an oauth header
    string tokhdr = header(cadr(stokuri), "", cadr(vv));

    // Send the access token request
    char* ptokres = oauth_http_post2(c_str(car(stokuri)), "", c_str(tokhdr));
    if (ptokres == NULL)
        return mkfailure<int>("Couldn't post access_token request");
    const string tokres(ptokres);
    free(ptokres);
    debug(tokres, "modoauth1::access_token::res");
    const list<list<value> > tokresargs = httpd::queryArgs(tokres);

    // Retrieve the access token
    const list<value> atv = assoc<value>("oauth_token", tokresargs);
    if (isNil(atv) || isNil(cdr(atv)))
        return mkfailure<int>("Couldn't retrieve oauth_token");
    const list<value> asv = assoc<value>("oauth_token_secret", tokresargs);
    if (isNil(asv) || isNil(cdr(asv)))
        return mkfailure<int>("Couldn't retrieve oauth_token_secret");
    debug(atv, "modoauth1::access_token::token");

    // Build and sign user profile request URI
    const string profuri = httpd::unescape(cadr(info));
    const list<string> sprofuri = sign("GET", profuri, appkey, cadr(atv), cadr(asv));
    debug(sprofuri, "modoauth1::access_token::sprofuri");

    // Put the args into an oauth header
    string profhdr = header(cadr(sprofuri), "", "");

    // Send the user profile request
    char* pprofres = oauth_http_get2(c_str(car(sprofuri)), NULL, c_str(profhdr));
    if (pprofres == NULL)
        return mkfailure<int>("Couldn't get user info");
    const string profres(pprofres);
    free(pprofres);
    debug(profres, "modoauth1::access_token::profres");

    // Retrieve the user info from the profile
    const failable<list<value> > iv = profileUserInfo(cadr(cid), profres);
    if (!hasContent(iv))
        return mkfailure<int>(iv);

    // Store user info in memcached keyed by session ID
    const value sid = string("OAuth1_") + mkrand();
    const failable<bool> prc = memcache::put(mklist<value>("tuscanyOAuth1", sid), content(iv), mc);
    if (!hasContent(prc))
        return mkfailure<int>(prc);

    // Send session ID to the client in a cookie
    debug(c_str(openauth::cookie("TuscanyOAuth1", sid, httpd::hostName(r))), "modoauth1::access_token::setcookie");
    apr_table_set(r->err_headers_out, "Set-Cookie", c_str(openauth::cookie("TuscanyOAuth1", sid, httpd::hostName(r))));
    return httpd::externalRedirect(httpd::url(httpd::unescape(cadr(ref)), r), r);
}

/**
 * Check user authentication.
 */
static int checkAuthn(request_rec *r) {
    // Decline if we're not enabled or AuthType is not set to Open
    const DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_oauth1);
    if (!dc.enabled)
        return DECLINED;
    const char* atype = ap_auth_type(r);
    debug(atype, "modopenauth::checkAuthn::auth_type");
    if (atype == NULL || strcasecmp(atype, "Open"))
        return DECLINED;

    // Create a scoped memory pool
    gc_scoped_pool pool(r->pool);

    // Get the server configuration
    debug_httpdRequest(r, "modoauth1::checkAuthn::input");
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_oauth1);

    // Get session id from the request
    const maybe<string> sid = openauth::sessionID(r, "TuscanyOAuth1");
    if (hasContent(sid)) {
        // Decline if the session id was not created by this module
        if (substr(content(sid), 0, 7) != "OAuth1_")
            return DECLINED;

        // If we're authenticated store the user info in the request
        const failable<value> info = userInfo(content(sid), sc.mc);
        if (hasContent(info)) {
            r->ap_auth_type = const_cast<char*>(atype);
            return httpd::reportStatus(authenticated(dc.scopeattrs, content(info), r));
        }
    }

    // Handle OAuth authorize request step
    if (string(r->uri) == "/oauth1/authorize/") {
        r->ap_auth_type = const_cast<char*>(atype);
        return httpd::reportStatus(authorize(httpd::queryArgs(r), r, sc.appkeys, sc.mc));
    }

    // Handle OAuth access_token request step
    if (string(r->uri) == "/oauth1/access_token/") {
        r->ap_auth_type = const_cast<char*>(atype);
        return httpd::reportStatus(accessToken(httpd::queryArgs(r), r, sc.appkeys, sc.mc));
    }

    // Redirect to the login page, unless we have a session id or an authorization
    // header from another module
    if (apr_table_get(r->headers_in, (PROXYREQ_PROXY == r->proxyreq) ? "Proxy-Authorization" : "Authorization") != NULL)
        return DECLINED;
    if (hasContent(openauth::sessionID(r, "TuscanyOpenIDAuth")) ||
        hasContent(openauth::sessionID(r, "TuscanyOpenAuth")) ||
        hasContent(openauth::sessionID(r, "TuscanyOAuth2")))
        return DECLINED;
    if ((substr(string(r->uri), 0, 8) == "/oauth2/") || !isNil(assoc<value>("openid_identifier", httpd::queryArgs(r))))
        return DECLINED;
    r->ap_auth_type = const_cast<char*>(atype);
    return httpd::reportStatus(openauth::login(dc.login, r));
}

/**
 * Process the module configuration.
 */
int postConfigMerge(ServerConf& mainsc, server_rec* s) {
    if (s == NULL)
        return OK;
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_oauth1);
    debug(httpd::serverName(s), "modoauth1::postConfigMerge::serverName");

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
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_oauth1);
    debug(httpd::serverName(s), "modoauth1::postConfig::serverName");

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
        return new (gc_new<http::CURLSession>()) http::CURLSession(ca, cert, key, "");
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
    ServerConf* psc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_oauth1);
    if(psc == NULL) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_oauth1 loading failed. Causing apache to stop loading." << endl;
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
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth1);
    sc.appkeys = cons<list<value> >(mklist<value>(arg1, mklist<value>(arg2, arg3)), sc.appkeys);
    return NULL;
}
const char* confMemcached(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth1);
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
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth1);
    sc.ca = arg;
    return NULL;
}
const char* confCertFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth1);
    sc.cert = arg;
    return NULL;
}
const char* confCertKeyFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_oauth1);
    sc.key = arg;
    return NULL;
}
const char* confScopeAttr(cmd_parms *cmd, void* c, const char* arg1, const char* arg2) {
    gc_scoped_pool pool(cmd->pool);
    DirConf& dc = httpd::dirConf<DirConf>(c);
    dc.scopeattrs = cons<list<value> >(mklist<value>(arg1, arg2), dc.scopeattrs);
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE3("AddAuthOAuth1AppKey", (const char*(*)())confAppKey, NULL, RSRC_CONF, "OAuth 1.0 name app-id app-key"),
    AP_INIT_ITERATE("AddAuthOAuthMemcached", (const char*(*)())confMemcached, NULL, RSRC_CONF, "Memcached server host:port"),
    AP_INIT_FLAG("AuthOAuth", (const char*(*)())confEnabled, NULL, OR_AUTHCFG, "OAuth 1.0 authentication On | Off"),
    AP_INIT_TAKE1("AuthOAuthLoginPage", (const char*(*)())confLogin, NULL, OR_AUTHCFG, "OAuth 1.0 login page"),
    AP_INIT_TAKE1("AuthOAuthSSLCACertificateFile", (const char*(*)())confCAFile, NULL, RSRC_CONF, "OAUth 1.0 SSL CA certificate file"),
    AP_INIT_TAKE1("AuthOAuthSSLCertificateFile", (const char*(*)())confCertFile, NULL, RSRC_CONF, "OAuth 1.0 SSL certificate file"),
    AP_INIT_TAKE1("AuthOAuthSSLCertificateKeyFile", (const char*(*)())confCertKeyFile, NULL, RSRC_CONF, "OAuth 1.0 SSL certificate key file"),
    AP_INIT_TAKE2("AddAuthOAuth1ScopeAttr", (const char*(*)())confScopeAttr, NULL, OR_AUTHCFG, "OAuth 1.0 scope attribute"),
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

module AP_MODULE_DECLARE_DATA mod_tuscany_oauth1 = {
    STANDARD20_MODULE_STUFF,
    // dir config and merger
    tuscany::httpd::makeDirConf<tuscany::oauth1::DirConf>, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::oauth1::ServerConf>, NULL,
    // commands and hooks
    tuscany::oauth1::commands, tuscany::oauth1::registerHooks
};

}
