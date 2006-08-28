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

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#ifndef tuscany_sca_model_binding_h
#define tuscany_sca_model_binding_h
#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Represents a binding.
             * Bindings are used by services and references. References use bindings
             * to describe the access mechanism used to call an external service (which can
             * be a service provided by another SCA composite). Services use bindings to describe
             * the access mechanism that clients (which can be a client from another SCA composite)
             * have to use to call the service.
             * This interface will typically be extended by binding implementations to allow
             * specification of binding/transport specific information.
             */
            class Binding 
            {
                
            public:

                /**
                 * Constructor to create a new binding.
                 * @param uri The binding URI.
                 */ 
                Binding(const string& uri);

                /**
                 * Destructor.
                 */ 
                virtual ~Binding();
                
                /**
                 * Returns the binding type
                 * @return The binding type.
                 */
                virtual string getType() = 0;
                            
                /**
                 * Returns the binding URI.
                 * @return The binding URI.
                 */
                string getURI() const { return uri; };
                
            private:
            
                /**
                 * The binding URI.
                 */ 
                string uri;
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_binding_h
