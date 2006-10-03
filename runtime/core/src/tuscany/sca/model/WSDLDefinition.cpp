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

#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/WSDLOperation.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"

using namespace tuscany::sca;

using namespace commonj::sdo;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            
            // Constructor
            WSDLDefinition::WSDLDefinition(DataObjectPtr wsdlModel) 
                : wsdlModel(wsdlModel)
            {
                LOGENTRY(1, "WSDLDefinition::constructor");

                // Trace
                //Utils::printDO(wsdlModel);
                
                LOGEXIT(1, "WSDLDefinition::constructor");
            }

            WSDLDefinition::~WSDLDefinition()
            {
            }


            /// 
            /// The namespace of the service and other definitions defined in this wsdl definition
            string WSDLDefinition::getNamespace()
            {
                return wsdlModel->getCString("targetNamespace");
            }

            ///
            /// Find the operation defined in this wsdl
            ///
            const WSDLOperation& WSDLDefinition::findOperation(const string& serviceName, 
                                        const string& portName, 
                                        const string& operationName)
            {
                string message;
                
                string operationKey = serviceName+"#"+portName+"#"+operationName;
                OperationMap::iterator iter = operationMap.find(operationKey);
                if (iter != operationMap.end())
                {
                	return iter->second;
                }
                
                // Find the service
                DataObjectPtr service = findService(serviceName);
                if (!service)
                {
                    // Service not found
                    message = "Unable to find service ";
                    message = message + serviceName;
                    message = message + " in the WSDL definition";
                    throw SystemConfigurationException(message.c_str());
                }
                else
                {
                    
                    
                    // Found the service
                    DataObjectList& portList = service->getList("port");
                    for (int j=0; j<portList.size();j++)
                    {
                        string portListName(portList[j]->getCString("name"));
                        if (portListName.compare(portName) == 0)
                        {
                            // found port
                            // Add address at this point
                            string targetAddress(portList[j]->getCString("address/location"));
                            
                            // find operation by traversing the binding, portType then operation
                            string wsBindingName(portList[j]->getCString("binding"));
                            
                            DataObjectPtr wsBinding = findBinding(wsBindingName);
                            if (!wsBinding)
                            {
                                message = "Unable to find binding ";
                                message = message + wsBindingName;
                                message = message + " in the WSDL definition";
                                throw SystemConfigurationException(message.c_str());
                            }
                            

                            string soapAction = "";
                            bool documentStyle = true;
                            bool wrappedStyle = true;
                            bool useEncoded = false;
                            WSDLOperation::soapVersion soapVer = WSDLOperation::SOAP11;

                            // Find the binding operation
                            DataObjectList& bindingOperationList = wsBinding->getList("operation");
                            for (int i=0; i<bindingOperationList.size(); i++)
                            {
                                string name(bindingOperationList[i]->getCString("name"));
                                
                                if (name.compare(operationName) == 0)
                                {
                                    DataObjectPtr op = bindingOperationList[i]->getDataObject("operation");
                                    string opType = op->getType().getURI();
                                    if (opType == "http://schemas.xmlsoap.org/wsdl/soap12/")
                                    {
                                        soapVer = WSDLOperation::SOAP12;
                                    }
 
                                    // Get the soapAction
                                    soapAction = bindingOperationList[i]->getCString("operation/soapAction");
                                    
                                    // Get the style
                                    string style = bindingOperationList[i]->getCString("operation/style");
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
                                    string use = bindingOperationList[i]->getCString("input/body/use");
                                    if (use == "encoded")
                                    {
                                        useEncoded = true;
                                    }
                                }
                            }

                            
                            // TODO - get the style from the binding or operation????
                            
                            // Found the binding, get the portType
                            string wsPortTypeName(wsBinding->getCString("type"));
                            DataObjectPtr wsPortType = findPortType(wsPortTypeName);
                            if (!wsPortType)
                            {
                                message = "Unable to find PortType ";
                                message = message + wsPortTypeName;
                                message = message + " in the WSDL definition";
                                throw SystemConfigurationException(message.c_str());
                            }

                            //Utils::printDO(wsPortType);
                            
                            // Found the portType, find the operation
                            DataObjectList& operationList = wsPortType->getList("operation");
                            for (int k=0; k< operationList.size(); k++)
                            {
                                string opName(operationList[k]->getCString("name"));
                                if( opName.compare(operationName) == 0)
                                {
                                    // Found the operation

                                    // Find the type of the request message
                                    string inputMessageType =  string(operationList[k]->getCString("input/message"));

                                    DataObjectPtr wsMessageIn = findMessage(inputMessageType);
                                    if (!wsMessageIn)
                                    {
                                        message = "Unable to find message ";
                                        message = message + inputMessageType;
                                        message = message + " in the WSDL definition";
                                        throw SystemConfigurationException(message.c_str());
                                    }

                                    string requestType(wsMessageIn->getList("part")[0]->getCString("element"));

                                    // Find the type of the response message
                                    string outputMessageType =  string(operationList[k]->getCString("output/message"));

                                    DataObjectPtr wsMessageOut = findMessage(outputMessageType);
                                    if (!wsMessageOut)
                                    {
                                        message = "Unable to find message ";
                                        message = message + outputMessageType;
                                        message = message + " in the WSDL definition";
                                        throw SystemConfigurationException(message.c_str());
                                    }

                                    string responseType(wsMessageOut->getList("part")[0]->getCString("element"));
                                    
                                    WSDLOperation& wsdlOp = operationMap[operationKey];
                                    wsdlOp.setOperationName(operationName);
                                    wsdlOp.setSoapAction(soapAction);
                                    wsdlOp.setEndpoint(targetAddress);
                                    wsdlOp.setSoapVersion(soapVer);
                                    wsdlOp.setDocumentStyle(documentStyle);
                                    wsdlOp.setWrappedStyle(wrappedStyle);
                                    wsdlOp.setEncoded(useEncoded);
                                    wsdlOp.setInputType(requestType);
                                    wsdlOp.setOutputType(responseType);
                                    return wsdlOp;
                                }
                                
                            }
                            
                            message = "Unable to find Operation ";
                            message = message + operationName;
                            message = message + " in the WSDL definition";
                            throw SystemConfigurationException(message.c_str());                           
                        }
                    }
                    // cannot find the port
                    message = "Unable to find port ";
                    message = message + portName;
                    message = message + " in the WSDL definition";
                    throw SystemConfigurationException(message.c_str());
                }
                
            }
            
            ///
            /// Find the operation defined in this wsdl
            ///
            const WSDLOperation& WSDLDefinition::findOperation(const string& portTypeName, 
                                        const string& operationName)
            {
                string operationKey = portTypeName+"#"+operationName;
                OperationMap::iterator iter = operationMap.find(operationKey);
                if (iter != operationMap.end())
                {
                    return iter->second;
                }
                
                string soapAction = getNamespace() + "#" + operationName;
                bool documentStyle = true;
                bool wrappedStyle = true;
                bool useEncoded = false;
                WSDLOperation::soapVersion soapVer = WSDLOperation::SOAP11;

                // Get the portType
                DataObjectPtr wsPortType = findPortType(portTypeName);
                if (!wsPortType)
                {
                    string message = "Unable to find PortType ";
                    message = message + portTypeName;
                    message = message + " in the WSDL definition";
                    throw SystemConfigurationException(message.c_str());
                }

                //Utils::printDO(wsPortType);
                
                // Found the portType, find the operation
                DataObjectList& operationList = wsPortType->getList("operation");
                for (int k=0; k< operationList.size(); k++)
                {
                    string opName(operationList[k]->getCString("name"));
                    if( opName.compare(operationName) == 0)
                    {
                        // Found the operation

                        // Find the type of the request message
                        string inputMessageType =  string(operationList[k]->getCString("input/message"));

                        DataObjectPtr wsMessageIn = findMessage(inputMessageType);
                        if (!wsMessageIn)
                        {
                            string message = "Unable to find message ";
                            message = message + inputMessageType;
                            message = message + " in the WSDL definition";
                            throw SystemConfigurationException(message.c_str());
                        }

                        string requestType(wsMessageIn->getList("part")[0]->getCString("element"));

                        // Find the type of the response message
                        string outputMessageType =  string(operationList[k]->getCString("output/message"));

                        DataObjectPtr wsMessageOut = findMessage(outputMessageType);
                        if (!wsMessageOut)
                        {
                            string message = "Unable to find message ";
                            message = message + outputMessageType;
                            message = message + " in the WSDL definition";
                            throw SystemConfigurationException(message.c_str());
                        }

                        string responseType(wsMessageOut->getList("part")[0]->getCString("element"));
                        
                        WSDLOperation& wsdlOp = operationMap[operationKey];
                        wsdlOp.setOperationName(operationName);
                        wsdlOp.setSoapAction(soapAction);
                        wsdlOp.setEndpoint("");
                        wsdlOp.setSoapVersion(soapVer);
                        wsdlOp.setDocumentStyle(documentStyle);
                        wsdlOp.setWrappedStyle(wrappedStyle);
                        wsdlOp.setEncoded(useEncoded);
                        wsdlOp.setInputType(requestType);
                        wsdlOp.setOutputType(responseType);
                        return wsdlOp;
                    }
                }
                
                string message = "Unable to find Operation ";
                message = message + operationName;
                message = message + " in the WSDL definition";
                throw SystemConfigurationException(message.c_str());                           
                
            }
            
            ///
            /// Find a service
            ///
            DataObjectPtr WSDLDefinition::findService(const string& serviceName)
            {
                DataObjectPtr service = 0;
        

                // Find the binding
                DataObjectList& serviceList = wsdlModel->getList("service");
                for (int i=0; i<serviceList.size(); i++)
                {
                    string name(serviceList[i]->getCString("name"));

                    if (name.compare(serviceName) == 0)
                    {
                        return serviceList[i];
                    }
                }

                return service;
            }


            ///
            /// Find a named binding
            ///
            DataObjectPtr WSDLDefinition::findBinding(const string& bindingName)
            {
                DataObjectPtr binding = 0;
                string uri;
                string name;


                //Utils::tokeniseQName(bindingName, uri, name);
                Utils::rTokeniseString(":", bindingName, uri, name);


                // Find the binding
                DataObjectList& bindingList = wsdlModel->getList("binding");
                for (int i=0; i<bindingList.size(); i++)
                {
                    string nameBinding(bindingList[i]->getCString("name"));

                    if (nameBinding.compare(name) == 0)
                    {
                        return bindingList[i];
                    }
                }

                return binding;
            }

            ///
            /// Find a named portType
            ///
            DataObjectPtr WSDLDefinition::findPortType(const string& portTypeName)
            {
                DataObjectPtr portType = 0;
                string uri;
                string name;


                // Utils::tokeniseQName(portTypeName, uri, name);
                Utils::rTokeniseString(":", portTypeName, uri, name);


                // Find the binding
                DataObjectList& portTypeList = wsdlModel->getList("portType");
                for (int i=0; i<portTypeList.size(); i++)
                {
                    string namePortType(portTypeList[i]->getCString("name"));

                    if (namePortType.compare(name) == 0)
                    {
                        return portTypeList[i];
                    }
                }

                return portType;
            }

            ///
            /// Find a named message
            ///
            DataObjectPtr WSDLDefinition::findMessage(const string& messageName)
            {
                DataObjectPtr message = 0;
                string uri;
                string name;


                // Utils::tokeniseQName(messageName, uri, name);
                Utils::rTokeniseString(":", messageName, uri, name);


                // Find the binding
                DataObjectList& messageList = wsdlModel->getList("message");
                for (int i=0; i<messageList.size(); i++)
                {
                    string nameMessage(messageList[i]->getCString("name"));

                    if (nameMessage.compare(name) == 0)
                    {
                        return messageList[i];
                    }
                }

                return message;
            }
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
