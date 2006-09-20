/* 
+----------------------------------------------------------------------+
| (c) Copyright IBM Corporation 2005.                                  | 
| All Rights Reserved.                                                 |
+----------------------------------------------------------------------+ 
|                                                                      | 
| Licensed under the Apache License, Version 2.0 (the "License"); you  | 
| may not use this file except in compliance with the License. You may | 
| obtain a copy of the License at                                      | 
|  http://www.apache.org/licenses/LICENSE-2.0                          |
|                                                                      | 
| Unless required by applicable law or agreed to in writing, software  | 
| distributed under the License is distributed on an "AS IS" BASIS,    | 
| WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      | 
| implied. See the License for the specific language governing         | 
| permissions and limitations under the License.                       | 
+----------------------------------------------------------------------+ 
*/

#include "TestImpl.h"
#include <stdio.h>
#include <tuscany/sca/util/Utils.h>
using namespace tuscany::sca;
#include <commonj/sdo/SDO.h>
#include <sdo_axiom.h>
using namespace commonj::sdo;
using namespace commonj::sdo_axiom;

TestImpl::TestImpl()
{
}
    
TestImpl::~TestImpl()
{
}

char* TestImpl::doChars(char* arg1)
{
    printf("TestImpl::doChars %s\n", arg1);
    return arg1;
}


long TestImpl::doLong(long arg1)
{
    printf("TestImpl::doLong %d\n", arg1);
    return arg1;
}

int TestImpl::doInt(int arg1)
{
    printf("TestImpl::doInt %d\n", arg1);
    return arg1;
}

float TestImpl::doFloat(float arg1)
{
    printf("TestImpl::doFloat %f\n", arg1);
    return arg1;
}

long double TestImpl::doLongDouble(long double arg1)
{
    printf("TestImpl::doLongDouble %f\n", arg1);
    return arg1;
}

double TestImpl::doDouble(double arg1)
{
    printf("TestImpl::doDouble %f\n", arg1);
    return arg1;
}


bool TestImpl::doBool(bool arg1)
{
    printf("TestImpl::doBool %d\n", arg1);
    return arg1;
}

short TestImpl::doShort(short arg1)
{
    printf("TestImpl::doShort %d\n", arg1);
    return arg1;
}

char* TestImpl::doBytes(char* arg1)
{
    printf("TestImpl::doBytes %s\n", arg1);
    return arg1;
}

char TestImpl::doByte(char arg1)
{
    printf("TestImpl::doByte %c\n", arg1);
    return arg1;
}

DataObjectPtr TestImpl::doMixed(char* arg1, long arg2, DataObjectPtr arg3, bool arg4, double arg5)
{
    printf("TestImpl::doMixed %s %d %s %d %d\n", arg1, arg2, arg3->getType().getURI(), arg4, arg5);
    Utils::printDO(arg3);
    return arg3;
}

DataObjectPtr TestImpl::doDataObject(DataObjectPtr arg1)
{
    printf("TestImpl::doDataObject %s\n", arg1->getType().getURI());
    Utils::printDO(arg1);
    return arg1;
}

DataObjectPtr TestImpl::doAny(DataObjectPtr arg1)
{
    printf("TestImpl::doAny %s#%s\n", arg1->getType().getURI(), arg1->getType().getName());
    Utils::printDO(arg1);
    //return arg1;

    DataFactoryPtr factory = arg1->getDataFactory();
    DataObjectPtr data = factory->create(Type::SDOTypeNamespaceURI, "OpenDataObject");
    data->setCString("stringData", "Here is some string data");
    data->setBoolean("boolData", true);
    DataObjectList& list = data->getList("intDataList");
    list.append((long)123456789);
    list.append((long)111111111);
    list.append((long)222222222);
    Utils::printDO(data);
    return data;
}

//int[] TestImpl::doIntArray(int[] arg1, int arg2)
//{
//    printf("TestImpl::doIntArray with length %d\n", arg2);
//    for(int i=0; i<arg2; i++)
//    {
//        printf("%d\n", arg1[i]);
//    }
//    return arg1;
//}
//
//string TestImpl::doString(string arg1)
//{
//    printf("TestImpl::doString %s\n", arg1.c_str());
//    return arg1;
//}
//
//wchar_t TestImpl::doWChar(wchar_t arg1)
//{
//    printf("TestImpl::doWChar %s\n", arg1);
//    return arg1;
//}
//
//int64_t TestImpl::doInt64(int64_t arg1)
//{
//    printf("TestImpl::doInt64 %d\n", arg1);
//    return arg1;
//}

