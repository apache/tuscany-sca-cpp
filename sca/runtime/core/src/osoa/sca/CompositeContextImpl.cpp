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
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/CPPImplementation.h"
#include "osoa/sca/CompositeContextImpl.h"
#include "tuscany/sca/core/ComponentServiceWrapper.h"

using namespace tuscany::sca;
using namespace tuscany::sca::model;

namespace osoa
{
    namespace sca
    {
        // ===========
        // Constructor 
        // ===========
        CompositeContextImpl::CompositeContextImpl(Composite* mod)
            : composite(mod)
        {
        }

        // ==========
        // Destructor
        // ==========
        CompositeContextImpl::~CompositeContextImpl()
        {
            // --------------------------------------------
            // Delete the proxies served up by this context
            // --------------------------------------------
            for (PROXIES::iterator iter = proxies.begin(); iter != proxies.end(); iter++)
            {
                delete (ServiceProxy*)*iter;
            }
        }

        // ===========================================================================
        // locateService: return a proxy connected to a wrapper for the target service
        // ===========================================================================
        void* CompositeContextImpl::locateService(const char* serviceName)
        {
            LOGENTRY(1, "CompositeContextImpl::locateService");
                

            // ----------------------------
            // Locate the component service
            // ----------------------------
            Service* service = composite->findComponentService(serviceName);
            string msg;
            if (!service)
            {
                msg = "Service not found: ";
                msg = msg + serviceName;
                throw ServiceNotFoundException(msg.c_str());
            }

            // -------------------------
            // Create the ServiceWrapper
            // -------------------------
            ComponentServiceWrapper* serviceWrapper = ComponentServiceWrapper::createServiceWrapper(service);
            
            // ----------------------------
            // Get a Proxy for this service
            // ----------------------------
            try
            {
                ServiceProxy* serviceProxy =  new ServiceProxy(service->getComponent(), service->getName(), serviceWrapper);
                proxies.push_back(serviceProxy);
                LOGEXIT(1, "CompositeContextImpl::locateService");
                return serviceProxy->getProxy();
            }
            catch (ServiceRuntimeException&)
            {
                delete serviceWrapper;
                throw ;
            }

        }
        
        // ==============================================
        // getDataFactory: return the data factory for the current composite
        // ==============================================
        DataFactoryPtr CompositeContextImpl::getDataFactory()
        {
            LOGENTRY(1, "CompositeContextImpl::getDataFactory");
            DataFactoryPtr dataFactory = composite->getDataFactory();
            
            LOGEXIT(1, "CompositeContextImpl::getDataFactory");
            return dataFactory;
        }
        
        
        
    } // End namespace sca
} // End namespace osoa
