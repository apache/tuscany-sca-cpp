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

#include "osoa/sca/ServiceRuntimeException.h"

#include "tuscany/sca/cpp/CPPServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
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

using namespace std;
using namespace osoa::sca;
using namespace tuscany::sca::model;
using namespace tuscany::sca::util;

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
                logentry();
    
                // ----------------------
                // Get the component
                // ----------------------
                Component* component = reference->getComponent();
                string name = reference->getType()->getName();
                
                // Get the service wrapper
                CPPReferenceBinding* referenceBinding = (CPPReferenceBinding*)reference->getBinding();
                
                ServiceWrapper* serviceWrapper = referenceBinding->getTargetServiceBinding()->getServiceWrapper();
    
                createProxy(component, name, serviceWrapper);
            }
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            CPPServiceProxy::CPPServiceProxy(Service* service)
                : ServiceProxy(0)
            {
                logentry();
                
                // ----------------------
                // Get the component
                // ----------------------
                Component* component = service->getComponent();
                string name = service->getType()->getName();
                
                // Get the service wrapper
                ServiceWrapper* serviceWrapper = service->getBinding()->getServiceWrapper();
    
                createProxy(component, name, serviceWrapper);
            }
            
            void CPPServiceProxy::createProxy(Component* component, const string& name, ServiceWrapper* serviceWrapper)
            {
                logentry();
                
                ComponentType* componentType = component->getType();
                if (!componentType)
                {
                    string msg = "Component " + component->getName() + " has no implementation defined";
                    throwException(ServiceNotFoundException, msg.c_str());
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
                    logerror("Unable to locate %s in library %s",
                        proxyFactoryName.c_str(), fullLibraryName.c_str());
                    string msg = "Unable to locate " + proxyFactoryName + " in library " + fullLibraryName;
                    throwException(ServiceNotFoundException, msg.c_str());
                }
                
                // -----------------------------------
                // Now create an instance of the proxy                
                // -----------------------------------
                void* proxy = proxyFactory(serviceWrapper); 
                if (!proxy)
                {
                    logerror("Factory method %s in library %s returned null",
                        proxyFactoryName.c_str(), fullLibraryName.c_str());
                    string msg = "Factory method " + proxyFactoryName + " in library " + fullLibraryName + " returned null";
                    throwException(ServiceNotFoundException, msg.c_str());
                }
                else
                {
                    proxies.push_back(proxy);
                }
    
                // -------------------------
                // Get the destructor method
                // -------------------------
                destructor  = (PROXYDESTRUCTOR)proxyLibrary.getSymbol(proxyDestructorName);        
            }
    
            // ==========
            // Destructor
            // ==========
            CPPServiceProxy::~CPPServiceProxy()
            {
                logentry();
    
                // Delete the proxies
                if (destructor != NULL && proxies.size() != 0)
                {
                    destructor(*proxies.begin());
                }
            }
            
            CPPServiceProxy::PROXIES CPPServiceProxy::getProxies()
            {
                return proxies;
            }
            
            void* CPPServiceProxy::getProxy()
            {
                logentry();

                if (proxies.size() != 0)
                {
                    return *proxies.begin();
                }
                else
                {
                    return NULL;
                }
            }
    
        } // End namespace cpp        
    } // End namespace sca
} // End namespace tuscany
