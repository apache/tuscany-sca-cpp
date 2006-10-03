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

#ifndef osoa_sca_serviceruntimeexception_h
#define osoa_sca_serviceruntimeexception_h

#include "tuscany/sca/python/export.h"

#include "tuscany/sca/util/Exceptions.h"
using tuscany::sca::TuscanyRuntimeException;
namespace osoa
{
    namespace sca
    {
        /**
         * Top level exception to represent all the exceptions that may be 
         * thrown by an SCA runtime implementation.
         */
        class ServiceRuntimeException : public TuscanyRuntimeException
        {
        public:
            ServiceRuntimeException(
                const char *name="ServiceRuntimeException",
                severity_level sev=Severe,
                const char* msg_text="")
                : TuscanyRuntimeException(name, sev, msg_text)
            {
            }
        }; // End ServiceRuntimeException class definition
        

        /**
         * A remotable service is currently unavailable. It is possible that a retry
         * may resolve this exception.
         */
        class ServiceUnavailableException: public ServiceRuntimeException
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
        class ServiceNotFoundException: public ServiceRuntimeException
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
        class ComponentContextException: public ServiceRuntimeException
        {
        public:
            ComponentContextException(const char* msg)
                : ServiceRuntimeException("ComponentContextException", Error,
                msg)
            {
            }
        private:
        }; // End ComponentContextException class definition
        
        /**
         * There is an error whilst trying to invoke a component.
         */
        class ComponentInvocationException: public ServiceRuntimeException
        {
        public:
            ComponentInvocationException(const char* msg)
                : ServiceRuntimeException("ComponentInvocationException", Error,
                msg)
            {
            }
        private:
        }; // End ComponentInvocationException class definition
        
        
    } // End namespace sca
} // End namespace osoa

#endif // osoa_sca_serviceruntimeexception_h
