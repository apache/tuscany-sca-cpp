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
#include "slist.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "cache.hpp"
#include "../atom/atom.hpp"
#include "../json/json.hpp"
#include "../scdl/scdl.hpp"
#include "../http/curl.hpp"
#include "../http/httpd.hpp"
#include "mod-eval.hpp"
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
    ServerConf(server_rec* s) : s(s), home("") {
    }
    server_rec* s;
    std::string home;
};

/**
 * Port number used for wiring requests. Set it to zero to use the current
 * server port number, set it to a port number to direct wiring requests
 * to that port, for debugging or tracing for example.
 */
int debugWiringPort = 0;

/**
 * Directory configuration.
 */
class DirConf {
public:
    DirConf(char* dirspec) : dirspec(dirspec), contributionPath(""), compositeName(""), componentName(""), implementationPath("") {
    }
    char* dirspec;
    std::string contributionPath;
    std::string compositeName;
    std::string componentName;
    std::string implementationPath;
    cached<failable<value, std::string> > component;
    cached<failable<ilambda, std::string> > implementation;
};

/**
 * Handle an HTTP GET.
 */
const failable<int, std::string> get(request_rec* r, const ilambda& impl, const list<value>& px) {

    // Inspect the query string
    const list<list<value> > args = httpd::queryArgs(r);
    const list<value> ia = assoc(value("id"), args);
    const list<value> ma = assoc(value("method"), args);

    // Evaluate a JSON-RPC request and return a JSON result
    if (!isNil(ia) && !isNil(ma)) {

        // Extract the request id, method and params
        const value id = cadr(ia);
        const value func = std::string(cadr(ma)).c_str();
        const list<value> params = httpd::queryParams(args);

        // Apply the requested function
        const failable<value, std::string> val = impl(func, append(params, px));
        if (!hasContent(val))
            return mkfailure<int, std::string>(reason(val));

        // Return JSON result
        json::JSONContext cx;
        return httpd::writeResult(json::jsonResult(id, content(val), cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM GET request and return an ATOM feed
    const list<value> id(httpd::path(r->path_info));
    if (isNil(id)) {
        const failable<value, std::string> val = impl("getall", px);
        if (!hasContent(val))
            return mkfailure<int, std::string>(reason(val));
        return httpd::writeResult(atom::writeATOMFeed(atom::feedValuesToElements(content(val))), "application/atom+xml;type=feed", r);
    }

    // Evaluate an ATOM GET and return an ATOM entry
    const failable<value, std::string> val = impl("get", cons<value>(car(id), px));
    if (!hasContent(val))
        return mkfailure<int, std::string>(reason(val));
    return httpd::writeResult(atom::writeATOMEntry(atom::entryValuesToElements(content(val))), "application/atom+xml;type=entry", r);
}

/**
 * Handle an HTTP POST.
 */
const failable<int, std::string> post(request_rec* r, const ilambda& impl, const list<value>& px) {
    const list<std::string> ls = httpd::read(r);
    httpd::logStrings(ls, "content");

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
        const failable<value, std::string> val = impl(func, append(params, px));
        if (!hasContent(val))
            return mkfailure<int, std::string>(reason(val));

        // Return JSON result
        return httpd::writeResult(json::jsonResult(id, content(val), cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM POST request and return the created resource location
    if (ct.find("application/atom+xml") != std::string::npos) {

        // Evaluate the request expression
        const value entry = httpd::feedEntry(content(atom::readEntry(ls)));
        const failable<value, std::string> val = impl("post", cons<value>(entry, px));
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
const failable<int, std::string> put(request_rec* r, const ilambda& impl, const list<value>& px) {
    const list<std::string> ls = httpd::read(r);
    httpd::logStrings(ls, "content");

    // Evaluate an ATOM PUT request
    const list<value> id(httpd::path(r->path_info));
    const value entry = httpd::feedEntry(content(atom::readEntry(ls)));
    const failable<value, std::string> val = impl("put", append(mklist<value>(entry, car(id)), px));
    if (!hasContent(val))
        return mkfailure<int, std::string>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Handle an HTTP DELETE.
 */
const failable<int, std::string> del(request_rec* r, const ilambda& impl, const list<value>& px) {

    // Evaluate an ATOM delete request
    const list<value> id(httpd::path(r->path_info));
    const failable<value, std::string> val = impl("delete", cons<value>(car(id), px));
    if (!hasContent(val))
        return mkfailure<int, std::string>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Read the SCDL configuration of a component.
 */
const failable<value, std::string> readComponent(const std::string& path, const std::string& name) {

    // Read composite
    std::ifstream is(path);
    if (is.fail() || is.bad())
        return mkfailure<value, std::string>("Could not read composite: " + path);

    // Return the component
    const list<value> c = scdl::components(readXML(streamList(is)));
    const value comp = scdl::named(name, c);
    if (isNil(comp))
        return mkfailure<value, std::string>("Could not find component: " + name);
    return comp;
}

const cached<failable<value, std::string> > component(DirConf* conf) {
    const std::string path(conf->contributionPath + conf->compositeName);
    const lambda<failable<value, std::string>(std::string, std::string)> rc(readComponent);
    const lambda<unsigned long(std::string)> ft(latestFileTime);
    return cached<failable<value, std::string> >(curry(rc, path, conf->componentName), curry(ft, path));
}

/**
 * Convert a list of component references to a list of HTTP proxy lambdas.
 */
const value mkproxy(const value& ref, const std::string& base, const http::CURLHandle& ch) {
    return eval::primitiveProcedure(http::proxy(base + std::string(scdl::name(ref)), ch));
}

const list<value> proxies(const list<value>& refs, const std::string& base, const http::CURLHandle& ch) {
    if (isNil(refs))
        return refs;
    return cons(mkproxy(car(refs), base, ch), proxies(cdr(refs), base, ch));
}

/**
 * Returns the component implementation with the given implementation type and path.
 * For now only Scheme and C++ implementations are supported.
 */
const cached<failable<ilambda, std::string> > implementation(const std::string& itype, const std::string& path) {
    if (itype.find(".scheme") != std::string::npos)
        return latest(scm::readImplementation(path));
    if (itype.find(".cpp") != std::string::npos)
        return latest(cpp::readImplementation(path));
    return cached<failable<ilambda, std::string> >();
}

/**
 * HTTP request handler.
 */
int handler(request_rec *r) {
    if(strcmp(r->handler, "mod_tuscany_eval"))
        return DECLINED;
    httpd::logRequest(r, "mod_tuscany_eval::handler");

    // Set up the read policy
    const int rc = httpd::setupReadPolicy(r);
    if(rc != OK)
        return rc;

    // Retrieve the latest component configuration
    DirConf& conf = httpd::dirConf<DirConf>(r, &mod_tuscany_eval);
    conf.component = latest(conf.component);
    const failable<value, std::string> comp(content(conf.component));
    if (!hasContent(comp))
        return HTTP_NOT_FOUND;

    // Retrieve the latest implementation
    const value ielement= scdl::implementation(content(comp));
    const std::string path = conf.contributionPath + std::string(scdl::uri(ielement));
    if (path != conf.implementationPath) {
        conf.implementationPath = path;
        conf.implementation = implementation(elementName(ielement), path);
    }
    else
        conf.implementation = latest(conf.implementation);
    const failable<ilambda, std::string> impl(content(conf.implementation));
    if (!hasContent(impl))
        return HTTP_NOT_FOUND;

    // Convert component references to configured proxy lambdas
    std::ostringstream base;
    base << "http://localhost:" << (debugWiringPort == 0? ap_get_server_port(r) : debugWiringPort) << "/references/" << std::string(scdl::name(content(comp))) << "/";
    http::CURLHandle ch;
    const list<value> px(proxies(scdl::references(content(comp)), base.str(), ch));

    // Handle HTTP method
    if (r->header_only)
         return OK;
    if(r->method_number == M_GET)
        return httpd::reportStatus(get(r, content(impl), px));
    if(r->method_number == M_POST)
        return httpd::reportStatus(post(r, content(impl), px));
    if(r->method_number == M_PUT)
        return httpd::reportStatus(put(r, content(impl), px));
    if(r->method_number == M_DELETE)
        return httpd::reportStatus(del(r, content(impl), px));
    return HTTP_NOT_IMPLEMENTED;
}

/**
 * Configuration commands.
 */
const char *confHome(cmd_parms *cmd, void *dummy, const char *arg) {
    ServerConf *c = (ServerConf*)ap_get_module_config(cmd->server->module_config, &mod_tuscany_eval);
    c->home = arg;
    return NULL;
}
const char *confContribution(cmd_parms *cmd, void *c, const char *arg) {
    DirConf* conf = (DirConf*)c;
    conf->contributionPath = arg;
    conf->component = component(conf);
    return NULL;
}
const char *confComposite(cmd_parms *cmd, void *c, const char *arg) {
    DirConf* conf = (DirConf*)c;
    conf->compositeName = arg;
    conf->component = component(conf);
    return NULL;
}
const char *confComponent(cmd_parms *cmd, void *c, const char *arg) {
    DirConf* conf = (DirConf*)c;
    conf->componentName = arg;
    conf->component = component(conf);
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE1("TuscanyHome", (const char*(*)())confHome, NULL, RSRC_CONF, "Tuscany home directory"),
    AP_INIT_TAKE1("SCAContribution", (const char*(*)())confContribution, NULL, ACCESS_CONF, "SCA contribution location"),
    AP_INIT_TAKE1("SCAComposite", (const char*(*)())confComposite, NULL, ACCESS_CONF, "SCA composite location"),
    AP_INIT_TAKE1("SCAComponent", (const char*(*)())confComponent, NULL, ACCESS_CONF, "SCA component name"),
    {NULL}
};

int postConfig(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *s) {
   return OK;
}

void childInit(apr_pool_t* p, server_rec* svr_rec) {
    ServerConf *c = (ServerConf*)ap_get_module_config(svr_rec->module_config, &mod_tuscany_eval);
    if(c == NULL) {
        std::cerr << "[Tuscany] Due to one or more errors mod_tuscany_eval loading failed. Causing apache to stop loading." << std::endl;
        exit(APEXIT_CHILDFATAL);
    }
}

void registerHooks(apr_pool_t *p) {
    ap_hook_post_config(postConfig, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(childInit, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(handler, NULL, NULL, APR_HOOK_MIDDLE);
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
