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

#include "WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;



extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper* WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper(target);
    }
}

WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper::WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper(Service* target) : ComponentServiceWrapper(target)
{
    impl = (WSCompositeServiceTypeTestImpl*)getImplementation();
}

WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper::~WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper()
{
    releaseImplementation();
}

void* WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper::newImplementation()
{
    return new WSCompositeServiceTypeTestImpl;
}

void WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper::deleteImplementation()
{
    delete impl;
}

void WSCompositeServiceTypeTestImpl_WSCompositeServiceTypeTestService_Wrapper::invokeService(Operation& operation)
{
    const string& operationName = operation.getName();

    if (operationName == "doChars")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        *(char**)operation.getReturnValue() = impl->doChars(p0);
        return;
    }
    if (operationName == "doLong")
    {
        long& p0 = *( long*)operation.getParameterValue(0);
        *(long*)operation.getReturnValue() = impl->doLong(p0);
        return;
    }
    if (operationName == "doInt")
    {
        int& p0 = *( int*)operation.getParameterValue(0);
        *(int*)operation.getReturnValue() = impl->doInt(p0);
        return;
    }
    if (operationName == "doFloat")
    {
        float& p0 = *( float*)operation.getParameterValue(0);
        *(float*)operation.getReturnValue() = impl->doFloat(p0);
        return;
    }
    if (operationName == "doLongDouble")
    {
        long double& p0 = *( long double*)operation.getParameterValue(0);
        *(long double*)operation.getReturnValue() = impl->doLongDouble(p0);
        return;
    }
    if (operationName == "doDouble")
    {
        double& p0 = *( double*)operation.getParameterValue(0);
        *(double*)operation.getReturnValue() = impl->doDouble(p0);
        return;
    }
    if (operationName == "doBool")
    {
        bool& p0 = *( bool*)operation.getParameterValue(0);
        *(bool*)operation.getReturnValue() = impl->doBool(p0);
        return;
    }
    if (operationName == "doShort")
    {
        short& p0 = *( short*)operation.getParameterValue(0);
        *(short*)operation.getReturnValue() = impl->doShort(p0);
        return;
    }
    if (operationName == "doBytes")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        *(char**)operation.getReturnValue() = impl->doBytes(p0);
        return;
    }
    if (operationName == "doByte")
    {
        char& p0 = *( char*)operation.getParameterValue(0);
        *(char*)operation.getReturnValue() = impl->doByte(p0);
        return;
    }
    if (operationName == "doMixed")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        long& p1 = *( long*)operation.getParameterValue(1);
        DataObjectPtr& p2 = *(DataObjectPtr*)operation.getParameterValue(2);        bool& p3 = *( bool*)operation.getParameterValue(3);
        double& p4 = *( double*)operation.getParameterValue(4);
        *(DataObjectPtr*)operation.getReturnValue() = impl->doMixed(p0, p1, p2, p3, p4);
        return;
    }
    if (operationName == "doDataObject")
    {
        DataObjectPtr& p0 = *(DataObjectPtr*)operation.getParameterValue(0);        *(DataObjectPtr*)operation.getReturnValue() = impl->doDataObject(p0);
        return;
    }
    if (operationName == "doAny")
    {
        DataObjectPtr& p0 = *(DataObjectPtr*)operation.getParameterValue(0);        *(DataObjectPtr*)operation.getReturnValue() = impl->doAny(p0);
        return;
    }
        

    throw ServiceRuntimeException("Invalid operation");
    
}

