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

#include "osoa/sca/CompositeContext.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/cpp/CompositeContextImpl.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "commonj/sdo/SDO.h"

using commonj::sdo::DataFactoryPtr;
using namespace tuscany::sca;
using namespace tuscany::sca::cpp;


namespace osoa
{
    namespace sca
    {
        // ===========
        // Constructor
        // ===========    
        CompositeContext::CompositeContext(CompositeContext* implementation)
            : impl(implementation)
        {
        }

        // ===================================
        // Copy constructor: create a new impl
        // ===================================
        CompositeContext::CompositeContext(const CompositeContext& ctx)
        {
            impl = new CompositeContextImpl(
                tuscany::sca::SCARuntime::getInstance()->getDefaultComponent());
        }
        
        // =============================
        // operator= : create a new impl
        // =============================
        CompositeContext& CompositeContext::operator=(const CompositeContext& ctx)
        {
            if (this != &ctx)
            {
                impl = new CompositeContextImpl(
                    tuscany::sca::SCARuntime::getInstance()->getDefaultComponent());
            }
            return *this;
        }
        
        // ==========
        // Destructor
        // ==========
        CompositeContext::~CompositeContext()
        {
            LOGENTRY(1, "CompositeContext::destructor");
            delete impl;
            LOGEXIT(1, "CompositeContext::destructor");
        }
        
        // ====================================================
        // getCurrent: create a context from the current composite
        // ====================================================
        CompositeContext CompositeContext::getCurrent()
        {
            LOGENTRY(1, "CompositeContext::getCurrent");
            CompositeContext* cci = new CompositeContextImpl(
                tuscany::sca::SCARuntime::getInstance()->getDefaultComponent());
            LOGEXIT(1, "CompositeContext::getCurrent");
            return CompositeContext(cci);
        }

        // =============
        // locateService
        // =============
        void* CompositeContext::locateService(const char* serviceName)
        {
            LOGENTRY(1, "CompositeContext::locateService");
            void* sp = impl->locateService(serviceName);
            LOGEXIT(1, "CompositeContext::locateService");
            return sp;
        }

        // =============
        // getDataFactory
        // =============
        DataFactoryPtr CompositeContext::getDataFactory()
        {
            LOGENTRY(1, "CompositeContext::getDataFactory");
            DataFactoryPtr df = impl->getDataFactory();
            LOGEXIT(1, "CompositeContext::getDataFactory");
            return df;
        }

        
    } // End namespace sca
} // End namespace osoa
