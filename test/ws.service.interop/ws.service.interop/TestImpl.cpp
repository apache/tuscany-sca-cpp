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

#include <tuscany/sca/util/Utils.h>
#include <tuscany/sca/core/SCARuntime.h>
#include <tuscany/sca/model/Component.h>
#include <tuscany/sca/model/Composite.h>
using namespace tuscany::sca;
#include <commonj/sdo/SDO.h>
using namespace commonj::sdo;

#include <stdio.h>

#include <string>
using std::string;

TestImpl::TestImpl()
{
}
    
TestImpl::~TestImpl()
{
}

bool TestImpl::compareWithFile(DataObjectPtr suppliedDO, string test)
{
    Composite* composite = SCARuntime::getInstance()->getCurrentComponent()->getComposite();
    string rootPath = composite->getRoot();
    string xmlFileName = rootPath+"/"+test+"-in.xml";

    XMLHelperPtr helper = composite->getXMLHelper();
    XMLDocumentPtr expectedDoc = helper->loadFile(xmlFileName.c_str());
    DataObjectPtr expectedDO = expectedDoc->getRootDataObject();

    printf( "Comparing supplied DataObject against expected DataObject\n");

    string diff = "";
    bool result = Utils::compareDataObjects(suppliedDO, expectedDO, diff);
    if(!result)
    {
        printf("Supplied DO differs from expected DO:\n%s\nSupplied DO:\n", diff.c_str() );
        Utils::printDO(suppliedDO);
        printf("Expected DO:\n");
        Utils::printDO(expectedDO);
    }
    else
    {
        printf("DataObjects match!\n");
    }

    return result;
}

DataObjectPtr TestImpl::interop01(DataObjectPtr arg1)
{
    printf("TestImpl::interop01 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop01"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop02(DataObjectPtr arg1)
{
    printf("TestImpl::interop02 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop02"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop03(DataObjectPtr arg1)
{
    printf("TestImpl::interop03 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop03"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop04(DataObjectPtr arg1)
{
    printf("TestImpl::interop04 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop04"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop05(DataObjectPtr arg1)
{
    printf("TestImpl::interop05 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop05"))
    {
        return arg1;
    }
    return NULL;
}
string TestImpl::interop06(string arg1)
{
    printf("TestImpl::interop06 %s\n", arg1.c_str());    
    return arg1;
}
DataObjectPtr TestImpl::interop07(DataObjectPtr arg1)
{
    printf("TestImpl::interop07 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop07"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop08(DataObjectPtr arg1)
{
    printf("TestImpl::interop08 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop08"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop09(DataObjectPtr arg1)
{
    printf("TestImpl::interop09 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop09"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop10(DataObjectPtr arg1)
{
    printf("TestImpl::interop10 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop10"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop11(DataObjectPtr arg1)
{
    printf("TestImpl::interop11 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop11"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop12(DataObjectPtr arg1)
{
    printf("TestImpl::interop12 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop12"))
    {
        return arg1;
    }
    return NULL;
}
// DataObjectPtr TestImpl::interop13(DataObjectPtr arg1)
// {
//     printf("TestImpl::interop13 %s\n", arg1->getType().getURI());
//     Utils::printDO(arg1);
//     return arg1;
// }
DataObjectPtr TestImpl::interop14(DataObjectPtr arg1)
{
    printf("TestImpl::interop14 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop14"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop15(DataObjectPtr arg1)
{
    printf("TestImpl::interop15 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop15"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop16(DataObjectPtr arg1)
{
    printf("TestImpl::interop16 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop16"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop17(DataObjectPtr arg1)
{
    printf("TestImpl::interop17 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop17"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop18(DataObjectPtr arg1)
{
    printf("TestImpl::interop18 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop18"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop19(DataObjectPtr arg1)
{
    printf("TestImpl::interop19 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop19"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop20(DataObjectPtr arg1)
{
    printf("TestImpl::interop20 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop20"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop21(DataObjectPtr arg1)
{
    printf("TestImpl::interop21 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop21"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop22(DataObjectPtr arg1)
{
    printf("TestImpl::interop22 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop22"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop23(DataObjectPtr arg1)
{
    printf("TestImpl::interop23 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop23"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop24(DataObjectPtr arg1)
{
    printf("TestImpl::interop24 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop24"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop25(DataObjectPtr arg1)
{
    printf("TestImpl::interop25 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop25"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop26(DataObjectPtr arg1)
{
    printf("TestImpl::interop26 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop26"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop27(DataObjectPtr arg1)
{
    printf("TestImpl::interop27 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop27"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop28(DataObjectPtr arg1)
{
    printf("TestImpl::interop28 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop28"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop29(DataObjectPtr arg1)
{
    printf("TestImpl::interop29 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop29"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop30(DataObjectPtr arg1)
{
    printf("TestImpl::interop30 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop30"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop31(DataObjectPtr arg1)
{
    printf("TestImpl::interop31 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop31"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop32(DataObjectPtr arg1)
{
    printf("TestImpl::interop32 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop32"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop33(DataObjectPtr arg1)
{
    printf("TestImpl::interop33 %s\n", arg1->getType().getURI());
    bool result = compareWithFile(arg1, "interop33");
    if(result)
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop34(DataObjectPtr arg1)
{
    printf("TestImpl::interop34 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop34"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop35(DataObjectPtr arg1)
{
    printf("TestImpl::interop35 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop35"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop36(DataObjectPtr arg1)
{
    printf("TestImpl::interop36 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop36"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop37(DataObjectPtr arg1)
{
    printf("TestImpl::interop37 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop37"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop38(DataObjectPtr arg1)
{
    printf("TestImpl::interop38 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop38"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop39(DataObjectPtr arg1)
{
    printf("TestImpl::interop39 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop39"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop40(DataObjectPtr arg1)
{
    printf("TestImpl::interop40 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop40"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop41(DataObjectPtr arg1)
{
    printf("TestImpl::interop41 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop41"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop42(DataObjectPtr arg1)
{
    printf("TestImpl::interop42 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop42"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop43(DataObjectPtr arg1)
{
    printf("TestImpl::interop43 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop43"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop44(DataObjectPtr arg1)
{
    printf("TestImpl::interop44 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop44"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop45(DataObjectPtr arg1)
{
    printf("TestImpl::interop45 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop45"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop46(DataObjectPtr arg1)
{
    printf("TestImpl::interop46 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop46"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::interop47(DataObjectPtr arg1)
{
    printf("TestImpl::interop47 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop47"))
    {
        return arg1;
    }
    return NULL;
}
// DataObjectPtr TestImpl::interop48(DataObjectPtr arg1)
// {
//     printf("TestImpl::interop48 %s\n", arg1->getType().getURI());
//     Utils::printDO(arg1);
//     return arg1;
// }
// DataObjectPtr TestImpl::interop49(DataObjectPtr arg1)
// {
//     printf("TestImpl::interop49 %s\n", arg1->getType().getURI());
//     Utils::printDO(arg1);
//     return arg1;
// }
DataObjectPtr TestImpl::interop50(DataObjectPtr arg1)
{
    printf("TestImpl::interop50 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "interop50"))
    {
        return arg1;
    }
    return NULL;
}
