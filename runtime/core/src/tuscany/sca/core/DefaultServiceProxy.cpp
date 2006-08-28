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

#include "tuscany/sca/core/DefaultServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
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

using namespace osoa::sca;

//TODO Remove this hack, we keep a definition of CPPImplementation
// here to be able to get the CPP implementation info from a CPP
// implementation and create a corresponding proxy
// To remove this hack we just need to add a getServiceProxy method
// to ServiceBinding...
namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            class CPPImplementation : public ComponentType
            {
                
            public:
                CPPImplementation(const string& library, const string& header,
                    const string&headerPath, const string& headerStub, const string& className);
                virtual ~CPPImplementation();
                virtual void initializeComponent(Component* component);
                const string& getLibrary() const { return library; }
                const string& getHeader() const { return header; }
                const string& getHeaderStub() const { return headerStub; }
                const string& getHeaderPath() const { return headerPath; }
                const string& getClass() const { return className; }
                
            private:
                string library;
                string header;
                string headerStub;
                string headerPath;
                string className;
            };
            
            CPPImplementation::CPPImplementation(const string& library, const string& header,
                    const string&headerPath, const string& headerStub, const string& className)
                : ComponentType("")
            {
            }

            CPPImplementation::~CPPImplementation()
            {
            }
            
            void CPPImplementation::initializeComponent(Component* component)
            {
            }
            
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany


namespace tuscany
{
    namespace sca
    {
        using tuscany::sca::cpp::CPPImplementation;
        
        // ============================
        // Constructor: Create a proxy
        // ============================
        DefaultServiceProxy::DefaultServiceProxy(Service* service)
            : ServiceProxy(0)
        {
            LOGENTRY(1,"DefaultServiceProxy::constructor");
            
            // ----------------------
            // Get the component
            // ----------------------
            Component* component = service->getComponent();
            string name = service->getType()->getName();
            
            // Get the service wrapper
            ServiceWrapper* serviceWrapper = service->getBinding()->getServiceWrapper();

            createProxy(component, name, serviceWrapper);

            LOGEXIT(1,"DefaultServiceProxy::constructor");
        }
        
        void DefaultServiceProxy::createProxy(Component* component, const string& name, ServiceWrapper* serviceWrapper)
        {
            LOGENTRY(1,"DefaultServiceProxy::createProxy");
            
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
            string libraryName = impl->getLibrary();
            string headerStub = impl->getHeaderStub();

            string fullLibraryName = component->getComposite()->getRoot() + "/" + libraryName;
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
                LOGERROR_2(1, "DefaultServiceProxy::createProxy: Unable to locate %s in library %s",
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
                LOGERROR_2(1, "DefaultServiceProxy::createProxy: Factory method %s in library %s returned null",
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
            
            LOGEXIT(1,"DefaultServiceProxy::createProxy");
        }

        // ==========
        // Destructor
        // ==========
        DefaultServiceProxy::~DefaultServiceProxy()
        {
            LOGENTRY(1,"DefaultServiceProxy::destructor");

            // Delete the proxies
            if (destructor != NULL && proxies.size() != 0)
            {
                destructor(proxies[0]);
            }
            LOGEXIT(1,"DefaultServiceProxy::destructor");
        }
        
        ServiceProxy::PROXIES DefaultServiceProxy::getProxies()
        {
            return proxies;
        }
        
        void* DefaultServiceProxy::getProxy()
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
    
    } // End namespace sca
} // End namespace tuscany

