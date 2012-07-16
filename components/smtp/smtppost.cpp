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
const failable<value> post(const string& url, const string& user, const string& pass, const string& from, const string& to, const string& subject, const value& val, http::CURLSession& cs) {
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
    CURL* ch = content(fch);
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
    return value(true);
}

/**
 * Evaluate an SMTP post/send.
 */
const failable<value> get(const lambda<value(const list<value>&)>& url,
        const lambda<value(const list<value>&)>& user, const lambda<value(const list<value>&)>& pass,
        const lambda<value(const list<value>&)>& from, const lambda<value(const list<value>&)>& to,
        const lambda<value(const list<value>&)>& subject, const lambda<value(const list<value>&)>& val,
        http::CURLSession& ch) {
    debug("smtppost::get");
    const value u = url(mklist<value>("get", list<value>()));
    const value i = user(mklist<value>("get", list<value>()));
    const value p = pass(mklist<value>("get", list<value>()));
    const value f = from(mklist<value>("get", list<value>()));
    const value t = to(mklist<value>("get", list<value>()));
    const value s = subject(mklist<value>("get", list<value>()));
    const value v = val(mklist<value>("get", list<value>()));
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
 * Component implementation lambda function.
 */
class applysmtp {
public:
    applysmtp(const lambda<value(const list<value>&)>& url,
            const lambda<value(const list<value>&)>& user, const lambda<value(const list<value>&)>& pass,
            const lambda<value(const list<value>&)>& from, const lambda<value(const list<value>&)>& to,
            const lambda<value(const list<value>&)>& subject, const lambda<value(const list<value>&)>& val,
            perthread_ptr<http::CURLSession>& ch) :
        url(url), user(user), pass(pass), from(from), to(to), subject(subject), val(val), ch(ch) {
    }

    const value operator()(const list<value>& params) const {
        debug(params, "smtppost::applysmtp::params");
        const value func(car(params));
        if (func == "get")
            return get(url, user, pass, from, to, subject, val, *ch);
        return mkfailure<value>();
    }

private:
    const lambda<value(const list<value>&)> url;
    const lambda<value(const list<value>&)> user;
    const lambda<value(const list<value>&)> pass;
    const lambda<value(const list<value>&)> from;
    const lambda<value(const list<value>&)> to;
    const lambda<value(const list<value>&)> subject;
    const lambda<value(const list<value>&)> val;
    perthread_ptr<http::CURLSession> ch;
};

/**
 * Create a new CURL session.
 */
const gc_ptr<http::CURLSession> newsession() {
    return new (gc_new<http::CURLSession>()) http::CURLSession("", "", "", "", 0);
}

/**
 * Start the component.
 */
const failable<value> start(const list<value>& params) {
    // Create a CURL session
    perthread_ptr<http::CURLSession> ch = perthread_ptr<http::CURLSession>(lambda<gc_ptr<http::CURLSession>()>(newsession));

    // Return the component implementation lambda function
    return value(lambda<value(const list<value>&)>(applysmtp(car(params), cadr(params), caddr(params), cadddr(params), caddddr(params), cadddddr(params), caddddddr(params), ch)));
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
