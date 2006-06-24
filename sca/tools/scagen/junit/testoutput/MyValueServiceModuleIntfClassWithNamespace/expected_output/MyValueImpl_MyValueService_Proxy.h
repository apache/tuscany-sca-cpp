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

#ifndef MyValueImpl_MyValueService_Proxy_h
#define MyValueImpl_MyValueService_Proxy_h

#if defined(WIN32) || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif 

#include "MyValue.h"
#include "tuscany/sca/core/ServiceWrapper.h"

class MyValueImpl_MyValueService_Proxy : public Other::MyValue
{
public:
    MyValueImpl_MyValueService_Proxy(tuscany::sca::ServiceWrapper*);
    virtual ~MyValueImpl_MyValueService_Proxy();
    virtual float getMyValueOther(const char* customerID);
    virtual float getMyValueSOther(const string& customerID);
    virtual string getCustnameOther( string& customerID);
    virtual const string& getCustnamecsOther( string customerID);
private:
    tuscany::sca::ServiceWrapper* target;
};

#endif // MyValueImpl_MyValueService_Proxy_h

