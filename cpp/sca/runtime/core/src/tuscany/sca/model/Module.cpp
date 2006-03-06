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
#include "tuscany/sca/model/Module.h"


namespace tuscany
{
    namespace sca
    {
        
        namespace model
        {
            
            // Constructor
            Module::Module(const std::string& moduleName) 
                : name(moduleName)
            {
                LOGENTRY(1, "Module::constructor");
                LOGEXIT(1, "Module::constructor");
            }
            
            Module::~Module()
            {
            }
            
            void Module::setRoot(const std::string& rootDirectory)
            {
                moduleRoot = rootDirectory;
            }
            
            
            ///
            /// Add a new component to the module component
            ///
            Component* Module::addComponent(const std::string& name)
            {
                LOGENTRY(1, "Module::addComponent");
                Component* newComponent = new Component(name, this);
                components[name] = newComponent;
                LOGEXIT(1, "Module::addComponent");
                return newComponent;
            }
            
            Component* Module::findComponent(const std::string& name)
            {
                LOGENTRY(1, "Module::findComponent");
                Component* foundComponent = components[name];
                LOGEXIT(1, "Module::findComponent");
                return foundComponent;
            }
            
            Service* Module::findComponentService(const std::string& name)
            {
                LOGENTRY(1, "Module::findComponentService");
                
                Service* service = 0;
                
                string componentName;
                string serviceName;
                Utils::tokeniseUri(name, componentName, serviceName);
                
                // Locate the component
                Component* foundComponent = components[name];
                if (foundComponent)
                {
                    // Locate the service
                    service = foundComponent->findService(serviceName);
                }
                LOGEXIT(1, "Module::findComponentService");
                return service;
            }
            
            ExternalService* Module::findExternalService(const std::string& name)
            {
                LOGENTRY(1, "Module::findExternalService");
                ExternalService* foundService = externalServices[name];
                LOGEXIT(1, "Module::findExternalService");
                return foundService;
            }
            
            
            EntryPoint* Module::addEntryPoint(const std::string& name)
            {
                LOGENTRY(1, "Module::addEntryPoint");
                EntryPoint* ep = new EntryPoint(name);
                entryPoints[name] = ep;
                LOGEXIT(1, "Module::addEntryPoint");
                return findEntryPoint(name);
            }
            
            EntryPoint* Module::findEntryPoint(const std::string& name)
            {
                return entryPoints[name];
            }


            ExternalService* Module::addExternalService(const std::string& name)
            {
                LOGENTRY(1, "Module::addExternalService");
                ExternalService* es = new ExternalService(name, this);
                externalServices[name] = es;
                LOGEXIT(1, "Module::addExternalService");
                return es;
            }
            
            void Module::addWire(const std::string& source, const std::string& target)
            {
                LOGENTRY(1, "Module::addWire");
                wires.push_back(Wire(source, target));
                LOGEXIT(1, "Module::addWire");
            }
            
            
            void Module::resolveWires()
            {
                LOGENTRY(1, "Module::resolveWires");
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
                        target = findExternalService(iter->getTarget());
                    }
                    if (!target)
                    {
                        LOGERROR_1(0, "Module::resolveWires: Wire target %s not found", iter->getTarget().c_str());
                    }
                    else
                    {
                        EntryPoint* entrypoint = findEntryPoint(iter->getSourceComponent());
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
                                    LOGERROR_1(0, "Module::resolveWires: Wire source reference %s not found", iter->getSourceReference().c_str());
                                }
                            }
                            else
                            {
                                LOGERROR_1(0, "Module::resolveWires: Wire source %s not found", iter->getSourceComponent().c_str());
                            }
                        }
                    }
                }

                LOGEXIT(1, "Module::resolveWires");
            }


            
            void Module::addWsdl(commonj::sdo::DataObjectPtr wsdlModel)
            {
                LOGENTRY(1, "Module::addWsdl");
                Wsdl* wsdl = new Wsdl(wsdlModel);
                wsdls[wsdl->getNamespace()] = wsdl;
                LOGEXIT(1, "Module::addWsdl");

            }

            Wsdl* Module::findWsdl(const std::string& wsdlNamespace )
            {
                return wsdls[wsdlNamespace];

            }


            // Get an XSDHelper - one will be created for each module
            commonj::sdo::XSDHelperPtr Module::getXSDHelper()
            {
                if (xsdHelper == 0)
                {
                    xsdHelper = commonj::sdo::HelperProvider::getXSDHelper();
                }

                return xsdHelper;
            }

            // Get an XMLHelper - one will be created for each module
            commonj::sdo::XMLHelperPtr Module::getXMLHelper()
            {
                if (xmlHelper == 0)
                {
                    xmlHelper = commonj::sdo::HelperProvider::getXMLHelper(getXSDHelper()->getDataFactory());
                }

                return xmlHelper;
            }


            // Get a data factory - the one in the xsd/xml helper
            commonj::sdo::DataFactoryPtr Module::getDataFactory()
            {
                return getXSDHelper()->getDataFactory();
            }
            
        } // End namespace model
        
    } // End namespace sca
} // End namespace tuscany
