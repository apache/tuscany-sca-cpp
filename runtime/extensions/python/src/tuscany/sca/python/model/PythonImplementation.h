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

#ifndef tuscany_sca_python_model_pythonimplementation_h
#define tuscany_sca_python_model_pythonimplementation_h
#include "tuscany/sca/model/ComponentType.h"

#include <map>
using std::map;
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
             * Holds information about an SCA implementation written in Python
             */
            class PythonImplementation : public ComponentType
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
                 * @param composite The composite containing this implementation.
                 * @param module Name of the module.
                 * @param modulePath Path to the module (could be a blank string
                 * if this is not specified).
                 * @param className Name of the class in the module (could be a blank string
                 * if this is not specified).
                 */
                PythonImplementation(Composite* composite, const string& module, const string& modulePath, const string& className, Scope scope);
                
                /**
                 * Destructor
                 */
                virtual ~PythonImplementation();

                /**
                 * Initialize a component of this type.
                 * @param component The component to initialize.
                 */
                virtual void initializeComponent(Component* component);

                /**
                 * Returns the name of the module.
                 * @return The name of the module.
                 */
                const string& getModule() const { return module; }

                /**
                 * Get the header path.
                 * @return The pathe element of the header.
                 */
                const string& getModulePath() const { return modulePath; }

                /**
                 * Get the name of the class.
                 * @return The class name if specified.
                 */
                const string& getClass() const { return className; }
                
                /**
                 * Returns the implementation scope
                 */
                 Scope getScope() const { return scope; }
                
            private:
                
                /**
                 * Name of the module.
                 */
                string module;

                /**
                 * Path to the module.
                 */
                string modulePath;

                /**
                 * Name of the class in the header file declaring the implementation.
                 * May be an empty string if not set in the SCDL file.
                 */
                string className;
                
                /**
                 * The implementation scope
                 */
                 Scope scope;
            };
            
        } // End namespace python
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_python_model_pythonimplementation_h
