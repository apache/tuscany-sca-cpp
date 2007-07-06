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

#include <sstream>

#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/WSDLOperation.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/core/Exceptions.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::util;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            // Constructor
            WSDLDefinition::WSDLDefinition(DataObjectPtr wsdlModel)
            {
                logentry(); 

                wsdlModels.insert(wsdlModels.end(), wsdlModel);
                mapOperations( wsdlModel );
            }

            WSDLDefinition::~WSDLDefinition()
            {
                logentry(); 
            }

            /// 
            /// The namespace of the service and other definitions defined in this wsdl definition
            string WSDLDefinition::getNamespace()
            {
                logentry(); 
                return wsdlModels[0]->getCString("targetNamespace");
            }

            void WSDLDefinition::addWSDLModel(DataObjectPtr wsdlModel)
            {
                logentry(); 
                wsdlModels.insert(wsdlModels.end(), wsdlModel);
                mapOperations( wsdlModel );
            }

            ///
            /// Find the operation defined in this wsdl
            ///
            const WSDLOperation& WSDLDefinition::findOperation(const string& serviceName, 
                                        const string& portName, 
                                        const string& operationName)
            {
                logentry(); 

                string operationKey = serviceName+"#"+portName;
                STR_OPERATION_MAP::const_iterator spIter = servicePortMap.find(operationKey);
                if( spIter == servicePortMap.end() )
                {
                    stringstream errMessage;
                    errMessage
                      << "Unable to find Service and Port: "
                      << serviceName << "," << portName
                      << " in the WSDL definition";
                    throwException(SystemConfigurationException, errMessage.str().c_str());
                }

                OPERATION_MAP::const_iterator opIter = spIter->second.find(operationName);

                if (opIter != spIter->second.end())
                {
                    return opIter->second;
                }
                else
                {
                    stringstream errMessage;
                    errMessage
                      << "Unable to find operation "
                      << serviceName << ":" << portName << ":" << operationName
                      << " in the WSDL definition";

                    throwException(SystemConfigurationException, errMessage.str().c_str());
                }
            }
            
            ///
            /// Find the operation defined in this wsdl
            ///
            const WSDLOperation& WSDLDefinition::findOperation(const string& portTypeName, 
                                        const string& operationName)
            {
                logentry(); 

                STR_OPERATION_MAP::const_iterator ptIter = portTypeMap.find(portTypeName);
                if( ptIter == portTypeMap.end() )
                {
                    stringstream errMessage;
                    errMessage
                      << "Unable to find PortType: "
                      << portTypeName
                      << " in the WSDL definition";
                    throwException(SystemConfigurationException, errMessage.str().c_str());
                }

                OPERATION_MAP::const_iterator opIter = ptIter->second.find(operationName);

                if (opIter != ptIter->second.end())
                {
                    return opIter->second;
                }
                else
                {
                    stringstream errMessage;
                    errMessage
                      << "Unable to find Operation "
                      << portTypeName << ":" << operationName
                      << " in the WSDL definition";
                    throwException(SystemConfigurationException, errMessage.str().c_str());
                }
            }

            std::list<std::string> WSDLDefinition::getPortTypes()
            {
                logentry();

                std::list<std::string> ptList;
                STR_OPERATION_MAP::const_iterator ptIter    = portTypeMap.begin();
                STR_OPERATION_MAP::const_iterator ptIterEnd = portTypeMap.end();

                for( ; ptIter != ptIterEnd; ++ptIter )
                {
                  ptList.push_back( ptIter->first );
                }

                return ptList;
            }

            std::list<std::string> WSDLDefinition::getOperations( const std::string &portTypeName )
            {
                logentry();

                std::list<std::string> ptOpList;
                STR_OPERATION_MAP::const_iterator ptIter = portTypeMap.find( portTypeName );

                if( ptIter == portTypeMap.end() )
                {
                    stringstream errMessage;
                    errMessage
                      << "Unable to find PortType: "
                      << portTypeName
                      << " in the WSDL definition";
                    throwException(SystemConfigurationException, errMessage.str().c_str());
                }

                OPERATION_MAP::const_iterator opIter    = ptIter->second.begin();
                OPERATION_MAP::const_iterator opIterEnd = ptIter->second.end();

                for( ; opIter != opIterEnd; ++opIter )
                {
                  ptOpList.push_back( opIter->first );
                }

                return ptOpList;
            }

            ///
            /// Find a service
            ///
            DataObjectPtr WSDLDefinition::findService(const string& serviceName)
            {
                logentry(); 

                DataObjectPtr service = 0;

                // Find the binding
                for (unsigned int m = 0; m < wsdlModels.size(); m++)
                {
                    DataObjectList& serviceList = wsdlModels[m]->getList("service");
                    for (unsigned int i=0; i<serviceList.size(); i++)
                    {
                        string name(serviceList[i]->getCString("name"));
    
                        if (name.compare(serviceName) == 0)
                        {
                            return serviceList[i];
                        }
                    }
                }

                return service;
            }


            ///
            /// Find a named binding
            ///
            DataObjectPtr WSDLDefinition::findBinding(const string& bindingName)
            {
                logentry(); 

                DataObjectPtr binding = 0;
                string uri;
                string name;

                Utils::rTokeniseString(":", bindingName, uri, name);

                // Find the binding
                for (unsigned int m = 0; m < wsdlModels.size(); m++)
                {
                    DataObjectList& bindingList = wsdlModels[m]->getList("binding");
                    for (unsigned int i=0; i<bindingList.size(); i++)
                    {
                        string nameBinding(bindingList[i]->getCString("name"));
    
                        if (nameBinding.compare(name) == 0)
                        {
                            return bindingList[i];
                        }
                    }
                }

                return binding;
            }

            ///
            /// Find a named portType
            ///
            DataObjectPtr WSDLDefinition::findPortType(const string& portTypeName)
            {
                logentry(); 

                DataObjectPtr portType = 0;
                string uri;
                string name;

                Utils::rTokeniseString(":", portTypeName, uri, name);

                // Find the binding
                for (unsigned int m = 0; m < wsdlModels.size(); m++)
                {
                    DataObjectList& portTypeList = wsdlModels[m]->getList("portType");
                    for (unsigned int i=0; i<portTypeList.size(); i++)
                    {
                        string namePortType(portTypeList[i]->getCString("name"));
    
                        if (namePortType.compare(name) == 0)
                        {
                            return portTypeList[i];
                        }
                    }
                }

                return portType;
            }

            ///
            /// Find a named message
            ///
            DataObjectPtr WSDLDefinition::findMessage(const string& messageName)
            {
                logentry(); 

                DataObjectPtr message = 0;
                string uri;
                string name;

                Utils::rTokeniseString(":", messageName, uri, name);

                // Find the binding
                for (unsigned int m = 0; m < wsdlModels.size(); m++)
                {
                    DataObjectList& messageList = wsdlModels[m]->getList("message");
                    for (unsigned int i=0; i<messageList.size(); i++)
                    {
                        string nameMessage(messageList[i]->getCString("name"));
    
                        if (nameMessage.compare(name) == 0)
                        {
                            return messageList[i];
                        }
                    }
                }

                return message;
            }

            ///
            /// Traverse the WSDL SDO and insert operations into the operationMap
            ///
            void WSDLDefinition::mapOperations( DataObjectPtr wsdlModel )
            {
                logentry(); 

                DataObjectList& serviceList = wsdlModel->getList("service");

                // Iterate through the WSDL services
                for (unsigned int i=0; i < serviceList.size(); i++)
                {
                    string serviceName( serviceList[i]->getCString("name") );

                    // Iterate through the WSDL service ports
                    DataObjectList& portList = serviceList[i]->getList("port");
                    for (unsigned int j=0; j < portList.size();j++)
                    {
                        string portName( portList[j]->getCString("name") );
                        string targetAddress(portList[j]->getCString("address/location"));
                        string wsBindingName(portList[j]->getCString("binding"));

                        // get the binding specified in the WSDL service port
                        DataObjectPtr wsBinding = findBinding(wsBindingName);
                        if (!wsBinding)
                        {
                            // Invalid WSDL
                            stringstream errMessage;
                            errMessage
                              << "Unable to find binding "
                              << wsBindingName
                              << " in the WSDL definition";
                            throwException(SystemConfigurationException, errMessage.str().c_str());
                        }

                        // Check if its a SOAP binding, if not go on to the next binding
                        // doing a find like this will work for SOAP11 and SOAP12
                        DataObjectPtr wsBindingSubBinding = wsBinding->getDataObject("binding");
                        string bindingURI(wsBindingSubBinding->getType().getURI());
                        if (bindingURI.find("http://schemas.xmlsoap.org/wsdl/soap") == string::npos)
                        {
                            loginfo("Discarding non-SOAP Binding %s", wsBindingName.c_str() );
                            continue;
                        }

                        // Get the port type specified the WSDL binding
                        string wsBindingPortTypeName(wsBinding->getCString("type"));
                        DataObjectPtr portType = findPortType(wsBindingPortTypeName);
                        if (!portType)
                        {
                            // Invalid WSDL
                            stringstream errMessage;
                            errMessage
                             << "Unable to find PortType "
                             << wsBindingPortTypeName
                             << " in the WSDL definition";
                            throwException(SystemConfigurationException, errMessage.str().c_str());
                        }
                        string portTypeName( portType->getCString("name") );

                        // Fill in this map with operation names to WSDLOperations
                        // then after iterating through the operations, add the map
                        // to the portTypeMap, keyed off of the portTypeName
                        OPERATION_MAP operationMap;

                        // For each binding and portType operation:
                        // - get the soap action, style, and use from the binding
                        // - get the input and/or output message types
                        // its ok if not all of the PortType operations are not defined in the binding
                        DataObjectList& bindingOperationList = wsBinding->getList("operation");
                        DataObjectList& portTypeOperationList = portType->getList("operation");
                        for (unsigned int k=0; k < bindingOperationList.size(); k++)
                        {
                            DataObjectPtr bindingOp = bindingOperationList[k];
                            string operationName = bindingOp->getCString("name");

                            // Get the corresponding PortType operation
                            // I know this may not be very efficient, but its a necessary evil
                            bool foundPortType = false;
                            DataObjectPtr portTypeOp;
                            for (unsigned int l=0;
                                (!foundPortType && l < portTypeOperationList.size());
                                l++)
                            {
                                //portTypeOp = portTypeOperationList[l]->getDataObject("operation");
                                portTypeOp = portTypeOperationList[l];
                                if (operationName == portTypeOp->getCString("name") )
                                {
                                    foundPortType = true;
                                }
                            }

                            if( !foundPortType )
                            {
                                // Invalid WSDL
                                stringstream errMessage;
                                errMessage
                                  << "Unable to find PortType operation for binding operation: "
                                  << operationName
                                  << " in the WSDL definition";
                                throwException(SystemConfigurationException, errMessage.str().c_str());
                            }

                            string soapAction = "";
                            bool documentStyle = true;
                            bool wrappedStyle = true;
                            bool useEncodedInput = false;
                            bool useEncodedOutput = false;
                            WSDLOperation::soapVersion soapVer = WSDLOperation::SOAP11;

                            string opType = bindingOp->getType().getURI();
                            if (opType == "http://schemas.xmlsoap.org/wsdl/soap12/")
                            {
                                soapVer = WSDLOperation::SOAP12;
                            }
 
                            // Get the soapAction
                            soapAction = bindingOp->getCString("operation/soapAction");

                            // Get the style
                            string style = bindingOp->getCString("operation/style");
                            if (style == "")
                            {
                                style = wsBinding->getCString("binding/style");
                            }
                            if (style != "document")
                            {
                                documentStyle = false;
                                wrappedStyle = false;
                            }
 
                            // get the use
                            string use = bindingOp->getCString("input/body/use");
                            if (use == "encoded")
                            {
                                useEncodedInput = true;
                            }

                            use = bindingOp->getCString("output/body/use");
                            if (use == "encoded")
                            {
                                useEncodedOutput = true;
                            }

                            // Get the request message type from the PortType
                            DataObjectPtr wsMessageIn =
                              findMessage(portTypeOp->getCString("input/message"));
                            if (!wsMessageIn)
                            {
                                stringstream errMessage;
                                errMessage
                                  << "unable to find PortType input message "
                                  << portTypeOp->getCString("input/message")
                                  << " in the wsdl definition";
                                throwException(SystemConfigurationException, errMessage.str().c_str());
                            }
                            string inputMessageType( wsMessageIn->getCString("name") );
                            // If it doesnt have a namespace prefix, add the target namespace
                            if (inputMessageType.find("#") == string::npos)
                            {
                                inputMessageType.insert(0, (getNamespace() + "#") );
                            }

                            // Get the response message type from the PortType
                            DataObjectPtr wsMessageOut =
                              findMessage(portTypeOp->getCString("output/message"));
                            if (!wsMessageOut)
                            {
                                // TODO this is ok for one way operations, right?
                                stringstream errMessage;
                                errMessage
                                  << "unable to find PortType output message "
                                  << portTypeOp->getCString("output/message")
                                  << " in the wsdl definition";
                                throwException(SystemConfigurationException, errMessage.str().c_str());
                            }
                            string outputMessageType( wsMessageOut->getCString("name") );
                            // If it doesnt have a namespace prefix, add the target namespace
                            if (outputMessageType.find("#") == string::npos)
                            {
                                outputMessageType.insert(0, (getNamespace() + "#") );
                            }

                            WSDLOperation wsdlOp;
                            wsdlOp.setOperationName(operationName);
                            wsdlOp.setSoapAction(soapAction);
                            wsdlOp.setEndpoint(targetAddress);
                            wsdlOp.setSoapVersion(soapVer);
                            wsdlOp.setDocumentStyle(documentStyle);
                            wsdlOp.setWrappedStyle(wrappedStyle);
                            wsdlOp.setInputEncoded(useEncodedInput);
                            wsdlOp.setOutputEncoded(useEncodedOutput);
                            wsdlOp.setInputMessageType(inputMessageType);
                            wsdlOp.setOutputMessageType(outputMessageType);
                            wsdlOp.setInputMessage(wsMessageIn);
                            wsdlOp.setOutputMessage(wsMessageOut);

                            operationMap[ operationName ] = wsdlOp;

                        } // end bindingOperationList

                        portTypeMap[portTypeName] = operationMap;
                        servicePortMap[(serviceName+"#"+portName)] = operationMap;

                    } // end portTypeList
                } // end serviceList
            } // end method mapOperations

        } // end namespace model
    } // end namespace sca
} // End namespace tuscany
