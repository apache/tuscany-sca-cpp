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

#include "tuscany/sca/cpp/CPPExtension.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/cpp/CPPImplementationExtension.h"
#include "tuscany/sca/cpp/CPPInterfaceExtension.h"


extern "C"
{
    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
        void tuscany_sca_extension_initialize()
    {
        tuscany::sca::cpp::CPPExtension::initialize();
    }
}

namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
            // ===================================================================
            // Constructor for the CPPExtension class. 
            // ===================================================================
            CPPExtension::CPPExtension()
            { 
                logentry();
            }
            
            // ===================================================================
            // Destructor for the CPPExtension class.
            // ===================================================================
            CPPExtension::~CPPExtension()
            { 
                logentry();
            }

            void CPPExtension::initialize()
            { 
                logentry();
                SCARuntime::getInstance()->registerImplementationExtension(new CPPImplementationExtension());
                SCARuntime::getInstance()->registerInterfaceExtension(new CPPInterfaceExtension());
            }

        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
