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
 * Axis2/C module that dispatches all server requests to the Tuscany Axis/2C service.
 */

#define WANT_HTTPD_LOG 1
#include "axis2.hpp"

namespace tuscany {
namespace webservice {

/**
 * Determine the service and operation to dispatch a request to.
 */
axis2_svc_t* AXIS2_CALL dispatchFindService(axis2_msg_ctx_t* msg_ctx, const axutil_env_t* env) {
    const axis2_conf_ctx_t* conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);
    const axis2_conf_t* conf = axis2_conf_ctx_get_conf(conf_ctx, env);
    axis2_svc_t* svc = axis2_conf_get_svc(conf, env, "TuscanyService");
    return svc;
}

axis2_op_t *AXIS2_CALL dispatchFindOp(unused axis2_msg_ctx_t* msg_ctx, const axutil_env_t* env, axis2_svc_t* svc) {
    axutil_qname_t* op_qname = axutil_qname_create(env, "execute", NULL, NULL);
    axis2_op_t *op = axis2_svc_get_op_with_name(svc, env, axutil_qname_get_localpart(op_qname, env));
    axutil_qname_free(op_qname, env);
    return op;
}

/**
 * Dispatcher invoke function, called by Axis2/C.
 */
axis2_status_t AXIS2_CALL dispatchInvoke( struct axis2_handler* handler, const axutil_env_t* env, axis2_msg_ctx_t* msg_ctx) {
    if (!(axis2_msg_ctx_get_server_side(msg_ctx, env)))
        return AXIS2_SUCCESS;
    axis2_msg_ctx_set_find_svc(msg_ctx, env, dispatchFindService);
    axis2_msg_ctx_set_find_op(msg_ctx, env, dispatchFindOp);
    return axis2_disp_find_svc_and_op(handler, env, msg_ctx);
}

/**
 * Create a dispatch handler.
 */
AXIS2_EXPORT axis2_handler_t* AXIS2_CALL dispatchHandler(const axutil_env_t* env, unused axutil_string_t* name) {
    axis2_handler_t *handler = axis2_handler_create(env);
    if (handler == NULL)
        return NULL;
    axis2_handler_set_invoke(handler, env, dispatchInvoke);
    return handler;
}

/**
 * Initialize dispatch module.
 */
axis2_status_t AXIS2_CALL dispatchInit(unused axis2_module_t * module, unused const axutil_env_t * env, unused axis2_conf_ctx_t * conf_ctx, unused axis2_module_desc_t * module_desc) {
    return AXIS2_SUCCESS;
}

/**
 * Initialize dispatch module function map.
 */
axis2_status_t AXIS2_CALL dispatchFuncMap(axis2_module_t * module, const axutil_env_t * env) {
    module->handler_create_func_map = axutil_hash_make(env);
    axutil_hash_set(module->handler_create_func_map, "TuscanyDispatcher", AXIS2_HASH_KEY_STRING, (const void *)dispatchHandler);
    return AXIS2_SUCCESS;
}

/**
 * Shutdown dispatch module.
 */
axis2_status_t AXIS2_CALL dispatchShutdown(axis2_module_t* module, const axutil_env_t* env) {
    if (module->handler_create_func_map != NULL) {
        axutil_hash_free(module->handler_create_func_map, env);
        module->handler_create_func_map = NULL;
    }
    AXIS2_FREE(env->allocator, module);
    return AXIS2_SUCCESS;
}

/**
 * Return a new dispatch module.
 */
const axis2_module_ops_t dispatchOps = {
    dispatchInit,
    dispatchShutdown,
    dispatchFuncMap
};

axis2_module_t* dispatchModule(const axutil_env_t* env) {
    axis2_module_t *module = (axis2_module_t*)AXIS2_MALLOC(env->allocator, sizeof(axis2_module_t));
    if (module == NULL)
        return NULL;
    module->ops = &dispatchOps;
    module->handler_create_func_map = NULL;
    return module;
}

}
}

extern "C"
{

/**
 * Axis2/C module entry point functions.
 */
AXIS2_EXPORT int axis2_get_instance(axis2_module_t** inst, const axutil_env_t* env) {
   *inst = tuscany::webservice::dispatchModule(env);
    if(*inst == NULL)
        return AXIS2_FAILURE;
    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(axis2_module_t* inst, const axutil_env_t* env) {
    if (inst != NULL)
         return tuscany::webservice::dispatchShutdown(inst, env);
    return AXIS2_FAILURE;
}

}
