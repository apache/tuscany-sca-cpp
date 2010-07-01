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
#include "../http/curl.hpp"
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
    ServerConf(server_rec* s) : s(s), wiringServerName(""), contributionPath(""), compositeName(""), ca(""), cert(""), key("") {
    }

    const server_rec* s;
    lambda<value(const list<value>&)> lifecycle;
    string wiringServerName;
    string contributionPath;
    string compositeName;
    string ca;
    string cert;
    string key;
    list<value> implementations;
    list<value> implTree;
};

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
        const failable<value> val = failableResult(impl(cons(func, httpd::queryParams(args))));
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
    if (contains(ct, "application/json-rpc") || contains(ct, "text/plain")) {

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
        apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, httpd::url(content(val), r)));
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
    if (strncmp(r->uri, "/components/", 12) != 0)
        return DECLINED;
    r->handler = "mod_tuscany_eval";
    return OK;
}

/**
 * Store current HTTP request for access from property lambda functions.
 */
#ifdef WANT_THREADS
__thread
#endif
const request_rec* currentRequest = NULL;

class scoped_request {
public:
    scoped_request(const request_rec* r) {
        currentRequest = r;
    }

    ~scoped_request() {
        currentRequest = NULL;
    }
};

/**
 * HTTP request handler.
 */
int handler(request_rec *r) {
    if(strcmp(r->handler, "mod_tuscany_eval"))
        return DECLINED;
    gc_scoped_pool pool(r->pool);
    scoped_request sr(r);
    httpdDebugRequest(r, "modeval::handler::input");

    // Get the component implementation lambda
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_eval);
    const list<value> path(pathValues(r->uri));
    const list<value> impl(assoctree<value>(cadr(path), sc.implTree));
    if (isNil(impl))
        return httpd::reportStatus(mkfailure<int>(string("Couldn't find component implementation")));

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
 * Convert a list of component references to a list of HTTP proxy lambdas.
 */
const value mkrefProxy(const value& ref, const string& base, const string& ca, const string& cert, const string& key) {
    return lambda<value(const list<value>&)>(http::proxy(base + string(scdl::name(ref)), ca, cert, key));
}

const list<value> refProxies(const list<value>& refs, const string& base, const string& ca, const string& cert, const string& key) {
    if (isNil(refs))
        return refs;
    return cons(mkrefProxy(car(refs), base, ca, cert, key), refProxies(cdr(refs), base, ca, cert, key));
}

/**
 * Convert a list of component properties to a list of lambda functions that just return
 * the property value. The user and email properties are configured with the values
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

struct emailPropProxy {
    const value v;
    emailPropProxy(const value& v) : v(v) {
    }
    const value operator()(unused const list<value>& params) const {
        const char* email = apr_table_get(currentRequest->subprocess_env, "EMAIL");
        if (email == NULL || *email == '\0')
            return v;
        return string(email);
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
    if (scdl::name(prop) == "email")
        return lambda<value(const list<value>&)>(emailPropProxy(elementValue(prop)));
    if (scdl::name(prop) == "user")
        return lambda<value(const list<value>&)>(userPropProxy(elementValue(prop)));
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

const value evalComponent(ServerConf& sc, server_rec& server, const value& comp) {
    extern const failable<lambda<value(const list<value>&)> > evalImplementation(const string& cpath, const value& impl, const list<value>& px, const lambda<value(const list<value>&)>& lifecycle);

    const value impl = scdl::implementation(comp);

    // Convert component references to configured proxy lambdas
    ostringstream base;
    if (sc.wiringServerName == "")
        base << (server.server_scheme == NULL? "http" : server.server_scheme)
            << "://" << (server.server_hostname == NULL? "localhost" : server.server_hostname)
            << ":" << (server.port == 0? 80 : server.port)
            << "/references/" << string(scdl::name(comp)) << "/";
    else
        base << sc.wiringServerName << "/references/" << string(scdl::name(comp)) << "/";
    const list<value> rpx(refProxies(scdl::references(comp), str(base), sc.ca, sc.cert, sc.key));

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
const list<value> componentToImplementationAssoc(ServerConf& sc, server_rec& server, const list<value>& c) {
    if (isNil(c))
        return c;
    return cons<value>(mklist<value>(scdl::name(car(c)), evalComponent(sc, server, car(c))), componentToImplementationAssoc(sc, server, cdr(c)));
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
const failable<bool> confComponents(ServerConf& sc, server_rec& server) {
    if (sc.contributionPath == "" || sc.compositeName == "")
        return false;

    // Chdir to the deployed contribution
    if (chdir(c_str(sc.contributionPath)) != 0)
        return mkfailure<bool>("Couldn't chdir to the deployed contribution");

    // Read the components and get their implementation lambda functions
    const failable<list<value> > comps = readComponents(sc.contributionPath + sc.compositeName);
    if (!hasContent(comps))
        return mkfailure<bool>(reason(comps));
    sc.implementations = componentToImplementationAssoc(sc, server, content(comps));
    debug(sc.implementations, "modeval::confComponents::implementations");

    // Store the implementation lambda functions in a tree for fast retrieval
    sc.implTree = mkbtree(sort(sc.implementations));
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

    // Store the implementation lambda functions in a tree for fast retrieval
    sc.implTree = mkbtree(sort(sc.implementations));
    return true;
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
    sc.wiringServerName = mainsc.wiringServerName;
    sc.contributionPath = mainsc.contributionPath;
    sc.compositeName = mainsc.compositeName;
    sc.ca = mainsc.ca;
    sc.cert = mainsc.cert;
    sc.key = mainsc.key;
    sc.implementations = mainsc.implementations;
    sc.implTree = mainsc.implTree;
    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t *p, unused apr_pool_t *plog, unused apr_pool_t *ptemp, server_rec *s) {
    extern const value applyLifecycle(const list<value>&);

    gc_scoped_pool pool(p);
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_eval);

    // Count the calls to post config
    const string k("tuscany::modeval::postConfig");
    const int count = (int)httpd::userData(k, s);
    httpd::putUserData(k, (void*)(count + 1), s);

    // Count == 0, do nothing as post config is always called twice,
    // count == 1 is the first start, count > 1 is a restart
    if (count == 0)
        return OK;
    if (count == 1) {
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
    debug(sc.wiringServerName, "modeval::postConfig::wiringServerName");
    debug(sc.contributionPath, "modeval::postConfig::contributionPath");
    debug(sc.compositeName, "modeval::postConfig::compositeName");
    const failable<bool> res = confComponents(sc, *s);
    if (!hasContent(res)) {
        cerr << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << endl;
        return -1;
    }

    // Register a cleanup callback, called when the server is stopped or restarted
    apr_pool_pre_cleanup_register(p, (void*)&sc, serverCleanup);

    // Merge the config into any virtual hosts
    return postConfigMerge(sc, s->next);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* p, server_rec* s) {
    gc_scoped_pool pool(p);
    ServerConf* sc = (ServerConf*)ap_get_module_config(s->module_config, &mod_tuscany_eval);
    if(sc == NULL) {
        cerr << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }

    // Start the components in the child process
    const failable<bool> res = startComponents(*sc);
    if (!hasContent(res)) {
        cerr << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }

    // Register a cleanup callback, called when the child is stopped or restarted
    apr_pool_pre_cleanup_register(p, (void*)sc, serverCleanup);
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
    AP_INIT_TAKE12("SCASetEnv", (const char*(*)())confEnv, NULL, OR_FILEINFO, "Environment variable name and optional value"),
    AP_INIT_TAKE1("SCASSLCACertificateFile", (const char*(*)())confCAFile, NULL, RSRC_CONF, "SSL CA certificate file"),
    AP_INIT_TAKE1("SCASSLCertificateFile", (const char*(*)())confCertFile, NULL, RSRC_CONF, "SSL certificate file"),
    AP_INIT_TAKE1("SCASSLCertificateKeyFile", (const char*(*)())confCertKeyFile, NULL, RSRC_CONF, "SSL certificate key file"),
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
