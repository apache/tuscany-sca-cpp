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

#include "DivideImpl_DivideService_Proxy.h"

#include "osoa/sca/sca.h"

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    DivideImpl_DivideService_Proxy* DivideImpl_DivideService_Proxy_Factory(tuscany::sca::ServiceWrapper* target)
    {
        return new DivideImpl_DivideService_Proxy(target);
    }

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    void DivideImpl_DivideService_Proxy_Destructor(void* proxy)
    {
        delete (DivideImpl_DivideService_Proxy*)proxy;
    }
}

DivideImpl_DivideService_Proxy::DivideImpl_DivideService_Proxy(tuscany::sca::ServiceWrapper* targ) : target(targ)
{
}

DivideImpl_DivideService_Proxy::~DivideImpl_DivideService_Proxy()
{
    if (target)
        delete target;
}

float DivideImpl_DivideService_Proxy::div( float arg0,  float arg1)
{
    tuscany::sca::Operation operation("div");
    operation.addParameter("num1", &arg0);
    operation.addParameter("num2", &arg1);
    float ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(float*)operation.getReturnValue();
}


