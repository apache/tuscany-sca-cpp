/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more provider license agreements.  See the NOTICE file
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

#ifndef tuscany_modeval_hpp
#define tuscany_modeval_hpp

/**
 * HTTPD module used to eval component implementations.
 */

#include <sys/stat.h>

#include "string.hpp"
#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "../scheme/io.hpp"
#include "../atom/atom.hpp"
#include "../json/json.hpp"
#include "../scdl/scdl.hpp"
#include "../http/http.hpp"
#include "../http/httpd.hpp"

#include "apr_md5.h"
#include "ap_provider.h"
#include "mod_auth.h"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_eval;
}

namespace tuscany {
namespace server {
namespace modeval {

/**
 * SSL certificate configuration.
 */
class SSLConf {
public:
    SSLConf() {
    }

    gc_mutable_ref<string> ca;
    gc_mutable_ref<string> cert;
    gc_mutable_ref<string> key;
};

/**
 * Virtual host configuration.
 */
class VhostConf {
public:
    VhostConf() {
    }

    gc_mutable_ref<string> domain;
    gc_mutable_ref<string> contribPath;
    gc_mutable_ref<string> composName;
    gc_mutable_ref<string> contributorName;
    gc_mutable_ref<value> contributor;
    gc_mutable_ref<string> authenticatorName;
    gc_mutable_ref<value> authenticator;
};

/**
 * Contribution configuration.
 */
class ContribConf {
public:
    ContribConf() {
    }

    gc_mutable_ref<string> contribPath;
    gc_mutable_ref<string> composName;
};

/**
 * Composite assocs.
 */
class Composite {
public:
    Composite() {
    }

    Composite(const list<value>& refs, const list<value>& svcs, const list<value>& impls) : refs(refs), svcs(svcs), impls(impls) {
    }

    gc_mutable_ref<list<value> > refs;
    gc_mutable_ref<list<value> > svcs;
    gc_mutable_ref<list<value> > impls;
};

/**
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf() {
    }

    ServerConf(apr_pool_t* const p, const server_rec* s) : p(p), server(s), timeout(0) {
    }

    const gc_pool p;
    const server_rec* server;
    gc_mutable_ref<value> lifecycle;
    ContribConf contribc;
    SSLConf sslc;
    int timeout;
    VhostConf vhostc;
    Composite compos;
};

/**
 * Request configuration.
 */
class RequestConf {
public:
    RequestConf(apr_pool_t* const p, const request_rec* r) : p(p), request(r), vhost(false), valias(false) {
    }

    const gc_pool p;
    const request_rec* request;
    bool vhost;
    bool valias;
    gc_mutable_ref<list<value> > rpath;
    gc_mutable_ref<list<value> > vpath;
    gc_mutable_ref<list<value> > impls;
};

/**
 * Authentication cache store function.
 */
static APR_OPTIONAL_FN_TYPE(ap_authn_cache_store) *authnCacheStore = NULL;

/**
 * Convert a result represented as a (content reason? code?) tuple to a
 * failable monad.
 */
const failable<value> failableResult(const list<value>& v) {
    if (isNull(cdr(v)))
        return car(v);
    return mkfailure<value>(string(cadr(v)), isNull(cddr(v))? -1 : (int)caddr(v), false);
}

/**
 * Store current HTTP request for access from property and proxy lambda functions.
 */
#ifdef WANT_THREADS
const perthread_ptr<request_rec> currentRequest;
#else
request_rec* currentRequest = NULL;
#endif

class ScopedRequest {
public:
    ScopedRequest(request_rec* const r) {
        currentRequest = r;
    }

    ~ScopedRequest() {
        currentRequest = NULL;
    }
};

/**
 * Make an HTTP proxy lambda to an absolute URI
 */
const value mkhttpProxy(const string& uri, const int timeout) {
    debug(uri, "modeval::mkhttpProxy::uri");
    return lvvlambda(http::proxy(uri, emptyString, emptyString, emptyString, emptyString, timeout));
}

/**
 * Return a component implementation proxy lambda.
 */
class implProxy {
public:
    implProxy(const value& name, const gc_mutable_ref<list<value> >& impls, const SSLConf& sslc, const int timeout) : name(name), impls(impls), sslc(sslc), timeout(timeout) {
    }

    const value callImpl(const value& cname, const list<value>& aparams) const {
        debug(impls, "modeval::implProxy::callImpl::impls");

        // Lookup the component implementation
        const list<value> impl(rbtreeAssoc<value>(cname, (list<value>)impls));
        if (isNull(impl))
            return mkfailure<value>(string("Couldn't find component implementation: ") + (string)cname);

        // Call its lambda function
        const lvvlambda l(cadr<value>(impl));
        const value func = c_str(car(aparams));
        const failable<value> val = failableResult(l(cons(func, cdr(aparams))));
        debug(val, "modeval::implProxy::result");
        if (!hasContent(val))
            return nilValue;
        return content(val);
    }

    const value operator()(const list<value>& params) const {
        debug(name, "modeval::implProxy::name");
        debug(params, "modeval::implProxy::input");

        // If the reference was 'wiredByImpl' use the first param as target
        if (isNull(name)) {
            const value uri = cadr(params);
            const list<value> aparams = cons<value>(car(params), cddr(params));
            debug(uri, "modeval::implProxy::wiredByImpl::uri");
            debug(aparams, "modeval::implProxy::wiredByImpl::input");

            // Use an HTTP proxy if the target is an absolute :// target
            if (http::isAbsolute(uri)) {
                const gc_pool p(currentRequest->pool);

                // Interpret a uri in the form app://appname, convert it using the scheme,
                // top level domain and port number from the current request
                if (http::scheme(uri, p) == "app") {
                    ostringstream appuri;
                    appuri << httpd::scheme(currentRequest) << "://" << substr(uri, 6) << "." << http::topDomain(httpd::hostName(currentRequest)) << ":" << httpd::port(currentRequest) << "/";
                    debug(str(appuri), "modeval::implProxy::httpproxy::appuri");
                    const lvvlambda px = lvvlambda(http::proxy(str(appuri), sslc.ca, sslc.cert, sslc.key, httpd::cookie(currentRequest), timeout));
                    return px(aparams);
                }
                
                // Pass our SSL certificate and the cookie from the current request
                // only if the target is in the same top level domain
                if (http::topDomain(http::hostName(uri, p)) == http::topDomain(httpd::hostName(currentRequest))) {
                    debug(uri, "modeval::implProxy::httpproxy::samedomain");
                    const lvvlambda px = lvvlambda(http::proxy(uri, sslc.ca, sslc.cert, sslc.key, httpd::cookie(currentRequest), timeout));
                    return px(aparams);
                }

                // No SSL certificate or cookie on a cross domain call
                debug(uri, "modeval::implProxy::httpproxy::crossdomain");
                const lvvlambda px = lvvlambda(http::proxy(uri, emptyString, emptyString, emptyString, emptyString, timeout));
                return px(aparams);
            }

            // Call the component implementation
            return callImpl(uri, aparams);
        }

        // Call the component implementation
        return callImpl(name, params);
    }

private:
    const value name;
    const gc_mutable_ref<list<value> >& impls;
    const SSLConf& sslc;
    const int timeout;
};

const value mkimplProxy(const value& name, const gc_mutable_ref<list<value> >& impls, const SSLConf& sslc, const int timeout) {
    debug(name, "modeval::implProxy::impl");
    return lvvlambda(implProxy(name, impls, sslc, timeout));
}

/**
 * Return a proxy lambda for an unwired reference.
 */
const value mkunwiredProxy(const string& name) {
    debug(name, "modeval::mkunwiredProxy::name");
    const lvvlambda unwiredProxy = [name](const list<value>& params) -> const value {
        debug(name, "modeval::unwiredProxy::name");
        debug(params, "modeval::unwiredProxy::params");

        // Get function returns a default empty value
        if (car(params) == "get") {
            debug(nilValue, "modeval::unwiredProxy::result");
            return nilValue;
        }

        // All other functions return a failure
        return mkfailure<value>(string("Reference is not wired: ") + name);
    };
    return unwiredProxy;
}

/**
 * Convert a list of component references to a list of proxy lambdas.
 */
const value mkrefProxy(const value& ref, const gc_mutable_ref<list<value> >& impls, const SSLConf& sslc, const int timeout) {
    const value target = scdl::target(ref);
    const bool wbyimpl = scdl::wiredByImpl(ref);
    debug(ref, "modeval::mkrefProxy::ref");
    debug(target, "modeval::mkrefProxy::target");
    debug(wbyimpl, "modeval::mkrefProxy::wiredByImpl");

    // Use an HTTP proxy or an internal proxy to the component implementation
    if (wbyimpl)
        return mkimplProxy(nilValue, impls, sslc, timeout);
    if (isNull(target))
        return mkunwiredProxy(scdl::name(ref));
    if (http::isAbsolute(target))
        return mkhttpProxy(target, timeout);
    return mkimplProxy(car(pathValues(target)), impls, sslc, timeout);
}

const list<value> refProxies(const list<value>& refs, const gc_mutable_ref<list<value> >& impls, const SSLConf& sslc, const int timeout) {
    if (isNull(refs))
        return refs;
    return cons(mkrefProxy(car(refs), impls, sslc, timeout), refProxies(cdr(refs), impls, sslc, timeout));
}

/**
 * Convert a list of component properties to a list of lambda functions that just return
 * the property value. The host, user and email properties are configured with the values
 * from the HTTP request, if any.
 */
const lvvlambda mkvaluePropProxy(const value& v) {
    return [v](unused const list<value>& params) -> const value {
        return v;
    };
}

const lvvlambda mkhostPropProxy(const value& v) {
    return [v](unused const list<value>& params) -> const value {
        if (currentRequest == NULL)
            return http::hostName();
        const value h = httpd::hostName(currentRequest, v);
        debug(h, "modeval::hostPropProxy::value");
        return h;
    };
}

const lvvlambda mkappPropProxy(const value& v) {
    return [v](unused const list<value>& params) -> const value {
        if (currentRequest == NULL)
            return v;
        const RequestConf& reqc = httpd::requestConf<RequestConf>(currentRequest, &mod_tuscany_eval);
        const value a = isNull((const list<value>)reqc.vpath)? v : car((const list<value>)reqc.vpath);
        debug(a, "modeval::appPropProxy::value");
        return a;
    };
}

const lvvlambda mkpathPropProxy(const value& v) {
    return [v](unused const list<value>& params) -> const value {
        if (currentRequest == NULL)
            return v;
        const RequestConf& reqc = httpd::requestConf<RequestConf>(currentRequest, &mod_tuscany_eval);
        const value p = (const list<value>)reqc.rpath; 
        debug(p, "modeval::pathPropProxy::value");
        return p;
    };
}

const lvvlambda mkqueryPropProxy(const value& v) {
    return [v](unused const list<value>& params) -> const value {
        if (currentRequest == NULL)
            return v;
        const value q = httpd::unescapeArgs(httpd::queryArgs(currentRequest));
        debug(q, "modeval::queryPropProxy::value");
        return q;
    };
}

const lvvlambda mkenvPropProxy(const string& name, const value& v) {
    return [name, v](unused const list<value>& params) -> const value {
        if (currentRequest == NULL)
            return v;
        const char* env = apr_table_get(currentRequest->subprocess_env, c_str(name));
        if (env == NULL || *env == '\0')
            return v;
        debug(name, "modeval::envPropProxy::name");
        const value e = string(env);
        debug(e, "modeval::envPropProxy::value");
        return e;
    };
}

const lvvlambda mkrealmPropProxy(const value& v) {
    return [v](unused const list<value>& params) -> const value {
        if (currentRequest == NULL)
            return v;
        const char* env = apr_table_get(currentRequest->subprocess_env, "REALM");
        if (env == NULL)
            return v;
        const string realm = httpd::realm(string(env));
        if (length(realm) == 0)
            return v;
        const value r = realm;
        debug(r, "modeval::realmPropProxy::value");
        return r;
    };
}

const lvvlambda mktimeoutPropProxy(const value& v) {
    return [v](unused const list<value>& params) -> const value {
        if (currentRequest == NULL)
            return v;
        const ServerConf& sc = httpd::serverConf<ServerConf>(currentRequest, &mod_tuscany_eval);
        const value r = sc.timeout;
        debug(r, "modeval::timeoutPropProxy::value");
        return r;
    };
}

const lvvlambda mkuserPropProxy(const value& v) {
    return [v](unused const list<value>& params) -> const value {
        if (currentRequest == NULL)
            return v;
        if (currentRequest->user == NULL)
            return v;
        const value u = string(currentRequest->user);
        debug(u, "modeval::userPropProxy::value");
        return u;
    };
}

const value mkpropProxy(const value& prop) {
    const value n = scdl::name(prop);
    const value v = elementHasValue(prop)? elementValue(prop) : emptyStringValue;
    if (n == "app")
        return mkappPropProxy(v);
    if (n == "host")
        return mkhostPropProxy(v);
    if (n == "path")
        return mkpathPropProxy(v);
    if (n == "query")
        return mkqueryPropProxy(v);
    if (n == "user")
        return mkuserPropProxy(v);
    if (n == "realm")
        return mkrealmPropProxy(v);
    if (n == "timeout")
        return mktimeoutPropProxy(v);
    if (n == "email")
        return mkenvPropProxy("EMAIL", v);
    if (n == "nickname")
        return mkenvPropProxy("NICKNAME", v);
    if (n == "fullname")
        return mkenvPropProxy("FULLNAME", v);
    if (n == "firstname")
        return mkenvPropProxy("FIRSTNAME", v);
    if (n == "lastname")
        return mkenvPropProxy("LASTNAME", v);
    return mkvaluePropProxy(v);
}

const list<value> propProxies(const list<value>& props) {
    if (isNull(props))
        return props;
    return cons(mkpropProxy(car(props)), propProxies(cdr(props)));
}

/**
 * Evaluate a component and convert it to an applicable lambda function.
 */
const value evalComponent(const string& contribPath, const value& comp, const gc_mutable_ref<list<value> >& impls, const lvvlambda& lifecycle, const SSLConf& sslc, const int timeout) {
    extern const failable<lvvlambda > evalImplementation(const string& cpath, const value& impl, const list<value>& px, const lvvlambda& lifecycle);

    const value impl = scdl::implementation(comp);
    debug(comp, "modeval::evalComponent::comp");
    debug(impl, "modeval::evalComponent::impl");

    // Convert component references to configured proxy lambdas
    const list<value> rpx(refProxies(scdl::references(comp), impls, sslc, timeout));

    // Convert component properties to configured proxy lambdas
    const list<value> ppx(propProxies(scdl::properties(comp)));

    // Evaluate the component implementation and convert it to an applicable lambda function
    const failable<lvvlambda > cimpl(evalImplementation(contribPath, impl, append(rpx, ppx), lifecycle));
    if (!hasContent(cimpl)) {
        // Return a lambda function which will report a component evaluation failure
        // on all subsequent calls expect start/stop
        const value r = reason(cimpl);
        const lvvlambda implementationFailure = [r](const list<value>& params) -> const value {
            const value func = car(params);
            if (func == "start" || func == "stop")
                return mklist<value>(lvvlambda());
            return mklist<value>(nilValue, r);
        };
        return implementationFailure;
    }
    return content(cimpl);
}

/**
 * Return a list of component-name + configured-implementation pairs.
 */
const list<value> componentToImplementationAssoc(const list<value>& c, const string& contribPath, const gc_mutable_ref<list<value> >& impls, const lvvlambda& lifecycle, const SSLConf& sslc, const int timeout) {
    if (isNull(c))
        return c;
    return cons<value>(mklist<value>(scdl::name(car(c)),
                evalComponent(contribPath, car(c), impls, lifecycle, sslc, timeout)),
                    componentToImplementationAssoc(cdr(c), contribPath, impls, lifecycle, sslc, timeout));
}

/**
 * Read the components declared in a composite.
 */
const failable<list<value> > readComponents(const string& path) {
    ifstream is(path);
    if (fail(is))
        return mkfailure<list<value> >(string("Could not read composite: ") + path);
    return scdl::components(content(xml::readElements(streamList(is))));
}

/**
 * Get the components returned by a contributor.
 */
const failable<list<value> > getComponents(const lvvlambda& contributor, const string& name) {
    const failable<value> val = failableResult(contributor(cons<value>("get", mklist<value>(mklist<value>(name)))));
    if (!hasContent(val))
        return mkfailure<list<value> >(val);
    debug(content(val), "modeval::getComponents::val");
    const list<value> valc = assoc<value>(value("content"), cdr<value>(car<value>(content(val))));
    if (isNull(valc))
        return mkfailure<list<value> >(string("Could not get composite: ") + name);
    const list<value> comp = assoc<value>(value("composite"), cdr<value>(valc));
    debug(comp, "modeval::getComponents::comp");
    if (isNull(comp))
        return mkfailure<list<value> >(string("Could not get composite: ") + name);
    const failable<list<string> > x = xml::writeElements(car<value>(valuesToElements(mklist<value>(mklist<value>(comp)))));
    if (!hasContent(x))
        return mkfailure<list<value> >(x);
    return scdl::components(content(xml::readElements(content(x))));
}

/**
 * Apply a list of component implementations to a start or restart lifecycle expression.
 * Return the functions returned by the component implementations.
 */
const failable<list<value> > applyLifecycleExpr(const list<value>& impls, const list<value>& expr) {
    if (isNull(impls))
        return nilListValue;

    // Evaluate lifecycle expression against a component implementation lambda
    const lvvlambda l = cadr<value>(car(impls));
    const failable<value> r = failableResult(l(expr));
    if (!hasContent(r))
        return mkfailure<list<value> >(r);
    const lvvlambda rl = content(r);

    // Use the returned lambda function, if any, from now on
    const lvvlambda al = isNull(rl)? l : rl;

    // Continue with the rest of the list
    const failable<list<value> > nr = applyLifecycleExpr(cdr(impls), expr);
    if (!hasContent(nr))
        return nr;
    return cons<value>(mklist<value>(car<value>(car(impls)), value(al)), content(nr));
}

/**
 * Return a list of component-name + references pairs. The references are
 * arranged in trees of reference-name + reference-target pairs.
 */
const list<value> componentReferenceToTargetTree(const value& c) {
    return mklist<value>(scdl::name(c), mkbrbtree(sort(scdl::referenceToTargetAssoc(scdl::references(c)))));
}

const list<value> componentReferenceToTargetAssoc(const list<value>& c) {
    if (isNull(c))
        return c;
    return cons<value>(componentReferenceToTargetTree(car(c)), componentReferenceToTargetAssoc(cdr(c)));
}

/**
 * Return a list of service-URI-path + component-name pairs. Service-URI-paths are
 * represented as lists of URI path fragments.
 */
const list<value> defaultBindingURI(const string& cn, const string& sn) {
    return mklist<value>(cn, sn);
}

const list<value> bindingToComponentAssoc(const string& cn, const string& sn, const list<value>& b) {
    if (isNull(b))
        return b;
    const value uri(scdl::uri(car(b)));
    if (isNull(uri))
        return cons<value>(mklist<value>(defaultBindingURI(cn, sn), cn), bindingToComponentAssoc(cn, sn, cdr(b)));
    return cons<value>(mklist<value>(pathValues(c_str(string(uri))), cn), bindingToComponentAssoc(cn, sn, cdr(b)));
}

const list<value> serviceToComponentAssoc(const string& cn, const list<value>& s) {
    if (isNull(s))
        return s;
    const string sn(scdl::name(car(s)));
    const list<value> btoc(bindingToComponentAssoc(cn, sn, scdl::bindings(car(s))));
    if (isNull(btoc))
        return cons<value>(mklist<value>(defaultBindingURI(cn, sn), cn), serviceToComponentAssoc(cn, cdr(s)));
    return append<value>(btoc, serviceToComponentAssoc(cn, cdr(s)));
}

const list<value> uriToComponentAssoc(const list<value>& c) {
    if (isNull(c))
        return c;
    return append<value>(serviceToComponentAssoc(scdl::name(car(c)), scdl::services(car(c))), uriToComponentAssoc(cdr(c)));
}

/**
 * Configure the components declared in the deployed composite.
 */
const failable<Composite> confComponents(const string& contribPath, const string& composName, const value& contributor, const string& vhost, const gc_mutable_ref<list<value> >& impls, const lvvlambda lifecycle, const SSLConf& sslc, const int timeout) {
    debug(contribPath, "modeval::confComponents::contribPath");
    debug(composName, "modeval::confComponents::composName");
    debug(contributor, "modeval::confComponents::contributor");
    debug(vhost, "modeval::confComponents::vhost");
    debug(impls, "modeval::confComponents::impls");

    const failable<list<value> > fcomps = isNull(contributor)?
        readComponents(scdl::resourcePath(length(vhost) != 0? contribPath + vhost + "/" : contribPath, composName)) :
        getComponents(contributor, vhost);
    if (!hasContent(fcomps))
        return mkfailure<Composite>(fcomps);

    const list<value> comps = content(fcomps);
    debug(comps, "modeval::confComponents::comps");

    const list<value> refs = mkbrbtree(sort(componentReferenceToTargetAssoc(comps)));
    debug(flatten(refs), "modeval::confComponents::refs");

    const list<value> svcs = mkbrbtree(sort(uriToComponentAssoc(comps)));
    debug(flatten(svcs), "modeval::confComponents::svcs");

    const list<value> cimpls = mkbrbtree(sort(componentToImplementationAssoc(comps,
                    isNull(contributor)? length(vhost) != 0? contribPath + vhost + "/" : contribPath : contribPath,
                    impls, lifecycle, sslc, timeout)));
    debug(flatten(cimpls), "modeval::confComponents::impls");

    return Composite(refs, svcs, cimpls);
}

/**
 * Start the components declared in a composite.
 */
const failable<list<value> > startComponents(const list<value>& impls) {
    debug(flatten(impls), "modeval::startComponents::impls");
    const failable<list<value> > fsimpls = applyLifecycleExpr(flatten(impls), mklist<value>("start"));
    if (!hasContent(fsimpls))
        return mkfailure<list<value> >(fsimpls);

    const list<value> simpls = content(fsimpls);
    debug(impls, "modeval::startComponents::simpls");
    return mkbrbtree(sort(simpls));
}

/**
 * Stop the components declared in a composite.
 */
const failable<bool> stopComponents(const list<value>& simpls) {
    debug(flatten(simpls), "modeval::stopComponents::simpls");
    applyLifecycleExpr(flatten(simpls), mklist<value>("stop"));
    return true;
}

/**
 * Returns the media type accepted by a client.
 */
const value acceptMediaType(request_rec* const r) {
    const char* const xa = apr_table_get(r->headers_in, "X-Accept");
    const char* const a = xa != NULL? xa : apr_table_get(r->headers_in, "Accept");
    if (a == NULL)
        return nilValue;
    const string s(a);
    if (contains(s, "text/x-scheme"))
        return string("scheme");
    if (contains(s, "application/json"))
        return string("json");
    if (contains(s, "text/xml"))
        return string("xml");
    return nilValue;
}

/**
 * Handle an HTTP GET.
 */
const failable<int> get(const list<value>& rpath, request_rec* const r, const lvvlambda& impl) {
    debug(r->uri, "modeval::get::uri");

    // Inspect the query string
    const list<value> args = httpd::unescapeArgs(httpd::queryArgs(r));
    const list<value> ia = assoc(value("id"), args);
    const list<value> ma = assoc(value("method"), args);

    // Evaluate a JSON-RPC request and return a JSON result
    if (!isNull(ia) && !isNull(ma)) {

        // Extract the request id, method and params
        const value id = cadr(ia);
        const value func = c_str(json::funcName(string(cadr(ma))));

        // Apply the requested function
        const failable<value> val = failableResult(impl(cons(func, json::queryParams(args))));
        if (!hasContent(val))
            return mkfailure<int>(val);

        // Return JSON result
        return httpd::writeResult(json::jsonResult(id, content(val)), "application/json-rpc; charset=utf-8", r);
    }

    // Evaluate the GET expression
    const list<value> params(append<value>(cddr(rpath), mkvalues(args)));
    const failable<value> val = failableResult(impl(cons<value>("get", mklist<value>(params))));
    if (!hasContent(val))
        return mkfailure<int>(val);
    const value c = content(val);
    debug(c, "modeval::get::content");

    // Return a nil value as a not found status
    if (!isList(c) && isNull(c))
        return HTTP_NOT_FOUND;

    // Write in the format requested by the client, if any
    const list<value> fmt = assoc<value>("format", args);
    const value mtype = !isNull(fmt)? cadr(fmt) : acceptMediaType(r);
    if (!isNull(mtype)) {
        if (mtype == "scheme")
            return httpd::writeResult(scheme::writeValue(c), "text/x-scheme; charset=utf-8", r);
        if (mtype == "json")
            return httpd::writeResult(json::writeValue(c), "application/json; charset=utf-8", r);
        if (mtype == "xml")
            return httpd::writeResult(xml::writeElements(valuesToElements(c)), "text/xml; charset=utf-8", r);
    }

    // Write a simple value as a JSON value
    if (!isList(c)) {
        debug(c, "modeval::get::value");
        return httpd::writeResult(json::writeValue(c), "application/json; charset=utf-8", r);
    }

    // Write an empty list as a JSON value
    if (isNull((list<value>)c)) {
        debug(nilListValue, "modeval::get::empty");
        return httpd::writeResult(json::writeValue(c), "application/json; charset=utf-8", r);
    }

    // Write content-type / content-list pair
    if (isString(car<value>(c)) && !isNull(cdr<value>(c)) && isList(cadr<value>(c)))
        return httpd::writeResult(convertValues<string>(cadr<value>(c)), car<value>(c), r);

    // Write an assoc value as a JSON value
    if (isSymbol(car<value>(c)) && !isNull(cdr<value>(c))) {
        debug(c, "modeval::get::assoc");
        return httpd::writeResult(json::writeValue(c), "application/json; charset=utf-8", r);
    }

    // Write an ATOM feed or entry
    const list<value> e = valuesToElements(c);
    if (isList(car<value>(e)) && !isNull(car<value>(e))) {
        const list<value> el = car<value>(e);
        if (isSymbol(car<value>(el)) && car<value>(el) == element && !isNull(cdr<value>(el)) && isSymbol(cadr<value>(el)) && elementHasChildren(el) && !elementHasValue(el)) {
            if (cadr<value>(el) == atom::feed)
                return httpd::writeResult(atom::writeATOMFeed(e), "application/atom+xml; charset=utf-8", r);
            if (cadr<value>(el) == atom::entry)
                return httpd::writeResult(atom::writeATOMEntry(e), "application/atom+xml; charset=utf-8", r);
        }
    }

    // Write any other compound value as a JSON value
    return httpd::writeResult(json::writeValue(c), "application/json; charset=utf-8", r);
}

/**
 * Handle an HTTP POST.
 */
const failable<int> post(const list<value>& rpath, request_rec* const r, const lvvlambda& impl) {
    debug(r->uri, "modeval::post::uri");

    // Evaluate a JSON-RPC request and return a JSON result
    const string ct = httpd::contentType(r);
    if (contains(ct, "application/json-rpc") || contains(ct, "text/plain") || contains(ct, "application/x-www-form-urlencoded")) {

        // Read the JSON request
        const int rc = httpd::setupReadPolicy(r);
        if(rc != OK)
            return rc;
        const list<string> ls = httpd::read(r);
        debug(ls, "modeval::post::input");
        const value jsreq = content(json::readValue(ls));
        const list<value> args = httpd::postArgs(jsreq);

        // Extract the request id, method and params
        const value id = cadr(assoc(value("id"), args));
        const value func = c_str(json::funcName(cadr(assoc(value("method"), args))));
        const list<value> params = (list<value>)cadr(assoc(value("params"), args));

        // Evaluate the request expression
        const failable<value> val = failableResult(impl(cons<value>(func, params)));
        if (!hasContent(val))
            return mkfailure<int>(val);

        // Return JSON result
        return httpd::writeResult(json::jsonResult(id, content(val)), "application/json-rpc; charset=utf-8", r);
    }

    // Evaluate an ATOM POST request and return the location of the corresponding created resource
    if (contains(ct, "application/atom+xml")) {

        // Read the ATOM entry
        const int rc = httpd::setupReadPolicy(r);
        if(rc != OK)
            return rc;
        const list<string> ls = httpd::read(r);
        debug(ls, "modeval::post::input");
        const value aval = elementsToValues(content(atom::isATOMEntry(ls)? atom::readATOMEntry(ls) : atom::readATOMFeed(ls)));

        // Evaluate the POST expression
        const failable<value> val = failableResult(impl(cons<value>("post", mklist<value>(cddr(rpath), aval))));
        if (!hasContent(val))
            return mkfailure<int>(val);

        // Report HTTP status code
        const value rval = content(val);
        if (isNull(rval) || rval == falseValue)
            return HTTP_NOT_FOUND;
        if (isNumber(rval))
            return (int)rval;

        // Return the successfully created resource location
        debug(rval, "modeval::post::location");
        apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, c_str(httpd::url(r->uri, rval, r))));
        r->status = HTTP_CREATED;
        return OK;
    }

    // Unknown content type, wrap the HTTP request structure in a value and pass it to
    // the component implementation function
    const failable<value> val = failableResult(impl(cons<value>("handle", mklist<value>(httpd::requestValue(r)))));
    if (!hasContent(val))
        return mkfailure<int>(val);
    return (int)content(val);
}

/**
 * Handle an HTTP PUT.
 */
const failable<int> put(const list<value>& rpath, request_rec* const r, const lvvlambda& impl) {
    debug(r->uri, "modeval::put::uri");

    // Read the ATOM entry
    const int rc = httpd::setupReadPolicy(r);
    if(rc != OK)
        return rc;
    const list<string> ls = httpd::read(r);
    debug(ls, "modeval::put::input");
    const value aval = elementsToValues(content(atom::isATOMEntry(ls)? atom::readATOMEntry(ls) : atom::readATOMFeed(ls)));

    // Evaluate the PUT expression and update the corresponding resource
    const failable<value> val = failableResult(impl(cons<value>("put", mklist<value>(cddr(rpath), aval))));
    if (!hasContent(val))
        return mkfailure<int>(val);

    // Report HTTP status
    const value rval = content(val);
    if (isNull(rval) || rval == falseValue)
        return HTTP_NOT_FOUND;
    if (isNumber(rval))
        return (int)rval;
    return OK;
}

/**
 * Handle an HTTP PATCH.
 */
const failable<int> patch(const list<value>& rpath, request_rec* const r, const lvvlambda& impl) {
    debug(r->uri, "modeval::put::patch");

    // Read the ATOM entry
    const int rc = httpd::setupReadPolicy(r);
    if(rc != OK)
        return rc;
    const list<string> ls = httpd::read(r);
    debug(ls, "modeval::patch::input");
    const value aval = elementsToValues(content(atom::isATOMEntry(ls)? atom::readATOMEntry(ls) : atom::readATOMFeed(ls)));

    // Evaluate the PATCH expression and update the corresponding resource
    const failable<value> val = failableResult(impl(cons<value>("patch", mklist<value>(cddr(rpath), aval))));
    if (!hasContent(val))
        return mkfailure<int>(val);

    // Report HTTP status
    const value rval = content(val);
    if (isNull(rval) || rval == falseValue)
        return HTTP_NOT_FOUND;
    if (isNumber(rval))
        return (int)rval;
    return OK;
}

/**
 * Handle an HTTP DELETE.
 */
const failable<int> del(const list<value>& rpath, request_rec* const r, const lvvlambda& impl) {
    debug(r->uri, "modeval::delete::uri");

    // Evaluate an ATOM delete request
    const failable<value> val = failableResult(impl(cons<value>("delete", mklist<value>(cddr(rpath)))));
    if (!hasContent(val))
        return mkfailure<int>(val);

    // Report HTTP status
    const value rval = content(val);
    if (isNull(rval) || rval == falseValue)
        return HTTP_NOT_FOUND;
    if (isNumber(rval))
        return (int)rval;
    return OK;
}

/**
 * Proceed to handle a service component request.
 */
int proceedToHandler(request_rec* const r, const int rc) {
    r->handler = "mod_tuscany_eval";
    return rc;
}

int proceedToHandler(request_rec* const r, const int rc, const bool valias, const list<value>& rpath, const list<value>& vpath, const list<value>& impls) {
    r->handler = "mod_tuscany_eval";
    r->filename = apr_pstrdup(r->pool, c_str(string("/redirect:") + r->uri));

    // Store the selected vhost, path and composite in the request
    RequestConf& reqc = httpd::requestConf<RequestConf>(r, &mod_tuscany_eval);
    reqc.valias = valias;
    reqc.rpath = rpath;
    reqc.vpath = vpath;
    reqc.impls = impls;
    return rc;
}

/**
 * Route a component request to the specified component.
 */
int translateComponent(request_rec* const r, const list<value>& rpath, const list<value>& vpath, const list<value>& impls) {
    debug(rpath, "modeval::translateComponent::rpath");
    debug(flatten(impls), "modeval::translateComponent::impls");

    // Find the requested component
    if (isNull(cdr(rpath)))
        return HTTP_NOT_FOUND;
    const list<value> impl(rbtreeAssoc(cadr(rpath), impls));
    if (isNull(impl))
        return HTTP_NOT_FOUND;
    debug(impl, "modeval::translateComponent::impl");

    return proceedToHandler(r, OK, false, rpath, vpath, impls);;
}

/**
 * Route a /references/component-name/reference-name request,
 * to the target of the component reference.
 */
int translateReference(request_rec* const r, const list<value>& rpath, const list<value>& vpath, const list<value>& refs, const list<value>& impls) {
    debug(rpath, "modeval::translateReference::rpath");
    debug(flatten(refs), "modeval::translateReference::refs");

    // Find the requested component
    if (isNull(cdr(rpath)))
        return HTTP_NOT_FOUND;
    const list<value> comp(rbtreeAssoc(cadr(rpath), refs));
    if (isNull(comp))
        return HTTP_NOT_FOUND;
    debug(comp, "modeval::translateReference::comp");

    // Find the requested reference and target configuration
    const list<value> ref(rbtreeAssoc<value>(caddr(rpath), cadr(comp)));
    if (isNull(ref))
        return HTTP_NOT_FOUND;
    debug(ref, "modeval::translateReference::ref");

    const string target(cadr(ref));
    debug(target, "modeval::translateReference::target");

    // Route to an absolute target URI using mod_proxy or an HTTP client redirect
    const list<value> pathInfo = cdddr(rpath);
    if (http::isAbsolute(target)) {
        const string turi = target + (string)path(pathInfo) + (r->args != NULL? string("?") + string(r->args) : emptyString);
        const string proxy(string("proxy:") + turi);
        debug(proxy, "modeval::translateReference::proxy");
        r->filename = apr_pstrdup(r->pool, c_str(proxy));
        r->proxyreq = PROXYREQ_REVERSE;
        r->handler = "proxy-server";
        apr_table_setn(r->notes, "proxy-nocanon", "1");
        return OK;
    }

    // Route to a relative target URI using a local internal redirect
    // / c / target component name / request path info
    const value tname = substr(target, 0, find(target, '/'));
    const list<value> redir = cons<value>(string("c"), cons(tname, pathInfo));
    debug(redir, "modeval::translateReference::redirect");
    return proceedToHandler(r, OK, false, redir, vpath, impls);;
}

/**
 * Find a leaf matching a request path in a tree of URI paths.
 */
const int matchPath(const list<value>& k, const list<value>& p) {
    if (isNull(p))
        return true;
    if (isNull(k))
        return false;
    if (car(k) != car(p))
        return false;
    return matchPath(cdr(k), cdr(p));
}

const list<value> assocPath(const value& k, const list<value>& tree) {
    if (isNull(tree))
        return tree;
    if (matchPath(k, car<value>(car(tree))))
        return car(tree);
    if (k < car<value>(car(tree)))
        return assocPath(k, cadr(tree));
    return assocPath(k, caddr(tree));
}

/**
 * Route a service request to the component providing the requested service.
 */
int translateService(request_rec* const r, const list<value>& rpath, const list<value>& vpath, const list<value>& svcs, const list<value>& impls) {
    debug(rpath, "modeval::translateService::rpath");
    debug(flatten(svcs), "modeval::translateService::svcs");

    // Find the requested component
    if (isNull(rpath))
        return HTTP_NOT_FOUND;
    const list<value> svc(assocPath(rpath, svcs));
    if (isNull(svc))
        return DECLINED;
    debug(svc, "modeval::translateService::svc");

    // Dispatch to the target component using a local internal redirect
    // / c / target component name / request path info
    const list<value> redir = cons<value>(string("c"), cons<value>(cadr(svc), httpd::pathInfo(rpath, car(svc))));
    debug(redir, "modeval::translateService::redirect");
    return proceedToHandler(r, OK, false, redir, vpath, impls);
}

/**
 * Translate a request to the target app and component.
 */
const int translateRequest(request_rec* const r, const list<value>& rpath, const list<value>& vpath, const list<value>& refs, const list<value>& svcs, const list<value>& impls) {
    debug(vpath, "modeval::translateRequest::vpath");
    debug(rpath, "modeval::translateRequest::rpath");
    const string prefix = isNull(rpath)? emptyStringValue : car(rpath);

    // Translate a component request
    if ((prefix == string("components") || prefix == string("c")) && translateComponent(r, rpath, vpath, impls) == OK)
        return proceedToHandler(r, OK);

    // Translate a component reference request
    if ((prefix == string("references") || prefix == string("r")) && translateReference(r, rpath, vpath, refs, impls) == OK)
        return proceedToHandler(r, OK);

    // Attempt to translate the request to a service request
    if (translateService(r, rpath, vpath, svcs, impls) == OK)
        return proceedToHandler(r, OK);

    // Attempt to map a request targeting the main host to an actual file
    if (isNull(vpath)) {
        const failable<request_rec*> fnr = httpd::internalSubRequest(r->uri, r);
        if (!hasContent(fnr))
            return rcode(fnr);
        request_rec* const nr = content(fnr);
        nr->uri = r->filename;
        const int tr = ap_core_translate(nr);
        if (tr != OK)
            return tr;
        if (ap_directory_walk(nr) == OK && ap_file_walk(nr) == OK && nr->finfo.filetype != APR_NOFILE) {

            // Found the target file, let the default handler serve it
            debug(nr->filename, "modeval::translateRequest::file");
            return DECLINED;
        }
    } else {

        // Make sure a document root request ends with a '/' using
        // an external redirect
        if (isNull(rpath) && r->uri[strlen(r->uri) - 1] != '/') {
            const string target = string(r->uri) + string("/") + (r->args != NULL? string("?") + string(r->args) : emptyString);
            debug(target, "modeval::translateRequest::location");
            return proceedToHandler(r, httpd::externalRedirect(target, r));
        }

        // If the request didn't match a service, reference or component,
        // redirect it to / v / app / path. This will allow mapping to
        // the actual app resource using HTTPD aliases.
        debug(true, "modeval::translateRequest::valias");
        return proceedToHandler(r, OK, true, rpath, vpath, impls);
    }

    return HTTP_NOT_FOUND;
}

/**
 * Translate a request.
 */
int translate(request_rec *r) {
    if(r->method_number != M_GET && r->method_number != M_POST && r->method_number != M_PUT && r->method_number != M_PATCH && r->method_number != M_DELETE)
        return DECLINED;

    const gc_scoped_pool sp(r->pool);
    debug_httpdRequest(r, "modeval::translate::input");

    // Get the server configuration
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_eval);

    // Parse the request path
    const list<value> rpath = pathValues(r->uri);

    // Let default handler handle a resource request
    const string prefix = isNull(rpath)? emptyStringValue : car(rpath);
    if (prefix == string("vhosts") || prefix == string("v"))
        return DECLINED;

    // Get the request configuration
    RequestConf& reqc = httpd::requestConf<RequestConf>(r, &mod_tuscany_eval);

    // If the request is targeting a virtual host, configure the components
    // in that virtual host
    if (length(sc.vhostc.domain) != 0 && (length(sc.vhostc.contribPath) != 0 || !isNull(sc.vhostc.contributor)) && httpd::isVhostRequest(sc.server, sc.vhostc.domain, r)) {
        const string vname = http::subDomain(httpd::hostName(r));
        const failable<Composite> fvcompos = confComponents(sc.vhostc.contribPath, sc.vhostc.composName, sc.vhostc.contributor, vname, reqc.impls, (value)sc.lifecycle, sc.sslc, sc.timeout);
        if (!hasContent(fvcompos))
            return DECLINED;
        const Composite vcompos = content(fvcompos);

        // Flag the request as virtual host based
        reqc.vhost = true;

        // Translate the request
        reqc.impls = vcompos.impls;
        return translateRequest(r, rpath, mklist<value>(vname), vcompos.refs, vcompos.svcs, reqc.impls);
    }

    // Translate a request targeting the main host
    const int rc = translateRequest(r, rpath, nilListValue, sc.compos.refs, sc.compos.svcs, sc.compos.impls);
    if (rc != HTTP_NOT_FOUND)
        return rc;

    // Attempt to map the first segment of the request path to a virtual host
    if (length(prefix) != 0 && (length(sc.vhostc.contribPath) != 0 || !isNull(sc.vhostc.contributor))) {
        const string vname = prefix;
        const failable<Composite> fvcompos = confComponents(sc.vhostc.contribPath, sc.vhostc.composName, sc.vhostc.contributor, vname, reqc.impls, (value)sc.lifecycle, sc.sslc, sc.timeout);
        if (!hasContent(fvcompos))
            return DECLINED;
        const Composite vcompos = content(fvcompos);

        // Translate the request
        reqc.impls = vcompos.impls;
        return translateRequest(r, cdr(rpath), mklist<value>(vname), vcompos.refs, vcompos.svcs, reqc.impls);
    }
    return DECLINED;
}

/**
 * Handle a component request.
 */
const int handleRequest(const list<value>& rpath, request_rec* const r, const list<value>& impls) {
    debug(rpath, "modeval::handleRequest::path");

    // Get the component implementation lambda
    const list<value> impl(rbtreeAssoc<value>(cadr(rpath), impls));
    if (isNull(impl)) {
        mkfailure<int>(string("Couldn't find component implementation: ") + (string)cadr(rpath));
        return HTTP_NOT_FOUND;
    }
    const lvvlambda l(cadr<value>(impl));

    // Handle HTTP method
    if (r->header_only)
         return OK;
    if(r->method_number == M_GET)
        return httpd::reportStatus(get(rpath, r, l));
    if(r->method_number == M_POST)
        return httpd::reportStatus(post(rpath, r, l));
    if(r->method_number == M_PUT)
        return httpd::reportStatus(put(rpath, r, l));
    if(r->method_number == M_PATCH)
        return httpd::reportStatus(patch(rpath, r, l));
    if(r->method_number == M_DELETE)
        return httpd::reportStatus(del(rpath, r, l));
    return HTTP_NOT_IMPLEMENTED;
}

/**
 * HTTP request handler.
 */
int handler(request_rec* r) {
    if (r->handler != NULL && r->handler[0] != '\0')
        return DECLINED;

    // Attempt to translate the request
    const int trc = translate(r);

    // Pass if we couldn't translate the request
    if(trc != OK)
        return trc;
    if(strcmp(r->handler, "mod_tuscany_eval"))
        return DECLINED;

    // Create a scope for the current request
    const gc_scoped_pool sp(r->pool);
    ScopedRequest sr(r);

    debug_httpdRequest(r, "modeval::handler::input");

    // Get the request configuration
    RequestConf& reqc = httpd::requestConf<RequestConf>(r, &mod_tuscany_eval);

    // Handle an internal redirect as directed by the translate hook
    if (reqc.valias) {
        const string redir = path(cons<value>(string("v"), reqc.vhost? (const list<value>)reqc.vpath : nilListValue)) + string(r->uri) + (r->args != NULL? string("?") + string(r->args) : emptyString);
        debug(redir, "modeval::handler::internalredirect");
        return httpd::internalRedirect(redir, r);
    }
    if (isNull((const list<value>)reqc.rpath))
        return HTTP_NOT_FOUND;

    // Get the server configuration
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_eval);

    // Handle a request targeting a component in a virtual host
    if (!isNull((const list<value>)reqc.vpath)) {

        // Start the components in the virtual host
        const failable<list<value> > fsimpls = startComponents(reqc.impls);
        if (!hasContent(fsimpls))
            return HTTP_INTERNAL_SERVER_ERROR;
        const list<value> simpls = content(fsimpls);

        // Merge the components in the virtual host with the components in the main host
        reqc.impls = mkbrbtree(sort(append(flatten((const list<value>)sc.compos.impls), flatten(simpls))));

        // Handle the request against the running components
        const int rc = handleRequest(reqc.rpath, r, reqc.impls);

        // Stop the components in the virtual host
        stopComponents(simpls);
        return rc;
    }

    // Handle a request targeting a component in the main host
    return handleRequest(reqc.rpath, r, sc.compos.impls);
}

/**
 * Call an authenticator component to check a user's password.
 */
authn_status checkPassword(request_rec* r, const char* u, const char* p) {
    const gc_scoped_pool sp(r->pool);

    // Prevent FakeBasicAuth spoofing
    const string user = u;
    const string password = p;
    debug(user, "modeval::checkPassword::user");
    if (substr(user, 0, 1) != "/" && find(user, "/") != length(user) && password == "password") {
        mkfailure<int>(string("Encountered FakeBasicAuth spoof: ") + user, HTTP_UNAUTHORIZED);
        return AUTH_DENIED;
    }

    // Get the server configuration
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_eval);
    if (isNull(sc.vhostc.authenticator)) {
        mkfailure<int>("SCA authenticator not configured");
        return AUTH_GENERAL_ERROR;
    }

    // Retrieve the user's password hash
    const list<value> uid = pathValues(user);
    const failable<value> val = failableResult(((value)sc.vhostc.authenticator)(cons<value>("get", mklist<value>(uid))));
    if (!hasContent(val)) {
        mkfailure<int>(string("SCA authentication check user failed, user not found: ") + user, rcode(val), user != "admin");
        return AUTH_USER_NOT_FOUND;
    }
    const value hval = content(val);
    const list<value> hcontent = isList(hval) && !isNull(hval) && isList(car<value>(hval)) && !isNull(car<value>(hval))?  assoc<value>(value("content"), cdr<value>(car<value>(hval))) : nilListValue;
    const list<value> hassoc = isNull(hcontent)? nilListValue : assoc<value>(value("hash"), cdr<value>(hcontent));
    if (isNull(hassoc)) {
        mkfailure<int>(string("SCA authentication check user failed, hash not found: ") + user, -1, user != "admin");
        return AUTH_USER_NOT_FOUND;
    }
    const string hash = cadr<value>(hassoc);
    if (length(hash) == 0) {
        mkfailure<int>(string("SCA authentication check user failed: ") + user);
        return AUTH_USER_NOT_FOUND;
    }

    // Cache the hash in the auth cache provider, if available
    if (authnCacheStore != NULL)
        authnCacheStore(r, "component", u, NULL, c_str(hash));

    // Validate the presented password against the hash
    const apr_status_t rv = apr_password_validate(p, c_str(hash));
    if (rv != APR_SUCCESS) {
        mkfailure<int>(string("SCA authentication user password check failed: ") + user);
        return AUTH_DENIED;
    }
    return AUTH_GRANTED;
}

/**
 * Cleanup callback, called when the server is stopped or restarted.
 */
apr_status_t serverCleanup(void* v) {
    const gc_pool pool;
    ServerConf& sc = *(ServerConf*)v;
    debug("modeval::serverCleanup");

    // Stop the component implementations
    stopComponents(sc.compos.impls);

    // Call the module lifecycle function
    if (isNull((value)sc.lifecycle))
        return APR_SUCCESS;
    const lvvlambda ll = (value)sc.lifecycle;
    if (isNull(ll))
        return APR_SUCCESS;

    debug((value)sc.lifecycle, "modeval::serverCleanup::stop");
    ll(mklist<value>("stop"));

    return APR_SUCCESS;
}

/**
 * Called after all the configuration commands have been run.
 * Process the server configuration and configure the deployed components.
 */
const int postConfigMerge(const ServerConf& mainsc, server_rec* const s) {
    if (s == NULL)
        return OK;
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_eval);
    debug(httpd::serverName(s), "modeval::postConfigMerge::serverName");
    sc.lifecycle = mainsc.lifecycle;
    sc.contribc = mainsc.contribc;
    sc.vhostc = mainsc.vhostc;
    if (sc.sslc.ca == emptyString) sc.sslc.ca = mainsc.sslc.ca;
    if (sc.sslc.cert == emptyString) sc.sslc.cert = mainsc.sslc.cert;
    if (sc.sslc.key == emptyString) sc.sslc.key = mainsc.sslc.key;
    sc.timeout = mainsc.timeout;
    sc.compos = mainsc.compos;
    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t *p, unused apr_pool_t *plog, unused apr_pool_t *ptemp, server_rec *s) {
    extern const value applyLifecycle(const list<value>&);

    const gc_scoped_pool sp(p);

    // Get the server configuration and determine the server name
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_eval);
    debug(httpd::serverName(s), "modeval::postConfig::serverName");
    debug(sc.contribc.contribPath, "modeval::postConfig::contribPath");
    debug(sc.contribc.composName, "modeval::postConfig::composName");

    // Count the calls to post config
    const string k("tuscany::modeval::postConfig");
    const long int count = (long int)httpd::userData(k, s);
    httpd::putUserData(k, (void*)(count + 1), s);

    // Count == 0, do nothing as post config is always called twice,
    // count == 1 is the first start, count > 1 is a restart
    if (count == 0)
        return OK;

    if (count == 1) {
        // Chdir to the deployed contribution
        if (chdir(c_str(sc.contribc.contribPath)) != 0) {
            mkfailure<bool>(string("Couldn't chdir to the deployed contribution: ") + sc.contribc.contribPath);
            return -1;
        }

        debug("modeval::postConfig::start");
        const failable<value> r = failableResult(applyLifecycle(mklist<value>("start")));
        if (!hasContent(r))
            return -1;
        debug(content(r), "modeval::postConfig::setlifecycle");
        sc.lifecycle = content(r);
    }
    if (count > 1) {
        debug("modeval::postConfig::restart");
        const failable<value> r = failableResult(applyLifecycle(mklist<value>("restart")));
        if (!hasContent(r))
            return -1;
        debug(content(r), "modeval::postConfig::setlifecycle");
        sc.lifecycle = content(r);
    }

    // Configure the deployed components
    const failable<Composite> compos = confComponents(sc.contribc.contribPath, sc.contribc.composName, nilValue, emptyString, sc.compos.impls, (value)sc.lifecycle, sc.sslc, sc.timeout);
    if (!hasContent(compos)) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << endl;
        return -1;
    }
    sc.compos = content(compos);

    // Register a cleanup callback, called when the server is stopped or restarted
    apr_pool_pre_cleanup_register(p, (void*)&sc, serverCleanup);

    // Merge the configuration into the virtual hosts
    return postConfigMerge(sc, s->next);
}

/**
 * Exit after a failure.
 */
void failureExitChild() {
    cfailure << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << endl;
    exit(APEXIT_CHILDFATAL);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* p, server_rec* s) {
    const gc_scoped_pool sp(p);

    ServerConf* const psc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_eval);
    if(psc == NULL)
        failureExitChild();
    ServerConf& sc = *psc;

    // Start the components in the child process
    const failable<list<value> > fsimpls = startComponents(sc.compos.impls);
    if (!hasContent(fsimpls))
        failureExitChild();
    sc.compos.impls = content(fsimpls);
    
    // Get the vhost contributor component implementation lambda
    if (length(sc.vhostc.contributorName) != 0) {
        const list<value> impl(rbtreeAssoc<value>((string)sc.vhostc.contributorName, (const list<value>)sc.compos.impls));
        if (isNull(impl)) {
            mkfailure<int>(string("Couldn't find contributor component implementation: ") + sc.vhostc.contributorName);
            failureExitChild();
        }
        sc.vhostc.contributor = cadr<value>(impl);
    }

    // Get the vhost authenticator component implementation lambda
    if (length(sc.vhostc.authenticatorName) != 0) {
        const list<value> impl(rbtreeAssoc<value>((string)sc.vhostc.authenticatorName, (const list<value>)sc.compos.impls));
        if (isNull(impl)) {
            mkfailure<int>(string("Couldn't find authenticator component implementation: ") + sc.vhostc.authenticatorName);
            failureExitChild();
        }
        sc.vhostc.authenticator = cadr<value>(impl);
    }

    // Merge the updated configuration into the virtual hosts
    postConfigMerge(sc, s->next);

    // Register a cleanup callback, called when the child is stopped or restarted
    apr_pool_pre_cleanup_register(p, (void*)psc, serverCleanup);
}

/**
 * Configuration commands.
 */
const char* confContribution(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.contribc.contribPath = arg;
    return NULL;
}
const char* confComposite(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.contribc.composName = arg;
    return NULL;
}
const char* confVirtualDomain(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostc.domain = arg;
    return NULL;
}
const char* confVirtualContribution(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostc.contribPath = arg;
    return NULL;
}
const char* confVirtualContributor(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostc.contributorName = arg;
    return NULL;
}
const char* confVirtualComposite(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostc.composName = arg;
    return NULL;
}
const char* confAuthenticator(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostc.authenticatorName = arg;
    return NULL;
}
const char* confCAFile(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.sslc.ca = arg;
    return NULL;
}
const char* confCertFile(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.sslc.cert = arg;
    return NULL;
}
const char* confCertKeyFile(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.sslc.key = arg;
    return NULL;
}
const char* confTimeout(cmd_parms *cmd, unused void *c, const char *arg) {
    const gc_scoped_pool sp(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.timeout = atoi(arg);
    return NULL;
}
const char* confEnv(unused cmd_parms *cmd, unused void *c, const char *name, const char *value) {
    const gc_scoped_pool sp(cmd->pool);
    setenv(name, value != NULL? value : "", 1);
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE1("SCAContribution", (const char*(*)())confContribution, NULL, RSRC_CONF, "SCA contribution location"),
    AP_INIT_TAKE1("SCAComposite", (const char*(*)())confComposite, NULL, RSRC_CONF, "SCA composite location"),
    AP_INIT_TAKE1("SCAVirtualDomain", (const char*(*)())confVirtualDomain, NULL, RSRC_CONF, "SCA virtual host domain"),
    AP_INIT_TAKE1("SCAVirtualContribution", (const char*(*)())confVirtualContribution, NULL, RSRC_CONF, "SCA virtual host contribution path"),
    AP_INIT_TAKE1("SCAVirtualContributor", (const char*(*)())confVirtualContributor, NULL, RSRC_CONF, "SCA virtual host contributor component"),
    AP_INIT_TAKE1("SCAVirtualComposite", (const char*(*)())confVirtualComposite, NULL, RSRC_CONF, "SCA virtual composite location"),
    AP_INIT_TAKE1("SCAAuthenticator", (const char*(*)())confAuthenticator, NULL, RSRC_CONF, "SCA authenticator component"),
    AP_INIT_TAKE12("SCASetEnv", (const char*(*)())confEnv, NULL, OR_FILEINFO, "Environment variable name and optional value"),
    AP_INIT_TAKE1("SCAWiringSSLCACertificateFile", (const char*(*)())confCAFile, NULL, RSRC_CONF, "SCA wiring SSL CA certificate file"),
    AP_INIT_TAKE1("SCAWiringSSLCertificateFile", (const char*(*)())confCertFile, NULL, RSRC_CONF, "SCA wiring SSL certificate file"),
    AP_INIT_TAKE1("SCAWiringSSLCertificateKeyFile", (const char*(*)())confCertKeyFile, NULL, RSRC_CONF, "SCA wiring SSL certificate key file"),
    AP_INIT_TAKE1("SCAWiringTimeout", (const char*(*)())confTimeout, NULL, RSRC_CONF, "SCA wiring timeout"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};


const authn_provider AuthnProvider = {
    &checkPassword,
    NULL
};

void retrieveAuthnCacheStore() {
    authnCacheStore = APR_RETRIEVE_OPTIONAL_FN(ap_authn_cache_store);
}

void registerHooks(unused apr_pool_t *p) {
    ap_hook_post_config(postConfig, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(childInit, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_register_auth_provider(p, AUTHN_PROVIDER_GROUP, "component", AUTHN_PROVIDER_VERSION, &AuthnProvider, AP_AUTH_INTERNAL_PER_CONF);
    ap_hook_optional_fn_retrieve(retrieveAuthnCacheStore, NULL, NULL, APR_HOOK_MIDDLE);
}

}
}
}

extern "C" {

module AP_MODULE_DECLARE_DATA mod_tuscany_eval = {
    STANDARD20_MODULE_STUFF,
    // dir config and merger
    NULL, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::server::modeval::ServerConf>, NULL,
    // commands and hooks
    tuscany::server::modeval::commands, tuscany::server::modeval::registerHooks
};

}

#endif
