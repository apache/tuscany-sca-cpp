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

#ifndef WSCompositeServiceTestImpl_WSCompositeServiceTestService_Wrapper_h
#define WSCompositeServiceTestImpl_WSCompositeServiceTestService_Wrapper_h

#if defined(WIN32) || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif 

#include "WSCompositeServiceTestImpl.h"
#include "tuscany/sca/cpp/CPPServiceWrapper.h"

class WSCompositeServiceTestImpl_WSCompositeServiceTestService_Wrapper : public tuscany::sca::cpp::CPPServiceWrapper
{
public:
    WSCompositeServiceTestImpl_WSCompositeServiceTestService_Wrapper(tuscany::sca::model::Service* target);
    virtual ~WSCompositeServiceTestImpl_WSCompositeServiceTestService_Wrapper();
    virtual void invokeService(tuscany::sca::Operation& operation);
    virtual void* newImplementation();
    virtual void deleteImplementation();
private:
    WSCompositeServiceTestImpl* impl;
};

#endif // WSCompositeServiceTestImpl_WSCompositeServiceTestService_Wrapper_h

