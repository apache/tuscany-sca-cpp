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

#ifndef osoa_sca_componentcontextimpl_h
#define osoa_sca_componentcontextimpl_h

#include "osoa/sca/ServiceList.h"

#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/core/ServiceProxy.h"
#include "tuscany/sca/model/Service.h"

using namespace tuscany::sca;
using namespace tuscany::sca::model;

namespace osoa
{
    namespace sca
    {
        /** 
         * Contains the actual implementation of a ComponentContext interface.
         */
        class ComponentContextImpl  
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
            void* getService(const char* referenceName);

            /** 
             * See ComponentContext.
             */
            ServiceList getServices(const char* referenceName);

            /** 
             * See ComponentContext.
             */
            DataObjectPtr getProperties();

            /** 
             * See ComponentContext.
             */
            commonj::sdo::DataFactoryPtr getDataFactory();

            /** 
             * Returns the contained Component.
             * @return The Component to which this context refers.
             */
            Component* getComponent() {return component;}
            
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
                ServiceReference* serviceReference,
                WireTarget* target);

            /**
             * Helper method to return a wrapper for a target service.
             * @param target The target for which this wrapper is to be created.
             * @return The service wrapper.
             */
            ServiceWrapper* getServiceWrapper(WireTarget* target);

            
            typedef vector<ServiceProxy*> PROXIES;
            /**
             * A vector of the proxies created by this ComponentContext. The 
             * proxies will be destroyed when the ComponentContext is destroyed.
             */
            PROXIES proxies;
        };
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_componentcontextimpl_h
