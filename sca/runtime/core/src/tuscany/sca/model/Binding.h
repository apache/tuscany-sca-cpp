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
             * Information about the binding for CompositeServiceType and CompositeReferenceType. An
             * abstract class which will be extended by classes that hold specific
             * information for each type of binding.
             */
            class Binding 
            {
                
            public:
                /**
                 * Supported binding types.
                 */
                enum Type
                {
                    WS,
                    SCA
                };
                
                /**
                 * Constructor to create a new binding.
                 * @param uri The uri specified in the scdl file.
                 */ 
                Binding(const string& uri);  

                /**
                 * Destructor.
                 */ 
                virtual ~Binding();
                            
                /**
                 * Return the enumerated type of binding.
                 * @return The type of the binding (see Binding#Type).
                 */ 
                virtual Type getBindingType() = 0;

                /**
                 * Return the uri of the binding.
                 * @return The uri of the binding.
                 */
                string getUri() {return uri;};
                
            private:
                /**
                 * The uri of the binding.
                 */ 
                string uri;
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_binding_h

