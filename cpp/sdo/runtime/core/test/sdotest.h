/*
 *
 *  Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
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

/* $Rev$ $Date: 2005/12/22 16:54:31 $ */

#include "commonj/sdo/SDO.h"
using namespace commonj::sdo;

class sdotest {
    public:


        static void changeSummaryHeader(ChangeSummaryPtr cs);
        static void changeSummaryFooter();
        static void changeSummaryChange(int level, ChangeSummaryPtr cs, DataObjectPtr dob);
        static void changeSummaryDeletedObject(int indent, const char* name, ChangeSummaryPtr cs, DataObjectPtr dob);
        static void changeSummaryElements(int indent, ChangeSummaryPtr cs, DataObjectPtr dob);
        static void changeSummarySetting(Setting& s);
        static bool changeSummaryAttributes(ChangeSummaryPtr cs, DataObjectPtr dol);

        static void b47293();
        static void propdefaulttest();
        static void showdefault(const Type& tm);
        static void graham5();
        static void graham4();
        static void leak();
        static void twolists();
        static void printset(ChangeSummaryPtr cs);
        static void detachtest();
        static void includetest();
        static void testLoad();
        static void b46633();
        static void clonetest();
        static void testUtil();
        static void testErrors();
        static void b46734();
        static void b46693();
        static void b46634();
        static void b46613();
        static void b46617();
        static void b46617b();
        static void doctest();
        static void notfound();


        static void serializeChangeSummary(ChangeSummaryPtr cs);
        static void cssave();
        static void csload();
        static void cssave2();
        static void csload2();
        static void bug2();
        static void setnull();
        static void bug45933();
        static void testsubsload();
        static void testabstract();
        static void testsubs();
        static void testSCA();
        static void testOpen();
        static void saveOpen();
        static void loadOpen();
        static void testui();
        static void merle1();
        static void graham1();
        static void graham2();
        static void graham3();
        static void loadManyOpen();
        static void loadOpenNS();
        
        
        static  void eqhelpertest();
        static  void cohelpertest();
        static  void datetest();
        static  void matttest1();
        static  void adddeletetest();
        static  void carotest3();
        static  void carotest2();
        static  void carotest();
        static  void setmany();
        static  void noncontest();
        static  void versiontest();
        static  void defaulttest();
        static void querytest();
        static void getproptest();
        static void nulltest();
        static void usertest();
        static void rcptest();
        static void boolbug();
        static void scope1();
        static void scope2();
        static void scope3();
        static void conversiontest();
        static void changesummarytest();
        static void scenario1();
        static void scenario2();
        static void scenario3();
        static void scenario4();
        static void scenario5();
        static void printseq(SequencePtr sptr);
        static void seqtest();
        static void printDataObject(DataObjectPtr dol);
        static void printValue(DataObjectPtr dob, const Property& prop);
        static void printList(DataObjectPtr dob, const Property& prop);
        static void printOldValues(ChangeSummaryPtr cs, DataObjectPtr dob);
        static void dumpchangesummary(ChangeSummaryPtr cs);
        static void dumpproperties(DataObjectPtr root);
        static void testGetters(DataObjectPtr dor);
        static void testGetter(DataObjectPtr dor, char* str);
        static int main(int argc, char** argv);
};