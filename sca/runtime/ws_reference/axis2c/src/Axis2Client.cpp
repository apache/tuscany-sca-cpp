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
#include "Axis2Client.h"


#include "osoa/sca/sca.h"
using namespace osoa::sca;
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/model/WSBinding.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"

using namespace tuscany::sca::ws;

#include <axis2_client.h>

#include <axis2_error_default.h>
#include <axis2_log_default.h>
#include <axis2_defines.h>
#include <axis2_soap.h>
#include <platforms/axis2_platform_auto_sense.h>

#include <sdo_axiom.h>
using namespace commonj::sdo;
using namespace commonj::sdo_axiom;

Axis2Client::Axis2Client(ExternalService* extService)
    : externalService(extService)
{
}

Axis2Client::~Axis2Client()
{
}

void Axis2Client::invoke(tuscany::sca::Operation& operation)
{
    LOGENTRY(1, "Axis2Client::invoke");

    // From the external service ws-binding, get the namespace of the wsdl endpoint
    WSBinding* binding = (WSBinding *)externalService->getBinding();
    // TODO check 0 throw

    string portNamespace = binding->getWSDLNamespaceURL();

    // Lookup the wsdl model from the module, keyed on the namespace 
    // (the wsdl will have been loaded at startup, based on the information in the config file)
    Wsdl* wsdlDefinition = externalService->getContainingModule()->findWsdl(portNamespace);
    if (wsdlDefinition == 0)
    {
    	string msg = "WSDL not found for " + portNamespace;
      	throw SystemConfigurationException(msg.c_str());
    }
    
    const string& operationName = operation.getName();

    // Match the operation in Operation to the operation in the wsdl port type.
    const WsdlOperation& wsdlOperation =  wsdlDefinition->findOperation(
                                                       binding->getServiceName(),
                                                       binding->getPortName(),
                                                       operationName);  

    // Initialize Axis2 stuff
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_log_t *log = axis2_log_create(allocator, NULL, "tuscany_client.log");
    axis2_error_t *error = axis2_error_create(allocator);
    axis2_env_t *env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_TRACE;
    axis2_error_init();

    axis2_char_t* address = (axis2_char_t*)wsdlOperation.getEndpoint().c_str();
    axis2_char_t* opName = (axis2_char_t*)operationName.c_str();
    axis2_char_t* soap_action = (axis2_char_t*)wsdlOperation.getSoapAction().c_str();
    axis2_char_t* serviceName = (axis2_char_t*)binding->getServiceName().c_str();
    
    // create OM from Operation and wsdlOperation
    axis2_om_node_t* payload = createPayload(operation, wsdlOperation, &env);

    /* Create EPR with given address */
    axis2_endpoint_ref_t* endpoint_ref = axis2_endpoint_ref_create(&env, address);
    
    /* Setup options */
    axis2_options_t* options = axis2_options_create(&env);
    AXIS2_OPTIONS_SET_TO(options, &env, endpoint_ref);
    int soap_version = AXIS2_SOAP11;
    if (wsdlOperation.getSoapVersion() == WsdlOperation::SOAP12)
    {
    	soap_version = AXIS2_SOAP12;
    }
    
    AXIS2_OPTIONS_SET_SOAP_VERSION(options, &env, soap_version);
    AXIS2_OPTIONS_SET_ACTION(options, &env, soap_action);

    /* Create service client */
    
    axis2_char_t* client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
    {
      	throw SystemConfigurationException("Environment error: AXIS2C_HOME not set");
    }
    axis2_svc_client_t* svc_client = axis2_svc_client_create(&env, client_home);
    if (!svc_client)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "invoke FAILED: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
                        
        throw ServiceRuntimeException("Axis2Client: axis2_svc_client_create failed");
    }

    /* Set service client options */
    AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, &env, options);    
    
    /* Send request */
    axis2_om_node_t* ret_node = AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, &env, payload);
    if(ret_node)
    {
    	setReturn(ret_node, operation, wsdlOperation, &env);
    }
    else
    {
    	AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invoke failed: Error code:"
                        " %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        throw ServiceRuntimeException("Axis2Client: Invoke failed");
    }
    
    if (svc_client)
    {
        AXIS2_SVC_CLIENT_FREE(svc_client, &env);
        svc_client = NULL;
    } 

    LOGEXIT(1, "Axis2Client::invoke");
}

axis2_om_node_t* Axis2Client::createPayload(Operation& operation, 
                                            const WsdlOperation& wsdlOp,
                                            axis2_env_t** env)
{	
    LOGENTRY(1, "Axis2Client::createPayload");
    axis2_om_node_t* requestOM = NULL;
    
    // map the operation request to the wsdl
    if (wsdlOp.isDocumentStyle())
    {
    	// Document style 
    	// only support single part messages - WS-I compliant
    	
    	// Build up the payload as an SDO

        // Get the data factory for the module (it will already have the types loaded for the xsds)
      	DataFactoryPtr dataFactory = externalService->getContainingModule()->getDataFactory();
    	DataObjectPtr requestDO = dataFactory->create(wsdlOp.getInputTypeUri().c_str(), 
    	                                              wsdlOp.getInputTypeName().c_str());
    	
    	// Each parameter in the operation should be a property on the request dataobject
    	for (int i=0; i<operation.getNParms(); i++)
    	{
    		Operation::Parameter& parm = operation.getParameter(i);
    		switch(parm.getType())
    		{
    			case Operation::BOOL: 
    			{
    				requestDO->setBoolean(i, *(bool*)parm.getValue());
    				break;
    			}
    			case Operation::SHORT: 
    			{
    				requestDO->setShort(i, *(short*)parm.getValue());
    				break;
    			}
    			case Operation::LONG: 
    			{
    				requestDO->setLong(i, *(long*)parm.getValue());
    				break;
    			}
    			case Operation::USHORT: 
    			{
    				requestDO->setInteger(i, *(unsigned short*)parm.getValue());
    				break;
    			}
    			case Operation::ULONG: 
    			{
    				requestDO->setInteger(i, *(unsigned long*)parm.getValue());
    				break;
    			}
    			case Operation::FLOAT: 
    			{
    				requestDO->setFloat(i, *(float*)parm.getValue());
    				break;
    			}
    			case Operation::DOUBLE: 
    			{
    				requestDO->setDouble(i, *(double*)parm.getValue());
    				break;
    			}
    			case Operation::LONGDOUBLE: 
    			{
    				requestDO->setDouble(i, *(long double*)parm.getValue());
    				break;
    			}
    			case Operation::CHARS: 
    			{
    				requestDO->setCString(i, *(char**)parm.getValue());
    				break;
    			}
    			case Operation::STRING: 
    			{
    				requestDO->setCString(i, (*(string*)parm.getValue()).c_str());
    				break;
    			}
    			case Operation::DATAOBJECT: 
    			{
    				requestDO->setDataObject(i, *(DataObjectPtr*)parm.getValue());
    				break;
    			}
    			default: throw "unsupported parameter type";
    		}
    	}
    	
    	// Create the Axiom object from the request dataobject
    	AxiomHelper myHelper;
    	requestOM = myHelper.toAxiomNode(requestDO);
    }
    else
    {
    	// RPC
    }
    
    // Logging start
    axis2_xml_writer_t* xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE, AXIS2_XML_PARSER_TYPE_BUFFER);
    axis2_om_output_t* om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(requestOM, env, om_output);
    axis2_char_t* buffer = (axis2_char_t*)AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    LOGINFO_1(3, "Sending this OM node in XML : %s \n",  buffer); 
    // Logging end

    AXIS2_OM_OUTPUT_FREE(om_output, env);
    AXIS2_FREE((*env)->allocator, buffer);

    return requestOM;
    
    LOGEXIT(1, "Axis2Client::createPayload");
}

void Axis2Client::setReturn(axis2_om_node_t* ret_node,
                            Operation& operation, 
                            const WsdlOperation& wsdlOp,
                            axis2_env_t** env)
{	
    LOGENTRY(1, "Axis2Client::setReturn");
    /* Get the response value from the SOAP message */
    
    // logging start
    axis2_xml_writer_t* writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0, AXIS2_XML_PARSER_TYPE_BUFFER);
    axis2_om_output_t* om_output = axis2_om_output_create (env, writer);
    AXIS2_OM_NODE_SERIALIZE (ret_node, env, om_output);
    axis2_char_t* buffer = (axis2_char_t*)AXIS2_XML_WRITER_GET_XML(writer, env);
    LOGINFO_1(3,"Received OM node in XML : %s\n", buffer);
    // Logging end
    
    AXIS2_OM_OUTPUT_FREE(om_output, env);
    AXIS2_FREE((*env)->allocator, buffer);
    	
    DataFactoryPtr dataFactory = externalService->getContainingModule()->getDataFactory();
    AxiomHelper myHelper;
    DataObjectPtr returnDO = myHelper.toSdo(ret_node, dataFactory);

    unsigned int index = 0;
    switch(operation.getReturnType())
	{
		case Operation::BOOL: 
		{
			*(bool*)operation.getReturnValue() = returnDO->getBoolean(index);
			break;
		}
		case Operation::SHORT: 
		{
			*(short*)operation.getReturnValue() = returnDO->getShort(index);
			break;
		}
		case Operation::LONG: 
		{
			*(long*)operation.getReturnValue() = returnDO->getLong(index);
			break;
		}
		case Operation::USHORT: 
		{
			*(unsigned short*)operation.getReturnValue() = (unsigned short)returnDO->getInteger(index);
			break;
		}
		case Operation::ULONG: 
		{
			*(unsigned long*)operation.getReturnValue() = (unsigned long)returnDO->getInteger(index);
			break;
		}
		case Operation::FLOAT: 
		{
			*(float*)operation.getReturnValue() = returnDO->getFloat(index);
			break;
		}
		case Operation::DOUBLE: 
		{
			*(double*)operation.getReturnValue() = returnDO->getDouble(index);
			break;
		}
		case Operation::LONGDOUBLE: 
		{
			*(long double*)operation.getReturnValue() = returnDO->getDouble(index);
			break;
		}
		case Operation::CHARS: 
		{
			*(char**)operation.getReturnValue() = strdup(returnDO->getCString(index));
			break;
		}
		case Operation::STRING: 
		{
			*(string*)operation.getReturnValue() = returnDO->getCString(index);
			break;
		}
		case Operation::DATAOBJECT: 
		{
			*(DataObjectPtr*)operation.getReturnValue() = returnDO->getDataObject(index);
			break;
		}
		default:;
	}
    LOGEXIT(1, "Axis2Client::setReturn");
}

