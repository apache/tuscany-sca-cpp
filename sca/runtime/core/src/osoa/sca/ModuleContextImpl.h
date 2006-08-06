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

#ifndef osoa_sca_modulecontextimpl_h
#define osoa_sca_modulecontextimpl_h
#include "tuscany/sca/model/Module.h"
#include "tuscany/sca/core/ServiceProxy.h"

namespace osoa
{
    namespace sca
    {
        /** 
         * Contains the actual implementation of a ModuleContext interface.
         */
        class ModuleContextImpl  
        {
        
        public:
            /** 
             * Constructor that takes a Module which represents the runtime
             * model for this context.
             */
            ModuleContextImpl(tuscany::sca::model::Module* module);

            /**
             * See ModuleContext#locateService.
             */
            void* locateService(const char* serviceName);
            
            /** 
             * See ModuleContext.
             */
            commonj::sdo::DataFactoryPtr getDataFactory();

            /**
             * Destructor.
             */
            virtual ~ModuleContextImpl();
        private:
            ModuleContextImpl(const ModuleContextImpl&);
            ModuleContextImpl& operator=(const ModuleContextImpl&);

            /**
             * Pointer to the runtime model Module object to which this
             * context refers.
             */
            tuscany::sca::model::Module* module;

            typedef vector<tuscany::sca::ServiceProxy*> PROXIES;
            /**
             * Vector of proxies created from calls to the locateService
             * method.
             */
            PROXIES proxies;
        };
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_modulecontextimpl_h
