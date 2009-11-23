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

#ifndef tuscany_sca_extension_rest_model_restreferencebinding_h
#define tuscany_sca_extension_rest_model_restreferencebinding_h

#include <string>

#include "tuscany/sca/model/ReferenceBinding.h"

namespace tuscany
{
    namespace sca
    {
        namespace rest
        {
            /**
             * Information about a web service binding for service or a reference.
             */
            class RESTReferenceBinding : public tuscany::sca::model::ReferenceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 */
                RESTReferenceBinding(tuscany::sca::model::Reference* reference, const std::string& uri);  

                /**
                 * Destructor.
                 */
                virtual ~RESTReferenceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual std::string getType() { return "http://www.osoa.org/xmlns/sca/1.0#RESTBinding"; };
                            
                 /**
                  * Configure this binding from a service binding.
                  */
                  virtual void configure(tuscany::sca::model::ServiceBinding* serviceBinding);
                                
                /**
                 * Create a proxy representing the reference to the
                 * client component.
                 */
                 virtual ServiceProxy* getServiceProxy();

            private:
            
                /**
                 * The proxy representing the reference to the client
                 * component.
                 */
                ServiceProxy* serviceProxy;
            };
            
        } // End namespace rest
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_rest_model_restreferencebinding_h
