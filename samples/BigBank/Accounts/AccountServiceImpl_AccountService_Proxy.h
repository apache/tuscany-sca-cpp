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

#ifndef AccountServiceImpl_AccountService_Proxy_h
#define AccountServiceImpl_AccountService_Proxy_h

#include "AccountService.h"
#include "tuscany/sca/core/ServiceWrapper.h"

class AccountServiceImpl_AccountService_Proxy : public com::bigbank::account::AccountService
{
public:
    AccountServiceImpl_AccountService_Proxy(tuscany::sca::ServiceWrapper*);
    virtual ~AccountServiceImpl_AccountService_Proxy();
    virtual commonj::sdo::DataObjectPtr getAccountReport(const char* id);
private:
    tuscany::sca::ServiceWrapper* target;
};

#endif // AccountServiceImpl_AccountService_Proxy_h

