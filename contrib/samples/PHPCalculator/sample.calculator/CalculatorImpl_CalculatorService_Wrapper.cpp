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

#include "CalculatorImpl_CalculatorService_Wrapper.h"

#include "osoa/sca/sca.h"



extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CalculatorImpl_CalculatorService_Wrapper* CalculatorImpl_CalculatorService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new CalculatorImpl_CalculatorService_Wrapper(target);
    }
}

CalculatorImpl_CalculatorService_Wrapper::CalculatorImpl_CalculatorService_Wrapper(tuscany::sca::model::Service* target) : tuscany::sca::cpp::CPPServiceWrapper(target)
{
    impl = (CalculatorImpl*)getImplementation();
}

CalculatorImpl_CalculatorService_Wrapper::~CalculatorImpl_CalculatorService_Wrapper()
{
    releaseImplementation();
}

void* CalculatorImpl_CalculatorService_Wrapper::newImplementation()
{
    return new CalculatorImpl;
}

void CalculatorImpl_CalculatorService_Wrapper::deleteImplementation()
{
    delete impl;
}

void CalculatorImpl_CalculatorService_Wrapper::invokeService(tuscany::sca::Operation& operation)
{
    const std::string& operationName = operation.getName();

    if (operationName == "add")
    {
        float& p0 = *( float*)operation.getParameterValue(0);
        float& p1 = *( float*)operation.getParameterValue(1);

        if(operation.getReturnValue() != NULL)
        {
            *(float*)operation.getReturnValue() = impl->add(p0, p1);
        }
        else
        {
            float* ret = new float;
            *ret = impl->add(p0, p1);
            operation.setReturnValue((const float*)ret);
        }
        return;
    }
    if (operationName == "sub")
    {
        float& p0 = *( float*)operation.getParameterValue(0);
        float& p1 = *( float*)operation.getParameterValue(1);

        if(operation.getReturnValue() != NULL)
        {
            *(float*)operation.getReturnValue() = impl->sub(p0, p1);
        }
        else
        {
            float* ret = new float;
            *ret = impl->sub(p0, p1);
            operation.setReturnValue((const float*)ret);
        }
        return;
    }
    if (operationName == "mul")
    {
        float& p0 = *( float*)operation.getParameterValue(0);
        float& p1 = *( float*)operation.getParameterValue(1);

        if(operation.getReturnValue() != NULL)
        {
            *(float*)operation.getReturnValue() = impl->mul(p0, p1);
        }
        else
        {
            float* ret = new float;
            *ret = impl->mul(p0, p1);
            operation.setReturnValue((const float*)ret);
        }
        return;
    }
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

