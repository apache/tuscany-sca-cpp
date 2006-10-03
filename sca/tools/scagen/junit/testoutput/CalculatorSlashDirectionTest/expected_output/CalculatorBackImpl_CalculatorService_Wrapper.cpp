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

#include "CalculatorBackImpl_CalculatorService_Wrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;



extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CalculatorBackImpl_CalculatorService_Wrapper* CalculatorBackImpl_CalculatorService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new CalculatorBackImpl_CalculatorService_Wrapper(target);
    }
}

CalculatorBackImpl_CalculatorService_Wrapper::CalculatorBackImpl_CalculatorService_Wrapper(Service* target) : CPPServiceWrapper(target)
{
    impl = (CalculatorBackImpl*)getImplementation();
}

CalculatorBackImpl_CalculatorService_Wrapper::~CalculatorBackImpl_CalculatorService_Wrapper()
{
    releaseImplementation();
}

void* CalculatorBackImpl_CalculatorService_Wrapper::newImplementation()
{
    return new CalculatorBackImpl;
}

void CalculatorBackImpl_CalculatorService_Wrapper::deleteImplementation()
{
    delete impl;
}

void CalculatorBackImpl_CalculatorService_Wrapper::invokeService(Operation& operation)
{
    const string& operationName = operation.getName();

    if (operationName == "subtractBack")
    {
        long& p0 = *( long*)operation.getParameterValue(0);
        long& p1 = *( long*)operation.getParameterValue(1);
        *(long*)operation.getReturnValue() = impl->subtractBack(p0, p1);
        return;
    }
    if (operationName == "addBack")
    {
        long& p0 = *( long*)operation.getParameterValue(0);
        long& p1 = *( long*)operation.getParameterValue(1);
        *(long*)operation.getReturnValue() = impl->addBack(p0, p1);
        return;
    }
        

    throw ServiceRuntimeException("Invalid operation");
    
}

