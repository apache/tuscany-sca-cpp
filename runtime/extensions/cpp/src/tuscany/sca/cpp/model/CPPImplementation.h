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

#ifndef tuscany_sca_cpp_model_cppimplementation_h
#define tuscany_sca_cpp_model_cppimplementation_h

#include <map>
#include <string>

#include "tuscany/sca/model/ComponentType.h"

namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            /**
             * Holds information about an SCA implementation written in C++
             */
            class CPPImplementation : public tuscany::sca::model::ComponentType
            {
                
            public:
                /**
                 * Scope of the component implementation.
                 */
                enum Scope
                {
                    COMPOSITE,
                    STATELESS
                };

                /**
                 * Constructor.
                 * @param dllName Name of the shared library.
                 * @param header Name of the header file that contains the class declaring the 
                 * implementation class.
                 * @param className Name of the class in the header file (could be a blank string
                 * if this is not specified).
                 */
                CPPImplementation(tuscany::sca::model::Composite* composite,
                    const std::string& library, const std::string& header, const std::string&headerPath,
                    const std::string& headerStub, const std::string& className, Scope scope);
                
                /**
                 * Destructor
                 */
                virtual ~CPPImplementation();

                /**
                 * Initialize a component of this type.
                 * @param component The component to initialize.
                 */
                virtual void initializeComponent(tuscany::sca::model::Component* component);

                /**
                 * Returns the name of the shared library.
                 * @return The name of the shared library.
                 */
                const std::string& getLibrary() const { return library; }

                /**
                 * Get the name of the header file.
                 * @return Name of the header file.
                 */
                const std::string& getHeader() const { return header; }

                /**
                 * Get the header file name without the extension.
                 * @return The name of the header file without any extension.
                 */
                const std::string& getHeaderStub() const { return headerStub; }

                /**
                 * Get the header path.
                 * @return The pathe element of the header.
                 */
                const std::string& getHeaderPath() const { return headerPath; }

                /**
                 * Get the name of the class.
                 * @return The class name if specified.
                 */
                const std::string& getClass() const { return className; }
                
                /**
                 * Returns the implementation scope
                 */
                 Scope getScope() { return scope; }
                
                /**
                 * Returns the implementation instance (to be used if the scope is set to composite)
                 */
                 void* getStaticImplementation() { return staticImpl; }
                
                /**
                 * Sets the implementation instance (to be used if the scope is set to composite)
                 */
                 void setStaticImplementation(void* staticImpl);
                
            private:
                
                /**
                 * Name of the shared library.
                 */
                std::string library;

                /**
                 * Name of the header file describing the interface.
                 */
                std::string header;

                /**
                 * Name of the header file without the extension.
                 */
                std::string headerStub;

                /**
                 * Path element of the header.
                 */
                std::string headerPath;

                /**
                 * Name of the class in the header file declaring the implementation.
                 * May be an empty string if not set in the SCDL file.
                 */
                std::string className;
                
                /**
                 * Scope of the implementation
                 */
                Scope scope;
    
                /**
                 * Holds the implementation instance if the scope is set to composite.
                 */
                void* staticImpl;
            };
            
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_model_cppimplementation_h
