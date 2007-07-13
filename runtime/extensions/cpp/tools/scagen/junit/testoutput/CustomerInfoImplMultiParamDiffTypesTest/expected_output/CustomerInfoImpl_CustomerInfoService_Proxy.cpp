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

#include "CustomerInfoImpl_CustomerInfoService_Proxy.h"

#include "osoa/sca/sca.h"

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CustomerInfoImpl_CustomerInfoService_Proxy* CustomerInfoImpl_CustomerInfoService_Proxy_Factory(tuscany::sca::ServiceWrapper* target)
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

CustomerInfoImpl_CustomerInfoService_Proxy::CustomerInfoImpl_CustomerInfoService_Proxy(tuscany::sca::ServiceWrapper* targ) : target(targ)
{
}

CustomerInfoImpl_CustomerInfoService_Proxy::~CustomerInfoImpl_CustomerInfoService_Proxy()
{
    if (target)
        delete target;
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationChar( char* arg0, const char* arg1)
{
    tuscany::sca::Operation operation("getCustomerInformationChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAChar( char* arg0, const char* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoAChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBChar( char* arg0,  char* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoBChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCChar( char* arg0,  char arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoCChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDChar( char* arg0,  char arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoDChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEChar( char* arg0,  char* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoEChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFChar( char* arg0,  char* arg1,  char* arg2)
{
    tuscany::sca::Operation operation("getCustomerInfoFChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    operation.addParameter(&arg2);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGChar( char* arg0,  char* arg1,  char* arg2,  signed char* arg3)
{
    tuscany::sca::Operation operation("getCustomerInfoGChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    operation.addParameter(&arg2);
    operation.addParameter(&arg3);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHChar( unsigned char* arg0, const char arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoHChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHChar( unsigned char* arg0, const char* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoHChar");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationLong(const long* arg0, const long* arg1)
{
    tuscany::sca::Operation operation("getCustomerInformationLong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoALong(const long* arg0, const long* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoALong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBLong( long* arg0,  long* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoBLong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCLong( long arg0,  long arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoCLong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDLong( long arg0,  long arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoDLong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoELong( long* arg0,  long* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoELong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFLong( long* arg0,  long* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoFLong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGLong( signed long* arg0,  signed long* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoGLong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHLong( signed long* arg0,  signed long* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoHLong");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const long* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const long**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationInt( char* arg0, const int* arg1)
{
    tuscany::sca::Operation operation("getCustomerInformationInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAInt( char* arg0, const int* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoAInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBInt( char* arg0,  int* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoBInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCInt( char* arg0,  int arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoCInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDInt( char* arg0,  int arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoDInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEInt( char* arg0,  int* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoEInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFInt( char* arg0,  int* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoFInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGInt( char* arg0,  signed int* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoGInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHInt( char* arg0,  unsigned int* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoHInt");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const int**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationint64( Diamond& arg0, const __int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInformationint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAint64( Diamond& arg0, const __int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoAint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBint64( Diamond& arg0,  __int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoBint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCint64( Diamond& arg0,  __int64 arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoCint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDint64( Diamond& arg0,  __int64 arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoDint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEint64( Diamond& arg0,  __int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoEint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFint64( Diamond& arg0,  __int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoFint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGint64( Diamond& arg0,  signed __int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoGint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHint64(const Diamond& arg0,  unsigned int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoHint64");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const __int64* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const __int64**)operation.getReturnValue();
}

const void* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationVoid( float& arg0, const __int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInformationVoid");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    const void* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const void**)operation.getReturnValue();
}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAVoid(const float& arg0, const __int64* arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoAVoid");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    target->invoke(operation);
    return;
}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBVoid()
{
    tuscany::sca::Operation operation("getCustomerInfoBVoid");
    target->invoke(operation);
    return;
}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCVoid()
{
    tuscany::sca::Operation operation("getCustomerInfoCVoid");
    target->invoke(operation);
    return;
}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDVoid()
{
    tuscany::sca::Operation operation("getCustomerInfoDVoid");
    operation.addParameter(&arg0);
    target->invoke(operation);
    return;
}

char CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEVoid()
{
    tuscany::sca::Operation operation("getCustomerInfoEVoid");
    operation.addParameter(&arg0);
    char ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(char*)operation.getReturnValue();
}

char CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFVoid()
{
    tuscany::sca::Operation operation("getCustomerInfoFVoid");
    char ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(char*)operation.getReturnValue();
}

char CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGVoid()
{
    tuscany::sca::Operation operation("getCustomerInfoGVoid");
    operation.addParameter(&arg0);
    char ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(char*)operation.getReturnValue();
}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAInline( int arg0,  int arg1,  int arg2,  int arg3,  char* customer arg4)
{
    tuscany::sca::Operation operation("getCustomerInfoAInline");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    operation.addParameter(&arg2);
    operation.addParameter(&arg3);
    operation.addParameter(&arg4);
    const char* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(const char**)operation.getReturnValue();
}

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBInline( int* arg0,  int* arg1,  int* arg2,  char* customer arg3)
{
    tuscany::sca::Operation operation("getCustomerInfoBInline");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    operation.addParameter(&arg2);
    operation.addParameter(&arg3);
    int ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(int*)operation.getReturnValue();
}

friend const unsigned int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCInline( char* customer arg0)
{
    tuscany::sca::Operation operation("getCustomerInfoCInline");
    operation.addParameter(&arg0);
    friend const unsigned int* ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(friend const unsigned int**)operation.getReturnValue();
}

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoTrickyA(const char arg0, const char arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoTrickyA");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    int ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(int*)operation.getReturnValue();
}

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoTrickyB( int arg0,  int arg1)
{
    tuscany::sca::Operation operation("getCustomerInfoTrickyB");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    int ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(int*)operation.getReturnValue();
}


