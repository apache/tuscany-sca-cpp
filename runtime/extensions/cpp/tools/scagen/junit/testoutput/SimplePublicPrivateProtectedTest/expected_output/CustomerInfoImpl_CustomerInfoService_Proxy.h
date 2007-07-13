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
    virtual const char* getCustomerInformationCharPublic( char* p1, const char* customerID);
    virtual const char* getCustomerInfoACharPublic( char* p1, const char* );
    virtual const char* getCustomerInfoBCharPublic( char* p1,  char* customerID);
private:
    tuscany::sca::ServiceWrapper* target;
};

#endif // CustomerInfoImpl_CustomerInfoService_Proxy_h

