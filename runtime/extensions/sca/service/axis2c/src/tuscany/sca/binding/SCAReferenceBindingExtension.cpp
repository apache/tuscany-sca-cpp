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


#include "SCAReferenceBindingExtension.h"
#include "model/SCAReferenceBinding.h"
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
        tuscany::sca::binding::SCAReferenceBindingExtension::initialize();
    }
}

namespace tuscany
{
    namespace sca
    {
        namespace binding
        {
            // ===================================================================
            // Constructor for the SCAReferenceBinding class. 
            // ===================================================================
            SCAReferenceBindingExtension::SCAReferenceBindingExtension()
            { 
                logentry();
            }
            
            // ===================================================================
            // Destructor for the SCAReferenceBindingExtension class.
            // ===================================================================
            SCAReferenceBindingExtension::~SCAReferenceBindingExtension()
            { 
                logentry();
            }

            const string SCAReferenceBindingExtension::extensionName("sca");
            const string SCAReferenceBindingExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#SCABinding");

            // ===================================================================
            // loadModelElement - load the info from binding.ws
            // ===================================================================
            ReferenceBinding* SCAReferenceBindingExtension::getReferenceBinding(Composite *composite, Reference* reference, DataObjectPtr scdlBinding)
            {
                logentry();

                string bindingType = scdlBinding->getType().getName();
                if (bindingType == "SCABinding")
                {
                    string uri = scdlBinding->getCString("uri");

                    SCAReferenceBinding* serviceBinding = new SCAReferenceBinding(reference, uri);
                    
                    return serviceBinding;
                }
                else
                {
                    return NULL;
                }
            }

           void SCAReferenceBindingExtension::initialize()
            { 
                logentry();
                SCARuntime::getInstance()->registerReferenceBindingExtension(new SCAReferenceBindingExtension());
            }

        } // End namespace binding
    } // End namespace sca
} // End namespace tuscany
