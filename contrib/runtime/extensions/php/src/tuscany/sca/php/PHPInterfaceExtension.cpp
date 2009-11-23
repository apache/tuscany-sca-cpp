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

#include "tuscany/sca/php/PHPInterfaceExtension.h"
#include "tuscany/sca/php/model/PHPInterface.h"
#include "tuscany/sca/util/Logging.h"

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace php
        {
            // ===================================================================
            // Constructor for the PHPInterfaceExtension class. 
            // ===================================================================
            PHPInterfaceExtension::PHPInterfaceExtension()
            { 
                logentry();
            }
            
            // ===================================================================
            // Destructor for the PHPInterfaceExtension class.
            // ===================================================================
            PHPInterfaceExtension::~PHPInterfaceExtension()
            { 
                logentry();
            }

            const string PHPInterfaceExtension::extensionName("php");
            const string PHPInterfaceExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#PHPInterface");

            // ===================================================================
            // loadModelElement - load the info from interface.php 
            // ===================================================================
            tuscany::sca::model::Interface* PHPInterfaceExtension::getInterface(Composite* composite, DataObjectPtr scdlInterface)
            {
                logentry();

                // Determine the type
                string ifType = scdlInterface->getType().getName();
                if (ifType == "PHPInterface")
                {
                    bool remotable = scdlInterface->getBoolean("remotable");                    
                    bool conversational = scdlInterface->getBoolean("conversational");                    

                    return new PHPInterface(remotable, conversational);
                }
                return 0;
            }

        } // End namespace php
    } // End namespace sca
} // End namespace tuscany
