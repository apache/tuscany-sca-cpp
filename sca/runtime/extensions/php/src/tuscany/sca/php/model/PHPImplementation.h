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

#ifndef tuscany_sca_php_model_phpimplementation_h
#define tuscany_sca_php_model_phpimplementation_h
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
        namespace php
        {
            /**
             * Holds information about an SCA implementation written in PHP
             */
            class PHPImplementation : public ComponentType
            {
                
            public:
                /**
                 * Constructor.
                 * @param module Name of the module.
                 * @param modulePath Path to the module (could be a blank string
                 * if this is not specified).
                 * @param className Name of the class in the module (could be a blank string
                 * if this is not specified).
                 */
                PHPImplementation(const string& module, const string& className);
                
                /**
                 * Destructor
                 */
                virtual ~PHPImplementation();

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
                 * Get the name of the class.
                 * @return The class name if specified.
                 */
                const string& getClass() const { return className; }
                
            private:
                
                /**
                 * Name of the module.
                 */
                string module;

                /**
                 * Name of the class in the header file declaring the implementation.
                 * May be an empty string if not set in the SCDL file.
                 */
                string className;
            };
            
        } // End namespace php
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_php_model_phpimplementation_h
