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

#ifndef tuscany_sca_cpp_model_cppinterface_h
#define tuscany_sca_cpp_model_cppinterface_h

#include "tuscany/sca/model/Interface.h"

#include <map>
using std::map;
#include <string>
using std::string;

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            /**
             * Holds information about an interface described using a C++
             * header file.
             */
            class CPPInterface : public Interface
            {
                
            public:    
                /**
                 * Constuctor.
                 * @param header Name of the header file containing the class that
                 * describes the interface.
                 * @param className Name of the class in the header file that 
                 * describes the interface. 
                 * @param scope The scope of the interface (stateless or composite).
                 * @param remotable True if the interface is remotable.
                 */
                CPPInterface(
                    const string& header,
                    const string& className,
                    bool remotable,
                    bool conversational);  

                /**
                 * Destructor.
                 */
                virtual ~CPPInterface();
                
                /**
                 * Get the name of the header file.
                 * @return The name of the header file containing the definition of the
                 * interface.
                 */
                const string& getHeader() { return header; }

                /**
                 * Return the name of the header file without the extension.
                 * @return Header file name without any extension.
                 */
                const string& getHeaderStub() { return headerStub; }

                /**
                 * Get the name of the class.
                 * @return The name of the class defining the interface.
                 */
                const string& getClass() { return className; }

           private:
           
                /**
                 * Name of the header file containing the definition of the interface.
                 */
                string header;

                /**
                 * Name of the header file without the extension.
                 */
                string headerStub;

                /**
                 * Name of the class in the header file.
                 */
                string className;

            };
            
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_model_cppinterface_h

