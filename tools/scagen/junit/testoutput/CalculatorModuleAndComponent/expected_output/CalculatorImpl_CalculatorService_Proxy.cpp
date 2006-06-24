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

#include "CalculatorImpl_CalculatorService_Proxy.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CalculatorImpl_CalculatorService_Proxy* CalculatorImpl_CalculatorService_Proxy_Factory(ServiceWrapper* target)
    {
        return new CalculatorImpl_CalculatorService_Proxy(target);
    }

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    void CalculatorImpl_CalculatorService_Proxy_Destructor(void* proxy)
    {
        delete (CalculatorImpl_CalculatorService_Proxy*)proxy;
    }
}

CalculatorImpl_CalculatorService_Proxy::CalculatorImpl_CalculatorService_Proxy(ServiceWrapper* targ) : target(targ)
{
}

CalculatorImpl_CalculatorService_Proxy::~CalculatorImpl_CalculatorService_Proxy()
{
    if (target)
        delete target;
}

long CalculatorImpl_CalculatorService_Proxy::subtract( long arg0,  long arg1)
{
    Operation operation("subtract");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    long ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return ret;
}


