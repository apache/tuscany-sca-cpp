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
#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#pragma warning(disable: 4091)
#endif

#include <axis2_handler_desc.h>
#include <axis2_qname.h>
#include <axis2_relates_to.h>
#include <axis2_svc.h>
#include <axis2_const.h>
#include <axis2_conf_ctx.h>
#include <axis2_addr.h>
#include <axis2_utils.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>

extern "C"
{                                                                                   

axis2_status_t AXIS2_CALL 
Axis2Dispatcher_invoke (
    axis2_handler_t * handler, 
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx);
                           
axis2_svc_t *AXIS2_CALL 
Axis2Dispatcher_find_svc(
    axis2_msg_ctx_t *msg_ctx,
    const axis2_env_t *env);
                            
axis2_op_t *AXIS2_CALL 
Axis2Dispatcher_find_op(
    axis2_msg_ctx_t *msg_ctx, 
    const axis2_env_t *env,
    axis2_svc_t *svc);

AXIS2_EXPORT axis2_handler_t* AXIS2_CALL
Axis2Dispatcher_create(const axis2_env_t *env, 
                         axis2_qname_t *qname) 
{
    axis2_handler_t *handler = NULL;
    
    handler = axis2_handler_create(env);
    if (!handler)
    {
        return NULL;
    }
   
    /* handler init is handled by conf loading, so no need to do it here */
    
    /* set the base struct's invoke op */
    if (handler->ops) 
        handler->ops->invoke = Axis2Dispatcher_invoke;

    return handler;
}

axis2_svc_t *AXIS2_CALL 
Axis2Dispatcher_find_svc(
    axis2_msg_ctx_t *msg_ctx, 
    const axis2_env_t *env) 
{    
    axis2_svc_t *svc = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);    
    
    axis2_conf_ctx_t *conf_ctx = NULL;
    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
    if (conf_ctx)
    {
        axis2_conf_t *conf = NULL;
        conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
        if (conf)
        {
            axis2_char_t* service_name = "TuscanyService";
            svc = AXIS2_CONF_GET_SVC(conf, env, service_name);
            if (svc)
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                    "Service found using target endpoint address");
            }
        }
    }                    

    return svc;
}

axis2_op_t *AXIS2_CALL 
Axis2Dispatcher_find_op(
    axis2_msg_ctx_t *msg_ctx, 
    const axis2_env_t *env,
    axis2_svc_t *svc)
{
    axis2_op_t *op = NULL;
    
    AXIS2_ENV_CHECK(env, NULL); 

    axis2_qname_t *op_qname = NULL;
    axis2_char_t* execute_op_name = "execute";
    op_qname = axis2_qname_create(env, execute_op_name, NULL, NULL);
    
    op = AXIS2_SVC_GET_OP_WITH_NAME(svc, env, AXIS2_QNAME_GET_LOCALPART(op_qname, env));
    
    AXIS2_QNAME_FREE(op_qname, env);
    if (op)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
            "TuscanyService execute operation found");
    }
    return op;
}
            
axis2_status_t AXIS2_CALL 
Axis2Dispatcher_invoke(
    axis2_handler_t * handler, 
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);    

    if (!(AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env)))
        return AXIS2_SUCCESS;
    
    msg_ctx->ops->find_svc = Axis2Dispatcher_find_svc;
    msg_ctx->ops->find_op = Axis2Dispatcher_find_op;
    
    axis2_svc_t *axis_service = NULL;
    axis2_op_t *op = NULL;
    
    axis_service = AXIS2_MSG_CTX_GET_SVC(msg_ctx, env);
        
    if (!axis_service) 
    {
        axis_service = AXIS2_MSG_CTX_FIND_SVC(msg_ctx, env);
        if (axis_service) 
        {
            AXIS2_MSG_CTX_SET_SVC(msg_ctx, env, axis_service);
            /*TODO Set the Service Group Context to the message Context*/
        }
    }
    op = AXIS2_MSG_CTX_GET_OP(msg_ctx, env);
    if (!op)
    {
        op = AXIS2_MSG_CTX_FIND_OP(msg_ctx, env, axis_service);
        
        if (op)
        {
            AXIS2_MSG_CTX_SET_OP(msg_ctx, env, op);
        }
    }

    return AXIS2_SUCCESS;
}

}
