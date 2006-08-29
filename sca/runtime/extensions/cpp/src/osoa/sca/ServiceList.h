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

#ifndef osoa_sca_servicelist_h
#define osoa_sca_servicelist_h

#include "osoa/sca/export.h"

namespace tuscany
{
    namespace sca
    {

        namespace cpp
        {
            class ComponentContextImpl;
        }
    }
}

namespace osoa
{
    namespace sca
    {

        /**
         * Holds a list of services that can be accessed from an SCA
         * component. Each entry can be cast to the business interface
         * of the target component. All the entries will be of the
         * same type.
         */
        class SCA_CPP_API ServiceList  
        {
        
        public:
            /**
             * Return the number of services in the list.
             * @return Number of services.
             */
            unsigned int size() {return numServices;}

            /**
             * Return the service at this position.
             */
            void* operator[] (unsigned int pos);

            ServiceList(const ServiceList& serviceList);
            ServiceList& operator=(const ServiceList& serviceList);
            virtual ~ServiceList();
            
        private:
        
            //TODO Should remove this dependency on the implementation
            friend class tuscany::sca::cpp::ComponentContextImpl;

            /**
             * Create a new service list with the known number of 
             * target services. Once created, the service list can only
             * have services added to it up to the number of targets
             * specified on this constructor.
             * @param numTargets Number of target services to be held in this list.
             */
            ServiceList(unsigned int numTargets);

            /**
             * Add a service to this list.
             */
            void addService(void* service);

            unsigned int maxServices;
            unsigned int numServices;

            /** 
             * Pointer to an array of services. 
             */
            void** services;
                        
        };
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_servicelist_h
