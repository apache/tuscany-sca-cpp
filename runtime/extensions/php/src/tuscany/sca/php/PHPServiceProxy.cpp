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

#include "tuscany/sca/php/PHPServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/php/model/PHPReferenceBinding.h"

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace php
        {
            
            // ============================================
            // Constructor: Create a proxy from a reference
            // ============================================
            PHPServiceProxy::PHPServiceProxy(Reference* reference)
                : ServiceProxy(reference)
            {
                logentry();
 
                // Get the service wrapper
                PHPReferenceBinding* referenceBinding = (PHPReferenceBinding*)reference->getBinding();                
                serviceWrapper = referenceBinding->getTargetServiceBinding()->getServiceWrapper();
            }

            // ==========================================
            // Constructor: Create a proxy from a service
            // ==========================================
            PHPServiceProxy::PHPServiceProxy(Service* service)
                : ServiceProxy(NULL)
            {
                logentry();
 
                // Get the service wrapper
                serviceWrapper = service->getBinding()->getServiceWrapper();                
            }

            // ==========
            // Destructor
            // ==========
            PHPServiceProxy::~PHPServiceProxy()
            {
                logentry();
            }

            // =====================================================
            // invokeService: invoke the service wired to this proxy
            // =====================================================
            void PHPServiceProxy::invokeService(Operation& operation)
            {
                logentry();

                // Invoke the service
                serviceWrapper->invoke(operation);
            }
        } // End namespace php        
    } // End namespace sca
} // End namespace tuscany
