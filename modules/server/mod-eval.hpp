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

#include "string.hpp"
#include "stream.hpp"
#include "function.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
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
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf(apr_pool_t* p, server_rec* s) : p(p), server(s), wiringServerName(""), contributionPath(""), compositeName(""), virtualHostContributionPath(""), virtualHostCompositeName(""), ca(""), cert(""), key("") {
    }

    const gc_pool p;
    server_rec* server;
    lambda<value(const list<value>&)> lifecycle;
    string wiringServerName;
    string contributionPath;
    string compositeName;
    string virtualHostContributionPath;
    string virtualHostCompositeName;
    string ca;
    string cert;
    string key;
    list<value> implementations;
    list<value> implTree;
};

/**
 * Return true if a server contains a composite configuration.
 */
const bool hasCompositeConf(const ServerConf& sc) {
    return sc.contributionPath != "" && sc.compositeName != "";
}

/**
 * Return true if a server contains a virtual host composite configuration.
 */
const bool hasVirtualCompositeConf(const ServerConf& sc) {
    return sc.virtualHostContributionPath != "" && sc.virtualHostCompositeName != "";
}

/**
 * Convert a result represented as a content + failure pair to a
 * failable monad.
 */
const failable<value> failableResult(const list<value>& v) {
    if (isNil(cdr(v)))
        return car(v);
    return mkfailure<value>(string(cadr(v)));
}

/**
 * Handle an HTTP GET.
 */
const failable<int> get(request_rec* r, const lambda<value(const list<value>&)>& impl) {
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
        json::JSONContext cx;
        return httpd::writeResult(json::jsonResult(id, content(val), cx), "application/json-rpc", r);
    }

    // Evaluate the GET expression
    const list<value> path(pathValues(r->uri));
    const failable<value> val = failableResult(impl(cons<value>("get", mklist<value>(cddr(path)))));
    if (!hasContent(val))
        return mkfailure<int>(reason(val));
    const value c = content(val);

    // Write returned content-type / content-list pair
    if (isList(cadr<value>(c)))
        return httpd::writeResult(convertValues<string>(cadr<value>(c)), car<value>(c), r);

    // Write returned ATOM feed or entry
    if (isNil(cddr(path)))
        return httpd::writeResult(atom::writeATOMFeed(atom::feedValuesToElements(c)), "application/atom+xml;type=feed", r);
    else
        return httpd::writeResult(atom::writeATOMEntry(atom::entryValuesToElements(c)), "application/atom+xml;type=entry", r);
}

/**
 * Handle an HTTP POST.
 */
const failable<int> post(request_rec* r, const lambda<value(const list<value>&)>& impl) {
    debug(r->uri, "modeval::post::url");

    // Evaluate a JSON-RPC request and return a JSON result
    const string ct = httpd::contentType(r);
    if (contains(ct, "application/json-rpc") || contains(ct, "text/plain") || contains(ct, "application/x-www-form-urlencoded")) {

        // Read the JSON request
        const int rc = httpd::setupReadPolicy(r);
        if(rc != OK)
            return rc;
        const list<string> ls = httpd::read(r);
        debug(ls, "modeval::post::input");
        json::JSONContext cx;
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
        return httpd::writeResult(json::jsonResult(id, content(val), cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM POST request and return the location of the corresponding created resource
    if (contains(ct, "application/atom+xml")) {

        // Read the ATOM entry
        const list<value> path(pathValues(r->uri));
        const int rc = httpd::setupReadPolicy(r);
        if(rc != OK)
            return rc;
        const list<string> ls = httpd::read(r);
        debug(ls, "modeval::post::input");
        const value entry = atom::entryValue(content(atom::readATOMEntry(ls)));

        // Evaluate the POST expression
        const failable<value> val = failableResult(impl(cons<value>("post", mklist<value>(cddr(path), entry))));
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
const failable<int> put(request_rec* r, const lambda<value(const list<value>&)>& impl) {
    debug(r->uri, "modeval::put::url");

    // Read the ATOM entry
    const list<value> path(pathValues(r->uri));
    const int rc = httpd::setupReadPolicy(r);
    if(rc != OK)
        return rc;
    const list<string> ls = httpd::read(r);
    debug(ls, "modeval::put::input");
    const value entry = atom::entryValue(content(atom::readATOMEntry(ls)));

    // Evaluate the PUT expression and update the corresponding resource
    const failable<value> val = failableResult(impl(cons<value>("put", mklist<value>(cddr(path), entry))));
    if (!hasContent(val))
        return mkfailure<int>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Handle an HTTP DELETE.
 */
const failable<int> del(request_rec* r, const lambda<value(const list<value>&)>& impl) {
    debug(r->uri, "modeval::delete::url");

    // Evaluate an ATOM delete request
    const list<value> path(pathValues(r->uri));
    const failable<value> val = failableResult(impl(cons<value>("delete", mklist<value>(cddr(path)))));
    if (!hasContent(val))
        return mkfailure<int>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Translate a component request.
 */
int translate(request_rec *r) {
    if(r->method_number != M_GET && r->method_number != M_POST && r->method_number != M_PUT && r->method_number != M_DELETE)
        return DECLINED;
    if (strncmp(r->uri, "/components/", 12) != 0)
        return DECLINED;
    r->handler = "mod_tuscany_eval";
    return OK;
}

/**
 * Make an HTTP proxy lambda.
 */
const value mkhttpProxy(const ServerConf& sc, const string& ref, const string& base) {
    const string uri = base + ref;
    debug(uri, "modeval::mkhttpProxy::uri");
    return lambda<value(const list<value>&)>(http::proxy(uri, sc.ca, sc.cert, sc.key, sc.p));
}

/**
 * Return a component implementation proxy lambda.
 */
class implProxy {
public:
    implProxy(const ServerConf& sc, const value& name) : sc(sc), name(name) {
    }

    const value operator()(const list<value>& params) const {
        debug(params, "modeval::implProxy::input");

        // Lookup the component implementation
        const list<value> impl(assoctree<value>(name, sc.implTree));
        if (isNil(impl))
            return mkfailure<value>(string("Couldn't find component implementation: ") + name);

        // Call its lambda function
        const lambda<value(const list<value>&)> l(cadr<value>(impl));
        const value func = c_str(car(params));
        const failable<value> val = failableResult(l(cons(func, cdr(params))));
        debug(val, "modeval::implProxy::result");
        if (!hasContent(val))
            return value();
        return content(val);
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
 * Convert a list of component references to a list of proxy lambdas.
 */
const value mkrefProxy(const ServerConf& sc, const value& ref, const string& base) {
    const value target = scdl::target(ref);
    debug(ref, "modeval::mkrefProxy::ref");
    debug(target, "modeval::mkrefProxy::target");

    // Use an HTTP proxy or an internal proxy to the component implementation
    if (isNil(target) || httpd::isAbsolute(target))
        return mkhttpProxy(sc, scdl::name(ref), base);
    return mkimplProxy(sc, car(pathValues(target)));
}

const list<value> refProxies(const ServerConf& sc, const list<value>& refs, const string& base) {
    if (isNil(refs))
        return refs;
    return cons(mkrefProxy(sc, car(refs), base), refProxies(sc, cdr(refs), base));
}

/**
 * Store current HTTP request for access from property lambda functions.
 */
#ifdef WANT_THREADS
__thread
#endif
request_rec* currentRequest = NULL;

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
        return httpd::hostName(currentRequest, v);
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
    if (scdl::name(prop) == "host")
        return lambda<value(const list<value>&)>(hostPropProxy(elementValue(prop)));
    if (scdl::name(prop) == "user")
        return lambda<value(const list<value>&)>(userPropProxy(elementValue(prop)));
    if (scdl::name(prop) == "realm")
        return lambda<value(const list<value>&)>(envPropProxy("REALM", elementValue(prop)));
    if (scdl::name(prop) == "email")
        return lambda<value(const list<value>&)>(envPropProxy("EMAIL", elementValue(prop)));
    if (scdl::name(prop) == "nickname")
        return lambda<value(const list<value>&)>(envPropProxy("NICKNAME", elementValue(prop)));
    if (scdl::name(prop) == "fullname")
        return lambda<value(const list<value>&)>(envPropProxy("FULLNAME", elementValue(prop)));
    if (scdl::name(prop) == "firstname")
        return lambda<value(const list<value>&)>(envPropProxy("FIRSTNAME", elementValue(prop)));
    if (scdl::name(prop) == "lastname")
        return lambda<value(const list<value>&)>(envPropProxy("LASTNAME", elementValue(prop)));
    return lambda<value(const list<value>&)>(propProxy(elementValue(prop)));
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
    ostringstream base;
    base << sc.wiringServerName << "/references/" << string(scdl::name(comp)) << "/";
    const list<value> rpx(refProxies(sc, scdl::references(comp), str(base)));

    // Convert component proxies to configured proxy lambdas
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
 * Configure the components declared in the deployed composite.
 */
const failable<bool> confComponents(ServerConf& sc) {
    if (!hasCompositeConf(sc))
        return false;
    debug(sc.contributionPath, "modeval::confComponents::contributionPath");
    debug(sc.compositeName, "modeval::confComponents::compositeName");
    if (sc.ca != "") debug(sc.ca, "modeval::confComponents::sslCA");
    if (sc.cert != "") debug(sc.cert, "modeval::confComponents::sslCert");
    if (sc.key != "") debug(sc.key, "modeval::confComponents::sslKey");

    // Read the components and get their implementation lambda functions
    const failable<list<value> > comps = readComponents(sc.contributionPath + sc.compositeName);
    if (!hasContent(comps))
        return mkfailure<bool>(reason(comps));
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
 * Virtual host scoped server configuration.
 */
class VirtualHostConf {
public:
    VirtualHostConf(const gc_pool& p, const ServerConf& sc) : sc(sc), vsc(pool(p), sc.server) {
        vsc.virtualHostContributionPath = sc.virtualHostContributionPath;
        vsc.virtualHostCompositeName = sc.virtualHostCompositeName;
        vsc.ca = sc.ca;
        vsc.cert = sc.cert;
        vsc.key = sc.key;
    }

    ~VirtualHostConf() {
        extern const failable<bool> virtualHostCleanup(const ServerConf& vsc, const ServerConf& sc);
        virtualHostCleanup(vsc, sc);
    }

    const ServerConf& sc;
    ServerConf vsc;
};

/**
 * Configure and start the components deployed in a virtual host.
 */
const failable<bool> virtualHostConfig(ServerConf& vsc, const ServerConf& sc, request_rec* r) {

    // Determine the server name and wiring server name
    debug(httpd::serverName(vsc.server), "modeval::virtualHostConfig::serverName");
    debug(httpd::serverName(r), "modeval::virtualHostConfig::virtualHostName");
    vsc.wiringServerName = httpd::serverName(r);
    debug(vsc.wiringServerName, "modeval::virtualHostConfig::wiringServerName");
    debug(vsc.virtualHostContributionPath, "modwiring::virtualHostConfig::virtualHostContributionPath");

    // Resolve the configured virtual contribution under
    // the virtual host's SCA contribution root
    vsc.contributionPath = vsc.virtualHostContributionPath + httpd::subdomain(httpd::hostName(r)) + "/";
    vsc.compositeName = vsc.virtualHostCompositeName;

    // Chdir to the virtual host's contribution
    if (chdir(c_str(sc.contributionPath)) != 0)
        return mkfailure<bool>(string("Couldn't chdir to the deployed contribution: ") + sc.contributionPath);

    // Configure the deployed components
    const failable<bool> cr = confComponents(vsc);
    if (!hasContent(cr))
        return cr;

    // Start the configured components
    const failable<bool> sr = startComponents(vsc);
    if (!hasContent(sr))
        return sr;

    // Store the implementation lambda functions (from both the virtual host and the
    // main server) in a tree for fast retrieval
    vsc.implTree = mkbtree(sort(append(vsc.implementations, sc.implementations)));
    return true;
}

/**
 * Cleanup a virtual host.
 */
const failable<bool> virtualHostCleanup(const ServerConf& vsc, const ServerConf& sc) {
    if (!hasCompositeConf(vsc))
        return true;
    debug("modeval::virtualHostCleanup");

    // Stop the component implementations
    applyLifecycleExpr(vsc.implementations, mklist<value>("stop"));

    // Chdir back to the main server's contribution
    if (chdir(c_str(sc.contributionPath)) != 0)
        return mkfailure<bool>(string("Couldn't chdir to the deployed contribution: ") + sc.contributionPath);
    return true;
}

/**
 * HTTP request handler.
 */
int handler(request_rec *r) {
    if(r->method_number != M_GET && r->method_number != M_POST && r->method_number != M_PUT && r->method_number != M_DELETE)
        return DECLINED;
    if(strcmp(r->handler, "mod_tuscany_eval"))
        return DECLINED;

    gc_scoped_pool pool(r->pool);
    ScopedRequest sr(r);
    httpdDebugRequest(r, "modeval::handler::input");

    // Get the server configuration
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_eval);

    // Process dynamic virtual host configuration, if any
    VirtualHostConf vhc(gc_pool(r->pool), sc);
    const bool usevh = hasVirtualCompositeConf(vhc.vsc) && httpd::isVirtualHostRequest(sc.server, r);
    if (usevh) {
        const failable<bool> cr = virtualHostConfig(vhc.vsc, sc, r);
        if (!hasContent(cr))
            return httpd::reportStatus(mkfailure<int>(reason(cr)));
    }

    // Get the component implementation lambda
    const list<value> path(pathValues(r->uri));
    const list<value> impl(assoctree<value>(cadr(path), usevh? vhc.vsc.implTree : sc.implTree));
    if (isNil(impl))
        return httpd::reportStatus(mkfailure<int>(string("Couldn't find component implementation: ") + cadr(path)));

    // Handle HTTP method
    const lambda<value(const list<value>&)> l(cadr<value>(impl));
    if (r->header_only)
         return OK;
    if(r->method_number == M_GET)
        return httpd::reportStatus(get(r, l));
    if(r->method_number == M_POST)
        return httpd::reportStatus(post(r, l));
    if(r->method_number == M_PUT)
        return httpd::reportStatus(put(r, l));
    if(r->method_number == M_DELETE)
        return httpd::reportStatus(del(r, l));
    return HTTP_NOT_IMPLEMENTED;
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
    if (sc.wiringServerName == "")
        sc.wiringServerName = mainsc.wiringServerName != ""? mainsc.wiringServerName : httpd::serverName(s);
    debug(sc.wiringServerName, "modeval::postConfigMerge::wiringServerName");
    sc.contributionPath = mainsc.contributionPath;
    sc.compositeName = mainsc.compositeName;
    sc.virtualHostContributionPath = mainsc.virtualHostContributionPath;
    sc.virtualHostCompositeName = mainsc.virtualHostCompositeName;
    if (sc.ca == "") sc.ca = mainsc.ca;
    if (sc.cert == "") sc.cert = mainsc.cert;
    if (sc.key == "") sc.key = mainsc.key;
    sc.implementations = mainsc.implementations;
    sc.implTree = mainsc.implTree;
    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t *p, unused apr_pool_t *plog, unused apr_pool_t *ptemp, server_rec *s) {
    extern const value applyLifecycle(const list<value>&);

    gc_scoped_pool pool(p);

    // Get the server configuration and determine the wiring server name
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_eval);
    debug(httpd::serverName(s), "modeval::postConfig::serverName");
    if (sc.wiringServerName == "") sc.wiringServerName = httpd::serverName(s);
    debug(sc.wiringServerName, "modeval::postConfig::wiringServerName");

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
        sc.lifecycle = content(r);
    }
    if (count > 1) {
        debug("modeval::postConfig::restart");
        const failable<value> r = failableResult(applyLifecycle(mklist<value>("restart")));
        if (!hasContent(r))
            return -1;
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

    // Merge the updated configuration into the virtual hosts
    postConfigMerge(sc, s->next);

    // Register a cleanup callback, called when the child is stopped or restarted
    apr_pool_pre_cleanup_register(p, (void*)psc, serverCleanup);
}

/**
 * Configuration commands.
 */
const char* confWiringServerName(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.wiringServerName = arg;
    return NULL;
}
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
const char* confVirtualContribution(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.virtualHostContributionPath = arg;
    return NULL;
}
const char* confVirtualComposite(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.virtualHostCompositeName = arg;
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
    AP_INIT_TAKE1("SCAWiringServerName", (const char*(*)())confWiringServerName, NULL, RSRC_CONF, "SCA wiring server name"),
    AP_INIT_TAKE1("SCAContribution", (const char*(*)())confContribution, NULL, RSRC_CONF, "SCA contribution location"),
    AP_INIT_TAKE1("SCAComposite", (const char*(*)())confComposite, NULL, RSRC_CONF, "SCA composite location"),
    AP_INIT_TAKE1("SCAVirtualContribution", (const char*(*)())confVirtualContribution, NULL, RSRC_CONF, "SCA virtual host contribution location"),
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
    ap_hook_translate_name(translate, NULL, NULL, APR_HOOK_FIRST);
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
