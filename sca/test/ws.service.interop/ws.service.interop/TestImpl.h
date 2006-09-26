/* 
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

#ifndef TESTIMPL_H
#define TESTIMPL_H

#include "Test.h"

class TestImpl : public Test
{
public:
    TestImpl();
    virtual ~TestImpl();

    // Test interface                
    virtual DataObjectPtr Interop01(DataObjectPtr arg1);
    virtual DataObjectPtr Interop02(DataObjectPtr arg1);
    virtual DataObjectPtr Interop03(DataObjectPtr arg1);
    virtual DataObjectPtr Interop04(DataObjectPtr arg1);
    virtual DataObjectPtr Interop05(DataObjectPtr arg1);
    virtual string Interop06(string arg1);
    virtual DataObjectPtr Interop07(DataObjectPtr arg1);
    virtual DataObjectPtr Interop08(DataObjectPtr arg1);
    virtual DataObjectPtr Interop09(DataObjectPtr arg1);
    virtual DataObjectPtr Interop10(DataObjectPtr arg1);
    virtual DataObjectPtr Interop11(DataObjectPtr arg1);
    virtual DataObjectPtr Interop12(DataObjectPtr arg1);
    //virtual DataObjectPtr Interop13(DataObjectPtr arg1);
    virtual DataObjectPtr Interop14(DataObjectPtr arg1);
    virtual DataObjectPtr Interop15(DataObjectPtr arg1);
    virtual DataObjectPtr Interop16(DataObjectPtr arg1);
    virtual DataObjectPtr Interop17(DataObjectPtr arg1);
    virtual DataObjectPtr Interop18(DataObjectPtr arg1);
    virtual DataObjectPtr Interop19(DataObjectPtr arg1);
    virtual DataObjectPtr Interop20(DataObjectPtr arg1);
    virtual DataObjectPtr Interop21(DataObjectPtr arg1);
    virtual DataObjectPtr Interop22(DataObjectPtr arg1);
    virtual DataObjectPtr Interop23(DataObjectPtr arg1);
    virtual DataObjectPtr Interop24(DataObjectPtr arg1);
    virtual DataObjectPtr Interop25(DataObjectPtr arg1);
    virtual DataObjectPtr Interop26(DataObjectPtr arg1);
    virtual DataObjectPtr Interop27(DataObjectPtr arg1);
    virtual DataObjectPtr Interop28(DataObjectPtr arg1);
    virtual DataObjectPtr Interop29(DataObjectPtr arg1);
    virtual DataObjectPtr Interop30(DataObjectPtr arg1);
    virtual DataObjectPtr Interop31(DataObjectPtr arg1);
    virtual DataObjectPtr Interop32(DataObjectPtr arg1);
    virtual DataObjectPtr Interop33(DataObjectPtr arg1);
    virtual DataObjectPtr Interop34(DataObjectPtr arg1);
    virtual DataObjectPtr Interop35(DataObjectPtr arg1);
    virtual DataObjectPtr Interop36(DataObjectPtr arg1);
    virtual DataObjectPtr Interop37(DataObjectPtr arg1);
    virtual DataObjectPtr Interop38(DataObjectPtr arg1);
    virtual DataObjectPtr Interop39(DataObjectPtr arg1);
    virtual DataObjectPtr Interop40(DataObjectPtr arg1);
    virtual DataObjectPtr Interop41(DataObjectPtr arg1);
    virtual DataObjectPtr Interop42(DataObjectPtr arg1);
    virtual DataObjectPtr Interop43(DataObjectPtr arg1);
    virtual DataObjectPtr Interop44(DataObjectPtr arg1);
    virtual DataObjectPtr Interop45(DataObjectPtr arg1);
    virtual DataObjectPtr Interop46(DataObjectPtr arg1);
    virtual DataObjectPtr Interop47(DataObjectPtr arg1);
    //virtual DataObjectPtr Interop48(DataObjectPtr arg1);
    //virtual DataObjectPtr Interop49(DataObjectPtr arg1);
    virtual DataObjectPtr Interop50(DataObjectPtr arg1);
private:
    virtual bool compareWithFile(DataObjectPtr arg1, string test);
};

#endif

