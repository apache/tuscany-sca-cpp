/* 
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

#include "MyValueImpl_MyValueService_Wrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

using myvaluecorp::implns::MyValueImpl;

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

MyValueImpl_MyValueService_Wrapper::MyValueImpl_MyValueService_Wrapper(Service* target) : ComponentServiceWrapper(target)
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

void MyValueImpl_MyValueService_Wrapper::invokeService(Operation& operation)
{
    const string& operationName = operation.getName();

    if (operationName == "getMyValueOther")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);
        *(float*)operation.getReturnValue() = impl->getMyValueOther(p0);
        return;
    }
    if (operationName == "getMyValueSOther")
    {
        string& p0 = *(string*)operation.getParameterValue(0);
        *(float*)operation.getReturnValue() = impl->getMyValueSOther(p0);
        return;
    }
    if (operationName == "getCustnameOther")
    {
        string& p0 = *(string*)operation.getParameterValue(0);
        *(string*)operation.getReturnValue() = impl->getCustnameOther(p0);
        return;
    }
    if (operationName == "getCustnamecsOther")
    {
        string& p0 = *( string*)operation.getParameterValue(0);
        const string& ret = impl->getCustnamecsOther(p0);
        operation.setReturnValue(&ret);
        return;
    }
        

    throw ServiceRuntimeException("Invalid operation");
    
}

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

#include "MyValueImpl_MyValueService_Wrapper.h"

#include "osoa/sca/sca.h"
using namespace osoa::sca;
using namespace tuscany::sca;

using myvaluecorp::implns::MyValueImpl;

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

MyValueImpl_MyValueService_Wrapper::MyValueImpl_MyValueService_Wrapper(Service* target) : ComponentServiceWrapper(target)
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

void MyValueImpl_MyValueService_Wrapper::invokeService(Operation& operation)
{
    const string& operationName = operation.getName();

    if (operationName == "getMyValueOther")
    {
        const char* p0 = *(const char**)operation.getParameterValue(0);
        *(float*)operation.getReturnValue() = impl->getMyValueOther(p0);
        return;
    }
    if (operationName == "getMyValueSOther")
    {
        string& p0 = *(string*)operation.getParameterValue(0);
        *(float*)operation.getReturnValue() = impl->getMyValueSOther(p0);
        return;
    }
    if (operationName == "getCustnameOther")
    {
        string& p0 = *(string*)operation.getParameterValue(0);
        *(string*)operation.getReturnValue() = impl->getCustnameOther(p0);
        return;
    }
    if (operationName == "getCustnamecsOther")
    {
        string& p0 = *( string*)operation.getParameterValue(0);
        const string& ret = impl->getCustnamecsOther(p0);
        operation.setReturnValue(&ret);
        return;
    }
        

    throw ServiceRuntimeException("Invalid operation");
    
}

