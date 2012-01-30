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

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_eval;
}

namespace tuscany {
namespace server {
namespace modeval {

/**
 * Set to true to wire using mod_proxy, false to wire using HTTP client redirects.
 */
const bool useModProxy = true;

/**
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf(apr_pool_t* p, server_rec* s) : p(p), server(s) {
    }

    ServerConf(apr_pool_t* p, const ServerConf& ssc, const string& name) : p(p), server(ssc.server), lifecycle(ssc.lifecycle), vhostName(name), compositeName(ssc.vhostCompositeName), ca(ssc.ca), cert(ssc.cert), key(ssc.key), vhostContributor(ssc.vhostContributor) {
        contributionPath = length(ssc.vhostContributionPath) != 0? ssc.vhostContributionPath + name + "/" : ssc.contributionPath;
    }

    const gc_pool p;
    server_rec* server;
    lambda<value(const list<value>&)> lifecycle;
    string vhostName;
    string contributionPath;
    string compositeName;
    string vhostDomain;
    string vhostContributionPath;
    string vhostCompositeName;
    string vhostContributorName;
    string ca;
    string cert;
    string key;
    list<value> references;
    list<value> services;
    list<value> implementations;
    list<value> implTree;
    value vhostContributor;
};

/**
 * Return true if a server contains a contribution configuration.
 */
const bool hasContributionConf(const ServerConf& sc) {
    return length(sc.contributionPath) != 0;
}

/**
 * Return true if a server contains a virtual host domain configuration.
 */
const bool hasVhostDomainConf(const ServerConf& sc) {
    return length(sc.vhostDomain) != 0;
}

/**
 * Return true if a server contains a virtual host contribution configuration.
 */
const bool hasVhostContributionConf(const ServerConf& sc) {
    return length(sc.vhostContributionPath) != 0 || length(sc.vhostContributorName) != 0;
}

/**
 * Convert a result represented as a content + failure pair to a
 * failable monad.
 */
const failable<value> failableResult(const list<value>& v) {
    if (isNil(cdr(v)))
        return car(v);
    return mkfailure<value>(string(cadr(v)), false);
}

/**
 * Store current HTTP request for access from property and proxy lambda functions.
 */
#ifdef WANT_THREADS
perthread_ptr<request_rec> currentRequest;
#else
request_rec* currentRequest = NULL;
#endif

class ScopedRequest {
public:
    ScopedRequest(request_rec* r) {
        currentRequest = r;
    }

    ~ScopedRequest() {
        currentRequest = NULL;
    }
};

/**
 * Make an HTTP proxy lambda to a component reference.
 */
const value mkhttpProxy(const ServerConf& sc, const string& ref, const string& base) {
    const string uri = base + ref;
    debug(uri, "modeval::mkhttpProxy::uri");
    return lambda<value(const list<value>&)>(http::proxy(uri, sc.ca, sc.cert, sc.key, "", sc.p));
}

/**
 * Make an HTTP proxy lambda to an absolute URI
 */
const value mkhttpProxy(const ServerConf& sc, const string& uri) {
    debug(uri, "modeval::mkhttpProxy::uri");
    return lambda<value(const list<value>&)>(http::proxy(uri, "", "", "", "", sc.p));
}

/**
 * Return a component implementation proxy lambda.
 */
class implProxy {
public:
    implProxy(const ServerConf& sc, const value& name) : sc(sc), name(name) {
    }

    const value callImpl(const value& cname, const list<value>& aparams) const {

        // Lookup the component implementation
        const list<value> impl(assoctree<value>(cname, sc.implTree));
        if (isNil(impl))
            return mkfailure<value>(string("Couldn't find component implementation: ") + cname);

        // Call its lambda function
        const lambda<value(const list<value>&)> l(cadr<value>(impl));
        const value func = c_str(car(aparams));
        const failable<value> val = failableResult(l(cons(func, cdr(aparams))));
        debug(val, "modeval::implProxy::result");
        if (!hasContent(val))
            return value();
        return content(val);
    }

    const value operator()(const list<value>& params) const {
        debug(name, "modeval::implProxy::name");
        debug(params, "modeval::implProxy::input");

        // If the reference was 'wiredByImpl' use the first param as target
        if (isNil(name)) {
            const value uri = cadr(params);
            const list<value> aparams = cons<value>(car(params), cddr(params));
            debug(uri, "modeval::implProxy::wiredByImpl::uri");
            debug(aparams, "modeval::implProxy::wiredByImpl::input");

            // Use an HTTP proxy if the target is an absolute :// target
            if (http::isAbsolute(uri)) {
                gc_pool p(currentRequest->pool);

                // Interpret a uri in the form app://appname, convert it using the scheme,
                // top level domain and port number from the current request
                if (http::scheme(uri, p) == "app") {
                    ostringstream appuri;
                    appuri << httpd::scheme(currentRequest) << "://" << substr(uri, 6) << "." << http::topDomain(httpd::hostName(currentRequest)) << ":" << httpd::port(currentRequest) << "/";
                    debug(str(appuri), "modeval::implProxy::httpproxy::appuri");
                    const lambda<value(const list<value>&)> px = lambda<value(const list<value>&)>(http::proxy(str(appuri), sc.ca, sc.cert, sc.key, httpd::cookie(currentRequest), p));
                    return px(aparams);
                }
                
                // Pass our certificate and the cookie from the current request
                // only if the target is in the same top level domain
                if (http::topDomain(http::hostName(uri, p)) == http::topDomain(httpd::hostName(currentRequest))) {
                    debug(uri, "modeval::implProxy::httpproxy::samedomain");
                    const lambda<value(const list<value>&)> px = lambda<value(const list<value>&)>(http::proxy(uri, sc.ca, sc.cert, sc.key, httpd::cookie(currentRequest), p));
                    return px(aparams);
                }

                // No certificate or cookie on a cross domain call
                debug(uri, "modeval::implProxy::httpproxy::crossdomain");
                const lambda<value(const list<value>&)> px = lambda<value(const list<value>&)>(http::proxy(uri, "", "", "", "", p));
                return px(aparams);
            }

            // Call the component implementation
            return callImpl(uri, aparams);
        }

        // Call the component implementation
        return callImpl(name, params);
    }

private:
    const ServerConf& sc;
    const value name;
};

const value mkimplProxy(const ServerConf& sc, const value& name) {
    debug(name, "modeval::implProxy::impl");
    return lambda<value(const list<value>&)>(implProxy(sc, name));
}

/**
 * Return a proxy lambda for an unwired reference.
 */
class unwiredProxy {
public:
    unwiredProxy(const value& name) : name(name) {
    }

    const value operator()(const list<value>& params) const {
        debug(name, "modeval::unwiredProxy::name");
        debug(params, "modeval::unwiredProxy::input");

        // Get function returns a default empty value
        if (car(params) == "get") {
            debug(value(), "modeval::unwiredProxy::result");
            return value();
        }

        // All other functions return a failure
        return mkfailure<value>(string("Reference is not wired: ") + name);
    }

private:
    const value name;
};

/**
 * Make a proxy lambda for an unwired reference.
 */
const value mkunwiredProxy(const string& ref) {
    debug(ref, "modeval::mkunwiredProxy::ref");
    return lambda<value(const list<value>&)>(unwiredProxy(ref));
}

/**
 * Convert a list of component references to a list of proxy lambdas.
 */
const value mkrefProxy(const ServerConf& sc, const value& ref) {
    const value target = scdl::target(ref);
    const bool wbyimpl = scdl::wiredByImpl(ref);
    debug(ref, "modeval::mkrefProxy::ref");
    debug(target, "modeval::mkrefProxy::target");
    debug(wbyimpl, "modeval::mkrefProxy::wiredByImpl");

    // Use an HTTP proxy or an internal proxy to the component implementation
    if (wbyimpl)
        return mkimplProxy(sc, value());
    if (isNil(target))
        return mkunwiredProxy(scdl::name(ref));
    if (http::isAbsolute(target))
        return mkhttpProxy(sc, target);
    return mkimplProxy(sc, car(pathValues(target)));
}

const list<value> refProxies(const ServerConf& sc, const list<value>& refs) {
    if (isNil(refs))
        return refs;
    return cons(mkrefProxy(sc, car(refs)), refProxies(sc, cdr(refs)));
}

/**
 * Convert a list of component properties to a list of lambda functions that just return
 * the property value. The host, user and email properties are configured with the values
 * from the HTTP request, if any.
 */
struct propProxy {
    const value v;
    propProxy(const value& v) : v(v) {
    }
    const value operator()(unused const list<value>& params) const {
        return v;
    }
};

struct hostPropProxy {
    const value v;
    hostPropProxy(const value& v) : v(v) {
    }
    const value operator()(unused const list<value>& params) const {
        const value r = httpd::hostName(currentRequest, v);
        debug(r, "modeval::hostPropProxy::value");
        return r;
    }
};

struct appPropProxy {
    const value v;
    appPropProxy(const value& v) : v(v) {
    }
    const value operator()(unused const list<value>& params) const {
        const char* n = apr_table_get(currentRequest->notes, "X-Request-AppName");
        const value a = n != NULL? value(string(n)) : v;
        debug(a, "modeval::appPropProxy::value");
        return a;
    }
};

struct pathPropProxy {
    pathPropProxy(unused const value& v) {
    }
    const value operator()(unused const list<value>& params) const {
        const char* u = apr_table_get(currentRequest->notes, "X-Request-URI");
        const value v = u != NULL? pathValues(string(u)) : list<value>();
        debug(v, "modeval::pathPropProxy::value");
        return v;
    }
};

struct queryPropProxy {
    queryPropProxy(unused const value& v) {
    }
    const value operator()(unused const list<value>& params) const {
        const value v = httpd::unescapeArgs(httpd::queryArgs(currentRequest));
        debug(v, "modeval::queryPropProxy::value");
        return v;
    }
};

struct envPropProxy {
    const string name;
    const value v;
    envPropProxy(const string& name, const value& v) : name(name), v(v) {
    }
    const value operator()(unused const list<value>& params) const {
        const char* env = apr_table_get(currentRequest->subprocess_env, c_str(name));
        if (env == NULL || *env == '\0')
            return v;
        return string(env);
    }
};

struct userPropProxy {
    const value v;
    userPropProxy(const value& v) : v(v) {
    }
    const value operator()(unused const list<value>& params) const {
        if (currentRequest->user == NULL)
            return v;
        return string(currentRequest->user);
    }
};

const value mkpropProxy(const value& prop) {
    const value n = scdl::name(prop);
    const value v = elementHasValue(prop)? elementValue(prop):value(string(""));
    if (n == "app")
        return lambda<value(const list<value>&)>(appPropProxy(v));
    if (n == "host")
        return lambda<value(const list<value>&)>(hostPropProxy(v));
    if (n == "path")
        return lambda<value(const list<value>&)>(pathPropProxy(v));
    if (n == "query")
        return lambda<value(const list<value>&)>(queryPropProxy(v));
    if (n == "user")
        return lambda<value(const list<value>&)>(userPropProxy(v));
    if (n == "realm")
        return lambda<value(const list<value>&)>(envPropProxy("REALM", v));
    if (n == "email")
        return lambda<value(const list<value>&)>(envPropProxy("EMAIL", v));
    if (n == "nickname")
        return lambda<value(const list<value>&)>(envPropProxy("NICKNAME", v));
    if (n == "fullname")
        return lambda<value(const list<value>&)>(envPropProxy("FULLNAME", v));
    if (n == "firstname")
        return lambda<value(const list<value>&)>(envPropProxy("FIRSTNAME", v));
    if (n == "lastname")
        return lambda<value(const list<value>&)>(envPropProxy("LASTNAME", v));
    return lambda<value(const list<value>&)>(propProxy(v));
}

const list<value> propProxies(const list<value>& props) {
    if (isNil(props))
        return props;
    return cons(mkpropProxy(car(props)), propProxies(cdr(props)));
}

/**
 * Evaluate a component and convert it to an applicable lambda function.
 */
struct implementationFailure {
    const value reason;
    implementationFailure(const value& r) : reason(r) {
    }

    // Default implementation representing an implementation that
    // couldn't be evaluated. Report the evaluation error on all
    // subsequent calls expect start/stop.
    const value operator()(unused const list<value>& params) const {
        const value func = car(params);
        if (func == "start" || func == "stop")
            return mklist<value>(lambda<value(const list<value>&)>());
        return mklist<value>(value(), reason);
    }
};

const value evalComponent(ServerConf& sc, const value& comp) {
    extern const failable<lambda<value(const list<value>&)> > evalImplementation(const string& cpath, const value& impl, const list<value>& px, const lambda<value(const list<value>&)>& lifecycle);

    const value impl = scdl::implementation(comp);
    debug(comp, "modeval::evalComponent::comp");
    debug(impl, "modeval::evalComponent::impl");

    // Convert component references to configured proxy lambdas
    const list<value> rpx(refProxies(sc, scdl::references(comp)));

    // Convert component properties to configured proxy lambdas
    const list<value> ppx(propProxies(scdl::properties(comp)));

    // Evaluate the component implementation and convert it to an applicable lambda function
    const failable<lambda<value(const list<value>&)> > cimpl(evalImplementation(sc.contributionPath, impl, append(rpx, ppx), sc.lifecycle));
    if (!hasContent(cimpl))
        return lambda<value(const list<value>&)>(implementationFailure(reason(cimpl)));
    return content(cimpl);
}

/**
 * Return a list of component-name + configured-implementation pairs.
 */
const list<value> componentToImplementationAssoc(ServerConf& sc, const list<value>& c) {
    if (isNil(c))
        return c;
    return cons<value>(mklist<value>(scdl::name(car(c)), evalComponent(sc, car(c))), componentToImplementationAssoc(sc, cdr(c)));
}

/**
 * Read the components declared in a composite.
 */
const failable<list<value> > readComponents(const string& path) {
    ifstream is(path);
    if (fail(is))
        return mkfailure<list<value> >(string("Could not read composite: ") + path);
    return scdl::components(readXML(streamList(is)));
}

/**
 * Apply a list of component implementations to a start or restart lifecycle expression.
 * Return the functions returned by the component implementations.
 */
const failable<list<value> > applyLifecycleExpr(const list<value>& impls, const list<value>& expr) {
    if (isNil(impls))
        return list<value>();

    // Evaluate lifecycle expression against a component implementation lambda
    const lambda<value(const list<value>&)> l = cadr<value>(car(impls));
    const failable<value> r = failableResult(l(expr));
    if (!hasContent(r))
        return mkfailure<list<value> >(reason(r));
    const lambda<value(const list<value>&)> rl = content(r);

    // Use the returned lambda function, if any, from now on
    const lambda<value(const list<value>&)> al = isNil(rl)? l : rl;

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
    return mklist<value>(scdl::name(c), mkbtree(sort(scdl::referenceToTargetAssoc(scdl::references(c)))));
}

const list<value> componentReferenceToTargetAssoc(const list<value>& c) {
    if (isNil(c))
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
    if (isNil(b))
        return b;
    const value uri(scdl::uri(car(b)));
    if (isNil(uri))
        return cons<value>(mklist<value>(defaultBindingURI(cn, sn), cn), bindingToComponentAssoc(cn, sn, cdr(b)));
    return cons<value>(mklist<value>(pathValues(c_str(string(uri))), cn), bindingToComponentAssoc(cn, sn, cdr(b)));
}

const list<value> serviceToComponentAssoc(const string& cn, const list<value>& s) {
    if (isNil(s))
        return s;
    const string sn(scdl::name(car(s)));
    const list<value> btoc(bindingToComponentAssoc(cn, sn, scdl::bindings(car(s))));
    if (isNil(btoc))
        return cons<value>(mklist<value>(defaultBindingURI(cn, sn), cn), serviceToComponentAssoc(cn, cdr(s)));
    return append<value>(btoc, serviceToComponentAssoc(cn, cdr(s)));
}

const list<value> uriToComponentAssoc(const list<value>& c) {
    if (isNil(c))
        return c;
    return append<value>(serviceToComponentAssoc(scdl::name(car(c)), scdl::services(car(c))), uriToComponentAssoc(cdr(c)));
}

/**
 * Configure the components declared in the deployed composite.
 */
const failable<bool> confComponents(ServerConf& sc) {
    if (!hasContributionConf(sc))
        return false;
    debug(sc.contributionPath, "modeval::confComponents::contributionPath");
    debug(sc.contributionPath, "modeval::confComponents::contributorName");
    debug(sc.compositeName, "modeval::confComponents::compositeName");
    if (sc.ca != "") debug(sc.ca, "modeval::confComponents::sslCA");
    if (sc.cert != "") debug(sc.cert, "modeval::confComponents::sslCert");
    if (sc.key != "") debug(sc.key, "modeval::confComponents::sslKey");

    // Read the components and get their services, references and implementation
    // lambda functions
    const failable<list<value> > comps = readComponents(scdl::resourcePath(sc.contributionPath, sc.compositeName));
    if (!hasContent(comps))
        return mkfailure<bool>(reason(comps));

    const list<value> refs = componentReferenceToTargetAssoc(content(comps));
    debug(refs, "modeval::confComponents::references");
    sc.references = mkbtree(sort(refs));

    const list<value> svcs = uriToComponentAssoc(content(comps));
    debug(svcs, "modeval::confComponents::services");
    sc.services = mkbtree(sort(svcs));

    sc.implementations = componentToImplementationAssoc(sc, content(comps));
    debug(sc.implementations, "modeval::confComponents::implementations");
    return true;
}

/**
 * Start the components declared in the deployed composite.
 */
const failable<bool> startComponents(ServerConf& sc) {

    // Start the components and record the returned implementation lambda functions
    debug(sc.implementations, "modeval::startComponents::start");
    const failable<list<value> > impls = applyLifecycleExpr(sc.implementations, mklist<value>("start"));
    if (!hasContent(impls))
        return mkfailure<bool>(reason(impls));
    sc.implementations = content(impls);
    debug(sc.implementations, "modeval::startComponents::implementations");
    return true;
}

/**
 * Configure and start the components deployed in a virtual host.
 */
const failable<bool> vhostConfig(ServerConf& sc, const ServerConf& ssc, request_rec* r) {
    debug(httpd::serverName(ssc.server), "modeval::vhostConfig::serverName");
    debug(httpd::serverName(r), "modeval::vhostConfig::vhostName");
    debug(ssc.vhostContributionPath, "modeval::vhostConfig::vhostContributionPath");
    debug(sc.contributionPath, "modeval::vhostConfig::contributionPath");

    // Configure the deployed components
    const failable<bool> cr = confComponents(sc);
    if (!hasContent(cr))
        return cr;

    // Store the virtual host configuration in the request config

    return true;
}

/**
 * Start the components deployed in a virtual host.
 */
const failable<bool> vhostStart(ServerConf& sc, const ServerConf& ssc, request_rec* r) {
    debug(httpd::serverName(ssc.server), "modeval::vhostStart::serverName");
    debug(httpd::serverName(r), "modeval::vhostStart::vhostName");
    debug(ssc.vhostContributionPath, "modeval::vhostStart::vhostContributionPath");
    debug(sc.contributionPath, "modeval::vhostStart::contributionPath");

    // Configure the components deployed in a virtual host
    const failable<bool> cr = vhostConfig(sc, ssc, r);
    if (!hasContent(cr))
        return cr;

    // Start the configured components
    const failable<bool> sr = startComponents(sc);
    if (!hasContent(sr))
        return sr;

    // Store the implementation lambda functions (from both the virtual host and the
    // main server) in a tree for fast retrieval
    sc.implTree = mkbtree(sort(append(sc.implementations, ssc.implementations)));
    return true;
}

/**
 * Stop a virtual host.
 */
const failable<bool> vhostStop(const ServerConf& sc, unused const ServerConf& ssc) {
    if (!hasContributionConf(sc))
        return true;
    debug("modeval::vhostStop");

    // Stop the component implementations
    applyLifecycleExpr(sc.implementations, mklist<value>("stop"));

    return true;
}

/**
 * Handle an HTTP GET.
 */
const failable<int> get(const list<value>& rpath, request_rec* r, const lambda<value(const list<value>&)>& impl) {
    debug(r->uri, "modeval::get::uri");

    // Inspect the query string
    const list<list<value> > args = httpd::queryArgs(r);
    const list<value> ia = assoc(value("id"), args);
    const list<value> ma = assoc(value("method"), args);

    // Evaluate a JSON-RPC request and return a JSON result
    if (!isNil(ia) && !isNil(ma)) {

        // Extract the request id, method and params
        const value id = cadr(ia);
        const value func = c_str(json::funcName(string(cadr(ma))));

        // Apply the requested function
        const failable<value> val = failableResult(impl(cons(func, json::queryParams(args))));
        if (!hasContent(val))
            return mkfailure<int>(reason(val));

        // Return JSON result
        js::JSContext cx;
        return httpd::writeResult(json::jsonResult(id, content(val), cx), "application/json-rpc; charset=utf-8", r);
    }

    // Evaluate the GET expression
    const list<value> params(append<value>(cddr(rpath), mkvalues(args)));
    const failable<value> val = failableResult(impl(cons<value>("get", mklist<value>(params))));
    if (!hasContent(val))
        return mkfailure<int>(reason(val));
    const value c = content(val);
    debug(c, "modeval::get::content");

    // Check if the client requested a specific format
    const list<value> fmt = assoc<value>("format", args);

    // Write as a scheme value if requested by the client
    if (!isNil(fmt) && cadr(fmt) == "scheme")
        return httpd::writeResult(mklist<string>(scheme::writeValue(c)), "text/plain; charset=utf-8", r);

    // Write a simple value as a JSON value
    if (!isList(c)) {
        js::JSContext cx;
        if (isSymbol(c)) {
            const list<value> lc = mklist<value>(mklist<value>("name", value(string(c))));
            debug(lc, "modeval::get::symbol");
            return httpd::writeResult(json::writeJSON(valuesToElements(lc), cx), "application/json; charset=utf-8", r);
        }
        const list<value> lc = mklist<value>(mklist<value>("value", c));
        debug(lc, "modeval::get::value");
        return httpd::writeResult(json::writeJSON(valuesToElements(lc), cx), "application/json; charset=utf-8", r);
    }

    // Write an empty list as a JSON empty value
    if (isNil((list<value>)c)) {
        js::JSContext cx;
        debug(list<value>(), "modeval::get::empty");
        return httpd::writeResult(json::writeJSON(list<value>(), cx), "application/json; charset=utf-8", r);
    }

    // Write content-type / content-list pair
    if (isString(car<value>(c)) && !isNil(cdr<value>(c)) && isList(cadr<value>(c)))
        return httpd::writeResult(convertValues<string>(cadr<value>(c)), car<value>(c), r);

    // Write an assoc value as a JSON result
    if (isSymbol(car<value>(c)) && !isNil(cdr<value>(c))) {
        js::JSContext cx;
        const list<value> lc = mklist<value>(c);
        debug(lc, "modeval::get::assoc");
        debug(valuesToElements(lc), "modeval::get::assoc::element");
        return httpd::writeResult(json::writeJSON(valuesToElements(lc), cx), "application/json; charset=utf-8", r);
    }

    // Write value as JSON if requested by the client
    if (!isNil(fmt) && cadr(fmt) == "json") {
        js::JSContext cx;
        return httpd::writeResult(json::writeJSON(valuesToElements(c), cx), "application/json; charset=utf-8", r);
    }

    // Convert list of values to element values
    const list<value> e = valuesToElements(c);
    debug(e, "modeval::get::elements");

    // Write an ATOM feed or entry
    if (isList(car<value>(e)) && !isNil(car<value>(e))) {
        const list<value> el = car<value>(e);
        if (isSymbol(car<value>(el)) && car<value>(el) == element && !isNil(cdr<value>(el)) && isSymbol(cadr<value>(el)) && elementHasChildren(el) && !elementHasValue(el)) {
            if (cadr<value>(el) == atom::feed)
                return httpd::writeResult(atom::writeATOMFeed(e), "application/atom+xml; charset=utf-8", r);
            if (cadr<value>(el) == atom::entry)
                return httpd::writeResult(atom::writeATOMEntry(e), "application/atom+xml; charset=utf-8", r);
        }
    }

    // Write any other compound value as a JSON value
    js::JSContext cx;
    return httpd::writeResult(json::writeJSON(e, cx), "application/json; charset=utf-8", r);
}

/**
 * Handle an HTTP POST.
 */
const failable<int> post(const list<value>& rpath, request_rec* r, const lambda<value(const list<value>&)>& impl) {
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
        js::JSContext cx;
        const list<value> json = elementsToValues(content(json::readJSON(ls, cx)));
        const list<list<value> > args = httpd::postArgs(json);

        // Extract the request id, method and params
        const value id = cadr(assoc(value("id"), args));
        const value func = c_str(json::funcName(cadr(assoc(value("method"), args))));
        const list<value> params = (list<value>)cadr(assoc(value("params"), args));

        // Evaluate the request expression
        const failable<value> val = failableResult(impl(cons<value>(func, params)));
        if (!hasContent(val))
            return mkfailure<int>(reason(val));

        // Return JSON result
        return httpd::writeResult(json::jsonResult(id, content(val), cx), "application/json-rpc; charset=utf-8", r);
    }

    // Evaluate an ATOM POST request and return the location of the corresponding created resource
    if (contains(ct, "application/atom+xml")) {

        // Read the ATOM entry
        const int rc = httpd::setupReadPolicy(r);
        if(rc != OK)
            return rc;
        const list<string> ls = httpd::read(r);
        debug(ls, "modeval::post::input");
        const value entry = elementsToValues(content(atom::readATOMEntry(ls)));

        // Evaluate the POST expression
        const failable<value> val = failableResult(impl(cons<value>("post", mklist<value>(cddr(rpath), entry))));
        if (!hasContent(val))
            return mkfailure<int>(reason(val));

        // Return the created resource location
        debug(content(val), "modeval::post::location");
        apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, c_str(httpd::url(r->uri, content(val), r))));
        r->status = HTTP_CREATED;
        return OK;
    }

    // Unknown content type, wrap the HTTP request struct in a value and pass it to
    // the component implementation function
    const failable<value> val = failableResult(impl(cons<value>("handle", mklist<value>(httpd::requestValue(r)))));
    if (!hasContent(val))
        return mkfailure<int>(reason(val));
    return (int)content(val);
}

/**
 * Handle an HTTP PUT.
 */
const failable<int> put(const list<value>& rpath, request_rec* r, const lambda<value(const list<value>&)>& impl) {
    debug(r->uri, "modeval::put::uri");

    // Read the ATOM entry
    const int rc = httpd::setupReadPolicy(r);
    if(rc != OK)
        return rc;
    const list<string> ls = httpd::read(r);
    debug(ls, "modeval::put::input");
    const value entry = elementsToValues(content(atom::readATOMEntry(ls)));

    // Evaluate the PUT expression and update the corresponding resource
    const failable<value> val = failableResult(impl(cons<value>("put", mklist<value>(cddr(rpath), entry))));
    if (!hasContent(val))
        return mkfailure<int>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Handle an HTTP DELETE.
 */
const failable<int> del(const list<value>& rpath, request_rec* r, const lambda<value(const list<value>&)>& impl) {
    debug(r->uri, "modeval::delete::uri");

    // Evaluate an ATOM delete request
    const failable<value> val = failableResult(impl(cons<value>("delete", mklist<value>(cddr(rpath)))));
    if (!hasContent(val))
        return mkfailure<int>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Route a /references/component-name/reference-name request,
 * to the target of the component reference.
 */
int translateReference(const ServerConf& sc, request_rec *r, const list<value>& rpath, const list<value>& apath) {
    httpdDebugRequest(r, "modeval::translateReference::input");
    debug(r->uri, "modeval::translateReference::uri");
    debug(apath, "modeval::translateReference::apath");
    debug(rpath, "modeval::translateReference::rpath");

    // Find the requested component
    if (isNil(cdr(rpath)))
        return HTTP_NOT_FOUND;
    const list<value> comp(assoctree(cadr(rpath), sc.references));
    if (isNil(comp))
        return HTTP_NOT_FOUND;

    // Find the requested reference and target configuration
    const list<value> ref(assoctree<value>(caddr(rpath), cadr(comp)));
    if (isNil(ref))
        return HTTP_NOT_FOUND;
    const string target(cadr(ref));
    debug(target, "modeval::translateReference::target");

    // Route to an absolute target URI using mod_proxy or an HTTP client redirect
    const list<value> pathInfo = cdddr(rpath);
    if (http::isAbsolute(target)) {
        if (useModProxy) {
            // Build proxy URI
            string turi = target + path(pathInfo) + (r->args != NULL? string("?") + r->args : string(""));
            const string proxy(string("proxy:") + turi);
            debug(proxy, "modeval::translateReference::proxy");
            r->filename = apr_pstrdup(r->pool, c_str(proxy));
            r->proxyreq = PROXYREQ_REVERSE;
            r->handler = "proxy-server";
            apr_table_setn(r->notes, "proxy-nocanon", "1");
            return OK;
        }

        debug(target, "modeval::translateReference::location");
        r->handler = "mod_tuscany_eval";
        return httpd::externalRedirect(target, r);
    }

    // Route to a relative target URI using a local internal redirect
    // /components/, target component name and request path info
    const value tname = substr(target, 0, find(target, '/'));
    const string redir = path(append(apath, cons<value>(string("c"), cons(tname, pathInfo))));
    debug(redir, "modeval::translateReference::redirect");
    r->uri = apr_pstrdup(r->pool, c_str(redir));
    r->handler = "mod_tuscany_eval";
    return OK;
}

/**
 * Find a leaf matching a request path in a tree of URI paths.
 */
const int matchPath(const list<value>& k, const list<value>& p) {
    if (isNil(p))
        return true;
    if (isNil(k))
        return false;
    if (car(k) != car(p))
        return false;
    return matchPath(cdr(k), cdr(p));
}

const list<value> assocPath(const value& k, const list<value>& tree) {
    if (isNil(tree))
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
int translateService(const ServerConf& sc, request_rec *r, const list<value>& rpath, const list<value>& apath) {
    httpdDebugRequest(r, "modeval::translateService::input");
    debug(r->uri, "modeval::translateService::uri");

    // Find the requested component
    debug(sc.services, "modeval::translateService::services");
    const list<value> svc(assocPath(rpath, sc.services));
    if (isNil(svc))
        return DECLINED;
    debug(svc, "modeval::translateService::service");

    // Build a component-name + path-info URI
    const list<value> target(append(apath, cons<value>(string("c"), cons<value>(cadr(svc), httpd::pathInfo(rpath, car(svc))))));
    debug(target, "modeval::translateService::target");

    // Dispatch to the target component using a local internal redirect
    const string redir(path(target));
    debug(redir, "modeval::translateService::redirect");
    r->uri = apr_pstrdup(r->pool, c_str(redir));
    r->handler = "mod_tuscany_eval";
    return OK;
}

/**
 * Translate a request to the target app and component.
 */
const int translateRequest(const ServerConf& sc, request_rec* r, const list<value>& rpath, const list<value>& apath) {
    debug(apath, "modeval::translateRequest::apath");
    debug(rpath, "modeval::translateRequest::rpath");
    if (isNil(apath) && isNil(rpath))
        return DECLINED;

    if (!isNil(rpath)) {

        // If the request is targeting a virtual host, use the corresponding
        // virtual host configuration
        if (isNil(apath)) {
            if (hasVhostDomainConf(sc) && hasVhostContributionConf(sc) && httpd::isVhostRequest(sc.server, sc.vhostDomain, r)) {
                const string aname = httpd::hostName(r);
                ServerConf vsc(r->pool, sc, aname);
                if (!hasContent(vhostConfig(vsc, sc, r)))
                    return DECLINED;
                return translateRequest(vsc, r, rpath, mklist<value>(aname));
            }
        }

        // Let default handler handle a resource request
        const value prefix = car(rpath);
        if (prefix == string("vhosts") || prefix == string("v"))
            return DECLINED;

        // Let our handler handle a component request
        if (prefix == string("components") || prefix == string("c")) {
            r->handler = "mod_tuscany_eval";
            return OK;
        }

        // Translate a component reference request
        if (prefix == string("references") || prefix == string("r"))
            return translateReference(sc, r, rpath, apath);

        // Attempt to translate the request to a service request
        if (translateService(sc, r, rpath, apath) == OK)
            return OK;

        // Attempt to map the request to an actual file
        if (isNil(apath)) {
            const failable<request_rec*, int> fnr = httpd::internalSubRequest(r->uri, r);
            if (!hasContent(fnr))
                return HTTP_INTERNAL_SERVER_ERROR;
            request_rec* nr = content(fnr);
            nr->uri = r->filename;
            const int tr = ap_core_translate(nr);
            if (tr != OK)
                return tr;
            if (ap_directory_walk(nr) == OK && ap_file_walk(nr) == OK && nr->finfo.filetype != APR_NOFILE) {
                debug(nr->filename, "modeval::translateRequest::file");
                return DECLINED;
            }

            // If the request is targeting a virtual app, use the corresponding
            // virtual host configuration
            if (hasVhostContributionConf(sc)) {
                const string cp = sc.vhostContributionPath + string(prefix) + "/" + sc.vhostCompositeName;
                ServerConf vsc(r->pool, sc, string(prefix));
                if (!hasContent(vhostConfig(vsc, sc, r)))
                    return DECLINED;
                return translateRequest(vsc, r, cdr(rpath), mklist<value>(car(rpath)));
            }
        }
    }

    // If we're in a virtual app and the request didn't match a service,
    // reference or component, redirect it to /v/<uri>. This will allow
    // mapping to the actual app resources using HTTPD aliases.
    if (!isNil(apath)) {
        if (isNil(rpath) && r->uri[strlen(r->uri) -1] != '/') {

            // Make sure a document root request ends with a '/', using
            // an external redirect
            const string target = string(r->uri) + string("/") + (r->args != NULL? string("?") + r->args : string(""));;
            debug(target, "modeval::translateRequest::location");
            r->handler = "mod_tuscany_eval";
            return httpd::externalRedirect(target, r);
        }

        // Do an internal redirect to /v/<uri>
        const string redir = string("/redirect:") + string("/v") + string(r->uri);
        debug(redir, "modeval::translateRequest::redirect");
        r->filename = apr_pstrdup(r->pool, c_str(redir));
        r->handler = "mod_tuscany_eval";
        return OK;
    }

    return DECLINED;
}

/**
 * Translate a request.
 */
int translate(request_rec *r) {
    if(r->method_number != M_GET && r->method_number != M_POST && r->method_number != M_PUT && r->method_number != M_DELETE)
        return DECLINED;

    // Create a scoped memory pool
    gc_scoped_pool pool(r->pool);

    httpdDebugRequest(r, "modeval::translate::input");

    // Get the server configuration
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_eval);

    // Translate the request
    return translateRequest(sc, r, pathValues(r->uri), list<value>());
}

/**
 * Handle a component request.
 */
const int handleRequest(const ServerConf& sc, const list<value>& rpath, request_rec *r) {
    debug(rpath, "modeval::handleRequest::path");
    if (isNil(cdr(rpath)))
        return HTTP_NOT_FOUND;

    if (hasVhostContributionConf(sc)) {

        // Handle a request targeting a component in a virtual host
        if (hasVhostDomainConf(sc) && httpd::isVhostRequest(sc.server, sc.vhostDomain, r)) {

            // Determine the app name from the host sub-domain name, and
            // store it in a request note
            const string app = http::subDomain(httpd::hostName(r));
            apr_table_setn(r->notes, "X-Request-AppName", apr_pstrdup(r->pool, c_str(app)));
            ServerConf vsc(r->pool, sc, app);
            if (!hasContent(vhostStart(vsc, sc, r)))
                return HTTP_INTERNAL_SERVER_ERROR;
            const int rc = handleRequest(vsc, rpath, r);
            vhostStop(vsc, sc);
            return rc;
        }

        // Handle a request targeting a component in a virtual app
        const value c = car(rpath);
        if (c != string("components") && c != string("c")) {

            // Determine the app name from the request URI path and
            // store it in a request note
            const string app = string(c);
            const string cp = sc.vhostContributionPath + app + "/" + sc.vhostCompositeName;
            apr_table_setn(r->notes, "X-Request-AppName", apr_pstrdup(r->pool, c_str(app)));

            ServerConf vsc(r->pool, sc, app);
            if (!hasContent(vhostStart(vsc, sc, r)))
                return HTTP_INTERNAL_SERVER_ERROR;
            const int rc = handleRequest(vsc, cdr(rpath), r);
            vhostStop(vsc, sc);
            return rc;
        }
    }

    // Store the request uri path in a request note
    apr_table_setn(r->notes, "X-Request-URI", apr_pstrdup(r->pool, c_str(path(rpath))));

    // Get the component implementation lambda
    const list<value> impl(assoctree<value>(cadr(rpath), sc.implTree));
    if (isNil(impl)) {
        mkfailure<int>(string("Couldn't find component implementation: ") + cadr(rpath));
        return HTTP_NOT_FOUND;
    }

    // Handle HTTP method
    const lambda<value(const list<value>&)> l(cadr<value>(impl));
    if (r->header_only)
         return OK;
    if(r->method_number == M_GET)
        return httpd::reportStatus(get(rpath, r, l));
    if(r->method_number == M_POST)
        return httpd::reportStatus(post(rpath, r, l));
    if(r->method_number == M_PUT)
        return httpd::reportStatus(put(rpath, r, l));
    if(r->method_number == M_DELETE)
        return httpd::reportStatus(del(rpath, r, l));
    return HTTP_NOT_IMPLEMENTED;
}

/**
 * HTTP request handler.
 */
int handler(request_rec *r) {
    if(r->method_number != M_GET && r->method_number != M_POST && r->method_number != M_PUT && r->method_number != M_DELETE)
        return DECLINED;
    if(strcmp(r->handler, "mod_tuscany_eval"))
        return DECLINED;

    // Nothing to do for an external redirect
    if (r->status == HTTP_MOVED_TEMPORARILY)
        return OK;

    // Handle an internal redirect as directed by the translate hook
    if (r->filename != NULL && !strncmp(r->filename, "/redirect:", 10)) {
        if (r->args == NULL)
            return httpd::internalRedirect(httpd::redirectURI(string(r->filename + 10), string(r->path_info)), r);
        return httpd::internalRedirect(httpd::redirectURI(string(r->filename + 10), string(r->path_info), string(r->args)), r);
    }


    // Create a scope for the current request
    ScopedRequest sr(r);

    // Get the server configuration
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_eval);

    // Handle the request
    return handleRequest(sc, pathValues(r->uri), r);
}

/**
 * Cleanup callback, called when the server is stopped or restarted.
 */
apr_status_t serverCleanup(void* v) {
    gc_pool pool;
    ServerConf& sc = *(ServerConf*)v;
    debug("modeval::serverCleanup");

    // Stop the component implementations
    applyLifecycleExpr(sc.implementations, mklist<value>("stop"));

    // Call the module lifecycle function
    if (isNil(sc.lifecycle))
        return APR_SUCCESS;
    debug("modeval::serverCleanup::stop");
    sc.lifecycle(mklist<value>("stop"));

    return APR_SUCCESS;
}

/**
 * Called after all the configuration commands have been run.
 * Process the server configuration and configure the deployed components.
 */
const int postConfigMerge(const ServerConf& mainsc, server_rec* s) {
    if (s == NULL)
        return OK;
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_eval);
    debug(httpd::serverName(s), "modeval::postConfigMerge::serverName");
    sc.lifecycle = mainsc.lifecycle;
    sc.vhostName = mainsc.vhostName;
    sc.contributionPath = mainsc.contributionPath;
    sc.compositeName = mainsc.compositeName;
    sc.vhostDomain = mainsc.vhostDomain;
    sc.vhostContributionPath = mainsc.vhostContributionPath;
    sc.vhostCompositeName = mainsc.vhostCompositeName;
    sc.vhostContributorName = mainsc.vhostContributorName;
    if (sc.ca == "") sc.ca = mainsc.ca;
    if (sc.cert == "") sc.cert = mainsc.cert;
    if (sc.key == "") sc.key = mainsc.key;
    sc.references = mainsc.references;
    sc.services = mainsc.services;
    sc.implementations = mainsc.implementations;
    sc.implTree = mainsc.implTree;
    sc.vhostContributor = mainsc.vhostContributor;
    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t *p, unused apr_pool_t *plog, unused apr_pool_t *ptemp, server_rec *s) {
    extern const value applyLifecycle(const list<value>&);

    gc_scoped_pool pool(p);

    // Get the server configuration and determine the server name
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_eval);
    debug(httpd::serverName(s), "modeval::postConfig::serverName");

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
        if (chdir(c_str(sc.contributionPath)) != 0) {
            mkfailure<bool>(string("Couldn't chdir to the deployed contribution: ") + sc.contributionPath);
            return -1;
        }

        debug("modeval::postConfig::start");
        const failable<value> r = failableResult(applyLifecycle(mklist<value>("start")));
        if (!hasContent(r))
            return -1;
        debug("modeval::postConfig::setlifecycle");
        sc.lifecycle = content(r);
    }
    if (count > 1) {
        debug("modeval::postConfig::restart");
        const failable<value> r = failableResult(applyLifecycle(mklist<value>("restart")));
        if (!hasContent(r))
            return -1;
        debug("modeval::postConfig::setlifecycle");
        sc.lifecycle = content(r);
    }

    // Configure the deployed components
    const failable<bool> res = confComponents(sc);
    if (!hasContent(res)) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << endl;
        return -1;
    }

    // Register a cleanup callback, called when the server is stopped or restarted
    apr_pool_pre_cleanup_register(p, (void*)&sc, serverCleanup);

    // Merge the configuration into the virtual hosts
    return postConfigMerge(sc, s->next);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* p, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf* psc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_eval);
    if(psc == NULL) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
    ServerConf& sc = *psc;

    // Start the components in the child process
    const failable<bool> res = startComponents(sc);
    if (!hasContent(res)) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
    
    // Store the implementation lambda functions in a tree for fast retrieval
    sc.implTree = mkbtree(sort(sc.implementations));

    // Create a proxy for the vhost contributor if needed
    if (length(sc.vhostContributorName) != 0) 
        sc.vhostContributor = mkimplProxy(sc, sc.vhostContributorName);

    // Merge the updated configuration into the virtual hosts
    postConfigMerge(sc, s->next);

    // Register a cleanup callback, called when the child is stopped or restarted
    apr_pool_pre_cleanup_register(p, (void*)psc, serverCleanup);
}

/**
 * Configuration commands.
 */
const char* confContribution(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.contributionPath = arg;
    return NULL;
}
const char* confComposite(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.compositeName = arg;
    return NULL;
}
const char* confVirtualDomain(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostDomain = arg;
    return NULL;
}
const char* confVirtualContribution(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostContributionPath = arg;
    return NULL;
}
const char* confVirtualContributor(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostContributorName = arg;
    return NULL;
}
const char* confVirtualComposite(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.vhostCompositeName = arg;
    return NULL;
}
const char* confCAFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.ca = arg;
    return NULL;
}
const char* confCertFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.cert = arg;
    return NULL;
}
const char* confCertKeyFile(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.key = arg;
    return NULL;
}
const char* confEnv(unused cmd_parms *cmd, unused void *c, const char *name, const char *value) {
    gc_scoped_pool pool(cmd->pool);
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
    AP_INIT_TAKE12("SCASetEnv", (const char*(*)())confEnv, NULL, OR_FILEINFO, "Environment variable name and optional value"),
    AP_INIT_TAKE1("SCAWiringSSLCACertificateFile", (const char*(*)())confCAFile, NULL, RSRC_CONF, "SCA wiring SSL CA certificate file"),
    AP_INIT_TAKE1("SCAWiringSSLCertificateFile", (const char*(*)())confCertFile, NULL, RSRC_CONF, "SCA wiring SSL certificate file"),
    AP_INIT_TAKE1("SCAWiringSSLCertificateKeyFile", (const char*(*)())confCertKeyFile, NULL, RSRC_CONF, "SCA wiring SSL certificate key file"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};


void registerHooks(unused apr_pool_t *p) {
    ap_hook_post_config(postConfig, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(childInit, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_translate_name(translate, NULL, NULL, APR_HOOK_LAST);
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
