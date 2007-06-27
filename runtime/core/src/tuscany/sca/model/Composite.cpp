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
#include "tuscany/sca/model/CompositeReference.h"
#include "tuscany/sca/model/CompositeReferenceBinding.h"
#include "tuscany/sca/model/CompositeService.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/Wire.h"
#include "tuscany/sca/model/WSDLDefinition.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/model/ReferenceBinding.h"

using namespace std;
using namespace tuscany::sca::util;

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
            
            void Composite::addCompositeReference(CompositeReference* compositeReference)
            {
                logentry(); 
                components[compositeReference->getName()] = compositeReference;

                // Create a reference type describing the composite reference                
                ServiceType* serviceType = compositeReference->getType()->findServiceType("");
                ReferenceType* referenceType = new ReferenceType(
                    this, compositeReference->getName(),
                    serviceType->getInterface(),
                    serviceType->getCallbackInterface(),
                    compositeReference->getMultiplicity());
                addReferenceType(referenceType);
            }
            
            void Composite::addCompositeService(CompositeService* compositeService)
            {
                logentry(); 
                components[compositeService->getName()] = compositeService;
                
                // Create a service type describing the composite service
                ReferenceType* referenceType = compositeService->getType()->findReferenceType("");
                ServiceType* serviceType = new ServiceType(
                    this, compositeService->getName(),
                    referenceType->getInterface(),
                    referenceType->getCallbackInterface());
                addServiceType(serviceType);
            }
            
            void Composite::initializeComponent(Component* component)
            {
                ComponentType::initializeComponent(component);
                
                // Create bindings for all the services
                const Component::SERVICE_MAP& services = component->getServices();
                Component::SERVICE_MAP::const_iterator iter = services.begin();
                for (unsigned int i=0; i< services.size(); i++)
                {
                    Service *service = iter->second;
                    //CompositeServiceBinding* binding = new CompositeServiceBinding(service);
                    //service->setBinding(binding);
                    iter++;
                }
                
                // Create bindings for all the references
                const Component::REFERENCE_MAP& references = component->getReferences();
                Component::REFERENCE_MAP::const_iterator refiter = references.begin();
                for (unsigned int ri=0; ri< references.size(); ri++)
                {
                    Reference *reference = refiter->second;
                    CompositeReferenceBinding* binding = new CompositeReferenceBinding(reference);
                    reference->setBinding(binding);
                    refiter++;
                }
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
                includes[composite->getName()] = composite;

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

                    // Find the source component and reference
                    Component* component = findComponent(wire->getSourceComponent());
                    Reference* reference;                    
                    if (component)
                    {
                        reference = component->findReference(wire->getSourceReference());
                        if (!reference)
                        {
                            logerror("Wire source reference %s not found", wire->getSourceReference().c_str());
                        }
                    }
                    else
                    {
                        reference = NULL;
                        logerror("Wire source %s not found", wire->getSourceComponent().c_str());
                    }
                    
                    // Configure the reference binding with the wire target URI 
                    if (reference)
                    {
                        reference->getBinding()->configure(wire->getTarget());
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
                // remember the map operator[] inserts a blank object if key not found
                WSDL_MAP::iterator iter = wsdlDefinitions.find( wsdlNamespace );
                if( iter == wsdlDefinitions.end() )
                {
                    return NULL;
                }

                return iter->second;
            }

            std::list<std::string> Composite::getWSDLNamespaces()
            {
                logentry();
                std::list<std::string> namespaceList;
                WSDL_MAP::const_iterator iter = wsdlDefinitions.begin();
                WSDL_MAP::const_iterator iterEnd = wsdlDefinitions.end();

                for( ; iter != iterEnd; ++iter )
                {
                    namespaceList.push_back( iter->first );
                }

                return namespaceList;
            }

            Composite* Composite::findIncludedComposite(const std::string& compositeName)
            {
                logentry();

                // remember the map operator[] inserts a blank object if key not found
                INCLUDES::iterator iter = includes.find( compositeName );
                if( iter == includes.end() )
                {
                  return NULL;
                }

                return iter->second;
            }

            std::list<std::string> Composite::getIncludedComposites()
            {
                logentry();
                std::list<std::string> compositeList;
                INCLUDES::const_iterator iter = includes.begin();
                INCLUDES::const_iterator iterEnd = includes.end();

                for( ; iter != iterEnd; ++iter )
                {
                    compositeList.push_back( iter->first );
                }

                return compositeList;
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
