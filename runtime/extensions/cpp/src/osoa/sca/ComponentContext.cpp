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

#include "commonj/sdo/SDO.h"

#include "osoa/sca/ComponentContext.h"
#include "osoa/sca/ServiceRuntimeException.h"

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/cpp/ComponentContextImpl.h"
#include "tuscany/sca/core/SCARuntime.h"

using namespace commonj::sdo;
using namespace tuscany::sca;
using namespace tuscany::sca::cpp;
using namespace tuscany::sca::model;

namespace osoa
{
    namespace sca
    {
        
        // =======================================================
        // getCurrent: create a context from the current component
        // =======================================================
        ComponentContext ComponentContext::getCurrent()
        {
            logentry();
            try
            {
                Component* component = tuscany::sca::SCARuntime::getInstance()->getCurrentComponent();
                if (!component)
                {
                    throwException(ComponentContextException, "No current component");
                }
                ComponentContext* cci = new ComponentContextImpl(component);
                return ComponentContext(cci);
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
        
        // ===========
        // Constructor
        // ===========    
        ComponentContext::ComponentContext(ComponentContext* implementation)
            : impl(implementation)
        {
            logentry();
        }
        
        // ==========
        // Destructor
        // ==========
        ComponentContext::~ComponentContext()
        {
            logentry();
            delete impl;
        }

        // ===================================
        // Copy constructor: create a new impl
        // ===================================
        ComponentContext::ComponentContext(const ComponentContext& ctx)
        {
            logentry();
            Component* component = ((ComponentContextImpl*)impl)->getComponent();
            impl = new ComponentContextImpl(component);
        }
        
        // =============================
        // operator= : create a new impl
        // =============================
        ComponentContext& ComponentContext::operator=(const ComponentContext& ctx)
        {
            logentry();
            if (this != &ctx)
            {
                Component* component = ((ComponentContextImpl*)impl)->getComponent();
                impl = new ComponentContextImpl(component);
            }
            return *this;
        }

        // ==========
        // getService
        // ==========
        void* ComponentContext::getService(const std::string& referenceName)
        {
            logentry();
            void* service = impl->getService(referenceName);
            return service;
        }
        
        // ===========
        // getServices
        // ===========
        std::list<void*> ComponentContext::getServices(const std::string& referenceName)
        {
            logentry();
            return impl->getServices(referenceName);
        }
        
        // ============
        // getProperties
        // =============
        DataObjectPtr ComponentContext::getProperties()
        {
            logentry();
            DataObjectPtr properties = impl->getProperties();
            return properties;
        }

        // ============
        // getDataFactory
        // =============
        DataFactoryPtr ComponentContext::getDataFactory()
        {
            logentry();
            DataFactoryPtr dataFactory = impl->getDataFactory();
            return dataFactory;
        }

        
    } // End namespace sca
} // End namespace osoa
