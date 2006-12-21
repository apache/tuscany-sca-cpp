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

#ifndef tuscany_sca_cpp_model_cppinterface_h
#define tuscany_sca_cpp_model_cppinterface_h

#include <string>

#include "tuscany/sca/model/Interface.h"


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
            class CPPInterface : public tuscany::sca::model::Interface
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
                    const std::string& header,
                    const std::string& className,
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
                const std::string& getHeader() { return header; }

                /**
                 * Return the name of the header file without the extension.
                 * @return Header file name without any extension.
                 */
                const std::string& getHeaderStub() { return headerStub; }

                /**
                 * Get the name of the class.
                 * @return The name of the class defining the interface.
                 */
                const std::string& getClass() { return className; }

                /**
                * return the QName of the schema type for this interface type
                * (e.g. "http://www.osoa.org/xmlns/sca/1.0#interface.cpp")
                */
                virtual const std::string& getInterfaceTypeQName() { return typeQName; };
    
           private:
           
                /**
                 * Name of the header file containing the definition of the interface.
                 */
                std::string header;

                /**
                 * Name of the header file without the extension.
                 */
                std::string headerStub;

                /**
                 * Name of the class in the header file.
                 */
                std::string className;

                /**
                 * The QName of the schema type for this interface type.
                 */
                static const std::string typeQName;
            };
            
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_model_cppinterface_h

