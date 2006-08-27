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

#include "CPPImplementationExtension.h"
#include "ImplementationCPP.h"
#include "tuscany/sca/util/Logging.h"


namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            // ===================================================================
            // Constructor for the CPPImplementationExtension class. 
            // ===================================================================
            CPPImplementationExtension::CPPImplementationExtension()
            { 
                LOGENTRY(1, "CPPImplementationExtension::constructor");     
                LOGEXIT(1, "CPPImplementationExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the CPPImplementationExtension class.
            // ===================================================================
            CPPImplementationExtension::~CPPImplementationExtension()
            { 
                LOGENTRY(1, "CPPImplementationExtension::destructor");;           
                LOGEXIT(1, "CPPImplementationExtension::destructor");
            }

            const string CPPImplementationExtension::extensionName("cpp");
            const string CPPImplementationExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#CPPImplementation");

            // ===================================================================
            // loadModelElement - load the info from implementation.cpp 
            // ===================================================================
            void CPPImplementationExtension::loadModelElement(commonj::sdo::DataObjectPtr scdlImplementation, 
                                                              model::Component* component)
            {
                string implType = scdlImplementation->getType().getName();
                if (implType == "CPPImplementation")
                {
                    string library = scdlImplementation->getCString("library");
                    string header = scdlImplementation->getCString("header");
                    string className = scdlImplementation->getCString("class");
                    // The following works for now ONLY bacause ImplementationCPP is identical 
                    // to tusscany::sca::model::CPPIplementation
                    ImplementationCPP* cppImpl = new ImplementationCPP(library, header, className);
                    component->setImplementation(cppImpl);
                }
            }

        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
