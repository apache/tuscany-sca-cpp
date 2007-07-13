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

#include "CustomerInfoImpl2_CustomerInfoService_Wrapper.h"

#include "osoa/sca/sca.h"



extern "C"
{

    #if defined(WIN32) || defined(_WINDOWS)
    __declspec(dllexport) 
    #endif
    CustomerInfoImpl2_CustomerInfoService_Wrapper* CustomerInfoImpl2_CustomerInfoService_Wrapper_Factory(tuscany::sca::model::Service* target)
    {
        return new CustomerInfoImpl2_CustomerInfoService_Wrapper(target);
    }
}

CustomerInfoImpl2_CustomerInfoService_Wrapper::CustomerInfoImpl2_CustomerInfoService_Wrapper(tuscany::sca::model::Service* target) : tuscany::sca::cpp::CPPServiceWrapper(target)
{
    impl = (CustomerInfoImpl2*)getImplementation();
}

CustomerInfoImpl2_CustomerInfoService_Wrapper::~CustomerInfoImpl2_CustomerInfoService_Wrapper()
{
    releaseImplementation();
}

void* CustomerInfoImpl2_CustomerInfoService_Wrapper::newImplementation()
{
    return new CustomerInfoImpl2;
}

void CustomerInfoImpl2_CustomerInfoService_Wrapper::deleteImplementation()
{
    delete impl;
}

void CustomerInfoImpl2_CustomerInfoService_Wrapper::invokeService(tuscany::sca::Operation& operation)
{
    const std::string& operationName = operation.getName();

    if (operationName == "getCustomerInformation")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);

        if(operation.getReturnValue() != NULL)
        {
            *(const char**)operation.getReturnValue() = impl->getCustomerInformation(p0);
        }
        else
        {
            const char** ret = new const char*;
            *ret = impl->getCustomerInformation(p0);
            operation.setReturnValue((const const char**)ret);
        }
        return;
    }
        

    throw osoa::sca::ServiceRuntimeException("Invalid operation");
    
}

