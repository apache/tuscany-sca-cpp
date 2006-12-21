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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/cpp/model/CPPReferenceBinding.h"
#include "tuscany/sca/cpp/CPPServiceProxy.h"

using namespace tuscany::sca::model;


namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {

            // Constructor
            CPPReferenceBinding::CPPReferenceBinding(Reference* reference)
                : ReferenceBinding(reference, ""), serviceProxy(NULL), targetServiceBinding(NULL)
            {
            }
            
            // Destructor
            CPPReferenceBinding::~CPPReferenceBinding()
            {
            }
            
            ServiceProxy* CPPReferenceBinding::getServiceProxy()
            {
                return serviceProxy;
            }
            
            void CPPReferenceBinding::configure(ServiceBinding* binding)
            {
                targetServiceBinding = binding;
                
                serviceProxy = new CPPServiceProxy(getReference());
            }
                
        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
