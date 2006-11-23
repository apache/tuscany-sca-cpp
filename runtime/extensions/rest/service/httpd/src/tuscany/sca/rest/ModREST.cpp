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
typedef struct rest_server_config_rec
{
    char * home;
} rest_server_config_rec_t;

typedef struct rest_dir_config_rec
{
    char * root;
    char * path;
    char * component;
} rest_dir_config_rec_t;

extern module AP_MODULE_DECLARE_DATA sca_rest_module;

static const char *rest_set_home(cmd_parms *cmd, void *dummy, 
                        const char *arg);
                        
static const char *rest_set_path(cmd_parms *cmd, void *conf, 
                        const char *arg);
                        
static const char *rest_set_root(cmd_parms *cmd, void *conf, 
                        const char *arg);
                        
static const char *rest_set_component(cmd_parms *cmd, void *conf, 
                        const char *arg);

static void* rest_create_svr(apr_pool_t *p, server_rec *s);


static const char *rest_set_home(cmd_parms *cmd, void *dummy, 
                        const char *arg)
{
    rest_server_config_rec_t *conf = (rest_server_config_rec_t*)ap_get_module_config(
                        cmd->server->module_config, &sca_rest_module);
    conf->home = apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static const char *rest_set_path(cmd_parms *cmd, void *c, 
                        const char *arg)
{
    rest_dir_config_rec_t *conf = (rest_dir_config_rec_t*)c;
    conf->path = apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static const char *rest_set_root(cmd_parms *cmd, void *c, 
                        const char *arg)
{
    rest_dir_config_rec_t *conf = (rest_dir_config_rec_t*)c;
    conf->root = apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static const char *rest_set_component(cmd_parms *cmd, void *c, 
                        const char *arg)
{
    rest_dir_config_rec_t *conf = (rest_dir_config_rec_t*)c;
    conf->component = apr_pstrdup(cmd->pool, arg);
    return NULL;
}


static const command_rec rest_module_cmds[] = 
{
    AP_INIT_TAKE1("TuscanyHome", (const char*(*)())rest_set_home, NULL, RSRC_CONF,
                  "Tuscany home directory"),
    AP_INIT_TAKE1("TuscanyPath", (const char*(*)())rest_set_path, NULL, ACCESS_CONF,
                  "Tuscany SCA composite search path"),
    AP_INIT_TAKE1("TuscanyRoot", (const char*(*)())rest_set_root, NULL, ACCESS_CONF,
                  "Tuscany root SCA configuration path"),
    AP_INIT_TAKE1("TuscanyComponent", (const char*(*)())rest_set_component, NULL, ACCESS_CONF,
                  "SCA component name"),
    {NULL}
};


static int rest_handler(request_rec *request)
{
    
    if (strcmp(request->handler, "sca_rest_module"))
    {
        return DECLINED;
    }
    
    // Set up the read policy
    int rc = ap_setup_client_block(request, REQUEST_CHUNKED_DECHUNK);
    if (rc != OK)
    {
        return rc;
    }
    ap_should_client_block(request);
    
    const char* http_version = request->protocol;
    
    const char* content_type = apr_table_get(request->headers_in, "Content-Type");
    request->content_type = content_type;
    
    int content_length = request->remaining;
    if (request->read_chunked == true && content_length == 0)
    {
        content_length = -1;
        request->chunked = true;
    }

    // Set the content type    
    ap_set_content_type(request, "text/html");
    
    // Send the response document
    ap_rputs("<html><body><p>mod_rest works!", request);

    rest_server_config_rec_t* server_conf = (rest_server_config_rec_t*)ap_get_module_config(request->server->module_config, &sca_rest_module);
    ap_rprintf(request, "<p>home: %s", server_conf->home);

    rest_dir_config_rec_t* dir_conf = (rest_dir_config_rec_t*)ap_get_module_config(request->per_dir_config, &sca_rest_module);
    ap_rprintf(request, "<p>root: %s", dir_conf->root);
    if (dir_conf->path)
    {
        ap_rprintf(request, "<p>path: %s", dir_conf->path);
    }
    if (dir_conf->path)
    {
        ap_rprintf(request, "<p>component: %s", dir_conf->component);
    }
    
    ap_rputs("</body></html>", request);
    
    //return HTTP_INTERNAL_SERVER_ERROR;
    return OK;
}

static int rest_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp,
                       server_rec *s)
{
    return OK;
}

static void rest_child_init(apr_pool_t* p, server_rec* svr_rec)
{
    rest_server_config_rec_t *conf = (rest_server_config_rec_t*)ap_get_module_config(
                        svr_rec->module_config, &sca_rest_module); 

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

static void *rest_create_dir_config(apr_pool_t *p, char *dirspec)
{
    rest_dir_config_rec_t* conf = (rest_dir_config_rec_t* )apr_palloc(p, sizeof(*conf));
    conf->path = NULL;
    conf->root = NULL;
    conf->component = NULL;
    return conf;
}

static void* rest_create_server_config(apr_pool_t *p, server_rec *s)
{
    rest_server_config_rec_t* conf = (rest_server_config_rec_t* )apr_palloc(p, sizeof(*conf));
    conf->home = NULL;
    return conf;
}

module AP_MODULE_DECLARE_DATA sca_rest_module =
{
    STANDARD20_MODULE_STUFF,
    rest_create_dir_config,         /* dir config */
    NULL,                       /* dir merger --- default is to override */
    rest_create_server_config,        /* server config */
    NULL,                       /* merge server config */
    rest_module_cmds,         /* command table */
    register_hooks              /* register_hooks */
};

}
