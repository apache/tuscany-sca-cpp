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

#include "tuscany/sca/core/ComponentServiceWrapper.h"

#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/CPPImplementation.h"
using namespace osoa::sca;

namespace tuscany
{
    namespace sca
    {
        // ===================
        // Static data members
        // ===================
        void* ComponentServiceWrapper::staticImpl = 0;

        // ===========
        // Constructor
        // ===========
        ComponentServiceWrapper::ComponentServiceWrapper(Service* target)
            : ServiceWrapper(target), service(target)
        {
            LOGENTRY(1,"ComponentServiceWrapper::constructor");
            if (target->getServiceType() != WireTarget::ComponentServiceType)
            {
                throw ServiceRuntimeException("ComponentServiceWrapper: target is not a component service");
            }
            
            component = target->getComponent();
            interf = (CPPInterface*)target->getInterface();

            remotable = interf->getRemotable();

            
            LOGEXIT(1,"ComponentServiceWrapper::constructor");
            
        }
        
        // ==========
        // Destructor
        // ==========
        ComponentServiceWrapper::~ComponentServiceWrapper()
        {
            LOGENTRY(1,"ComponentServiceWrapper::destructor");
            LOGEXIT(1,"ComponentServiceWrapper::destructor");
        }
        
        
        // ======================================================================
        // getImplementation: get an implementation for this scope
        // ======================================================================
        void* ComponentServiceWrapper::getImplementation()
        {
            CPPInterface::SCOPE scope = interf->getScope();
            if (scope == CPPInterface::MODULE)
            {
                if (!staticImpl)
                {
                    staticImpl = newImplementation();
                }
                return staticImpl;
            }
            else // (scope == CPPInterface::STATELESS)
            {
                return newImplementation();
            }        
        }
        
        // ======================================================================
        // releaseImplementation: release the implementation for this scope
        // ======================================================================
        void ComponentServiceWrapper::releaseImplementation()
        {
            CPPInterface::SCOPE scope = interf->getScope();
            if (scope == CPPInterface::STATELESS)
            {
                deleteImplementation();
            }            
        }
        
        // ======================================================================
        // invoke: wrapper call to service with setting the component context
        // ======================================================================
        void ComponentServiceWrapper::invoke(Operation& operation)
        {
            LOGENTRY(1,"ComponentServiceWrapper::invoke");

            SCARuntime* runtime = SCARuntime::getInstance();
            runtime->setCurrentComponent(component);
            
            try
            {
                invokeService(operation);
            }
            catch (...)
            {
                runtime->unsetCurrentComponent();
                throw;
            }
            runtime->unsetCurrentComponent();
            LOGEXIT(1,"ComponentServiceWrapper::invoke");
            
        }
        
        void ComponentServiceWrapper::setLibrary(Library* lib)
        {
            wrapperLibrary = lib;
        }
        
        // ======================================================================
        // createServiceWrapper: create a wrapper for the target ComponentService
        // ======================================================================
        ComponentServiceWrapper* ComponentServiceWrapper::createServiceWrapper(Service* service)
        {            
            string msg;
            ComponentServiceWrapper* serviceWrapper = 0;
            
            // -----------------------------------------------
            // Get the implementation for the target component
            // -----------------------------------------------
            Component* targetComponent = service->getComponent();
            Implementation* impl = targetComponent->getImplementation();
            if (!impl)
            {
                msg = "Component " + targetComponent->getName() + " has no implementation defined";
                throw ServiceNotFoundException(msg.c_str());
            }
            
            // TODO: This only handle CPP implementation for now
            if (impl->getImplementationType() == Implementation::CPP)
            {
                // ----------------------------------------------------
                // Get implementation dll name and wrapper factory name
                // ----------------------------------------------------
                string dllName = ((CPPImplementation*)impl)->getDll();
                string wrapperFactoryName = ((CPPImplementation*)impl)->getHeaderStub() 
                    + "_" + service->getName() + "_Wrapper_Factory";
                
                // ------------
                // Load the dll
                // ------------
                string fullDllName = targetComponent->getModule()->getRoot() + "/" + dllName;
                typedef ComponentServiceWrapper* (* WRAPPERFACTORY) (Service*);                
                Library* wrapperLib = new Library(fullDllName);
                
                // -------------------------
                // Locate the factory method
                // -------------------------
                WRAPPERFACTORY wrapperFactory = (WRAPPERFACTORY)wrapperLib->getSymbol(wrapperFactoryName);
                if (!wrapperFactory)
                {
                    LOGERROR_2(1, "ComponentServiceWrapper::createServiceWrapper: Unable to locate %s in library %s",
                        wrapperFactoryName.c_str(), fullDllName.c_str());
                    msg = "Unable to locate " + wrapperFactoryName + " in library " + fullDllName;
                    throw ServiceNotFoundException(msg.c_str());
                }
                
                // -------------------------------------
                // Now create an instance of the wrapper
                // -------------------------------------                
                serviceWrapper = wrapperFactory(service);
                if (!serviceWrapper)
                {
                    LOGERROR_2(1, "ComponentServiceWrapper::createServiceWrapper: Factory method %s in library %s returned null",
                        wrapperFactoryName.c_str(), fullDllName.c_str());
                    msg = "Factory method " + wrapperFactoryName + " in library " + fullDllName + " returned null";
                    throw ServiceNotFoundException(msg.c_str());
                }                
                serviceWrapper->setLibrary(wrapperLib);
            }
            else
            {
                // ----------------------
                // Non CPP Implementation
                // ----------------------
                msg = "Unsupported implementation type for service: " + service->getName();
                throw ServiceNotFoundException(msg.c_str());
            }        
            
            return serviceWrapper;
        }    
        
    } // End namespace sca
} // End namespace tuscany
