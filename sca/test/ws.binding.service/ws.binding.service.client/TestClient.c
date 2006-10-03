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
build_om_payload_for_doDataObject(axis2_env_t *env);

axiom_node_t *
build_om_payload_for_doDataObject_Failure(axis2_env_t *env);

axiom_node_t *
build_om_payload_for_doAny(axis2_env_t *env);

axiom_node_t *
build_om_payload_for_doMixed(axis2_env_t *env);

axiom_node_t *
build_om_payload_for_doSingleData(axis2_env_t *env, char* name, char* data);

void runCall(axis2_env_t* env, char* endpoint, char* opName, axiom_node_t* payload, int expectFailure);

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
    env = axis2_env_create_all("ws.binding.service.test.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of echo service */
    address = "http://localhost:9090/axis2/services/ws.binding.service";
    if (argc > 1 )
        address = argv[1];
    if (AXIS2_STRCMP(address, "-h") == 0)
    {
        printf("Usage : %s [endpoint_url]\n", argv[0]);
        printf("use -h for help\n");
        return 0;
    }
    printf ("Using endpoint : %s\n", address);


    /* do all the single data types */
    payload = build_om_payload_for_doSingleData(env, "doChars", "Here is some char* data");
    runCall(env, address, "doChars", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doLong", "123456789");
    runCall(env, address, "doLong", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doInt", "123456789");
    runCall(env, address, "doInt", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doFloat", "123.45");
    runCall(env, address, "doFloat", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doLongDouble", "12345.67891");
    runCall(env, address, "doLongDouble", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doDouble", "1234.4567");
    runCall(env, address, "doDouble", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doBool", "true");
    runCall(env, address, "doBool", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doShort", "12345");
    runCall(env, address, "doShort", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doBytes", "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef");
    runCall(env, address, "doBytes", payload, 0);
    payload = build_om_payload_for_doSingleData(env, "doByte", "66");
    runCall(env, address, "doByte", payload, 0);
    payload = build_om_payload_for_doDataObject(env);
    runCall(env, address, "doDataObject", payload, 0);
    payload = build_om_payload_for_doMixed(env);
    runCall(env, address, "doMixed", payload, 0);
    payload = build_om_payload_for_doAny(env);
    runCall(env, address, "doAny", payload, 0);

    payload = build_om_payload_for_doDataObject_Failure(env);
    runCall(env, address, "doDataObject", payload, 1);

    return 0;
}

void runCall(axis2_env_t *env, char* endpoint, char* opName, axiom_node_t* payload, int expectFailure)
{
    axis2_char_t *address = endpoint;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    axis2_char_t *client_home = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axiom_node_t *ret_node = NULL;       
    char action [100];
    int len = 0;

    printf("runSingleDataCall with endpoint %s, opName %s\n", endpoint, opName);

    client_home = AXIS2_GETENV("AXIS2C_HOME");

    /* Create EPR with given address */
    endpoint_ref = axis2_endpoint_ref_create(env, address);

    svc_client = axis2_svc_client_create(env, client_home);
    if (!svc_client)
    {
        printf("Error creating service client\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
    }

    options = axis2_options_create(env);

    AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);

    len = sprintf(action, "http://www.WSEntryPointTest.org/WSEntryPointTest/%s", opName);
    AXIS2_OPTIONS_SET_ACTION(options, env, action);

    /* Set service client options */
    AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, env, options);    

    /* Engage addressing composite */
    AXIS2_SVC_CLIENT_ENGAGE_MODULE(svc_client, env, AXIS2_MODULE_ADDRESSING);
    
    /* Send request */
    ret_node = AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload);
    if(ret_node)
    {
        axis2_char_t *om_str = NULL;
        om_str = AXIOM_NODE_TO_STRING(ret_node, env);
        if (om_str)
            printf("\nReceived OM : %s\n", om_str);
        printf("\nWSEntryPointTest %s invoke SUCCESSFUL!\n", opName);
    }
    else
    {
        if(expectFailure)
        {
            printf("\nWSEntryPointTest %s failure as expected: SUCCESSFUL!\n", opName);
        }
        else
        {
		    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
						    " %d :: %s", env->error->error_number,
                            AXIS2_ERROR_GET_MESSAGE(env->error));
            printf("WSEntryPointTest %s invoke FAILED!\n", opName);
        }
    }

    if (svc_client)
    {
        AXIS2_SVC_CLIENT_FREE(svc_client, env);
        svc_client = NULL;
    }
    if (endpoint_ref)
    {
        AXIS2_ENDPOINT_REF_FREE(endpoint_ref, env);
        endpoint_ref = NULL;
    }
}

/* build SOAP request message content using OM */
axiom_node_t *
build_om_payload_for_doDataObject(axis2_env_t *env)
{
    axiom_node_t* req_om_node = NULL;
    axiom_element_t* req_om_elem = NULL;
    axiom_node_t* someData_om_node = NULL;
    axiom_element_t* someData_om_elem = NULL;
    axiom_node_t* someStringData_om_node = NULL;
    axiom_element_t* someStringData_om_elem = NULL;
    axiom_node_t* someIntData_om_node = NULL;
    axiom_element_t* someIntData_om_elem = NULL;
    axiom_node_t* someFloatData_om_node = NULL;
    axiom_element_t* someFloatData_om_elem = NULL;
    axiom_node_t* someDateData_om_node = NULL;
    axiom_element_t* someDateData_om_elem = NULL;
    axiom_node_t* someListData_om_node = NULL;
    axiom_element_t* someListData_om_elem = NULL;
    axiom_node_t* listItem1_om_node = NULL;
    axiom_element_t* listItem1_om_elem = NULL;
    axiom_node_t* listItem2_om_node = NULL;
    axiom_element_t* listItem2_om_elem = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;

    ns1 = axiom_namespace_create (env, "http://www.WSEntryPointTest.org/WSEntryPointTest/", "ns1");
    req_om_elem = axiom_element_create(env, NULL, "doDataObject", ns1, &req_om_node);
    someData_om_elem = axiom_element_create(env, req_om_node, "someData", ns1, &someData_om_node);
    someStringData_om_elem = axiom_element_create(env, someData_om_node, "someStringData", ns1, &someStringData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someStringData_om_elem, env, "Here is some string data", someStringData_om_node);
    someIntData_om_elem = axiom_element_create(env, someData_om_node, "someIntData", ns1, &someIntData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someIntData_om_elem, env, "12345678", someIntData_om_node);
    someFloatData_om_elem = axiom_element_create(env, someData_om_node, "someFloatData", ns1, &someFloatData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someFloatData_om_elem, env, "1234.5678", someFloatData_om_node);
    someDateData_om_elem = axiom_element_create(env, someData_om_node, "someDateData", ns1, &someDateData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someDateData_om_elem, env, "1977-06-27", someDateData_om_node);
    someListData_om_elem = axiom_element_create(env, someData_om_node, "someListData", ns1, &someListData_om_node);
    listItem1_om_elem = axiom_element_create(env, someListData_om_node, "listItem", ns1, &listItem1_om_node);
    AXIOM_ELEMENT_SET_TEXT(listItem1_om_elem, env, "Here is list item 1", listItem1_om_node);
    listItem2_om_elem = axiom_element_create(env, someListData_om_node, "listItem", ns1, &listItem2_om_node);
    AXIOM_ELEMENT_SET_TEXT(listItem2_om_elem, env, "Here is list item 2", listItem2_om_node);
    
    om_str = AXIOM_NODE_TO_STRING(req_om_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return req_om_node;
}

/* build SOAP request message content using OM */
axiom_node_t *
build_om_payload_for_doDataObject_Failure(axis2_env_t *env)
{
    axiom_node_t* req_om_node = NULL;
    axiom_element_t* req_om_elem = NULL;
    axiom_node_t* someData_om_node = NULL;
    axiom_element_t* someData_om_elem = NULL;
    axiom_node_t* someStringData_om_node = NULL;
    axiom_element_t* someStringData_om_elem = NULL;
    axiom_node_t* someIntData_om_node = NULL;
    axiom_element_t* someIntData_om_elem = NULL;
    axiom_node_t* someFloatData_om_node = NULL;
    axiom_element_t* someFloatData_om_elem = NULL;
    axiom_node_t* someDateData_om_node = NULL;
    axiom_element_t* someDateData_om_elem = NULL;
    axiom_node_t* someListData_om_node = NULL;
    axiom_element_t* someListData_om_elem = NULL;
    axiom_node_t* listItem1_om_node = NULL;
    axiom_element_t* listItem1_om_elem = NULL;
    axiom_node_t* listItem2_om_node = NULL;
    axiom_element_t* listItem2_om_elem = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;

    // Create bad Axiom doc
    ns1 = axiom_namespace_create (env, "http://www.WSEntryPointTest.org/WSEntryPointTest/", "ns1");
    req_om_elem = axiom_element_create(env, NULL, "doDataObject", ns1, &req_om_node);
    someData_om_elem = axiom_element_create(env, req_om_node, "someUnknownData", ns1, &someData_om_node);
    someStringData_om_elem = axiom_element_create(env, someData_om_node, "someStringData", ns1, &someStringData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someStringData_om_elem, env, "Here is some string data", someStringData_om_node);
    someIntData_om_elem = axiom_element_create(env, someData_om_node, "someIntData", ns1, &someIntData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someIntData_om_elem, env, "12345678", someIntData_om_node);
    someFloatData_om_elem = axiom_element_create(env, someData_om_node, "someFloatData", ns1, &someFloatData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someFloatData_om_elem, env, "1234.5678", someFloatData_om_node);
    someDateData_om_elem = axiom_element_create(env, someData_om_node, "someDateData", ns1, &someDateData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someDateData_om_elem, env, "1977-06-27", someDateData_om_node);
    someListData_om_elem = axiom_element_create(env, someData_om_node, "someListData", ns1, &someListData_om_node);
    listItem1_om_elem = axiom_element_create(env, someListData_om_node, "listItem", ns1, &listItem1_om_node);
    AXIOM_ELEMENT_SET_TEXT(listItem1_om_elem, env, "Here is list item 1", listItem1_om_node);
    listItem2_om_elem = axiom_element_create(env, someListData_om_node, "listItem", ns1, &listItem2_om_node);
    AXIOM_ELEMENT_SET_TEXT(listItem2_om_elem, env, "Here is list item 2", listItem2_om_node);
    
    om_str = AXIOM_NODE_TO_STRING(req_om_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return req_om_node;
}



/* build SOAP request message content using OM */
axiom_node_t *
build_om_payload_for_doMixed(axis2_env_t *env)
{
    axiom_node_t* req_om_node = NULL;
    axiom_element_t* req_om_elem = NULL;
    axiom_node_t* arg1_om_node = NULL;
    axiom_element_t* arg1_om_elem = NULL;
    axiom_node_t* arg2_om_node = NULL;
    axiom_element_t* arg2_om_elem = NULL;
    axiom_node_t* arg3_om_node = NULL;
    axiom_element_t* arg3_om_elem = NULL;
    axiom_node_t* arg4_om_node = NULL;
    axiom_element_t* arg4_om_elem = NULL;
    axiom_node_t* arg5_om_node = NULL;
    axiom_element_t* arg5_om_elem = NULL;
    axiom_node_t* someStringData_om_node = NULL;
    axiom_element_t* someStringData_om_elem = NULL;
    axiom_node_t* someIntData_om_node = NULL;
    axiom_element_t* someIntData_om_elem = NULL;
    axiom_node_t* someFloatData_om_node = NULL;
    axiom_element_t* someFloatData_om_elem = NULL;
    axiom_node_t* someDateData_om_node = NULL;
    axiom_element_t* someDateData_om_elem = NULL;
    axiom_node_t* someListData_om_node = NULL;
    axiom_element_t* someListData_om_elem = NULL;
    axiom_node_t* listItem1_om_node = NULL;
    axiom_element_t* listItem1_om_elem = NULL;
    axiom_node_t* listItem2_om_node = NULL;
    axiom_element_t* listItem2_om_elem = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;

    ns1 = axiom_namespace_create (env, "http://www.WSEntryPointTest.org/WSEntryPointTest/", "ns1");
    req_om_elem = axiom_element_create(env, NULL, "doMixed", ns1, &req_om_node);
    arg1_om_elem = axiom_element_create(env, req_om_node, "arg1", ns1, &arg1_om_node);
    AXIOM_ELEMENT_SET_TEXT(arg1_om_elem, env, "Here is some char* data", arg1_om_node);
    arg2_om_elem = axiom_element_create(env, req_om_node, "arg2", ns1, &arg2_om_node);
    AXIOM_ELEMENT_SET_TEXT(arg2_om_elem, env, "123456789", arg2_om_node);
    arg3_om_elem = axiom_element_create(env, req_om_node, "arg3", ns1, &arg3_om_node);
    arg4_om_elem = axiom_element_create(env, req_om_node, "arg4", ns1, &arg4_om_node);
    AXIOM_ELEMENT_SET_TEXT(arg4_om_elem, env, "true", arg4_om_node);
    arg5_om_elem = axiom_element_create(env, req_om_node, "arg5", ns1, &arg5_om_node);
    AXIOM_ELEMENT_SET_TEXT(arg5_om_elem, env, "1234.5678", arg5_om_node);
    someStringData_om_elem = axiom_element_create(env, arg3_om_node, "someStringData", ns1, &someStringData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someStringData_om_elem, env, "Here is some string data", someStringData_om_node);
    someIntData_om_elem = axiom_element_create(env, arg3_om_node, "", ns1, &someIntData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someIntData_om_elem, env, "12345678", someIntData_om_node);
    someFloatData_om_elem = axiom_element_create(env, arg3_om_node, "someFloatData", ns1, &someFloatData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someFloatData_om_elem, env, "1234.5678", someFloatData_om_node);
    someDateData_om_elem = axiom_element_create(env, arg3_om_node, "someDateData", ns1, &someDateData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someDateData_om_elem, env, "1977-06-27", someDateData_om_node);
    someListData_om_elem = axiom_element_create(env, arg3_om_node, "someListData", ns1, &someListData_om_node);
    listItem1_om_elem = axiom_element_create(env, someListData_om_node, "listItem", ns1, &listItem1_om_node);
    AXIOM_ELEMENT_SET_TEXT(listItem1_om_elem, env, "Here is list item 1", listItem1_om_node);
    listItem2_om_elem = axiom_element_create(env, someListData_om_node, "listItem", ns1, &listItem2_om_node);
    AXIOM_ELEMENT_SET_TEXT(listItem2_om_elem, env, "Here is list item 2", listItem2_om_node);
    
    om_str = AXIOM_NODE_TO_STRING(req_om_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return req_om_node;
}


axiom_node_t *
build_om_payload_for_doSingleData(axis2_env_t *env, char* name, char* data)
{
    axiom_node_t* req_om_node = NULL;
    axiom_element_t* req_om_elem = NULL;
    axiom_node_t* someData_om_node = NULL;
    axiom_element_t* someData_om_elem = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;

    ns1 = axiom_namespace_create (env, "http://www.WSEntryPointTest.org/WSEntryPointTest/", "ns1");
    req_om_elem = axiom_element_create(env, NULL, name, ns1, &req_om_node);
    someData_om_elem = axiom_element_create(env, req_om_node, "someData", ns1, &someData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someData_om_elem, env, data, someData_om_node);
    
    om_str = AXIOM_NODE_TO_STRING(req_om_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return req_om_node;
}


axiom_node_t *
build_om_payload_for_doAny(axis2_env_t *env)
{
    axiom_node_t* req_om_node = NULL;
    axiom_element_t* req_om_elem = NULL;
    axiom_node_t* someData_om_node = NULL;
    axiom_element_t* someData_om_elem = NULL;
    axiom_node_t* someStringData_om_node = NULL;
    axiom_element_t* someStringData_om_elem = NULL;
    axiom_node_t* someIntData_om_node = NULL;
    axiom_element_t* someIntData_om_elem = NULL;
    axiom_node_t* someFloatData_om_node = NULL;
    axiom_element_t* someFloatData_om_elem = NULL;
    axiom_node_t* someDateData_om_node = NULL;
    axiom_element_t* someDateData_om_elem = NULL;
    axiom_node_t* someListData_om_node = NULL;
    axiom_element_t* someListData_om_elem = NULL;
    axiom_node_t* listItem1_om_node = NULL;
    axiom_element_t* listItem1_om_elem = NULL;
    axiom_node_t* listItem2_om_node = NULL;
    axiom_element_t* listItem2_om_elem = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;

    ns1 = axiom_namespace_create (env, "http://www.WSEntryPointTest.org/WSEntryPointTest/", "ns1");
    req_om_elem = axiom_element_create(env, NULL, "doAny", ns1, &req_om_node);
    someData_om_elem = axiom_element_create(env, req_om_node, "anElem", ns1, &someData_om_node);
    someStringData_om_elem = axiom_element_create(env, someData_om_node, "hello", ns1, &someStringData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someStringData_om_elem, env, "world", someStringData_om_node);
    someIntData_om_elem = axiom_element_create(env, someData_om_node, "someIntData", ns1, &someIntData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someIntData_om_elem, env, "12345678", someIntData_om_node);
    someFloatData_om_elem = axiom_element_create(env, someData_om_node, "someFloatData", ns1, &someFloatData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someFloatData_om_elem, env, "1234.5678", someFloatData_om_node);
    someDateData_om_elem = axiom_element_create(env, someData_om_node, "someDateData", ns1, &someDateData_om_node);
    AXIOM_ELEMENT_SET_TEXT(someDateData_om_elem, env, "1977-06-27", someDateData_om_node);
    someListData_om_elem = axiom_element_create(env, someData_om_node, "someListData", ns1, &someListData_om_node);
    listItem1_om_elem = axiom_element_create(env, someListData_om_node, "listItem", ns1, &listItem1_om_node);
    AXIOM_ELEMENT_SET_TEXT(listItem1_om_elem, env, "Here is list item 1", listItem1_om_node);
    listItem2_om_elem = axiom_element_create(env, someListData_om_node, "listItem", ns1, &listItem2_om_node);
    AXIOM_ELEMENT_SET_TEXT(listItem2_om_elem, env, "Here is list item 2", listItem2_om_node);

    om_str = AXIOM_NODE_TO_STRING(req_om_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return req_om_node;
}
