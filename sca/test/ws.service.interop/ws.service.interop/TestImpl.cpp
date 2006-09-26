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
#include <axiom.h>
#include <axis2_util.h>

#include <tuscany/sca/util/Utils.h>
#include <tuscany/sca/core/SCARuntime.h>
#include <tuscany/sca/model/Component.h>
#include <tuscany/sca/model/Composite.h>
using namespace tuscany::sca;
#include <commonj/sdo/SDO.h>
using namespace commonj::sdo;
#include <sdo_axiom.h>
using namespace commonj::sdo_axiom;

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

DataObjectPtr TestImpl::Interop01(DataObjectPtr arg1)
{
    printf("TestImpl::Interop01 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop01"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop02(DataObjectPtr arg1)
{
    printf("TestImpl::Interop02 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop02"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop03(DataObjectPtr arg1)
{
    printf("TestImpl::Interop03 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop03"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop04(DataObjectPtr arg1)
{
    printf("TestImpl::Interop04 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop04"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop05(DataObjectPtr arg1)
{
    printf("TestImpl::Interop05 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop05"))
    {
        return arg1;
    }
    return NULL;
}
string TestImpl::Interop06(string arg1)
{
    printf("TestImpl::Interop06 %s\n", arg1.c_str());    
    return arg1;
}
DataObjectPtr TestImpl::Interop07(DataObjectPtr arg1)
{
    printf("TestImpl::Interop07 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop07"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop08(DataObjectPtr arg1)
{
    printf("TestImpl::Interop08 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop08"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop09(DataObjectPtr arg1)
{
    printf("TestImpl::Interop09 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop09"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop10(DataObjectPtr arg1)
{
    printf("TestImpl::Interop10 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop10"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop11(DataObjectPtr arg1)
{
    printf("TestImpl::Interop11 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop11"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop12(DataObjectPtr arg1)
{
    printf("TestImpl::Interop12 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop12"))
    {
        return arg1;
    }
    return NULL;
}
// DataObjectPtr TestImpl::Interop13(DataObjectPtr arg1)
// {
//     printf("TestImpl::Interop13 %s\n", arg1->getType().getURI());
//     Utils::printDO(arg1);
//     return arg1;
// }
DataObjectPtr TestImpl::Interop14(DataObjectPtr arg1)
{
    printf("TestImpl::Interop14 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop14"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop15(DataObjectPtr arg1)
{
    printf("TestImpl::Interop15 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop15"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop16(DataObjectPtr arg1)
{
    printf("TestImpl::Interop16 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop16"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop17(DataObjectPtr arg1)
{
    printf("TestImpl::Interop17 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop17"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop18(DataObjectPtr arg1)
{
    printf("TestImpl::Interop18 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop18"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop19(DataObjectPtr arg1)
{
    printf("TestImpl::Interop19 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop19"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop20(DataObjectPtr arg1)
{
    printf("TestImpl::Interop20 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop20"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop21(DataObjectPtr arg1)
{
    printf("TestImpl::Interop21 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop21"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop22(DataObjectPtr arg1)
{
    printf("TestImpl::Interop22 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop22"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop23(DataObjectPtr arg1)
{
    printf("TestImpl::Interop23 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop23"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop24(DataObjectPtr arg1)
{
    printf("TestImpl::Interop24 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop24"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop25(DataObjectPtr arg1)
{
    printf("TestImpl::Interop25 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop25"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop26(DataObjectPtr arg1)
{
    printf("TestImpl::Interop26 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop26"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop27(DataObjectPtr arg1)
{
    printf("TestImpl::Interop27 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop27"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop28(DataObjectPtr arg1)
{
    printf("TestImpl::Interop28 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop28"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop29(DataObjectPtr arg1)
{
    printf("TestImpl::Interop29 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop29"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop30(DataObjectPtr arg1)
{
    printf("TestImpl::Interop30 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop30"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop31(DataObjectPtr arg1)
{
    printf("TestImpl::Interop31 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop31"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop32(DataObjectPtr arg1)
{
    printf("TestImpl::Interop32 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop32"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop33(DataObjectPtr arg1)
{
    printf("TestImpl::Interop33 %s\n", arg1->getType().getURI());
    bool result = true;
    try
    {

        result = compareWithFile(arg1, "Interop33");
    }
    catch(...)
    {
        cout<<"Caught exception"<<endl;
    }
    if(result)
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop34(DataObjectPtr arg1)
{
    printf("TestImpl::Interop34 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop34"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop35(DataObjectPtr arg1)
{
    printf("TestImpl::Interop35 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop35"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop36(DataObjectPtr arg1)
{
    printf("TestImpl::Interop36 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop36"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop37(DataObjectPtr arg1)
{
    printf("TestImpl::Interop37 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop37"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop38(DataObjectPtr arg1)
{
    printf("TestImpl::Interop38 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop38"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop39(DataObjectPtr arg1)
{
    printf("TestImpl::Interop39 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop39"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop40(DataObjectPtr arg1)
{
    printf("TestImpl::Interop40 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop40"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop41(DataObjectPtr arg1)
{
    printf("TestImpl::Interop41 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop41"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop42(DataObjectPtr arg1)
{
    printf("TestImpl::Interop42 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop42"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop43(DataObjectPtr arg1)
{
    printf("TestImpl::Interop43 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop43"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop44(DataObjectPtr arg1)
{
    printf("TestImpl::Interop44 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop44"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop45(DataObjectPtr arg1)
{
    printf("TestImpl::Interop45 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop45"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop46(DataObjectPtr arg1)
{
    printf("TestImpl::Interop46 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop46"))
    {
        return arg1;
    }
    return NULL;
}
DataObjectPtr TestImpl::Interop47(DataObjectPtr arg1)
{
    printf("TestImpl::Interop47 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop47"))
    {
        return arg1;
    }
    return NULL;
}
// DataObjectPtr TestImpl::Interop48(DataObjectPtr arg1)
// {
//     printf("TestImpl::Interop48 %s\n", arg1->getType().getURI());
//     Utils::printDO(arg1);
//     return arg1;
// }
// DataObjectPtr TestImpl::Interop49(DataObjectPtr arg1)
// {
//     printf("TestImpl::Interop49 %s\n", arg1->getType().getURI());
//     Utils::printDO(arg1);
//     return arg1;
// }
DataObjectPtr TestImpl::Interop50(DataObjectPtr arg1)
{
    printf("TestImpl::Interop50 %s\n", arg1->getType().getURI());
    if(compareWithFile(arg1, "Interop50"))
    {
        return arg1;
    }
    return NULL;
}
