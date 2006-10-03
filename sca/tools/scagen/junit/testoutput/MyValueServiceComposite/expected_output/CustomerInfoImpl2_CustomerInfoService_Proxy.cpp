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

#include "CustomerInfoImpl2_CustomerInfoService_Proxy.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CustomerInfoImpl2_CustomerInfoService_Proxy* CustomerInfoImpl2_CustomerInfoService_Proxy_Factory(ServiceWrapper* target)
    {
        return new CustomerInfoImpl2_CustomerInfoService_Proxy(target);
    }

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    void CustomerInfoImpl2_CustomerInfoService_Proxy_Destructor(void* proxy)
    {
        delete (CustomerInfoImpl2_CustomerInfoService_Proxy*)proxy;
    }
}

CustomerInfoImpl2_CustomerInfoService_Proxy::CustomerInfoImpl2_CustomerInfoService_Proxy(ServiceWrapper* targ) : target(targ)
{
}

CustomerInfoImpl2_CustomerInfoService_Proxy::~CustomerInfoImpl2_CustomerInfoService_Proxy()
{
    if (target)
        delete target;
}

const char* CustomerInfoImpl2_CustomerInfoService_Proxy::getCustomerInformation(const char* arg0)
{
    Operation operation("getCustomerInformation");
    operation.addParameter(&arg0);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return ret;
}


