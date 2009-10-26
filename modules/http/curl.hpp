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
 * Context passed to CURL write callback function.
 */
template<typename R> class CURLWriteContext {
public:
    CURLWriteContext(const lambda<R(R, std::string)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }
    const lambda<R(R, std::string)> reduce;
    R accum;
};

/**
 * Called by CURL to write received data.
 */
template<typename R> size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *data) {
  CURLWriteContext<R>& wcx = *(static_cast<CURLWriteContext<R>*> (data));
  const size_t realsize = size * nmemb;
  wcx.accum = wcx.reduce(wcx.accum, std::string((const char*)ptr, realsize));
  return realsize;
}
            
/**
 * Called by CURL to write received header data.
 */
template<typename R> size_t headerCallback(void *ptr, size_t size, size_t nmemb, void *data) {
  CURLWriteContext<R>& wcx = *(static_cast<CURLWriteContext<R>*> (data));
  const size_t realsize = size * nmemb;
  wcx.accum = wcx.reduce(wcx.accum, std::string((const char*)ptr, realsize));
  return realsize;
}

/**
 * HTTP GET, get a resource from a URL.
 */
template<typename R> const failable<R, int> get(const lambda<R(R, std::string)>& reduce, const R& initial, const std::string& url) {
    CURLWriteContext<R> wcx(reduce, initial);

    // Init the curl session
    CURLHandle ch;
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(ch, CURLOPT_URL, url.c_str());
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, (size_t (*)(void*, size_t, size_t, void*))writeCallback<R>);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, &wcx);
    curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, (size_t (*)(void*, size_t, size_t, void*))headerCallback<R>);
    curl_easy_setopt(ch, CURLOPT_HEADERDATA, &wcx);
     
    // Perform the HTTP GET
    const CURLcode rc = curl_easy_perform(ch);
    if (rc)
        return rc;

    // Return the HTTP return code or content
    long httprc;
    curl_easy_getinfo (ch, CURLINFO_RESPONSE_CODE, &httprc);
    if (httprc != 200)
      return httprc;
    return wcx.accum;
}

/**
 * HTTP GET, get a list of values representing a resource from a URL.
 */
const list<std::string> writeStringList(const list<std::string>& listSoFar, const std::string& s) {
    return cons(s, listSoFar);
}

const failable<list<std::string>, int> get(const std::string& url) {
    const failable<list<std::string>, int> r = get<list<std::string> >(writeStringList, list<std::string>(), url);
    if (!hasValue(r))
        return r;
    return reverse(list<std::string>(r));
}

}
}

#endif /* tuscany_curl_hpp */
