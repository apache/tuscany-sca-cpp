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

#include "CPPInterfaceExtension.h"
#include "CPPInterface.h"
#include "tuscany/sca/util/Logging.h"


namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            // ===================================================================
            // Constructor for the CPPInterfaceExtension class. 
            // ===================================================================
            CPPInterfaceExtension::CPPInterfaceExtension()
            { 
                LOGENTRY(1, "CPPInterfaceExtension::constructor");     
                LOGEXIT(1, "CPPInterfaceExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the CPPInterfaceExtension class.
            // ===================================================================
            CPPInterfaceExtension::~CPPInterfaceExtension()
            { 
                LOGENTRY(1, "CPPInterfaceExtension::destructor");;           
                LOGEXIT(1, "CPPInterfaceExtension::destructor");
            }

            const string CPPInterfaceExtension::extensionName("cpp");
            const string CPPInterfaceExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#CPPInterface");

            // ===================================================================
            // loadModelElement - load the info from interface.cpp 
            // ===================================================================
            tuscany::sca::model::Interface* CPPInterfaceExtension::getInterface(Composite* composite, DataObjectPtr scdlInterface)
            {
                // Determine the type
                string ifType = scdlInterface->getType().getName();
                if (ifType == "CPPInterface")
                {
                    string header = scdlInterface->getCString("header");
                    string className = scdlInterface->getCString("class");
                    string scope = scdlInterface->getCString("scope");
                    bool remotable = scdlInterface->getBoolean("remotable");
                    
                    return new CPPInterface(header, className, scope, remotable);
                }
                return 0;
            }

        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
