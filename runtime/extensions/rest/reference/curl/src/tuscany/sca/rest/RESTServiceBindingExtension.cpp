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

#include "RESTServiceBindingExtension.h"
#include "model/RESTServiceBinding.h"
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
        tuscany::sca::rest::RESTServiceBindingExtension::initialize();
    }
}


namespace tuscany
{
    namespace sca
    {
        namespace rest
        {
            // ===================================================================
            // Constructor for the RESTServiceBinding class. 
            // ===================================================================
            RESTServiceBindingExtension::RESTServiceBindingExtension()
            { 
                logentry();
            }
            
            // ===================================================================
            // Destructor for the RESTServiceBindingExtension class.
            // ===================================================================
            RESTServiceBindingExtension::~RESTServiceBindingExtension()
            { 
                logentry();
            }

            const string RESTServiceBindingExtension::extensionName("rest");
            const string RESTServiceBindingExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#RESTBinding");
                                                                                                    

            // ===================================================================
            // loadModelElement - load the info from binding.rest
            // ===================================================================
            ServiceBinding* RESTServiceBindingExtension::getServiceBinding(Composite *composite, Service* service, DataObjectPtr scdlBinding)
            {
                string bindingType = scdlBinding->getType().getName();
                if (bindingType == "RESTBinding")
                {
                    string uri = scdlBinding->getCString("uri");

                    RESTServiceBinding* serviceBinding = new RESTServiceBinding(service, uri);
                    
                    return serviceBinding;
                }
                else
                {
                    return NULL;
                }
            }

            void RESTServiceBindingExtension::initialize()
            { 
                logentry();
                SCARuntime::getInstance()->registerServiceBindingExtension(new RESTServiceBindingExtension());
            }

        } // End namespace rest
    } // End namespace sca
} // End namespace tuscany
