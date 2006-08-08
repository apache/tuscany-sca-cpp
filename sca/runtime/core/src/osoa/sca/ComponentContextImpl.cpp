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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
#include "osoa/sca/ComponentContextImpl.h"
#include "tuscany/sca/model/WireTarget.h"
#include "tuscany/sca/core/ComponentServiceWrapper.h"
#include "tuscany/sca/core/ExternalServiceWrapper.h"
#include "tuscany/sca/model/CPPImplementation.h"
#include "tuscany/sca/model/Composite.h"

using namespace tuscany::sca;
using namespace tuscany::sca::model;

namespace osoa
{
    namespace sca
    {
        // ===========
        // Constructor
        // ===========
        ComponentContextImpl::ComponentContextImpl(Component* comp)
            : component(comp)
        {
            LOGENTRY(1, "ComponentContextImpl::constructor");
            LOGEXIT(1, "ComponentContextImpl::constructor");
        }
        
        // ==========
        // Destructor
        // ==========
        ComponentContextImpl::~ComponentContextImpl()
        {
            // --------------------------------------------
            // Delete the proxies served up by this context
            // --------------------------------------------
            for (PROXIES::iterator iter = proxies.begin(); iter != proxies.end(); iter++)
            {
                delete (ServiceProxy*)*iter;
            }
        }
        
        
        // ==========================================================================
        // getServices: return a list of Proxies for services wired to this reference
        // ==========================================================================
        ServiceList ComponentContextImpl::getServices(const char* referenceName)
        {
            LOGENTRY(1, "ComponentContextImpl::getServices");
            
            string message;

            // --------------------------------------------------------------
            // locate reference in the current component and determine target
            // --------------------------------------------------------------
            ServiceReference* serviceReference = component->findReference(referenceName);
            if (!serviceReference)
            {
                message = "Reference not defined: ";
                message = message + referenceName;
                throw ServiceNotFoundException(message.c_str());
            }

            // Get the target services from the ServiceReference
            const ServiceReference::TARGETS& targets = serviceReference->getTargets();
            
            // --------------------
            // Validate the request
            // --------------------
            switch (serviceReference->getMultiplicity())
            {
            case ServiceReference::ONE_MANY:
            case ServiceReference::ONE_ONE:
                {
                    if (targets.size() == 0)
                    {
                        message = "Reference ";
                        message = message + referenceName + " not wired";
                        throw ServiceNotFoundException(message.c_str());
                    }
                }
            default:
                {
                }
            } // end switch

            // ------------------------------
            // Create a proxy for each target
            // ------------------------------
            ServiceList services(targets.size());
            for (ServiceReference::TARGETS::const_iterator iter = targets.begin();
            iter!=targets.end();
            iter++)
            {
                services.addService(getServiceProxy(serviceReference, *iter));                
            }
            
            return services;
            
        } // End getServices()
        
        
        // ===================================================================
        // getService: return a Proxy for the services wired to this reference
        // ===================================================================
        void* ComponentContextImpl::getService(const char* referenceName)
        {
            LOGENTRY(1, "ComponentContextImpl::getService");
            
            string message;
            
            // --------------------------------------------------------------
            // locate reference in the current component and determine target
            // --------------------------------------------------------------
            ServiceReference* serviceReference = component->findReference(referenceName);
            if (!serviceReference)
            {
                message = "Reference not defined: ";
                message = message + referenceName;
                throw ServiceNotFoundException(message.c_str());
            }
            
            // Get the target service from the ServiceReference
            const ServiceReference::TARGETS& targets = serviceReference->getTargets();
            
            // --------------------
            // Validate the request
            // --------------------
            switch (serviceReference->getMultiplicity())
            {
            case ServiceReference::ZERO_MANY:
            case ServiceReference::ONE_MANY:
                {
                    message = "getService() called for reference with multiplicity >1 :";
                    message = message + referenceName;
                    throw ServiceNotFoundException(message.c_str());                    
                }
            case ServiceReference::ONE_ONE:
                {
                    if (targets.size() == 0)
                    {
                        message = "Reference ";
                        message = message + referenceName + " not wired";
                        throw ServiceNotFoundException(message.c_str());
                    }
                }
            default:
                {
                }
            } // end switch

            // Return the proxy
            return getServiceProxy(serviceReference, *targets.begin());
            
        } // End getService()
        
        
        // ==================================================================
        // getServiceProxy: Create and return an instance of the ServiceProxy
        // ==================================================================
        void* ComponentContextImpl::getServiceProxy(
            ServiceReference* serviceReference,
            WireTarget* target)
        {
            // -----------------------------------
            // Get a ServiceWrapper for the target
            // -----------------------------------
            ServiceWrapper* serviceWrapper = getServiceWrapper(target);
            
            // ------------------------------
            // Get a Proxy for this reference
            // ------------------------------
            try
            {
                ServiceProxy* serviceProxy =  new ServiceProxy(component, serviceReference->getName(), serviceWrapper);
                void* service = serviceProxy->getProxy();
                // service MUST be set here or an exception will have been thrown
                proxies.push_back(serviceProxy);
                return service;
            }
            catch (ServiceRuntimeException&)
            {
                delete serviceWrapper;
                throw ;
            }
        }
        
        
        // ======================================================================
        // getServiceWrapper: Create and return an instance of the ServiceWrapper
        // ======================================================================
        ServiceWrapper* ComponentContextImpl::getServiceWrapper(WireTarget* target)
        {    
            // -------------------------
            // Determine type of Service
            // -------------------------
            switch (target->getServiceType())
            {
            case WireTarget::ExternalServiceType:
                {
                    // ----------------
                    // External Service
                    // ----------------
                    return ExternalServiceWrapper::createServiceWrapper((ExternalService*)target);
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
        
        

        // ==============================================
        // getProperties: return the component properties
        // ==============================================
        DataObjectPtr ComponentContextImpl::getProperties()
        {
            LOGENTRY(1, "ComponentContextImpl::getProperties");
            DataObjectPtr properties = component->getProperties();
            
            LOGEXIT(1, "ComponentContextImpl::getProperties");
            return properties;
        }

        // ==============================================
        // getDataFactory: return the data factory for the composite in which
        // this component resides
        // ==============================================
        DataFactoryPtr ComponentContextImpl::getDataFactory()
        {
            LOGENTRY(1, "ComponentContextImpl::getProperties");
            DataFactoryPtr dataFactory = component->getComposite()->getDataFactory();
            
            LOGEXIT(1, "ComponentContextImpl::getDataFactory");
            return dataFactory;
        }
        
        
    } // End namespace sca
} // End namespace osoa
