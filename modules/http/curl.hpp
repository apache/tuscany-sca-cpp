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
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "../atom/atom.hpp"
#include "../json/json.hpp"

namespace tuscany {
namespace http {

/**
 * Set to true to log HTTP content.
 */
bool logContent = false;

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
class CURLHandle {
public:
    CURLHandle() : h(curl_easy_init()) {
    }
    ~CURLHandle() {
        curl_easy_cleanup(h);
    }

    operator CURL*() const {
        return h;
    }
private:
    CURL* h;
};

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
    CURLWriteContext(const lambda<R(std::string, R)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }
    const lambda<R(std::string, R)> reduce;
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
curl_slist* headers(curl_slist* cl, const list<std::string> h) {
    if (isNil(h))
        return cl;
    return headers(curl_slist_append(cl, std::string(car(h)).c_str()), cdr(h));
}

template<typename R> const failable<list<R>, std::string> apply(const list<list<std::string> > req, const lambda<R(std::string, R)>& reduce, const R& initial, const std::string& url, const std::string& verb, CURLHandle& ch) {

    // Init the curl session
    curl_easy_reset(ch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl/1.0");

    //TODO use HTTP chunking, for now just convert request to a single string
    std::ostringstream os;
    write(cadr(req), os);
    const std::string s = os.str();
    const int sz = s.length();
    if (sz < 1400)
        curl_easy_setopt(ch, CURLOPT_TCP_NODELAY, true);

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
        return std::string(curl_easy_strerror(rc));
    long httprc;
    curl_easy_getinfo (ch, CURLINFO_RESPONSE_CODE, &httprc);
    if (httprc != 200 && httprc != 201) {
        std::ostringstream es;
        es << "HTTP code " << httprc;
        return es.str();
    }
    return mklist<R>(hcx.accum, wcx.accum);
}

/**
 * Evaluate an expression remotely, at the given URL.
 */
const failable<value, std::string>  evalExpr(const value expr, const std::string& url, CURLHandle& ch) {

    // Convert expression to a JSON-RPC request
    json::JSONContext cx;
    const failable<list<std::string>, std::string> jsreq = jsonRequest(1, car<value>(expr), cdr<value>(expr), cx);
    if (!hasValue(jsreq))
        return std::string(jsreq);

    if (logContent) {
        std::cout<< "content: " << std::endl;
        write(jsreq, std::cout);
        std::cout<< std::endl;
        std::cout.flush();
    }

    // POST it to the URL
    const list<std::string> h = mklist<std::string>("Content-Type: application/json-rpc");
    const failable<list<list<std::string> >, std::string> res = apply<list<std::string> >(mklist<list<std::string> >(h, jsreq), rcons<std::string>, list<std::string>(), url, "POST", ch);
    if (!hasValue(res))
        return std::string(res);

    // Return result
    if (logContent) {
        std::cout << "content:" << std::endl;
        write(cadr<list<std::string> >(res), std::cout);
        std::cout << std::endl;
    }
    const list<value> val = elementsToValues(json::readJSON(cadr<list<std::string> >(res), cx));
    return cadr<value>(cadr<value>(val));
}

/**
 * HTTP GET, return the resource at the given URL.
 */
template<typename R> const failable<list<R>, std::string> get(const lambda<R(std::string, R)>& reduce, const R& initial, const std::string& url, CURLHandle& ch) {
    const list<list<std::string> > req = mklist(list<std::string>(), list<std::string>());
    return apply(req, reduce, initial, url, "GET", ch);
}

/**
 * HTTP GET, return a list of values representing the resource at the given URL.
 */
const failable<value, std::string> get(const std::string& url, CURLHandle& ch) {

    // Get the contents of the resource at the given URL
    const failable<list<list<std::string> >, std::string> res = get<list<std::string> >(rcons<std::string>, list<std::string>(), url, ch);
    if (!hasValue(res))
        return std::string(res);
    const list<list<std::string> > ls = res;

    // TODO Return an ATOM feed
    const std::string ct;
    if (ct.find("application/atom+xml") != std::string::npos) {
    }

    // Return the content as a string value
    std::ostringstream os;
    write(reverse(cadr(ls)), os);
    return value(os.str());
}

/**
 * HTTP POST.
 */
const failable<value, std::string> post(const value& val, const std::string& url, CURLHandle& ch) {

    // Convert value to an ATOM entry
    const failable<list<std::string>, std::string> entry = atom::writeATOMEntry(atom::entryValuesToElements(val));
    if (!hasValue(entry))
        return std::string(entry);
    if (logContent) {
        std::cout << "content:" << std::endl;
        write(list<std::string>(entry), std::cout);
        std::cout << std::endl;
    }

    // POST it to the URL
    const list<std::string> h = mklist<std::string>("Content-Type: application/atom+xml");
    const list<list<std::string> > req = mklist<list<std::string> >(h, entry);
    const failable<list<list<std::string> >, std::string> res = apply<list<std::string> >(req, rcons<std::string>, list<std::string>(), url, "POST", ch);
    if (!hasValue(res))
        return std::string(res);
    return value(true);
}

/**
 * HTTP PUT.
 */
const failable<value, std::string> put(const value& val, const std::string& url, CURLHandle& ch) {

    // Convert value to an ATOM entry
    const failable<list<std::string>, std::string> entry = atom::writeATOMEntry(atom::entryValuesToElements(val));
    if (!hasValue(entry))
        return std::string(entry);
    if (logContent) {
        std::cout << "content:" << std::endl;
        write(list<std::string>(entry), std::cout);
        std::cout << std::endl;
    }

    // POST it to the URL
    const list<std::string> h = mklist<std::string>("Content-Type: application/atom+xml");
    const list<list<std::string> > req = mklist<list<std::string> >(h, entry);
    const failable<list<list<std::string> >, std::string> res = apply<list<std::string> >(req, rcons<std::string>, list<std::string>(), url, "PUT", ch);
    if (!hasValue(res))
        return std::string(res);
    return value(true);
}

/**
 * HTTP DELETE.
 */
const failable<value, std::string> del(const std::string& url, CURLHandle& ch) {
    const list<list<std::string> > req = mklist(list<std::string>(), list<std::string>());
    const failable<list<list<std::string> >, std::string> res = apply<list<std::string> >(req, rcons<std::string>, list<std::string>(), url, "DELETE", ch);
    if (!hasValue(res))
        return std::string(res);
    return value(true);
}

}
}

#endif /* tuscany_curl_hpp */
