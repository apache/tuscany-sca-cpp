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
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/core/CompositeContextImpl.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/core/ServiceProxy.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/core/DefaultServiceProxy.h"
#include "commonj/sdo/SDO.h"

using namespace tuscany::sca::model;
using namespace osoa::sca;
using namespace commonj::sdo;

namespace tuscany
{
    namespace sca
    {
        // ===========
        // Constructor 
        // ===========
        CompositeContextImpl::CompositeContextImpl(Component* component)
            : CompositeContext(0), component(component),  composite((Composite*)component->getType())
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

            // ----------------------------
            // Get a Proxy for this service
            // ----------------------------

            // The locate service API is used from CPP clients so we are using
            // our default service proxy here
            //TODO is that right?
            ServiceProxy* serviceProxy =  new DefaultServiceProxy(service);
            proxies.push_back(serviceProxy);
            LOGEXIT(1, "CompositeContextImpl::locateService");
            return serviceProxy->getProxy();

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
} // End namespace tuscany
