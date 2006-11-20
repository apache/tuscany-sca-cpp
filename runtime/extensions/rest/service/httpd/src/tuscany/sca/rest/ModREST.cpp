/*
 * Copyright 2006 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "apr_strings.h"
#include "apr_fnmatch.h"
#include "apr_strings.h"
#include "apr_lib.h"

#define APR_WANT_STRFUNC
#include "apr_want.h"

#include "ap_config.h"
#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_request.h"
#include "http_protocol.h"
#include "http_log.h"
#include "http_main.h"
#include "util_script.h"

#include "mod_core.h"


extern "C"
{                                                                                   

/* Configuration structure populated by apache2.conf */
typedef struct rest_config_rec
{
    char * home;
    char * root;
    char * path;
    char * component;
} rest_config_rec_t;

extern module AP_MODULE_DECLARE_DATA rest_module;

static const char *rest_set_home(cmd_parms *cmd, void *dummy, 
                        const char *arg);
                        
static const char *rest_set_path(cmd_parms *cmd, void *dummy, 
                        const char *arg);
                        
static const char *rest_set_root(cmd_parms *cmd, void *dummy, 
                        const char *arg);
                        
static const char *rest_set_component(cmd_parms *cmd, void *dummy, 
                        const char *arg);

static void* rest_create_svr(apr_pool_t *p, server_rec *s);


static const char *rest_set_home(cmd_parms *cmd, void *dummy, 
                        const char *arg)
{
    rest_config_rec_t *conf = (rest_config_rec_t*)ap_get_module_config(
                        cmd->server->module_config, &rest_module);
    conf->home = apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static const char *rest_set_path(cmd_parms *cmd, void *dummy, 
                        const char *arg)
{
    rest_config_rec_t *conf = (rest_config_rec_t*)ap_get_module_config(
                        cmd->server->module_config, &rest_module);
    conf->path = apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static const char *rest_set_root(cmd_parms *cmd, void *dummy, 
                        const char *arg)
{
    rest_config_rec_t *conf = (rest_config_rec_t*)ap_get_module_config(
                        cmd->server->module_config, &rest_module);
    conf->root = apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static const char *rest_set_component(cmd_parms *cmd, void *dummy, 
                        const char *arg)
{
    rest_config_rec_t *conf = (rest_config_rec_t*)ap_get_module_config(
                        cmd->server->module_config, &rest_module);
    conf->component = apr_pstrdup(cmd->pool, arg);
    return NULL;
}


static const command_rec rest_module_cmds[] = 
{
    AP_INIT_TAKE1("TuscanyHome", (const char*(*)())rest_set_home, NULL, ACCESS_CONF,
                  "Tuscany home directory"),
    AP_INIT_TAKE1("TuscanyPath", (const char*(*)())rest_set_path, NULL, ACCESS_CONF,
                  "Tuscany SCA composite search path"),
    AP_INIT_TAKE1("TuscanyRoot", (const char*(*)())rest_set_root, NULL, ACCESS_CONF,
                  "Tuscany root SCA configuration path"),
    AP_INIT_TAKE1("TuscanyComponent", (const char*(*)())rest_set_component, NULL, ACCESS_CONF,
                  "SCA component name"),
    {NULL}
};


static int rest_handler(request_rec *r)
{
    return OK;
}

static int rest_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp,
                       server_rec *s)
{
    return OK;
}

static void rest_child_init(apr_pool_t* p, server_rec* svr_rec)
{
    rest_config_rec_t *conf = (rest_config_rec_t*)ap_get_module_config(
                        svr_rec->module_config, &rest_module); 

    
    if(false)
    {
        fprintf(stderr, "[Tuscany] Due to one or more errors mod_rest loading"
                        " failed. Causing apache2 to stop loading\n");
        exit(APEXIT_CHILDFATAL);
    }
}

static void register_hooks(apr_pool_t *p)
{
    ap_hook_handler(rest_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_post_config(rest_init, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(rest_child_init, NULL, NULL, APR_HOOK_MIDDLE);
}

static void* rest_create_svr(apr_pool_t *p, server_rec *s)
{
    rest_config_rec_t* conf = (rest_config_rec_t* )apr_palloc(p, sizeof(*conf));
    conf->home = NULL;
    conf->path = NULL;
    conf->root = NULL;
    conf->component = NULL;
    return conf;
}

module AP_MODULE_DECLARE_DATA rest_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,                       /* dir config creater */
    NULL,                       /* dir merger --- default is to override */
    rest_create_svr,        /* server config */
    NULL,                       /* merge server config */
    rest_module_cmds,         /* command table */
    register_hooks              /* register_hooks */
};


}
