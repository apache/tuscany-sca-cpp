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

#include "osoa/sca/ComponentContext.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
#include "osoa/sca/ComponentContextImpl.h"
#include "tuscany/sca/core/SCARuntime.h"

using namespace tuscany::sca;

namespace osoa
{
    namespace sca
    {
        
        // =======================================================
        // getCurrent: create a context from the current component
        // =======================================================
        ComponentContext ComponentContext::getCurrent()
        {
            LOGENTRY(1, "ComponentContext::getCurrent");
            Component* component = SCARuntime::getInstance()->getCurrentComponent();
            if (!component)
            {
                throw ComponentContextException("No current component");
            }
            ComponentContextImpl* cci = new ComponentContextImpl(component);
            LOGEXIT(1, "ComponentContext::constructor");
            return ComponentContext(cci);
        }
        
        // ===========
        // Constructor
        // ===========    
        ComponentContext::ComponentContext(ComponentContextImpl* implementation)
            : impl(implementation)
        {
            LOGENTRY(1, "ComponentContext::constructor");
            LOGEXIT(1, "ComponentContext::constructor");
        }
        
        // ==========
        // Destructor
        // ==========
        ComponentContext::~ComponentContext()
        {
            LOGENTRY(1, "ComponentContext::destructor");
            delete impl;
            LOGEXIT(1, "ComponentContext::destructor");
        }

        // ===================================
        // Copy constructor: create a new impl
        // ===================================
        ComponentContext::ComponentContext(const ComponentContext& ctx)
        {
            impl = new ComponentContextImpl(impl->getComponent());
        }
        
        // =============================
        // operator= : create a new impl
        // =============================
        ComponentContext& ComponentContext::operator=(const ComponentContext& ctx)
        {
            if (this != &ctx)
            {
                impl = new ComponentContextImpl(impl->getComponent());
            }
            return *this;
        }

        // ==========
        // getService
        // ==========
        void* ComponentContext::getService(const char* referenceName)
        {
            LOGENTRY(1, "ComponentContext::getService");
            void* service = impl->getService(referenceName);
            LOGEXIT(1, "ComponentContext::getService");
            return service;
        }
        
        // ===========
        // getServices
        // ===========
        ServiceList ComponentContext::getServices(const char* referenceName)
        {
            return impl->getServices(referenceName);
        }
        
        // ============
        // getProperties
        // =============
        DataObjectPtr ComponentContext::getProperties()
        {
            LOGENTRY(1, "ComponentContext::getProperties");
            DataObjectPtr properties = impl->getProperties();
            LOGEXIT(1, "ComponentContext::getProperties");
            return properties;
        }

        // ============
        // getDataFactory
        // =============
        DataFactoryPtr ComponentContext::getDataFactory()
        {
            LOGENTRY(1, "ComponentContext::getDataFactory");
            DataFactoryPtr dataFactory = impl->getDataFactory();
            LOGEXIT(1, "ComponentContext::getDataFactory");
            return dataFactory;
        }

        
    } // End namespace sca
} // End namespace osoa
