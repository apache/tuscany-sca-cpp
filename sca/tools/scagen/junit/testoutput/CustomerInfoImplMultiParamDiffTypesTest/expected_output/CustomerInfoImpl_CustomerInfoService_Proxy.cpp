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

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationChar( char* arg0, const char* arg1)
{
    Operation operation("getCustomerInformationChar", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAChar( char* arg0, const char* arg1)
{
    Operation operation("getCustomerInfoAChar", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBChar( char* arg0,  char* arg1)
{
    Operation operation("getCustomerInfoBChar", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCChar( char* arg0,  char arg1)
{
    Operation operation("getCustomerInfoCChar", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDChar( char* arg0,  char arg1)
{
    Operation operation("getCustomerInfoDChar", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEChar( char* arg0,  char* arg1)
{
    Operation operation("getCustomerInfoEChar", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFChar( char* arg0,  char* arg1,  char* arg2)
{
    Operation operation("getCustomerInfoFChar", 3);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

    operation.setParameter(2, (void*)arg2);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGChar( char* arg0,  char* arg1,  char* arg2,  signed char* arg3)
{
    Operation operation("getCustomerInfoGChar", 4);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

    operation.setParameter(2, (void*)arg2);

    operation.setParameter(3, (void*)arg3);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHChar( unsigned char* arg0, const char arg1)
{
    Operation operation("getCustomerInfoHChar", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHChar( unsigned char* arg0, const char* arg1)
{
    Operation operation("getCustomerInfoHChar", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationLong(const long* arg0, const long* arg1)
{
    Operation operation("getCustomerInformationLong", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoALong(const long* arg0, const long* arg1)
{
    Operation operation("getCustomerInfoALong", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBLong( long* arg0,  long* arg1)
{
    Operation operation("getCustomerInfoBLong", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCLong( long arg0,  long arg1)
{
    Operation operation("getCustomerInfoCLong", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDLong( long arg0,  long arg1)
{
    Operation operation("getCustomerInfoDLong", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoELong( long* arg0,  long* arg1)
{
    Operation operation("getCustomerInfoELong", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFLong( long* arg0,  long* arg1)
{
    Operation operation("getCustomerInfoFLong", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGLong( signed long* arg0,  signed long* arg1)
{
    Operation operation("getCustomerInfoGLong", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHLong( signed long* arg0,  signed long* arg1)
{
    Operation operation("getCustomerInfoHLong", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationInt( char* arg0, const int* arg1)
{
    Operation operation("getCustomerInformationInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAInt( char* arg0, const int* arg1)
{
    Operation operation("getCustomerInfoAInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBInt( char* arg0,  int* arg1)
{
    Operation operation("getCustomerInfoBInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCInt( char* arg0,  int arg1)
{
    Operation operation("getCustomerInfoCInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDInt( char* arg0,  int arg1)
{
    Operation operation("getCustomerInfoDInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEInt( char* arg0,  int* arg1)
{
    Operation operation("getCustomerInfoEInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFInt( char* arg0,  int* arg1)
{
    Operation operation("getCustomerInfoFInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGInt( char* arg0,  signed int* arg1)
{
    Operation operation("getCustomerInfoGInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHInt( char* arg0,  unsigned int* arg1)
{
    Operation operation("getCustomerInfoHInt", 2);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationint64( Diamond& arg0, const __int64* arg1)
{
    Operation operation("getCustomerInformationint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAint64( Diamond& arg0, const __int64* arg1)
{
    Operation operation("getCustomerInfoAint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBint64( Diamond& arg0,  __int64* arg1)
{
    Operation operation("getCustomerInfoBint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCint64( Diamond& arg0,  __int64 arg1)
{
    Operation operation("getCustomerInfoCint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDint64( Diamond& arg0,  __int64 arg1)
{
    Operation operation("getCustomerInfoDint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)&arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEint64( Diamond& arg0,  __int64* arg1)
{
    Operation operation("getCustomerInfoEint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFint64( Diamond& arg0,  __int64* arg1)
{
    Operation operation("getCustomerInfoFint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGint64( Diamond& arg0,  signed __int64* arg1)
{
    Operation operation("getCustomerInfoGint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHint64(const Diamond& arg0,  unsigned int64* arg1)
{
    Operation operation("getCustomerInfoHint64", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const void* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationVoid( float& arg0, const __int64* arg1)
{
    Operation operation("getCustomerInformationVoid", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return (const void*)operation.getReturnValue();

}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAVoid(const float& arg0, const __int64* arg1)
{
    Operation operation("getCustomerInfoAVoid", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)arg1);

 
    target->invoke(operation);
    return;

}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBVoid()
{
    Operation operation("getCustomerInfoBVoid", 0);
 
    target->invoke(operation);
    return;

}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCVoid()
{
    Operation operation("getCustomerInfoCVoid", 0);
 
    target->invoke(operation);
    return;

}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDVoid()
{
    Operation operation("getCustomerInfoDVoid", 0);
 
    target->invoke(operation);
    return;

}

char CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEVoid()
{
    Operation operation("getCustomerInfoEVoid", 0);
 char ret;
    operation.setReturnValue((void*)&ret);
    target->invoke(operation);
    return ret;

}

char CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFVoid()
{
    Operation operation("getCustomerInfoFVoid", 0);
 char ret;
    operation.setReturnValue((void*)&ret);
    target->invoke(operation);
    return ret;

}

char CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGVoid()
{
    Operation operation("getCustomerInfoGVoid", 0);
 char ret;
    operation.setReturnValue((void*)&ret);
    target->invoke(operation);
    return ret;

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAInline( int arg0,  int arg1,  int arg2,  int arg3,  char* customer arg4)
{
    Operation operation("getCustomerInfoAInline", 5);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)&arg1);

    operation.setParameter(2, (void*)&arg2);

    operation.setParameter(3, (void*)&arg3);

    operation.setParameter(4, (void*)arg4);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBInline( int* arg0,  int* arg1,  int* arg2,  char* customer arg3)
{
    Operation operation("getCustomerInfoBInline", 4);
    operation.setParameter(0, (void*)arg0);

    operation.setParameter(1, (void*)arg1);

    operation.setParameter(2, (void*)arg2);

    operation.setParameter(3, (void*)arg3);

 int ret;
    operation.setReturnValue((void*)&ret);
    target->invoke(operation);
    return ret;

}

friend const unsigned int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCInline( char* customer arg0)
{
    Operation operation("getCustomerInfoCInline", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (friend const unsigned int*)operation.getReturnValue();

}

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoTrickyA(const char arg0, const char arg1)
{
    Operation operation("getCustomerInfoTrickyA", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)&arg1);

 int ret;
    operation.setReturnValue((void*)&ret);
    target->invoke(operation);
    return ret;

}

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoTrickyB( int arg0,  int arg1)
{
    Operation operation("getCustomerInfoTrickyB", 2);
    operation.setParameter(0, (void*)&arg0);

    operation.setParameter(1, (void*)&arg1);

 int ret;
    operation.setReturnValue((void*)&ret);
    target->invoke(operation);
    return ret;

}


