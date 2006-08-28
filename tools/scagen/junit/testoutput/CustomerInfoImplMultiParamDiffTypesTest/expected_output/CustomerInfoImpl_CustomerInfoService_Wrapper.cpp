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

#include "CustomerInfoImpl_CustomerInfoService_Wrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;



extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CustomerInfoImpl_CustomerInfoService_Wrapper* CustomerInfoImpl_CustomerInfoService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new CustomerInfoImpl_CustomerInfoService_Wrapper(target);
    }
}

CustomerInfoImpl_CustomerInfoService_Wrapper::CustomerInfoImpl_CustomerInfoService_Wrapper(Service* target) : CPPServiceWrapper(target)
{
    impl = (CustomerInfoImpl*)getImplementation();
}

CustomerInfoImpl_CustomerInfoService_Wrapper::~CustomerInfoImpl_CustomerInfoService_Wrapper()
{
    releaseImplementation();
}

void* CustomerInfoImpl_CustomerInfoService_Wrapper::newImplementation()
{
    return new CustomerInfoImpl;
}

void CustomerInfoImpl_CustomerInfoService_Wrapper::deleteImplementation()
{
    delete impl;
}

void CustomerInfoImpl_CustomerInfoService_Wrapper::invokeService(Operation& operation)
{
    const string& operationName = operation.getName();

    if (operationName == "getCustomerInformationChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        const char* p1 = *(const char**)operation.getParameterValue(1);
        *(const char**)operation.getReturnValue() = impl->getCustomerInformationChar(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoAChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        const char* p1 = *(const char**)operation.getParameterValue(1);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoAChar(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoBChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        char* p1 = *( char**)operation.getParameterValue(1);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoBChar(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoCChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        char& p1 = *( char*)operation.getParameterValue(1);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoCChar(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoDChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        char& p1 = *( char*)operation.getParameterValue(1);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoDChar(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoEChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        char* p1 = *( char**)operation.getParameterValue(1);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoEChar(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoFChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        char* p1 = *( char**)operation.getParameterValue(1);
        char* p2 = *( char**)operation.getParameterValue(2);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoFChar(p0, p1, p2);
        return;
    }
    if (operationName == "getCustomerInfoGChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        char* p1 = *( char**)operation.getParameterValue(1);
        char* p2 = *( char**)operation.getParameterValue(2);
        signed char* p3 = *( signed char**)operation.getParameterValue(3);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoGChar(p0, p1, p2, p3);
        return;
    }
    if (operationName == "getCustomerInfoHChar")
    {
        unsigned char* p0 = *( unsigned char**)operation.getParameterValue(0);
        const char& p1 = *(const char*)operation.getParameterValue(1);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoHChar(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoHChar")
    {
        unsigned char* p0 = *( unsigned char**)operation.getParameterValue(0);
        const char* p1 = *(const char**)operation.getParameterValue(1);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoHChar(p0, p1);
        return;
    }
    if (operationName == "getCustomerInformationLong")
    {
        const long* p0 = *(const long**)operation.getParameterValue(0);
        const long* p1 = *(const long**)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInformationLong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoALong")
    {
        const long* p0 = *(const long**)operation.getParameterValue(0);
        const long* p1 = *(const long**)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInfoALong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoBLong")
    {
        long* p0 = *( long**)operation.getParameterValue(0);
        long* p1 = *( long**)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInfoBLong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoCLong")
    {
        long& p0 = *( long*)operation.getParameterValue(0);
        long& p1 = *( long*)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInfoCLong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoDLong")
    {
        long& p0 = *( long*)operation.getParameterValue(0);
        long& p1 = *( long*)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInfoDLong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoELong")
    {
        long* p0 = *( long**)operation.getParameterValue(0);
        long* p1 = *( long**)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInfoELong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoFLong")
    {
        long* p0 = *( long**)operation.getParameterValue(0);
        long* p1 = *( long**)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInfoFLong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoGLong")
    {
        signed long* p0 = *( signed long**)operation.getParameterValue(0);
        signed long* p1 = *( signed long**)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInfoGLong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoHLong")
    {
        signed long* p0 = *( signed long**)operation.getParameterValue(0);
        signed long* p1 = *( signed long**)operation.getParameterValue(1);
        *(const long**)operation.getReturnValue() = impl->getCustomerInfoHLong(p0, p1);
        return;
    }
    if (operationName == "getCustomerInformationInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        const int* p1 = *(const int**)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInformationInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoAInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        const int* p1 = *(const int**)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInfoAInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoBInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        int* p1 = *( int**)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInfoBInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoCInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        int& p1 = *( int*)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInfoCInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoDInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        int& p1 = *( int*)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInfoDInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoEInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        int* p1 = *( int**)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInfoEInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoFInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        int* p1 = *( int**)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInfoFInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoGInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        signed int* p1 = *( signed int**)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInfoGInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoHInt")
    {
        char* p0 = *( char**)operation.getParameterValue(0);
        unsigned int* p1 = *( unsigned int**)operation.getParameterValue(1);
        *(const int**)operation.getReturnValue() = impl->getCustomerInfoHInt(p0, p1);
        return;
    }
    if (operationName == "getCustomerInformationint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        const __int64* p1 = *(const __int64**)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInformationint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoAint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        const __int64* p1 = *(const __int64**)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoAint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoBint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        __int64* p1 = *( __int64**)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoBint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoCint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        __int64& p1 = *( __int64*)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoCint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoDint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        __int64& p1 = *( __int64*)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoDint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoEint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        __int64* p1 = *( __int64**)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoEint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoFint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        __int64* p1 = *( __int64**)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoFint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoGint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        signed __int64* p1 = *( signed __int64**)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoGint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoHint64")
    {
        Diamond& p0 = *(Diamond*)operation.getParameterValue(0);
        unsigned int64* p1 = *( unsigned int64**)operation.getParameterValue(1);
        *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoHint64(p0, p1);
        return;
    }
    if (operationName == "getCustomerInformationVoid")
    {
        float& p0 = *(float*)operation.getParameterValue(0);
        const __int64* p1 = *(const __int64**)operation.getParameterValue(1);
        *(const void**)operation.getReturnValue() = impl->getCustomerInformationVoid(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoAVoid")
    {
        float& p0 = *(float*)operation.getParameterValue(0);
        const __int64* p1 = *(const __int64**)operation.getParameterValue(1);
            impl->getCustomerInfoAVoid(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoBVoid")
    {
            impl->getCustomerInfoBVoid();
        return;
    }
    if (operationName == "getCustomerInfoCVoid")
    {
            impl->getCustomerInfoCVoid();
        return;
    }
    if (operationName == "getCustomerInfoDVoid")
    {
            impl->getCustomerInfoDVoid();
        return;
    }
    if (operationName == "getCustomerInfoEVoid")
    {
        *(char*)operation.getReturnValue() = impl->getCustomerInfoEVoid();
        return;
    }
    if (operationName == "getCustomerInfoFVoid")
    {
        *(char*)operation.getReturnValue() = impl->getCustomerInfoFVoid();
        return;
    }
    if (operationName == "getCustomerInfoGVoid")
    {
        *(char*)operation.getReturnValue() = impl->getCustomerInfoGVoid();
        return;
    }
    if (operationName == "getCustomerInfoAInline")
    {
        int& p0 = *( int*)operation.getParameterValue(0);
        int& p1 = *( int*)operation.getParameterValue(1);
        int& p2 = *( int*)operation.getParameterValue(2);
        int& p3 = *( int*)operation.getParameterValue(3);
        char* customer p4 = *( char* customer*)operation.getParameterValue(4);
        *(const char**)operation.getReturnValue() = impl->getCustomerInfoAInline(p0, p1, p2, p3, p4);
        return;
    }
    if (operationName == "getCustomerInfoBInline")
    {
        int* p0 = *( int**)operation.getParameterValue(0);
        int* p1 = *( int**)operation.getParameterValue(1);
        int* p2 = *( int**)operation.getParameterValue(2);
        char* customer p3 = *( char* customer*)operation.getParameterValue(3);
        *(int*)operation.getReturnValue() = impl->getCustomerInfoBInline(p0, p1, p2, p3);
        return;
    }
    if (operationName == "getCustomerInfoCInline")
    {
        char* customer p0 = *( char* customer*)operation.getParameterValue(0);
        *(friend const unsigned int**)operation.getReturnValue() = impl->getCustomerInfoCInline(p0);
        return;
    }
    if (operationName == "getCustomerInfoTrickyA")
    {
        const char& p0 = *(const char*)operation.getParameterValue(0);
        const char& p1 = *(const char*)operation.getParameterValue(1);
        *(int*)operation.getReturnValue() = impl->getCustomerInfoTrickyA(p0, p1);
        return;
    }
    if (operationName == "getCustomerInfoTrickyB")
    {
        int& p0 = *( int*)operation.getParameterValue(0);
        int& p1 = *( int*)operation.getParameterValue(1);
        *(int*)operation.getReturnValue() = impl->getCustomerInfoTrickyB(p0, p1);
        return;
    }
        

    throw ServiceRuntimeException("Invalid operation");
    
}

