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


#ifndef tuscany_sca_python_model_pythonimplementation_h
#define tuscany_sca_python_model_pythonimplementation_h

#include <string>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/model/ComponentType.h"


namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            /**
             * Holds information about an SCA implementation written in Python
             */
            class PythonImplementation : public tuscany::sca::model::ComponentType
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
                PythonImplementation(tuscany::sca::model::Composite* composite,
                    const std::string& module, const std::string& modulePath, const std::string& className,
                    Scope scope);
                
                /**
                 * Destructor
                 */
                virtual ~PythonImplementation();

                /**
                 * Initialize a component of this type.
                 * @param component The component to initialize.
                 */
                virtual void initializeComponent(tuscany::sca::model::Component* component);

                /**
                 * Override the ComponentType::findReferenceType method
                 * to allow Python components to be defined without requiring 
                 * a componentType side-file
                 */
                virtual tuscany::sca::model::ReferenceType* findReferenceType(const std::string& referenceName);

                /**
                 * Override the ComponentType::findPropertyType method
                 * to allow Python components to be defined without requiring 
                 * a componentType side-file
                 */
                virtual const commonj::sdo::Property* findPropertyType(const std::string& propertyName);


                /**
                 * Returns the name of the module.
                 * @return The name of the module.
                 */
                const std::string& getModule() const { return module; }

                /**
                 * Get the header path.
                 * @return The pathe element of the header.
                 */
                const std::string& getModulePath() const { return modulePath; }

                /**
                 * Get the name of the class.
                 * @return The class name if specified.
                 */
                const std::string& getClass() const { return className; }
                
                /**
                 * Returns the implementation scope
                 */
                 Scope getScope() const { return scope; }
                
            private:
                
                /**
                 * Name of the module.
                 */
                std::string module;

                /**
                 * Path to the module.
                 */
                std::string modulePath;

                /**
                 * Name of the class in the header file declaring the implementation.
                 * May be an empty string if not set in the SCDL file.
                 */
                std::string className;
                
                /**
                 * The implementation scope
                 */
                 Scope scope;
            };
            
        } // End namespace python
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_python_model_pythonimplementation_h
