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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#include "AccountDataServiceImpl_AccountDataService_Wrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    AccountDataServiceImpl_AccountDataService_Wrapper* AccountDataServiceImpl_AccountDataService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new AccountDataServiceImpl_AccountDataService_Wrapper(target);
    }
}

AccountDataServiceImpl_AccountDataService_Wrapper::AccountDataServiceImpl_AccountDataService_Wrapper(Service* target) : ComponentServiceWrapper(target)
{
    impl = (com::bigbank::account::AccountDataServiceImpl*)getImplementation();
}

AccountDataServiceImpl_AccountDataService_Wrapper::~AccountDataServiceImpl_AccountDataService_Wrapper()
{
    releaseImplementation();
}

void* AccountDataServiceImpl_AccountDataService_Wrapper::newImplementation()
{
    return new com::bigbank::account::AccountDataServiceImpl;
}

void AccountDataServiceImpl_AccountDataService_Wrapper::deleteImplementation()
{
    delete impl;
}

void AccountDataServiceImpl_AccountDataService_Wrapper::invokeService(Operation& operation)
{
    const string& operationName = operation.getName();

    if (operationName == "getCheckingAccount")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);
        *(commonj::sdo::DataObjectPtr*)operation.getReturnValue() = impl->getCheckingAccount(p0);
        return;
    }
    if (operationName == "getSavingsAccount")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);
        *(commonj::sdo::DataObjectPtr*)operation.getReturnValue() = impl->getSavingsAccount(p0);
        return;
    }
    if (operationName == "getStockAccount")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);
        *(commonj::sdo::DataObjectPtr*)operation.getReturnValue() = impl->getStockAccount(p0);
        return;
    }
        

    throw ServiceRuntimeException("Invalid operation");
    
}

