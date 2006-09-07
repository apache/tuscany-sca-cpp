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

#include "tuscany/sca/php/PHPExtension.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/php/PHPImplementationExtension.h"


extern "C"
{
    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
        void tuscany_sca_extension_initialize()
    {
        tuscany::sca::php::PHPExtension::initialize();
    }
}

namespace tuscany
{
    namespace sca
    {
        namespace php
        {
            // ===================================================================
            // Constructor for the PHPExtension class. 
            // ===================================================================
            PHPExtension::PHPExtension()
            { 
                LOGENTRY(1, "PHPExtension::constructor");     
                LOGEXIT(1, "PHPExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the PHPExtension class.
            // ===================================================================
            PHPExtension::~PHPExtension()
            { 
                LOGENTRY(1, "PHPExtension::destructor");;           
                LOGEXIT(1, "PHPExtension::destructor");
            }

            void PHPExtension::initialize()
            { 
                LOGENTRY(1, "PHPExtension::initialize");;           
                SCARuntime::getInstance()->registerImplementationExtension(new PHPImplementationExtension());
                //SCARuntime::getInstance()->registerInterfaceExtension(new PHPInterfaceExtension());
                LOGEXIT(1, "PHPExtension::initialize");;           
            }

        } // End namespace php
    } // End namespace sca
} // End namespace tuscany
