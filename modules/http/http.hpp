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

#ifndef tuscany_http_hpp
#define tuscany_http_hpp

/**
 * CURL HTTP client functions.
 */

#include <unistd.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <apr_network_io.h>
#include <apr_portable.h>
#include <apr_poll.h>

#include "string.hpp"
#include "gc.hpp"
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "../atom/atom.hpp"
#include "../rss/rss.hpp"
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
    CURLSession() : h(NULL), p(NULL), sock(NULL), wpollset(NULL), wpollfd(NULL), rpollset(NULL), rpollfd(NULL), owner(false), ca(""), cert(""), key("") {
    }

    CURLSession(const string& ca, const string& cert, const string& key) : h(curl_easy_init()), p(gc_pool(mkpool())), sock(NULL), wpollset(NULL), wpollfd(NULL), rpollset(NULL), rpollfd(NULL), owner(true), ca(ca), cert(cert), key(key) {
    }

    CURLSession(const CURLSession& c) : h(c.h), p(c.p), sock(c.sock), wpollset(c.wpollset), wpollfd(c.wpollfd), rpollset(c.rpollset), rpollfd(c.rpollfd), owner(false), ca(c.ca), cert(c.cert), key(c.key) {
    }

    ~CURLSession() {
        if (!owner)
            return;
        if (h == NULL)
            return;
        curl_easy_cleanup(h);
        destroy(p);
    }

private:
    CURL* h;
    gc_pool p;
    apr_socket_t* sock;
    apr_pollset_t* wpollset;
    apr_pollfd_t* wpollfd;
    apr_pollset_t* rpollset;
    apr_pollfd_t* rpollfd;
    bool owner;

    friend CURL* handle(const CURLSession& cs);
    friend apr_socket_t* sock(const CURLSession& cs);
    friend const failable<bool> connect(const string& url, CURLSession& cs);
    friend const failable<bool> send(const char* c, const int l, const CURLSession& cs);
    friend const failable<int> recv(char* c, const int l, const CURLSession& cs);

public:
    string ca;
    string cert;
    string key;
};

/**
 * Returns the CURL handle used by a CURL session.
 */
CURL* handle(const CURLSession& cs) {
    return cs.h;
}

/**
 * Return an apr_socket_t for the socket used by a CURL session.
 */
apr_socket_t* sock(const CURLSession& cs) {
    return cs.sock;
}

/**
 * Convert a socket fd to an apr_socket_t.
 */
apr_socket_t* sock(const int sd, const gc_pool& p) {
    int fd = sd;
    apr_socket_t* s = NULL;
    apr_os_sock_put(&s, &fd, pool(p));
    return s;
}

/**
 * Convert a CURL return code to an error string.
 */
const string curlreason(CURLcode rc) {
    return curl_easy_strerror(rc);
}

/**
 * Convert an APR status to an error string.
 */
const string apreason(apr_status_t rc) {
    char buf[256];
    return apr_strerror(rc, buf, sizeof(buf));
}

/**
 * Setup a CURL session
 */
const failable<CURL*> setup(const string& url, const CURLSession& cs) {

    // Init CURL session
    CURL* ch = handle(cs);
    curl_easy_reset(ch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl/1.0");

    // Setup protocol options
    curl_easy_setopt(ch, CURLOPT_TCP_NODELAY, true);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(ch, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);

    // Setup SSL options
    if (cs.ca != "") {
        debug(cs.ca, "http::apply::ca");
        curl_easy_setopt(ch, CURLOPT_CAINFO, c_str(cs.ca));
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 2);
    } else
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, false);
    if (cs.cert != "") {
        debug(cs.cert, "http::apply::cert");
        curl_easy_setopt(ch, CURLOPT_SSLCERT, c_str(cs.cert));
        curl_easy_setopt(ch, CURLOPT_SSLCERTTYPE, "PEM");
    }
    if (cs.key != "") {
        debug(cs.key, "http::apply::key");
        curl_easy_setopt(ch, CURLOPT_SSLKEY, c_str(cs.key));
        curl_easy_setopt(ch, CURLOPT_SSLKEYTYPE, "PEM");
    }

    // Set target URL
    curl_easy_setopt(ch, CURLOPT_URL, c_str(url));

    return ch;
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

    // Setup the CURL session
    const failable<CURL*> fch = setup(url, cs);
    if (!hasContent(fch))
        return mkfailure<list<R>>(reason(fch));
    CURL* ch = content(fch);

    // Set the request headers
    curl_slist* hl = headers(NULL, car(hdr));
    if (hl != NULL)
        curl_easy_setopt(ch, CURLOPT_HTTPHEADER, hl);

    // Convert request body to a string
    // TODO use HTTP chunking instead
    ostringstream os;
    write(cadr(hdr), os);
    const string s = str(os);
    const int sz = length(s);

    // Setup the read, write header and write data callbacks
    CURLReadContext rcx(mklist(s));
    curl_easy_setopt(ch, CURLOPT_READFUNCTION, (size_t (*)(void*, size_t, size_t, void*))readCallback);
    curl_easy_setopt(ch, CURLOPT_READDATA, &rcx);
    CURLWriteContext<R> hcx(reduce, initial);
    curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, (size_t (*)(void*, size_t, size_t, void*))(writeCallback<R>));
    curl_easy_setopt(ch, CURLOPT_HEADERDATA, &hcx);
    CURLWriteContext<R> wcx(reduce, initial);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, (size_t (*)(void*, size_t, size_t, void*))(writeCallback<R>));
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, &wcx);

    // Apply the HTTP verb
    if (verb == "POST") {
        curl_easy_setopt(ch, CURLOPT_POST, true);
        curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, sz);
    } else if (verb == "PUT") {
        curl_easy_setopt(ch, CURLOPT_UPLOAD, true);
        curl_easy_setopt(ch, CURLOPT_INFILESIZE, sz);
    } else if (verb == "DELETE")
        curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "DELETE");
    const CURLcode rc = curl_easy_perform(ch);

    // Free the headers
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
const failable<value> evalExpr(const value& expr, const string& url, const CURLSession& cs) {
    debug(url, "http::evalExpr::url");
    debug(expr, "http::evalExpr::input");

    // Convert expression to a JSON-RPC request
    json::JSONContext cx;
    const failable<list<string> > jsreq = json::jsonRequest(1, car<value>(expr), cdr<value>(expr), cx);
    if (!hasContent(jsreq))
        return mkfailure<value>(reason(jsreq));

    // POST it to the URL
    const list<string> h = mklist<string>("Content-Type: application/json-rpc");
    const failable<list<list<string> > > res = apply<list<string> >(mklist<list<string> >(h, content(jsreq)), rcons<string>, list<string>(), url, "POST", cs);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));

    // Parse and return JSON-RPC result
    const failable<value> rval = json::jsonResultValue(cadr<list<string> >(content(res)), cx);
    debug(rval, "http::evalExpr::result");
    if (!hasContent(rval))
        return mkfailure<value>(reason(rval));
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
template<typename R> const failable<list<R> > get(const lambda<R(const string&, const R)>& reduce, const R& initial, const string& url, const CURLSession& cs) {
    debug(url, "http::get::url");
    const list<list<string> > req = mklist(list<string>(), list<string>());
    return apply(req, reduce, initial, url, "GET", cs);
}

/**
 * HTTP GET, return a list of values representing the resource at the given URL.
 */
const failable<value> getcontent(const string& url, const CURLSession& cs) {
    debug(url, "http::get::url");

    // Get the contents of the resource at the given URL
    const failable<list<list<string> > > res = get<list<string>>(rcons<string>, list<string>(), url, cs);
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
const failable<value> get(const string& url, const CURLSession& cs) {
    debug(url, "http::get::url");

    // Get the contents of the resource at the given URL
    const failable<list<list<string> > > res = get<list<string> >(rcons<string>, list<string>(), url, cs);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));
    const list<string> ls(reverse(cadr(content(res))));

    const string ct(content(contentType(car(content(res)))));
    debug(ct, "http::get::contentType");
    if (contains(ct, "application/atom+xml;type=entry")) {
        // Read an ATOM entry
        const value val(atom::entryValue(content(atom::readATOMEntry(ls))));
        debug(val, "http::get::result");
        return val;
    }
    if (contains(ct, "application/atom+xml;type=feed") || atom::isATOMFeed(ls)) {
        // Read an ATOM feed
        const value val(atom::feedValues(content(atom::readATOMFeed(ls))));
        debug(val, "http::get::result");
        return val;
    }
    if (contains(ct, "application/rss+xml") || rss::isRSSFeed(ls)) {
        // Read an RSS feed
        const value val(rss::feedValues(content(rss::readRSSFeed(ls))));
        debug(val, "http::get::result");
        return val;
    }
    if (contains(ct, "text/javascript") || contains(ct, "application/json")) {
        json::JSONContext cx;
        const value val(json::jsonValues(content(json::readJSON(ls, cx))));
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
const failable<value> post(const value& val, const string& url, const CURLSession& cs) {

    // Convert value to an ATOM entry
    const failable<list<string> > entry = atom::writeATOMEntry(atom::entryValuesToElements(val));
    if (!hasContent(entry))
        return mkfailure<value>(reason(entry));
    debug(url, "http::post::url");
    debug(content(entry), "http::post::input");

    // POST it to the URL
    const list<string> h = mklist<string>("Content-Type: application/atom+xml");
    const list<list<string> > req = mklist<list<string> >(h, content(entry));
    const failable<list<list<string> > > res = apply<list<string>>(req, rcons<string>, list<string>(), url, "POST", cs);
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
const failable<value> put(const value& val, const string& url, const CURLSession& cs) {

    // Convert value to an ATOM entry
    const failable<list<string> > entry = atom::writeATOMEntry(atom::entryValuesToElements(val));
    if (!hasContent(entry))
        return mkfailure<value>(reason(entry));
    debug(url, "http::put::url");
    debug(content(entry), "http::put::input");

    // PUT it to the URL
    const list<string> h = mklist<string>("Content-Type: application/atom+xml");
    const list<list<string> > req = mklist<list<string> >(h, content(entry));
    const failable<list<list<string> > > res = apply<list<string> >(req, rcons<string>, list<string>(), url, "PUT", cs);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));

    debug(true, "http::put::result");
    return value(true);
}

/**
 * HTTP DELETE.
 */
const failable<value, string> del(const string& url, const CURLSession& cs) {
    debug(url, "http::delete::url");

    const list<list<string> > req = mklist(list<string>(), list<string>());
    const failable<list<list<string> > > res = apply<list<string> >(req, rcons<string>, list<string>(), url, "DELETE", cs);
    if (!hasContent(res))
        return mkfailure<value>(reason(res));

    debug(true, "http::delete::result");
    return value(true);
}

/**
 * Returns the current host name.
 */
const string hostname() {
    char h[256];
    if (gethostname(h, 256) == -1)
        return "localhost";
    return h;
}

/**
 * Create an APR pollfd for a socket.
 */
apr_pollfd_t* pollfd(apr_socket_t* s, const int e, const gc_pool& p) {
    apr_pollfd_t* pfd = gc_new<apr_pollfd_t>(p);
    pfd->p = pool(p);
    pfd->desc_type = APR_POLL_SOCKET;
    pfd->reqevents = (apr_int16_t)e;
    pfd->rtnevents = (apr_int16_t)e;
    pfd->desc.s = s;
    pfd->client_data = NULL;
    return pfd;
}

/**
 * Connect to a URL.
 */
const failable<bool> connect(const string& url, CURLSession& cs) {

    // Setup the CURL session
    const failable<CURL*> fch = setup(url, cs);
    if (!hasContent(fch))
        return mkfailure<bool>(reason(fch));
    CURL* ch = content(fch);

    // Connect
    curl_easy_setopt(ch, CURLOPT_CONNECT_ONLY, true);
    const CURLcode rc = curl_easy_perform(ch);
    if (rc)
        return mkfailure<bool>(string(curl_easy_strerror(rc)));

    // Convert the connected socket to an apr_socket_t
    int sd;
    const CURLcode grc = curl_easy_getinfo(ch, CURLINFO_LASTSOCKET, &sd);
    if (grc)
        return mkfailure<bool>(string(curl_easy_strerror(grc)));
    cs.sock = sock(sd, cs.p);

    // Create pollsets and pollfds which can be used to poll the socket
    apr_status_t rpcrc = apr_pollset_create(&cs.rpollset, 1, pool(cs.p), 0);
    if (rpcrc != APR_SUCCESS)
        return mkfailure<bool>(apreason(rpcrc));
    cs.rpollfd = pollfd(cs.sock, APR_POLLIN, cs.p);
    apr_pollset_add(cs.rpollset, cs.rpollfd);
    apr_status_t wpcrc = apr_pollset_create(&cs.wpollset, 1, pool(cs.p), 0);
    if (wpcrc != APR_SUCCESS)
        return mkfailure<bool>(apreason(wpcrc));
    cs.wpollfd = pollfd(cs.sock, APR_POLLOUT, cs.p);
    apr_pollset_add(cs.wpollset, cs.wpollfd);

    return true;
}

/**
 * Send an array of chars.
 */
const failable<bool> send(const char* c, const int l, const CURLSession& cs) {

    // Send the data
    size_t wl = 0;
    const CURLcode rc = curl_easy_send(cs.h, c, (size_t)l, &wl);
    if (rc == CURLE_OK && wl == (size_t)l)
        return true;
    if (rc != CURLE_AGAIN)
        return mkfailure<bool>(curlreason(rc));

    // If the socket was not ready, wait for it to become ready
    const apr_pollfd_t* pollfds;
    apr_int32_t pollcount;
    apr_status_t pollrc = apr_pollset_poll(cs.wpollset, -1, &pollcount, &pollfds);
    if (pollrc != APR_SUCCESS)
        return mkfailure<bool>(apreason(pollrc));

    // Send what's left
    return send(c + wl, (int)((size_t)l - wl), cs);
}

/**
 * Receive an array of chars.
 */
const failable<int> recv(char* c, const int l, const CURLSession& cs) {

    // Receive data
    size_t rl;
    const CURLcode rc = curl_easy_recv(cs.h, c, (size_t)l, &rl);
    if (rc == CURLE_OK)
        return (int)rl;
    if (rc == 1)
        return 0;
    if (rc != CURLE_AGAIN)
        return mkfailure<int>(curlreason(rc));

    // If the socket was not ready, wait for it to become ready
    const apr_pollfd_t* pollfds;
    apr_int32_t pollcount;
    apr_status_t pollrc = apr_pollset_poll(cs.rpollset, -1, &pollcount, &pollfds);
    if (pollrc != APR_SUCCESS)
        return mkfailure<int>(apreason(pollrc));

    // Receive again
    return recv(c, l, cs);
}

/**
 * HTTP client proxy function.
 */
struct proxy {
    proxy(const string& uri, const string& ca, const string& cert, const string& key, const gc_pool& p) : p(p), uri(uri), ca(ca), cert(cert), key(key), cs(*(new (gc_new<CURLSession>(p)) CURLSession(ca, cert, key))) {
    }

    const value operator()(const list<value>& args) const {
        failable<value> val = evalExpr(args, uri, cs);
        if (!hasContent(val))
            return value();
        return content(val);
    }

    const gc_pool p;
    const string uri;
    const string ca;
    const string cert;
    const string key;
    const CURLSession& cs;
};

}
}

#endif /* tuscany_http_hpp */
