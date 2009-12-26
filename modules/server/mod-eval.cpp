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

/**
 * HTTPD module used to eval component implementations.
 */

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "function.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "slist.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "../atom/atom.hpp"
#include "../json/json.hpp"
#include "../scdl/scdl.hpp"
#include "../http/curl.hpp"
#include "../http/httpd.hpp"
#include "mod-scm.hpp"
#include "mod-cpp.hpp"

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
    ServerConf(server_rec* s) : s(s), home(""), wiringServerName("") {
    }
    const server_rec* s;
    std::string home;
    std::string wiringServerName;
};

/**
 * Directory configuration.
 */
class DirConf {
public:
    DirConf(char* dirspec) : dirspec(dirspec), contributionPath(""), compositeName("") {
    }
    const char* dirspec;
    std::string contributionPath;
    std::string compositeName;
    list<value> implementations;
};

/**
 * Convert a result represented as a content + failure pair to a
 * failable monad.
 */
const failable<value, std::string> failableResult(const list<value>& v) {
    if (isNil(cdr(v)))
        return car(v);
    return mkfailure<value, std::string>(cadr(v));
}

/**
 * Handle an HTTP GET.
 */
const failable<int, std::string> get(request_rec* r, const lambda<value(const list<value>&)>& impl) {
    debug(r->uri, "modeval::get::uri");

    // Inspect the query string
    const list<list<value> > args = httpd::queryArgs(r);
    const list<value> ia = assoc(value("id"), args);
    const list<value> ma = assoc(value("method"), args);

    // Evaluate a JSON-RPC request and return a JSON result
    if (!isNil(ia) && !isNil(ma)) {

        // Extract the request id, method and params
        const value id = cadr(ia);
        const value func = std::string(cadr(ma)).c_str();

        // Apply the requested function
        const failable<value, std::string> val = failableResult(impl(cons(func, httpd::queryParams(args))));
        if (!hasContent(val))
            return mkfailure<int, std::string>(reason(val));

        // Return JSON result
        json::JSONContext cx;
        return httpd::writeResult(json::jsonResult(id, content(val), cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM GET request and return an ATOM feed
    const list<value> path(httpd::path(r->uri));
    if (isNil(cddr(path))) {
        const failable<value, std::string> val = failableResult(impl(cons<value>("getall", list<value>())));
        if (!hasContent(val))
            return mkfailure<int, std::string>(reason(val));
        return httpd::writeResult(atom::writeATOMFeed(atom::feedValuesToElements(content(val))), "application/atom+xml;type=feed", r);
    }

    // Evaluate an ATOM GET and return an ATOM entry
    const failable<value, std::string> val = failableResult(impl(cons<value>("get", mklist<value>(caddr(path)))));
    if (!hasContent(val))
        return mkfailure<int, std::string>(reason(val));
    return httpd::writeResult(atom::writeATOMEntry(atom::entryValuesToElements(content(val))), "application/atom+xml;type=entry", r);
}

/**
 * Handle an HTTP POST.
 */
const failable<int, std::string> post(request_rec* r, const lambda<value(const list<value>&)>& impl) {
    const list<std::string> ls = httpd::read(r);
    debug(r->uri, "modeval::post::url");
    debug(ls, "modeval::post::input");

    // Evaluate a JSON-RPC request and return a JSON result
    const std::string ct = httpd::contentType(r);
    if (ct.find("application/json-rpc") != std::string::npos || ct.find("text/plain") != std::string::npos) {
        json::JSONContext cx;
        const list<value> json = elementsToValues(content(json::readJSON(ls, cx)));
        const list<list<value> > args = httpd::postArgs(json);

        // Extract the request id, method and params
        const value id = cadr(assoc(value("id"), args));
        const value func = std::string(cadr(assoc(value("method"), args))).c_str();
        const list<value> params = (list<value>)cadr(assoc(value("params"), args));

        // Evaluate the request expression
        const failable<value, std::string> val = failableResult(impl(cons<value>(func, params)));
        if (!hasContent(val))
            return mkfailure<int, std::string>(reason(val));

        // Return JSON result
        return httpd::writeResult(json::jsonResult(id, content(val), cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM POST request and return the created resource location
    if (ct.find("application/atom+xml") != std::string::npos) {

        // Evaluate the request expression
        const value entry = atom::entryValue(content(atom::readEntry(ls)));
        const failable<value, std::string> val = failableResult(impl(cons<value>("post", mklist<value>(entry))));
        if (!hasContent(val))
            return mkfailure<int, std::string>(reason(val));

        // Return the created resource location
        apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, httpd::url(content(val), r)));
        r->status = HTTP_CREATED;
        return OK;
    }

    return HTTP_NOT_IMPLEMENTED;
}

/**
 * Handle an HTTP PUT.
 */
const failable<int, std::string> put(request_rec* r, const lambda<value(const list<value>&)>& impl) {
    const list<std::string> ls = httpd::read(r);
    debug(r->uri, "modeval::put::url");
    debug(ls, "modeval::put::input");

    // Evaluate an ATOM PUT request
    const list<value> path(httpd::path(r->uri));
    const value entry = atom::entryValue(content(atom::readEntry(ls)));
    const failable<value, std::string> val = failableResult(impl(cons<value>("put", mklist<value>(caddr(path), entry))));
    if (!hasContent(val))
        return mkfailure<int, std::string>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Handle an HTTP DELETE.
 */
const failable<int, std::string> del(request_rec* r, const lambda<value(const list<value>&)>& impl) {
    debug(r->uri, "modeval::delete::url");

    // Evaluate an ATOM delete request
    const list<value> path(httpd::path(r->uri));
    const failable<value, std::string> val = failableResult(impl(cons<value>("delete", mklist<value>(caddr(path)))));
    if (!hasContent(val))
        return mkfailure<int, std::string>(reason(val));
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
 * HTTP request handler.
 */
int handler(request_rec *r) {
    if(strcmp(r->handler, "mod_tuscany_eval"))
        return DECLINED;
    httpdDebugRequest(r, "modeval::handler::input");

    // Set up the read policy
    const int rc = httpd::setupReadPolicy(r);
    if(rc != OK)
        return rc;

    // Get the component implementation lambda
    DirConf& dc = httpd::dirConf<DirConf>(r, &mod_tuscany_eval);
    const list<value> path(httpd::path(r->uri));
    const list<value> impl(assoctree<value>(cadr(path), dc.implementations));
    if (isNil(impl))
        return HTTP_NOT_FOUND;

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
const value mkproxy(const value& ref, const std::string& base) {
    return lambda<value(const list<value>&)>(http::proxy(base + std::string(scdl::name(ref))));
}

const list<value> proxies(const list<value>& refs, const std::string& base) {
    if (isNil(refs))
        return refs;
    return cons(mkproxy(car(refs), base), proxies(cdr(refs), base));
}

/**
 * Return a configured component implementation.
 * For now only Scheme and C++ implementations are supported.
 */
const failable<lambda<value(const list<value>&)>, std::string> readImplementation(const std::string& itype, const std::string& path, const list<value>& px) {
    if (itype.find(".scheme") != std::string::npos)
        return scm::readImplementation(path, px);
    if (itype.find(".cpp") != std::string::npos)
        return cpp::readImplementation(path, px);
    return mkfailure<lambda<value(const list<value>&)>, std::string>("Unsupported implementation type: " + itype);
}

const value confImplementation(DirConf& dc, ServerConf& sc, server_rec& server, const value& comp) {
    const value impl = scdl::implementation(comp);
    const std::string path = dc.contributionPath + std::string(scdl::uri(impl));

    // Convert component references to configured proxy lambdas
    std::ostringstream base;
    if (sc.wiringServerName == "")
        base << (server.server_scheme == NULL? "http" : server.server_scheme)
            << "://" << (server.server_hostname == NULL? "localhost" : server.server_hostname)
            << ":" << (server.port == 0? 80 : server.port)
            << "/references/" << std::string(scdl::name(comp)) << "/";
    else
        base << sc.wiringServerName << "/references/" << std::string(scdl::name(comp)) << "/";
    const list<value> px(proxies(scdl::references(comp), base.str()));

    // Read and configure the implementation
    const failable<lambda<value(const list<value>&)>, std::string> cimpl(readImplementation(elementName(impl), path, px));
    if (!hasContent(cimpl))
        return reason(cimpl);
    return content(cimpl);
}

/**
 * Return a tree of component-name + configured-implementation pairs.
 */
const list<value> componentToImplementationAssoc(DirConf& dc, ServerConf& sc, server_rec& server, const list<value>& c) {
    if (isNil(c))
        return c;
    return cons<value>(mklist<value>(scdl::name(car(c)), confImplementation(dc, sc, server, car(c))), componentToImplementationAssoc(dc, sc, server, cdr(c)));
}

const list<value> componentToImplementationTree(DirConf& dc, ServerConf& sc, server_rec& server, const list<value>& c) {
    return mkbtree(sort(componentToImplementationAssoc(dc, sc, server, c)));
}

/**
 * Read the components declared in a composite.
 */
const failable<list<value>, std::string> readComponents(const std::string& path) {
    std::ifstream is(path);
    if (is.fail() || is.bad())
        return mkfailure<list<value>, std::string>("Could not read composite: " + path);
    return scdl::components(readXML(streamList(is)));
}

/**
 * Configure the components declared in the deployed composite.
 */
const bool confComponents(DirConf& dc, ServerConf& sc, server_rec& server) {
    if (dc.contributionPath == "" || dc.compositeName == "")
        return true;
    const failable<list<value>, std::string> comps = readComponents(dc.contributionPath + dc.compositeName);
    if (!hasContent(comps))
        return true;
    dc.implementations = componentToImplementationTree(dc, sc, server, content(comps));
    debug(dc.implementations, "modeval::confComponents::implementations");
    return true;
}

/**
 * Configuration commands.
 */
const char *confHome(cmd_parms *cmd, unused void *c, const char *arg) {
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.home = arg;
    return NULL;
}
const char *confWiringServerName(cmd_parms *cmd, unused void *c, const char *arg) {
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    sc.wiringServerName = arg;
    return NULL;
}
const char *confContribution(cmd_parms *cmd, void *c, const char *arg) {
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    DirConf& dc = *(DirConf*)c;
    dc.contributionPath = arg;
    confComponents(dc, sc, *cmd->server);
    return NULL;
}
const char *confComposite(cmd_parms *cmd, void *c, const char *arg) {
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_eval);
    DirConf& dc = *(DirConf*)c;
    dc.compositeName = arg;
    confComponents(dc, sc, *cmd->server);
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE1("TuscanyHome", (const char*(*)())confHome, NULL, RSRC_CONF, "Tuscany home directory"),
    AP_INIT_TAKE1("SCAWiringServerName", (const char*(*)())confWiringServerName, NULL, RSRC_CONF, "SCA wiring server name"),
    AP_INIT_TAKE1("SCAContribution", (const char*(*)())confContribution, NULL, ACCESS_CONF, "SCA contribution location"),
    AP_INIT_TAKE1("SCAComposite", (const char*(*)())confComposite, NULL, ACCESS_CONF, "SCA composite location"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};

int postConfig(unused apr_pool_t *p, unused apr_pool_t *plog, unused apr_pool_t *ptemp, unused server_rec *s) {
    return OK;
}

void childInit(unused apr_pool_t* p, server_rec* svr_rec) {
    ServerConf* c = (ServerConf*)ap_get_module_config(svr_rec->module_config, &mod_tuscany_eval);
    if(c == NULL) {
        std::cerr << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << std::endl;
        exit(APEXIT_CHILDFATAL);
    }
}

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
    tuscany::httpd::makeDirConf<tuscany::server::modeval::DirConf>, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::server::modeval::ServerConf>, NULL,
    // commands and hooks
    tuscany::server::modeval::commands, tuscany::server::modeval::registerHooks
};

}
