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

#include "tuscany/sca/rest/RESTInterfaceExtension.h"
#include "tuscany/sca/rest/model/RESTInterface.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::model;

extern "C"
{
#if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
#endif
        void tuscany_sca_rest_interface_initialize()
    {
        tuscany::sca::rest::RESTInterfaceExtension::initialize();
    }
}

namespace tuscany
{
    namespace sca
    {
        namespace rest
        {
            // ===================================================================
            // Constructor for the RESTInterfaceExtension class. 
            // ===================================================================
            RESTInterfaceExtension::RESTInterfaceExtension()
            { 
                logentry();
            }
            
            // ===================================================================
            // Destructor for the RESTInterfaceExtension class.
            // ===================================================================
            RESTInterfaceExtension::~RESTInterfaceExtension()
            { 
                logentry();
            }

            const string RESTInterfaceExtension::extensionName("rest");
            const string RESTInterfaceExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#RESTInterface");

            // ===================================================================
            // loadModelElement - load the info from interface.rest 
            // ===================================================================
            tuscany::sca::model::Interface* RESTInterfaceExtension::getInterface(Composite* composite, DataObjectPtr scdlInterface)
            {
                logentry();

                // Determine the type
                string ifType = scdlInterface->getType().getName();
                if (ifType == "RESTInterface")
                {
                    return new RESTInterface();
                }
                return 0;
            }

           void RESTInterfaceExtension::initialize()
            { 
                logentry();
                SCARuntime::getCurrentRuntime()->registerInterfaceExtension(new RESTInterfaceExtension());
            }

        } // End namespace rest
    } // End namespace sca
} // End namespace tuscany
