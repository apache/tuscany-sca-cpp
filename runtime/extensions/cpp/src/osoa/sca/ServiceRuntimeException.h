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

#ifndef osoa_sca_serviceruntimeexception_h
#define osoa_sca_serviceruntimeexception_h

#include "osoa/sca/export.h"

#include "tuscany/sca/core/Exceptions.h"

namespace osoa
{
    namespace sca
    {
        /**
         * Top level exception to represent all the exceptions that may be 
         * thrown by an SCA runtime implementation.
         */
        class SCA_CPP_API ServiceRuntimeException : public tuscany::sca::TuscanyRuntimeException
        {
        public:
            ServiceRuntimeException(
                const char *name="ServiceRuntimeException",
                severity_level sev=Severe,
                const char* msg_text="")
                : tuscany::sca::TuscanyRuntimeException(name, sev, msg_text)
            {
            }

            ServiceRuntimeException(const tuscany::sca::TuscanyRuntimeException& e)
                : tuscany::sca::TuscanyRuntimeException(e)
            {
            }
        }; // End ServiceRuntimeException class definition

        /**
         * A remotable service is currently unavailable. It is possible that a retry
         * may resolve this exception.
         */
        class SCA_CPP_API ServiceUnavailableException: public ServiceRuntimeException
        {
        public:
            ServiceUnavailableException(const char* serviceName)
                : ServiceRuntimeException("ServiceUnavailableException", Warning,
                serviceName)
            {
            }
        private:
        }; // End ServiceUnavailableException class definition


        /**
         * The target of a wire cannot be found, or the reference has not been
         * configured.
         */
        class SCA_CPP_API ServiceNotFoundException: public ServiceRuntimeException
        {
        public:
            ServiceNotFoundException(const char* msg)
                : ServiceRuntimeException("ServiceNotFoundException", Error,
                msg)
            {
            }
        private:
        }; // End ServiceNotFoundException class definition


        /**
         * There is no current component (for example, if a non-SCA component
         * tries to get the current ComponentContext).
         */
        class SCA_CPP_API ComponentContextException: public ServiceRuntimeException
        {
        public:
            ComponentContextException(const char* msg)
                : ServiceRuntimeException("ComponentContextException", Error,
                msg)
            {
            }
        private:
        }; // End ComponentContextException class definition
        
        
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_serviceruntimeexception_h
