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

#include "WSEntryPointTestImpl.h"
#include <stdio.h>
#include <tuscany/sca/util/Utils.h>
using namespace tuscany::sca;
#include <commonj/sdo/SDO.h>
using namespace commonj::sdo;

WSEntryPointTestImpl::WSEntryPointTestImpl()
{
}
    
WSEntryPointTestImpl::~WSEntryPointTestImpl()
{
}

// WSEntryPointTest interface
DataObjectPtr WSEntryPointTestImpl::doDataObject(DataObjectPtr arg1)
{
    printf("WSEntryPointTestImpl::doDataObject %s\n", arg1->getType().getURI());
    Utils::printDO(arg1);
    return arg1;
}

char* WSEntryPointTestImpl::doChars(char* arg1)
{
    printf("WSEntryPointTestImpl::doChars %s\n", arg1);
    return arg1;
}


long WSEntryPointTestImpl::doLong(long arg1)
{
    printf("WSEntryPointTestImpl::doLong %d\n", arg1);
    return arg1;
}

int WSEntryPointTestImpl::doInt(int arg1)
{
    printf("WSEntryPointTestImpl::doInt %d\n", arg1);
    return arg1;
}

float WSEntryPointTestImpl::doFloat(float arg1)
{
    printf("WSEntryPointTestImpl::doFloat %d\n", arg1);
    return arg1;
}

long double WSEntryPointTestImpl::doLongDouble(long double arg1)
{
    printf("WSEntryPointTestImpl::doLongDouble %d\n", arg1);
    return arg1;
}

double WSEntryPointTestImpl::doDouble(double arg1)
{
    printf("WSEntryPointTestImpl::doDouble %d\n", arg1);
    return arg1;
}


bool WSEntryPointTestImpl::doBool(bool arg1)
{
    printf("WSEntryPointTestImpl::doBool %d\n", arg1);
    return arg1;
}

short WSEntryPointTestImpl::doShort(short arg1)
{
    printf("WSEntryPointTestImpl::doShort %d\n", arg1);
    return arg1;
}

char* WSEntryPointTestImpl::doBytes(char* arg1)
{
    printf("WSEntryPointTestImpl::doBytes %s\n", arg1);
    return arg1;
}

DataObjectPtr WSEntryPointTestImpl::doMixed(char* arg1, long arg2, DataObjectPtr arg3, bool arg4, double arg5)
{
    printf("WSEntryPointTestImpl::doMixed %s %d %s %d %d\n", arg1, arg2, arg3->getType().getURI(), arg4, arg5);
    Utils::printDO(arg3);
    return arg3;
}


//int[] WSEntryPointTestImpl::doIntArray(int[] arg1, int arg2)
//{
//    printf("WSEntryPointTestImpl::doIntArray with length %d\n", arg2);
//    for(int i=0; i<arg2; i++)
//    {
//        printf("%d\n", arg1[i]);
//    }
//    return arg1;
//}
//
//string WSEntryPointTestImpl::doString(string arg1)
//{
//    printf("WSEntryPointTestImpl::doString %s\n", arg1.c_str());
//    return arg1;
//}
//
//wchar_t WSEntryPointTestImpl::doWChar(wchar_t arg1)
//{
//    printf("WSEntryPointTestImpl::doWChar %s\n", arg1);
//    return arg1;
//}
//
//int64_t WSEntryPointTestImpl::doInt64(int64_t arg1)
//{
//    printf("WSEntryPointTestImpl::doInt64 %d\n", arg1);
//    return arg1;
//}

