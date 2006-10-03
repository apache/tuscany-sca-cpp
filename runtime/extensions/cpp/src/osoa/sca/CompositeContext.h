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

#ifndef osoa_sca_compositecontext_h
#define osoa_sca_compositecontext_h

#include "osoa/sca/export.h"
#include "commonj/sdo/SDO.h"

namespace osoa
{
    namespace sca
    {
        /**
         * An SCA component implementation, or a non-SCA client, uses the 
         * CompositeContext class to retrieve information about the configured 
         * SCA composite.
         */
        class SCA_CPP_API CompositeContext  
        {
        
        public:
            /** 
             * Return a new CompositeContext for the current Component.
             */
            static CompositeContext getCurrent();

            /** 
             * Resolve a service name into a single component service.
             * @param serviceName The name of the service in the form
             * "component name"/"service name". The service name is
             * optional in the component has one service.
             * @return A pointer to an object which can be cast to the
             * business interface of the target service.
             */
            virtual void* locateService(const std::string& serviceName);

            /** 
             * Get an SDO data factory which will allow the component to 
             * create data objects for all the types configured for this
             * component.
             * @return A data factory to be used by the component to create
             * new data objects.
             */
            virtual commonj::sdo::DataFactoryPtr getDataFactory();

            /**
             * Destructor.
             */
            virtual ~CompositeContext();

            /**
             * Constructor
             */
            CompositeContext(const CompositeContext&);
            
            /**
             * Copy operator
             */
            CompositeContext& operator=(const CompositeContext&);
            
            /**
             * Constructor to create an interface class from the contained
             * implementation.
             * @param  implementation The actual implementation class.
             */
            CompositeContext(CompositeContext* implementation);
            
        private:

            /**
             * Pointer to the class which provides the actual implementation.
             */
            CompositeContext* impl;
            
        };
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_compositecontext_h
