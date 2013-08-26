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

#ifndef tuscany_openauth_hpp
#define tuscany_openauth_hpp

/**
 * Tuscany Open auth support utility functions.
 */

#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../json/json.hpp"
#include "../http/httpd.hpp"
#include "../http/http.hpp"

namespace tuscany {
namespace openauth {

/**
 * Return the session id from a request.
 */
const char* const cookieName(const char* const cs) {
    if (*cs != ' ')
        return cs;
    return cookieName(cs + 1);
}

const maybe<string> sessionID(const list<string>& c, const string& key) {
    if (isNull(c))
        return maybe<string>();
    const string cn = cookieName(c_str(car(c)));
    const size_t i = find(cn, "=");
    if (i < length(cn)) {
        const list<string> kv = mklist<string>(substr(cn, 0, i), substr(cn, i+1));
        if (!isNull(kv) && !isNull(cdr(kv))) {
            if (car(kv) == key)
                return cadr(kv);
        }
    }
    return sessionID(cdr(c), key);
}

const maybe<string> sessionID(const request_rec* const r, const string& key) {
    const string c = httpd::cookie(r);
    debug(c, "openauth::sessionid::cookies");
    if (length(c) == 0)
        return maybe<string>();
    return sessionID(tokenize(";", c), key);
}

/**
 * Convert a number of seconds to an expiration date.
 */
const string expires(const int s) {
    const time_t t = time(NULL) + s;
    char exp[32];
    strftime(exp, 32, "%a, %d-%b-%Y %H:%M:%S GMT", gmtime(&t));
    return string(exp);
}

/**
 * Convert a session id to a cookie string.
 */
const string cookie(const string& key, const string& sid, const string& domain) {
    if (length(sid) == 0) {
        const string c = key + string("=") + "; max-age=0; domain=." + httpd::realm(domain) + "; path=/; secure; httponly";
        debug(c, "openauth::cookie");
        return c;
    }
    const string exp = "604800";
    const string c = key + string("=") + sid + (length(exp) != 0? string("; max-age=") + exp : emptyString)  + "; domain=." + httpd::realm(domain) + "; path=/; secure; httponly";
    debug(c, "openauth::cookie");
    return c;
}

/**
 * Redirect to the configured login page.
 */
const failable<int> login(const string& page, const value& ref, const value& attempt, request_rec* const r) {

    // Don't redirect non-cacheable requests, just respond with an uncacheable 403 response
    const char* cc = apr_table_get(r->headers_in, "X-Cache-Control");
    if(cc != NULL && !strcmp(cc, "no-cache")) {
        apr_table_setn(r->headers_out, "Cache-Control", "no-cache, no-store, must-revalidate, max-age=0");
        apr_table_setn(r->err_headers_out, "Cache-Control", "no-cache, no-store, must-revalidate, max-age=0");
        return HTTP_FORBIDDEN;
    }

    // Redirect to the login page
    const list<value> rarg = ref == string("/")? nilListValue : mklist<value>(mklist<value>("openauth_referrer", httpd::escape(httpd::url(isNull(ref)? r->uri : ref, r))));
    const list<value> aarg = isNull(attempt)? nilListValue : mklist<value>(mklist<value>("openauth_attempt", attempt));
    const list<value> largs = append(rarg, aarg);
    const string loc = isNull(largs)? httpd::url(page, r) : httpd::url(page, r) + string("?") + http::queryString(largs);
    debug(loc, "openauth::login::uri");
    return httpd::externalRedirect(loc, r);
}

/**
 * Report a request auth status.
 */
const int reportStatus(const failable<int>& authrc, const string& page, const value& attempt, request_rec* const r) {
    debug(authrc, "openauth::reportStatus::authrc");

    // Redirect to the login page if not authorized
    if (!hasContent(authrc) && rcode(authrc) == HTTP_UNAUTHORIZED) {

        // Clear any auth cookies
        if(hasContent(sessionID(r, "TuscanyOpenAuth")))
            apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOpenAuth", emptyString, httpd::hostName(r))));
        if(hasContent(sessionID(r, "TuscanyOpenIDAuth")))
            apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOpenIDAuth", emptyString, httpd::hostName(r))));
        if(hasContent(sessionID(r, "TuscanyOAuth1")))
            apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOAuth1", emptyString, httpd::hostName(r))));
        if(hasContent(sessionID(r, "TuscanyOAuth2")))
            apr_table_set(r->err_headers_out, "Set-Cookie", c_str(cookie("TuscanyOAuth2", emptyString, httpd::hostName(r))));

        return httpd::reportStatus(login(page, string("/"), attempt, r));
    }

    return httpd::reportStatus(authrc);
}

}
}

#endif /* tuscany_openauth_hpp */
