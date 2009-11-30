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
 * HTTPD module used to wire component references.
 */

#include <sys/stat.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "list.hpp"
#include "slist.hpp"
#include "value.hpp"
#include "debug.hpp"
#include "monad.hpp"
#include "cache.hpp"
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
    ServerConf(server_rec* s) : s(s), home(""), wiringHost("") {
    }
    const server_rec* s;
    std::string home;
    std::string wiringHost;
};

/**
 * Set to true to wire using mod_proxy, false to wire using HTTP client redirects.
 */
const bool useModProxy = true;

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
    cached<failable<list<value>, std::string> > components;
};

/**
 * Read the SCDL configuration of the deployed components.
 */
const failable<list<value>, std::string> readComponents(const std::string& path) {
    std::ifstream is(path);
    if (is.fail() || is.bad())
        return mkfailure<list<value>, std::string>("Could not read composite: " + path);
    return scdl::components(readXML(streamList(is)));
}

const cached<failable<list<value>, std::string> > components(DirConf* conf) {
    const std::string path(conf->contributionPath + conf->compositeName);
    const lambda<failable<list<value>, std::string>(std::string)> rc(readComponents);
    const lambda<unsigned long(std::string)> ft(latestFileTime);
    return cached<failable<list<value>, std::string> >(curry(rc, path), curry(ft, path));
}

/**
 * Returns true if a URI is absolute.
 */
const bool isAbsolute(const std::string& uri) {
    return uri.find("://") != std::string::npos;
}

/**
 * Translate an HTTP request URI. Wire a URI in the form /references/component-name/reference-name
 * to the target of the reference.
 */
int translate(request_rec *r) {
    if (strncmp(r->uri, "/references/", 12) != 0)
        return DECLINED;
    httpdDebugRequest(r, "modwiring::translate::input");
    debug(r->uri, "modwiring::translate::uri");

    // Find the requested component, reference and its target configuration
    DirConf& conf = httpd::dirConf<DirConf>(r, &mod_tuscany_wiring);
    conf.components = latest(conf.components);
    const failable<list<value>, std::string> comps(content(conf.components));
    if (!hasContent(comps))
        return HTTP_INTERNAL_SERVER_ERROR;
    const list<value> rpath(httpd::path(r->uri));
    const value comp(scdl::named(cadr(rpath), list<value>(content(comps))));
    if (isNil(comp))
        return HTTP_NOT_FOUND;
    const value ref(scdl::named(caddr(rpath), scdl::references(comp)));
    if (isNil(ref))
        return HTTP_NOT_FOUND;
    const std::string target(scdl::target(ref));

    // Absolute target URI
    if (isAbsolute(target)) {

        // Wire using mod_proxy
        if (useModProxy) {
            r->filename = apr_pstrdup(r->pool, std::string("proxy:" + target).c_str());
            r->proxyreq = PROXYREQ_REVERSE;
            r->handler = "proxy-server";
            return OK;
        }

        // Wire using an HTTP client redirect
        r->status = HTTP_MOVED_TEMPORARILY;
        apr_table_setn(r->headers_out, "Location", apr_pstrdup(r->pool, target.c_str()));
        r->handler = "mod_tuscany_wiring";
        return OK;

    }

    // Local internal redirect
    r->filename = apr_pstrdup(r->pool, std::string("/redirect:/" + target).c_str());
    r->handler = "mod_tuscany_wiring";
    return OK;
}

/**
 * Construct a redirect URI.
 */
const std::string redirect(const std::string& file, const std::string& pi) {
    return file + pi;
}

const std::string redirect(const std::string& file, const std::string& pi, const std::string& args) {
    return file + pi + "?" + args;
}

/**
 * HTTP request internal redirect handler.
 */
int handler(request_rec *r) {
    if(strcmp(r->handler, "mod_tuscany_wiring"))
        return DECLINED;
    httpdDebugRequest(r, "modwiring::handler::input");

    // Do an internal redirect
    if (r->filename == NULL || strncmp(r->filename, "/redirect:", 10) != 0)
        return DECLINED;
    debug(r->uri, "modwiring::handler::uri");
    debug(r->filename, "modwiring::handler::filename");

    if (r->args == NULL) {
        ap_internal_redirect(apr_pstrdup(r->pool, redirect(r->filename + 10, r->path_info).c_str()), r);
        return OK;
    }
    ap_internal_redirect(apr_pstrdup(r->pool, redirect(r->filename + 10, r->path_info, r->args).c_str()), r);
    return OK;
}

/**
 * Configuration commands.
 */
const char *confHome(cmd_parms *cmd, unused void *dummy, const char *arg) {
    ServerConf *c = (ServerConf*)ap_get_module_config(cmd->server->module_config, &mod_tuscany_wiring);
    c->home = arg;
    return NULL;
}
const char *confWiringHost(cmd_parms *cmd, unused void *dummy, const char *arg) {
    ServerConf *c = (ServerConf*)ap_get_module_config(cmd->server->module_config, &mod_tuscany_wiring);
    c->wiringHost = arg;
    return NULL;
}
const char *confContribution(unused cmd_parms *cmd, void *c, const char *arg) {
    DirConf* conf = (DirConf*)c;
    conf->contributionPath = arg;
    conf->components = components(conf);
    return NULL;
}
const char *confComposite(unused cmd_parms *cmd, void *c, const char *arg) {
    DirConf* conf = (DirConf*)c;
    conf->compositeName = arg;
    conf->components = components(conf);
    return NULL;
}

/**
 * HTTP server module declaration.
 */
const command_rec commands[] = {
    AP_INIT_TAKE1("TuscanyHome", (const char*(*)())confHome, NULL, RSRC_CONF, "Tuscany home directory"),
    AP_INIT_TAKE1("SCAWiringHost", (const char*(*)())confWiringHost, NULL, RSRC_CONF, "SCA wiring host"),
    AP_INIT_TAKE1("SCAContribution", (const char*(*)())confContribution, NULL, ACCESS_CONF, "SCA contribution location"),
    AP_INIT_TAKE1("SCAComposite", (const char*(*)())confComposite, NULL, ACCESS_CONF, "SCA composite location"),
    {NULL, NULL, NULL, 0, NO_ARGS, NULL}
};

int postConfig(unused apr_pool_t *p, unused apr_pool_t *plog, unused apr_pool_t *ptemp, unused server_rec *s) {
   return OK;
}

void childInit(unused apr_pool_t* p, server_rec* svr_rec) {
    ServerConf *conf = (ServerConf*)ap_get_module_config(svr_rec->module_config, &mod_tuscany_wiring);
    if(conf == NULL) {
        std::cerr << "[Tuscany] Due to one or more errors mod_tuscany_wiring loading failed. Causing apache to stop loading." << std::endl;
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

module AP_MODULE_DECLARE_DATA mod_tuscany_wiring = {
    STANDARD20_MODULE_STUFF,
    // dir config and merger
    tuscany::httpd::makeDirConf<tuscany::server::modwiring::DirConf>, NULL,
    // server config and merger
    tuscany::httpd::makeServerConf<tuscany::server::modwiring::ServerConf>, NULL,
    // commands and hooks
    tuscany::server::modwiring::commands, tuscany::server::modwiring::registerHooks
};

}
