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

#include "CustomerInfoImpl_CustomerInfoService_Wrapper.h"

#include "osoa/sca/sca.h"



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

CustomerInfoImpl_CustomerInfoService_Wrapper::CustomerInfoImpl_CustomerInfoService_Wrapper(tuscany::sca::model::Service* target) : tuscany::sca::cpp::CPPServiceWrapper(target)
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

void CustomerInfoImpl_CustomerInfoService_Wrapper::invokeService(tuscany::sca::Operation& operation)
{
    const std::string& operationName = operation.getName();

    if (operationName == "getCustomerInformationChar")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInformationChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInformationChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoAChar")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoAChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoAChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoBChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoBChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoBChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoCChar")
    {
        char& p0 = *( char*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoCChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoCChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoDChar")
    {
        char& p0 = *( char*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoDChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoDChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoEChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoEChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoEChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoFChar")
    {
        char* p0 = *( char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoFChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoFChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoGChar")
    {
        signed char* p0 = *( signed char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoGChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoGChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoHChar")
    {
        unsigned char* p0 = *( unsigned char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoHChar(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoHChar(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInformationLong")
    {
        const long* p0 = *(const long**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInformationLong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInformationLong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoALong")
    {
        const long* p0 = *(const long**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInfoALong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInfoALong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoBLong")
    {
        long* p0 = *( long**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInfoBLong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInfoBLong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoCLong")
    {
        long& p0 = *( long*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInfoCLong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInfoCLong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoDLong")
    {
        long& p0 = *( long*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInfoDLong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInfoDLong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoELong")
    {
        long* p0 = *( long**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInfoELong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInfoELong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoFLong")
    {
        long* p0 = *( long**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInfoFLong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInfoFLong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoGLong")
    {
        signed long* p0 = *( signed long**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInfoGLong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInfoGLong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoHLong")
    {
        unsigned long* p0 = *( unsigned long**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const long**)operation.getReturnValue() = impl->getCustomerInfoHLong(p0);
        }
        else
        {
            const long** ret = new const long*;
            *ret = impl->getCustomerInfoHLong(p0);
            operation.setReturnValue((const const long**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInformationInt")
    {
        const int* p0 = *(const int**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInformationInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInformationInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoAInt")
    {
        const int* p0 = *(const int**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInfoAInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInfoAInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoBInt")
    {
        int* p0 = *( int**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInfoBInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInfoBInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoCInt")
    {
        int& p0 = *( int*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInfoCInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInfoCInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoDInt")
    {
        int& p0 = *( int*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInfoDInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInfoDInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoEInt")
    {
        int* p0 = *( int**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInfoEInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInfoEInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoFInt")
    {
        int* p0 = *( int**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInfoFInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInfoFInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoGInt")
    {
        signed int* p0 = *( signed int**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInfoGInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInfoGInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoHInt")
    {
        unsigned int* p0 = *( unsigned int**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const int**)operation.getReturnValue() = impl->getCustomerInfoHInt(p0);
        }
        else
        {
            const int** ret = new const int*;
            *ret = impl->getCustomerInfoHInt(p0);
            operation.setReturnValue((const const int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInformationint64")
    {
        const __int64* p0 = *(const __int64**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInformationint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInformationint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoAint64")
    {
        const __int64* p0 = *(const __int64**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoAint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInfoAint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoBint64")
    {
        __int64* p0 = *( __int64**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoBint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInfoBint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoCint64")
    {
        __int64& p0 = *( __int64*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoCint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInfoCint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoDint64")
    {
        __int64& p0 = *( __int64*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoDint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInfoDint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoEint64")
    {
        __int64* p0 = *( __int64**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoEint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInfoEint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoFint64")
    {
        __int64* p0 = *( __int64**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoFint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInfoFint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoGint64")
    {
        signed __int64* p0 = *( signed __int64**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoGint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInfoGint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoHint64")
    {
        unsigned int64* p0 = *( unsigned int64**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const __int64**)operation.getReturnValue() = impl->getCustomerInfoHint64(p0);
        }
        else
        {
            const __int64** ret = new const __int64*;
            *ret = impl->getCustomerInfoHint64(p0);
            operation.setReturnValue((const const __int64**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInformationVoid")
    {
        const __int64* p0 = *(const __int64**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const void**)operation.getReturnValue() = impl->getCustomerInformationVoid(p0);
        }
        else
        {
            const void** ret = new const void*;
            *ret = impl->getCustomerInformationVoid(p0);
            operation.setReturnValue((const const void**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoAVoid")
    {
        const __int64* p0 = *(const __int64**)operation.getParameterValue(0);
            impl->getCustomerInfoAVoid(p0);
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
    if (operationName == "getCustomerInfoAInline")
    {
        char* customer p0 = *( char* customer*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInfoAInline(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInfoAInline(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoBInline")
    {
        char* customer p0 = *( char* customer*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(int*)operation.getReturnValue() = impl->getCustomerInfoBInline(p0);
        }
        else
        {
            int* ret = new int;
            *ret = impl->getCustomerInfoBInline(p0);
            operation.setReturnValue((const int*)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoCInline")
    {
        char* customer p0 = *( char* customer*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(friend const unsigned int**)operation.getReturnValue() = impl->getCustomerInfoCInline(p0);
        }
        else
        {
            friend const unsigned int** ret = new friend const unsigned int*;
            *ret = impl->getCustomerInfoCInline(p0);
            operation.setReturnValue((const friend const unsigned int**)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoTrickyA")
    {
        const char& p0 = *(const char*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(int*)operation.getReturnValue() = impl->getCustomerInfoTrickyA(p0);
        }
        else
        {
            int* ret = new int;
            *ret = impl->getCustomerInfoTrickyA(p0);
            operation.setReturnValue((const int*)ret);
        }
        return;
    }
    if (operationName == "getCustomerInfoTrickyB")
    {
        int& p0 = *( int*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(int*)operation.getReturnValue() = impl->getCustomerInfoTrickyB(p0);
        }
        else
        {
            int* ret = new int;
            *ret = impl->getCustomerInfoTrickyB(p0);
            operation.setReturnValue((const int*)ret);
        }
        return;
    }
        

    throw osoa::sca::ServiceRuntimeException("Invalid operation");
    
}
