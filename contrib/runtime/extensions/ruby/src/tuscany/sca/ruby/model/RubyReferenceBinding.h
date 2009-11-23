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


#ifndef tuscany_sca_ruby_model_rubyreferencebinding_h
#define tuscany_sca_ruby_model_rubyreferencebinding_h

#include <string>

#include "tuscany/sca/model/ReferenceBinding.h"


namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            /**
             * Information about a Ruby service binding for service or a reference.
             */
            class RubyReferenceBinding : public tuscany::sca::model::ReferenceBinding
            {    
            public:

                /**
                 * Constructor.
                 */
                RubyReferenceBinding(tuscany::sca::model::Reference* reference);  

                /**
                 * Destructor.
                 */
                virtual ~RubyReferenceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual std::string getType() { return "http://www.osoa.org/xmlns/sca/1.0#RubyImplementationBinding"; };
                            
                /**
                 * Create a proxy representing the reference to the
                 * client component.
                 */
                 virtual ServiceProxy* getServiceProxy();
                                
                 /**
                  * Configure this binding from a service binding.
                  */
                  virtual void configure(tuscany::sca::model::ServiceBinding* serviceBinding);

            private:
                
                /**
                 * The proxy representing the reference to the client
                 * component.
                 */
                ServiceProxy* serviceProxy;
            };
            
        } // End namespace ruby
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_ruby_model_rubyreferencebinding_h
