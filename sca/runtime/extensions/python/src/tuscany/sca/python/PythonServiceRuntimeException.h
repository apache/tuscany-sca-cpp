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

#ifndef tuscany_sca_python_pythonserviceruntimeexception_h
#define tuscany_sca_python_pythonserviceruntimeexception_h

#include "export.h"

#include "tuscany/sca/util/Exceptions.h"
using tuscany::sca::TuscanyRuntimeException;
namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            /**
            * Exception to represent a failure in a Python call.
            */
            class SCA_PYTHON_API PythonServiceRuntimeException : public TuscanyRuntimeException
            {
            public:
                PythonServiceRuntimeException(
                    const char *name="PythonServiceRuntimeException",
                    severity_level sev=Severe,
                    const char* msg_text="")
                    : TuscanyRuntimeException(name, sev, msg_text)
                {
                }
            }; // End PythonServiceRuntimeException class definition        
        
            /**
            * The target of a wire cannot be found, or the reference has not been
            * configured.
            */
            class SCA_PYTHON_API PythonServiceNotFoundException: public PythonServiceRuntimeException
            {
            public:
                PythonServiceNotFoundException(const char* msg)
                    : PythonServiceRuntimeException("PythonServiceNotFoundException", Error,
                    msg)
                {
                }
            private:
            }; // End PythonServiceNotFoundException class definition


            /**
            * There is no current component (for example, if a non-SCA component
            * tries to get the current ComponentContext).
            */
            class SCA_PYTHON_API PythonComponentContextException: public PythonServiceRuntimeException
            {
            public:
                PythonComponentContextException(const char* msg)
                    : PythonServiceRuntimeException("PythonComponentContextException", Error,
                    msg)
                {
                }
            private:
            }; // End PythonComponentContextException class definition

            /**
            * There is no current component (for example, if a non-SCA component
            * tries to get the current ComponentContext).
            */
            class SCA_PYTHON_API PythonComponentInvocationException: public PythonServiceRuntimeException
            {
            public:
                PythonComponentInvocationException(const char* msg)
                    : PythonServiceRuntimeException("ComponentInvocationException", Error,
                    msg)
                {
                }
            private:
            }; // End PythonComponentInvocationException class definition

        } // End namespace python
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_python_pythonserviceruntimeexception_h
