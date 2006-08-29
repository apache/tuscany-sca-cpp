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

#include "WSServiceBindingExtension.h"
#include "model/WSServiceBinding.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/core/SCARuntime.h"

extern "C"
{
    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
        void tuscany_sca_extension_initialize()
    {
        tuscany::sca::ws::WSServiceBindingExtension::initialize();
    }
}


namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
            // ===================================================================
            // Constructor for the WSServiceBinding class. 
            // ===================================================================
            WSServiceBindingExtension::WSServiceBindingExtension()
            { 
                LOGENTRY(1, "WSServiceBindingExtension::constructor");     
                LOGEXIT(1, "WSServiceBindingExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the WSServiceBindingExtension class.
            // ===================================================================
            WSServiceBindingExtension::~WSServiceBindingExtension()
            { 
                LOGENTRY(1, "WSServiceBindingExtension::destructor");;           
                LOGEXIT(1, "WSServiceBindingExtension::destructor");
            }

            const string WSServiceBindingExtension::extensionName("ws");
            const string WSServiceBindingExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#WebServiceBinding");
                                                                                                    

            // ===================================================================
            // loadModelElement - load the info from binding.ws
            // ===================================================================
            ServiceBinding* WSServiceBindingExtension::getServiceBinding(Composite *composite, Service* service, DataObjectPtr scdlBinding)
            {
                string bindingType = scdlBinding->getType().getName();
                if (bindingType == "WebServiceBinding")
                {
                    string uri = scdlBinding->getCString("uri");
                    string port = scdlBinding->getCString("port");

                    WSServiceBinding* serviceBinding = new WSServiceBinding(service, uri, port);
                    
                    return serviceBinding;
                }
                else
                {
                    return NULL;
                }
            }

            void WSServiceBindingExtension::initialize()
            { 
                LOGENTRY(1, "WSServiceBindingExtension::initialize");;           
                SCARuntime::getInstance()->registerServiceBindingExtension(new WSServiceBindingExtension());
                LOGEXIT(1, "WSServiceBindingExtension::initialize");;           
            }

        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany
