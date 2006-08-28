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

/* $Rev$ $Date$ */

#include "tuscany/sca/util/Logging.h"
#include "CPPImplementation.h"
#include "CPPServiceBinding.h"
#include "CPPReferenceBinding.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/util/Utils.h"

namespace tuscany
{
    namespace sca
    {

        namespace cpp
        {

            // Constructor
            CPPImplementation::CPPImplementation(const string& library, const string& header,
                    const string&headerPath, const string& headerStub, const string& className)
                : ComponentType(headerPath + headerStub),
                    library(library), header(header), headerPath(headerPath),
                    headerStub(headerStub), className(className)
            {
            }

            CPPImplementation::~CPPImplementation()
            {
            }
            
            void CPPImplementation::initializeComponent(Component* component)
            {
                ComponentType::initializeComponent(component);
                
                // Create CPP bindings for all the services
                Component::SERVICE_MAP services = component->getServices();
                for (Component::SERVICE_MAP::iterator iter = services.begin();
                iter != services.end();
                iter++)
                {
                    Service *service = iter->second;
                    CPPServiceBinding* binding = new CPPServiceBinding(service);
                    service->setBinding(binding);
                }
                
                // Create CPP bindings for all the references
                Component::REFERENCE_MAP references = component->getReferences();
                for (Component::REFERENCE_MAP::iterator iter = references.begin();
                iter != references.end();
                iter++)
                {
                    Reference *reference = iter->second;
                    CPPReferenceBinding* binding = new CPPReferenceBinding(reference);
                    reference->setBinding(binding);
                }
            }
            
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
