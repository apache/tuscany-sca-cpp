/*
 * Copyright 2004,2005 The Apache Software Foundation.
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
 
#include "axis2_Account_stub.h"


axis2_stub_t * 
axis2_Account_stub_create_with_endpoint_ref_and_client_home(const axis2_env_t *env,
                                            axis2_endpoint_ref_t *endpoint_ref,
                                            axis2_char_t *client_home)
{
    axis2_stub_t *stub = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
   
    stub = (axis2_stub_t *) 
    axis2_stub_create_with_endpoint_ref_and_client_home(env, endpoint_ref,
            client_home);
    if(NULL == stub)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_NO_MEMORY, (axis2_status_codes)AXIS2_FAILURE); 
        return NULL;
    }
    axis2_populate_axis_service( stub, env);
    return stub;
}

void axis2_populate_axis_service( axis2_stub_t* stub, const axis2_env_t *env)
{
   axis2_svc_client_t* svc_client = NULL;
   axis2_qname_t *op_qname =  NULL;
   axis2_svc_t* svc = NULL;
   axis2_op_t* op = NULL;

   /*Modifying the Service*/
   svc_client = AXIS2_STUB_GET_SVC_CLIENT (stub, env );
   svc = (axis2_svc_t*)AXIS2_SVC_CLIENT_GET_AXIS_SERVICE ( svc_client, env );

   /*creating the operations*/

   op_qname = axis2_qname_create(env,"getAccountReport" , "", NULL);
   op = axis2_op_create_with_qname(env, op_qname);
   AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, env, AXIS2_MEP_URI_OUT_IN);
   AXIS2_SVC_ADD_OP(svc, env, op);
   
 }

axis2_stub_t *
axis2_Account_stub_create_with_endpoint_uri_and_client_home(const axis2_env_t *env,
                                            const axis2_char_t *endpoint_uri,
                                            const axis2_char_t *client_home)
{
    axis2_stub_t *stub = NULL;
    
   AXIS2_ENV_CHECK(env, NULL);
   
   stub = (axis2_stub_t *) 
        axis2_stub_create_with_endpoint_uri_and_client_home(env, endpoint_uri,
            client_home);
   if(NULL == stub)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_NO_MEMORY, (axis2_status_codes)AXIS2_FAILURE); 
        return NULL;
    }
    
    axis2_populate_axis_service( stub, env);
    
   return stub;
}

/***************************Function implementation****************************/

axiom_node_t *
axis2_Account_stub_getAccountReport(axis2_stub_t *stub,
                        const axis2_env_t *env,
                        axiom_node_t *node)
{
    axis2_svc_client_t *svc_client = NULL;
    axiom_node_t *ret_node = NULL;
    axis2_qname_t *op_qname =  NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    op_qname = axis2_qname_create(env, "getAccountReport" , "", NULL);
    ret_node =  AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OP_QNAME( svc_client, env, op_qname, node);
    
    return ret_node;
}


