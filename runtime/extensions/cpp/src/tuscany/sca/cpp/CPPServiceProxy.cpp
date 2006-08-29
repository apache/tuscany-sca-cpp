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

#include "tuscany/sca/cpp/CPPServiceProxy.h"
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
#include "tuscany/sca/cpp/model/CPPImplementation.h"
#include "tuscany/sca/cpp/model/CPPReferenceBinding.h"

using namespace osoa::sca;

namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            CPPServiceProxy::CPPServiceProxy(Reference* reference)
                : ServiceProxy(reference)
            {
                LOGENTRY(1,"CPPServiceProxy::constructor");
    
                // ----------------------
                // Get the component
                // ----------------------
                Component* component = reference->getComponent();
                string name = reference->getType()->getName();
                
                // Get the service wrapper
                CPPReferenceBinding* referenceBinding = (CPPReferenceBinding*)reference->getBinding();
                
                ServiceWrapper* serviceWrapper = referenceBinding->getTargetServiceBinding()->getServiceWrapper();
    
                createProxy(component, name, serviceWrapper);
    
                LOGEXIT(1,"CPPServiceProxy::constructor");
            }
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            CPPServiceProxy::CPPServiceProxy(Service* service)
                : ServiceProxy(0)
            {
                LOGENTRY(1,"CPPServiceProxy::constructor");
                
                // ----------------------
                // Get the component
                // ----------------------
                Component* component = service->getComponent();
                string name = service->getType()->getName();
                
                // Get the service wrapper
                ServiceWrapper* serviceWrapper = service->getBinding()->getServiceWrapper();
    
                createProxy(component, name, serviceWrapper);
    
                LOGEXIT(1,"CPPServiceProxy::constructor");
            }
            
            void CPPServiceProxy::createProxy(Component* component, const string& name, ServiceWrapper* serviceWrapper)
            {
                LOGENTRY(1,"CPPServiceProxy::createProxy");
                
                ComponentType* componentType = component->getType();
                if (!componentType)
                {
                    string msg = "Component " + component->getName() + " has no implementation defined";
                    throw ServiceNotFoundException(msg.c_str());
                }
                
                // If we got here we have a CPP implementation
                CPPImplementation* impl = (CPPImplementation *)componentType;
                
                // ----------------------------------------------------
                // Get implementation dll name and service factory name
                // ----------------------------------------------------
                string library = impl->getLibrary();
                string headerStub = impl->getHeaderStub();
    
                string fullLibraryName = component->getComposite()->getRoot() + "/" + library;
                string proxyFactoryName = headerStub + "_" + name + "_Proxy_Factory";
                string proxyDestructorName = headerStub + "_" + name + "_Proxy_Destructor";        
                typedef void* (* PROXYFACTORY) (ServiceWrapper*);
                
                // ------------
                // Load the dll
                // ------------
                proxyLibrary = Library(fullLibraryName);
    
                // -------------------------
                // Locate the factory method
                // -------------------------
                PROXYFACTORY proxyFactory = (PROXYFACTORY)proxyLibrary.getSymbol(proxyFactoryName);        
                if (!proxyFactory)
                {
                    LOGERROR_2(1, "CPPServiceProxy::createProxy: Unable to locate %s in library %s",
                        proxyFactoryName.c_str(), fullLibraryName.c_str());
                    string msg = "Unable to locate " + proxyFactoryName + " in library " + fullLibraryName;
                    throw ServiceNotFoundException(msg.c_str());
                }
                
                // -----------------------------------
                // Now create an instance of the proxy                
                // -----------------------------------
                void* proxy = proxyFactory(serviceWrapper); 
                if (!proxy)
                {
                    LOGERROR_2(1, "CPPServiceProxy::createProxy: Factory method %s in library %s returned null",
                        proxyFactoryName.c_str(), fullLibraryName.c_str());
                    string msg = "Factory method " + proxyFactoryName + " in library " + fullLibraryName + " returned null";
                    throw ServiceNotFoundException(msg.c_str());
                }
                else
                {
                    proxies.push_back(proxy);
                }
    
                // -------------------------
                // Get the destructor method
                // -------------------------
                destructor  = (PROXYDESTRUCTOR)proxyLibrary.getSymbol(proxyDestructorName);        
                
                LOGEXIT(1,"CPPServiceProxy::createProxy");
            }
    
            // ==========
            // Destructor
            // ==========
            CPPServiceProxy::~CPPServiceProxy()
            {
                LOGENTRY(1,"CPPServiceProxy::destructor");
    
                // Delete the proxies
                if (destructor != NULL && proxies.size() != 0)
                {
                    destructor(proxies[0]);
                }
                LOGEXIT(1,"CPPServiceProxy::destructor");
            }
            
            ServiceProxy::PROXIES CPPServiceProxy::getProxies()
            {
                return proxies;
            }
            
            void* CPPServiceProxy::getProxy()
            {
                if (proxies.size() != 0)
                {
                    return proxies[0];
                }
                else
                {
                    return NULL;
                }
            }
    
        } // End namespace cpp        
    } // End namespace sca
} // End namespace tuscany
