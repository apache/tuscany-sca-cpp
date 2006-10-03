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

#ifndef tuscany_sca_python_model_pythonreferencebinding_h
#define tuscany_sca_python_model_pythonreferencebinding_h

#include "tuscany/sca/model/ReferenceBinding.h"


#include <string>
using std::string;

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            /**
             * Information about a Python service binding for service or a reference.
             */
            class PythonReferenceBinding : public ReferenceBinding
            {    
            public:

                /**
                 * Constructor.
                 */
                PythonReferenceBinding(Reference* reference);  

                /**
                 * Destructor.
                 */
                virtual ~PythonReferenceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual string getType() { return "http://www.osoa.org/xmlns/sca/1.0#PythonImplementationBinding"; };
                            
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
            
        } // End namespace python
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_python_model_pythonreferencebinding_h
