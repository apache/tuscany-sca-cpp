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

#define WANT_HTTPD_LOG 1
#include "string.hpp"
#include "stream.hpp"
#include "list.hpp"
#include "tree.hpp"
#include "value.hpp"
#include "monad.hpp"
#include "../scdl/scdl.hpp"
#include "../http/http.hpp"
#include "../http/httpd.hpp"

extern "C" {
extern module AP_MODULE_DECLARE_DATA mod_tuscany_wiring;
}

namespace tuscany {
namespace server {
namespace modwiring {

/**
 * Set to true to wire using mod_proxy, false to wire using HTTP client redirects.
 */
const bool useModProxy = true;

/**
 * Server configuration.
 */
class ServerConf {
public:
    ServerConf(apr_pool_t* p, server_rec* s) : p(p), server(s), contributionPath(""), compositeName(""), virtualHostDomain(""), virtualHostContributionPath(""), virtualHostCompositeName("") {
    }

    ServerConf(apr_pool_t* p, const ServerConf& ssc, const string& name) : p(p), server(ssc.server), contributionPath(ssc.virtualHostContributionPath + name + "/"), compositeName(ssc.virtualHostCompositeName), virtualHostDomain(""), virtualHostContributionPath(""), virtualHostCompositeName("") {
    }

    const gc_pool p;
    server_rec* server;
    string contributionPath;
    string compositeName;
    string virtualHostDomain;
    string virtualHostContributionPath;
    string virtualHostCompositeName;
    list<value> references;
    list<value> services;
};

/**
 * Return true if a server contains a composite configuration.
 */
const bool hasCompositeConf(const ServerConf& sc) {
    return sc.contributionPath != "" && sc.compositeName != "";
}

/**
 * Return true if a server contains a virtual host domain configuration.
 */
const bool hasVirtualDomainConf(const ServerConf& sc) {
    return sc.virtualHostDomain != "";
}

/**
 * Return true if a server contains a virtual host composite configuration.
 */
const bool hasVirtualCompositeConf(const ServerConf& sc) {
    return sc.virtualHostContributionPath != "" && sc.virtualHostCompositeName != "";
}

/**
 * Route a /references/component-name/reference-name request,
 * to the target of the component reference.
 */
int translateReference(const ServerConf& sc, request_rec *r, const list<value>& rpath, const list<value>& apath) {
    httpdDebugRequest(r, "modwiring::translateReference::input");
    debug(r->uri, "modwiring::translateReference::uri");
    debug(apath, "modwiring::translateReference::apath");
    debug(rpath, "modwiring::translateReference::rpath");

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
    debug(target, "modwiring::translateReference::target");

    // Route to an absolute target URI using mod_proxy or an HTTP client redirect
    const list<value> pathInfo = cdddr(rpath);
    if (http::isAbsolute(target)) {
        if (useModProxy) {
            // Build proxy URI
            string turi = target + path(pathInfo) + (r->args != NULL? string("?") + r->args : string(""));
            const string proxy(string("proxy:") + turi);
            debug(proxy, "modwiring::translateReference::proxy");
            r->filename = apr_pstrdup(r->pool, c_str(proxy));
            r->proxyreq = PROXYREQ_REVERSE;
            r->handler = "proxy-server";
            apr_table_setn(r->notes, "proxy-nocanon", "1");
            return OK;
        }

        debug(target, "modwiring::translateReference::location");
        r->handler = "mod_tuscany_wiring";
        return httpd::externalRedirect(target, r);
    }

    // Route to a relative target URI using a local internal redirect
    // /components/, target component name and request path info
    const value tname = substr(target, 0, find(target, '/'));
    const string tp = path(append(apath, cons<value>(string("c"), cons(tname, pathInfo)))) + (r->args != NULL? string("?") + r->args : string(""));
    const string redir(string("/redirect:") + tp);
    debug(redir, "modwiring::translateReference::redirect");
    r->filename = apr_pstrdup(r->pool, c_str(redir));
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
int translateService(const ServerConf& sc, request_rec *r, const list<value>& rpath, const list<value>& apath) {
    httpdDebugRequest(r, "modwiring::translateService::input");
    debug(r->uri, "modwiring::translateService::uri");

    // Find the requested component
    debug(sc.services, "modwiring::translateService::services");
    const list<value> svc(assocPath(rpath, sc.services));
    if (isNil(svc))
        return DECLINED;
    debug(svc, "modwiring::translateService::service");

    // Build a component-name + path-info URI
    const list<value> target(append(apath, cons<value>(string("c"), cons<value>(cadr(svc), httpd::pathInfo(rpath, car(svc))))));
    debug(target, "modwiring::translateService::target");

    // Dispatch to the target component using a local internal redirect
    const string tp(path(target) + (r->args != NULL? string("?") + r->args : string("")));
    const string redir(string("/redirect:") + tp);
    debug(redir, "modwiring::translateService::redirect");
    r->filename = apr_pstrdup(r->pool, c_str(redir));
    r->handler = "mod_tuscany_wiring";
    return OK;
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
    if (!hasCompositeConf(sc))
        return true;
    debug(sc.contributionPath, "modwiring::confComponents::contributionPath");
    debug(sc.compositeName, "modwiring::confComponents::compositeName");

    // Read the component configuration and store the references and service URIs
    // in trees for fast retrieval later
    const failable<list<value> > comps = readComponents(scdl::resourcePath(sc.contributionPath, sc.compositeName));
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
 * Configure and start the components deployed in a virtual host.
 */
const failable<ServerConf> virtualHostConfig(ServerConf& sc, const ServerConf& ssc, request_rec* r) {
    debug(httpd::serverName(ssc.server), "modwiring::virtualHostConfig::serverName");
    debug(httpd::serverName(r), "modwiring::virtualHostConfig::virtualHostName");
    debug(ssc.virtualHostContributionPath, "modwiring::virtualHostConfig::virtualHostContributionPath");
    debug(sc.contributionPath, "modwiring::virtualHostConfig::contributionPath");

    // Configure the wiring for the deployed components
    confComponents(sc);

    return sc;
}

/**
 * Translate an HTTP service or reference request and route it
 * to the target component.
 */
const int translateRequest(const ServerConf& sc, request_rec *r, const list<value>& rpath, const list<value>& apath) {
    debug(apath, "modwiring::translateRequest::apath");
    debug(rpath, "modwiring::translateRequest::rpath");
    if (isNil(apath) && isNil(rpath))
        return DECLINED;

    if (!isNil(rpath)) {

        // No translation needed for a component or resource request
        const value c = car(rpath);
        if (c == string("components") || c == string("c") || c == string("vhosts") || c == string("v"))
            return DECLINED;

        // If the request is targeting a virtual host, use the corresponding
        // virtual host configuration
        const bool vdc = hasVirtualDomainConf(sc);
        const bool vcc = hasVirtualCompositeConf(sc);
        if (vdc && vcc && httpd::isVirtualHostRequest(sc.server, sc.virtualHostDomain, r)) {
            ServerConf vsc(r->pool, sc, http::subDomain(httpd::hostName(r)));
            if (!hasContent(virtualHostConfig(vsc, sc, r)))
                return HTTP_INTERNAL_SERVER_ERROR;
            return translateRequest(vsc, r, rpath, list<value>());
        }

        // Translate a component reference request
        if (c == string("references") || c == string("r"))
            return translateReference(sc, r, rpath, apath);

        // Attempt to translate the request to a service request
        if (translateService(sc, r, rpath, apath) == OK)
            return OK;

        // If the request is targeting a virtual app, use the corresponding
        // virtual host configuration
        if (vcc) {
            const string cp = sc.virtualHostContributionPath + string(c) + "/" + sc.virtualHostCompositeName;
            struct stat st;
            const int s = stat(c_str(cp), &st);
            if (s == -1)
                return DECLINED;
            ServerConf vsc(r->pool, sc, string(c));
            if (!hasContent(virtualHostConfig(vsc, sc, r)))
                return HTTP_INTERNAL_SERVER_ERROR;
            return translateRequest(vsc, r, cdr(rpath), mklist<value>(car(rpath)));
        }
    }

    // If we're in a virtual app and the request is targeting a regular
    // resource, redirect it to /v/<uri>. This will allow mapping to the
    // actual resources using HTTPD aliases.
    if (!isNil(apath)) {
        const string tp = string("/v") + string(r->uri) + (r->args != NULL? string("?") + r->args : string(""));
        const string redir = string("/redirect:") + tp;
        debug(redir, "modwiring::translateRequest::redirect");
        r->filename = apr_pstrdup(r->pool, c_str(redir));
        r->handler = "mod_tuscany_wiring";
        return OK;
    }

    return DECLINED;
}

/**
 * Translate an HTTP service or reference request and route it
 * to the target component.
 */
int translate(request_rec *r) {
    if(r->method_number != M_GET && r->method_number != M_POST && r->method_number != M_PUT && r->method_number != M_DELETE)
        return DECLINED;

    // Create a scoped memory pool
    gc_scoped_pool pool(r->pool);
    httpdDebugRequest(r, "modwiring::translate::input");

    // Get the server configuration
    const ServerConf& sc = httpd::serverConf<ServerConf>(r, &mod_tuscany_wiring);

    // Translate the request
    return translateRequest(sc, r, pathValues(r->uri), list<value>());
}

/**
 * HTTP request handler, redirect to a target component.
 */
int handler(request_rec *r) {
    if(r->method_number != M_GET && r->method_number != M_POST && r->method_number != M_PUT && r->method_number != M_DELETE)
        return DECLINED;
    if(strcmp(r->handler, "mod_tuscany_wiring"))
        return DECLINED;
    if (r->filename == NULL || strncmp(r->filename, "/redirect:", 10) != 0)
        return DECLINED;

    // Nothing to do for an external redirect
    if (r->status == HTTP_MOVED_TEMPORARILY)
        return OK;

    // Create a scoped memory pool
    gc_scoped_pool pool(r->pool);

    httpdDebugRequest(r, "modwiring::handler::input");
    debug(r->uri, "modwiring::handler::uri");
    debug(r->filename, "modwiring::handler::filename");
    debug(r->path_info, "modwiring::handler::path info");

    // Do an internal redirect
    if (r->args == NULL)
        return httpd::internalRedirect(httpd::redirectURI(string(r->filename + 10), string(r->path_info)), r);
    return httpd::internalRedirect(httpd::redirectURI(string(r->filename + 10), string(r->path_info), string(r->args)), r);
}

/**
 * Called after all the configuration commands have been run.
 * Process the server configuration and configure the wiring for the deployed components.
 */
const int postConfigMerge(const ServerConf& mainsc, server_rec* s) {
    if (s == NULL)
        return OK;
    debug(httpd::serverName(s), "modwiring::postConfigMerge::serverName");
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_wiring);
    sc.contributionPath = mainsc.contributionPath;
    sc.compositeName = mainsc.compositeName;
    sc.virtualHostDomain = mainsc.virtualHostDomain;
    sc.virtualHostContributionPath = mainsc.virtualHostContributionPath;
    sc.virtualHostCompositeName = mainsc.virtualHostCompositeName;
    sc.references = mainsc.references;
    sc.services = mainsc.services;
    return postConfigMerge(mainsc, s->next);
}

int postConfig(apr_pool_t *p, unused apr_pool_t *plog, unused apr_pool_t *ptemp, server_rec *s) {
    gc_scoped_pool pool(p);

    // Count the calls to post config, skip the first one as
    // postConfig is always called twice
    const string k("tuscany::modwiring::postConfig");
    const long int count = (long int)httpd::userData(k, s);
    httpd::putUserData(k, (void*)(count + 1), s);
    if (count == 0)
        return OK;

    // Configure the wiring for the deployed components
    debug(httpd::serverName(s), "modwiring::postConfig::serverName");
    ServerConf& sc = httpd::serverConf<ServerConf>(s, &mod_tuscany_wiring);
    confComponents(sc);

    // Merge the config into any virtual hosts
    return postConfigMerge(sc, s->next);
}

/**
 * Child process initialization.
 */
void childInit(apr_pool_t* p, server_rec* s) {
    gc_scoped_pool pool(p);
    if(ap_get_module_config(s->module_config, &mod_tuscany_wiring) == NULL) {
        cfailure << "[Tuscany] Due to one or more errors mod_tuscany_wiring loading failed. Causing apache to stop loading." << endl;
        exit(APEXIT_CHILDFATAL);
    }
}

/**
 * Configuration commands.
 */
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
const char *confVirtualDomain(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_wiring);
    sc.virtualHostDomain = arg;
    return NULL;
}
const char *confVirtualContribution(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_wiring);
    sc.virtualHostContributionPath = arg;
    return NULL;
}
const char *confVirtualComposite(cmd_parms *cmd, unused void *c, const char *arg) {
    gc_scoped_pool pool(cmd->pool);
    ServerConf& sc = httpd::serverConf<ServerConf>(cmd, &mod_tuscany_wiring);
    sc.virtualHostCompositeName = arg;
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE1("SCAContribution", (const char*(*)())confContribution, NULL, RSRC_CONF, "SCA contribution location"),
    AP_INIT_TAKE1("SCAComposite", (const char*(*)())confComposite, NULL, RSRC_CONF, "SCA composite location"),
    AP_INIT_TAKE1("SCAVirtualDomain", (const char*(*)())confVirtualDomain, NULL, RSRC_CONF, "SCA virtual host domain"),
    AP_INIT_TAKE1("SCAVirtualContribution", (const char*(*)())confVirtualContribution, NULL, RSRC_CONF, "SCA virtual host contribution location"),
    AP_INIT_TAKE1("SCAVirtualComposite", (const char*(*)())confVirtualComposite, NULL, RSRC_CONF, "SCA virtual host composite location"),
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
