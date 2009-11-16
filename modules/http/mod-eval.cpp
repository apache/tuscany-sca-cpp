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

#include <sys/stat.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "list.hpp"
#include "slist.hpp"
#include "value.hpp"
#include "element.hpp"
#include "monad.hpp"
#include "../atom/atom.hpp"
#include "../json/json.hpp"
#include "../eval/driver.hpp"
#include "../scdl/scdl.hpp"
#include "../cache/cache.hpp"
#include "curl.hpp"
#include "httpd.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_eval;
}

namespace tuscany {
namespace httpd {
namespace modeval {

/**
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf() : home("") {
    }
    std::string home;
};

const ServerConf& serverConf(const request_rec* r) {
    return *(ServerConf*)ap_get_module_config(r->server->module_config, &mod_tuscany_eval);
}

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
    DirConf() : contributionPath(""), compositeName(""), componentName(""), implementationPath("") {
    }
    std::string contributionPath;
    std::string compositeName;
    std::string componentName;
    std::string implementationPath;
    cache::cached<failable<value, std::string> > component;
    cache::cached<failable<value, std::string> > implementation;
};

DirConf& dirConf(const request_rec* r) {
    return *(DirConf*)ap_get_module_config(r->per_dir_config, &mod_tuscany_eval);
}

/**
 * Evaluate an expression against a component implementation.
 */
const failable<value, std::string> evalExpr(const value& expr, const value& impl) {
    gc_pool pool;
    eval::Env globalEnv = eval::setupEnvironment(pool);
    if (logContent) {
        std::cout<< "expr: " << expr << std::endl;
        std::cout.flush();
    }
    const value val = eval::evalScript(expr, impl, globalEnv, pool);
    if (logContent) {
        std::cout<< "val: " << val << std::endl;
        std::cout.flush();
    }

    if (isNil(val))
        return mkfailure<value, std::string>("Could not evaluate expression");
    return val;
}

/**
 * Returns a list of param values other than the id and method args from a list
 * of key value pairs.
 */
const list<value> queryParams(const list<list<value> >& a) {
    if (isNil(a))
        return list<value>();
    const list<value> p = car(a);
    if (car(p) == value("id") || car(p) == value("method"))
        return queryParams(cdr(a));
    return cons(cadr(p), queryParams(cdr(a)));
}

/**
 * Write an HTTP result.
 */
const failable<int, std::string> writeResult(const failable<list<std::string>, std::string>& ls, const std::string& ct, request_rec* r) {
    if (!hasValue(ls))
        return mkfailure<int, std::string>(reason(ls));
    std::ostringstream os;
    write(ls, os);
    if (logContent) {
        std::cout<< "content: " << std::endl << os.str() << std::endl;
        std::cout.flush();
    }

    const std::string etag(ap_md5(r->pool, (const unsigned char*)std::string(os.str()).c_str()));
    const char* match = apr_table_get(r->headers_in, "If-None-Match");
    apr_table_setn(r->headers_out, "ETag", apr_pstrdup(r->pool, etag.c_str()));
    if (match != NULL  && etag == match) {
        r->status = HTTP_NOT_MODIFIED;
        return OK;
    }
    ap_set_content_type(r, ct.c_str());
    ap_rputs(std::string(os.str()).c_str(), r);
    return OK;
}

/**
 * Handle an HTTP GET.
 */
const failable<int, std::string> get(request_rec* r, const value& impl, const list<value>& px) {

    // Inspect the query string
    const list<list<value> > args = queryArgs(r);
    const list<value> ia = assoc(value("id"), args);
    const list<value> ma = assoc(value("method"), args);

    // Evaluate a JSON-RPC request and return a JSON result
    if (!isNil(ia) && !isNil(ma)) {

        // Extract the request id, method and params
        const value id = cadr(ia);
        const value func = std::string(cadr(ma)).c_str();
        const list<value> params = queryParams(args);

        // Evaluate the request expression
        const failable<value, std::string> val = evalExpr(cons<value>(func, eval::quotedParameters(append(params, px))), impl);
        if (!hasValue(val))
            return mkfailure<int, std::string>(reason(val));

        // Return JSON result
        json::JSONContext cx;
        return writeResult(json::jsonResult(id, val, cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM GET request and return an ATOM feed
    const list<value> id(path(r->path_info));
    if (isNil(id)) {
        const failable<value, std::string> val = evalExpr(cons<value>("getall", eval::quotedParameters(px)), impl);
        if (!hasValue(val))
            return mkfailure<int, std::string>(reason(val));
        const value feed = val;
        return writeResult(atom::writeATOMFeed(atom::feedValuesToElements(feed)), "application/atom+xml;type=feed", r);
    }

    // Evaluate an ATOM GET and return an ATOM entry
    const failable<value, std::string> val = evalExpr(cons<value>("get", eval::quotedParameters(cons<value>(car(id), px))), impl);
    if (!hasValue(val))
        return mkfailure<int, std::string>(reason(val));
    const value entry = val;
    return writeResult(atom::writeATOMEntry(atom::entryValuesToElements(entry)), "application/atom+xml;type=entry", r);
    
}

/**
 * Read the content of a POST.
 */
const list<std::string> read(request_rec* r) {
    char b[2048];
    const int n = ap_get_client_block(r, b, 2048);
    if (n <= 0)
        return list<std::string>();
    return cons(std::string(b, n), read(r));
}

/**
 * Convert a URI value to an absolute URL.
 */
const char* url(const value& v, request_rec* r) {
    std::string u = r->uri;
    u.append("/");
    u.append(v);
    return ap_construct_url(r->pool, u.c_str(), r);
}

/**
 * Convert an ATOM entry to a value.
 */
const value feedEntry(const list<value>& e) {
    const list<value> v = elementsToValues(mklist<value>(caddr(e)));
    return cons(car(e), mklist<value>(cadr(e), cdr<value>(car(v))));
}

/**
 * Handle an HTTP POST.
 */
const failable<int, std::string> post(request_rec* r, const value& impl, const list<value>& px) {
    const list<std::string> ls = read(r);
    if (logContent) {
        std::cout<< "content: " << std::endl;
        write(ls, std::cout);
        std::cout<< std::endl;
        std::cout.flush();
    }

    // Evaluate a JSON-RPC request and return a JSON result
    const std::string ct = contentType(r);
    if (ct.find("application/json-rpc") != std::string::npos || ct.find("text/plain") != std::string::npos) {
        json::JSONContext cx;
        const list<value> json = elementsToValues(json::readJSON(ls, cx));
        const list<list<value> > args = postArgs(json);

        // Extract the request id, method and params
        const value id = cadr(assoc(value("id"), args));
        const value func = std::string(cadr(assoc(value("method"), args))).c_str();
        const list<value> params = (list<value>)cadr(assoc(value("params"), args));

        // Evaluate the request expression
        const failable<value, std::string> val = evalExpr(cons<value>(func, eval::quotedParameters(append(params, px))), impl);
        if (!hasValue(val))
            return mkfailure<int, std::string>(reason(val));

        // Return JSON result
        return writeResult(json::jsonResult(id, val, cx), "application/json-rpc", r);
    }

    // Evaluate an ATOM POST request and return the created resource location
    if (ct.find("application/atom+xml") != std::string::npos) {

        // Evaluate the request expression
        const value entry = feedEntry(atom::readEntry(ls));
        const failable<value, std::string> val = evalExpr(cons<value>("post", eval::quotedParameters(cons<value>(entry, px))), impl);
        if (!hasValue(val))
            return mkfailure<int, std::string>(reason(val));

        // Return the created resource location
        apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, url(val, r)));
        r->status = HTTP_CREATED;
        return OK;
    }

    return HTTP_NOT_IMPLEMENTED;
}

/**
 * Handle an HTTP PUT.
 */
const failable<int, std::string> put(request_rec* r, const value& impl, const list<value>& px) {
    const list<std::string> ls = read(r);
    if (logContent) {
        std::cout<< "content: " << std::endl;
        write(ls, std::cout);
        std::cout<< std::endl;
        std::cout.flush();
    }

    // Evaluate an ATOM PUT request
    const list<value> id(path(r->path_info));
    const value entry = feedEntry(atom::readEntry(ls));
    const failable<value, std::string> val = evalExpr(cons<value>("put", eval::quotedParameters(append(mklist<value>(entry, car(id)), px))), impl);
    if (!hasValue(val))
        return mkfailure<int, std::string>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Handle an HTTP DELETE.
 */
const failable<int, std::string> del(request_rec* r, const value& impl, const list<value>& px) {

    // Evaluate an ATOM delete request
    const list<value> id(path(r->path_info));
    const failable<value, std::string> val = evalExpr(cons<value>("delete", eval::quotedParameters(cons<value>(car(id), px))), impl);
    if (!hasValue(val))
        return mkfailure<int, std::string>(reason(val));
    if (val == value(false))
        return HTTP_NOT_FOUND;
    return OK;
}

/**
 * Report request execution status.
 */
const int reportStatus(const failable<int, std::string>& rc) {
    if (!hasValue(rc))
        return HTTP_INTERNAL_SERVER_ERROR;
    return rc;
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

const cache::cached<failable<value, std::string> > component(DirConf* conf) {
    const std::string path(conf->contributionPath + conf->compositeName);
    const lambda<failable<value, std::string>(std::string, std::string)> rc(readComponent);
    const lambda<unsigned long(std::string)> ft(cache::latestFileTime);
    return cache::cached<failable<value, std::string> >(curry(rc, path, conf->componentName), curry(ft, path));
}

/**
 * Read a component implementation.
 */
const failable<value, std::string> readImplementation(const std::string path) {
    std::ifstream is(path.c_str(), std::ios_base::in);
    if (is.fail() || is.bad())
        return mkfailure<value, std::string>("Could not read implementation: " + path);
    const value impl = eval::readScript(is);
    if (isNil(impl))
        return mkfailure<value, std::string>("Could not read implementation: " + path);
    return impl;
}

const cache::cached<failable<value, std::string> > implementation(const std::string& path) {
    const lambda<failable<value, std::string>(std::string)> ri(readImplementation);
    const lambda<unsigned long(std::string)> ft(cache::latestFileTime);
    return cache::cached<failable<value, std::string> >(curry(ri, path), curry(ft, path));
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
 * HTTP request handler.
 */
int handler(request_rec *r) {
    if(strcmp(r->handler, "mod_tuscany_eval"))
        return DECLINED;

    // Log the request
    if(logRequests)
        logRequest(r, "mod_tuscany_eval::handler");

    // Set up the read policy
    const int rc = ap_setup_client_block(r, REQUEST_CHUNKED_DECHUNK);
    if(rc != OK)
        return rc;
    ap_should_client_block(r);
    if(r->read_chunked == true && r->remaining == 0)
        r->chunked = true;
    //apr_table_setn(r->headers_out, "Connection", "close");

    // Retrieve the latest component configuration
    DirConf& conf = dirConf(r);
    conf.component = cache::latest(conf.component);
    const failable<value, std::string> comp(conf.component);
    if (!hasValue(comp))
        return HTTP_NOT_FOUND;

    // Retrieve the latest implementation
    const std::string path = conf.contributionPath + std::string(scdl::uri(scdl::implementation(comp)));
    if (path != conf.implementationPath) {
        conf.implementationPath = path;
        conf.implementation = cache::latest(implementation(path));
    }
    else
        conf.implementation = cache::latest(conf.implementation);
    const failable<value, std::string> impl(conf.implementation);
    if (!hasValue(impl))
        return HTTP_NOT_FOUND;

    // Convert component references to configured proxy lambdas
    std::ostringstream base;
    base << "http://localhost:" << (debugWiringPort == 0? ap_get_server_port(r) : debugWiringPort) << "/references/" << std::string(scdl::name(comp)) << "/";
    http::CURLHandle ch;
    const list<value> px(proxies(scdl::references(comp), base.str(), ch));

    // Handle HTTP method
    if (r->header_only)
         return OK;
    if(r->method_number == M_GET)
        return reportStatus(get(r, impl, px));
    if(r->method_number == M_POST)
        return reportStatus(post(r, impl, px));
    if(r->method_number == M_PUT)
        return reportStatus(put(r, impl, px));
    if(r->method_number == M_DELETE)
        return reportStatus(del(r, impl, px));
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

void *makeDirConf(apr_pool_t *p, char *dirspec) {
    DirConf* c = new (apr_palloc(p, sizeof(DirConf))) DirConf();
    apr_pool_cleanup_register(p, c, gc_pool_cleanupCallback<DirConf>, apr_pool_cleanup_null) ;
    return c;
}
void* makeServerConf(apr_pool_t *p, server_rec *s) {
    ServerConf* c = new (apr_palloc(p, sizeof(ServerConf))) ServerConf();
    apr_pool_cleanup_register(p, c, gc_pool_cleanupCallback<ServerConf>, apr_pool_cleanup_null) ;
    return c;
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
    // dir config
    tuscany::httpd::modeval::makeDirConf,
    // dir merger, default is to override
    NULL,
    // server config
    tuscany::httpd::modeval::makeServerConf,
    // merge server config
    NULL,
    // command table
    tuscany::httpd::modeval::commands,
    // register hooks
    tuscany::httpd::modeval::registerHooks
};

}
