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

#include "MyValueImpl_MyValueService_Wrapper.h"

#include "osoa/sca/sca.h"



extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    MyValueImpl_MyValueService_Wrapper* MyValueImpl_MyValueService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new MyValueImpl_MyValueService_Wrapper(target);
    }
}

MyValueImpl_MyValueService_Wrapper::MyValueImpl_MyValueService_Wrapper(tuscany::sca::model::Service* target) : tuscany::sca::cpp::CPPServiceWrapper(target)
{
    impl = (MyValueImpl*)getImplementation();
}

MyValueImpl_MyValueService_Wrapper::~MyValueImpl_MyValueService_Wrapper()
{
    releaseImplementation();
}

void* MyValueImpl_MyValueService_Wrapper::newImplementation()
{
    return new MyValueImpl;
}

void MyValueImpl_MyValueService_Wrapper::deleteImplementation()
{
    delete impl;
}

void MyValueImpl_MyValueService_Wrapper::invokeService(tuscany::sca::Operation& operation)
{
    const std::string& operationName = operation.getName();

    if (operationName == "getMyValue")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(float*)operation.getReturnValue() = impl->getMyValue(p0);
        }
        else
        {
            float* ret = new float;
            *ret = impl->getMyValue(p0);
            operation.setReturnValue((const float*)ret);
        }
        return;
    }
    if (operationName == "getMyValueS")
    {
        string& p0 = *(string*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(float*)operation.getReturnValue() = impl->getMyValueS(p0);
        }
        else
        {
            float* ret = new float;
            *ret = impl->getMyValueS(p0);
            operation.setReturnValue((const float*)ret);
        }
        return;
    }
    if (operationName == "getCustname")
    {
        string& p0 = *(string*)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(string*)operation.getReturnValue() = impl->getCustname(p0);
        }
        else
        {
            string* ret = new string;
            *ret = impl->getCustname(p0);
            operation.setReturnValue((const string*)ret);
        }
        return;
    }
    if (operationName == "getCustnamecs")
    {
        string& p0 = *( string*)operation.getParameterValue(0);
        const string& ret = impl->getCustnamecs(p0);
        operation.setReturnValue(&ret);
        return;
    }
        

    throw osoa::sca::ServiceRuntimeException("Invalid operation");
    
}

