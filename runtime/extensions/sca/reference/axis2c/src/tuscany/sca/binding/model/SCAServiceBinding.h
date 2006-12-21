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

#ifndef tuscany_sca_extension_binding_model_scaservicebinding_h
#define tuscany_sca_extension_binding_model_scaservicebinding_h

#include <string>

#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/ws/model/WSServiceBinding.h"


namespace tuscany
{
    namespace sca
    {
        namespace binding
        {
            /**
             * Information about an SCA service binding for service or a reference.
             */
            class SCAServiceBinding : public tuscany::sca::ws::WSServiceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 */
                SCAServiceBinding(tuscany::sca::model::Service* service, const std::string& uri);  

                /**
                 * Destructor.
                 */
                virtual ~SCAServiceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual std::string getType() { return "http://www.osoa.org/xmlns/sca/1.0#SCABinding"; };
                            
            private:
            
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_ws_model_wsservicebinding_h
