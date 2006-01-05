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

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationChar(const char* arg0)
{
    Operation operation("getCustomerInformationChar", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAChar(const char* arg0)
{
    Operation operation("getCustomerInfoAChar", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBChar( char* arg0)
{
    Operation operation("getCustomerInfoBChar", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCChar( char arg0)
{
    Operation operation("getCustomerInfoCChar", 1);
    operation.setParameter(0, (void*)&arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDChar( char arg0)
{
    Operation operation("getCustomerInfoDChar", 1);
    operation.setParameter(0, (void*)&arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEChar( char* arg0)
{
    Operation operation("getCustomerInfoEChar", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFChar( char* arg0)
{
    Operation operation("getCustomerInfoFChar", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGChar( signed char* arg0)
{
    Operation operation("getCustomerInfoGChar", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHChar( unsigned char* arg0)
{
    Operation operation("getCustomerInfoHChar", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationLong(const long* arg0)
{
    Operation operation("getCustomerInformationLong", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoALong(const long* arg0)
{
    Operation operation("getCustomerInfoALong", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBLong( long* arg0)
{
    Operation operation("getCustomerInfoBLong", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCLong( long arg0)
{
    Operation operation("getCustomerInfoCLong", 1);
    operation.setParameter(0, (void*)&arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDLong( long arg0)
{
    Operation operation("getCustomerInfoDLong", 1);
    operation.setParameter(0, (void*)&arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoELong( long* arg0)
{
    Operation operation("getCustomerInfoELong", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFLong( long* arg0)
{
    Operation operation("getCustomerInfoFLong", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGLong( signed long* arg0)
{
    Operation operation("getCustomerInfoGLong", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const long* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHLong( unsigned long* arg0)
{
    Operation operation("getCustomerInfoHLong", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const long*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationInt(const int* arg0)
{
    Operation operation("getCustomerInformationInt", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAInt(const int* arg0)
{
    Operation operation("getCustomerInfoAInt", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBInt( int* arg0)
{
    Operation operation("getCustomerInfoBInt", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCInt( int arg0)
{
    Operation operation("getCustomerInfoCInt", 1);
    operation.setParameter(0, (void*)&arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDInt( int arg0)
{
    Operation operation("getCustomerInfoDInt", 1);
    operation.setParameter(0, (void*)&arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEInt( int* arg0)
{
    Operation operation("getCustomerInfoEInt", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFInt( int* arg0)
{
    Operation operation("getCustomerInfoFInt", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGInt( signed int* arg0)
{
    Operation operation("getCustomerInfoGInt", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const int* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHInt( unsigned int* arg0)
{
    Operation operation("getCustomerInfoHInt", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const int*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationint64(const __int64* arg0)
{
    Operation operation("getCustomerInformationint64", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAint64(const __int64* arg0)
{
    Operation operation("getCustomerInfoAint64", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBint64( __int64* arg0)
{
    Operation operation("getCustomerInfoBint64", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoCint64( __int64 arg0)
{
    Operation operation("getCustomerInfoCint64", 1);
    operation.setParameter(0, (void*)&arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoDint64( __int64 arg0)
{
    Operation operation("getCustomerInfoDint64", 1);
    operation.setParameter(0, (void*)&arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoEint64( __int64* arg0)
{
    Operation operation("getCustomerInfoEint64", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoFint64( __int64* arg0)
{
    Operation operation("getCustomerInfoFint64", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoGint64( signed __int64* arg0)
{
    Operation operation("getCustomerInfoGint64", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const __int64* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoHint64( unsigned int64* arg0)
{
    Operation operation("getCustomerInfoHint64", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const __int64*)operation.getReturnValue();

}

const void* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInformationVoid(const __int64* arg0)
{
    Operation operation("getCustomerInformationVoid", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const void*)operation.getReturnValue();

}

void CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAVoid(const __int64* arg0)
{
    Operation operation("getCustomerInfoAVoid", 1);
    operation.setParameter(0, (void*)arg0);

 
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

const char* CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoAInline( char* customer arg0)
{
    Operation operation("getCustomerInfoAInline", 1);
    operation.setParameter(0, (void*)arg0);

 
    target->invoke(operation);
    return (const char*)operation.getReturnValue();

}

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoBInline( char* customer arg0)
{
    Operation operation("getCustomerInfoBInline", 1);
    operation.setParameter(0, (void*)arg0);

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

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoTrickyA(const char arg0)
{
    Operation operation("getCustomerInfoTrickyA", 1);
    operation.setParameter(0, (void*)&arg0);

 int ret;
    operation.setReturnValue((void*)&ret);
    target->invoke(operation);
    return ret;

}

int CustomerInfoImpl_CustomerInfoService_Proxy::getCustomerInfoTrickyB( int arg0)
{
    Operation operation("getCustomerInfoTrickyB", 1);
    operation.setParameter(0, (void*)&arg0);

 int ret;
    operation.setReturnValue((void*)&ret);
    target->invoke(operation);
    return ret;

}


