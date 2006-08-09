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

#include "WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper* WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper(target);
    }
}

WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper::WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper(Service* target) : ComponentServiceWrapper(target)
{
    impl = (WSCompositeServiceTypeTestRPCImpl*)getImplementation();
}

WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper::~WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper()
{
    releaseImplementation();
}

void* WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper::newImplementation()
{
    return new WSCompositeServiceTypeTestRPCImpl;
}

void WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper::deleteImplementation()
{
    delete impl;
}

void WSCompositeServiceTypeTestRPCImpl_WSCompositeServiceTypeTestRPCService_Wrapper::invokeService(Operation& operation)
{
    const string& operationName = operation.getName();

    if (operationName == "doString")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        *(char**)operation.getReturnValue() = impl->doString(p0);
        return;
    }
        

    throw ServiceRuntimeException("Invalid operation");
    
}

