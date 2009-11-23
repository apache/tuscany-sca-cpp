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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/cpp/ComponentContextImpl.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ReferenceBinding.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/ServiceBinding.h"

using namespace std;
using namespace osoa::sca;
using namespace commonj::sdo;
using namespace tuscany::sca::model;

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
            logentry();
        }
        
        // ==========
        // Destructor
        // ==========
        ComponentContextImpl::~ComponentContextImpl()
        {
            logentry();
        }
        
        
        // ==========================================================================
        // getServices: return a list of Proxies for services wired to this reference
        // ==========================================================================
        std::list<void*> ComponentContextImpl::getServices(const std::string& referenceName)
        {
            logentry();
            try
            {
                // --------------------------------------------------------------
                // locate reference in the current component and determine target
                // --------------------------------------------------------------
                Reference* reference = component->findReference(referenceName);
                if (!reference)
                {
                    string message = "Reference not defined: ";
                    message = message + referenceName;
                    throwException(ServiceNotFoundException, message.c_str());
                }
    
                // Get a service proxy from the binding configured on the reference
                CPPServiceProxy* serviceProxy =  (CPPServiceProxy*)reference->getBinding()->getServiceProxy();
                if (serviceProxy == NULL)
                {
                    string message = "Reference ";
                    message = message + referenceName + " not wired";
                    throwException(ServiceNotFoundException, message.c_str());
                }
                
                return serviceProxy->getProxies();
            }
            catch (ServiceRuntimeException&)
            {
                throw;
            }
            catch (TuscanyRuntimeException& e)
            {
                throwException(ServiceRuntimeException, e);
            }
            
        } // End getServices()
        
        
        // ===================================================================
        // getService: return a Proxy for the services wired to this reference
        // ===================================================================
        void* ComponentContextImpl::getService(const std::string& referenceName)
        {
            logentry();
            try
            {            
                // --------------------------------------------------------------
                // locate reference in the current component and determine target
                // --------------------------------------------------------------
                Reference* reference = component->findReference(referenceName);
                if (!reference)
                {
                    string message = "Reference not defined: ";
                    message = message + referenceName;
                    throwException(ServiceNotFoundException, message.c_str());
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
                        throwException(ServiceNotFoundException, message.c_str());
                    }
                default:
                    {
                    }
                } // end switch
    
                // Get a service proxy from the binding configured on the reference
                CPPServiceProxy* serviceProxy =  (CPPServiceProxy*)reference->getBinding()->getServiceProxy();
                if (serviceProxy == NULL)
                {
                    string message = "Reference ";
                    message = message + referenceName + " not wired";
                    throwException(ServiceNotFoundException, message.c_str());
                }
                
                void* service = serviceProxy->getProxy();
                
                return service;
            }
            catch (ServiceRuntimeException&)
            {
                throw;
            }
            catch (TuscanyRuntimeException& e)
            {
                throwException(ServiceRuntimeException, e);
            }
            
        } // End getService()
        
        // ==============================================
        // getProperties: return the component properties
        // ==============================================
        DataObjectPtr ComponentContextImpl::getProperties()
        {
            logentry();
            try
            {
                DataObjectPtr properties = component->getProperties();
                return properties;
            }
            catch (ServiceRuntimeException&)
            {
                throw;
            }
            catch (TuscanyRuntimeException& e)
            {
                throwException(ServiceRuntimeException, e);
            }
        }

        // ==============================================
        // getDataFactory: return the data factory for the composite in which
        // this component resides
        // ==============================================
        commonj::sdo::DataFactoryPtr ComponentContextImpl::getDataFactory()
        {
            logentry();
            try
            {
                commonj::sdo::DataFactoryPtr dataFactory = component->getComposite()->getDataFactory();
                return dataFactory;
            }
            catch (ServiceRuntimeException&)
            {
                throw;
            }
            catch (TuscanyRuntimeException& e)
            {
                throwException(ServiceRuntimeException, e);
            }
        }
        
       } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
