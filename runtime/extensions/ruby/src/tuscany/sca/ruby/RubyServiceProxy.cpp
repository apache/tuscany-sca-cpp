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

#include "tuscany/sca/ruby/RubyServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ComponentType.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/ruby/model/RubyImplementation.h"
#include "tuscany/sca/ruby/model/RubyReferenceBinding.h"

namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            RubyServiceProxy::RubyServiceProxy(Reference* reference)
                : ServiceProxy(reference)
            {
                LOGENTRY(1,"RubyServiceProxy::constructor");
    
                // ----------------------
                // Get the component
                // ----------------------
                Component* component = reference->getComponent();
                string name = reference->getType()->getName();
                
                // Get the service wrapper
                RubyReferenceBinding* referenceBinding = (RubyReferenceBinding*)reference->getBinding();
                
                serviceWrapper = referenceBinding->getTargetServiceBinding()->getServiceWrapper();
    
                LOGEXIT(1,"RubyServiceProxy::constructor");
            }
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            RubyServiceProxy::RubyServiceProxy(Service* service)
                : ServiceProxy(0)
            {
                LOGENTRY(1,"RubyServiceProxy::constructor");
                
                // ----------------------
                // Get the component
                // ----------------------
                Component* component = service->getComponent();
                string name = service->getType()->getName();
                
                // Get the service wrapper
                serviceWrapper = service->getBinding()->getServiceWrapper();
    
                LOGEXIT(1,"RubyServiceProxy::constructor");
            }
            
            // ==========
            // Destructor
            // ==========
            RubyServiceProxy::~RubyServiceProxy()
            {
                LOGENTRY(1,"RubyServiceProxy::destructor");
                LOGEXIT(1,"RubyServiceProxy::destructor");
            }
            
        } // End namespace ruby        
    } // End namespace sca
} // End namespace tuscany
