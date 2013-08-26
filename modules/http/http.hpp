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
    CURLSession() : p(), h(*(new (gc_new<handles>()) handles())), owner(false), ca(emptyString), cert(emptyString), key(emptyString), cookie(emptyString), timeout(0) {
    }

    CURLSession(const string& ca, const string& cert, const string& key, const string& cookie, const int timeout) : p(), h(*(new (gc_new<handles>()) handles())), owner(true), ca(ca), cert(cert), key(key), cookie(cookie), timeout(timeout) {
    }

    CURLSession(const CURLSession& c) : p(c.p), h(c.h), owner(false), ca(c.ca), cert(c.cert), key(c.key), cookie(c.cookie), timeout(c.timeout) {
    }

    CURLSession& operator=(const CURLSession& c) = delete;

    ~CURLSession() {
        if (!owner)
            return;
        if (h.h == NULL)
            return;
        curl_easy_cleanup(h.h);
    }

private:
    class handles {
    public:
        handles() : h(NULL), sock(NULL), wpollset(NULL), wpollfd(NULL), rpollset(NULL), rpollfd(NULL) {
        }

        handles(const handles& c) : h(c.h), sock(c.sock), wpollset(c.wpollset), wpollfd(c.wpollfd), rpollset(c.rpollset), rpollfd(c.rpollfd) {
        }

    private:
        CURL* h;
        apr_socket_t* sock;
        apr_pollset_t* wpollset;
        apr_pollfd_t* wpollfd;
        apr_pollset_t* rpollset;
        apr_pollfd_t* rpollfd;

        friend class CURLSession;
        friend CURL* const handle(const CURLSession& cs);
        friend apr_socket_t* const sock(const CURLSession& cs);
        friend const failable<CURL*> setup(const string& url, const CURLSession& cs);
        friend const failable<bool> cleanup(const CURLSession& cs);
        friend const failable<bool> connect(const string& url, const CURLSession& cs);
        friend const failable<bool> send(const char* const c, const size_t l, const CURLSession& cs);
        friend const failable<size_t> recv(char* const c, const size_t l, const CURLSession& cs);
    };

    const gc_child_pool p;
    handles& h;
    const bool owner;

    friend CURL* const handle(const CURLSession& cs);
    friend apr_socket_t* const sock(const CURLSession& cs);
    friend const failable<CURL*> setup(const string& url, const CURLSession& cs);
    friend const failable<bool> cleanup(const CURLSession& cs);
    friend const failable<bool> connect(const string& url, const CURLSession& cs);
    friend const failable<bool> send(const char* const c, const size_t l, const CURLSession& cs);
    friend const failable<size_t> recv(char* const c, const size_t l, const CURLSession& cs);

public:
    const string ca;
    const string cert;
    const string key;
    const string cookie;
    const int timeout;
};

/**
 * Returns the CURL handle used by a CURL session.
 */
CURL* const handle(const CURLSession& cs) {
    return cs.h.h;
}

/**
 * Return an apr_socket_t for the socket used by a CURL session.
 */
apr_socket_t* const sock(const CURLSession& cs) {
    return cs.h.sock;
}

/**
 * Convert a socket fd to an apr_socket_t.
 */
apr_socket_t* const sock(const int sd, const gc_pool& p) {
    int fd = sd;
    apr_socket_t* s = NULL;
    apr_os_sock_put(&s, &fd, pool(p));
    return s;
}

/**
 * Convert a CURL return code to an error string.
 */
const string curlreason(const CURLcode rc) {
    return curl_easy_strerror(rc);
}

/**
 * Convert an APR status to an error string.
 */
const string apreason(const apr_status_t rc) {
    char buf[256];
    return apr_strerror(rc, buf, sizeof(buf));
}

/**
 * Escape a URI or a query argument.
 */
const char escape_c2x[] = "0123456789ABCDEF";

const string escape(const string& unesc, const char* reserv) {
    char* const copy = (char*)apr_palloc(gc_current_pool(), 3 * length(unesc) + 3);
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
        return emptyString;
    if (u.hostname == NULL)
        return emptyString;
    return u.hostname;
}

/**
 * Parse a URI and return its scheme.
 */
const string scheme(const string& uri, const gc_pool& p) {
    apr_uri_t u;
    const apr_status_t rc = apr_uri_parse(pool(p), c_str(uri), &u);
    if (rc != APR_SUCCESS)
        return emptyString;
    if (u.scheme == NULL)
        return emptyString;
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
    if (cs.h.h != NULL)
        cleanup(cs);
    cs.h.h = curl_easy_init();
    debug(cs.h.h, "http::setup::init::h");
    CURL* const ch = cs.h.h;
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl/1.0");
#ifdef WANT_MAINTAINER_CURL_VERBOSE
    curl_easy_setopt(ch, CURLOPT_VERBOSE, true);
#endif

    // Setup protocol options
    curl_easy_setopt(ch, CURLOPT_TCP_NODELAY, true);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(ch, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
    curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(ch, CURLOPT_DNS_USE_GLOBAL_CACHE, 0);
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, cs.timeout);

    // Setup SSL options
    if (cs.ca != emptyString) {
        debug(cs.ca, "http::setup::ca");
        curl_easy_setopt(ch, CURLOPT_CAINFO, c_str(cs.ca));
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 2);
    } else
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, false);
    if (cs.cert != emptyString) {
        debug(cs.cert, "http::setup::cert");
        curl_easy_setopt(ch, CURLOPT_SSLCERT, c_str(cs.cert));
        curl_easy_setopt(ch, CURLOPT_SSLCERTTYPE, "PEM");
    }
    if (cs.key != emptyString) {
        debug(cs.key, "http::setup::key");
        curl_easy_setopt(ch, CURLOPT_SSLKEY, c_str(cs.key));
        curl_easy_setopt(ch, CURLOPT_SSLKEYTYPE, "PEM");
    }
    if (cs.cookie != emptyString) {
        debug(cs.cookie, "http::setup::cookie");
        curl_easy_setopt(ch, CURLOPT_COOKIE, c_str(cs.cookie));
    }

    // Set up HTTP basic auth if requested
    apr_uri_t u;
    apr_pool_t* const p = gc_current_pool();
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
 * Cleanup a CURL session
 */
const failable<bool> cleanup(const CURLSession& cs) {
    if (cs.h.h == NULL)
        return true;
    debug(cs.h.h, "http::cleanup::cleanup::h");
    curl_easy_cleanup(cs.h.h);
    cs.h.h = NULL;
    return true;
}

/**
 * Context passed to the read callback function.
 */
class CURLReadContext {
public:
    CURLReadContext(const list<string>& ilist) : ilist(ilist) {
    }

    gc_mutable_ref<list<string> > ilist;
};

/**
 * Called by CURL to read data to send.
 */
size_t readCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    CURLReadContext& rcx = *(CURLReadContext*)data;
    if (isNull((const list<string>)rcx.ilist))
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
    CURLWriteContext(const lambda<const R(const string&, const R)>& reduce, const R& accum) : reduce(reduce), accum(accum) {
    }

    const lambda<const R(const string&, const R)> reduce;
    gc_mutable_ref<R> accum;
};

/**
 * Called by CURL to write received data.
 */
template<typename R> size_t writeCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    CURLWriteContext<R>& wcx = *(CURLWriteContext<R>*)data;
    const size_t realsize = size * nmemb;
    wcx.accum = wcx.reduce(string((const char*)ptr, realsize), wcx.accum);
    return realsize;
}

/**
 * Apply an HTTP verb to a list containing a list of headers and a list of content, and
 * a reduce function used to process the response.
 */
curl_slist* headers(curl_slist* const cl, const list<string>& h) {
    if (isNull(h))
        return cl;
    return headers(curl_slist_append(cl, c_str(string(car(h)))), cdr(h));
}

template<typename R> const failable<list<R> > apply(const list<list<string> >& hdr, const lambda<const R(const string&, const R)>& reduce, const R& initial, const string& url, const string& verb, const CURLSession& cs) {
    debug(url, "http::apply::url");
    debug(verb, "http::apply::verb");

    // Setup the CURL session
    const failable<CURL*> fch = setup(url, cs);
    if (!hasContent(fch)) {
        cleanup(cs);
        return mkfailure<list<R>>(fch);
    }
    CURL* const ch = content(fch);

    // Set the request headers
    curl_slist* hl = headers(NULL, car(hdr));
    hl = curl_slist_append(hl, "X-Accept: text/x-scheme; charset=utf-8");
    hl = curl_slist_append(hl, "X-Cache-Control: no-cache");
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
    if (rc) {
        cleanup(cs);
        return mkfailure<list<R> >(string(curl_easy_strerror(rc)));
    }
    long httprc;
    curl_easy_getinfo (ch, CURLINFO_RESPONSE_CODE, &httprc);
    if (httprc != 200 && httprc != 201) {
        cleanup(cs);
        return mkfailure<list<R> >(string("HTTP code not 200"), (int)httprc, (httprc != 301 && httprc != 302 && httprc != 404));
    }

    cleanup(cs);
    return mklist<R>(hcx.accum, wcx.accum);
}

/**
 * Evaluate an expression remotely, at the given URL.
 */
const failable<value> evalExpr(const value& expr, const string& url, const CURLSession& cs) {
    debug(url, "http::evalExpr::url");
    debug(expr, "http::evalExpr::input");

    // Convert expression to a JSON-RPC request
    const failable<list<string> > jsreq = json::jsonRequest(1, car<value>(expr), cdr<value>(expr));
    if (!hasContent(jsreq))
        return mkfailure<value>(jsreq);

    // POST it to the URL
    const list<string> h = mklist<string>("Content-Type: application/json-rpc");
    const failable<list<list<string> > > res = apply<list<string> >(mklist<list<string> >(h, content(jsreq)), rcons<string>, list<string>(), url, "POST", cs);
    if (!hasContent(res))
        return mkfailure<value>(res);

    // Parse and return JSON-RPC result
    const failable<value> rval = json::jsonResultValue(cadr<list<string> >(content(res)));
    debug(rval, "http::evalExpr::result");
    if (!hasContent(rval))
        return mkfailure<value>(rval);
    return content(rval);
}

/**
 * Find and return a header.
 */
const maybe<string> header(const char* const prefix, const list<string>& h) {
    if (isNull(h))
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
    return hasContent(l)? content(l) : emptyString;
}

/**
 * Convert a location to an entry id.
 */
const value entryId(const failable<string> l) {
    if (!hasContent(l))
        return nilListValue;
    const string ls(content(l));
    return value(mklist<value>(string(substr(ls, find_last(ls, '/') + 1))));
}

/**
 * Find and return a content-type header.
 */
const string contentType(const list<string>& h) {
    const maybe<string> ct = header("Content-Type: ", h);
    return hasContent(ct)? content(ct) : emptyString;
}

/**
 * HTTP GET, return the resource at the given URL.
 */
template<typename R> const failable<list<R> > get(const lambda<const R(const string&, const R)>& reduce, const R& initial, const string& url, const CURLSession& cs) {
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

    if (contains(ct, "text/x-scheme")) {
        // Read a Scheme value
        ostringstream os;
        write(ls, os);
        istringstream is(str(os));
        const value val(content(scheme::readValue(is)));
        debug(val, "http::responseValue::result");
        return val;
    }
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
        const value val(content(json::readValue(ls)));
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

        const value val(content(json::readValue(jls)));
        debug(val, "http::responseValue::result");
        return val;
    }
    if (contains(ct, "text/xml") || contains(ct, "application/xml") || xml::isXML(ls)) {
        // Read an XML document
        const value val(elementsToValues(content(xml::readElements(ls))));
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

    // Write in the format requested by the client, if any
    const list<value> fmt = assoc<value>("format", nilListValue);
    if (!isNull(fmt)) {
        if (cadr(fmt) == "scheme")
            return writeRequest(scheme::writeValue(c), "text/x-scheme; charset=utf-8");
        if (cadr(fmt) == "json")
            return writeRequest(json::writeValue(c), "application/json; charset=utf-8");
        if (cadr(fmt) == "xml")
            return writeRequest(xml::writeElements(valuesToElements(c)), "text/xml; charset=utf-8");
    }

    // Write a simple value as a JSON value
    if (!isList(c)) {
        debug(c, "http::contentRequest::value");
        return writeRequest(json::writeValue(c), "application/json; charset=utf-8");
    }

    // Write an empty list as a JSON value
    if (isNull((list<value>)c)) {
        debug(nilListValue, "http::contentRequest::empty");
        return writeRequest(json::writeValue(c), "application/json; charset=utf-8");
    }

    // Write content-type / content-list pair
    if (isString(car<value>(c)) && !isNull(cdr<value>(c)) && isList(cadr<value>(c)))
        return writeRequest(convertValues<string>(cadr<value>(c)), car<value>(c));

    // Write an assoc value as a JSON value
    if (isSymbol(car<value>(c)) && !isNull(cdr<value>(c))) {
        debug(c, "http::contentRequest::assoc");
        return writeRequest(json::writeValue(c), "application/json; charset=utf-8");
    }

    // Write an ATOM feed or entry
    const list<value> e = valuesToElements(c);
    if (isList(car<value>(e)) && !isNull(car<value>(e))) {
        const list<value> el = car<value>(e);
        if (isSymbol(car<value>(el)) && car<value>(el) == element && !isNull(cdr<value>(el)) && isSymbol(cadr<value>(el)) && elementHasChildren(el) && !elementHasValue(el)) {
            if (cadr<value>(el) == atom::feed)
                return writeRequest(atom::writeATOMFeed(e), "application/atom+xml; charset=utf-8");
            if (cadr<value>(el) == atom::entry)
                return writeRequest(atom::writeATOMEntry(e), "application/atom+xml; charset=utf-8");
        }
    }

    // Write any other compound value as a JSON value
    return writeRequest(json::writeValue(c), "application/json; charset=utf-8");
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
    return trueValue;
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
    return trueValue;
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
    return trueValue;
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
apr_pollfd_t* const pollfd(apr_socket_t* const s, const int e, const gc_pool& p) {
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
const failable<bool> connect(const string& url, const CURLSession& cs) {
    debug(url, "http::connect::url");

    // Setup the CURL session
    const failable<CURL*> fch = setup(url, cs);
    if (!hasContent(fch)) {
        cleanup(cs);
        return mkfailure<bool>(fch);
    }
    CURL* const ch = content(fch);

    // Connect
    curl_easy_setopt(ch, CURLOPT_CONNECT_ONLY, true);
    const CURLcode rc = curl_easy_perform(ch);
    if (rc) {
        cleanup(cs);
        return mkfailure<bool>(string(curl_easy_strerror(rc)));
    }

    // Convert the connected socket to an apr_socket_t
    int sd;
    const CURLcode grc = curl_easy_getinfo(ch, CURLINFO_LASTSOCKET, &sd);
    if (grc) {
        cleanup(cs);
        return mkfailure<bool>(string(curl_easy_strerror(grc)));
    }
    cs.h.sock = sock(sd, cs.p);

    // Create pollsets and pollfds which can be used to poll the socket
    const apr_status_t rpcrc = apr_pollset_create(&cs.h.rpollset, 1, pool(cs.p), 0);
    if (rpcrc != APR_SUCCESS) {
        cleanup(cs);
        return mkfailure<bool>(apreason(rpcrc));
    }
    cs.h.rpollfd = pollfd(cs.h.sock, APR_POLLIN, cs.p);
    apr_pollset_add(cs.h.rpollset, cs.h.rpollfd);
    const apr_status_t wpcrc = apr_pollset_create(&cs.h.wpollset, 1, pool(cs.p), 0);
    if (wpcrc != APR_SUCCESS) {
        cleanup(cs);
        return mkfailure<bool>(apreason(wpcrc));
    }
    cs.h.wpollfd = pollfd(cs.h.sock, APR_POLLOUT, cs.p);
    apr_pollset_add(cs.h.wpollset, cs.h.wpollfd);

    return true;
}

/**
 * Send an array of chars.
 */
const failable<bool> send(const char* const c, const size_t l, const CURLSession& cs) {

    // Send the data
    size_t wl = 0;
    const CURLcode rc = curl_easy_send(cs.h.h, c, (size_t)l, &wl);
    if (rc == CURLE_OK && wl == (size_t)l)
        return true;
    if (rc != CURLE_AGAIN) {
        cleanup(cs);
        return mkfailure<bool>(curlreason(rc));
    }

    // If the socket was not ready, wait for it to become ready
    const apr_pollfd_t* pollfds;
    apr_int32_t pollcount;
    const apr_status_t pollrc = apr_pollset_poll(cs.h.wpollset, -1, &pollcount, &pollfds);
    if (pollrc != APR_SUCCESS)
        return mkfailure<bool>(apreason(pollrc));

    // Send what's left
    return send(c + wl, l - wl, cs);
}

/**
 * Receive an array of chars.
 */
const failable<size_t> recv(char* const c, const size_t l, const CURLSession& cs) {

    // Receive data
    size_t rl;
    const CURLcode rc = curl_easy_recv(cs.h.h, c, (size_t)l, &rl);
    if (rc == CURLE_OK)
        return (size_t)rl;
    if (rc == 1)
        return 0;
    if (rc != CURLE_AGAIN) {
        cleanup(cs);
        return mkfailure<size_t>(curlreason(rc));
    }

    // If the socket was not ready, wait for it to become ready
    const apr_pollfd_t* pollfds;
    apr_int32_t pollcount;
    const apr_status_t pollrc = apr_pollset_poll(cs.h.rpollset, -1, &pollcount, &pollfds);
    if (pollrc != APR_SUCCESS) {
        cleanup(cs);
        return mkfailure<size_t>(apreason(pollrc));
    }

    // Receive again
    return recv(c, l, cs);
}

/**
 * Converts a list of key value pairs to a query string.
 */
ostringstream& queryString(const list<value>& args, ostringstream& os) {
    if (isNull(args))
        return os;
    const list<value> arg = car(args);
    debug(arg, "http::queryString::arg");
    if (isNull(arg) || isNull(cdr(arg)))
        return queryString(cdr(args), os);
    os << car(arg) << "=" << c_str(cadr(arg));
    if (!isNull(cdr(args)))
        os << "&";
    return queryString(cdr(args), os);
}

const string queryString(const list<value>& args) {
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
class proxy {
public:
    proxy(const string& uri, const string& ca, const string& cert, const string& key, const string& cookie, const int timeout) : uri(uri), cs(mksession(ca, cert, key, cookie, timeout)) {
    }
    
    const value operator()(const list<value>& args) const {
        debug(args, "http::proxy::args");
        const value fun = car(args);
        if (fun == "get") {
            const list<value> lp = filter<value>(filterPath, cadr(args));
            debug(lp, "http::proxy::path");
            const list<value> lq = map<value, value>(escapeQuery, filter<value>(filterQuery, cadr(args)));
            debug(lq, "http::proxy::query");
            const value q = queryString(lq);
            const failable<value> val = get(uri + (string)path(lp) + (q != emptyString? string("?") + (string)q : emptyString), *cs);
            return content(val);
        }
        if (fun == "post") {
            const failable<value> val = post(caddr(args), uri + (string)path(cadr(args)), *cs);
            return content(val);
        }
        if (fun == "put") {
            const failable<value> val = put(caddr(args), uri + (string)path(cadr(args)), *cs);
            return content(val);
        }
        if (fun == "patch") {
            const failable<value> val = patch(caddr(args), uri + (string)path(cadr(args)), *cs);
            return content(val);
        }
        if (fun == "delete") {
            const failable<value> val = del(uri + (string)path(cadr(args)), *cs);
            return content(val);
        }
        const failable<value> val = evalExpr(args, uri, *cs);
        return content(val);
    }

private:
    const string uri;
    const perthread_ptr<http::CURLSession> cs;

    
    const perthread_ptr<http::CURLSession> mksession(const string& ca, const string& cert, const string& key, const string& cookie, const int timeout) {
        const gc_pool cp = gc_current_pool();
        const lambda<const gc_ptr<http::CURLSession>()> newsession = [ca, cert, key, cookie, timeout, cp]() -> const gc_ptr<http::CURLSession> {
            const gc_scoped_pool sp(pool(cp));
            return new (gc_new<http::CURLSession>()) http::CURLSession(ca, cert, key, cookie, timeout);
        };
        return *(new (gc_new<perthread_ptr<http::CURLSession> >()) perthread_ptr<CURLSession>(newsession));
    }
};

}
}

#endif /* tuscany_http_hpp */
