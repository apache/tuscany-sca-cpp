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

#include "DivideImpl_DivideService_Wrapper.h"

#include "osoa/sca/sca.h"



extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    DivideImpl_DivideService_Wrapper* DivideImpl_DivideService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new DivideImpl_DivideService_Wrapper(target);
    }
}

DivideImpl_DivideService_Wrapper::DivideImpl_DivideService_Wrapper(tuscany::sca::model::Service* target) : tuscany::sca::cpp::CPPServiceWrapper(target)
{
    impl = (DivideImpl*)getImplementation();
}

DivideImpl_DivideService_Wrapper::~DivideImpl_DivideService_Wrapper()
{
    releaseImplementation();
}

void* DivideImpl_DivideService_Wrapper::newImplementation()
{
    return new DivideImpl;
}

void DivideImpl_DivideService_Wrapper::deleteImplementation()
{
    delete impl;
}

void DivideImpl_DivideService_Wrapper::invokeService(tuscany::sca::Operation& operation)
{
    const std::string& operationName = operation.getName();

    if (operationName == "div")
    {
        float& p0 = *( float*)operation.getParameterValue(0);
        float& p1 = *( float*)operation.getParameterValue(1);

        if(operation.getReturnValue() != NULL)
        {
            *(float*)operation.getReturnValue() = impl->div(p0, p1);
        }
        else
        {
            float* ret = new float;
            *ret = impl->div(p0, p1);
            operation.setReturnValue((const float*)ret);
        }
        return;
    }
        

    throw osoa::sca::ServiceRuntimeException("Invalid operation");
    
}

