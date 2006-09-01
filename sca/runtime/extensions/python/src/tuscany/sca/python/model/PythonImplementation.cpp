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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/python/model/PythonImplementation.h"
#include "tuscany/sca/python/model/PythonServiceBinding.h"
//#include "tuscany/sca/python/model/PythonReferenceBinding.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/util/Utils.h"

namespace tuscany
{
    namespace sca
    {

        namespace python
        {

            // Constructor
            PythonImplementation::PythonImplementation(const string& module, const string& modulePath, const string& className)
                : ComponentType(modulePath + "/" + module),
                    module(module), modulePath(modulePath), className(className)
            {
                LOGENTRY(1,"PythonImplementation::constructor");
                LOGEXIT(1,"PythonImplementation::constructor");
            }

            PythonImplementation::~PythonImplementation()
            {
                LOGENTRY(1,"PythonImplementation::destructor");
                LOGEXIT(1,"PythonImplementation::destructor");
            }
            
            void PythonImplementation::initializeComponent(Component* component)
            {
                LOGENTRY(1,"PythonImplementation::initializeComponent");
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
                
                // References not yet supported..

                //// Create Python bindings for all the references
                //const Component::REFERENCE_MAP& references = component->getReferences();
                //Component::REFERENCE_MAP::const_iterator refiter = references.begin();
                //for (int ri=0; ri< references.size(); ri++)
                //{
                //    Reference *reference = refiter->second;
                //    PythonReferenceBinding* binding = new PythonReferenceBinding(reference);
                //    reference->setBinding(binding);
                //    refiter++;
                //}
                LOGEXIT(1,"PythonImplementation::initializeComponent");
            }
            
        } // End namespace python
    } // End namespace sca
} // End namespace tuscany
