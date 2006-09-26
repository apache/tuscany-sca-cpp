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

#include <axiom.h>
#include <axis2_util.h>
#include <axiom_soap.h>
#include <axis2_client.h>

#include <commonj/sdo/SDO.h>
using namespace commonj::sdo;

#include <sdo_axiom.h>
using namespace commonj::sdo_axiom;

#include <tuscany/sca/util/Utils.h>
using namespace tuscany::sca;

#include <string>
#include <iostream>
using namespace std;

axiom_node_t* get_om_payload_from_xml_file(axis2_env_t *env, const char* currentTest);
axiom_node_t* runCall(axis2_env_t *env, char* endpoint, char* opName, axiom_node_t* payload);
void loadAllXSD();
bool compareAxiomNodes(axiom_node_t* returned_payload, axiom_node_t* payload_data);

XSDHelperPtr xsdHelper = NULL;
XMLHelperPtr xmlHelper = NULL;


int main(int argc, char** argv)
{
    axis2_env_t *env = NULL;
    axis2_char_t *address = NULL;
    axis2_char_t *testNumberString = "-1";
    
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    axis2_char_t *client_home = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axiom_node_t *payload = NULL;
    axiom_node_t *ret_node = NULL;
   
    /* Set up the envioronment */
    env = axis2_env_create_all("ws.service.interop.client.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of echo service */
    address = "http://localhost:9090/axis2/services/ws.service";
    if (argc > 1 )
        testNumberString = argv[1];
    if (argc > 2 )
        address = argv[2];
    if (AXIS2_STRCMP(testNumberString, "-h") == 0)
    {
        cout<<"Usage : "<< argv[0] <<"[test_number] [endpoint_url]"<<endl;
        cout<<"use -h for help"<<endl;
        return 0;
    }

    int testNumber = atoi(testNumberString);
    if(testNumber > 0 && testNumber <= 50)
    {
        cout<<"Running test : "<< testNumber << endl;
    }
    else
    {
        testNumber = -1;
        cout<<"Running all tests" << endl;
    }

    cout<<"Using endpoint : "<< address << endl;

    const string interop = "Interop";
                    
    string currentTest;

    int testsPassed = 0;
    int testsRun = 0;
    string failedTests = "";
    string unsupportedTests = "";

    loadAllXSD();

    // Tests 1 to 50
    for(int i=1; i<=50; i++)
    {
        if( testNumber != i &&
            testNumber > 0 )
        {
            // Only running a single, chosen test
            continue;
        }
        if( i==13 || 
            i==48 || 
            i==49 )
        {
            // Ignore non-existant schemas
            continue;
        }

        currentTest = interop;
        if(i<10)
        {
            currentTest.append("0");
        }
        char buf[3];
        sprintf(buf, "%d", i);
        currentTest.append(buf);

        cout << "Running test " << currentTest << endl;

        if( i==5 || 
            i==25 || 
            i==27 || 
            i==39 )
        {
            cout << "We do not currently support schema " << currentTest << endl; 
            unsupportedTests += currentTest + " ";
            continue;
        }

        testsRun++;

        payload = get_om_payload_from_xml_file(env, currentTest.c_str());
        axiom_node_t* returned_payload = NULL;
        bool success = false;
        if(payload)
        {
            returned_payload = runCall(env, address, (char*) currentTest.c_str(), payload);
        }

        if(returned_payload)
        {
            // Get the inner node to compare
            axiom_node_t* payload_data = AXIOM_NODE_GET_FIRST_CHILD(payload, env);
            success = compareAxiomNodes(returned_payload, payload_data);
        }
        
        if(success)
        {
            cout << currentTest << " PASSED!" << endl;
            testsPassed++;
        }
        else
        {
            failedTests += currentTest + " ";
            cout << currentTest << " FAILED!" << endl;
        }

    }
    cout << "Passed " << testsPassed << " of " << testsRun << " tests" << endl;
    if(failedTests.size() > 0)
    {
        cout << "Failed: " << failedTests << endl;
    }
    if(unsupportedTests.size() > 0)
    {
        cout << "Unsupported: " << unsupportedTests << endl;
    }
    return 0;
}

axiom_node_t* runCall(axis2_env_t *env, char* endpoint, char* opName, axiom_node_t* payload)
{
    axis2_char_t *address = endpoint;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    axis2_char_t *client_home = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axiom_node_t *ret_node = NULL;       
    axiom_node_t *ret = NULL;       
    char action [100];
    int len = 0;

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
        return NULL;
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
        ret = AXIOM_NODE_GET_FIRST_CHILD(ret_node, env);
        if(!ret)
        {
            cout << "No data in response - failure on server" << endl;
        }
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
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
    return ret;
}


bool compareAxiomNodes(axiom_node_t* returned_payload, axiom_node_t* expected_payload)
{
    bool success = false;

    AxiomHelper* axiomHelper = AxiomHelper::getHelper();
    DataObjectPtr expectedDO = axiomHelper->toSdo(expected_payload, xsdHelper->getDataFactory());
    DataObjectPtr returnedDO = axiomHelper->toSdo(returned_payload, xsdHelper->getDataFactory());

    string diff;
    success = Utils::compareDataObjects(expectedDO, returnedDO, diff);

    if(!success)
    {        
        cout << "Expected data does not match returned data:\n" << diff << "\nExpected data:"<<endl;
        Utils::printDO(expectedDO);
        cout << "Returned data:"<<endl;
        Utils::printDO(returnedDO);
    }
    return success;
}

void loadAllXSD()
{
    const string resourcesDir = "./resources/";
    const string dotxsd = ".xsd";

    try
    {       
        // Create an xsd helper
        xsdHelper = HelperProvider::getXSDHelper();

        // Tests 1 to 50
        for(int i=1; i<=50; i++)
        {
            if( i==13 || 
                i==48 || 
                i==49 ||
                i==5 || 
                i==25 || 
                i==27 || 
                i==39 )
            {
                // Ignore non-existant/unsupported schemas
                continue;
            }

            string currentTest = "Interop";
            if(i<10)
            {
                currentTest.append("0");
            }
            char buf[3];
            sprintf(buf, "%d", i);
            currentTest.append(buf);

            string xsdFileName = resourcesDir + currentTest + dotxsd;
            xsdHelper->defineFile(xsdFileName.c_str());
        }

    }
    catch(SDORuntimeException& ex)
    {
        cout << "SDORuntimeException whilst loading xsd" << endl;
        cout << ex.getMessageText() << endl;
    }
}

/* build SOAP request message content using OM */
axiom_node_t* get_om_payload_from_xml_file(axis2_env_t *env, const char* currentTest)
{
    axiom_node_t* req_om_node = NULL;
    axiom_node_t* payload_om_node = NULL;
    const string resourcesDir = "./resources/";
    const string dotxml = "-in.xml";

    string xmlFileName = resourcesDir + currentTest + dotxml;

    try
    {       

        xmlHelper = HelperProvider::getXMLHelper(xsdHelper->getDataFactory());

        XMLDocumentPtr payloadFile = xmlHelper->loadFile(xmlFileName.c_str());
        string rootElemURI = payloadFile->getRootElementURI();
        string rootElemName = payloadFile->getRootElementName();
    
        DataObjectPtr payloadDO = payloadFile->getRootDataObject();

        AxiomHelper* axiomHelper = AxiomHelper::getHelper();
        payload_om_node = axiomHelper->toAxiomNode(payloadDO, rootElemURI.c_str(), rootElemName.c_str());

        if(!payload_om_node)
        {
            payload_om_node = axiomHelper->toAxiomNode(payloadDO);
        }

        if(payload_om_node)
        {
            axiom_element_t* req_om_elem = NULL;
            axiom_namespace_t *ns1 = NULL;
            axis2_char_t *om_str = NULL;

            ns1 = axiom_namespace_create (env, "http://www.apache.org/tuscany/interop", "ns1");
            req_om_elem = axiom_element_create(env, NULL, currentTest, ns1, &req_om_node);
            AXIOM_NODE_DETACH(payload_om_node, env);
            AXIOM_NODE_ADD_CHILD(req_om_node, env, payload_om_node);
        }
        else
        {
            cout << "Could not convert xml document("<< xmlFileName <<") to Axiom" << endl;
            return NULL;            
        }

    }
    catch(SDORuntimeException& ex)
    {
        cout << "SDORuntimeException whilst loading xml ("<< xmlFileName <<") & converting to Axiom" << endl;
        cout << ex.getMessageText() << endl;
        return NULL;
    }

    return req_om_node;
}

