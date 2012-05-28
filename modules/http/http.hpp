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
#include <curl/easy.h>
#include <apr.h>
#include <apr_lib.h>
#include <apr_network_io.h>
#include <apr_portable.h>
#include <apr_poll.h>
#include <apr_uri.h>

#include "string.hpp"
#include "gc.hpp"
#include "list.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "parallel.hpp"
#include "../scheme/io.hpp"
#include "../atom/atom.hpp"
#include "../rss/rss.hpp"
#include "../json/json.hpp"

namespace tuscany {
namespace http {

/**
 * Enable CURL verbose debug log.
 */
//#define WANT_MAINTAINER_CURL_VERBOSE

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
    CURLSession() : h(NULL), p(NULL), sock(NULL), wpollset(NULL), wpollfd(NULL), rpollset(NULL), rpollfd(NULL), owner(false), ca(""), cert(""), key(""), cookie("") {
    }

    CURLSession(const string& ca, const string& cert, const string& key, const string& cookie) : h(curl_easy_init()), p(gc_pool(mkpool())), sock(NULL), wpollset(NULL), wpollfd(NULL), rpollset(NULL), rpollfd(NULL), owner(true), ca(ca), cert(cert), key(key), cookie(cookie) {
    }

    CURLSession(const CURLSession& c) : h(c.h), p(c.p), sock(c.sock), wpollset(c.wpollset), wpollfd(c.wpollfd), rpollset(c.rpollset), rpollfd(c.rpollfd), owner(false), ca(c.ca), cert(c.cert), key(c.key), cookie(c.cookie) {
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
    friend const failable<CURL*> setup(const string& url, const CURLSession& cs);
    friend const failable<bool> connect(const string& url, CURLSession& cs);
    friend const failable<bool> send(const char* c, const size_t l, const CURLSession& cs);
    friend const failable<size_t> recv(char* c, const size_t l, const CURLSession& cs);

public:
    string ca;
    string cert;
    string key;
    string cookie;
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
 * Escape a URI or a query argument.
 */
const char escape_c2x[] = "0123456789ABCDEF";

const string escape(const string& unesc, const char* reserv) {
    char* copy = (char*)apr_palloc(gc_current_pool(), 3 * length(unesc) + 3);
    const unsigned char* s = (const unsigned char *)c_str(unesc);
    unsigned char* d = (unsigned char*)copy;
    unsigned c;
    while ((c = *s)) {
        if (!apr_isalnum(c) && !strchr(reserv, c)) {
            *d++ = '%';
            *d++ = escape_c2x[c >> 4];
            *d++ = escape_c2x[c & 0xf];
        }
        else {
            *d++ = (unsigned char)c;
        }
        ++s;
    }
    *d = '\0';
    return copy;
}

const string escapeURI(const string& uri) {
    debug(uri, "http::escapeURI::uri");
    const string e = escape(uri, "?$-_.+!*'(),:@&=/~%");
    debug(e, "http::escapeURI::result");
    return e;
}

const string escapeArg(const string& arg) {
    debug(arg, "http::escapeArg::arg");
    const string e = escape(arg, "-_.~");
    debug(e, "http::escapeArg::result");
    return e;
}

/**
 * Return true if a URI is absolute.
 */
const bool isAbsolute(const string& uri) {
    return contains(uri, "://");
}

/**
 * Parse a URI and return its host name.
 */
const string hostName(const string& uri, const gc_pool& p) {
    apr_uri_t u;
    const apr_status_t rc = apr_uri_parse(pool(p), c_str(uri), &u);
    if (rc != APR_SUCCESS)
        return "";
    if (u.hostname == NULL)
        return "";
    return u.hostname;
}

/**
 * Parse a URI and return its scheme.
 */
const string scheme(const string& uri, const gc_pool& p) {
    apr_uri_t u;
    const apr_status_t rc = apr_uri_parse(pool(p), c_str(uri), &u);
    if (rc != APR_SUCCESS)
        return "";
    if (u.scheme == NULL)
        return "";
    return u.scheme;
}

/**
 * Return the first subdomain name in a host name.
 */
const string subDomain(const string& host) {
    return substr(host, 0, find(host, '.'));
}

/**
 * Return the top domain name in a host name.
 */
const string topDomain(const string& host) {
    const size_t d = find(host, '.');
    return d == length(host) ? host : substr(host, d + 1);
}

/**
 * Setup a CURL session
 */
const failable<CURL*> setup(const string& url, const CURLSession& cs) {

    // Init CURL session
    CURL* ch = handle(cs);
    curl_easy_reset(ch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl/1.0");
#ifdef WANT_MAINTAINER_CURL_VERBOSE
    curl_easy_setopt(ch, CURLOPT_VERBOSE, true);
#endif

    // Setup protocol options
    curl_easy_setopt(ch, CURLOPT_TCP_NODELAY, true);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(ch, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);

    // Setup SSL options
    if (cs.ca != "") {
        debug(cs.ca, "http::setup::ca");
        curl_easy_setopt(ch, CURLOPT_CAINFO, c_str(cs.ca));
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 2);
    } else
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, false);
    if (cs.cert != "") {
        debug(cs.cert, "http::setup::cert");
        curl_easy_setopt(ch, CURLOPT_SSLCERT, c_str(cs.cert));
        curl_easy_setopt(ch, CURLOPT_SSLCERTTYPE, "PEM");
    }
    if (cs.key != "") {
        debug(cs.key, "http::setup::key");
        curl_easy_setopt(ch, CURLOPT_SSLKEY, c_str(cs.key));
        curl_easy_setopt(ch, CURLOPT_SSLKEYTYPE, "PEM");
    }
    if (cs.cookie != "") {
        debug(cs.cookie, "http::setup::cookie");
        curl_easy_setopt(ch, CURLOPT_COOKIE, c_str(cs.cookie));
    }

    // Set up HTTP basic auth if requested
    apr_uri_t u;
    apr_pool_t* p = gc_current_pool();
    const apr_status_t prc = apr_uri_parse(p, c_str(url), &u);
    if (prc == APR_SUCCESS) {
        if (u.user != NULL) {
            debug(u.user, "http::setup::user");
            curl_easy_setopt(ch, CURLOPT_USERNAME, u.user);
        }
        if (u.password != NULL) {
            debug(u.password, "http::setup::pass");
            curl_easy_setopt(ch, CURLOPT_PASSWORD, u.password);
        }
        if (u.user != NULL || u.password != NULL) {
            curl_easy_setopt(ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
            
            // Set target URL, omitting the user:password part
            curl_easy_setopt(ch, CURLOPT_URL, c_str(escapeURI(apr_uri_unparse(p, &u, APR_URI_UNP_OMITUSERINFO))));

            return ch;
        }
    }

    // Set target URL
    curl_easy_setopt(ch, CURLOPT_URL, c_str(escapeURI(url)));

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
    debug(url, "http::apply::url");
    debug(verb, "http::apply::verb");

    // Setup the CURL session
    const failable<CURL*> fch = setup(url, cs);
    if (!hasContent(fch))
        return mkfailure<list<R>>(fch);
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
    const size_t sz = length(s);

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
    } else if (verb == "PATCH") {
        curl_easy_setopt(ch, CURLOPT_UPLOAD, true);
        curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "PATCH");
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
    js::JSContext cx;
    const failable<list<string> > jsreq = json::jsonRequest(1, car<value>(expr), cdr<value>(expr), cx);
    if (!hasContent(jsreq))
        return mkfailure<value>(jsreq);

    // POST it to the URL
    const list<string> h = mklist<string>("Content-Type: application/json-rpc");
    const failable<list<list<string> > > res = apply<list<string> >(mklist<list<string> >(h, content(jsreq)), rcons<string>, list<string>(), url, "POST", cs);
    if (!hasContent(res))
        return mkfailure<value>(res);

    // Parse and return JSON-RPC result
    const failable<value> rval = json::jsonResultValue(cadr<list<string> >(content(res)), cx);
    debug(rval, "http::evalExpr::result");
    if (!hasContent(rval))
        return mkfailure<value>(rval);
    return content(rval);
}

/**
 * Find and return a header.
 */
const maybe<string> header(const char* prefix, const list<string>& h) {
    if (isNil(h))
        return maybe<string>();
    const string s = car(h);
    if (find(s, prefix) != 0)
        return header(prefix, cdr(h));
    const string l(substr(s, length(prefix)));
    return substr(l, 0, find_first_of(l, "\r\n"));
}

/**
 * Find and return a location header.
 */
const string location(const list<string>& h) {
    const maybe<string> l = header("Location: ", h);
    return hasContent(l)? content(l) : "";
}

/**
 * Convert a location to an entry id.
 */
const value entryId(const failable<string> l) {
    if (!hasContent(l))
        return list<value>();
    const string ls(content(l));
    return value(mklist<value>(string(substr(ls, find_last(ls, '/') + 1))));
}

/**
 * Find and return a content-type header.
 */
const string contentType(const list<string>& h) {
    const maybe<string> ct = header("Content-Type: ", h);
    return hasContent(ct)? content(ct) : "";
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
        return mkfailure<value>(res);
    const list<string> ls(reverse(cadr(content(res))));

    // Return the content as a list of values
    const value val(mkvalues(ls));
    debug(val, "http::get::result");
    return val;
}

/**
 * Convert an HTTP content response to a value.
 */
const failable<value> responseValue(const list<list<string> > res) {

    // Parse the returned content
    const string ct = contentType(car(res));
    debug(ct, "http::responseValue::contentType");

    const list<string> ls(reverse(cadr(res)));
    debug(ls, "http::responseValue::content");

    if (atom::isATOMEntry(ls)) {
        // Read an ATOM entry
        const value val(elementsToValues(content(atom::readATOMEntry(ls))));
        debug(val, "http::responseValue::result");
        return val;
    }
    if (contains(ct, "application/atom+xml") || atom::isATOMFeed(ls)) {
        // Read an ATOM feed
        const value val(elementsToValues(content(atom::readATOMFeed(ls))));
        debug(val, "http::responseValue::result");
        return val;
    }
    if (contains(ct, "application/rss+xml") || rss::isRSSFeed(ls)) {
        // Read an RSS feed
        const value val(elementsToValues(content(rss::readRSSFeed(ls))));
        debug(val, "http::responseValue::result");
        return val;
    }
    if (contains(ct, "text/javascript") || contains(ct, "application/json") || json::isJSON(ls)) {
        // Read a JSON document
        js::JSContext cx;
        const value val(json::jsonValues(content(json::readJSON(ls, cx))));
        debug(val, "http::responseValue::result");
        return val;
    }
    if (contains(ct, "application/x-javascript")) {
        // Read a JSON document enclosed in a javascript function call
        // Extract the JSON out of the enclosing parenthesis
        ostringstream os;
        write(ls, os);
        const string s = str(os);
        const size_t fp = find(s, '(');
        const size_t lp = find_last(s, ')');
        const list<string> jls = mklist<string>(substr(s, fp + 1, lp - (fp + 1)));
        debug(jls, "http::responseValue::javascript::content");

        js::JSContext cx;
        const value val(json::jsonValues(content(json::readJSON(jls, cx))));
        debug(val, "http::responseValue::result");
        return val;
    }
    if (contains(ct, "text/xml") || contains(ct, "application/xml") || isXML(ls)) {
        // Read an XML document
        const value val(elementsToValues(readXML(ls)));
        debug(val, "http::responseValue::result");
        return val;
    }

    // Return the content type and a content list
    const value val(mklist<value>(ct, mkvalues(ls)));
    debug(val, "http::responseValue::result");
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
        return mkfailure<value>(res);

    // Parse the returned content
    return responseValue(content(res));
}

/**
 * Form an HTTP content request.
 */
const failable<list<list<string> > > writeRequest(const failable<list<string> >& ls, const string& ct) {
    if (!hasContent(ls))
        return mkfailure<list<list<string> > >(ls);
    const list<list<string> > req =  mklist<list<string> >(mklist<string>(string("Content-Type: ") + ct), content(ls));
    debug(req, "http::writeRequest::req");
    return req;
}

/**
 * Convert a value to an HTTP content request.
 */
const failable<list<list<string> > > contentRequest(const value& c, unused const string& url) {

    // Check if the client requested a specific format
    //TODO derive that from given URL
    const list<value> fmt = assoc<value>("format", list<value>());

    // Write as a scheme value if requested by the client
    if (!isNil(fmt) && cadr(fmt) == "scheme")
        return writeRequest(mklist<string>(scheme::writeValue(c)), "text/plain; charset=utf-8");

    // Write a simple value as a JSON value
    if (!isList(c)) {
        js::JSContext cx;
        if (isSymbol(c)) {
            const list<value> lc = mklist<value>(mklist<value>("name", value(string(c))));
            debug(lc, "http::contentRequest::symbol");
            return writeRequest(json::writeJSON(valuesToElements(lc), cx), "application/json; charset=utf-8");
        }
        const list<value> lc = mklist<value>(mklist<value>("value", c));
        debug(lc, "http::contentRequest::value");
        return writeRequest(json::writeJSON(valuesToElements(lc), cx), "application/json; charset=utf-8");
    }

    // Write an empty list as a JSON empty value
    if (isNil((list<value>)c)) {
        js::JSContext cx;
        debug(list<value>(), "http::contentRequest::empty");
        return writeRequest(json::writeJSON(list<value>(), cx), "application/json; charset=utf-8");
    }

    // Write content-type / content-list pair
    if (isString(car<value>(c)) && !isNil(cdr<value>(c)) && isList(cadr<value>(c)))
        return writeRequest(convertValues<string>(cadr<value>(c)), car<value>(c));

    // Write an assoc value as JSON
    if (isSymbol(car<value>(c)) && !isNil(cdr<value>(c))) {
        js::JSContext cx;
        const list<value> lc = mklist<value>(c);
        debug(lc, "http::contentRequest::assoc");
        debug(valuesToElements(lc), "http::contentRequest::assoc::element");
        return writeRequest(json::writeJSON(valuesToElements(lc), cx), "application/json; charset=utf-8");
    }

    // Write value as JSON if requested by the client
    if (!isNil(fmt) && cadr(fmt) == "json") {
        js::JSContext cx;
        return writeRequest(json::writeJSON(valuesToElements(c), cx), "application/json; charset=utf-8");
    }

    // Convert list of values to element values
    const list<value> e = valuesToElements(c);
    debug(e, "http::contentRequest::elements");

    // Write an ATOM feed or entry
    if (isList(car<value>(e)) && !isNil(car<value>(e))) {
        const list<value> el = car<value>(e);
        if (isSymbol(car<value>(el)) && car<value>(el) == element && !isNil(cdr<value>(el)) && isSymbol(cadr<value>(el)) && elementHasChildren(el) && !elementHasValue(el)) {
            if (cadr<value>(el) == atom::feed)
                return writeRequest(atom::writeATOMFeed(e), "application/atom+xml; charset=utf-8");
            if (cadr<value>(el) == atom::entry)
                return writeRequest(atom::writeATOMEntry(e), "application/atom+xml; charset=utf-8");
        }
    }

    // Write any other compound value as a JSON value
    js::JSContext cx;
    return writeRequest(json::writeJSON(e, cx), "application/json; charset=utf-8");
}

/**
 * HTTP POST.
 */
const failable<value> post(const value& val, const string& url, const CURLSession& cs) {
    debug(url, "http::post::url");

    // Convert value to a content request
    const failable<list<list<string> > > req = contentRequest(val, url);
    if (!hasContent(req))
        return mkfailure<value>(req);
    debug(content(req), "http::post::input");

    // POST it to the URL
    const failable<list<list<string> > > res = apply<list<string>>(content(req), rcons<string>, list<string>(), url, "POST", cs);
    if (!hasContent(res))
        return mkfailure<value>(res);

    // Return the new entry id from the HTTP location header, if any
    const string loc = location(car(content(res)));
    if (length(loc) != 0) {
        const value eid(entryId(location(car(content(res)))));
        debug(eid, "http::post::result");
        return eid;
    }

    // Return the returned content
    return responseValue(content(res));
}

/**
 * HTTP PUT.
 */
const failable<value> put(const value& val, const string& url, const CURLSession& cs) {
    debug(url, "http::put::url");

    // Convert value to a content request
    const failable<list<list<string> > > req = contentRequest(val, url);
    if (!hasContent(req))
        return mkfailure<value>(req);
    debug(content(req), "http::put::input");

    // PUT it to the URL
    const failable<list<list<string> > > res = apply<list<string> >(content(req), rcons<string>, list<string>(), url, "PUT", cs);
    if (!hasContent(res))
        return mkfailure<value>(res);

    debug(true, "http::put::result");
    return value(true);
}

/**
 * HTTP PATCH.
 */
const failable<value> patch(const value& val, const string& url, const CURLSession& cs) {
    debug(url, "http::put::patch");

    // Convert value to a content request
    const failable<list<list<string> > > req = contentRequest(val, url);
    if (!hasContent(req))
        return mkfailure<value>(req);
    debug(content(req), "http::patch::input");

    // PATCH it to the URL
    const failable<list<list<string> > > res = apply<list<string> >(content(req), rcons<string>, list<string>(), url, "PATCH", cs);
    if (!hasContent(res))
        return mkfailure<value>(res);

    debug(true, "http::patch::result");
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
        return mkfailure<value>(res);

    debug(true, "http::delete::result");
    return value(true);
}

/**
 * Returns the current host name.
 */
const string hostName() {
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
    debug(url, "http::connect::url");

    // Setup the CURL session
    const failable<CURL*> fch = setup(url, cs);
    if (!hasContent(fch))
        return mkfailure<bool>(fch);
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
const failable<bool> send(const char* c, const size_t l, const CURLSession& cs) {

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
    return send(c + wl, l - wl, cs);
}

/**
 * Receive an array of chars.
 */
const failable<size_t> recv(char* c, const size_t l, const CURLSession& cs) {

    // Receive data
    size_t rl;
    const CURLcode rc = curl_easy_recv(cs.h, c, (size_t)l, &rl);
    if (rc == CURLE_OK)
        return (size_t)rl;
    if (rc == 1)
        return 0;
    if (rc != CURLE_AGAIN)
        return mkfailure<size_t>(curlreason(rc));

    // If the socket was not ready, wait for it to become ready
    const apr_pollfd_t* pollfds;
    apr_int32_t pollcount;
    apr_status_t pollrc = apr_pollset_poll(cs.rpollset, -1, &pollcount, &pollfds);
    if (pollrc != APR_SUCCESS)
        return mkfailure<size_t>(apreason(pollrc));

    // Receive again
    return recv(c, l, cs);
}

/**
 * Converts a list of key value pairs to a query string.
 */
ostringstream& queryString(const list<list<value> > args, ostringstream& os) {
    if (isNil(args))
        return os;
    const list<value> arg = car(args);
    debug(arg, "http::queryString::arg");
    if (isNil(arg) || isNil(cdr(arg)))
        return queryString(cdr(args), os);
    os << car(arg) << "=" << c_str(cadr(arg));
    if (!isNil(cdr(args)))
        os << "&";
    return queryString(cdr(args), os);
}

const string queryString(const list<list<value> > args) {
    ostringstream os;
    return str(queryString(args, os));
}

/**
 * Filter path segment in a list of arguments.
 */
const bool filterPath(const value& arg) {
    return isString(arg) || isSymbol(arg);
}

/**
 * Filter query string arguments in a list of arguments.
 */
const bool filterQuery(const value& arg) {
    return isList(arg);
}

/**
 * Escape a query string argument.
 */
const value escapeQuery(const value& arg) {
    return arg;
    //return mklist<value>(car<value>(arg), escapeArg(cadr<value>(arg)));
}

/**
 * HTTP client proxy function.
 */
struct proxy {
    proxy(const string& uri, const string& ca, const string& cert, const string& key, const string& cookie, const gc_pool& p) : p(p), uri(uri), ca(ca), cert(cert), key(key), cookie(cookie), cs(*(new (gc_new<CURLSession>(p)) CURLSession(ca, cert, key, cookie))) {
    }
    
    const value operator()(const list<value>& args) const {
        debug(args, "http::proxy::args");
        const value fun = car(args);
        if (fun == "get") {
            const list<value> lp = filter<value>(filterPath, cadr(args));
            debug(lp, "http::proxy::path");
            const list<value> lq = map<value, value>(escapeQuery, filter<value>(filterQuery, cadr(args)));
            debug(lp, "http::proxy::query");
            const value p = path(lp);
            const value q = queryString(lq);
            const failable<value> val = get(uri + p + (q != ""? string("?") + q : string("")), cs);
            return content(val);
        }
        if (fun == "post") {
            const failable<value> val = post(caddr(args), uri + path(cadr(args)), cs);
            return content(val);
        }
        if (fun == "put") {
            const failable<value> val = put(caddr(args), uri + path(cadr(args)), cs);
            return content(val);
        }
        if (fun == "patch") {
            const failable<value> val = patch(caddr(args), uri + path(cadr(args)), cs);
            return content(val);
        }
        if (fun == "delete") {
            const failable<value> val = del(uri + path(cadr(args)), cs);
            return content(val);
        }
        const failable<value> val = evalExpr(args, uri, cs);
        return content(val);
    }

    const gc_pool p;
    const string uri;
    const string ca;
    const string cert;
    const string key;
    const string cookie;
    const CURLSession& cs;
};

}
}

#endif /* tuscany_http_hpp */
