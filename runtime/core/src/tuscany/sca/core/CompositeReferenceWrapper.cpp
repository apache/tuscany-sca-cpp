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

#include "tuscany/sca/core/CompositeReferenceWrapper.h"
#include <stdarg.h>

#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"

#include "tuscany/sca/ws/WSReferenceWrapper.h"
using tuscany::sca::ws::WSReferenceWrapper;

using namespace osoa::sca;


namespace tuscany
{
    namespace sca
    {
        
        //////////////////////////////////////////////////////////////////////
        // Construction/Destruction
        //////////////////////////////////////////////////////////////////////
        
        CompositeReferenceWrapper::CompositeReferenceWrapper(tuscany::sca::model::WireTarget* target)
            : ServiceWrapper(target)
        {
            LOGENTRY(1,"CompositeReferenceWrapper::constructor");
            if (target->getServiceType() != WireTarget::CompositeReferenceTypeType)
            {
                throw ServiceRuntimeException("CompositeReferenceWrapper: target is not an CompositeReferenceType");
            }
            
            LOGEXIT(1,"CompositeReferenceWrapper::constructor");
            
        }
        
        CompositeReferenceWrapper::~CompositeReferenceWrapper()
        {
            LOGENTRY(1,"CompositeReferenceWrapper::destructor");
            LOGEXIT(1,"CompositeReferenceWrapper::destructor");
        }

        // ======================================================================
        // createServiceWrapper: create a wrapper for the target CompositeReferenceType
        // ======================================================================
        CompositeReferenceWrapper* CompositeReferenceWrapper::createServiceWrapper(CompositeReferenceType* service)
        {            
            string msg;
            CompositeReferenceWrapper* serviceWrapper = 0;

            // -----------------------------------------------
            // Get the binding for the target component
            // -----------------------------------------------
            Binding* targetBinding = service->getBinding();
            if (!targetBinding)
            {
                msg = "External service " + service->getName() + " has no binding defined";
                throw ServiceNotFoundException(msg.c_str());
            }
            
            // TODO: This only handles ws implementation for now
            if (targetBinding->getBindingType() == Binding::WS)
            {
                // -------------------------------------
                // Create an instance of the wrapper
                // -------------------------------------                
                serviceWrapper = new WSReferenceWrapper(service);
                
            }
            else
            {
                // ----------------------
                // Non WS Binding
                // ----------------------
                msg = "Unsupported binding type for externalService: " + service->getName();
                throw ServiceNotFoundException(msg.c_str());
            }        
            
            return serviceWrapper;
        }    
    
        
    } // End namespace sca
} // End namespace tuscany
