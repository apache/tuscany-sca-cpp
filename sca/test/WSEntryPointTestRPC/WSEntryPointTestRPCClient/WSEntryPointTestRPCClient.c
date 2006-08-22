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

#include <stdio.h>
#include <axiom.h>
#include <axis2_util.h>
#include <axiom_soap.h>
#include <axis2_client.h>

axiom_node_t *
build_om_payload_for_WSEntryPointTestRPC_svc(axis2_env_t *env);


int main(int argc, char** argv)
{
    axis2_env_t *env = NULL;
    axis2_char_t *address = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    axis2_char_t *client_home = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axiom_node_t *payload = NULL;
    axiom_node_t *ret_node = NULL;
   
    /* Set up the envioronment */
    env = axis2_env_create_all("wsentrypointtestrpc.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of echo service */
    address = "http://localhost:9090/axis2/services/WSEntryPointTestRPC";
    if (argc > 1 )
        address = argv[1];
    if (AXIS2_STRCMP(address, "-h") == 0)
    {
        printf("Usage : %s [endpoint_url]\n", argv[0]);
        printf("use -h for help\n");
        return 0;
    }
    printf ("Using endpoint : %s\n", address);
    
    /* Create EPR with given address */
    endpoint_ref = axis2_endpoint_ref_create(env, address);

    /* Setup options */
    options = axis2_options_create(env);
    AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);
    AXIS2_OPTIONS_SET_ACTION(options, env,
        "http://www.WSEntryPointTest.org/WSEntryPointTestRPC/doString");

    /* Set up deploy folder. It is from the deploy folder, the configuration is picked up 
     * using the axis2.xml file.
     * In this sample client_home points to the Axis2/C default deploy folder. The client_home can 
     * be different from this folder on your system. For example, you may have a different folder 
     * (say, my_client_folder) with its own axis2.xml file. my_client_folder/packages will have the 
     * composites that the client uses
     */
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
    {
        printf("Please ensure AXIS2C_HOME is set as an env variable\n");
        return 0;
    }

    /* Create service client */
    svc_client = axis2_svc_client_create(env, client_home);
    if (!svc_client)
    {
        printf("Error creating service client\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
    }

    /* Set service client options */
    AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, &env, options);    
    
    /* Engage addressing composite */
    AXIS2_SVC_CLIENT_ENGAGE_COMPOSITE(svc_client, &env, AXIS2_COMPOSITE_ADDRESSING);
    
    /* Build the SOAP request message payload using OM API.*/
    payload = build_om_payload_for_WSEntryPointTestRPC_svc(env);
    
    /* Send request */
    ret_node = AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload);
    
    if(ret_node)
    {
        axis2_char_t *om_str = NULL;
        om_str = AXIOM_NODE_TO_STRING(ret_node, env);
        if (om_str)
            printf("\nReceived OM : %s\n", om_str);
        printf("\nWSEntryPointTestRPC client invoke SUCCESSFUL!\n");
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("WSEntryPointTestRPC client invoke FAILED!\n");
    }
    
    if (svc_client)
    {
        AXIS2_SVC_CLIENT_FREE(svc_client, env);
        svc_client = NULL;
    }
    if (endpoint_ref)
    {
        AXIS2_ENDPOINT_REF_FREE(endpoint_ref, &env);
        endpoint_ref = NULL;
    }
    return 0;
}

/* build SOAP request message content using OM */
axiom_node_t *
build_om_payload_for_WSEntryPointTestRPC_svc(axis2_env_t *env)
{
    axiom_node_t* req_om_node = NULL;
    axiom_element_t* req_om_elem = NULL;
    axiom_node_t* param_om_node = NULL;
    axiom_element_t* param_om_elem = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;
    
    ns1 = axiom_namespace_create (env, "http://www.WSEntryPointTest.org/WSEntryPointTestRPC/", "ns1");
    req_om_elem = axiom_element_create(env, NULL, "doString", ns1, &req_om_node);
    param_om_elem = axiom_element_create(env, req_om_node, "doStringRequest", ns1, &param_om_node);
    AXIOM_ELEMENT_SET_TEXT(param_om_elem, env, "WSEntryPointTestRPC Client Call", param_om_node);
    
    om_str = AXIOM_NODE_TO_STRING(req_om_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return req_om_node;
}
