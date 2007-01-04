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

#ifndef CustomerInfoImpl2_CustomerInfoService_Wrapper_h
#define CustomerInfoImpl2_CustomerInfoService_Wrapper_h

#if defined(WIN32) || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif 

#include "CustomerInfoImpl.h"
#include "tuscany/sca/cpp/CPPServiceWrapper.h"

class CustomerInfoImpl2_CustomerInfoService_Wrapper : public tuscany::sca::cpp::CPPServiceWrapper
{
public:
    CustomerInfoImpl2_CustomerInfoService_Wrapper(tuscany::sca::model::Service* target);
    virtual ~CustomerInfoImpl2_CustomerInfoService_Wrapper();
    virtual void invokeService(tuscany::sca::Operation& operation);
    virtual void* newImplementation();
    virtual void deleteImplementation();
private:
    myvaluecorp::implns::CustomerInfoImpl2* impl;
};

#endif // CustomerInfoImpl2_CustomerInfoService_Wrapper_h

