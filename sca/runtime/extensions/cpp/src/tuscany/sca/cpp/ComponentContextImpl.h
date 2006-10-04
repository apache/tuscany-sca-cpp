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

#ifndef tuscany_sca_cpp_componentcontextimpl_h
#define tuscany_sca_cpp_componentcontextimpl_h

#include <vector>
using std::vector;

#include "osoa/sca/ComponentContext.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/cpp/CPPServiceProxy.h"

using namespace osoa::sca;
        

namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {

        /** 
         * Contains the actual implementation of a ComponentContext interface.
         */
        class ComponentContextImpl : public ComponentContext  
        {
        
        public:
            /** 
             * Constructor that takes a Component which represents the runtime
             * model for this context.
             */
            ComponentContextImpl(Component* component);

            /**
             * Default constructor.
             */
            virtual ~ComponentContextImpl();

            /** 
             * See ComponentContext.
             */
            virtual void* getService(const std::string& referenceName);

            /** 
             * See ComponentContext.
             */
            virtual std::list<void*> getServices(const std::string& referenceName);

            /** 
             * See ComponentContext.
             */
            virtual DataObjectPtr getProperties();

            /** 
             * See ComponentContext.
             */
            virtual commonj::sdo::DataFactoryPtr getDataFactory();

            /** 
             * Returns the contained Component.
             * @return The Component to which this context refers.
             */
            virtual Component* getComponent() {return component;}
            
        private:
            ComponentContextImpl(const ComponentContextImpl&);
            ComponentContextImpl& operator=(const ComponentContextImpl&);

            /**
             * Pointer to the runtime model Component to which this 
             * context refers.
             */
            Component* component;

            /**
             * Helper method to return a proxy to a service.
             * @param serviceReference The source reference.
             * @param target The target to which this source reference is wired.
             * @return A pointer to an object which can be cast to the business
             * class representing the target.
             */
            void* getServiceProxy(
                Reference* serviceReference,
                Service* target);

            /**
             * Helper method to return a wrapper for a target service.
             * @param target The target for which this wrapper is to be created.
             * @return The service wrapper.
             */
            ServiceWrapper* getServiceWrapper(Service* target);

        };

        } // End namespaca cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_componentcontextimpl_h