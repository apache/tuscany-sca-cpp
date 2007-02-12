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

using namespace std;
using namespace tuscany::sca::model;
using namespace commonj::sdo;

extern "C"
{
#if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
#endif
        void tuscany_sca_ws_service_initialize()
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
                logentry();
            }
            
            // ===================================================================
            // Destructor for the WSReferenceBindingExtension class.
            // ===================================================================
            WSReferenceBindingExtension::~WSReferenceBindingExtension()
            { 
                logentry();
            }

            const string WSReferenceBindingExtension::extensionName("ws");
            const string WSReferenceBindingExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#WebServiceBinding");

            // ===================================================================
            // loadModelElement - load the info from binding.ws
            // ===================================================================
            ReferenceBinding* WSReferenceBindingExtension::getReferenceBinding(Composite *composite, Reference* reference, DataObjectPtr scdlBinding)
            {
                logentry();

                string uri = scdlBinding->getCString("uri");
                
                string endpoint;
                try
                {
                    endpoint = scdlBinding->getCString("endpoint");
                }
                catch (SDORuntimeException&)
                {
                    endpoint = "";
                }
                
                string version;
                try
                {
                    commonj::sdo::DataObjectList& soap = scdlBinding->getList("soapbinding");
                    if (soap.size()!=0)
                    {
                        version = soap.getCString(0);
                    }
                    else
                    {
                        version = "";
                    }
                }
                catch (SDORuntimeException&)
                {
                    version = "";
                }

                WSReferenceBinding* serviceBinding = new WSReferenceBinding(reference, uri, endpoint, version);
                
                return serviceBinding;
            }

           void WSReferenceBindingExtension::initialize()
            { 
                logentry();
                SCARuntime::getCurrentRuntime()->registerReferenceBindingExtension(new WSReferenceBindingExtension());
            }

        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany
