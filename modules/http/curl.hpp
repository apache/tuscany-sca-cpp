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
#include <string>
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
 * CURL library context, one per process.
 */
class CURLContext {
public:
    CURLContext() {
        curl_global_init(CURL_GLOBAL_ALL);
    }
    ~CURLContext() {
        curl_global_cleanup();
    }
};

CURLContext curlContext;

/**
 * Represents a CURL session handle.
 */
class CURLSession {
public:
    CURLSession() : ch(new CURLHandle()) {
    }

    ~CURLSession() {
    }

    CURLSession(const CURLSession& c) : ch(c.ch) {
    }

private:
    class CURLHandle {
    public:
        CURLHandle() : h(curl_easy_init()) {
        }
        ~CURLHandle() {
            curl_easy_cleanup(h);
            h = NULL;
        }
    private:
        CURL* h;

        friend CURL* handle(const CURLSession& c);
    };

    const gc_ptr<CURLHandle> ch;

    friend CURL* handle(const CURLSession& c);
};

/**
 * Returns the CURL handle used by a CURL session.
 */
CURL* handle(const CURLSession& c) {
    return c.ch->h;
}

/**
 * Context passed to the read callback function.
 */
class CURLReadContext {
public:
    CURLReadContext(const list<std::string>& ilist) : ilist(ilist) {
    }
    list<std::string> ilist;
};

/**
 * Called by CURL to read data to send.
 */
size_t readCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    CURLReadContext& rcx = *static_cast<CURLReadContext*>(data);
    if (isNil(rcx.ilist))
        return 0;
    rcx.ilist = fragment(rcx.ilist, size * nmemb);
    const std::string s = car(rcx.ilist);
    rcx.ilist = cdr(rcx.ilist);
    s.copy((char*)ptr, s.length());
    return s.length();
}

/**
 * Context passed to CURL write callback function.
 */
template<typename R> class CURLWriteContext {
public:
    CURLWriteContext(const lambda<R(const std::string&, const R)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }
    const lambda<R(const std::string&, const R)> reduce;
    R accum;
};

/**
 * Called by CURL to write received data.
 */
template<typename R> size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *data) {
  CURLWriteContext<R>& wcx = *(static_cast<CURLWriteContext<R>*> (data));
  const size_t realsize = size * nmemb;
  wcx.accum = wcx.reduce(std::string((const char*)ptr, realsize), wcx.accum);
  return realsize;
}
            
/**
 * Called by CURL to write received header data.
 */
template<typename R> size_t headerCallback(void *ptr, size_t size, size_t nmemb, void *data) {
  CURLWriteContext<R>& wcx = *(static_cast<CURLWriteContext<R>*> (data));
  const size_t realsize = size * nmemb;
  wcx.accum = wcx.reduce(std::string((const char*)ptr, realsize), wcx.accum);
  return realsize;
}

/**
 * Apply an HTTP verb to a list containing a list of headers and a list of content, and
 * a reduce function used to process the response.
 */
curl_slist* headers(curl_slist* cl, const list<std::string>& h) {
    if (isNil(h))
        return cl;
    return headers(curl_slist_append(cl, std::string(car(h)).c_str()), cdr(h));
}

template<typename R> const failable<list<R>, std::string> apply(const list<list<std::string> >& req, const lambda<R(const std::string&, const R)>& reduce, const R& initial, const std::string& url, const std::string& verb, const CURLSession& cs) {

    // Init the curl session
    CURL* ch = handle(cs);
    curl_easy_reset(ch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl/1.0");

    //TODO use HTTP chunking, for now just convert request to a single string
    std::ostringstream os;
    write(cadr(req), os);
    const std::string s = os.str();
    const int sz = s.length();

    // Setup the read, header and write callbacks
    CURLReadContext rcx(mklist(s));
    curl_easy_setopt(ch, CURLOPT_READFUNCTION, (size_t (*)(void*, size_t, size_t, void*))readCallback);
    curl_easy_setopt(ch, CURLOPT_READDATA, &rcx);
    CURLWriteContext<R> hcx(reduce, initial);
    curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, (size_t (*)(void*, size_t, size_t, void*))headerCallback<R>);
    curl_easy_setopt(ch, CURLOPT_HEADERDATA, &hcx);
    CURLWriteContext<R> wcx(reduce, initial);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, (size_t (*)(void*, size_t, size_t, void*))writeCallback<R>);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, &wcx);
    curl_easy_setopt(ch, CURLOPT_TCP_NODELAY, true);

    // Set the request headers
    curl_slist* hl = headers(NULL, car(req));
    if (hl != NULL)
        curl_easy_setopt(ch, CURLOPT_HTTPHEADER, hl);

    // Apply the HTTP verb
    curl_easy_setopt(ch, CURLOPT_URL, url.c_str());
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
        return mkfailure<list<R>, std::string>(curl_easy_strerror(rc));
    long httprc;
    curl_easy_getinfo (ch, CURLINFO_RESPONSE_CODE, &httprc);
    if (httprc != 200 && httprc != 201) {
        std::ostringstream es;
        es << "HTTP code " << httprc;
        return mkfailure<list<R>, std::string>(es.str());
    }
    return mklist<R>(hcx.accum, wcx.accum);
}

/**
 * Evaluate an expression remotely, at the given URL.
 */
const failable<value, std::string> evalExpr(const value& expr, const std::string& url, const CURLSession& ch) {
    debug(url, "http::evalExpr::url");
    debug(expr, "http::evalExpr::input");

    // Convert expression to a JSON-RPC request
    json::JSONContext cx;
    const failable<list<std::string>, std::string> jsreq = jsonRequest(1, car<value>(expr), cdr<value>(expr), cx);
    if (!hasContent(jsreq))
        return mkfailure<value, std::string>(reason(jsreq));

    // POST it to the URL
    const list<std::string> h = mklist<std::string>("Content-Type: application/json-rpc");
    const failable<list<list<std::string> >, std::string> res = apply<list<std::string> >(mklist<list<std::string> >(h, content(jsreq)), rcons<std::string>, list<std::string>(), url, "POST", ch);
    if (!hasContent(res))
        return mkfailure<value, std::string>(reason(res));

    // Return result
    failable<list<value>, std::string> jsres = json::readJSON(cadr<list<std::string> >(content(res)), cx);
    if (!hasContent(jsres))
        return mkfailure<value, std::string>(reason(jsres));
    const list<value> val = elementsToValues(content(jsres));

    const value rval(cadr<value>(cadr<value>(val)));
    debug(rval, "http::evalExpr::result");
    return rval;
}

/**
 * Find and return a header.
 */
const failable<std::string, std::string> header(const std::string& prefix, const list<std::string>& h) {
    if (isNil(h))
        return mkfailure<std::string, std::string>(std::string("Couldn't find header: ") + prefix);
    const std::string s = car(h);
    if (s.find(prefix) != 0)
        return header(prefix, cdr(h));
    const std::string l(s.substr(prefix.length()));
    return l.substr(0, l.find_first_of("\r\n"));
}

/**
 * Find and return a location header.
 */
const failable<std::string, std::string> location(const list<std::string>& h) {
    return header("Location: ", h);
}

/**
 * Convert a location to an entry id.
 */
const failable<value, std::string> entryId(const failable<std::string, std::string> l) {
    if (!hasContent(l))
        return mkfailure<value, std::string>(reason(l));
    const std::string ls(content(l));
    return value(ls.substr(ls.find_last_of("/") + 1));
}

/**
 * Find and return a content-type header.
 */
const failable<std::string, std::string> contentType(const list<std::string>& h) {
    return header("Content-Type: ", h);
}

/**
 * HTTP GET, return the resource at the given URL.
 */
template<typename R> const failable<list<R>, std::string> get(const lambda<R(const std::string&, const R)>& reduce, const R& initial, const std::string& url, const CURLSession& ch) {
    debug(url, "http::get::url");
    const list<list<std::string> > req = mklist(list<std::string>(), list<std::string>());
    return apply(req, reduce, initial, url, "GET", ch);
}

/**
 * HTTP GET, return a list of values representing the resource at the given URL.
 */
const failable<value, std::string> get(const std::string& url, const CURLSession& ch) {
    debug(url, "http::get::url");

    // Get the contents of the resource at the given URL
    const failable<list<list<std::string> >, std::string> res = get<list<std::string> >(rcons<std::string>, list<std::string>(), url, ch);
    if (!hasContent(res))
        return mkfailure<value, std::string>(reason(res));
    const list<std::string> ls(reverse(cadr(content(res))));

    const std::string ct(content(contentType(car(content(res)))));
    if (ct == "application/atom+xml;type=entry") {
        const value val(atom::entryValue(content(atom::readEntry(ls))));
        debug(val, "http::get::result");
        return val;
    }

    // Return the content as a string value
    std::ostringstream os;
    write(ls, os);
    const value val(os.str());
    debug(val, "http::get::result");
    return val;
}

/**
 * HTTP POST.
 */
const failable<value, std::string> post(const value& val, const std::string& url, const CURLSession& ch) {

    // Convert value to an ATOM entry
    const failable<list<std::string>, std::string> entry = atom::writeATOMEntry(atom::entryValuesToElements(val));
    if (!hasContent(entry))
        return mkfailure<value, std::string>(reason(entry));
    debug(url, "http::post::url");
    debug(content(entry), "http::post::input");

    // POST it to the URL
    const list<std::string> h = mklist<std::string>("Content-Type: application/atom+xml");
    const list<list<std::string> > req = mklist<list<std::string> >(h, content(entry));
    const failable<list<list<std::string> >, std::string> res = apply<list<std::string> >(req, rcons<std::string>, list<std::string>(), url, "POST", ch);
    if (!hasContent(res))
        return mkfailure<value, std::string>(reason(res));

    // Return the new entry id from the HTTP location header
    const failable<value, std::string> eid(entryId(location(car(content(res)))));
    debug(eid, "http::post::result");
    return eid;
}

/**
 * HTTP PUT.
 */
const failable<value, std::string> put(const value& val, const std::string& url, const CURLSession& ch) {

    // Convert value to an ATOM entry
    const failable<list<std::string>, std::string> entry = atom::writeATOMEntry(atom::entryValuesToElements(val));
    if (!hasContent(entry))
        return mkfailure<value, std::string>(reason(entry));
    debug(url, "http::put::url");
    debug(content(entry), "http::put::input");

    // PUT it to the URL
    const list<std::string> h = mklist<std::string>("Content-Type: application/atom+xml");
    const list<list<std::string> > req = mklist<list<std::string> >(h, content(entry));
    const failable<list<list<std::string> >, std::string> res = apply<list<std::string> >(req, rcons<std::string>, list<std::string>(), url, "PUT", ch);
    if (!hasContent(res))
        return mkfailure<value, std::string>(reason(res));

    debug(true, "http::put::result");
    return value(true);
}

/**
 * HTTP DELETE.
 */
const failable<value, std::string> del(const std::string& url, const CURLSession& ch) {
    debug(url, "http::delete::url");

    const list<list<std::string> > req = mklist(list<std::string>(), list<std::string>());
    const failable<list<list<std::string> >, std::string> res = apply<list<std::string> >(req, rcons<std::string>, list<std::string>(), url, "DELETE", ch);
    if (!hasContent(res))
        return mkfailure<value, std::string>(reason(res));

    debug(true, "http::delete::result");
    return value(true);
}

/**
 * HTTP client proxy function.
 */
struct proxy {
    proxy(const std::string& url) : url(url) {
    }

    const value operator()(const list<value>& args) const {
        CURLSession cs;
        failable<value, std::string> val = evalExpr(args, url, cs);
        if (!hasContent(val))
            return value();
        return content(val);
    }

    const std::string url;
};

}
}

#endif /* tuscany_curl_hpp */
