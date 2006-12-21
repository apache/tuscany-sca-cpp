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

#ifndef tuscany_sca_model_referencebinding_h
#define tuscany_sca_model_referencebinding_h

#include <string>

#include "tuscany/sca/model/Binding.h"

namespace tuscany
{
    namespace sca
    {
        class ServiceProxy;
       
        
        namespace model
        {
            class Reference;
            class ServiceBinding;
            
            /**
             * Represents a reference binding.
             * Bindings are used by services and references. References use bindings
             * to describe the access mechanism used to call an external service (which can
             * be a service provided by another SCA composite). Services use bindings to describe
             * the access mechanism that clients (which can be a client from another SCA composite)
             * have to use to call the service.
             * This interface will typically be extended by binding implementations to allow
             * specification of binding/transport specific information.
             */
            class ReferenceBinding : public Binding 
            {
            public:

                /**
                 * Constructor to create a new binding.
                 */ 
                SCA_API ReferenceBinding(Reference* reference, const std::string& uri);

                /**
                 * Destructor.
                 */ 
                SCA_API virtual ~ReferenceBinding();
                            
                /**
                 * Create a proxy representing the reference to the
                 * client component.
                 */
                 SCA_API virtual ServiceProxy* getServiceProxy() = 0;
                 
                 /**
                  * Configure this binding from a service binding.
                  */
                  SCA_API virtual void configure(ServiceBinding* serviceBinding) = 0;
                                
                 /**
                  * Configure this binding from a URI.
                  */
                  SCA_API virtual void configure(const std::string& uri);
                                
                  /**
                   * Returns the reference.
                   * @return The reference.
                   */
                   SCA_API Reference* getReference() const { return reference; };
                   
            private:
            
                /**
                 * The reference configured with the binding.
                 */
                Reference* reference;
            
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_referencebinding_h
