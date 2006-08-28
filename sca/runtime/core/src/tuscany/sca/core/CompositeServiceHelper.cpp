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

#include "tuscany/sca/core/CompositeServiceHelper.h"

#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/CompositeService.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/Service.h"
 
using namespace osoa::sca;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        // ===========
        // Constructor
        // ===========
        CompositeServiceHelper::CompositeServiceHelper(const char* compositeServiceName)
            : compositeService(0), composite(0)
        {
            LOGENTRY(1,"CompositeServiceHelper::constructor");

//TODO fix this
    
//            // initialize the system if necessary
//            Composite* system = SCARuntime::getInstance()->getSystem();
//
//            // ---------------------
//            // locate the CompositeService
//            // ---------------------
//            CompositeService compositeService =  system->findComponent(compositeServiceName);
//            if (!compositeService)
//            {
//                throw CompositeServiceNotFoundException(compositeServiceName);
//            }
//            
//            // -----------------------------------
//            // Get the composite and corresponding data factory
//            // -----------------------------------
//            Composite* composite = compositeService->getComposite();
//            if (composite)
//            {
//                dataFactory = composite->getDataFactory();
//                compositeService = composite->findCompositeService(compositeServiceName);
//            }
//
//            // ---------------------------------
//            // Get Wrappers for each wire target
//            // ---------------------------------
//
//            // Get the target services from the CompositeService
//            const CompositeService::TARGETS& targets = compositeService->getTargets();
//            
//            // --------------------
//            // Validate the request
//            // --------------------
//            switch (compositeService->getMultiplicity())
//            {
//            case CompositeService::ONE_MANY:
//            case CompositeService::ONE_ONE:
//                {
//                    if (targets.size() == 0)
//                    {
//                        message = "CompositeService " + compositeService->getName() + " not wired";
//                        throw ServiceNotFoundException(message.c_str());
//                    }
//                }
//            default:
//                {
//                }
//            } // end switch
//
//            for (CompositeService::TARGETS::const_iterator iter = targets.begin();
//            iter!=targets.end();
//            iter++)
//            {
//                ServiceWrapper* service = getServiceWrapper(*iter);
//                if (service)
//                {
//                    services.push_back(service);
//                }
//            }
        
            LOGEXIT(1,"CompositeServiceHelper::constructor");
        }
        
        // ==========
        // Destructor
        // ==========
        CompositeServiceHelper::~CompositeServiceHelper()
        {
            LOGENTRY(1,"CompositeServiceHelper::destructor");

            // Delete any ServiceWrappers
            for (SERVICES::iterator iter = services.begin(); iter < services.end(); iter++)
            {                
                delete (*iter);
            }
            LOGEXIT(1,"CompositeServiceHelper::destructor");
        }
        

        // ======================================================================
        // invoke: 
        // ======================================================================
        SCA_API void CompositeServiceHelper::invoke(Operation& operation)
        {
            LOGENTRY(1,"CompositeServiceHelper::invoke");
            string message;

            // --------------------
            // Validate the request
            // --------------------
            if (services.size() == 0)
            {
                string message = "CompositeService " + ((Component*)compositeService)->getName() + " not wired";
                throw ServiceNotFoundException(message.c_str());
            }

            // -------------------
            // Invoke each target
            // -------------------
            for (SERVICES::iterator iter = services.begin(); iter < services.end(); iter++)
            {                
                (*iter)->invoke(operation);
            }
            LOGEXIT(1,"CompositeServiceHelper::invoke");
        }


        // ======================================================================
        // getServiceWrapper: Create and return an instance of the ServiceWrapper
        // ======================================================================
        ServiceWrapper* CompositeServiceHelper::getServiceWrapper(Service* service)
        {    
//            // -------------------------
//            // Determine type of Service
//            // -------------------------
//            switch (target->getServiceType())
//            {
//            case WireTarget::CompositeReferenceType:
//                {
//                    // ----------------
//                    // External Service
//                    // ----------------
//                    return CompositeReferenceWrapper::getServiceWrapper((CompositeReference*)target);
//                }
//                
//            case WireTarget::ComponentServiceType:
//                {
//                    // -----------------
//                    // Component Service
//                    // -----------------
//                    return CPPServiceWrapper::getServiceWrapper((Service*)target);
//                }
//                
//            default:
//                {
//                    string message = "Undefined wire target type for : ";
//                    message = message + target->getName();
//                    throw ServiceNotFoundException(message.c_str());
//                }
//            } // end switch

            return NULL;
        }
        
    } // End namespace sca
} // End namespace tuscany
