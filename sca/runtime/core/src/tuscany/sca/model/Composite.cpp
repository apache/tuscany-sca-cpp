/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/Composite.h"


namespace tuscany
{
    namespace sca
    {
        
        namespace model
        {
            
            // Constructor
            Composite::Composite(const std::string& compositeName) 
                : name(compositeName)
            {
                LOGENTRY(1, "Composite::constructor");
                LOGEXIT(1, "Composite::constructor");
            }
            
            Composite::~Composite()
            {
            }
            
            void Composite::setRoot(const std::string& rootDirectory)
            {
                compositeRoot = rootDirectory;
            }
            
            
            ///
            /// Add a new component to the composite component
            ///
            Component* Composite::addComponent(const std::string& name)
            {
                LOGENTRY(1, "Composite::addComponent");
                Component* newComponent = new Component(name, this);
                components[name] = newComponent;
                LOGEXIT(1, "Composite::addComponent");
                return newComponent;
            }
            
            Component* Composite::findComponent(const std::string& name)
            {
                LOGENTRY(1, "Composite::findComponent");
                Component* foundComponent = components[name];
                LOGEXIT(1, "Composite::findComponent");
                return foundComponent;
            }
            
            Service* Composite::findComponentService(const std::string& name)
            {
                LOGENTRY(1, "Composite::findComponentService");
                
                Service* service = 0;
                
                string componentName;
                string serviceName;
                Utils::tokeniseUri(name, componentName, serviceName);
                
                // Locate the component
                Component* foundComponent = components[componentName];
                if (foundComponent)
                {
                    // Locate the service
                    service = foundComponent->findService(serviceName);
                }
                LOGEXIT(1, "Composite::findComponentService");
                return service;
            }
            
            CompositeReferenceType* Composite::findCompositeReferenceType(const std::string& name)
            {
                LOGENTRY(1, "Composite::findCompositeReferenceType");
                CompositeReferenceType* foundService = externalServices[name];
                LOGEXIT(1, "Composite::findCompositeReferenceType");
                return foundService;
            }
            
            
            CompositeServiceType* Composite::addCompositeServiceType(const std::string& name)
            {
                LOGENTRY(1, "Composite::addCompositeServiceType");
                CompositeServiceType* ep = new CompositeServiceType(name);
                compositeServices[name] = ep;
                LOGEXIT(1, "Composite::addCompositeServiceType");
                return findCompositeServiceType(name);
            }
            
            CompositeServiceType* Composite::findCompositeServiceType(const std::string& name)
            {
                return compositeServices[name];
            }


            CompositeReferenceType* Composite::addCompositeReferenceType(const std::string& name)
            {
                LOGENTRY(1, "Composite::addCompositeReferenceType");
                CompositeReferenceType* es = new CompositeReferenceType(name, this);
                externalServices[name] = es;
                LOGEXIT(1, "Composite::addCompositeReferenceType");
                return es;
            }
            
            void Composite::addWire(const std::string& source, const std::string& target)
            {
                LOGENTRY(1, "Composite::addWire");
                wires.push_back(Wire(source, target));
                LOGEXIT(1, "Composite::addWire");
            }
            
            
            void Composite::resolveWires()
            {
                LOGENTRY(1, "Composite::resolveWires");
                 for (WIRES::iterator iter = wires.begin();
                iter != wires.end();
                iter++)
                {
                    // -----------------
                    // Locate the target
                    // -----------------
                    WireTarget* target = findComponentService(iter->getTarget());
                    if (!target)
                    {
                        target = findCompositeReferenceType(iter->getTarget());
                    }
                    if (!target)
                    {
                        LOGERROR_1(0, "Composite::resolveWires: Wire target %s not found", iter->getTarget().c_str());
                    }
                    else
                    {
                        CompositeServiceType* entrypoint = findCompositeServiceType(iter->getSourceComponent());
                        if (entrypoint)
                        {
                            entrypoint->addTarget(target);
                        }
                        else
                        {
                            Component* component = findComponent(iter->getSourceComponent());
                            if (component)
                            {
                                ServiceReference* serviceReference = component->findReference(iter->getSourceReference());
                                if (serviceReference)
                                {
                                    serviceReference->addTarget(target);
                                }
                                else
                                {
                                    LOGERROR_1(0, "Composite::resolveWires: Wire source reference %s not found", iter->getSourceReference().c_str());
                                }
                            }
                            else
                            {
                                LOGERROR_1(0, "Composite::resolveWires: Wire source %s not found", iter->getSourceComponent().c_str());
                            }
                        }
                    }
                }

                LOGEXIT(1, "Composite::resolveWires");
            }


            
            void Composite::addWsdl(commonj::sdo::DataObjectPtr wsdlModel)
            {
                LOGENTRY(1, "Composite::addWsdl");
                Wsdl* wsdl = new Wsdl(wsdlModel);
                wsdls[wsdl->getNamespace()] = wsdl;
                LOGEXIT(1, "Composite::addWsdl");

            }

            Wsdl* Composite::findWsdl(const std::string& wsdlNamespace )
            {
                return wsdls[wsdlNamespace];

            }


            // Get an XSDHelper - one will be created for each composite
            commonj::sdo::XSDHelperPtr Composite::getXSDHelper()
            {
                if (xsdHelper == 0)
                {
                    xsdHelper = commonj::sdo::HelperProvider::getXSDHelper();
                }

                return xsdHelper;
            }

            // Get an XMLHelper - one will be created for each composite
            commonj::sdo::XMLHelperPtr Composite::getXMLHelper()
            {
                if (xmlHelper == 0)
                {
                    xmlHelper = commonj::sdo::HelperProvider::getXMLHelper(getXSDHelper()->getDataFactory());
                }

                return xmlHelper;
            }


            // Get a data factory - the one in the xsd/xml helper
            commonj::sdo::DataFactoryPtr Composite::getDataFactory()
            {
                return getXSDHelper()->getDataFactory();
            }
            
        } // End namespace model
        
    } // End namespace sca
} // End namespace tuscany
