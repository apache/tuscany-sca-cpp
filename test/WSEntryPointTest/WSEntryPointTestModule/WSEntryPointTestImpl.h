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

#ifndef WSEENTRYPOINTTESTIMPL_H
#define WSEENTRYPOINTTESTIMPL_H

#include "WSEntryPointTest.h"

class WSEntryPointTestImpl : public WSEntryPointTest
{
public:
    WSEntryPointTestImpl();
    virtual ~WSEntryPointTestImpl();

    // WSEntryPointTest interface
    virtual char* doChars(char* arg1);
    virtual long doLong(long arg1);
    virtual int doInt(int arg1);
    virtual float doFloat(float arg1);
    virtual long double doLongDouble(long double arg1);
    virtual double doDouble(double arg1);
    virtual bool doBool(bool arg1);
    virtual short doShort(short arg1);
    virtual char* doBytes(char* arg1);
    virtual char doByte(char arg1);
	virtual DataObjectPtr doMixed(char* arg1, long arg2, DataObjectPtr arg3, bool arg4, double arg5);
	virtual DataObjectPtr doDataObject(DataObjectPtr arg1);
	virtual DataObjectPtr doAny(DataObjectPtr arg1);

// These don't work because they are passing pointers around
//    virtual int[] doIntArray(int[] arg1, int arg2);
//    virtual string doString(string arg1);
// This doesn't work because there is no mapping from XSD types to SDO CharacterType     
//    virtual wchar_t doWChar(wchar_t arg1);
// This doesn't work because Operation does not support int64_t yet
//    virtual int64_t doInt64(int64_t arg1);

};

#endif

