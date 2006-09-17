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

#include <axis2_disp.h>
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

axis2_disp_t *AXIS2_CALL 
Axis2Dispatcher_create(
    const axis2_env_t *env) 
{
    axis2_disp_t *disp = NULL;
    axis2_handler_t *handler = NULL;
    axis2_qname_t *qname = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);

    qname = axis2_qname_create(env, "tuscany_dispatcher", 
                                                "http://tuscany.ws.apache.org",
                                                NULL);
    
    disp = axis2_disp_create(env, qname);
    if (!disp)
    { 
        return NULL;        
    }
    
    handler = AXIS2_DISP_GET_BASE(disp, env);
    if (!handler)
    {
        return NULL;        
    }

    handler->ops->invoke = Axis2Dispatcher_invoke;
    
    AXIS2_QNAME_FREE(qname, env);
    
    return disp;
}

axis2_svc_t *AXIS2_CALL 
Axis2Dispatcher_find_svc(
    axis2_msg_ctx_t *msg_ctx, 
    const axis2_env_t *env) 
{    
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    axis2_svc_t *svc = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);    
    
    endpoint_ref = AXIS2_MSG_CTX_GET_TO(msg_ctx, env);
    
    if (endpoint_ref)
    {
        axis2_char_t *address = NULL;        

        address = AXIS2_ENDPOINT_REF_GET_ADDRESS(endpoint_ref, env);
        if (address)
        {
            
            axis2_char_t **url_tokens = NULL;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                "Checking for TuscanyService using target endpoint address : %s", address);
            
            url_tokens = axis2_parse_request_url_for_svc_and_op(env, address);
            
            if (url_tokens)
            {                
                if (url_tokens[0])
                {                    
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
                                    
                                // TODO store the URL token in the message context
                                // url_tokens[0]
                            }
                        }
                    }                    
                }
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
    axiom_soap_envelope_t *soap_envelope = NULL;
    axis2_op_t *op = NULL;
    
    AXIS2_ENV_CHECK(env, NULL); 
    //AXIS2_PARAM_CHECK(env->error, svc, NULL);

    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    if (soap_envelope)
    {
        axiom_soap_body_t *soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
        if (soap_body)
        {
            axiom_node_t *body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
            if (body_node)
            {
                axiom_node_t *body_first_child_node = 
                    AXIOM_NODE_GET_FIRST_CHILD(body_node, env);
                if (body_first_child_node)
                {                    
                    if (AXIOM_NODE_GET_NODE_TYPE(body_first_child_node, env) == AXIOM_ELEMENT)
                    {
                        axiom_element_t *element = NULL;                        
                        element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(body_first_child_node, env);
                        if (element)
                        {
                            axis2_char_t *element_name = AXIOM_ELEMENT_GET_LOCALNAME(element, env);
                            if (element_name)
                            {
                                axis2_qname_t *op_qname = NULL;
                                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                                    "Checking for operation using SOAP message body's first child's local name : %s", 
                                        element_name);
                                        
                                axis2_char_t* execute_op_name = "execute";
                                op_qname = axis2_qname_create(env, execute_op_name, NULL, NULL);
                                
                                op = AXIS2_SVC_GET_OP_WITH_NAME(svc, env, AXIS2_QNAME_GET_LOCALPART(op_qname, env));
                                
                                AXIS2_QNAME_FREE(op_qname, env);
                                if (op)
                                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                                        "Operation found using SOAP message body's first child's local name");
                                
                            }
                        }
                    }
                }
            }
        }
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
    
    msg_ctx->ops->find_svc = Axis2Dispatcher_find_svc;
    msg_ctx->ops->find_op = Axis2Dispatcher_find_op;
    
    return axis2_disp_invoke(handler, env, msg_ctx);
}

/**
 * Following block distinguish the exposed part of the dll.
 */

AXIS2_EXPORT int axis2_get_instance(axis2_disp_t **inst,
                        axis2_env_t *env)
{
    *inst = Axis2Dispatcher_create(env);
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(axis2_disp_t *inst,
                            axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
	if (inst)
	{
        status = AXIS2_HANDLER_FREE(inst, env);
    }
    return status;
}
}
