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
#include "tuscany/sca/model/ReferenceBinding.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Service.h"


using namespace std;

namespace tuscany
{
    namespace sca
    {

        namespace model
        {

            // Constructor
            ReferenceBinding::ReferenceBinding(Reference *reference, const string& uri) :
                Binding(uri), reference(reference), targetServiceBinding(0)
            {
                logentry(); 
            }

            // Destructor
            ReferenceBinding::~ReferenceBinding()
            {
                logentry(); 
            }

            void ReferenceBinding::configure(const string& uri)
            {
                logentry();

                // Find the target service
                Component* component = reference->getComponent();
                Composite* composite = component->getComposite();
                Service* service;
                service = composite->findComponentService(uri);
                if (!service)
                {
                    logerror("Wire target %s not found", uri.c_str());
                }
                else
                {
                    // Configure this binding from the target service binding
                    configure(service->getBinding());
                }
            }
                                
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
