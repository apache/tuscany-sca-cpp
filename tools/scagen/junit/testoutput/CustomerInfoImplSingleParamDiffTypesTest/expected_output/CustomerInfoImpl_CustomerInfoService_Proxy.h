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

#ifndef CustomerInfoImpl_CustomerInfoService_Proxy_h
#define CustomerInfoImpl_CustomerInfoService_Proxy_h

#if defined(WIN32) || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif 

#include "/CustomerInfo.h"
#include "tuscany/sca/core/ServiceWrapper.h"

class CustomerInfoImpl_CustomerInfoService_Proxy : public CustomerInfo
{
public:
    CustomerInfoImpl_CustomerInfoService_Proxy(tuscany::sca::ServiceWrapper*);
    virtual ~CustomerInfoImpl_CustomerInfoService_Proxy();
    virtual const char* getCustomerInformationChar(const char* customerID);
    virtual const char* getCustomerInfoAChar(const char* );
    virtual const char* getCustomerInfoBChar( char* customerID);
    virtual const char* getCustomerInfoCChar( char customerID);
    virtual const char* getCustomerInfoDChar( char );
    virtual const char* getCustomerInfoEChar( char* );
    virtual const char* getCustomerInfoFChar( char* customerID);
    virtual const char* getCustomerInfoGChar( signed char* customerID);
    virtual const char* getCustomerInfoHChar( unsigned char* customerID);
    virtual const long* getCustomerInformationLong(const long* customerID);
    virtual const long* getCustomerInfoALong(const long* );
    virtual const long* getCustomerInfoBLong( long* customerID);
    virtual const long* getCustomerInfoCLong( long customerID);
    virtual const long* getCustomerInfoDLong( long );
    virtual const long* getCustomerInfoELong( long* );
    virtual const long* getCustomerInfoFLong( long* customerID);
    virtual const long* getCustomerInfoGLong( signed long* customerID);
    virtual const long* getCustomerInfoHLong( unsigned long* customerID);
    virtual const int* getCustomerInformationInt(const int* customerID);
    virtual const int* getCustomerInfoAInt(const int* );
    virtual const int* getCustomerInfoBInt( int* customerID);
    virtual const int* getCustomerInfoCInt( int customerID);
    virtual const int* getCustomerInfoDInt( int );
    virtual const int* getCustomerInfoEInt( int* );
    virtual const int* getCustomerInfoFInt( int* customerID);
    virtual const int* getCustomerInfoGInt( signed int* customerID);
    virtual const int* getCustomerInfoHInt( unsigned int* customerID);
    virtual const __int64* getCustomerInformationint64(const __int64* customerID);
    virtual const __int64* getCustomerInfoAint64(const __int64* );
    virtual const __int64* getCustomerInfoBint64( __int64* customerID);
    virtual const __int64* getCustomerInfoCint64( __int64 customerID);
    virtual const __int64* getCustomerInfoDint64( __int64 );
    virtual const __int64* getCustomerInfoEint64( __int64* );
    virtual const __int64* getCustomerInfoFint64( __int64* customerID);
    virtual const __int64* getCustomerInfoGint64( signed __int64* customerID);
    virtual const __int64* getCustomerInfoHint64( unsigned int64* customerID);
    virtual const void* getCustomerInformationVoid(const __int64* customerID);
    virtual void getCustomerInfoAVoid(const __int64* );
    virtual void getCustomerInfoBVoid();
    virtual void getCustomerInfoCVoid();
    virtual void getCustomerInfoDVoid( void );
    virtual const char* getCustomerInfoAInline( char* customer id);
    virtual int getCustomerInfoBInline( char* customer id);
    virtual friend const unsigned int* getCustomerInfoCInline( char* customer id);
    virtual int getCustomerInfoTrickyA(const char );
    virtual int getCustomerInfoTrickyB( int myInt);
private:
    tuscany::sca::ServiceWrapper* target;
};

#endif // CustomerInfoImpl_CustomerInfoService_Proxy_h

