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
const maybe<string> sessionID(const list<string> c) {
    if (isNil(c))
        return maybe<string>();
    const string cn = cookieName(c_str(car(c)));
    const size_t i = find(cn, "=");
    if (i < length(cn)) {
        const list<string> kv = mklist<string>(substr(cn, 0, i), substr(cn, i+1));
        if (!isNil(kv) && !isNil(cdr(kv))) {
            if (car(kv) == "TuscanyOpenAuth")
                return cadr(kv);
        }
    }
    return sessionID(cdr(c));
}

const maybe<string> sessionID(const request_rec* r) {
    const string c = httpd::cookie(r);
    debug(c, "openauth::sessionid::cookies");
    if (length(c) == 0)
        return maybe<string>();
    return sessionID(tokenize(";", c));
}

/**
 * Convert a session id to a cookie string.
 */
const string cookie(const string& sid, const string& domain) {
    const time_t t = time(NULL) + 86400;
    char exp[32];
    strftime(exp, 32, "%a, %d-%b-%Y %H:%M:%S GMT", gmtime(&t));
    const string c = string("TuscanyOpenAuth=") + sid + ";domain=." + domain + ";path=/;expires=" + string(exp) + ";secure=TRUE";
    debug(c, "openauth::cookie");
    return c;
}

/**
 * Redirect to the configured login page.
 */
const failable<int> login(const string& page, request_rec* r) {
    const list<list<value> > largs = mklist<list<value> >(mklist<value>("openauth_referrer", httpd::escape(httpd::url(r->uri, r))));
    const string loc = httpd::url(page, r) + string("?") + http::queryString(largs);
    debug(loc, "openauth::login::uri");
    return httpd::externalRedirect(loc, r);
}

}
}

#endif /* tuscany_openauth_hpp */
