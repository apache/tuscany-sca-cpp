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


#ifndef tuscany_sca_core_serviceproxy_h
#define tuscany_sca_core_serviceproxy_h

#include "tuscany/sca/export.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"


namespace tuscany
{
    namespace sca
    {
        
        /**
         * Holds a proxy for a given component and reference.
         * The proxy which is held inside a ServiceProxy will be specific to the component
         * and reference and will have been code generated and be contained in a dll 
         * created by a developer of an SCA application.
         */
        class SCA_API ServiceProxy 
        {
        public:
            /**
             * Create a new service proxy for a reference. The proxy will contain a pointer to
             * the target ServiceWrapper.
             * @param reference The reference on the source component.
             * @param target The wrapper of the service which is wired to this reference.
             */
            ServiceProxy(tuscany::sca::model::Reference* reference);

            /**
             * Destructor.
             */
            virtual    ~ServiceProxy();
            
            /**
             * Returns the reference represented by this proxy.
             * @return The Reference represented by this proxy.
             */
            tuscany::sca::model::Reference* getReference() const { return reference; };

        private:
        
            /**
             * The reference represented by this proxy.
             */
            tuscany::sca::model::Reference* reference;
        
        };
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_core_serviceproxy_h
