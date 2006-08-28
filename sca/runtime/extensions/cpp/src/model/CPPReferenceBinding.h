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

#ifndef tuscany_sca_extension_cpp_model_cppreferencebinding_h
#define tuscany_sca_extension_cpp_model_cppreferencebinding_h

#include "tuscany/sca/model/ReferenceBinding.h"

using namespace tuscany::sca;
using namespace tuscany::sca::model;

#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            /**
             * Information about a CPP service binding for service or a reference.
             */
            class CPPReferenceBinding : public ReferenceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 * @param port The definition of the port to which the entrypoint
                 * or external service is to be bound. This is of the form
                 * "namespace"#endpoint("service"/"port")
                 */
                CPPReferenceBinding(Reference* reference);  

                /**
                 * Destructor.
                 */
                virtual ~CPPReferenceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual string getType() { return "http://www.osoa.org/xmlns/sca/1.0#CPPImplementationBinding"; };
                            
                /**
                 * Create a proxy representing the reference to the
                 * client component.
                 */
                 virtual ServiceProxy* getServiceProxy();
                                
                 /**
                  * Configure this binding from a service binding.
                  */
                  virtual void configure(ServiceBinding* serviceBinding);
                  
                 /**
                  * Returns the target service binding.
                  */
                  ServiceBinding* getTargetServiceBinding() const { return targetServiceBinding; };
                                
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
            };
            
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_cpp_model_cppreferencebinding_h
