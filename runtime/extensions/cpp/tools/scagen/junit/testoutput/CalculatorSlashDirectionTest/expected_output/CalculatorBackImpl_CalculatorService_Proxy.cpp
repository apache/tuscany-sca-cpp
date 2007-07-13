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

#include "CalculatorBackImpl_CalculatorService_Proxy.h"

#include "osoa/sca/sca.h"

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CalculatorBackImpl_CalculatorService_Proxy* CalculatorBackImpl_CalculatorService_Proxy_Factory(tuscany::sca::ServiceWrapper* target)
    {
        return new CalculatorBackImpl_CalculatorService_Proxy(target);
    }

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    void CalculatorBackImpl_CalculatorService_Proxy_Destructor(void* proxy)
    {
        delete (CalculatorBackImpl_CalculatorService_Proxy*)proxy;
    }
}

CalculatorBackImpl_CalculatorService_Proxy::CalculatorBackImpl_CalculatorService_Proxy(tuscany::sca::ServiceWrapper* targ) : target(targ)
{
}

CalculatorBackImpl_CalculatorService_Proxy::~CalculatorBackImpl_CalculatorService_Proxy()
{
    if (target)
        delete target;
}

long CalculatorBackImpl_CalculatorService_Proxy::subtractBack( long arg0,  long arg1)
{
    tuscany::sca::Operation operation("subtractBack");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    long ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(long*)operation.getReturnValue();
}

long CalculatorBackImpl_CalculatorService_Proxy::addBack( long arg0,  long arg1)
{
    tuscany::sca::Operation operation("addBack");
    operation.addParameter(&arg0);
    operation.addParameter(&arg1);
    long ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(long*)operation.getReturnValue();
}


