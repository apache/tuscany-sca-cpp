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
 * SMTP client component implementation.
 */

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "function.hpp"
#include "list.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "../../modules/http/http.hpp"

namespace tuscany {
namespace smtppost {

/**
 * Post/send an email message using SMTP.
 */
const failable<value> post(const string& url, const string& user, const string& pass, const string& from, const string& to, const string& subject, const value& val, const http::CURLSession& cs) {
    // Convert value to a content request
    const failable<list<list<string> > > freq = http::contentRequest(val, url);
    if (!hasContent(freq))
        return mkfailure<value>(freq);
    const list<list<string> > req = content(freq);
    debug(req, "smtp::post::input");

    // Setup the CURL session
    const failable<CURL*> fch = http::setup(url, cs);
    if (!hasContent(fch)) {
        http::cleanup(cs);
        return mkfailure<value>(fch);
    }
    CURL* const ch = content(fch);
    curl_easy_setopt(ch, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

    // Convert message to a string
    ostringstream os;
    os << "From: " << from << "\n";
    os << "To: " << to << "\n";
    os << "Subject: " << subject << "\n";
    os << car(car(req)) << "\n\n";
    write(cadr(req), os);

    // Setup the read callbacks
    http::CURLReadContext rcx(mklist(str(os)));
    curl_easy_setopt(ch, CURLOPT_READFUNCTION, (size_t (*)(void*, size_t, size_t, void*))http::readCallback);
    curl_easy_setopt(ch, CURLOPT_READDATA, &rcx);

    // Setup the message properties
    curl_easy_setopt(ch, CURLOPT_USERNAME, c_str(user));
    curl_easy_setopt(ch, CURLOPT_PASSWORD, c_str(pass));
    curl_easy_setopt(ch, CURLOPT_MAIL_FROM, c_str(from));
    struct curl_slist* rcpt = curl_slist_append(NULL, c_str(to));
    curl_easy_setopt(ch, CURLOPT_MAIL_RCPT, rcpt);

    // Perform the SMTP send
    const CURLcode rc = curl_easy_perform(ch);

    // Free the recipients
    curl_slist_free_all(rcpt);

    // Return the CURL return code or true
    if (rc) {
        http::cleanup(cs);
        return mkfailure<value>(string(curl_easy_strerror(rc)));
    }

    http::cleanup(cs);
    return trueValue;
}

/**
 * Evaluate an SMTP post/send.
 */
const failable<value> get(const lvvlambda& url,
        const lvvlambda& user, const lvvlambda& pass,
        const lvvlambda& from, const lvvlambda& to,
        const lvvlambda& subject, const lvvlambda& val,
        const http::CURLSession& ch) {
    debug("smtppost::get");
    const value u = url(mklist<value>("get", nilListValue));
    const value i = user(mklist<value>("get", nilListValue));
    const value p = pass(mklist<value>("get", nilListValue));
    const value f = from(mklist<value>("get", nilListValue));
    const value t = to(mklist<value>("get", nilListValue));
    const value s = subject(mklist<value>("get", nilListValue));
    const value v = val(mklist<value>("get", nilListValue));
    debug(u, "smtppost::get::url");
    debug(i, "smtppost::get::user");
    debug(p, "smtppost::get::pass");
    debug(f, "smtppost::get::from");
    debug(t, "smtppost::get::to");
    debug(s, "smtppost::get::subject");
    debug(v, "smtppost::get::val");
    return post(u, i, p, f, t, s, v, ch);
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Create a CURL session
    const gc_pool cp(gc_current_pool());
    const lambda<const gc_ptr<http::CURLSession>()> newsession = [cp]() -> const gc_ptr<http::CURLSession> {
        const gc_scoped_pool sp(pool(cp));
        return new (gc_new<http::CURLSession>()) http::CURLSession(emptyString, emptyString, emptyString, emptyString, 0);
    };
    const perthread_ptr<http::CURLSession> ch = *(new (gc_new<perthread_ptr<http::CURLSession> >()) perthread_ptr<http::CURLSession>(newsession));

    // Return the component implementation lambda function
    const lvvlambda url = car(params);
    const lvvlambda user = cadr(params);
    const lvvlambda pass = caddr(params);
    const lvvlambda from = cadddr(params);
    const lvvlambda to = caddddr(params);
    const lvvlambda subject = cadddddr(params);
    const lvvlambda val = caddddddr(params);
    const lvvlambda applysmtp = [url, user, pass, from, to, subject, val, ch](const list<value>& params) -> const value {
        debug(params, "smtppost::applysmtp::params");
        const value func(car(params));
        if (func == "get")
            return get(url, user, pass, from, to, subject, val, *ch);
        return mkfailure<value>();
    };
    return value(applysmtp);
}

}
}

extern "C" {

const tuscany::value apply(const tuscany::list<tuscany::value>& params) {
    const tuscany::value func(car(params));
    if (func == "start")
        return tuscany::smtppost::start(cdr(params));
    return tuscany::mkfailure<tuscany::value>();
}

}
