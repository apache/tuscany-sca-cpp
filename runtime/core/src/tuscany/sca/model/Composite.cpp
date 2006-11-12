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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Wire.h"
#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/model/ReferenceBinding.h"

namespace tuscany
{
    namespace sca
    {
        
        namespace model
        {
            
            // Constructor
            Composite::Composite(const string& name, const string& root) 
                : ComponentType(0, name), root(root)
            {
                logentry(); 
            }
            
            // Destructor
            Composite::~Composite()
            {
                logentry(); 
            }
            
            void Composite::addComponent(Component* component)
            {
                logentry(); 
                components[component->getName()] = component;
            }
            
            Component* Composite::findComponent(const std::string& name)
            {
                logentry(); 
                Component* component = components[name];
                return component;
            }
            
            Service* Composite::findComponentService(const std::string& name)
            {
                logentry(); 
                
                Service* service = 0;
                
                string componentName;
                string serviceName;
                Utils::tokeniseUri(name, componentName, serviceName);
                
                // Locate the component
                Component* component = findComponent(componentName);
                if (component)
                {
                    // Locate the service
                    service = component->findService(serviceName);
                }
                return service;
            }
            
            void Composite::addWire(const std::string& source, const std::string& target)
            {
                logentry(); 
                Wire* wire=new Wire(source, target);
                wires.push_back(wire);
            }
            
            void Composite::addInclude(Composite* composite)
            {
                logentry(); 
                includes.push_back(composite);

                for (COMPONENT_MAP::iterator iter = composite->components.begin();
                iter != composite->components.end();
                iter++)
                {
                    components[iter->first] = iter->second;
                } 
            }
            
            void Composite::resolveWires()
            {
                logentry(); 

                for (WIRES::iterator iter = wires.begin();
                iter != wires.end();
                iter++)
                {
                    Wire* wire = *iter;
                    
                    // Locate the target
                    Service* service = findComponentService(wire->getTarget());
                    if (!service)
                    {
                        logerror("Wire target %s not found", wire->getTarget().c_str());
                    }
                    else
                    {
                        Component* component = findComponent(wire->getSourceComponent());
                        if (component)
                        {
                            Reference* reference = component->findReference(wire->getSourceReference());
                            if (reference)
                            {
                                
                                // Configure the binding on the reference from the binding on the target
                                // service
                                reference->getBinding()->configure(service->getBinding());
                            }
                            else
                            {
                                logerror("Wire source reference %s not found", wire->getSourceReference().c_str());
                            }
                        }
                        else
                        {
                            logerror("Wire source %s not found", wire->getSourceComponent().c_str());
                        }
                    }
                }
            }

            void Composite::addWSDLDefinition(commonj::sdo::DataObjectPtr wsdlModel)
            {
                logentry(); 
                WSDLDefinition* wsdlDefinition = new WSDLDefinition(wsdlModel);
                
                WSDLDefinition* existingDefinition = wsdlDefinitions[wsdlDefinition->getNamespace()];
                if (existingDefinition == NULL)
                {
                    wsdlDefinitions[wsdlDefinition->getNamespace()] = wsdlDefinition;
                }
                else{
                   existingDefinition->addWSDLModel(wsdlModel); 
                }
            }

            WSDLDefinition* Composite::findWSDLDefinition(const std::string& wsdlNamespace )
            {
                logentry(); 
                return wsdlDefinitions[wsdlNamespace];

            }

            commonj::sdo::XSDHelperPtr Composite::getXSDHelper()
            {
                logentry(); 
                if (xsdHelper == 0)
                {
                    xsdHelper = commonj::sdo::HelperProvider::getXSDHelper();
                }

                return xsdHelper;
            }

            commonj::sdo::XMLHelperPtr Composite::getXMLHelper()
            {
                logentry(); 
                if (xmlHelper == 0)
                {
                    xmlHelper = commonj::sdo::HelperProvider::getXMLHelper(getXSDHelper()->getDataFactory());
                }

                return xmlHelper;
            }

            commonj::sdo::DataFactoryPtr Composite::getDataFactory()
            {
                logentry(); 
                return getXSDHelper()->getDataFactory();
            }
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
