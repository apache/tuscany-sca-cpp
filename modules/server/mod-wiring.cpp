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
 * HTTPD module used to wire component references and route requests to
 * target service components.
 */

#include <sys/stat.h>

#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../scdl/scdl.hpp"
#include "../http/httpd.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_wiring;
}

namespace tuscany {
namespace server {
namespace modwiring {

/**
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf(server_rec* s) : s(s), start(false), home(""), wiringServerName(""), contributionPath(""), compositeName("") {
    }
    const server_rec* s;
    bool start;
    string home;
    string wiringServerName;
    string contributionPath;
    string compositeName;
    list<value> references;
    list<value> services;
};

/**
 * Set to true to wire using mod_proxy, false to wire using HTTP client redirects.
 */
const bool useModProxy = true;

/**
 * Returns true if a URI is absolute.
 */
const bool isAbsolute(const string& uri) {
    return contains(uri, "://");
}

/**
 * Route a /references/component-name/reference-name request,
 * to the target of the component reference.
 */
int translateReference(request_rec *r) {
    httpdDebugRequest(r, "modwiring::translateReference::input");
    debug(r->uri, "modwiring::translateReference::uri");

    // Find the requested component
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_wiring);
    const list<value> rpath(pathValues(r->uri));
    const list<value> comp(assoctree(cadr(rpath), sc.references));
    if (isNil(comp))
        return HTTP_NOT_FOUND;

    // Find the requested reference and target configuration
    const list<value> ref(assoctree<value>(caddr(rpath), cadr(comp)));
    if (isNil(ref))
        return HTTP_NOT_FOUND;
    const string target(cadr(ref));
    debug(target, "modwiring::translateReference::target");

    // Route to an absolute target URI using mod_proxy or an HTTP client redirect
    if (isAbsolute(target)) {
        if (useModProxy) {
            r->filename = apr_pstrdup(r->pool, c_str(string("proxy:") + target));
            r->proxyreq = PROXYREQ_REVERSE;
            r->handler = "proxy-server";
            return OK;
        }

        r->status = HTTP_MOVED_TEMPORARILY;
        apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, c_str(target)));
        r->handler = "mod_tuscany_wiring";
        return OK;
    }

    // Route to a relative target URI using a local internal redirect
    r->filename = apr_pstrdup(r->pool, c_str(string("/redirect:/components/") + target));
    r->handler = "mod_tuscany_wiring";
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
int translateService(request_rec *r) {
    httpdDebugRequest(r, "modwiring::translateService::input");
    debug(r->uri, "modwiring::translateService::uri");

    // Find the requested component
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_wiring);
    const list<value> p(pathValues(r->uri));
    const list<value> svc(assocPath(p, sc.services));
    if (isNil(svc))
        return DECLINED;
    debug(svc, "modwiring::translateService::service");

    // Build a component-name + path-info URI
    const list<value> target(cons<value>(cadr(svc), httpd::pathInfo(p, car(svc))));
    debug(target, "modwiring::translateService::target");

    // Dispatch to the target component using a local internal redirect
    const string tp(path(target));
    debug(tp, "modwiring::translateService::path");
    const string redir(string("/redirect:/components") + tp);
    debug(redir, "modwiring::translateService::redirect");
    r->filename = apr_pstrdup(r->pool, c_str(redir));
    r->handler = "mod_tuscany_wiring";
    return OK;
}

/**
 * Translate an HTTP service or reference request and route it
 * to the target component.
 */
int translate(request_rec *r) {
    gc_scoped_pool pool(r->pool);
    if (!strncmp(r->uri, "/components/", 12) != 0)
        return DECLINED;

    // Translate a component reference request
    if (!strncmp(r->uri, "/references/", 12) != 0)
        return translateReference(r);

    // Translate a service request
    return translateService(r);
}

/**
 * HTTP request handler, redirect to a target component.
 */
int handler(request_rec *r) {
    gc_scoped_pool pool(r->pool);
    if(strcmp(r->handler, "mod_tuscany_wiring"))
        return DECLINED;
    httpdDebugRequest(r, "modwiring::handler::input");

    // Do an internal redirect
    if (r->filename == NULL || strncmp(r->filename, "/redirect:", 10) != 0)
        return DECLINED;
    debug(r->uri, "modwiring::handler::uri");
    debug(r->filename, "modwiring::handler::filename");
    debug(r->path_info, "modwiring::handler::path info");

    if (r->args == NULL)
        return httpd::internalRedirect(httpd::redirectURI(string(r->filename + 10), string(r->path_info)), r);
    return httpd::internalRedirect(httpd::redirectURI(string(r->filename + 10), string(r->path_info), string(r->args)), r);
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
 * Configure the components declared in the server's deployment composite.
 */
const bool confComponents(ServerConf& sc) {
    if (sc.contributionPath == "" || sc.compositeName == "")
        return true;

    // Read the component configuration and store the references and service URIs
    // in trees for fast retrieval later
    const failable<list<value> > comps = readComponents(sc.contributionPath + sc.compositeName);
    if (!hasContent(comps))
        return true;
    const list<value> refs = componentReferenceToTargetAssoc(content(comps));
    debug(refs, "modwiring::confComponents::references");
    sc.references = mkbtree(sort(refs));

    const list<value> svcs = uriToComponentAssoc(content(comps));
    debug(svcs, "modwiring::confComponents::services");
    sc.services = mkbtree(sort(svcs));
    return true;
}

/**
 * Called after all the configuration commands have been run.
 * Process the server configuration and configure the wiring for the deployed components.
 */
int postConfig(unused apr_pool_t *p, unused apr_pool_t *plog, unused apr_pool_t *ptemp, server_rec *s) {
    // Count the calls to post config, skip the first one as
    // postConfig is always called twice
    const string k("tuscany::modwiring::postConfig");
    const int count = (int)httpd::userData(k, s);
    httpd::putUserData(k, (void*)(count + 1), s);
    if (count == 0)
        return OK;

    // Configure the wiring for the deployed components
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_wiring);
    debug(sc.wiringServerName, "modwiring::postConfig::wiringServerName");
    debug(sc.contributionPath, "modwiring::postConfig::contributionPath");
    debug(sc.compositeName, "modwiring::postConfig::compositeName");
    confComponents(sc);
    return OK;
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* p, server_rec* svr_rec) {
    gc_scoped_pool pool(p);
    ServerConf *conf = (ServerConf*)ap_get_module_config(svr_rec->module_config, &mod_tuscany_wiring);
    if(conf == NULL) {
        cerr << "[Tuscany] Due to one or more errors mod_tuscany_wiring loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
}

/**
 * Configuration commands.
 */
const char *confHome(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_wiring);
    sc.home = arg;
    return NULL;
}
const char *confWiringServerName(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_wiring);
    sc.wiringServerName = arg;
    return NULL;
}
const char *confContribution(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_wiring);
    sc.contributionPath = arg;
    return NULL;
}
const char *confComposite(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_wiring);
    sc.compositeName = arg;
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE1("TuscanyHome", (const char*(*)())confHome, NULL, RSRC_CONF, "Tuscany home directory"),
    AP_INIT_TAKE1("SCAWiringServerName", (const char*(*)())confWiringServerName, NULL, RSRC_CONF, "SCA wiring server name"),
    AP_INIT_TAKE1("SCAContribution", (const char*(*)())confContribution, NULL, RSRC_CONF, "SCA contribution location"),
    AP_INIT_TAKE1("SCAComposite", (const char*(*)())confComposite, NULL, RSRC_CONF, "SCA composite location"),
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

module AP_MODULE_DECLARE_DATA mod_tuscany_wiring = {
    STANDARD20_MODULE_STUFF,
    // dir config and merger
    NULL, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::server::modwiring::ServerConf>, NULL,
    // commands and hooks
    tuscany::server::modwiring::commands, tuscany::server::modwiring::registerHooks
};

}
