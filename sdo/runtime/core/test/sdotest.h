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

/* $Id: sdotest.h,v 1.21 2006/03/16 12:21:04 slattery Exp $ */

#include "commonj/sdo/SDO.h"
using namespace commonj::sdo;


// test

class sdotest {
    public:

        static bool silent;
        static bool ramping;

        /* utilities */

        static int  printseq(FILE *f, SequencePtr sptr);
        static void printDataStructure(FILE *f,DataFactory* dd);
        static void printDataObject(FILE *f, DataObjectPtr dol);
        static void printValue(FILE *f, DataObjectPtr dob, const Property& prop);
        static void printList(FILE *f, DataObjectPtr dob, const Property& prop);
        static void printOldValues(FILE *f, ChangeSummaryPtr cs, DataObjectPtr dob);
        static void dumpchangesummary(FILE *f, ChangeSummaryPtr cs);
        static int  comparefiles(char* fn1, char*fn2);
        static int dumpproperties(FILE *f, DataObjectPtr root);
        static int printset(FILE *f, ChangeSummaryPtr cs);

        static int xsdtosdo();

        static void changeSummaryHeader(ChangeSummaryPtr cs);
        static void changeSummaryFooter();
        static void changeSummaryChange(int level, ChangeSummaryPtr cs, DataObjectPtr dob);
        static void changeSummaryDeletedObject(int indent, const char* name, ChangeSummaryPtr cs, DataObjectPtr dob);
        static void changeSummaryElements(int indent, ChangeSummaryPtr cs, DataObjectPtr dob);
        static void changeSummarySetting(Setting& s);
        static bool changeSummaryAttributes(ChangeSummaryPtr cs, DataObjectPtr dol);
        static char* convert(const char* uri);

        /* work in progress - generation of structures */

        static void generate(DataFactoryPtr fac);
 
        /* tests */
        static int xhtml1();
        static int definetest();
        static int stocktest();

        static int scopetest();
        static DataObjectPtr scopetest2();


        static int pete();
        static int testgenerate(const char* xsd, const char* output);
        static int emptycs();
        static int compatiblefactory();
        static int transferto(DataObjectPtr d, DataFactoryPtr f, bool expecterror);
        static int openseq();
        static int testanytwo(const char* xsd, 
        const char* xsd2, const char* xml);
        static int testany(const char* xsd,
            const char* acceptedxsderror, 
            const char* xml,
            const char* acceptedxmlerror);
        static int testOrder(const char* x1, const char* x2,
            const char* name1, const char* name2);
        static int simple();
        static int testinc2(const char* inc, const char* name1,
            const char* name2);
        static int testastyle(FILE *f, const char* style);
        static int teststyles();
        static int badelement();
        static int oddchars();
        static int notns();
        static int unsetlist();
        static int travel();
        static int testwsdl();
        static int bunique();
        static int buniqueread();
        static int propdefaulttest();
        static int showdefault(FILE *f, const Type& tm);
        static int graham6();
        static int graham5();
        static int graham4();
        static int leak();
        static int twolists();
         static int detachtest();
        static int includetest();
        static int testLoad();
        static int clonetest();
        static int testUtil();
        static int testErrors();
        static int doctest();
        static int notfound();
        static int serializeChangeSummary(ChangeSummaryPtr cs);
        static int cssave();
        static int csload();
        static int cssave2();
        static int csload2();
        static int bug1();
        static int bug2();
        static int setnull();
        static int testsubsload();
        static int testabstract();
        static int testsubs();
        static int testSCA();
        static int testOpen();
        static int saveOpen();
        static int loadOpen();
        static int testui();
        static int merle1();
        static int graham1();
        static int graham2();
        static int graham3();
        static int loadManyOpen();
        static int loadOpenNS();
        
        
        static int eqhelpertest();
        static int cohelpertest();
        static int datetest();
        static int matttest1();
        static int adddeletetest();
        static int carotest3();
        static int carotest2();
        static int carotest();
        static int setmany();
        static int noncontest();
        static int versiontest();
        static int defaulttest();
        static int defaulttest_strobj();
        static int querytest();
        static int getproptest();
        static int nulltest();
        static int usertest();
        static int rcptest();
        static int boolbug();
        static int scope1();
        static int scope2();
        static int scope3();
        static int conversiontest();
        static int changesummarytest();
        static int scenario1();
        static int scenario2();
        static int scenario3();
        static int scenario4();
        static int scenario5();
        static int seqtest();
        static int printDataObject(DataObjectPtr dol);
        static int printValue(DataObjectPtr dob, const Property& prop);
        static int printList(DataObjectPtr dob, const Property& prop);
        static int printOldValues(ChangeSummaryPtr cs, DataObjectPtr dob);
        static int testGetters(DataObjectPtr dor);
        static int testGetter(DataObjectPtr dor, char* str);
        static int maintest();
        static int jira490();
        static int jira705();
        
        static int b48602();
        static int b48736();
        static int b48686();
        static int b48601();
        static int b48300();
        static int b47663();
        static int b47764();
        static int b47802();
        static int b47293();
        static int b47137();
        static int b47137b();
        static int b46633();
        static int b46734();
        static int b46693();
        static int b46634();
        static int b46613();
        static int b46617();
        static int b46617b();
        static int b45933();
        static int testXPath();
};
