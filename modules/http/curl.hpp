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

#ifndef tuscany_curl_hpp
#define tuscany_curl_hpp

/**
 * CURL HTTP client functions.
 */

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include "string.hpp"
#include "gc.hpp"
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "../atom/atom.hpp"
#include "../json/json.hpp"

namespace tuscany {
namespace http {

/**
 * CURL library runtime, one per process.
 */
class CURLRuntime {
public:
    CURLRuntime() {
        curl_global_init(CURL_GLOBAL_ALL);
    }
} curlRuntime;

/**
 * Represents a CURL session handle.
 */
class CURLSession {
public:
    CURLSession() : h(curl_easy_init()), owner(true) {
    }

    CURLSession(const CURLSession& c) : h(c.h), owner(false) {
    }

    ~CURLSession() {
        if (!owner)
            return;
        if (h == NULL)
            return;
        curl_easy_cleanup(h);
    }

private:
    CURL* h;
    const bool owner;

    friend CURL* handle(const CURLSession& c);
};

/**
 * Returns the CURL handle used by a CURL session.
 */
CURL* handle(const CURLSession& c) {
    return c.h;
}

/**
 * Context passed to the read callback function.
 */
class CURLReadContext {
public:
    CURLReadContext(const list<string>& ilist) : ilist(ilist) {
    }
    list<string> ilist;
};

/**
 * Called by CURL to read data to send.
 */
size_t readCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    CURLReadContext& rcx = *static_cast<CURLReadContext*>(data);
    if (isNil(rcx.ilist))
        return 0;
    const list<string> f(fragment(rcx.ilist, size * nmemb));
    const string s = car(f);
    rcx.ilist = cdr(f);
    memcpy(ptr, c_str(s), length(s));
    return length(s);
}

/**
 * Context passed to CURL write callback function.
 */
template<typename R> class CURLWriteContext {
public:
    CURLWriteContext(const lambda<R(const string&, const R)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }
    const lambda<R(const string&, const R)> reduce;
    R accum;
};

/**
 * Called by CURL to write received data.
 */
template<typename R> size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    CURLWriteContext<R>& wcx = *(static_cast<CURLWriteContext<R>*> (data));
    const size_t realsize = size * nmemb;
    wcx.accum = wcx.reduce(string((const char*)ptr, realsize), wcx.accum);
    return realsize;
}

/**
 * Apply an HTTP verb to a list containing a list of headers and a list of content, and
 * a reduce function used to process the response.
 */
curl_slist* headers(curl_slist* cl, const list<string>& h) {
    if (isNil(h))
        return cl;
    return headers(curl_slist_append(cl, c_str(string(car(h)))), cdr(h));
}

template<typename R> const failable<list<R> > apply(const list<list<string> >& hdr, const lambda<R(const string&, const R)>& reduce, const R& initial, const string& url, const string& verb, const CURLSession& cs) {

    // Init the curl session
    CURL* ch = handle(cs);
    curl_easy_reset(ch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl/1.0");

    //TODO use HTTP chunking, for now just convert request to a single string
    ostringstream os;
    write(cadr(hdr), os);
    const string s = str(os);
    const int sz = length(s);

    // Setup the read, header and write callbacks
    CURLReadContext rcx(mklist(s));
    curl_easy_setopt(ch, CURLOPT_READFUNCTION, (size_t (*)(void*, size_t, size_t, void*))readCallback);
    curl_easy_setopt(ch, CURLOPT_READDATA, &rcx);
    CURLWriteContext<R> hcx(reduce, initial);
    curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, (size_t (*)(void*, size_t, size_t, void*))(writeCallback<R>));
    curl_easy_setopt(ch, CURLOPT_HEADERDATA, &hcx);
    CURLWriteContext<R> wcx(reduce, initial);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, (size_t (*)(void*, size_t, size_t, void*))(writeCallback<R>));
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, &wcx);
    curl_easy_setopt(ch, CURLOPT_TCP_NODELAY, true);

    // Set the request headers
    curl_slist* hl = headers(NULL, car(hdr));
    if (hl != NULL)
        curl_easy_setopt(ch, CURLOPT_HTTPHEADER, hl);

    // Apply the HTTP verb
    curl_easy_setopt(ch, CURLOPT_URL, c_str(url));
    if (verb == "POST") {
        curl_easy_setopt(ch, CURLOPT_POST, true);
        curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, sz);
    } else if (verb == "PUT") {
        curl_easy_setopt(ch, CURLOPT_UPLOAD, true);
        curl_easy_setopt(ch, CURLOPT_INFILESIZE, sz);
    } else if (verb == "DELETE")
        curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "DELETE");
    const CURLcode rc = curl_easy_perform(ch);

    if (hl != NULL)
        curl_slist_free_all(hl);

    // Return the HTTP return code or content
    if (rc)
        return mkfailure<list<R> >(string(curl_easy_strerror(rc)));
    long httprc;
    curl_easy_getinfo (ch, CURLINFO_RESPONSE_CODE, &httprc);
    if (httprc != 200 && httprc != 201) {
        ostringstream es;
        es << "HTTP code " << httprc;
        return mkfailure<list<R> >(str(es));
    }
    return mklist<R>(hcx.accum, wcx.accum);
}

/**
 * Evaluate an expression remotely, at the given URL.
 */
const failable<value> evalExpr(const value& expr, const string& url, const CURLSession& ch) {
    debug(url, "http::evalExpr::url");
    debug(expr, "http::evalExpr::input");

    // Convert expression to a JSON-RPC request
    json::JSONContext cx;
    const failable<list<string> > jsreq = json::jsonRequest(1, car<value>(expr), cdr<value>(expr), cx);
    if (!hasContent(jsreq))
        return mkfailure<value>(reason(jsreq));

    // POST it to the URL
    const list<string> h = mklist<string>("Content-Type: application/json-rpc");
    const failable<list<list<string> > > res = apply<list<string> >(mklist<list<string> >(h, content(jsreq)), rcons<string>, list<string>(), url, "POST", ch);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));

    // Parse and return JSON-RPC result
    const failable<value> rval = json::jsonResultValue(cadr<list<string> >(content(res)), cx);
    if (!hasContent(rval))
        return mkfailure<value>(reason(rval));
    debug(content(rval), "http::evalExpr::result");
    return content(rval);
}

/**
 * Find and return a header.
 */
const failable<string> header(const char* prefix, const list<string>& h) {
    if (isNil(h))
        return mkfailure<string>(string("Couldn't find header: ") + prefix);
    const string s = car(h);
    if (find(s, prefix) != 0)
        return header(prefix, cdr(h));
    const string l(substr(s, length(prefix)));
    return substr(l, 0, find_first_of(l, "\r\n"));
}

/**
 * Find and return a location header.
 */
const failable<string> location(const list<string>& h) {
    return header("Location: ", h);
}

/**
 * Convert a location to an entry id.
 */
const failable<value> entryId(const failable<string> l) {
    if (!hasContent(l))
        return mkfailure<value>(reason(l));
    const string ls(content(l));
    return value(mklist<value>(string(substr(ls, find_last(ls, '/') + 1))));
}

/**
 * Find and return a content-type header.
 */
const failable<string> contentType(const list<string>& h) {
    return header("Content-Type: ", h);
}

/**
 * HTTP GET, return the resource at the given URL.
 */
template<typename R> const failable<list<R> > get(const lambda<R(const string&, const R)>& reduce, const R& initial, const string& url, const CURLSession& ch) {
    debug(url, "http::get::url");
    const list<list<string> > req = mklist(list<string>(), list<string>());
    return apply(req, reduce, initial, url, "GET", ch);
}

/**
 * HTTP GET, return a list of values representing the resource at the given URL.
 */
const failable<value> getcontent(const string& url, const CURLSession& ch) {
    debug(url, "http::get::url");

    // Get the contents of the resource at the given URL
    const failable<list<list<string> > > res = get<list<string>>(rcons<string>, list<string>(), url, ch);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));
    const list<string> ls(reverse(cadr(content(res))));

    // Return the content as a list of values
    const value val(mkvalues(ls));
    debug(val, "http::get::result");
    return val;
}

/**
 * HTTP GET, return a list of values representing the resource at the given URL.
 */
const failable<value> get(const string& url, const CURLSession& ch) {
    debug(url, "http::get::url");

    // Get the contents of the resource at the given URL
    const failable<list<list<string> > > res = get<list<string> >(rcons<string>, list<string>(), url, ch);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));
    const list<string> ls(reverse(cadr(content(res))));

    const string ct(content(contentType(car(content(res)))));
    if (ct == "application/atom+xml;type=entry") {
        const value val(atom::entryValue(content(atom::readEntry(ls))));
        debug(val, "http::get::result");
        return val;
    }

    // Return the content as a list of values
    const value val(mkvalues(ls));
    debug(val, "http::get::result");
    return val;
}

/**
 * HTTP POST.
 */
const failable<value> post(const value& val, const string& url, const CURLSession& ch) {

    // Convert value to an ATOM entry
    const failable<list<string> > entry = atom::writeATOMEntry(atom::entryValuesToElements(val));
    if (!hasContent(entry))
        return mkfailure<value>(reason(entry));
    debug(url, "http::post::url");
    debug(content(entry), "http::post::input");

    // POST it to the URL
    const list<string> h = mklist<string>("Content-Type: application/atom+xml");
    const list<list<string> > req = mklist<list<string> >(h, content(entry));
    const failable<list<list<string> > > res = apply<list<string>>(req, rcons<string>, list<string>(), url, "POST", ch);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));

    // Return the new entry id from the HTTP location header
    const failable<value> eid(entryId(location(car(content(res)))));
    debug(eid, "http::post::result");
    return eid;
}

/**
 * HTTP PUT.
 */
const failable<value> put(const value& val, const string& url, const CURLSession& ch) {

    // Convert value to an ATOM entry
    const failable<list<string> > entry = atom::writeATOMEntry(atom::entryValuesToElements(val));
    if (!hasContent(entry))
        return mkfailure<value>(reason(entry));
    debug(url, "http::put::url");
    debug(content(entry), "http::put::input");

    // PUT it to the URL
    const list<string> h = mklist<string>("Content-Type: application/atom+xml");
    const list<list<string> > req = mklist<list<string> >(h, content(entry));
    const failable<list<list<string> > > res = apply<list<string> >(req, rcons<string>, list<string>(), url, "PUT", ch);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));

    debug(true, "http::put::result");
    return value(true);
}

/**
 * HTTP DELETE.
 */
const failable<value, string> del(const string& url, const CURLSession& ch) {
    debug(url, "http::delete::url");

    const list<list<string> > req = mklist(list<string>(), list<string>());
    const failable<list<list<string> > > res = apply<list<string> >(req, rcons<string>, list<string>(), url, "DELETE", ch);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));

    debug(true, "http::delete::result");
    return value(true);
}

/**
 * HTTP client proxy function.
 */
struct proxy {
    proxy(const string& uri) : uri(uri) {
    }

    const value operator()(const list<value>& args) const {
        CURLSession cs;
        failable<value> val = evalExpr(args, uri, cs);
        if (!hasContent(val))
            return value();
        return content(val);
    }

    const string uri;
};

}
}

#endif /* tuscany_curl_hpp */
