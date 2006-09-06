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
#include "tuscany/sca/ruby/model/RubyImplementation.h"
#include "tuscany/sca/ruby/model/RubyServiceBinding.h"
#include "tuscany/sca/ruby/model/RubyReferenceBinding.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/util/Utils.h"

namespace tuscany
{
    namespace sca
    {

        namespace ruby
        {

            // Constructor
            RubyImplementation::RubyImplementation(const string& module, const string& className, const string& script)
                : ComponentType(script.substr(0, script.find_last_of('.'))),
                    module(module), className(className), script(script)
            {
            }

            RubyImplementation::~RubyImplementation()
            {
            }
            
            void RubyImplementation::initializeComponent(Component* component)
            {
                ComponentType::initializeComponent(component);
                
                // Create Ruby bindings for all the services
                const Component::SERVICE_MAP& services = component->getServices();
                Component::SERVICE_MAP::const_iterator iter = services.begin();
                for (int i=0; i< services.size(); i++)
                {
                    Service *service = iter->second;
                    RubyServiceBinding* binding = new RubyServiceBinding(service);
                    service->setBinding(binding);
                    iter++;
                }
                
                // Create Ruby bindings for all the references
                const Component::REFERENCE_MAP& references = component->getReferences();
                Component::REFERENCE_MAP::const_iterator refiter = references.begin();
                for (int ri=0; ri< references.size(); ri++)
                {
                    Reference *reference = refiter->second;
                    RubyReferenceBinding* binding = new RubyReferenceBinding(reference);
                    reference->setBinding(binding);
                    refiter++;
                }
            }
            
        } // End namespace ruby
    } // End namespace sca
} // End namespace tuscany
