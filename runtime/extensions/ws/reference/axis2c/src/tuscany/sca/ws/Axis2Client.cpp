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
#pragma warning(disable: 4091)
#pragma warning(disable: 4786)
#endif

#include <sstream>

#include <axis2_client.h>

#include <axis2_error_default.h>
#include <axis2_log_default.h>
#include <axis2_defines.h>
#include <axiom_soap_const.h>
#include <platforms/axis2_platform_auto_sense.h>

#include <sdo_axiom.h>

#include "Axis2Client.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "model/WSServiceBinding.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/WSDLInterface.h"
#include "tuscany/sca/model/WSDLOperation.h"
#include "tuscany/sca/model/WSDLMessagePart.h"

using namespace std;
using namespace commonj::sdo;
using namespace commonj::sdo_axiom;
using namespace tuscany::sca::model;


namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
            
            Axis2Client::Axis2Client(CompositeReference* compositeReference)
                : compositeReference(compositeReference)
            {
                logentry();
            }
            
            Axis2Client::~Axis2Client()
            {
                logentry();
            }
            
            void Axis2Client::invoke(tuscany::sca::Operation& operation)
            {
                logentry();

                // Initialize Axis2 stuff
                axis2_env_t *env = axis2_env_create_all("tuscany_ws_reference.log",AXIS2_LOG_LEVEL_TRACE);
                axis2_error_init();
                
                // Get the WS service binding and WSDLOperation
                Composite* composite=compositeReference->getComposite();
                Service* service = compositeReference->getService();
                WSServiceBinding* binding = (WSServiceBinding *)service->getBinding();
                const string& operationName = operation.getName();
                WSDLOperation wsdlOperation;
                
                // Get the WSDL namespace
                string wsdlNamespace = binding->getWSDLNamespaceURL();
                if (wsdlNamespace != "")
                {
                
                    // Lookup the wsdl model from the composite, keyed on the namespace 
                    // (the wsdl will have been loaded at startup)
                    WSDLDefinition* wsdlDefinition = composite->findWSDLDefinition(wsdlNamespace);
                    if (wsdlDefinition == 0)
                    {
                        string msg = "WSDL not found for " + wsdlNamespace;
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                    
                    // Match the operation in Operation to the operation in the wsdl port type.
                    try
                    {
                        wsdlOperation =  wsdlDefinition->findOperation(
                            binding->getServiceName(),
                            binding->getEndpointName(),
                            operationName);
                    }
                    catch(SystemConfigurationException&)
                    {   
                        throw;
                    }
                }
                else
                {
                    Interface* iface = service->getType()->getInterface();
                    if (iface != NULL &&
                        iface->getInterfaceTypeQName() == WSDLInterface::typeQName)
                    {
                        WSDLInterface* wsdlInterface = (WSDLInterface*)iface;
                        wsdlNamespace = wsdlInterface->getNamespaceURI();
                        
                        if (wsdlNamespace != "")
                        {
                            
                            WSDLDefinition* wsdl = composite->findWSDLDefinition(wsdlNamespace);
                            if (wsdl == 0)
                            {
                                string msg = "WSDL not found for " + wsdlNamespace;
                                throwException(SystemConfigurationException, msg.c_str());
                            }
                    
                            try
                            {
                                wsdlOperation = wsdl->findOperation(wsdlInterface->getName(), operationName);
                            }
                            catch(SystemConfigurationException&)
                            {   
                                throw;
                            }
                        }
                    }
                }
                
                if (wsdlNamespace == "")
                {
                    
                    // Create a default document literal wrapped WSDL operation
                    WSDLMessagePart inputPart(operationName, "", "http://tempuri.org");
                    WSDLMessagePart outputPart((operationName+"Response"), "", "http://tempuri.org");
                    wsdlNamespace = compositeReference->getName();
                    wsdlOperation = WSDLOperation();
                    wsdlOperation.setOperationName(operationName);
                    wsdlOperation.setSoapAction(wsdlNamespace+ "#" +operationName);
                    wsdlOperation.setEndpoint("");
                    wsdlOperation.setSoapVersion(WSDLOperation::SOAP11);
                    wsdlOperation.setDocumentStyle(true);
                    wsdlOperation.setWrappedStyle(true);
                    wsdlOperation.setInputEncoded(false);
                    wsdlOperation.setOutputEncoded(false);
                    wsdlOperation.setInputMessagePart(operationName, inputPart);
                    wsdlOperation.setOutputMessagePart((operationName+"Response"), outputPart);
                }
                else if (!wsdlOperation.isDocumentStyle() || !wsdlOperation.isWrappedStyle())
                {
                    throwException(ServiceInvocationException,
                        "Only wrapped document style WSDL operations are currentlysupported");
                }
                    
                // The URI specified in the binding overrides the address specified in
                // the WSDL
                axis2_char_t* address;

                // Get the URI configured on the top level component                
                string bindingURI = "";
                SCARuntime* runtime = SCARuntime::getCurrentRuntime();
                Component* component = runtime->getDefaultComponent();
                Reference* reference = component->findReference(compositeReference->getName());
                if (reference != NULL)
                {
                    ReferenceBinding* referenceBinding = reference->getBinding();
                    if (referenceBinding != NULL && referenceBinding->getURI() != "")
                    {
                        bindingURI = referenceBinding->getURI();
                    } 
                }
                if (bindingURI == "")
                {
                    // Get the URI configured on the binding
                    if (binding->getURI() != "")
                    {
                        bindingURI = binding->getURI();
                    }
                }
                if (bindingURI != "")
                {
                    // Prepend the default base URI if the URI is not absolute
                    if (bindingURI.find("://") == string::npos)
                    {
                        bindingURI = runtime->getDefaultBaseURI() + string("/axis2/services/") + bindingURI;
                    }
                    address = (axis2_char_t*)bindingURI.c_str();
                }
                else
                {
                    address = (axis2_char_t*)wsdlOperation.getEndpoint().c_str();
                }
                                
                axis2_char_t* opName = (axis2_char_t*)operationName.c_str();
                axis2_char_t* soap_action = (axis2_char_t*)wsdlOperation.getSoapAction().c_str();

                // create OM from Operation and wsdlOperation
                axiom_node_t* payload = createPayload(operation, wsdlOperation, env);
                /* Create EPR with given address */
                axis2_endpoint_ref_t* endpoint_ref = axis2_endpoint_ref_create(env, address);
                
                /* Setup options */
                axis2_options_t* options = axis2_options_create(env);
                AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);
                int soap_version = AXIOM_SOAP11;
                if (binding->getSOAPVersion() == "1.2")
                {
                    soap_version = AXIOM_SOAP12;
                }
                else
                {
                    if (wsdlOperation.getSoapVersion() == WSDLOperation::SOAP12)
                    {
                        soap_version = AXIOM_SOAP12;
                    }
                }
                
                AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, soap_version);
                AXIS2_OPTIONS_SET_ACTION(options, env, soap_action);
                AXIS2_OPTIONS_SET_XML_PARSER_RESET(options, env, AXIS2_FALSE);

                loginfo("WS SOAP action: %s", soap_action);
                loginfo("WS Endpoint address: %s", address);
                if(soap_version == AXIOM_SOAP11)
                {
                    // Only set the SOAP action when using SOAP1.1
                    AXIS2_OPTIONS_SET_SOAP_ACTION(options, env, soap_action);
                    loginfo("Set soap version: 1.1");
                }
                else if(soap_version == AXIOM_SOAP12)
                {
                    loginfo("Set soap version: 1.2");
                }
                else
                {
                    loginfo("Set soap version: unset");
                }
                /* Create service client */
                
                axis2_char_t* client_home = AXIS2_GETENV("AXIS2C_HOME");
                if (!client_home)
                {
                    throwException(SystemConfigurationException, "AXIS2C_HOME not set");
                }
                axis2_svc_client_t* svc_client = axis2_svc_client_create(env, client_home);
                if (!svc_client)
                {
                    ostringstream msg;
                    msg << "Axis2 svc_client_create failed, error: " << env->error->error_number <<
                    ", " << AXIS2_ERROR_GET_MESSAGE(env->error);
                    throwException(SystemConfigurationException, msg.str().c_str());
                }
                
                /* Set service client options */
                AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, env, options);    

                /* Engage addressing module */
                AXIS2_SVC_CLIENT_ENGAGE_MODULE(svc_client, env, AXIS2_MODULE_ADDRESSING);

                loginfo("Sending WS request");
                /* Send request */
                axiom_node_t* ret_node = AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload);
                if(ret_node)
                {
                    setReturn(ret_node, operation, wsdlOperation, env);

                    // Set return value - now need to detach the node from the Axiom document                     
                    // for clean-up
                    axiom_document_t *document = AXIOM_NODE_GET_DOCUMENT(ret_node, env);
                    if (document)
                    {
                        AXIOM_DOCUMENT_BUILD_ALL (document, env);
                    }
                    AXIOM_NODE_DETACH (ret_node, env);
                }
                else
                {
                    ostringstream msg;
                    msg << "Axis2 client_send_received failed, error: " << env->error->error_number <<
                    ", " << AXIS2_ERROR_GET_MESSAGE(env->error);
                    throwException(ServiceInvocationException, msg.str().c_str());
                }

                if (svc_client)
                {
                    AXIS2_SVC_CLIENT_FREE(svc_client, env);
                    svc_client = NULL;
                }

                if(env)
                {
                    axis2_env_free((axis2_env_t *) env);
                    env = NULL;
                }
                loginfo("Freed env");

            }

            axiom_node_t* Axis2Client::createPayload(Operation& operation, 
                const WSDLOperation& wsdlOperation,
                axis2_env_t* env)
            {	
                logentry();

                axiom_node_t* request_node = NULL;
                
                // Build up the payload as an SDO
                
                // Get the data factory for the composite (it will already have the typecreates loaded for the xsds)
                DataFactoryPtr dataFactory = compositeReference->getComposite()->getDataFactory();
                
                DataObjectPtr inputDataObject;
                string inputTypeUri;
                string inputTypeName;

                try
                {
                    // Since its Document wrapped, there will only be one part
                    std::list<std::string> partList = wsdlOperation.getInputMessagePartNames();
                    const WSDLMessagePart &inputMessage =
                      wsdlOperation.getInputMessagePart(partList.front());
                    inputTypeName = inputMessage.getPartType();
                    inputTypeUri = inputMessage.getPartUri();

                    // Create the input wrapper
                    const Type& rootType = dataFactory->getType(inputTypeUri.c_str(), "RootType");
                    const Property& prop = rootType.getProperty(inputTypeName.c_str());
                    const Type& inputType = prop.getType();
                    inputDataObject = dataFactory->create(inputType);
                }
                catch (SDORuntimeException&)
                {
                    try
                    {
                        // Create the input wrapper
                        const Type& inputType =
                          dataFactory->getType(inputTypeUri.c_str(), inputTypeName.c_str());
                        inputDataObject = dataFactory->create(inputType);
                    }
                    catch (SDORuntimeException&)
                    {
                        
                        // The input wrapper type is not known, create an open DataObject 
                        //inputDataObject = dataFactory->create("http://tempuri.org", "Wrapper");
                        inputDataObject = dataFactory->create(Type::SDOTypeNamespaceURI, "OpenDataObject");
                    }
                }
                        
                // Go through data object to set the input parameters
                PropertyList pl = inputDataObject->getType().getProperties();
            
                if(pl.size() == 0)
                {
                    if(inputDataObject->getType().isOpenType() && inputDataObject->getType().isDataObjectType())
                    {
                        /*
                         * This code deals with sending xsd:any elements
                         */
                        for (unsigned int i=0; i<operation.getNParms(); i++)
                        {
                            ostringstream pname;
                            pname << "param" << (i+1);
                            DataObjectList& l = inputDataObject->getList(pname.str());
                            
                            const Operation::Parameter& parm = operation.getParameter(i);
                            switch(parm.getType())
                            {
                            case Operation::BOOL: 
                                {
                                    l.append(*(bool*)parm.getValue());
                                    break;
                                }
                            case Operation::SHORT: 
                                {
                                    l.append(*(short*)parm.getValue());
                                    break;
                                }
                            case Operation::INT: 
                                {
                                    l.append(*(long*)parm.getValue());
                                    break;
                                }
                            case Operation::LONG: 
                                {
                                    l.append(*(long*)parm.getValue());
                                    break;
                                }
                            case Operation::USHORT: 
                                {
                                    l.append(*(short*)parm.getValue());
                                    break;
                                }
                            case Operation::UINT: 
                                {
                                    l.append(*(long*)parm.getValue());
                                    break;
                                }
                            case Operation::ULONG: 
                                {
                                    l.append(*(long*)parm.getValue());
                                    break;
                                }
                            case Operation::FLOAT: 
                                {
                                    l.append(*(float*)parm.getValue());
                                    break;
                                }
                            case Operation::DOUBLE: 
                                {
                                    l.append(*(long double*)parm.getValue());
                                    break;
                                }
                            case Operation::LONGDOUBLE: 
                                {
                                    l.append(*(long double*)parm.getValue());
                                    break;
                                }
                            case Operation::CHARS: 
                                {
                                    l.append(*(char**)parm.getValue());
                                    break;
                                }
                            case Operation::STRING: 
                                {
                                    l.append((*(string*)parm.getValue()).c_str());
                                    break;
                                }
                            case Operation::DATAOBJECT: 
                                {
                                    l.append(*(DataObjectPtr*)parm.getValue());
                                    break;
                                }
                            default:
                                {
                                    ostringstream msg;
                                    msg << "Unsupported parameter type: " << parm.getType();
                                    throwException(ServiceDataException, msg.str().c_str());
                                }
                            }
                        }
                    }
                }
                else {
                    
                    // Each parameter in the operation should be a property on the request dataobject
                    for (unsigned int i=0; i<operation.getNParms(); i++)
                    {
                        const Operation::Parameter& parm = operation.getParameter(i);
                        switch(parm.getType())
                        {
                        case Operation::BOOL: 
                            {
                                inputDataObject->setBoolean(i, *(bool*)parm.getValue());
                                break;
                            }
                        case Operation::SHORT: 
                            {
                                inputDataObject->setShort(i, *(short*)parm.getValue());
                                break;
                            }
                        case Operation::INT: 
                            {
                                inputDataObject->setInteger(i, *(int*)parm.getValue());
                                break;
                            }
                        case Operation::LONG: 
                            {
                                inputDataObject->setLong(i, *(long*)parm.getValue());
                                break;
                            }
                        case Operation::USHORT: 
                            {
                                inputDataObject->setInteger(i, *(unsigned short*)parm.getValue());
                                break;
                            }
                        case Operation::UINT: 
                            {
                                inputDataObject->setInteger(i, *(unsigned int*)parm.getValue());
                                break;
                            }
                        case Operation::ULONG: 
                            {
                                inputDataObject->setInteger(i, *(unsigned long*)parm.getValue());
                                break;
                            }
                        case Operation::FLOAT: 
                            {
                                inputDataObject->setFloat(i, *(float*)parm.getValue());
                                break;
                            }
                        case Operation::DOUBLE: 
                            {
                                inputDataObject->setDouble(i, *(double*)parm.getValue());
                                break;
                            }
                        case Operation::LONGDOUBLE: 
                            {
                                inputDataObject->setDouble(i, *(long double*)parm.getValue());
                                break;
                            }
                        case Operation::CHARS: 
                            {
                                inputDataObject->setCString(i, *(char**)parm.getValue());
                                break;
                            }
                        case Operation::STRING: 
                            {
                                inputDataObject->setCString(i, (*(string*)parm.getValue()).c_str());
                                break;
                            }
                        case Operation::DATAOBJECT: 
                            {
                                inputDataObject->setDataObject(i, *(DataObjectPtr*)parm.getValue());
                                break;
                            }
                        default:
                            ostringstream msg;
                            msg << "Unsupported parameter type: " << parm.getType();
                            throwException(ServiceDataException, msg.str().c_str());
                        }
                    }
                }
                
                // Create the Axiom object from the request dataobject
                AxiomHelper* axiomHelper = AxiomHelper::getHelper();
                request_node = axiomHelper->toAxiomNode(inputDataObject,
                    inputTypeUri.c_str(), inputTypeName.c_str());
                AxiomHelper::releaseHelper(axiomHelper);

                char* str =  AXIOM_NODE_TO_STRING(request_node, env);
                loginfo("Sending Axis2 OM: %s ",  str); 
                
                return request_node;
                
             }
             
             void Axis2Client::setReturn(axiom_node_t* ret_node,
                 Operation& operation, 
                 const WSDLOperation& wsdlOperation,
                 axis2_env_t* env)
             {	
                logentry();
                 
                DataFactoryPtr dataFactory = compositeReference->getComposite()->getDataFactory();

                // Get the AXIOM node representing the SOAP Body 
                axiom_node_t* body = AXIOM_NODE_GET_PARENT(ret_node, env);
                 
                // Convert the AXIOM node to an SDO DataObject
                char* str = NULL;
                str = AXIOM_NODE_TO_STRING(body, env);
                if (str)
                {
                    loginfo("Received Axis2 OM: %s ",  str); 
                }
                
                // Convert the SOAP body to an SDO DataObject
                AxiomHelper* axiomHelper = AxiomHelper::getHelper();
                DataObjectPtr outputBodyDataObject = axiomHelper->toSdo(body, dataFactory);
                AxiomHelper::releaseHelper(axiomHelper);
                
                if(!outputBodyDataObject)
                {
                    string msg = "Could not convert Axis2 OM node to SDO";
                    throwException(ServiceInvocationException, msg.c_str());
                }
                else
                {
                    ostringstream os;
                    os << outputBodyDataObject;
                    loginfo("Converted Axis2 OM node to SDO: %s", os.str().c_str());
                }
                
                XMLHelperPtr xmlHelper = compositeReference->getComposite()->getXMLHelper();

                // Get the first body part representing the doc-lit-wrapped wrapper element
                DataObjectPtr outputDataObject = NULL; 
                PropertyList bpl = outputBodyDataObject->getInstanceProperties();
                if (bpl.size()!=0)
                {
                    if (bpl[0].isMany())
                    {
                        DataObjectList& parts = outputBodyDataObject->getList((unsigned int)0);
                        outputDataObject = parts[0];
                    }
                    else
                    {
                        outputDataObject = outputBodyDataObject->getDataObject(bpl[0]);
                    }
                }
                if (outputDataObject == NULL)
                {
                    string msg = "Could not convert Axis2 body part to SDO";
                    throwException(ServiceInvocationException, msg.c_str());
                }
                
                PropertyList pl = outputDataObject->getType().getProperties();
                if (pl.size() == 0)
                {
                    if (outputDataObject->getType().isOpenType() && outputDataObject->getType().isDataObjectType())
                    {
                        SequencePtr sequence = outputDataObject->getSequence();
                        if (sequence != NULL && sequence->size() != 0)
                        {
                            // Return a text element        
                            if (sequence->isText(0))
                            {                                        
                                string* stringData = new string(sequence->getCStringValue(0));
                                operation.setReturnValue(stringData);
                            }
                            else
                            {
                                // Return a DataObject representing a complex element
                                DataObjectPtr *dataObjectData = new DataObjectPtr;
                                *dataObjectData = sequence->getDataObjectValue(0);
                                if(!*dataObjectData)
                                {
                                    loginfo("Null DataObject return value");
                                }
                                else
                                {
                                    (*dataObjectData)->detach();
                                }
                                operation.setReturnValue(dataObjectData);
                            }
                        }
                    }
                }
                else
                {
                    const Property* p = &pl[0];

                    switch(pl[0].getTypeEnum())
                    {
                    case Type::BooleanType:
                        {
                            bool* boolData = new bool;
                            *boolData = outputDataObject->getBoolean(pl[0]);
                            operation.setReturnValue(boolData);
                        }
                        break;
                    case Type::ByteType:
                        {
                            char* byteData = new char;
                            *byteData = outputDataObject->getByte(pl[0]);
                            operation.setReturnValue(byteData);
                        }
                        break;
                    case Type::BytesType:
                        {
                            int len = outputDataObject->getLength(pl[0]);
                            char** bytesData = new char*;
                            *bytesData = new char[len+1];
                            int bytesWritten = outputDataObject->getBytes(pl[0], *bytesData, len);
                            // Ensure the bytes end with the null char. Not sure if this is neccessary
                            if(bytesWritten <= len)
                            {
                                (*bytesData)[bytesWritten] = 0;
                            }
                            else
                            {
                                (*bytesData)[len] = 0;
                            }
                            //printf("outputDataObject has BytesType named %s with length %d\n", name, bytesWritten);
                            operation.setReturnValue(bytesData);
                        }
                        break;
                    case Type::CharacterType:
                        {
                            // This code should work but won't be used as there is no mapping from an XSD type to the SDO CharacterType
                            wchar_t* charData = new wchar_t;
                            *charData = outputDataObject->getCharacter(pl[0]);
                            operation.setReturnValue(charData);
                        }
                        break;
                    case Type::DoubleType:
                        {
                            long double* doubleData = new long double;
                            *doubleData = outputDataObject->getDouble(pl[0]);
                            operation.setReturnValue(doubleData);
                        }
                        break;
                    case Type::FloatType:
                        {
                            float* floatData = new float;
                            *floatData = outputDataObject->getFloat(pl[0]);
                            operation.setReturnValue(floatData); 
                        }
                        break;
                    case Type::IntType:
                        {
                            long* intData = new long;
                            *intData = outputDataObject->getInteger(pl[0]);
                            operation.setReturnValue(intData);
                        }
                        break;
                    case Type::ShortType:
                        {
                            short* shortData = new short;
                            *shortData = outputDataObject->getShort(pl[0]);
                            operation.setReturnValue(shortData);
                        }
                        break;
                    case Type::StringType:
                        {
                            string* stringData = new string(outputDataObject->getCString(pl[0]));
                            operation.setReturnValue(stringData);
                        }
                        break;
                    case Type::DataObjectType:
                        {
                            DataObjectPtr* dataObjectData = new DataObjectPtr;
                            *dataObjectData = outputDataObject->getDataObject(pl[0]);
                            if(!*dataObjectData)
                            {
                                loginfo("Null DataObject return value");
                            }
                            else
                            {
                               (*dataObjectData)->detach();
                            }
                            operation.setReturnValue(dataObjectData);
                        }
                        break;
                    case Type::OpenDataObjectType:
                        {         
                            /*
                             * This code deals with xsd:any element parameters
                             */
            
                            DataObjectList& dataObjectList = outputDataObject->getList(pl[0]);
                            
                            for(unsigned int j=0; j<dataObjectList.size(); j++)
                            {
                                DataObjectPtr dob = dataObjectList[j];
                                if(!dob)
                                {
                                    DataObjectPtr* dataObjectData = new DataObjectPtr;
                                    *dataObjectData = NULL;
                                    operation.setReturnValue(dataObjectData);
                                    loginfo("Null OpenDataObject return value");
                                }
                                else 
                                {
                                    
                                    SequencePtr sequence = dob->getSequence();
                                    if (sequence->size()!=0)
                                    {
                                        // Return a text element        
                                        if (sequence->isText(0))
                                        {                                        
                                            string* stringData = new string(sequence->getCStringValue(0));
                                            operation.setReturnValue(stringData);
                                        }
                                        else
                                        {
                                            // Return a DataObject representing a complex element
                                            DataObjectPtr *dataObjectData = new DataObjectPtr;
                                            *dataObjectData = sequence->getDataObjectValue(0);
                                            if(!*dataObjectData)
                                            {
                                                loginfo("Null DataObject return value");
                                            }
                                            else
                                            {
                                                (*dataObjectData)->detach();
                                            }
                                            operation.setReturnValue(dataObjectData);
                                        }
                                    }
                                    else
                                    {
                                        // Empty content, add an empty string
                                        loginfo("Null OpenDataObject return value");
                                        string *stringData = new string(""); 
                                        operation.setReturnValue(stringData);
                                    }
                                }
                            }
                        }
                        break;
                    default:
                        {
                            ostringstream msg;
                            msg << "Unsupported result type: " << pl[0].getTypeEnum();
                            throwException(SystemConfigurationException, msg.str().c_str());
                        }
                    }
                }
             }
             
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany
