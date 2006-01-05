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
    virtual const char* getCustomerInformation(const char* customerID);
private:
    tuscany::sca::ServiceWrapper* target;
};

#endif // CustomerInfoImpl_CustomerInfoService_Proxy_h

