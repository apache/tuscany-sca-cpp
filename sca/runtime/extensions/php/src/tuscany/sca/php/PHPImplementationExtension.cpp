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

#include "tuscany/sca/php/PHPImplementationExtension.h"
#include "tuscany/sca/php/model/PHPImplementation.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"


namespace tuscany
{
    namespace sca
    {
        namespace php
        {
            // ===================================================================
            // Constructor for the PHPImplementationExtension class. 
            // ===================================================================
            PHPImplementationExtension::PHPImplementationExtension()
            { 
                LOGENTRY(1, "PHPImplementationExtension::constructor");     
                LOGEXIT(1, "PHPImplementationExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the PHPImplementationExtension class.
            // ===================================================================
            PHPImplementationExtension::~PHPImplementationExtension()
            { 
                LOGENTRY(1, "PHPImplementationExtension::destructor");;           
                LOGEXIT(1, "PHPImplementationExtension::destructor");
            }

            const string PHPImplementationExtension::extensionName("php");
            const string PHPImplementationExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#PHPImplementation");

            // ===================================================================
            // loadModelElement - load the info from implementation.php 
            // ===================================================================
            ComponentType* PHPImplementationExtension::getImplementation(Composite *composite, DataObjectPtr scdlImplementation)
            {
                string implType = scdlImplementation->getType().getName();
                if (implType == "PHPImplementation")
                {
                    string module = scdlImplementation->getCString("module");
                    string className = scdlImplementation->getCString("class");
                    
                    PHPImplementation* phpImpl = new PHPImplementation(composite, module, className);
                    
                    return phpImpl;
                }
                else
                {
                    return NULL;
                }
            }

        } // End namespace php
    } // End namespace sca
} // End namespace tuscany
