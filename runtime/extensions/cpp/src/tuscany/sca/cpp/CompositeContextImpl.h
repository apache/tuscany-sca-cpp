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

#ifndef tuscany_sca_cpp_compositecontextimpl_h
#define tuscany_sca_cpp_compositecontextimpl_h

#include "osoa/sca/CompositeContext.h"

#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/cpp/CPPServiceProxy.h"

namespace tuscany
{
    namespace sca
    {
       
       namespace cpp
        {

        /** 
         * Contains the actual implementation of a CompositeContext interface.
         */
        class CompositeContextImpl : public osoa::sca::CompositeContext
        {
        
        public:
            /** 
             * Constructor that takes a Composite which represents the runtime
             * model for this context.
             */
            CompositeContextImpl(tuscany::sca::model::Component* component);

            /**
             * See CompositeContext#locateService.
             */
            virtual void* locateService(const std::string& serviceName);
            
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
            tuscany::sca::model::Composite* composite;

            /**
             * Pointer to the runtime model Component object to which this
             * context refers.
             */
            tuscany::sca::model::Component* component;

        };

       } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_compositecontextimpl_h
