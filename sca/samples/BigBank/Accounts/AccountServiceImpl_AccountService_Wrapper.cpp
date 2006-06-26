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

#include "AccountServiceImpl_AccountService_Wrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    AccountServiceImpl_AccountService_Wrapper* AccountServiceImpl_AccountService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new AccountServiceImpl_AccountService_Wrapper(target);
    }
}

AccountServiceImpl_AccountService_Wrapper::AccountServiceImpl_AccountService_Wrapper(Service* target) : ComponentServiceWrapper(target)
{
    impl = (AccountServiceImpl*)getImplementation();
}

AccountServiceImpl_AccountService_Wrapper::~AccountServiceImpl_AccountService_Wrapper()
{
    releaseImplementation();
}

void* AccountServiceImpl_AccountService_Wrapper::newImplementation()
{
    return new AccountServiceImpl;
}

void AccountServiceImpl_AccountService_Wrapper::deleteImplementation()
{
    delete impl;
}

void AccountServiceImpl_AccountService_Wrapper::invokeService(Operation& operation)
{
    const string& operationName = operation.getName();

    if (operationName == "getAccountReport")
    {
        DataObjectPtr& p0 = *(DataObjectPtr*)operation.getParameterValue(0);        *(commonj::sdo::DataObjectPtr*)operation.getReturnValue() = impl->getAccountReport(p0);
        return;
    }
        

    throw ServiceRuntimeException("Invalid operation");
    
}

