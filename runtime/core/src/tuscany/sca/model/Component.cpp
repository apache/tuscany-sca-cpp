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

#include <iostream>

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/ComponentType.h"

using namespace std;
using namespace commonj::sdo;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
           // Constructor
            Component::Component(Composite* composite, const std::string& componentName, ComponentType *componentType) 
                : name(componentName), composite(composite), type(componentType)
            {
                logentry();
                loginfo("Component name: %s", name.c_str());
                
                // Initialize the component from its component type
                componentType->initializeComponent(this);
                
            }

            Component::~Component()
            {
                logentry();
            }

            void Component::addService(Service* service)
            {
                logentry();
                services[service->getType()->getName()] = service;
            }
            
            Service* Component::findService(const string& serviceName)
            {
                logentry();
                
                // If serviceName is empty then return the ONLY service
                if (serviceName == "" 
                    && services.size() == 1)
                {
                    return services.begin()->second;
                }
                else
                {
                    Service* service = services[serviceName];
                    if (service == NULL)
                    {
                        // A service with an empty name will match any service name 
                        service = services[""];
                    }
                    return service;
                }
            }
            
            void Component::addReference(Reference* reference)
            {
                logentry();
                references[reference->getType()->getName()] = reference;
            }
            
            Reference* Component::findReference(const std::string& referenceName)
            {
                logentry();
                return references[referenceName];
            }

            DataObjectPtr Component::getProperties()
            {
                logentry();
                
                if (!properties)
                {
                    properties = type->getPropertyDataFactory()->create("org/osoa/sca", "Properties");
                }
                return properties;
            }
            
            void Component::setProperty(const string& name, DataObjectPtr value)
            {
                logentry();
                
                DataObjectPtr props = getProperties();

                // Get the property's type
                try
                {
                    const Property& propProperty = props->getProperty(name);
                    const Type& propType = propProperty.getType();
                    if (propType.isDataType())
                    {
                        if (propProperty.isMany())
                        {
                            DataObjectList& dol = props->getList(propProperty);
                            dol.append(value->getCString(""));
                        }
                        else
                        {
                            props->setCString(propProperty, value->getCString(""));
                        }
                    }
                    else
                    {
                        // Create a new instance of the DO
                        // iterate over properties setting each one

                        // for now:
                        props->setDataObject(propProperty, value);
                    }
                }
                catch (SDORuntimeException& ex)
                {
                    // Configuration error: property is not defined
                    throwException(SystemConfigurationException, ex);
                }

            }

        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
