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
#include "osoa/sca/ServiceRuntimeException.h"
#include "tuscany/sca/cpp/ComponentContextImpl.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ReferenceBinding.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/ServiceBinding.h"

namespace tuscany
{
    namespace sca
    {
       namespace cpp
        {

        // ===========
        // Constructor
        // ===========
        ComponentContextImpl::ComponentContextImpl(Component* comp)
            : ComponentContext(0),  component(comp)
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
            
            // --------------------------------------------------------------
            // locate reference in the current component and determine target
            // --------------------------------------------------------------
            Reference* reference = component->findReference(referenceName);
            if (!reference)
            {
                string message = "Reference not defined: ";
                message = message + referenceName;
                throw ServiceNotFoundException(message.c_str());
            }

            // Get a service proxy from the binding configured on the reference
            ServiceProxy* serviceProxy =  reference->getBinding()->getServiceProxy();
            if (serviceProxy == NULL)
            {
                string message = "Reference ";
                message = message + referenceName + " not wired";
                throw ServiceNotFoundException(message.c_str());
            }
            proxies.push_back(serviceProxy);
            
            ServiceProxy::PROXIES proxies = serviceProxy->getProxies();
            ServiceList services(proxies.size());
            for (ServiceProxy::PROXIES::const_iterator iter = proxies.begin();
            iter!=proxies.end();
            iter++)
            {
                services.addService(*iter);
            }
            
            return services;
            
        } // End getServices()
        
        
        // ===================================================================
        // getService: return a Proxy for the services wired to this reference
        // ===================================================================
        void* ComponentContextImpl::getService(const char* referenceName)
        {
            LOGENTRY(1, "ComponentContextImpl::getService");
            
            // --------------------------------------------------------------
            // locate reference in the current component and determine target
            // --------------------------------------------------------------
            Reference* reference = component->findReference(referenceName);
            if (!reference)
            {
                string message = "Reference not defined: ";
                message = message + referenceName;
                throw ServiceNotFoundException(message.c_str());
            }
            
            // --------------------
            // Validate the request
            // --------------------
            switch (reference->getType()->getMultiplicity())
            {
            case ReferenceType::ZERO_MANY:
            case ReferenceType::ONE_MANY:
                {
                    string message = "getService() called for reference with multiplicity >1 :";
                    message = message + referenceName;
                    throw ServiceNotFoundException(message.c_str());                    
                }
            default:
                {
                }
            } // end switch

            // Get a service proxy from the binding configured on the reference
            ServiceProxy* serviceProxy =  reference->getBinding()->getServiceProxy();
            if (serviceProxy == NULL)
            {
                string message = "Reference ";
                message = message + referenceName + " not wired";
                throw ServiceNotFoundException(message.c_str());
            }
            
            void* service = serviceProxy->getProxy();
            proxies.push_back(serviceProxy);
            return service;
            
        } // End getService()
        
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
        commonj::sdo::DataFactoryPtr ComponentContextImpl::getDataFactory()
        {
            LOGENTRY(1, "ComponentContextImpl::getProperties");
            commonj::sdo::DataFactoryPtr dataFactory = component->getComposite()->getDataFactory();
            
            LOGEXIT(1, "ComponentContextImpl::getDataFactory");
            return dataFactory;
        }
        
       } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
