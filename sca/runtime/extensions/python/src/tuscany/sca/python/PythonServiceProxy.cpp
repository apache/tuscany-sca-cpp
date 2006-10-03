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

#include "tuscany/sca/python/PythonServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "osoa/sca/ServiceRuntimeException.h"
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
#include "tuscany/sca/python/model/PythonImplementation.h"
#include "tuscany/sca/python/model/PythonReferenceBinding.h"

using namespace osoa::sca;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            
            // ============================================
            // Constructor: Create a proxy from a reference
            // ============================================
            PythonServiceProxy::PythonServiceProxy(Reference* reference)
                : ServiceProxy(reference)
            {
                LOGENTRY(1,"PythonServiceProxy::constructor(Reference)");
 
                // Get the service wrapper
                PythonReferenceBinding* referenceBinding = (PythonReferenceBinding*)reference->getBinding();                
                serviceWrapper = referenceBinding->getTargetServiceBinding()->getServiceWrapper();
    
                LOGEXIT(1,"PythonServiceProxy::constructor(Reference)");
            }

            // ==========================================
            // Constructor: Create a proxy from a service
            // ==========================================
            PythonServiceProxy::PythonServiceProxy(Service* service)
                : ServiceProxy(NULL)
            {
                LOGENTRY(1,"PythonServiceProxy::constructor(Service)");
 
                // Get the service wrapper
                serviceWrapper = service->getBinding()->getServiceWrapper();                
    
                LOGEXIT(1,"PythonServiceProxy::constructor(Service)");
            }

            // ==========
            // Destructor
            // ==========
            PythonServiceProxy::~PythonServiceProxy()
            {
                LOGENTRY(1,"PythonServiceProxy::destructor");
                LOGEXIT(1,"PythonServiceProxy::destructor");
            }

            // =====================================================
            // invokeService: invoke the service wired to this proxy
            // =====================================================
            void PythonServiceProxy::invokeService(Operation& operation)
            {
                LOGENTRY(1,"PythonServiceProxy::invokeService");

                // Invoke the service
                serviceWrapper->invoke(operation);

                LOGEXIT(1,"PythonServiceProxy::invokeService");
            }
        } // End namespace python        
    } // End namespace sca
} // End namespace tuscany
