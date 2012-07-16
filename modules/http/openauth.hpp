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
const char* cookieName(const char* cs) {
    if (*cs != ' ')
        return cs;
    return cookieName(cs + 1);
}
const maybe<string> sessionID(const list<string>& c, const string& key) {
    if (isNil(c))
        return maybe<string>();
    const string cn = cookieName(c_str(car(c)));
    const size_t i = find(cn, "=");
    if (i < length(cn)) {
        const list<string> kv = mklist<string>(substr(cn, 0, i), substr(cn, i+1));
        if (!isNil(kv) && !isNil(cdr(kv))) {
            if (car(kv) == key)
                return cadr(kv);
        }
    }
    return sessionID(cdr(c), key);
}

const maybe<string> sessionID(const request_rec* r, const string& key) {
    const string c = httpd::cookie(r);
    debug(c, "openauth::sessionid::cookies");
    if (length(c) == 0)
        return maybe<string>();
    return sessionID(tokenize(";", c), key);
}

/**
 * Convert a session id to a cookie string.
 */
const string cookie(const string& key, const string& sid, const string& domain) {
    const time_t t = time(NULL) + 86400;
    char exp[32];
    strftime(exp, 32, "%a, %d-%b-%Y %H:%M:%S GMT", gmtime(&t));
    const string c = key + string("=") + sid + "; expires=" + string(exp) + "; domain=." + httpd::realm(domain) + "; path=/";
    debug(c, "openauth::cookie");
    return c;
}

/**
 * Redirect to the configured login page.
 */
const failable<int> login(const string& page, const value& ref, const value& attempt, request_rec* r) {
    const list<list<value> > rarg = ref == string("/")? list<list<value> >() : mklist<list<value> >(mklist<value>("openauth_referrer", httpd::escape(httpd::url(isNil(ref)? r->uri : ref, r))));
    const list<list<value> > aarg = isNil(attempt)? list<list<value> >() : mklist<list<value> >(mklist<value>("openauth_attempt", attempt));
    const list<list<value> > largs = append<list<value> >(rarg, aarg);
    const string loc = isNil(largs)? httpd::url(page, r) : httpd::url(page, r) + string("?") + http::queryString(largs);
    debug(loc, "openauth::login::uri");
    return httpd::externalRedirect(loc, r);
}

}
}

#endif /* tuscany_openauth_hpp */
