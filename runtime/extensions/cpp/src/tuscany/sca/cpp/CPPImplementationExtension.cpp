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

#include "tuscany/sca/cpp/CPPImplementationExtension.h"
#include "tuscany/sca/cpp/model/CPPImplementation.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::util;
using namespace tuscany::sca::model;


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
                logentry();
            }
            
            // ===================================================================
            // Destructor for the CPPImplementationExtension class.
            // ===================================================================
            CPPImplementationExtension::~CPPImplementationExtension()
            { 
                logentry();
            }

            const string CPPImplementationExtension::extensionName("cpp");
            const string CPPImplementationExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#CPPImplementation");

            // ===================================================================
            // loadModelElement - load the info from implementation.cpp 
            // ===================================================================
            ComponentType* CPPImplementationExtension::getImplementation(Composite *composite, DataObjectPtr scdlImplementation)
            {
                logentry();
                string implType = scdlImplementation->getType().getName();
                if (implType == "CPPImplementation")
                {
                    string library = scdlImplementation->getCString("library");
                    string header = scdlImplementation->getCString("header");
                    string className = scdlImplementation->getCString("class");
                    string scopeName = scdlImplementation->getCString("scope");

                    CPPImplementation::Scope scope;
                    if (scopeName == "composite")
                    {
                        scope = CPPImplementation::COMPOSITE;
                    }
                    else
                    {
                        scope = CPPImplementation::STATELESS;
                    }

                    string headerPath;
                    string headerStub;

                    // Separate any path element
                    Utils::rTokeniseString("/", header, headerPath, headerStub);
                    if (headerPath != "")
                    {
                        headerPath += "/";
                    }
                    
                    // Determine the header stub name
                    string tmp;             
                    Utils::rTokeniseString(".h", headerStub, headerStub, tmp);
                    
                    CPPImplementation* cppImpl = new CPPImplementation(
                                    composite, library, header, headerPath, headerStub, className, scope);
                    
                    return cppImpl;
                }
                else
                {
                    return NULL;
                }
            }

        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
