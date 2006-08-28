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

#ifndef tuscany_sca_model_interface_h
#define tuscany_sca_model_interface_h

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Information about an interface. Subtypes will hold information
             * specific to a type of interface.
             */
            class SCA_API Interface 
            {
                
            public:
                
                Interface();  
                virtual ~Interface();
                            
            private:
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_interface_h

