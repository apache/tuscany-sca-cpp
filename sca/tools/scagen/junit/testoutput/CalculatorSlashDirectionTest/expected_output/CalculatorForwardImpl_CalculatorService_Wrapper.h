/* 
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef CalculatorForwardImpl_CalculatorService_Wrapper_h
#define CalculatorForwardImpl_CalculatorService_Wrapper_h

#if defined(WIN32) || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif 

#include "subFolder/CalculatorForwardImpl.h"
#include "tuscany/sca/cpp/CPPServiceWrapper.h"

class CalculatorForwardImpl_CalculatorService_Wrapper : public tuscany::sca::cpp::CPPServiceWrapper
{
public:
    CalculatorForwardImpl_CalculatorService_Wrapper(tuscany::sca::model::Service* target);
    virtual ~CalculatorForwardImpl_CalculatorService_Wrapper();
    virtual void invokeService(tuscany::sca::Operation& operation);
    virtual void* newImplementation();
    virtual void deleteImplementation();
private:
    CalculatorForwardImpl* impl;
};

#endif // CalculatorForwardImpl_CalculatorService_Wrapper_h

