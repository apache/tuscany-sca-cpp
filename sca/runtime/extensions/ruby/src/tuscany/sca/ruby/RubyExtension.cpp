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

#include "tuscany/sca/ruby/RubyExtension.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/ruby/RubyImplementationExtension.h"


extern "C"
{
    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
        void tuscany_sca_extension_initialize()
    {
        tuscany::sca::ruby::RubyExtension::initialize();
    }
}

namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            // ===================================================================
            // Constructor for the RubyExtension class. 
            // ===================================================================
            RubyExtension::RubyExtension()
            { 
                LOGENTRY(1, "RubyExtension::constructor");     
                LOGEXIT(1, "RubyExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the RubyExtension class.
            // ===================================================================
            RubyExtension::~RubyExtension()
            { 
                LOGENTRY(1, "RubyExtension::destructor");;           
                LOGEXIT(1, "RubyExtension::destructor");
            }

            void RubyExtension::initialize()
            { 
                LOGENTRY(1, "RubyExtension::initialize");;           
                SCARuntime::getInstance()->registerImplementationExtension(new RubyImplementationExtension());
                LOGEXIT(1, "RubyExtension::initialize");;           
            }

        } // End namespace ruby
    } // End namespace sca
} // End namespace tuscany
