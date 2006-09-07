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

#ifndef tuscany_sca_php_model_phpservicebinding_h
#define tuscany_sca_php_model_phpservicebinding_h

#include "tuscany/sca/model/ServiceBinding.h"
using namespace tuscany::sca::model;
#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace php
        {
            /**
             * Information about a PHP service binding for service or a reference.
             */
            class PHPServiceBinding : public ServiceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 * @param port The definition of the port to which the entrypoint
                 * or external service is to be bound. This is of the form
                 * "namespace"#endpoint("service"/"port")
                 */
                PHPServiceBinding(Service* service);  

                /**
                 * Destructor.
                 */
                virtual ~PHPServiceBinding();

                /**
                 * Returns the type of binding.
                 */                
                virtual string getType() { return "http://www.osoa.org/xmlns/sca/1.0#PHPImplementationBinding"; };
                            
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
            
        } // End namespace php
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_php_model_phpservicebinding_h
