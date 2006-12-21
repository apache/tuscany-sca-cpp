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

#include "tuscany/sca/cpp/CPPServiceWrapper.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Library.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/cpp/model/CPPImplementation.h"
#include "tuscany/sca/model/Interface.h"

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
            
            // ===================
            // Static data members
            // ===================
            void* CPPServiceWrapper::staticImpl = 0;
    
            // ===========
            // Constructor
            // ===========
            CPPServiceWrapper::CPPServiceWrapper(Service* service)
                : ServiceWrapper(service)
            {
                logentry();
    
                component = service->getComponent();
                interf = service->getType()->getInterface();
                implementation = (CPPImplementation*)component->getType();
            }
            
            // ==========
            // Destructor
            // ==========
            CPPServiceWrapper::~CPPServiceWrapper()
            {
                logentry();
            }
            
            
            // ======================================================================
            // getImplementation: get an implementation for this scope
            // ======================================================================
            void* CPPServiceWrapper::getImplementation()
            {
                logentry();
                if (implementation->getScope() == CPPImplementation::COMPOSITE)
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
            void CPPServiceWrapper::releaseImplementation()
            {
                logentry();
                if (implementation->getScope() == CPPImplementation::STATELESS)
                {
                    deleteImplementation();
                }            
            }
            
            // ======================================================================
            // invoke: wrapper call to service with setting the component context
            // ======================================================================
            void CPPServiceWrapper::invoke(Operation& operation)
            {
                logentry();
    
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
            }
            
            void CPPServiceWrapper::setLibrary(Library* lib)
            {
                wrapperLibrary = lib;
            }
            
            // ======================================================================
            // getServiceWrapper: create a wrapper for the target ComponentService
            // ======================================================================
            CPPServiceWrapper* CPPServiceWrapper::getServiceWrapper(Service* service)
            {            
                logentry();

                CPPServiceWrapper* serviceWrapper = 0;
                
                // -----------------------------------------------
                // Get the implementation for the target component
                // -----------------------------------------------
                Component* component = service->getComponent();
                CPPImplementation* impl = (CPPImplementation*)component->getType();
                if (!impl)
                {
                    string msg = "Component " + component->getName() + " has no implementation defined";
                    throwException(ServiceNotFoundException, msg.c_str());
                }
                
                // ----------------------------------------------------
                // Get implementation dll name and wrapper factory name
                // ----------------------------------------------------
                string libraryName = impl->getLibrary();
                string wrapperFactoryName = impl->getHeaderStub() 
                    + "_" + service->getType()->getName() + "_Wrapper_Factory";
                
                // ------------
                // Load the dll
                // ------------
                string fullLibraryName = component->getComposite()->getRoot() + "/" + libraryName;
                typedef CPPServiceWrapper* (* WRAPPERFACTORY) (Service*);                
                Library* wrapperLib = new Library(fullLibraryName);
                
                // -------------------------
                // Locate the factory method
                // -------------------------
                WRAPPERFACTORY wrapperFactory = (WRAPPERFACTORY)wrapperLib->getSymbol(wrapperFactoryName);
                if (!wrapperFactory)
                {
                    logerror("Unable to locate %s in library %s",
                        wrapperFactoryName.c_str(), fullLibraryName.c_str());
                    string msg = "Unable to locate " + wrapperFactoryName + " in library " + fullLibraryName;
                    throwException(ServiceNotFoundException, msg.c_str());
                }
                
                // -------------------------------------
                // Now create an instance of the wrapper
                // -------------------------------------                
                serviceWrapper = wrapperFactory(service);
                if (!serviceWrapper)
                {
                    logerror("Factory method %s in library %s returned null",
                        wrapperFactoryName.c_str(), fullLibraryName.c_str());
                    string msg = "Factory method " + wrapperFactoryName + " in library " + fullLibraryName + " returned null";
                    throwException(ServiceNotFoundException, msg.c_str());
                }                
                serviceWrapper->setLibrary(wrapperLib);
                
                return serviceWrapper;
            }    

        } // End namespace cpp        
    } // End namespace sca
} // End namespace tuscany
