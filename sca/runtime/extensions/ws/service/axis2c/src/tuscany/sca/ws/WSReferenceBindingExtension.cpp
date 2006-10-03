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


#include "WSReferenceBindingExtension.h"
#include "model/WSReferenceBinding.h"
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
        tuscany::sca::ws::WSReferenceBindingExtension::initialize();
    }
}

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
            // ===================================================================
            // Constructor for the WSReferenceBinding class. 
            // ===================================================================
            WSReferenceBindingExtension::WSReferenceBindingExtension()
            { 
                LOGENTRY(1, "WSReferenceBindingExtension::constructor");     
                LOGEXIT(1, "WSReferenceBindingExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the WSReferenceBindingExtension class.
            // ===================================================================
            WSReferenceBindingExtension::~WSReferenceBindingExtension()
            { 
                LOGENTRY(1, "WSReferenceBindingExtension::destructor");;           
                LOGEXIT(1, "WSReferenceBindingExtension::destructor");
            }

            const string WSReferenceBindingExtension::extensionName("ws");
            const string WSReferenceBindingExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#WebServiceBinding");

            // ===================================================================
            // loadModelElement - load the info from binding.ws
            // ===================================================================
            ReferenceBinding* WSReferenceBindingExtension::getReferenceBinding(Composite *composite, Reference* reference, DataObjectPtr scdlBinding)
            {
                string bindingType = scdlBinding->getType().getName();
                if (bindingType == "WebServiceBinding")
                {
                    string uri = scdlBinding->getCString("uri");
                    string endpoint = scdlBinding->getCString("endpoint");
                   commonj::sdo::DataObjectList& soap = scdlBinding->getList("soapbinding");
                    string version;
                    if (soap.size()!=0)
                    {
                        version = soap.getCString(0);
                    }
                    else
                    {
                        version = "";
                    }

                    WSReferenceBinding* serviceBinding = new WSReferenceBinding(reference, uri, endpoint, version);
                    
                    return serviceBinding;
                }
                else
                {
                    return NULL;
                }
            }

           void WSReferenceBindingExtension::initialize()
            { 
                LOGENTRY(1, "WSReferenceBindingExtension::initialize");;           
                SCARuntime::getInstance()->registerReferenceBindingExtension(new WSReferenceBindingExtension());
                LOGEXIT(1, "WSReferenceBindingExtension::initialize");;           
            }

        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany
