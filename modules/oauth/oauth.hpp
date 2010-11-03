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

#ifndef tuscany_oauth_hpp
#define tuscany_oauth_hpp

/**
 * OAuth support utility functions.
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
#include "../../components/cache/memcache.hpp"

namespace tuscany {
namespace oauth {

/**
 * Return the session id from a request.
 */
const char* cookieName(const char* cs) {
    if (*cs != ' ')
        return cs;
    return cookieName(cs + 1);
}
const maybe<string> sessionID(const list<string> c) {
    if (isNil(c))
        return maybe<string>();
    const list<string> kv = tokenize("=", cookieName(c_str(car(c))));
    if (!isNil(kv) && !isNil(cdr(kv))) {
        if (car(kv) == "TuscanyOpenAuth")
            return cadr(kv);
    }
    return sessionID(cdr(c));
}

const maybe<string> sessionID(const request_rec* r) {
    const char* c = apr_table_get(r->headers_in, "Cookie");
    debug(c, "oauth::sessionid::cookies");
    if (c == NULL)
        return maybe<string>();
    return sessionID(tokenize(";", c));
}

/**
 * Return the user info for a session.
 */
const failable<value> userInfo(const value& sid, const memcache::MemCached& mc) {
    return memcache::get(mklist<value>("tuscanyOpenAuth", sid), mc);
}

/**
 * Convert a session id to a cookie string.
 */
const string cookie(const string& sid) {
    const time_t t = time(NULL) + 86400;
    char exp[32];
    strftime(exp, 32, "%a, %d-%b-%Y %H:%M:%S GMT", gmtime(&t));
    const string c = string("TuscanyOpenAuth=") + sid + string(";path=/;expires=" + string(exp)) + ";secure=TRUE";
    debug(c, "oauth::cookie");
    return c;
}

/**
 * Redirect to the configured login page.
 */
const failable<int> login(const string& page, request_rec* r) {
    const list<list<value> > largs = mklist<list<value> >(mklist<value>("openauth_referrer", httpd::escape(httpd::url(r->uri, r))));
    const string loc = httpd::url(page, r) + string("?") + httpd::queryString(largs);
    debug(loc, "oauth::login::uri");
    return httpd::externalRedirect(loc, r);
}

}
}

#endif /* tuscany_oauth_hpp */
