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

#include "osoa/sca/ModuleContext.h"
#include "tuscany/sca/util/Logging.h"
#include "osoa/sca/ModuleContextImpl.h"
#include "tuscany/sca/core/SCARuntime.h"

using namespace tuscany::sca;

namespace osoa
{
    namespace sca
    {
        // ===========
        // Constructor
        // ===========    
        ModuleContext::ModuleContext(ModuleContextImpl* implementation)
            : impl(implementation)
        {
        }

        // ===================================
        // Copy constructor: create a new impl
        // ===================================
        ModuleContext::ModuleContext(const ModuleContext& ctx)
        {
            impl = new ModuleContextImpl(SCARuntime::getInstance()->getCurrentModule());
        }
        
        // =============================
        // operator= : create a new impl
        // =============================
        ModuleContext& ModuleContext::operator=(const ModuleContext& ctx)
        {
            if (this != &ctx)
            {
                impl = new ModuleContextImpl(SCARuntime::getInstance()->getCurrentModule());
            }
            return *this;
        }
        
        // ==========
        // Destructor
        // ==========
        ModuleContext::~ModuleContext()
        {
            LOGENTRY(1, "ModuleContext::destructor");
            delete impl;
            LOGEXIT(1, "ModuleContext::destructor");
        }
        
        // ====================================================
        // getCurrent: create a context from the current module
        // ====================================================
        ModuleContext ModuleContext::getCurrent()
        {
            LOGENTRY(1, "ModuleContext::getCurrent");
            ModuleContextImpl* mci = new ModuleContextImpl(SCARuntime::getInstance()->getCurrentModule());
            LOGEXIT(1, "ModuleContext::getCurrent");
            return ModuleContext(mci);
        }

        // =============
        // locateService
        // =============
        void* ModuleContext::locateService(const char* serviceName)
        {
            LOGENTRY(1, "ModuleContext::locateService");
            void* sp = impl->locateService(serviceName);
            LOGEXIT(1, "ModuleContext::locateService");
            return sp;
        }

        // =============
        // getDataFactory
        // =============
        DataFactoryPtr ModuleContext::getDataFactory()
        {
            LOGENTRY(1, "ModuleContext::getDataFactory");
            DataFactoryPtr df = impl->getDataFactory();
            LOGEXIT(1, "ModuleContext::getDataFactory");
            return df;
        }

        
    } // End namespace sca
} // End namespace osoa
