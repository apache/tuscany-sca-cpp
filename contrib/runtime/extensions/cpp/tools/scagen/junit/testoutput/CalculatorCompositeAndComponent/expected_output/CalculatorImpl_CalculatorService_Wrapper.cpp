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

    if (operationName == "subtract")
    {
        long& p0 = *( long*)operation.getParameterValue(0);
        long& p1 = *( long*)operation.getParameterValue(1);

        if(operation.getReturnValue() != NULL)
        {
            *(long*)operation.getReturnValue() = impl->subtract(p0, p1);
        }
        else
        {
            long* ret = new long;
            *ret = impl->subtract(p0, p1);
            operation.setReturnValue((const long*)ret);
        }
        return;
    }
        

    throw osoa::sca::ServiceRuntimeException("Invalid operation");
    
}

