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

#ifndef osoa_sca_modulecontext_h
#define osoa_sca_modulecontext_h

#include "osoa/sca/export.h"
#include "commonj/sdo/SDO.h"

namespace osoa
{
    namespace sca
    {
        class ModuleContextImpl;

        /**
         * An SCA component implementation, or a non-SCA client, uses the 
         * ModuleContext class to retrieve information about the configured 
         * SCA module.
         */
        class SCA_API ModuleContext  
        {
        
        public:
            /** 
             * Return a new ModuleContext for the current Component.
             */
            static ModuleContext getCurrent();

            /** 
             * Resolve a service name into a single component service.
             * @param serviceName The name of the service in the form
             * "component name"/"service name". The service name is
             * optional in the component has one service.
             * @return A pointer to an object which can be cast to the
             * business interface of the target service.
             */
            void* locateService(const char* serviceName);

            /** 
             * Get an SDO data factory which will allow the component to 
             * create data objects for all the types configured for this
             * component.
             * @return A data factory to be used by the component to create
             * new data objects.
             */
            commonj::sdo::DataFactoryPtr getDataFactory();

            /**
             * Destructor.
             */
            virtual ~ModuleContext();

            ModuleContext(const ModuleContext&);
            ModuleContext& operator=(const ModuleContext&);
            
        private:
            /**
             * Constructor to create an interface class from the contained
             * implementation.
             * @param  implementation The actual implementation class.
             */
            ModuleContext(ModuleContextImpl* implementation);

            /**
             * Pointer to the class which provides the actual implementation.
             */
            ModuleContextImpl* impl;
            
        };
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_modulecontext_h
