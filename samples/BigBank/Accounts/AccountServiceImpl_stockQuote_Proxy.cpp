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

#include "AccountServiceImpl_stockQuote_Proxy.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    AccountServiceImpl_stockQuote_Proxy* AccountServiceImpl_stockQuote_Proxy_Factory(ServiceWrapper* target)
    {
        return new AccountServiceImpl_stockQuote_Proxy(target);
    }

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    void AccountServiceImpl_stockQuote_Proxy_Destructor(void* proxy)
    {
        delete (AccountServiceImpl_stockQuote_Proxy*)proxy;
    }
}

AccountServiceImpl_stockQuote_Proxy::AccountServiceImpl_stockQuote_Proxy(ServiceWrapper* targ) : target(targ)
{
}

AccountServiceImpl_stockQuote_Proxy::~AccountServiceImpl_stockQuote_Proxy()
{
    if (target)
        delete target;
}

float AccountServiceImpl_stockQuote_Proxy::getQuote(const char* arg0)
{
    Operation operation("getQuote");
    operation.addParameter(&arg0);
    float ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return ret;
}


