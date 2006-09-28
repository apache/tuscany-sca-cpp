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
    virtual DataObjectPtr interop01(DataObjectPtr arg1);
    virtual DataObjectPtr interop02(DataObjectPtr arg1);
    virtual DataObjectPtr interop03(DataObjectPtr arg1);
    virtual DataObjectPtr interop04(DataObjectPtr arg1);
    virtual DataObjectPtr interop05(DataObjectPtr arg1);
    virtual string interop06(string arg1);
    virtual DataObjectPtr interop07(DataObjectPtr arg1);
    virtual DataObjectPtr interop08(DataObjectPtr arg1);
    virtual DataObjectPtr interop09(DataObjectPtr arg1);
    virtual DataObjectPtr interop10(DataObjectPtr arg1);
    virtual DataObjectPtr interop11(DataObjectPtr arg1);
    virtual DataObjectPtr interop12(DataObjectPtr arg1);
    //virtual DataObjectPtr interop13(DataObjectPtr arg1);
    virtual DataObjectPtr interop14(DataObjectPtr arg1);
    virtual DataObjectPtr interop15(DataObjectPtr arg1);
    virtual DataObjectPtr interop16(DataObjectPtr arg1);
    virtual DataObjectPtr interop17(DataObjectPtr arg1);
    virtual DataObjectPtr interop18(DataObjectPtr arg1);
    virtual DataObjectPtr interop19(DataObjectPtr arg1);
    virtual DataObjectPtr interop20(DataObjectPtr arg1);
    virtual DataObjectPtr interop21(DataObjectPtr arg1);
    virtual DataObjectPtr interop22(DataObjectPtr arg1);
    virtual DataObjectPtr interop23(DataObjectPtr arg1);
    virtual DataObjectPtr interop24(DataObjectPtr arg1);
    virtual DataObjectPtr interop25(DataObjectPtr arg1);
    virtual DataObjectPtr interop26(DataObjectPtr arg1);
    virtual DataObjectPtr interop27(DataObjectPtr arg1);
    virtual DataObjectPtr interop28(DataObjectPtr arg1);
    virtual DataObjectPtr interop29(DataObjectPtr arg1);
    virtual DataObjectPtr interop30(DataObjectPtr arg1);
    virtual DataObjectPtr interop31(DataObjectPtr arg1);
    virtual DataObjectPtr interop32(DataObjectPtr arg1);
    virtual DataObjectPtr interop33(DataObjectPtr arg1);
    virtual DataObjectPtr interop34(DataObjectPtr arg1);
    virtual DataObjectPtr interop35(DataObjectPtr arg1);
    virtual DataObjectPtr interop36(DataObjectPtr arg1);
    virtual DataObjectPtr interop37(DataObjectPtr arg1);
    virtual DataObjectPtr interop38(DataObjectPtr arg1);
    virtual DataObjectPtr interop39(DataObjectPtr arg1);
    virtual DataObjectPtr interop40(DataObjectPtr arg1);
    virtual DataObjectPtr interop41(DataObjectPtr arg1);
    virtual DataObjectPtr interop42(DataObjectPtr arg1);
    virtual DataObjectPtr interop43(DataObjectPtr arg1);
    virtual DataObjectPtr interop44(DataObjectPtr arg1);
    virtual DataObjectPtr interop45(DataObjectPtr arg1);
    virtual DataObjectPtr interop46(DataObjectPtr arg1);
    virtual DataObjectPtr interop47(DataObjectPtr arg1);
    //virtual DataObjectPtr interop48(DataObjectPtr arg1);
    //virtual DataObjectPtr interop49(DataObjectPtr arg1);
    virtual DataObjectPtr interop50(DataObjectPtr arg1);
private:
    virtual bool compareWithFile(DataObjectPtr arg1, string test);
};

#endif

