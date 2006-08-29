/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#include "tuscany/sca/core/ServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"

namespace tuscany
{
    namespace sca
    {
        // ============================
        // Constructor: Create a proxy
        // ============================
        ServiceProxy::ServiceProxy(Reference* reference)
            : reference(reference)
        {
            LOGENTRY(1,"ServiceProxy::constructor");
            LOGEXIT(1,"ServiceProxy::constructor");
            
        }

        // ==========
        // Destructor
        // ==========
        ServiceProxy::~ServiceProxy()
        {
            LOGENTRY(1,"ServiceProxy::destructor");
            LOGEXIT(1,"ServiceProxy::destructor");
        }
        
    } // End namespace sca
} // End namespace tuscany
