/*
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

#include "CustomerInfoImpl_CustomerInfoService_Proxy.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CustomerInfoImpl_CustomerInfoService_Proxy* CustomerInfoImpl_CustomerInfoService_Proxy_Factory(ServiceWrapper* target)
    {
        return new CustomerInfoImpl_CustomerInfoService_Proxy(target);
    }

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    void CustomerInfoImpl_CustomerInfoService_Proxy_Destructor(void* proxy)
    {
        delete (CustomerInfoImpl_CustomerInfoService_Proxy*)proxy;
    }
}

CustomerInfoImpl_CustomerInfoService_Proxy::CustomerInfoImpl_CustomerInfoService_Proxy(ServiceWrapper* targ) : target(targ)
{
}

CustomerInfoImpl_CustomerInfoService_Proxy::~CustomerInfoImpl_CustomerInfoService_Proxy()
{
    if (target)
        delete target;
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationCharPublic( char* arg0, const char* arg1)
{
    Operation operation("getCustomerInformationCharPublic", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoACharPublic( char* arg0, const char* arg1)
{
    Operation operation("getCustomerInfoACharPublic", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBCharPublic( char* arg0,  char* arg1)
{
    Operation operation("getCustomerInfoBCharPublic", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}


