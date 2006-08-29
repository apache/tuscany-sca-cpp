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

#include "osoa/sca/ServiceList.h"

#include "osoa/sca/ServiceRuntimeException.h"

namespace osoa
{
    namespace sca
    {
        // ===========
        // Constructor
        // ===========
        ServiceList::ServiceList(unsigned int numTargets)
            : maxServices(numTargets), numServices(0)
        {
            services = new void*[numTargets];
        }
        
        // ==========================================
        // Copy comstructor: create new service array
        // ==========================================
        ServiceList::ServiceList(const ServiceList& sl)
            : maxServices(sl.maxServices), numServices(sl.numServices)
        {
            services = new void*[maxServices];
            for (unsigned int i=0; i<numServices; i++)
            {
                services[i] = sl.services[i];
            }
        }
        
        // ===================================
        // operator=: create new service array
        // ===================================
        ServiceList& ServiceList::operator=(const ServiceList& sl)
        {
            if (this != &sl)
            {
                maxServices = sl.maxServices;
                numServices = sl.numServices;
                
                services = new void*[maxServices];
                for (unsigned int i=0; i<numServices; i++)
                {
                    services[i] = sl.services[i];
                }
            }
            return *this;
        }
        
        // ==========
        // Destructor
        // ==========
        ServiceList::~ServiceList()
        {
            delete [] services;
        }

        // =================================
        // operator[]: return service at pos
        // =================================
        void* ServiceList::operator[] (unsigned int pos)
        {
            if (pos < numServices)
            {
                return services[pos];
            }
            else
            {
                throw ServiceRuntimeException("ServiceList: index out of bounds");
            }
        }

        // ====================================
        // addService: add service to the array
        // ====================================
        void ServiceList::addService(void* service)
        {
            if (numServices < maxServices)
            {
                services[numServices] = service;
                numServices++;
            }
        }
        
    } // End namespace sca
} // End namespace osoa
