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

/* $Rev$ $Date$ */

#ifndef tuscany_sca_cpp_compositecontextimpl_h
#define tuscany_sca_cpp_compositecontextimpl_h

#include <vector>
using std::vector;

#include "osoa/sca/CompositeContext.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/cpp/CPPServiceProxy.h"

using namespace osoa::sca;

namespace tuscany
{
    namespace sca
    {
       
       namespace cpp
        {

        /** 
         * Contains the actual implementation of a CompositeContext interface.
         */
        class CompositeContextImpl : public CompositeContext
        {
        
        public:
            /** 
             * Constructor that takes a Composite which represents the runtime
             * model for this context.
             */
            CompositeContextImpl(Component* component);

            /**
             * See CompositeContext#locateService.
             */
            virtual void* locateService(const char* serviceName);
            
            /** 
             * See CompositeContext.
             */
            virtual commonj::sdo::DataFactoryPtr getDataFactory();

            /**
             * Destructor.
             */
            virtual ~CompositeContextImpl();
            
        private:
            CompositeContextImpl(const CompositeContextImpl&);
            CompositeContextImpl& operator=(const CompositeContextImpl&);

            /**
             * Pointer to the runtime model Composite object to which this
             * context refers.
             */
            Composite* composite;

            /**
             * Pointer to the runtime model Component object to which this
             * context refers.
             */
            Component* component;

            /**
             * Vector of proxies created from calls to the locateService
             * method.
             */
            typedef vector<CPPServiceProxy*> PROXIES;
            PROXIES proxies;
        };

       } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_compositecontextimpl_h
