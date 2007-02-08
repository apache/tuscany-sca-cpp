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
#include "tuscany/sca/python/model/PythonImplementation.h"
#include "tuscany/sca/python/model/PythonServiceBinding.h"
#include "tuscany/sca/python/model/PythonReferenceBinding.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/util/Utils.h"

using namespace std;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {

        namespace python
        {

            // Constructor
            PythonImplementation::PythonImplementation(
                Composite* composite, const string& module, const string& modulePath, const string& className, Scope scope)
                : ComponentType(composite, modulePath + "/" + module),
                    module(module), modulePath(modulePath), className(className), scope(scope)
            {
                logentry();

                // Create a default service for this componentType
                ServiceType* defaultServiceType = new ServiceType(this, "", NULL, NULL);
                addServiceType(defaultServiceType);
            }

            PythonImplementation::~PythonImplementation()
            {
                logentry();
            }

            /**
             * Overrides the findReferenceType method in ComponentType.
             * This allows us to create references without needing a componentType file.
             */
            ReferenceType* PythonImplementation::findReferenceType(const string& referenceName)
            {
                logentry();

                ReferenceType* refType = ComponentType::findReferenceType(referenceName); 
                if(!refType)
                {
                    // The reference has not yet been created - try creating it
                    refType = new ReferenceType(this, referenceName, NULL, NULL, ReferenceType::ONE_ONE);
                    addReferenceType(refType);
                }

                return refType;
            }
            
            /**
             * Overrides the findPropertyType method in ComponentType.
             * This allows us to create properties without needing a componentType file.
             */
            const commonj::sdo::Property* PythonImplementation::findPropertyType(const string& propertyName)
            {
                logentry();

                const commonj::sdo::Property* prop = ComponentType::findPropertyType(propertyName); 
                if(!prop)
                {
                    ComponentType::addPropertyType(propertyName, "http://www.w3.org/2001/XMLSchema#string", false, NULL);
                    // Just added it so it should now be available
                    prop = ComponentType::findPropertyType(propertyName);
                    loginfo("Added string property named %s to Python component", propertyName.c_str());
                }

                return prop;
            }
            
            void PythonImplementation::initializeComponent(Component* component)
            {
                logentry();
                ComponentType::initializeComponent(component);
                
                // Create Python bindings for all the services
                const Component::SERVICE_MAP& services = component->getServices();
                Component::SERVICE_MAP::const_iterator iter = services.begin();
                for (unsigned int i=0; i< services.size(); i++)
                {
                    Service *service = iter->second;
                    PythonServiceBinding* binding = new PythonServiceBinding(service);
                    service->setBinding(binding);
                    iter++;
                }
                
                // Create Python bindings for all the references
                const Component::REFERENCE_MAP& references = component->getReferences();
                Component::REFERENCE_MAP::const_iterator refiter = references.begin();
                for (int ri=0; ri< references.size(); ri++)
                {
                    Reference *reference = refiter->second;
                    PythonReferenceBinding* binding = new PythonReferenceBinding(reference);
                    reference->setBinding(binding);
                    refiter++;
                }
            }
            
        } // End namespace python
    } // End namespace sca
} // End namespace tuscany
