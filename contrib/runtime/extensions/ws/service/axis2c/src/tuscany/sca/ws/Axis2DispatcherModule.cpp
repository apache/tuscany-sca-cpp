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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#pragma warning(disable: 4091)
#endif

#include <axis2_module.h>
#include <axis2_addr_mod.h>
#include <axis2_conf_ctx.h>
#include <axis2_disp.h>

extern "C"
{

axis2_status_t AXIS2_CALL
Axis2DispatcherModule_shutdown(axis2_module_t *module,
                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
Axis2DispatcherModule_init(
        axis2_module_t *module,
        const axis2_env_t *env,
        axis2_conf_ctx_t *conf_ctx,
        axis2_module_desc_t *module_desc);

axis2_status_t AXIS2_CALL
Axis2DispatcherModule_fill_handler_create_func_map(axis2_module_t *module,
                                            const axis2_env_t *env);

AXIS2_EXTERN axis2_handler_t* AXIS2_CALL
Axis2Dispatcher_create(const axis2_env_t *env, 
                         axis2_qname_t *qname); 
     
axis2_module_t *
Axis2DispatcherModule_create(const axis2_env_t *env)
{
    axis2_module_t *module = NULL;
    module = (axis2_module_t*)AXIS2_MALLOC(env->allocator, 
        sizeof(axis2_module_t));

    
    module->ops = (axis2_module_ops_t*)AXIS2_MALLOC(
        env->allocator, sizeof(axis2_module_ops_t));

    module->ops->shutdown = Axis2DispatcherModule_shutdown;
    module->ops->init = Axis2DispatcherModule_init;
    module->ops->fill_handler_create_func_map = 
        Axis2DispatcherModule_fill_handler_create_func_map;

    return module;
}

axis2_status_t AXIS2_CALL
Axis2DispatcherModule_init(
        axis2_module_t *module,
        const axis2_env_t *env,
        axis2_conf_ctx_t *conf_ctx,
        axis2_module_desc_t *module_desc)
{
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
Axis2DispatcherModule_shutdown(axis2_module_t *module,
                        const axis2_env_t *env)
{
    if(module->ops)
    {
        AXIS2_FREE(env->allocator, module->ops);
        module->ops = NULL;
    }

    if(module->handler_create_func_map)
    {
        axis2_hash_free(module->handler_create_func_map, env);
        module->handler_create_func_map = NULL;
    }
    
    if(module)
    {
        AXIS2_FREE(env->allocator, module);
        module = NULL;
    }
    return AXIS2_SUCCESS; 
}

axis2_status_t AXIS2_CALL
Axis2DispatcherModule_fill_handler_create_func_map(axis2_module_t *module,
                                            const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    module->handler_create_func_map = axis2_hash_make(env);
    axis2_hash_set(module->handler_create_func_map, "TuscanyDispatcher", 
        (axis2_ssize_t)AXIS2_HASH_KEY_STRING, (const void *)Axis2Dispatcher_create);

    return AXIS2_SUCCESS;
}

/**
 * Following block distinguish the exposed part of the dll.
 */

AXIS2_EXPORT int 
axis2_get_instance(axis2_module_t **inst,
                   const axis2_env_t *env)
{
   *inst = Axis2DispatcherModule_create(env);
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int 
axis2_remove_instance(axis2_module_t *inst,
                      const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
   if (inst)
   {
        status = Axis2DispatcherModule_shutdown(inst, env);
    }
    return status;
}

}
