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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/php/model/PHPServiceBinding.h"
#include "tuscany/sca/php/PHPServiceWrapper.h"

namespace tuscany
{
    namespace sca
    {
        namespace php
        {

            // Constructor
            PHPServiceBinding::PHPServiceBinding(Service* service)
                : ServiceBinding(service, "")
            {
                LOGENTRY(1,"PHPServiceBinding::constructor");
                serviceWrapper = PHPServiceWrapper::getServiceWrapper(service);
                LOGEXIT(1,"PHPServiceBinding::constructor");
            }

            // Destructor
            PHPServiceBinding::~PHPServiceBinding()
            {
                LOGENTRY(1,"PHPServiceBinding::destructor");
                LOGEXIT(1,"PHPServiceBinding::destructor");
            }
            
            ServiceWrapper* PHPServiceBinding::getServiceWrapper()
            {
                LOGENTRY(1,"PHPServiceBinding::getServiceWrapper");
                LOGEXIT(1,"PHPServiceBinding::getServiceWrapper");
                return (ServiceWrapper*)serviceWrapper;
            }
                
        } // End namespace php
    } // End namespace sca
} // End namespace tuscany
