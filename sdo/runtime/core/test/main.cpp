/*
 *
 *  Copyright 2006 The Apache Software Foundation or its licensors, as applicable.
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

/* $Id: main.cpp,v 1.1 2006/03/16 10:54:16 slattery Exp $ */

#include <stdio.h>

#pragma warning(disable:4786)

#include <iostream>
using namespace std;

#include "sdotest.h"

extern "C"{

#define TEST(testname)\
    value = testname;\
    if (value == 0) {\
        cout << "Test Failed:" << totaltests  << endl;\
    }\
    testspassed += value;\
    totaltests++;

int main (int argc, char** argv)
{
    //Logger::setLogging(20);

    int testspassed=0;
    int totaltests=0;
    int value = 0;

    TEST ( sdotest::xhtml1() );
    TEST ( sdotest::scopetest() );
    TEST ( sdotest::xsdtosdo() );

    /* groups now supported*/
    TEST ( sdotest::testany("list1.xsd",0,"list1.xml", 0) );
    TEST ( sdotest::testany("b48636.xsd",0,"b48636.xml", 0) );
    TEST ( sdotest::testany("groupwithprefix.xsd", 0, 0, 0) );
    TEST ( sdotest::testany("groupwrongname.xsd","Use of undefined group NameGroup", 0, 0) );

    // late definition of group now supported
    TEST ( sdotest::testany("grouptoolate.xsd",0 , 0, 0) );

    // group definition within group now ignored

    TEST ( sdotest::testany("groupingroup.xsd","Use of undefined group NameGroup2",0, 0) );
    TEST ( sdotest::testany("grouprefingroup.xsd",0,0, 0) );
    TEST ( sdotest::testany("bothgroups.xsd",0,0, 0) );
    TEST ( sdotest::testany("bothgroupssamename.xsd",0,0, 0) );

    /* general tests */
    TEST (  sdotest::pete() );
    TEST (  sdotest::stocktest() );
    TEST (  sdotest::definetest() );
    TEST (  sdotest::emptycs() );
    TEST (  sdotest::openseq() );
    TEST (  sdotest::testanytwo("b48633b.xsd","b48633.xsd","b48633.xml") );
    TEST (  sdotest::testany("b48686.xsd",0, "b48686.xml", 0) );
    TEST (  sdotest::testany("b48636.xsd","Use of undefined group NameGroup","b48636.xml", 0) );
    TEST (  sdotest::b48602() );
    TEST (  sdotest::compatiblefactory() );
    TEST (  sdotest::simple() );
    TEST (  sdotest::testOrder("employee.xsd","extension.xsd","order1.dat","order1.txt") );
    TEST (  sdotest::testOrder("extension.xsd","employee.xsd","order2.dat","order2.txt") );
    TEST (  sdotest::testOpen() );
    TEST (  sdotest::testinc2("calculator2.wsdl","inc1.dat","inc1.txt") );
    TEST (  sdotest::testinc2("calculator2a.wsdl","inc2.dat","inc2.txt") );
    TEST (  sdotest::teststyles() );
    TEST (  sdotest::badelement() );
    TEST (  sdotest::oddchars() );
    TEST (  sdotest::notns() );
    TEST (  sdotest::unsetlist() );
    TEST (  sdotest::travel() );
    TEST (  sdotest::testwsdl() );
    TEST (  sdotest::bunique() );
    TEST (  sdotest::buniqueread() );
    TEST (  sdotest::propdefaulttest() );
    TEST (  sdotest::graham5() );
    TEST (  sdotest::graham4() );
    TEST (  sdotest::detachtest() );
    TEST (  sdotest::includetest() );
    TEST (  sdotest::testLoad() );
    TEST (  sdotest::leak() );
    TEST (  sdotest::twolists() );
    TEST (  sdotest::testUtil() );
    TEST (  sdotest::clonetest() );
    TEST (  sdotest::notfound() );
    TEST (  sdotest::testErrors() );
    TEST (  sdotest::loadManyOpen() );
    TEST (  sdotest::doctest() );
    TEST (  sdotest::graham3() );
    TEST (  sdotest::graham1() );
    TEST (  sdotest::graham2() );
    TEST (  sdotest::merle1() );
    TEST (  sdotest::loadOpenNS() );
    TEST (  sdotest::saveOpen() );
    TEST (  sdotest::loadOpen() );
    TEST (  sdotest::testui() );
    TEST (  sdotest::testSCA() );
    TEST (  sdotest::testabstract() );
    TEST (  sdotest::testsubsload() );
    TEST (  sdotest::testsubs() );
    TEST (  sdotest::setnull() );
    TEST (  sdotest::bug2() );
    TEST (  sdotest::cssave2() );
    TEST (  sdotest::csload2() );
    TEST (  sdotest::cssave() );
    TEST (  sdotest::csload() );
    TEST (  sdotest::bug1() );
    TEST (  sdotest::eqhelpertest() );
    TEST (  sdotest::cohelpertest() );
    TEST (  sdotest::datetest() );
    TEST (  sdotest::carotest3() );
    TEST (  sdotest::matttest1() );
    TEST (  sdotest::adddeletetest() );
    TEST (  sdotest::carotest2() );
    TEST (  sdotest::carotest() );
    TEST (  sdotest::setmany() );
    TEST (  sdotest::noncontest() );
    TEST (  sdotest::versiontest() );
    TEST (  sdotest::defaulttest() );
    TEST (  sdotest::defaulttest_strobj() );
    TEST (  sdotest::nulltest() );
    TEST (  sdotest::usertest() );
    TEST (  sdotest::querytest() );
    TEST (  sdotest::getproptest() );
    TEST (  sdotest::rcptest() );
    TEST (  sdotest::seqtest() );
    TEST (  sdotest::changesummarytest() );
    TEST (  sdotest::conversiontest() );
    TEST (  sdotest::boolbug() );
    TEST (  sdotest::scope1() );
    TEST (  sdotest::scope2() );
    TEST (  sdotest::scope3() );
    TEST (  sdotest::maintest() );

    /* regression tests */
    TEST (  sdotest::b48736() );
    TEST (  sdotest::b48601() );
    TEST (  sdotest::b47293() );
    TEST (  sdotest::b48300() );
    TEST (  sdotest::b47663() );
    TEST (  sdotest::b47802() );
    TEST (  sdotest::b47137() );
    TEST (  sdotest::b47137b() );
    TEST (  sdotest::b46633() );
    TEST (  sdotest::b46693() );
    TEST (  sdotest::b46734() );
    TEST (  sdotest::b46634() );
    TEST (  sdotest::b46617b() );
    TEST (  sdotest::b46617() );
    TEST (  sdotest::b46613() );
    TEST (  sdotest::b45933() );

    cout << "Total tests:" << totaltests << " Tests passed:" << testspassed << endl;

return 0;
}
}
