/*
 *
 *  Copyright 2005 International Business Machines Corporation
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

int main (int argc, char** argv)
{
    //Logger::setLogging(20);

    int testspassed=0;
    int totaltests=0;

    /* general tests */

    totaltests++;testspassed += sdotest::emptycs();
    totaltests++;testspassed += sdotest::openseq();
    totaltests++;testspassed += sdotest::testanytwo("b48633b.xsd","b48633.xsd","b48633.xml");
    totaltests++;testspassed += sdotest::testany("b48686.xsd",
        "Schema contains a list which is not yet implemented", "b48686.xml", 0);
    totaltests++;testspassed += sdotest::testany("b48636.xsd",
        "Schema contains a group or attributeGroup which are not yet implemented","b48636.xml", 0);
    totaltests++;testspassed += sdotest::b48602();
    totaltests++;testspassed += sdotest::compatiblefactory();
    totaltests++;testspassed += sdotest::simple();
    totaltests++;testspassed += sdotest::testOrder("employee.xsd","extension.xsd",
        "order1.dat","order1.txt");
    totaltests++;testspassed += sdotest::testOrder("extension.xsd","employee.xsd",
        "order2.dat","order2.txt");
    totaltests++;testspassed += sdotest::testOpen();
    totaltests++;testspassed += sdotest::testinc2("calculator2.wsdl","inc1.dat","inc1.txt");
    totaltests++;testspassed += sdotest::testinc2("calculator2a.wsdl","inc2.dat","inc2.txt");
    totaltests++;testspassed += sdotest::teststyles();
    totaltests++;testspassed += sdotest::badelement();
    totaltests++;testspassed += sdotest::oddchars();
    totaltests++;testspassed += sdotest::notns();
    totaltests++;testspassed += sdotest::unsetlist();
    totaltests++;testspassed += sdotest::travel();
    totaltests++;testspassed += sdotest::testwsdl();
    totaltests++;testspassed += sdotest::bunique();
    totaltests++;testspassed += sdotest::buniqueread();
    totaltests++;testspassed += sdotest::propdefaulttest();
    totaltests++;testspassed += sdotest::graham5();
    totaltests++;testspassed += sdotest::graham4();
    totaltests++;testspassed += sdotest::detachtest();
    totaltests++;testspassed += sdotest::includetest();
    totaltests++;testspassed += sdotest::testLoad();
    totaltests++;testspassed += sdotest::leak();
    totaltests++;testspassed += sdotest::twolists();
    totaltests++;testspassed += sdotest::testUtil();
    totaltests++;testspassed += sdotest::clonetest();
    totaltests++;testspassed += sdotest::notfound();
    totaltests++;testspassed += sdotest::testErrors();
    totaltests++;testspassed += sdotest::loadManyOpen();
    totaltests++;testspassed += sdotest::doctest();
    totaltests++;testspassed += sdotest::graham3();
    totaltests++;testspassed += sdotest::graham1();
    totaltests++;testspassed += sdotest::graham2();
    totaltests++;testspassed += sdotest::merle1();
    totaltests++;testspassed += sdotest::loadOpenNS();
    totaltests++;testspassed += sdotest::saveOpen();
    totaltests++;testspassed += sdotest::loadOpen();
    totaltests++;testspassed += sdotest::testui();
    totaltests++;testspassed += sdotest::testSCA();
    totaltests++;testspassed += sdotest::testabstract();
    totaltests++;testspassed += sdotest::testsubsload();
    totaltests++;testspassed += sdotest::testsubs();
    totaltests++;testspassed += sdotest::setnull();
    totaltests++;testspassed += sdotest::bug2();
    totaltests++;testspassed += sdotest::cssave2();
    totaltests++;testspassed += sdotest::csload2();
    totaltests++;testspassed += sdotest::cssave();
    totaltests++;testspassed += sdotest::csload();
    totaltests++;testspassed += sdotest::bug1();
    totaltests++;testspassed += sdotest::eqhelpertest();
    totaltests++;testspassed += sdotest::cohelpertest();
    totaltests++;testspassed += sdotest::datetest();
    totaltests++;testspassed += sdotest::carotest3();
    totaltests++;testspassed += sdotest::matttest1();
    totaltests++;testspassed += sdotest::adddeletetest();
    totaltests++;testspassed += sdotest::carotest2();
    totaltests++;testspassed += sdotest::carotest();
    totaltests++;testspassed += sdotest::setmany();
    totaltests++;testspassed += sdotest::noncontest();
    totaltests++;testspassed += sdotest::versiontest();
    totaltests++;testspassed += sdotest::defaulttest();
    totaltests++;testspassed += sdotest::nulltest();
    totaltests++;testspassed += sdotest::usertest();
    totaltests++;testspassed += sdotest::querytest();
    totaltests++;testspassed += sdotest::getproptest();
    totaltests++;testspassed += sdotest::rcptest();
    totaltests++;testspassed += sdotest::seqtest();
    totaltests++;testspassed += sdotest::changesummarytest();
    totaltests++;testspassed += sdotest::conversiontest();
    totaltests++;testspassed += sdotest::boolbug();
    totaltests++;testspassed += sdotest::scope1();
    totaltests++;testspassed += sdotest::scope2();
    totaltests++;testspassed += sdotest::scope3();
    totaltests++;testspassed += sdotest::maintest();

    /* regression tests */
    totaltests++;testspassed += sdotest::b48736();
    totaltests++;testspassed += sdotest::b48601();
    totaltests++;testspassed += sdotest::b47293();
    totaltests++;testspassed += sdotest::b48300();
    totaltests++;testspassed += sdotest::b47663();
    totaltests++;testspassed += sdotest::b47802();
    totaltests++;testspassed += sdotest::b47137();
    totaltests++;testspassed += sdotest::b47137b();
    totaltests++;testspassed += sdotest::b46633();
    totaltests++;testspassed += sdotest::b46693();
    totaltests++;testspassed += sdotest::b46734();
    totaltests++;testspassed += sdotest::b46634();
    totaltests++;testspassed += sdotest::b46617b();
    totaltests++;testspassed += sdotest::b46617();
    totaltests++;testspassed += sdotest::b46613();
    totaltests++;testspassed += sdotest::b45933();

    cout << "Total tests:" << totaltests << " Tests passed:" << testspassed;

return 0;
}
}
