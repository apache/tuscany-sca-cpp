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

#ifndef tuscany_sca_model_interface_h
#define tuscany_sca_model_interface_h

#include <string>

using namespace std;

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
            class Interface 
            {
                
            public:
                
                /**
                 * Constructor
                 */
                SCA_API Interface(bool remotable, bool conversational);
                  
                /**
                 * Destructor
                 */
                SCA_API virtual ~Interface();
                
                /**
                 * Returns true if the interface is remotable.
                 * @return True if the interface is remotable.
                 */
                SCA_API bool isRemotable() const { return remotable; };
                            
                /**
                 * Returns true if the interface is conversational.
                 * @return True if the interface is conversational.
                 */
                SCA_API bool isConversational() const { return conversational; };
                            
                /**
                * return the QName of schema type for this interface type
                * (e.g. "http://www.osoa.org/xmlns/sca/1.0#WSDLInterface")
                */
                SCA_API virtual const string& getInterfaceTypeQName() = 0;
    
            private:

                /**
                 * True if the interface is remotable
                 */            
                bool remotable;

                /**
                 * True if the interface is conversational
                 */            
                bool conversational;

            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_interface_h

