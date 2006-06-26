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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#ifndef AccountServiceImpl_AccountDataService_Proxy_h
#define AccountServiceImpl_AccountDataService_Proxy_h

#include "AccountDataService.h"
#include "tuscany/sca/core/ServiceWrapper.h"

class AccountServiceImpl_AccountDataService_Proxy : public com::bigbank::account::AccountDataService
{
public:
    AccountServiceImpl_AccountDataService_Proxy(tuscany::sca::ServiceWrapper*);
    virtual ~AccountServiceImpl_AccountDataService_Proxy();
    virtual commonj::sdo::DataObjectPtr getCheckingAccount(const char* id);
    virtual commonj::sdo::DataObjectPtr getSavingsAccount(const char* id);
    virtual commonj::sdo::DataObjectPtr getStockAccount(const char* id);
private:
    tuscany::sca::ServiceWrapper* target;
};

#endif // AccountServiceImpl_AccountDataService_Proxy_h

