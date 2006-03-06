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

#include "/CustomerInfo.h"
#include "tuscany/sca/core/ServiceWrapper.h"

class CustomerInfoImpl_CustomerInfoService_Proxy : public CustomerInfo
{
public:
    CustomerInfoImpl_CustomerInfoService_Proxy(tuscany::sca::ServiceWrapper*);
    virtual ~CustomerInfoImpl_CustomerInfoService_Proxy();
    virtual const char* getCustomerInformationChar( char* p1, const char* customerID);
    virtual const char* getCustomerInfoAChar( char* p1, const char* );
    virtual const char* getCustomerInfoBChar( char* p1,  char* customerID);
    virtual const char* getCustomerInfoCChar( char* p1,  char customerID);
    virtual const char* getCustomerInfoDChar( char* p1,  char );
    virtual const char* getCustomerInfoEChar( char* p1,  char* );
    virtual const char* getCustomerInfoFChar( char* p1,  char* p1,  char* customerID);
    virtual const char* getCustomerInfoGChar( char* p1,  char* ,  char* p1,  signed char* customerID);
    virtual const char* getCustomerInfoHChar( unsigned char* customerID, const char );
    virtual const char* getCustomerInfoHChar( unsigned char* customerID, const char* );
    virtual const long* getCustomerInformationLong(const long* customerID, const long* customerID);
    virtual const long* getCustomerInfoALong(const long* , const long* );
    virtual const long* getCustomerInfoBLong( long* customerID,  long* customerID2);
    virtual const long* getCustomerInfoCLong( long customerID,  long customerID2);
    virtual const long* getCustomerInfoDLong( long ,  long );
    virtual const long* getCustomerInfoELong( long* ,  long* );
    virtual const long* getCustomerInfoFLong( long* customerID,  long* customerID2);
    virtual const long* getCustomerInfoGLong( signed long* customerID,  signed long* customerID2);
    virtual const long* getCustomerInfoHLong( signed long* customerID,  signed long* customerID2);
    virtual const int* getCustomerInformationInt( char* , const int* customerID);
    virtual const int* getCustomerInfoAInt( char* , const int* );
    virtual const int* getCustomerInfoBInt( char* ,  int* customerID);
    virtual const int* getCustomerInfoCInt( char* ,  int customerID);
    virtual const int* getCustomerInfoDInt( char* ,  int );
    virtual const int* getCustomerInfoEInt( char* ,  int* );
    virtual const int* getCustomerInfoFInt( char* ,  int* customerID);
    virtual const int* getCustomerInfoGInt( char* ,  signed int* customerID);
    virtual const int* getCustomerInfoHInt( char* ,  unsigned int* customerID);
    virtual const __int64* getCustomerInformationint64( Diamond& , const __int64* customerID);
    virtual const __int64* getCustomerInfoAint64( Diamond& , const __int64* );
    virtual const __int64* getCustomerInfoBint64( Diamond& ,  __int64* customerID);
    virtual const __int64* getCustomerInfoCint64( Diamond& ,  __int64 customerID);
    virtual const __int64* getCustomerInfoDint64( Diamond& myDiamond,  __int64 );
    virtual const __int64* getCustomerInfoEint64( Diamond& myDiamond,  __int64* );
    virtual const __int64* getCustomerInfoFint64( Diamond& myDiamond,  __int64* customerID);
    virtual const __int64* getCustomerInfoGint64( Diamond& myDiamond,  signed __int64* customerID);
    virtual const __int64* getCustomerInfoHint64(const Diamond& myDiamond,  unsigned int64* customerID);
    virtual const void* getCustomerInformationVoid( float& f, const __int64* customerID);
    virtual void getCustomerInfoAVoid(const float& f, const __int64* );
    virtual void getCustomerInfoBVoid();
    virtual void getCustomerInfoCVoid();
    virtual void getCustomerInfoDVoid( void );
    virtual char getCustomerInfoEVoid( void );
    virtual char getCustomerInfoFVoid();
    virtual char getCustomerInfoGVoid( void );
    virtual const char* getCustomerInfoAInline( int ,  int ,  int ,  int ,  char* customer id);
    virtual int getCustomerInfoBInline( int* ,  int* f,  int* g,  char* customer id);
    virtual friend const unsigned int* getCustomerInfoCInline( char* customer id);
    virtual int getCustomerInfoTrickyA(const char , const char );
    virtual int getCustomerInfoTrickyB( int myInt,  int myInt);
private:
    tuscany::sca::ServiceWrapper* target;
};

#endif // CustomerInfoImpl_CustomerInfoService_Proxy_h

