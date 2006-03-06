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

#ifndef tuscany_sca_util_exceptions_h
#define tuscany_sca_util_exceptions_h

#include "osoa/sca/ServiceRuntimeException.h"
using osoa::sca::ServiceRuntimeException;

namespace tuscany
{
    namespace sca
    {
        /**
         * Indicates a problem in the consistency of the SCA model provided to the
         * Tuscany runtime.
         */
        class SystemConfigurationException: public ServiceRuntimeException
        {
        public:
            SystemConfigurationException(const char* msg)
                : ServiceRuntimeException("SystemConfigurationException", Severe,
                msg)
            {
            }
        private:
        };

        
    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_util_exceptions_h
