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

#include "tuscany/sca/core/ServiceProxy.h"

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/CPPImplementation.h"

using namespace osoa::sca;

namespace tuscany
{
    namespace sca
    {
        // ============================
        // Constructor: Create a proxy
        // ============================
        ServiceProxy::ServiceProxy(Component* component, const string& name, ServiceWrapper* target)
        {
            LOGENTRY(1,"ServiceProxy::constructor");
            string msg;

            // ----------------------
            // Get the implementation
            // ----------------------
            Implementation* impl = component->getImplementation();
            if (!impl)
            {
                msg = "Component " + component->getName() + " has no implementation defined";
                throw ServiceNotFoundException(msg.c_str());
            }
            
            // TODO: This only handle CPP implementation for now
            if (impl->getImplementationType() == "cpp")
            {
                // ----------------------------------------------------
                // Get implementation dll name and service factory name
                // ----------------------------------------------------
                string dllName = ((CPPImplementation*)impl)->getLibrary();
                string headerStub = ((CPPImplementation*)impl)->getHeaderStub();

                string fullDllName = component->getComposite()->getRoot() + "/" + dllName;
                string proxyFactoryName = headerStub + "_" + name + "_Proxy_Factory";
                string proxyDestructorName = headerStub + "_" + name + "_Proxy_Destructor";        
                typedef void* (* PROXYFACTORY) (ServiceWrapper*);
                
                // ------------
                // Load the dll
                // ------------
                proxyLibrary = Library(fullDllName);

                // -------------------------
                // Locate the factory method
                // -------------------------
                PROXYFACTORY proxyFactory = (PROXYFACTORY)proxyLibrary.getSymbol(proxyFactoryName);        
                if (!proxyFactory)
                {
                    LOGERROR_2(1, "ComponentContextImpl::getServiceProxy: Unable to locate %s in library %s",
                        proxyFactoryName.c_str(), fullDllName.c_str());
                    msg = "Unable to locate " + proxyFactoryName + " in library " + fullDllName;
                    throw ServiceNotFoundException(msg.c_str());
                }
                
                // -----------------------------------
                // Now create an instance of the proxy                
                // -----------------------------------
                proxy = proxyFactory(target); 
                if (!proxy)
                {
                    LOGERROR_2(1, "ComponentContextImpl::getServiceProxy: Factory method %s in library %s returned null",
                        proxyFactoryName.c_str(), fullDllName.c_str());
                    msg = "Factory method " + proxyFactoryName + " in library " + fullDllName + " returned null";
                    throw ServiceNotFoundException(msg.c_str());
                }

                // -------------------------
                // Get the destructor method
                // -------------------------
                destructor  = (PROXYDESTRUCTOR)proxyLibrary.getSymbol(proxyDestructorName);        
            }
            
            LOGEXIT(1,"ServiceProxy::constructor");
            
        }

        // ==========
        // Destructor
        // ==========
        ServiceProxy::~ServiceProxy()
        {
            LOGENTRY(1,"ServiceProxy::destructor");

            // Delete the proxy
            if (destructor)
            {
                destructor(proxy);
            }
            LOGEXIT(1,"ServiceProxy::destructor");
        }
    
        
    } // End namespace sca
} // End namespace tuscany
