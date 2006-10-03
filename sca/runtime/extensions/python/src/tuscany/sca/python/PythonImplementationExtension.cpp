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

#include "tuscany/sca/python/PythonImplementationExtension.h"
#include "tuscany/sca/python/model/PythonImplementation.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"


namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            // ===================================================================
            // Constructor for the PythonImplementationExtension class. 
            // ===================================================================
            PythonImplementationExtension::PythonImplementationExtension()
            { 
                LOGENTRY(1, "PythonImplementationExtension::constructor");     
                LOGEXIT(1, "PythonImplementationExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the PythonImplementationExtension class.
            // ===================================================================
            PythonImplementationExtension::~PythonImplementationExtension()
            { 
                LOGENTRY(1, "PythonImplementationExtension::destructor");;           
                LOGEXIT(1, "PythonImplementationExtension::destructor");
            }

            const string PythonImplementationExtension::extensionName("python");
            const string PythonImplementationExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#PythonImplementation");

            // ===================================================================
            // loadModelElement - load the info from implementation.python 
            // ===================================================================
            ComponentType* PythonImplementationExtension::getImplementation(Composite *composite, DataObjectPtr scdlImplementation)
            {
                string implType = scdlImplementation->getType().getName();
                if (implType == "PythonImplementation")
                {
                    string module = scdlImplementation->getCString("module");
                    string path = scdlImplementation->getCString("path");
                    string className = scdlImplementation->getCString("class");
                    string scopeName = scdlImplementation->getCString("scope");
                    
                    PythonImplementation::Scope scope;
                    if (scopeName == "composite")
                    {
                        scope = PythonImplementation::COMPOSITE;
                    }
                    else
                    {
                        scope = PythonImplementation::STATELESS;
                    }

                    PythonImplementation* pythonImpl = new PythonImplementation(composite, module, path, className, scope);
                    
                    return pythonImpl;
                }
                else
                {
                    return NULL;
                }
            }

        } // End namespace python
    } // End namespace sca
} // End namespace tuscany
