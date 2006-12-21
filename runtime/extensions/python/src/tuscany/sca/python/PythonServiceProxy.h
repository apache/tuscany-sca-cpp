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

#ifndef tuscany_sca_python_pythonserviceproxy_h
#define tuscany_sca_python_pythonserviceproxy_h

#include "export.h" 
#include "tuscany/sca/core/ServiceProxy.h" 
#include "tuscany/sca/core/ServiceWrapper.h" 
#include "tuscany/sca/util/Library.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/Service.h"


namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            
            /**
             * Holds a proxy for a given reference from a component implemented in Python.
             */
            class PythonServiceProxy : public ServiceProxy
            {
            public:
                /**
                 * Create a new service proxy for a reference. The proxy will contain a pointer to
                 * the target ServiceWrapper.
                 * @param reference The reference on the source component.
                 */
                PythonServiceProxy(tuscany::sca::model::Reference* reference);
       
               /**
                 * Create a new service proxy for a service. The proxy will contain a pointer to
                 * the target ServiceWrapper.
                 * @param service The service on the target component.
                 */
                SCA_PYTHON_API PythonServiceProxy(tuscany::sca::model::Service* service);
       
                /**
                 * Destructor.
                 */
                virtual ~PythonServiceProxy();
    
                /**
                 * Invoke the wired service.
                 */
                virtual void invokeService(Operation& operation);
    
            private:
    
                /**
                 * The target service wrapper
                 */
                ServiceWrapper* serviceWrapper;    
            };
            
        } // End namespace python
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_python_pythonserviceproxy_h
