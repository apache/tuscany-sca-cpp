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

#ifndef tuscany_sca_cpp_model_cppservicebinding_h
#define tuscany_sca_cpp_model_cppservicebinding_h

#include <string>

#include "tuscany/sca/model/ServiceBinding.h"


namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            /**
             * Information about a CPP service binding for service or a reference.
             */
            class CPPServiceBinding : public tuscany::sca::model::ServiceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 * @param port The definition of the port to which the entrypoint
                 * or external service is to be bound. This is of the form
                 * "namespace"#endpoint("service"/"port")
                 */
                CPPServiceBinding(tuscany::sca::model::Service* service);  

                /**
                 * Destructor.
                 */
                virtual ~CPPServiceBinding();

                /**
                 * Returns the type of binding.
                 */                
                virtual std::string getType() { return "http://www.osoa.org/xmlns/sca/1.0#CPPImplementationBinding"; };
                            
                /**
                 * Create a wrapper for the service configured by this
                 * binding.
                 */
                 virtual ServiceWrapper* getServiceWrapper();
                                
            private:

                /**
                 * The wrapper for the service configured by this binding.
                 */            
                ServiceWrapper* serviceWrapper;
            
            };
            
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_model_cppservicebinding_h
