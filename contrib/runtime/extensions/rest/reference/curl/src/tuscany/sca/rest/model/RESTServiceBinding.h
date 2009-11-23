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

#ifndef tuscany_sca_extension_rest_model_restservicebinding_h
#define tuscany_sca_extension_rest_model_restservicebinding_h

#include <string>

#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/model/Service.h"

namespace tuscany
{
    namespace sca
    {
        namespace rest
        {
            /**
             * Information about a REST binding for a service or a reference.
             */
            class RESTServiceBinding : public tuscany::sca::model::ServiceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 */
                RESTServiceBinding(tuscany::sca::model::Service* service, const std::string& uri);  

                /**
                 * Destructor.
                 */
                virtual ~RESTServiceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual std::string getType() { return "http://www.osoa.org/xmlns/sca/1.0#RESTBinding"; };
                            
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
            
        } // End namespace rest
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_rest_model_restservicebinding_h
