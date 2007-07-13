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

#include "MyValueImpl_MyValueService_Proxy.h"

#include "osoa/sca/sca.h"

extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    MyValueImpl_MyValueService_Proxy* MyValueImpl_MyValueService_Proxy_Factory(tuscany::sca::ServiceWrapper* target)
    {
        return new MyValueImpl_MyValueService_Proxy(target);
    }

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    void MyValueImpl_MyValueService_Proxy_Destructor(void* proxy)
    {
        delete (MyValueImpl_MyValueService_Proxy*)proxy;
    }
}

MyValueImpl_MyValueService_Proxy::MyValueImpl_MyValueService_Proxy(tuscany::sca::ServiceWrapper* targ) : target(targ)
{
}

MyValueImpl_MyValueService_Proxy::~MyValueImpl_MyValueService_Proxy()
{
    if (target)
        delete target;
}

float MyValueImpl_MyValueService_Proxy::getMyValue(const char* arg0)
{
    tuscany::sca::Operation operation("getMyValue");
    operation.addParameter("customerID", &arg0);
    float ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(float*)operation.getReturnValue();
}

float MyValueImpl_MyValueService_Proxy::getMyValueS(const string& arg0)
{
    tuscany::sca::Operation operation("getMyValueS");
    operation.addParameter("customerID", &arg0);
    float ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(float*)operation.getReturnValue();
}

string MyValueImpl_MyValueService_Proxy::getCustname( string& arg0)
{
    tuscany::sca::Operation operation("getCustname");
    operation.addParameter("customerID", &arg0);
    string ret;
    operation.setReturnValue(&ret);
    target->invoke(operation);
    return *(string*)operation.getReturnValue();
}

const string& MyValueImpl_MyValueService_Proxy::getCustnamecs( string arg0)
{
    tuscany::sca::Operation operation("getCustnamecs");
    operation.addParameter("customerID", &arg0);
    target->invoke(operation);
    return *(const string*)operation.getReturnValue();
}


