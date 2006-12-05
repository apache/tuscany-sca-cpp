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

#ifndef tuscany_sca_model_compositereferencebinding_h
#define tuscany_sca_model_compositereferencebinding_h

#include "tuscany/sca/model/ReferenceBinding.h"

#include <string>

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * A default composite reference binding
             */
            class CompositeReferenceBinding : public ReferenceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 */
                SCA_API CompositeReferenceBinding(Reference* reference);  

                /**
                 * Destructor.
                 */
                SCA_API virtual ~CompositeReferenceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual string getType() { return "http://www.osoa.org/xmlns/sca/1.0#CompositeServiceBinding"; };
                            
                 /**
                  * Configure this binding from a service binding.
                  */
                SCA_API virtual void configure(ServiceBinding* serviceBinding);
                                
                 /**
                  * Configure this binding from a URI.
                  */
                  SCA_API virtual void configure(const string& uri);
                                
                /**
                 * Create a proxy representing the reference to the
                 * client component.
                 */
                SCA_API virtual ServiceProxy* getServiceProxy();
                                
                 /**
                  * Returns the target service binding.
                  */
                  ServiceBinding* getTargetServiceBinding() const { return targetServiceBinding; };
                                
                /**
                 * Returns the binding URI.
                 * @return The binding URI.
                 */
                SCA_API virtual const string& getURI() const { return uri; };
                
            private:
            
                /**
                 * The proxy representing the reference to the client
                 * component.
                 */
                ServiceProxy* serviceProxy;
                
                /**
                 * The service binding of the target
                 */
                ServiceBinding* targetServiceBinding;
                
                /**
                 * The binding URI
                 */
                 string uri; 
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_compositereferencebinding_h
