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

#include "tuscany/sca/core/CompositeService.h"

#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/core/ComponentServiceWrapper.h"
#include "tuscany/sca/core/CompositeReferenceWrapper.h"
using namespace osoa::sca;

namespace tuscany
{
    namespace sca
    {
        // ===========
        // Constructor
        // ===========
        CompositeService::CompositeService(const char* epName)
            : subsystem(0), compositeService(0), composite(0)
        {
            LOGENTRY(1,"CompositeService::constructor");
            string message;
    
            // initialize the system if necessary
            System* system = SCARuntime::getInstance()->getSystem();

            string subsystemName;
            string compositeServiceName;

            Utils::tokeniseUri(epName, subsystemName, compositeServiceName);

            // --------------------
            // locate the Subsystem
            // --------------------
            subsystem = system->findSubsystem(subsystemName);
            if (!subsystem)
            {
                throw CompositeServiceTypeNotFoundException(epName);
            }

            // ---------------------
            // locate the CompositeServiceType
            // ---------------------
            compositeService =  subsystem->findCompositeServiceType(compositeServiceName);
            if (compositeService)
            {
                // found Subsystem CompositeServiceType
                throw CompositeServiceTypeNotFoundException("Subsystem CompositeServiceType not yet supported");
            }
            else
            {
                // -----------------------------------
                // locate the CompositeServiceType in the Composite
                // -----------------------------------
                string compositeComponentName;
                Utils::tokeniseUri(compositeServiceName, compositeComponentName, compositeServiceName);
                composite = subsystem->findCompositeByComponentName(compositeComponentName);
                if (composite)
                {
                    dataFactory = composite->getDataFactory();
                    compositeService = composite->findCompositeServiceType(compositeServiceName);
                }
            }
    
            if (!compositeService)
            {
                throw CompositeServiceTypeNotFoundException(epName);
            }

            // ---------------------------------
            // Get Wrappers for each wire target
            // ---------------------------------

            // Get the target services from the CompositeServiceType
            const CompositeServiceType::TARGETS& targets = compositeService->getTargets();
            
            // --------------------
            // Validate the request
            // --------------------
            switch (compositeService->getMultiplicity())
            {
            case CompositeServiceType::ONE_MANY:
            case CompositeServiceType::ONE_ONE:
                {
                    if (targets.size() == 0)
                    {
                        message = "CompositeServiceType " + compositeService->getName() + " not wired";
                        throw ServiceNotFoundException(message.c_str());
                    }
                }
            default:
                {
                }
            } // end switch

            for (CompositeServiceType::TARGETS::const_iterator iter = targets.begin();
            iter!=targets.end();
            iter++)
            {
                ServiceWrapper* service = getServiceWrapper(*iter);
                if (service)
                {
                    services.push_back(service);
                }
            }
        
            LOGEXIT(1,"CompositeService::constructor");
        }
        
        // ==========
        // Destructor
        // ==========
        CompositeService::~CompositeService()
        {
            LOGENTRY(1,"CompositeService::destructor");

            // Delete any ServiceWrappers
            for (SERVICES::iterator iter = services.begin(); iter < services.end(); iter++)
            {                
                delete (*iter);
            }
            LOGEXIT(1,"CompositeService::destructor");
        }
        

        // ======================================================================
        // invoke: 
        // ======================================================================
        SCA_API void CompositeService::invoke(Operation& operation)
        {
            LOGENTRY(1,"CompositeService::invoke");
            string message;

            // --------------------
            // Validate the request
            // --------------------
            if (services.size() == 0)
            {
                message = "CompositeServiceType " + compositeService->getName() + " not wired";
                throw ServiceNotFoundException(message.c_str());
            }

            // -------------------
            // Invoke each target
            // -------------------
            for (SERVICES::iterator iter = services.begin(); iter < services.end(); iter++)
            {                
                (*iter)->invoke(operation);
            }
            LOGEXIT(1,"CompositeService::invoke");
        }


        // ======================================================================
        // getServiceWrapper: Create and return an instance of the ServiceWrapper
        // ======================================================================
        ServiceWrapper* CompositeService::getServiceWrapper(WireTarget* target)
        {    
            // -------------------------
            // Determine type of Service
            // -------------------------
            switch (target->getServiceType())
            {
            case WireTarget::CompositeReferenceTypeType:
                {
                    // ----------------
                    // External Service
                    // ----------------
                    return CompositeReferenceWrapper::createServiceWrapper((CompositeReferenceType*)target);
                }
                
            case WireTarget::ComponentServiceType:
                {
                    // -----------------
                    // Component Service
                    // -----------------
                    return ComponentServiceWrapper::createServiceWrapper((Service*)target);
                }
                
            default:
                {
                    string message = "Undefined wire target type for : ";
                    message = message + target->getName();
                    throw ServiceNotFoundException(message.c_str());
                }
            } // end switch
        }
        
        
    } // End namespace sca
} // End namespace tuscany
