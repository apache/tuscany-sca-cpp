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

#include "tuscany/sca/python/PythonExtension.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/python/PythonImplementationExtension.h"
#include "tuscany/sca/python/PythonInterfaceExtension.h"


extern "C"
{
    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
        void tuscany_sca_extension_initialize()
    {
        tuscany::sca::python::PythonExtension::initialize();
    }
}

namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            // ===================================================================
            // Constructor for the PythonExtension class. 
            // ===================================================================
            PythonExtension::PythonExtension()
            { 
                LOGENTRY(1, "PythonExtension::constructor");     
                LOGEXIT(1, "PythonExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the PythonExtension class.
            // ===================================================================
            PythonExtension::~PythonExtension()
            { 
                LOGENTRY(1, "PythonExtension::destructor");;           
                LOGEXIT(1, "PythonExtension::destructor");
            }

            void PythonExtension::initialize()
            { 
                LOGENTRY(1, "PythonExtension::initialize");;           
                SCARuntime::getInstance()->registerImplementationExtension(new PythonImplementationExtension());
                SCARuntime::getInstance()->registerInterfaceExtension(new PythonInterfaceExtension());
                LOGEXIT(1, "PythonExtension::initialize");;           
            }

        } // End namespace python
    } // End namespace sca
} // End namespace tuscany
