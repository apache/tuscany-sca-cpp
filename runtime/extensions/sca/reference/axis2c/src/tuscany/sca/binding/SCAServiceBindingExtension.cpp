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

#include "SCAServiceBindingExtension.h"
#include "model/SCAServiceBinding.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/core/SCARuntime.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::model;

extern "C"
{
#if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
#endif
        void tuscany_sca_extension_initialize()
    {
        tuscany::sca::binding::SCAServiceBindingExtension::initialize();
    }
}


namespace tuscany
{
    namespace sca
    {
        namespace binding
        {
            // ===================================================================
            // Constructor for the SCAServiceBinding class. 
            // ===================================================================
            SCAServiceBindingExtension::SCAServiceBindingExtension()
            { 
                logentry();
            }
            
            // ===================================================================
            // Destructor for the SCAServiceBindingExtension class.
            // ===================================================================
            SCAServiceBindingExtension::~SCAServiceBindingExtension()
            { 
                logentry();
            }

            const string SCAServiceBindingExtension::extensionName("sca");
            const string SCAServiceBindingExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#SCABinding");
                                                                                                    

            // ===================================================================
            // loadModelElement - load the info from binding.ws
            // ===================================================================
            ServiceBinding* SCAServiceBindingExtension::getServiceBinding(Composite *composite, Service* service, DataObjectPtr scdlBinding)
            {
                logentry();

                string bindingType = scdlBinding->getType().getName();
                if (bindingType == "SCABinding")
                {
                    string uri = scdlBinding->getCString("uri");

                    SCAServiceBinding* serviceBinding = new SCAServiceBinding(service, uri);
                    
                    return serviceBinding;
                }
                else
                {
                    return NULL;
                }
            }

            void SCAServiceBindingExtension::initialize()
            { 
                logentry();
                SCARuntime::getCurrentRuntime()->registerServiceBindingExtension(new SCAServiceBindingExtension());
            }

        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany
