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

#include "WSCompositeServiceTypeTestImpl.h"
#include <stdio.h>
#include <tuscany/sca/util/Utils.h>
using namespace tuscany::sca;
#include <commonj/sdo/SDO.h>
#include <sdo_axiom.h>
using namespace commonj::sdo;
using namespace commonj::sdo_axiom;

WSCompositeServiceTypeTestImpl::WSCompositeServiceTypeTestImpl()
{
}
    
WSCompositeServiceTypeTestImpl::~WSCompositeServiceTypeTestImpl()
{
}

// WSCompositeServiceTypeTest interface
char* WSCompositeServiceTypeTestImpl::doChars(char* arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doChars %s\n", arg1);
    return arg1;
}


long WSCompositeServiceTypeTestImpl::doLong(long arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doLong %d\n", arg1);
    return arg1;
}

int WSCompositeServiceTypeTestImpl::doInt(int arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doInt %d\n", arg1);
    return arg1;
}

float WSCompositeServiceTypeTestImpl::doFloat(float arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doFloat %f\n", arg1);
    return arg1;
}

long double WSCompositeServiceTypeTestImpl::doLongDouble(long double arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doLongDouble %f\n", arg1);
    return arg1;
}

double WSCompositeServiceTypeTestImpl::doDouble(double arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doDouble %f\n", arg1);
    return arg1;
}


bool WSCompositeServiceTypeTestImpl::doBool(bool arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doBool %d\n", arg1);
    return arg1;
}

short WSCompositeServiceTypeTestImpl::doShort(short arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doShort %d\n", arg1);
    return arg1;
}

char* WSCompositeServiceTypeTestImpl::doBytes(char* arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doBytes %s\n", arg1);
    return arg1;
}

char WSCompositeServiceTypeTestImpl::doByte(char arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doByte %c\n", arg1);
    return arg1;
}

DataObjectPtr WSCompositeServiceTypeTestImpl::doMixed(char* arg1, long arg2, DataObjectPtr arg3, bool arg4, double arg5)
{
    printf("WSCompositeServiceTypeTestImpl::doMixed %s %d %s %d %d\n", arg1, arg2, arg3->getType().getURI(), arg4, arg5);
    Utils::printDO(arg3);
    return arg3;
}

DataObjectPtr WSCompositeServiceTypeTestImpl::doDataObject(DataObjectPtr arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doDataObject %s\n", arg1->getType().getURI());
    Utils::printDO(arg1);
    return arg1;
}

DataObjectPtr WSCompositeServiceTypeTestImpl::doAny(DataObjectPtr arg1)
{
    printf("WSCompositeServiceTypeTestImpl::doAny %s#%s\n", arg1->getType().getURI(), arg1->getType().getName());
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

//int[] WSCompositeServiceTypeTestImpl::doIntArray(int[] arg1, int arg2)
//{
//    printf("WSCompositeServiceTypeTestImpl::doIntArray with length %d\n", arg2);
//    for(int i=0; i<arg2; i++)
//    {
//        printf("%d\n", arg1[i]);
//    }
//    return arg1;
//}
//
//string WSCompositeServiceTypeTestImpl::doString(string arg1)
//{
//    printf("WSCompositeServiceTypeTestImpl::doString %s\n", arg1.c_str());
//    return arg1;
//}
//
//wchar_t WSCompositeServiceTypeTestImpl::doWChar(wchar_t arg1)
//{
//    printf("WSCompositeServiceTypeTestImpl::doWChar %s\n", arg1);
//    return arg1;
//}
//
//int64_t WSCompositeServiceTypeTestImpl::doInt64(int64_t arg1)
//{
//    printf("WSCompositeServiceTypeTestImpl::doInt64 %d\n", arg1);
//    return arg1;
//}

