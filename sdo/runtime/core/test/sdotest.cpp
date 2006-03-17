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

/* $Id: sdotest.cpp,v 1.22 2006/03/16 12:53:57 slattery Exp $ */

#include <stdio.h>

#pragma warning(disable:4786)

#include <iostream>
#include <fstream>
using namespace std;



#include "sdotest.h"





using namespace commonj::sdo;



/* USED IN RCPTEST */

typedef struct {
    //zend_object         zo;            /* The standard zend_object */
    DataObjectPtr    dop;            /* The Hydra DataObject */
    //zend_object        *df;            /* The Data Factory */
} sdo_doimpl_object;


int  sdotest::rcptest()
{

    try {
        DataFactoryPtr mdg;
        sdo_doimpl_object* ptr;

        mdg  = DataFactory::getDataFactory();
        mdg = 0;
        mdg  = DataFactory::getDataFactory();
        mdg = NULL;    
        mdg  = DataFactory::getDataFactory();

        mdg->addType("myspace","Root");

        mdg->addType("myspace","Company");

        mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
        mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);

        mdg->addPropertyToType("myspace","Root","companies",
                           "myspace","Company", true, false, true);

        const Type& tcc = mdg->getType("myspace","Root");


        ptr = new sdo_doimpl_object;
    
        ptr->dop = mdg->create((Type&)tcc);

        ptr->dop = 0; // null ;
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << "RefCountingPointer test failed" << endl;
        return 0;
    }
}



int sdotest::changesummarytest()
{
    if (scenario5() == 0) return 0;
    if (scenario1() == 0) return 0;
    if (scenario2() == 0) return 0;
    if (scenario3() == 0) return 0;
    if (scenario4() == 0) return 0;
    return 1;
}


int sdotest::scenario1()
{
    // scenario 1
    // create root type - root has cs
    // create a company type. and containment ref from root, many valued.
    // props on comp id, name both strings single v.
    // create root object
    // begin logging
    // create a company obj. name=acme id=123

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","Root");
    mdg->addPropertyToType("myspace","Root","whatever",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addType("myspace","Company");

    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);

    mdg->addPropertyToType("myspace","Root","companies",
                           "myspace","Company", true, false, true);

    const Type& tcc = mdg->getType("myspace","Root");
    DataObjectPtr dor = mdg->create((Type&)tcc);
    
    ChangeSummaryPtr cs = dor->getChangeSummary();
    cs->beginLogging();

    DataObjectPtr com = dor->createDataObject("companies");
    com->setCString("name","acme");
    com->setCString("id","123");

    FILE *f = fopen("scenario1.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open secnario1.dat" << endl;
        return 0;
    }

    dumpchangesummary(f, cs);
    
    fclose(f);

    cs->endLogging();

    //expect cdo:
    //root - isChanged
    //comp - isCreated
    
    return comparefiles("scenario1.dat","scenario1.txt");
}


int sdotest::testui()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    mdg->addType("companyNS","MainType");


    /* Now add the properties to the types...*/

   
    const Type& tmain   = mdg->getType("companyNS","MainType");
    const Type& tshort  = mdg->getType("commonj.sdo","Short");
    const Type& tint    = mdg->getType("commonj.sdo","Integer");

        
    mdg->addPropertyToType(tmain,"short",tshort,true);
    mdg->addPropertyToType(tmain,"int",tint, true);

    DataObjectPtr main = mdg->create((Type&)tmain);

    DataObjectList& dol1 = main->getList("short");
    DataObjectList& dol2 = main->getList("int");
    DataObjectList& dol3 = main->getList((unsigned int)0);
    DataObjectList& dol4 = main->getList(1);

    return 1;
    }

    catch (SDORuntimeException e)
    {
        if (!silent) cout << "Unsigned integer test failed" << endl << e << endl;
        return 0;
    }
    
}



int sdotest::scenario5()
{
    // scenario 5
    // create root type - root has cs
    // create a company type. and containment ref from root, many valued.
    // props on comp id, name both strings single v.
    // create root object
    // begin logging
    // create a company obj. name=acme id=123

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","Root");
    mdg->addPropertyToType("myspace","Root","whatever",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addType("myspace","Company");
    mdg->addType("myspace","Employee");
    mdg->addPropertyToType("myspace","Employee","name",
                           "commonj.sdo","String", false, false, false);


    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);

    mdg->addPropertyToType("myspace","Company","eotm",
                           "myspace","Employee", false, false, false);

    mdg->addPropertyToType("myspace","Company","employees",
                           "myspace","Employee", true, false, true);

    mdg->addPropertyToType("myspace","Root","companies",
                           "myspace","Company", true, false, true);

    const Type& tcc = mdg->getType("myspace","Root");
    DataObjectPtr dor = mdg->create((Type&)tcc);
    
    ChangeSummaryPtr cs = dor->getChangeSummary();

    DataObjectPtr com = dor->createDataObject("companies");
    com->setCString("name","acme");
    com->setCString("id","123");

    const Type& te = mdg->getType("myspace","Employee");

    DataObjectPtr emp = mdg->create(te);
    emp->setCString("name", "Mr Expendible");

    DataObjectList& emps = com->getList("employees");

    emps.append(emp);

    com->setDataObject("eotm",emp);

    cs->beginLogging();

    com->unset("eotm");

    FILE *f = fopen("scenario5.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open scenario5.dat" << endl;
        return 0;
    }

    dumpchangesummary(f, cs);

    fclose(f);

    cs->endLogging();

    return comparefiles("scenario5.dat","scenario5.txt");

    //expect com to have change record, nothing for eotm:

}

int sdotest::scenario2()
{
    // create root type - root has cs
    // create a company type. and containment ref from root, many valued.
    // 2 props on comp id, name both strings single v.
    // create root object
    // create a company obj. name=acme id=123
    // begin logging()
    // name="megacorp"

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","Root");
    mdg->addPropertyToType("myspace","Root","whatever",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addType("myspace","Company");
    
    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);

    mdg->addPropertyToType("myspace","Root","companies",
                           "myspace","Company", true, false, true);

    const Type& tcc = mdg->getType("myspace","Root");
    DataObjectPtr dor = mdg->create((Type&)tcc);

    DataObjectPtr com = dor->createDataObject("companies");
    com->setCString("name","acme");
    com->setCString("id","123");
    
    ChangeSummaryPtr cs = dor->getChangeSummary();
    cs->beginLogging();

    com->setCString("name","megacorp");

    FILE *f = fopen("scenario2.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open scenario2.dat" << endl;
        return 0;
    }

    dumpchangesummary(f, cs);

    fclose(f);

    cs->endLogging();

    //expect cdo:
    //root- nothing
    // company- isChanged
    //setting prop=name, value="acme"

    return comparefiles("scenario2.dat","scenario2.txt");

}

int sdotest::scenario3()
{

    //create root type - root has cs
    //create a company type. and containment ref from root, many valued.
    //2 props on comp id, name both strings single v.
    //create root object
    //create a company obj. name=acme id=123
    //begin logging()
    //delete company

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","Root");
    mdg->addPropertyToType("myspace","Root","whatever",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addType("myspace","Company");

    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Root","companies",
                           "myspace","Company", true, false, true);

    const Type& tcc = mdg->getType("myspace","Root");
    DataObjectPtr dor = mdg->create((Type&)tcc);

    DataObjectPtr com = dor->createDataObject("companies");
    com->setCString("name","acme");
    com->setCString("id","123");
    
    ChangeSummaryPtr cs = dor->getChangeSummary();
    cs->beginLogging();

    com->detach();

    FILE *f = fopen("scenario3.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open scenario3.dat" << endl;
        return 0;
    }

    dumpchangesummary(f, cs);

    cs->endLogging();

    fclose(f);

    // expect cdo
    // root - changed
    // company - deleted

    return comparefiles("scenario3.dat","scenario3.txt");

}

int sdotest::scenario4()
{
    //create root type - root has cs
    //create a company type. and containment ref from root, many valued.
    //create a dept    type. and containment ref from comp, many valued.
    //create a emp     type. and containment ref from dept, many valued.
    // give name and id properties to comp,dept, emp

    //create root object
    //create a company obj. name=acme id=123
    //create a company obj. name=megacorp id=124
    // give each company 2 depts
    // give each dept 2 employees

    // begin logging()
    // add new employee to first dept of acme
    // delete employee from second dept of acme

    // change name of first employee of first dept megacorp.
    // delete first dept of megacorp.

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","Root");
    mdg->addPropertyToType("myspace","Root","whatever",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addType("myspace","Company");
    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);

    mdg->addType("myspace","Department");
    mdg->addPropertyToType("myspace","Department","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Department","id",
                           "commonj.sdo","String", false, false, false);

    mdg->addType("myspace","Employee");
    mdg->addPropertyToType("myspace","Employee","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Employee","id",
                           "commonj.sdo","String", false, false, false);

    mdg->addPropertyToType("myspace","Root","companies",
                           "myspace","Company", true, false, true);

    mdg->addPropertyToType("myspace","Company","departments",
                           "myspace","Department", true, false, true);

    mdg->addPropertyToType("myspace","Department","employees",
                           "myspace","Employee", true, false, true);

    const Type& tcc = mdg->getType("myspace","Root");
    DataObjectPtr dor = mdg->create((Type&)tcc);

    DataObjectPtr com = dor->createDataObject("companies");
    com->setCString("name","acme");
    com->setCString("id","123");

    DataObjectPtr dep = com->createDataObject("departments");
    dep->setCString("name","widgets");
    dep->setCString("id","1");

    DataObjectPtr emp = dep->createDataObject("employees");
    emp->setCString("name","John O'Watt");
    emp->setCString("id","1");

    emp = dep->createDataObject("employees");
    emp->setCString("name","Ida Know");
    emp->setCString("id","2");
    
    
    dep = com->createDataObject("departments");
    dep->setCString("name","gadgets");
    dep->setCString("id","2");

    emp = dep->createDataObject("employees");
    emp->setCString("name","Ed Memoire");
    emp->setCString("id","1");

    emp = dep->createDataObject("employees");
    emp->setCString("name","Phyllis Tyne");
    emp->setCString("id","2");
    
    
    DataObjectPtr com2 = dor->createDataObject("companies");
    com2->setCString("name","megacorp");
    com2->setCString("id","124");

    dep = com2->createDataObject("departments");
    dep->setCString("name","windows");
    dep->setCString("id","1");

    emp = dep->createDataObject("employees");
    emp->setCString("name","Ivor Payne");
    emp->setCString("id","1");

    emp = dep->createDataObject("employees");
    emp->setCString("name","Colin Poisson");
    emp->setCString("id","2");

    dep = com2->createDataObject("departments");
    dep->setCString("name","portholes");
    dep->setCString("id","2");

    emp = dep->createDataObject("employees");
    emp->setCString("name","Graham Angreeneggs");
    emp->setCString("id","1");

    emp = dep->createDataObject("employees");
    emp->setCString("name","Helen Highwater");
    emp->setCString("id","2");
 
    DataObjectList& deps = com->getList("departments");
    DataObjectList& emps = deps[0]->getList("employees");
    DataObjectList& emps2 = deps[1]->getList("employees");
    DataObjectList& deps2 = com2->getList("departments");
    DataObjectList& emps3 = deps2[0]->getList("employees");
    DataObjectList& emps4 = deps2[1]->getList("employees");

    // should be able to get the change summary property, but it
    // should be zero. 
    // should be able to find out if the type is a change summary type

    if (! dor->getType().isChangeSummaryType())
    {
        if (!silent) cout << "The root object DOESNT HAVE a change summary !!!" << endl;
        return 0;
    }

    try {
        DataObjectPtr csptr = dor->getDataObject("whatever");

        if (!csptr) {
            if (!silent) cout << "CS property was zero - should be invisible!!!!" << endl;
            return 0;
        }
        else {
            if (!silent) cout << "CS property was NOT zero !!" << endl;
            return 0;
        } 
    }
    catch (SDOPropertyNotFoundException e)
    {
        // cout << "No property shows - this is correct" << endl;
    }
    

    if (com->getType().isChangeSummaryType())
    {
        if (!silent) cout << "The company object has a change summary !!!!" << endl;
        return 0;
    }

    ChangeSummaryPtr cs = dor->getChangeSummary();
    cs->beginLogging();

    DataObjectList& depsout = com->getList("departments");

    emp = depsout[0]->createDataObject("employees");
    emp->setCString("name","Irma Newby");
    emp->setCString("id","3");

    DataObjectList& empsout = depsout[1]->getList("employees");
    empsout.remove(0);

    DataObjectList& depsout2 = com2->getList("departments");
    DataObjectList& empsout2 = depsout2[0]->getList("employees");
    empsout2[0]->setCString("name","Ive had my name changed");

    depsout2[0]->detach();

    FILE *f = fopen("scenario4.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open secnario4.dat" << endl;
        return 0;
    }

    dumpchangesummary(f, cs);

    cs->endLogging();

    fclose(f);

    // expect:
    // company acme, department widgets changed.
    // employee 3 of dept widgets of acme (Irma Newby) created.
    // employee 1 of dept gadgets or acme deleted
    // no change record for employee 1 of company megacorp, dept windows
    // deletion for dept windows of megacorp
    // deletion for emp 1 of windows - showing original value for name (Ivor Payne)
    // deletion for emp 2 of windows.

    return comparefiles("scenario4.dat","scenario4.txt");

        
}

int sdotest::seqtest()
{
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    // company is sequenced.
    mdg->addType("myspace","Company",true,false);
    
    mdg->addType("myspace","Department",true,false);
    // string is many

    mdg->addPropertyToType("myspace","Company","string","commonj.sdo","String",
        true);

    mdg->addPropertyToType("myspace","Company","departments","myspace","Department",
        true);

    mdg->addPropertyToType("myspace","Department","name","commonj.sdo","String");


    const Type& tcc = mdg->getType("myspace","Company");
    
    DataObjectPtr dor = mdg->create(tcc);
    const Property& sprop = dor->getType().getProperty("string");
    SequencePtr sptr = dor->getSequence();

    FILE *f = fopen("sequence.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Cannot open sequence.dat" << endl;
        return 0;
    }

    printseq(f, sptr);

    sptr->addCString(sprop,"I am Item 1 of string");
    
    printseq(f, sptr);

    sptr->addText("I am the first free text");
    
    printseq(f, sptr);

    sptr->addCString(sprop,"I am Item 2 of string");
    
    printseq(f, sptr);

    sptr->setCStringValue(1,"I am free text which has been modified");

    DataObjectPtr dep1 = dor->createDataObject("departments");

    printseq(f, sptr);

    dep1->setCString("name","department1");

    printseq(f, sptr);

    DataObjectList& dol = dor->getList("departments");

    const Type& tcd = mdg->getType("myspace","Department");
    DataObjectPtr dep2 = mdg->create(tcd);

    printseq(f, sptr);

    dep2->setCString("name","department2");

    printseq(f, sptr);

    dol.append(dep2);
    
    printseq(f, sptr);

    fclose(f);

    return comparefiles("sequence.dat","sequence.txt");


}


int sdotest::boolbug()
{
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    mdg->addType("myspace","Company");
    mdg->addPropertyToType("myspace","Company","bool","commonj.sdo","Boolean");
    const Type& tcc = mdg->getType("myspace","Company");
    DataObjectPtr dor = mdg->create((Type&)tcc);
    try {
        bool b = dor->getBoolean("bool");
        return 1;
    }
    catch (SDOPropertyNotSetException)
    {
        if (!silent) cout << "WRONG not set exception" << endl;
        return 0;
    }
}

int sdotest::scope1()
{
    try 
    {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        return 1;
    }
    catch(SDORuntimeException e)
    {
        return 0;
    }
}

int sdotest::scope2()
{
    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    mdg->addType("myspace","Company");
    mdg->addPropertyToType("myspace","Company","csumm","commonj.sdo","ChangeSummary");
    const Type& tcc = mdg->getType("myspace","Company");
    DataObjectPtr dor = mdg->create((Type&)tcc);
    return 1;
    }
    catch (SDORuntimeException e)
    {
        return 0;
    }
}

int sdotest::scope3()
{
    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    mdg->addType("myspace","Main");
    mdg->addType("myspace","Subs");
    mdg->addPropertyToType("myspace","Main","name","commonj.sdo","String");
    mdg->addPropertyToType("myspace","Main","subs","myspace","Subs", true);
    mdg->addPropertyToType("myspace","Subs","name","commonj.sdo","String");

    const Type& tcc = mdg->getType("myspace","Main");
    DataObjectPtr dor = mdg->create((Type&)tcc);

    DataObjectPtr sub = dor->createDataObject("subs");
    sub->setCString("name", "sub1");

    sub = dor->createDataObject("subs");
    sub->setCString("name", "sub2");
    return 1;
    }
    catch (SDORuntimeException e)
    {
        return 0;
    }

}

int sdotest::testGetters(DataObjectPtr dor)
{
    if (!testGetter(dor,"boolean")) return 0;
    if (!testGetter(dor,"byte")) return 0;
    if (!testGetter(dor,"character")) return 0;
    if (!testGetter(dor,"short")) return 0;
    if (!testGetter(dor,"integer")) return 0;
    if (!testGetter(dor,"long")) return 0;
    if (!testGetter(dor,"double")) return 0;
    if (!testGetter(dor,"float")) return 0;
    if (!testGetter(dor,"date")) return 0;
    if (!testGetter(dor,"string")) return 0;
    if (!testGetter(dor,"bytes")) return 0;
    if (!testGetter(dor,"dataobject")) return 0;
    return 1;
}

int sdotest::testGetter(DataObjectPtr dor, char* str)
{

    try {
        bool b = dor->getBoolean(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        char by = dor->getByte(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG  unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        wchar_t cy = dor->getCharacter(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG  unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        short s = dor->getShort(str);
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      if (!strcmp(str,"bytes")) return 1;
      return 0;

    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG  unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        int i = dor->getInteger(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        long l = dor->getLong(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        int64_t ll = dor->getLong(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG  unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        float f = dor->getFloat(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        long double d = dor->getDouble(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    unsigned int len;

    try {
        len = dor->getLength(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG  unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    if (len > 0) {
        try  {
            wchar_t * buf = new wchar_t[len];
            unsigned int gotlen = dor->getString(str,buf,len);
        }
        catch (SDOPropertyNotSetException pe)
        {
            if (!silent) cout << "WRONG  unset and undefaulted" << endl; 
            return 0;
        }
        catch (SDOInvalidConversionException ec)
        {
            if (!strcmp(str,"dataobject")) return 1;
             return 0;

        }
        catch (SDORuntimeException e)
        {
            if (!silent) cout << e.getEClassName() << endl; 
            return 0;
        }
        try {
            char * cbuf = new char[len];
            unsigned int gotlen = dor->getBytes(str,cbuf,len);
        }
        catch (SDOPropertyNotSetException pe)
        {
            if (!silent) cout << "WRONG  unset and undefaulted" << endl; 
            return 0;
        }
        catch (SDOInvalidConversionException ec)
        {
            if (!strcmp(str,"dataobject")) return 1;
            return 0;

        }
        catch (SDORuntimeException e)
        {
            if (!silent) cout << e.getEClassName() << endl; 
            return 0;
        }
    }

    try {
        SDODate t = dor->getDate(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << " WRONG unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
       if (!strcmp(str,"boolean")) return 1;
       if (!strcmp(str,"boolean")) return 1;
       if (!strcmp(str,"string")) return 1;
       if (!strcmp(str,"dataobject")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        const char * string = dor->getCString(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG -  unset and undefaulted" << endl; 
        return 0;
    }
    catch (SDOInvalidConversionException ec)
    {
      if (!strcmp(str,"dataobject")) return 1;
      if (!strcmp(str,"date")) return 1;
      return 0;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }

    try {
        DataObjectPtr dob = dor->getDataObject(str);
    }
    catch (SDOPropertyNotSetException pe)
    {
        if (!silent) cout << "WRONG  unset and undefaulted" << endl; 
        return 0;
    }

    catch (SDOInvalidConversionException ec)
    {
        if (!strcmp(str,"dataobject")) return 0;
        return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << endl; 
        return 0;
    }
    return 1;

}

int sdotest::conversiontest()
{
    //cout << "Conversion tests" << endl;

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    mdg->addType("myspace","Container");
    mdg->addType("myspace","Contained");
    mdg->addPropertyToType("myspace","Container","boolean","commonj.sdo","Boolean");
    mdg->addPropertyToType("myspace","Container","byte","commonj.sdo","Byte");
    mdg->addPropertyToType("myspace","Container","character","commonj.sdo","Character");
    mdg->addPropertyToType("myspace","Container","short","commonj.sdo","Short");
    mdg->addPropertyToType("myspace","Container","integer","commonj.sdo","Integer");
    mdg->addPropertyToType("myspace","Container","long","commonj.sdo","Long");
    mdg->addPropertyToType("myspace","Container","float","commonj.sdo","Float");
    mdg->addPropertyToType("myspace","Container","double","commonj.sdo","Double");
    mdg->addPropertyToType("myspace","Container","string","commonj.sdo","String");
    mdg->addPropertyToType("myspace","Container","bytes","commonj.sdo","Bytes");
    mdg->addPropertyToType("myspace","Container","dataobject","myspace","Contained");
    mdg->addPropertyToType("myspace","Container","date","commonj.sdo","Date");
    mdg->addPropertyToType("myspace","Container","bigint","commonj.sdo","BigInteger");
    mdg->addPropertyToType("myspace","Container","bigdec","commonj.sdo","BigDecimal");


    const Type& tcc = mdg->getType("myspace","Container");
    DataObjectPtr dor = mdg->create((Type&)tcc);

    // phase 1 - all unset.......

    // cout << "+++++++++++++++PROPERTY VALUES UNSET ++++++++++++++++++" << endl;

    if (!testGetters(dor)) return 0;

    // cout << "+++++++++++++++PROPERTY VALUES SET ++++++++++++++++++++" << endl;

    DataObjectPtr sub = dor->createDataObject("dataobject");
    dor->setBoolean("boolean", true);
    dor->setByte("byte",20);
    dor->setCharacter("character", 1000);
    dor->setShort("short", (short)12345678);
    dor->setInteger("integer", 87654321);
    dor->setLong("long", 0xFFFFFFFFFFFF);
    dor->setFloat("float", (float)12345.678);
    dor->setDouble("double", 1234567.891);
    dor->setDate("date", 37575);
    wchar_t* chars = new wchar_t[50];
    for (int i=0;i<50;i++) {chars[i] = 0x7F20 + i ;}
    dor->setString("string",chars, 50);

    char* tchars = new char[50];
    for (int ii=0;ii<50;ii++) {tchars[ii] = ii + 32;}
    dor->setBytes("bytes",tchars, 50);


    if (!testGetters(dor)) return 0;

    // cout << "+++++++++++++++END OF TEST ++++++++++++++++++++++++++++" << endl;

    return 1;
}


int sdotest::usertest()
{
    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","Root"); 
    mdg->addType("myspace","UserTest"); 
    mdg->addType("myspace","AnObject");
    
    const Type& tr = mdg->getType("myspace", "Root");
    const Type& tm = mdg->getType("myspace", "UserTest");
    const Type& to = mdg->getType("myspace", "AnObject");

    mdg->addPropertyToType(tm,"boolean",    "commonj.sdo","Boolean");
    mdg->addPropertyToType(tm,"unsetboolean",    "commonj.sdo","Boolean");
    mdg->addPropertyToType(tm,"object",     "myspace","AnObject");
    mdg->addPropertyToType(tm,"unsetobject","myspace","AnObject");

    // many valued...
    mdg->addPropertyToType(tm,"longs",        "commonj.sdo","Integer", true);
    mdg->addPropertyToType(tm,"objects",     "myspace","AnObject", true);

    mdg->addPropertyToType(tr,"usertest",     "myspace","UserTest");

    DataObjectPtr root = mdg->create((Type&)tr);
    DataObjectPtr test = root->createDataObject("usertest");
    DataObjectPtr do1 = test->createDataObject("object");
    DataObjectPtr do2 = test->createDataObject("objects");
    DataObjectPtr do3 = test->createDataObject("objects");

    test->setBoolean("boolean", true);

    FILE *f = fopen ("userdata.dat","w+");

    if (f == 0) {
        if (!silent) cout << "Unable to open userdata.dat" << endl;
        return 0;
    }

    void* value = (void*)0xF1F1F1F1;

    test->setUserData(value);

    fprintf(f,"Expected 0xF1F1F1F1 %p\n",test->getUserData());

    root->setUserData("usertest",value);

    fprintf(f,"Expected 0xF1F1F1F1 %p\n",root->getUserData("usertest"));
    
    root->setUserData((unsigned int)0,(void*)0x20);

    fprintf(f,"Expected 0x20 %p\n" ,root->getUserData((unsigned int)0));

    const Property& prop = root->getType().getProperty("usertest");

    root->setUserData(prop,(void*)0x40020);

    fprintf(f,"Expected 0x40020 %p\n", root->getUserData(prop));
    
    test->setUserData("boolean", (void*)0x120);

    fprintf(f,"Expected 0 %p\n",test->getUserData("boolean"));

    test->setUserData("unsetboolean", (void*)0x340);

    fprintf(f,"Expected 0 %p\n", test->getUserData("boolean"));

    test->setUserData("object", (void*)0x120);

    fprintf(f,"Expected 120 %p\n",test->getUserData("object"));

    test->setUserData("unsetobject", (void*)0x540);

    fprintf(f,"Expected 0 %p\n",test->getUserData("unsetobject"));

    test->setUserData("objects", (void*)0x640);

    // TODO might be dodgy - this allows setting of user data on a base of a list
    fprintf(f,"Expected 640 %p\n",test->getUserData("objects"));

    test->setUserData("objects[1]", (void*)0x740);

    fprintf(f,"Expected 0x740 %p\n",test->getUserData("objects[1]"));

    fclose (f);

    return comparefiles("userdata.dat","userdata.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << "Exception in user test - unexpected" << endl;
        return 0;
    }
}

int sdotest::versiontest()
{
    if (!silent) cout << "The SDO version is :" <<    SdoRuntime::getVersion() << endl;
    if (!silent) cout << "The Major version is: " << SdoRuntime::getMajor() << endl;
    if (!silent) cout << "The Minor version is: " << SdoRuntime::getMinor() << endl;
    if (!silent) cout << "The Fix level     is: " << SdoRuntime::getFix() << endl;
    return 1;
}

int sdotest::noncontest()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","Company"); 
    mdg->addType("myspace","Department"); 
    mdg->addType("myspace","Employee");
    
    const Type& tr = mdg->getType("myspace", "Company");
    const Type& tm = mdg->getType("myspace", "Department");
    const Type& to = mdg->getType("myspace", "Employee");

    // many...
    mdg->addPropertyToType(tr,"departments","myspace", "Department", 
        true);
    
    //mdg->addPropertyToType(to,"name","commonj.sdo", "String");

    mdg->addPropertyToType(tm,"employees","myspace", "Employee", true);

    // single, non containment
    mdg->addPropertyToType(tr,"eom",  "myspace","Employee", false
        , false, false);

    DataObjectPtr comp = mdg->create((Type&)tr);

    {
        DataObjectPtr dept = comp->createDataObject("departments");
        DataObjectPtr emp1 = dept->createDataObject("employees");
        //emp1->setCString("name", "Will");
        DataObjectPtr emp2 = dept->createDataObject("employees");
        //emp1->setCString("name", "Bill");
        DataObjectPtr emp3 = dept->createDataObject("employees");
        //emp1->setCString("name", "Gill");
        comp->setDataObject("eom", emp3);
    }
    return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << "Failed in containment" << e << endl;
        return 0;
    }
}


int sdotest::defaulttest()
{
    try{ 

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","DefaultTest"); 
    mdg->addType("myspace","AnObject");
    
    const Type& tm = mdg->getType("myspace", "DefaultTest");
    const Type& to = mdg->getType("myspace", "AnObject");

    mdg->addPropertyToType(tm,"boolean",    "commonj.sdo","Boolean");

    mdg->setDefault("myspace","DefaultTest","boolean", true);

    mdg->addPropertyToType(tm,"byte",        "commonj.sdo","Byte");
    
    mdg->setDefault("myspace","DefaultTest","byte", (char)'d');

    mdg->addPropertyToType(tm,"character",  "commonj.sdo","Character");

    mdg->setDefault("myspace","DefaultTest","character", (wchar_t)'e');

    mdg->addPropertyToType(tm,"short",        "commonj.sdo","Short");

    mdg->setDefault("myspace","DefaultTest","short", (short)300);

    mdg->addPropertyToType(tm,"long",        "commonj.sdo","Integer");

    mdg->setDefault("myspace","DefaultTest","long", (long)400);

    mdg->addPropertyToType(tm,"longs",        "commonj.sdo","Integer", true);

    mdg->setDefault("myspace","DefaultTest","longs", (long)800);

    mdg->addPropertyToType(tm,"longlong",   "commonj.sdo","Long");

    mdg->setDefault("myspace","DefaultTest","longlong", (int64_t)500);

    mdg->addPropertyToType(tm,"float",        "commonj.sdo","Float");

    mdg->setDefault("myspace","DefaultTest","float", (float)600.0);

    mdg->addPropertyToType(tm,"longdouble", "commonj.sdo","Double");

    mdg->setDefault("myspace","DefaultTest","longdouble", (long double)700.0);

    mdg->addPropertyToType(tm,"date",       "commonj.sdo","Date");

    mdg->setDefault("myspace","DefaultTest","date", (long)900);

    mdg->addPropertyToType(tm,"string",       "commonj.sdo","String");

    wchar_t* help = new wchar_t[4];
    help[0] = 'H';
    help[1] = 'E';
    help[2] = 'L';
    help[3] = 'P';

    mdg->setDefault("myspace","DefaultTest","string", help, 4);

    delete help;

    char* help2 = new char[4];
    help2[0] = 'H';
    help2[1] = 'E';
    help2[2] = 'L';
    help2[3] = 'P';

    mdg->addPropertyToType(tm,"bytes",       "commonj.sdo","Bytes");

    mdg->setDefault("myspace","DefaultTest","bytes", help2, 4);

    delete help2;

    mdg->addPropertyToType(tm,"object",       "myspace","AnObject");

    DataObjectPtr test = mdg->create((Type&)tm);

    FILE *f = fopen("defaults.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open file defaults.dat" << endl;
        return 0;
    }


    fprintf(f, "Boolean default is true: %d\n",test->getBoolean("boolean"));

    fprintf(f, "Byte default is d: %d\n",test->getByte("byte"));
    
    fprintf(f, "Character default is e: %d\n",test->getCharacter("character"));

    fprintf(f, "Short default is 300: %d\n",test->getShort("short"));

    fprintf(f, "Long default is 400: %d\n",test->getInteger("long"));

    try {
    fprintf(f, "Longs default is 800: %d\n" ,test->getInteger("longs[1]"));
    }
    catch (SDOIndexOutOfRangeException ex)
    {
        fprintf(f,"Expected index out of range OK\n");
    }

    fprintf(f,"Float default is 600: %.3f\n",test->getFloat("float"));

    fprintf(f, "LongDouble default is 700: %.3f\n",test->getDouble("longdouble"));

    fprintf(f, "String default is HELP: ");
    unsigned int lenw = test->getLength("string");
    if (lenw > 0) {
        char* tw = new char[lenw];
        test->getBytes("string",tw,lenw);
        for (int i=0;i<lenw;i++)
        {
            fprintf(f,"%c",tw[i]);
        }
        fprintf(f,"\n");
    }

    fprintf(f,"Bytes default is HELP: ");
    unsigned int len = test->getLength("bytes");
    if (len > 0) {
        char* tc = new char[len];
        test->getBytes("bytes",tc,len);
        for (int i=0;i<len;i++)
        {
            fprintf(f,"%c", tc[i]);
        }
        fprintf(f,"\n");
    }
    fclose (f);
    return comparefiles("defaults.dat","defaults.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << e.getEClassName() << " in "; 
        if (!silent)cout << e.getFileName() << " at line ";
        if (!silent)cout << e.getLineNumber() << endl;
        if (!silent)cout << e.getFunctionName() << " ";
        if (!silent)cout << e.getMessageText() << endl;
        return 0;
    }
}


int sdotest::showdefault(FILE *f, const Type& tm)
{
    const Property& pboolean = tm.getProperty("boolean");
    const Property& pbyte = tm.getProperty("byte");
    const Property& pcharacter = tm.getProperty("character");
    const Property& pshort = tm.getProperty("short");
    const Property& plong = tm.getProperty("long");
    const Property& plongs = tm.getProperty("longs");
    const Property& plonglong = tm.getProperty("longlong");
    const Property& pfloat = tm.getProperty("float");
    const Property& plongdouble = tm.getProperty("longdouble");
    const Property& pdate = tm.getProperty("date");
    const Property& pstring = tm.getProperty("string");
    const Property& pbytes = tm.getProperty("bytes");
    const Property& pobject = tm.getProperty("object");
   
    // first see what we get for the default when there is none

    bool bb = pboolean.getBooleanDefault();
    const char* bs = pboolean.getCStringDefault();

    fprintf(f, "Boolean default is : %d\n",bb);
    fprintf(f, "Boolean default as a string is %s\n",bs);

    char cc = pbyte.getByteDefault();
    const char* cs = pbyte.getCStringDefault();
    fprintf(f, "Byte default is : %d\n",cc);
    fprintf(f, "Byte default as a string is %s\n",cs);

    wchar_t wc = pcharacter.getCharacterDefault();
    const char* ws = pcharacter.getCStringDefault();
    fprintf(f, "Character default is : %d\n",wc);
    fprintf(f, "Character default as a string is %s\n",ws);

    short ss = pshort.getShortDefault();
    fprintf(f, "Short default is : %d\n",ss);

    long ll = plong.getIntegerDefault();
    fprintf(f, "Integer default is : %d\n",ll);
    long ll2 = plongs.getLongDefault();
    fprintf(f, "Integer many default is : %d\n",ll2);

    int64_t llll = plonglong.getLongDefault();
    fprintf(f, "Long default is : %ld\n",(long)llll);

    float ff = pfloat.getFloatDefault();
    fprintf(f, "Float default is : %.3f\n",ff);
    
    long double  dd = plongdouble.getDoubleDefault();
    fprintf(f, "Double default is : %.3f\n",dd);

    const SDODate& sd = pdate.getDateDefault();
    fprintf(f, "Date default is : %d\n",sd.getTime());

    unsigned int l = pstring.getDefaultLength();
    if (l > 0)
    {
        wchar_t * buf = new wchar_t[l+1];
        l = pstring.getStringDefault(buf,l);
        fprintf(f, "String default length is %d\n", l);
        for (int i=0;i<l;i++)
        {
            fprintf(f, "%c",buf[i]);
        }
        fprintf(f, "\n");
        delete buf;
        const char* wws = pstring.getCStringDefault();
        fprintf(f, "String default as a string is %s\n", wws);

    }
    else
    {
        fprintf(f, "String default is zero length\n"); 
    }

    l = pbytes.getDefaultLength();
    if (l > 0)
    {
        char * buf = new char[l+1];
        l = pbytes.getBytesDefault(buf,l);
        fprintf(f, "Bytes default length is %d\n",l);
        for (int i=0;i<l;i++)
        {
            fprintf(f, "%c", buf[i]);
        }
        fprintf(f, "\n");
        delete buf;
    }
    else
    {
        fprintf(f, "Bytes default is zero length\n"); 
    }
    return 1;
}


int sdotest::propdefaulttest()
{
    try{ 

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","DefaultTest"); 
    mdg->addType("myspace","AnObject");
    
    const Type& tm = mdg->getType("myspace", "DefaultTest");
    const Type& to = mdg->getType("myspace", "AnObject");

    mdg->addPropertyToType(tm,"boolean",    "commonj.sdo","Boolean");
    mdg->addPropertyToType(tm,"byte",        "commonj.sdo","Byte");
    mdg->addPropertyToType(tm,"character",  "commonj.sdo","Character");
    mdg->addPropertyToType(tm,"short",        "commonj.sdo","Short");
    mdg->addPropertyToType(tm,"long",        "commonj.sdo","Integer");
    mdg->addPropertyToType(tm,"longs",        "commonj.sdo","Integer", true);
    mdg->addPropertyToType(tm,"longlong",   "commonj.sdo","Long");
    mdg->addPropertyToType(tm,"float",        "commonj.sdo","Float");
    mdg->addPropertyToType(tm,"longdouble", "commonj.sdo","Double");
    mdg->addPropertyToType(tm,"date",       "commonj.sdo","Date");
    mdg->addPropertyToType(tm,"string",     "commonj.sdo","String");
    mdg->addPropertyToType(tm,"bytes",      "commonj.sdo","Bytes");
    mdg->addPropertyToType(tm,"object"  ,    "myspace","AnObject");

    //cout << "Testing unset defaults....." << endl;

    FILE *f;

    f = fopen("showdefault1.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open showdefault1.dat" << endl;
        return 0;
    }
    showdefault(f, tm);

    fclose (f);
    if (!comparefiles("showdefault1.dat","showdefault1.txt"))
    {
        return 0;
    }

    //now set all the defaults....    

    mdg->setDefault("myspace","DefaultTest","boolean", true);
    mdg->setDefault("myspace","DefaultTest","byte", (char)'d');
    mdg->setDefault("myspace","DefaultTest","character", (wchar_t)'e');
    mdg->setDefault("myspace","DefaultTest","short", (short)300);
    mdg->setDefault("myspace","DefaultTest","long", (long)400);
    mdg->setDefault("myspace","DefaultTest","longs", (long)800);
    mdg->setDefault("myspace","DefaultTest","longlong", (int64_t)500);
    mdg->setDefault("myspace","DefaultTest","float", (float)600.0);
    mdg->setDefault("myspace","DefaultTest","longdouble", (long double)700.0);
    mdg->setDefault("myspace","DefaultTest","date", (long)900);


    wchar_t* help = new wchar_t[4];
    help[0] = 'H';
    help[1] = 'E';
    help[2] = 'L';
    help[3] = 'P';

    mdg->setDefault("myspace","DefaultTest","string", help, 4);

    delete help;

    char* help2 = new char[4];
    help2[0] = 'H';
    help2[1] = 'E';
    help2[2] = 'L';
    help2[3] = 'P';


    mdg->setDefault("myspace","DefaultTest","bytes", help2, 4);

    delete help2;

    // and have another go at getting them....

    f = fopen("showdefault2.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open showdefault2.dat" << endl;
        return 0;
    }
    showdefault(f, tm);

    fclose (f);
    if (!comparefiles("showdefault2.dat","showdefault2.txt"))
    {
        return 0;
    }

    return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << e.getEClassName() << " in "; 
        if (!silent)cout << e.getFileName() << " at line ";
        if (!silent)cout << e.getLineNumber() << endl;
        if (!silent)cout << e.getFunctionName() << " ";
        if (!silent)cout << e.getMessageText() << endl;
        return 0;
    }

}

int sdotest::nulltest()
{

    FILE *f = 0;
     
    try {

    int i;
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","NullTest",true, false); // sequenced
    mdg->addType("myspace","AnObject");
    
    const Type& tm = mdg->getType("myspace", "NullTest");
    const Type& to = mdg->getType("myspace", "AnObject");

    mdg->addPropertyToType(tm,"boolean",    "commonj.sdo","Boolean");
    mdg->addPropertyToType(tm,"byte",        "commonj.sdo","Byte");
    mdg->addPropertyToType(tm,"character",  "commonj.sdo","Character");
    mdg->addPropertyToType(tm,"short",        "commonj.sdo","Short");
    mdg->addPropertyToType(tm,"long",        "commonj.sdo","Integer");
    mdg->addPropertyToType(tm,"longlong",   "commonj.sdo","Long");
    mdg->addPropertyToType(tm,"float",        "commonj.sdo","Float");
    mdg->addPropertyToType(tm,"longdouble", "commonj.sdo","Double");
    mdg->addPropertyToType(tm,"date",       "commonj.sdo","Date");
    mdg->addPropertyToType(tm,"string",       "commonj.sdo","String");
    mdg->addPropertyToType(tm,"bytes",       "commonj.sdo","Bytes");
    mdg->addPropertyToType(tm,"object",       "myspace","AnObject");


    mdg->addPropertyToType(tm,"cs",       "commonj.sdo","ChangeSummary");

    DataObjectPtr test = mdg->create((Type&)tm);
    DataObjectPtr ob = mdg->create((Type&)to);
    DataObjectPtr ob2 = mdg->create((Type&)to);
    DataObjectPtr ob3 = mdg->create((Type&)to);


    f = fopen("nulltest.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open file nulltest.dat" << endl;
        return 0;
    }

    // first check all props are unset

    ChangeSummaryPtr cs = test->getChangeSummary();

    cs->beginLogging();

    PropertyList pl = test->getInstanceProperties();

    fprintf(f, "Initially - all properties unset, and default values...\n");
    for (i=0;i<pl.size(); i++)
    {
        if (pl[i].isMany())continue;
        try {
        if (pl[i].getType().isDataType()) {
            fprintf(f, "%s set:%d value ",pl[i].getName(),test->isSet(pl[i]));
            const char *xx = test->getCString(pl[i]);
            if (xx != 0)
            {
                fprintf(f, "%s\n",xx);
            }
            else
            {
                fprintf(f, " is empty \n");
            }
        }
        else {
            fprintf(f,"%s set: %d value:", pl[i].getName(),test->isSet(pl[i]));
            DataObjectPtr xy = test->getDataObject(pl[i]);
            if (xy != 0)
            {
                fprintf(f, "%s\n",xy);
            }
            else
            {
                fprintf(f, " is empty \n");
            }
        }
        }
        catch (SDOPropertyNotSetException)
        {
            if (!silent) cout <<  " WRONG - got not set exception" << endl;
            return 0;
        }
    }
    test->setBoolean("boolean", false);
    test->setByte("byte",0);
    test->setCharacter("character",0);
    test->setShort("short",0);
    test->setInteger("long",0);
    test->setLong("longlong",0 );
    test->setFloat("float",0);
    test->setDouble("longdouble",0 );
    test->setDate("date",0  );
    test->setString("string",0,0); 
    test->setBytes("bytes","",0) ;
    test->setDataObject("object",0) ;
    


    fprintf(f, "Should now have all properties set, and zero values...\n");
    for (i=0;i<pl.size(); i++)
    {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            fprintf(f, "%s set:%d value:%s\n", pl[i].getName(),test->isSet(pl[i]),test->getCString(pl[i]));
        }
        else {
            fprintf(f,"%s set:%d dataobject\n", pl[i].getName(),test->isSet(pl[i]));
        }

    }

    for (i=0;i<pl.size(); i++)
    {
        test->unset(pl[i]);
    }
    fprintf(f, "Should be back to having properties unset, and default values...\n");
    for (i=0;i<pl.size(); i++)
    {

        try {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            fprintf(f, "%s set:%d value:",pl[i].getName(),test->isSet(pl[i]));
            const char *xx = test->getCString(pl[i]);
            if (xx != 0)
            {
                fprintf(f, "%s\n", xx);
            }
            else
            {
                fprintf(f, " is empty \n");
            }
        }
        else {
            fprintf(f, "%s set: %d dataobject", pl[i].getName(),test->isSet(pl[i]));
            DataObjectPtr dp = test->getDataObject(pl[i]);
            if (dp != 0)
            {
                fprintf(f, "\n");
            }
            else
            {
                fprintf(f, " is empty \n");
            }
        }
        }
        catch (SDOPropertyNotSetException)
        {
            if (!silent) cout << "WRONG  - not set exception" << endl;
            return 0;
        }
    }

    SequencePtr s = test->getSequence();

    s->addBoolean(test->getType().getProperty("boolean")/*"boolean"*/, false);
    s->addByte(1/*"byte"*/,0);
    s->addCharacter(2/*"character"*/,0);
    s->addShort(3/*"short"*/,0);
    s->addInteger(4/*"long"*/,0);
    s->addLong(5/*"longlong"*/,0 );
    s->addFloat(6/*"float"*/,0);
    s->addDouble(7/*"longdouble"*/,0 );
    // bug ...s->setDate(8/*"date"*/,0  );
    s->addString(9/*"string"*/,0,0); 
    s->addBytes(10/*"bytes"*/,"",0) ;
    s->addDataObject(11/*"object"*/,0) ;

    for (i=0;i<pl.size(); i++)
    {
        try {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            fprintf(f, "%s set:%d value:", pl[i].getName(),test->isSet(pl[i]));
            const char*  xx = test->getCString(pl[i]);
            if ( xx != 0)
            {
                fprintf(f, "%s\n", xx);    
            }
            else
            {
                fprintf(f, " is empty \n");
            }
        }
        else {
            fprintf(f, "%s set:%d dataobject", pl[i].getName(),test->isSet(pl[i]));
            DataObjectPtr db = test->getDataObject(pl[i]);
            if (db != 0)
            {
                fprintf(f, "\n");
            }
            else
            {
                fprintf(f, " is empty \n");
            }
        }
        }
        catch (SDOPropertyNotSetException)
        {
            if (!silent) cout <<  "WRONG - not set exception " << endl;
            return 0;
        }
    }
    ChangedDataObjectList& cl = cs->getChangedDataObjects();
    for ( i =0; i< cl.size() ; i++) 
    {
        if (cs->isCreated(cl[i])) {
            fprintf(f, "Created dataobject\n");
        }
        if (cs->isDeleted(cl[i])) {
            fprintf(f, "Deleted dataobject\n");
        }
        if (cs->isModified(cl[i])) {
            fprintf(f, "Modified dataobject\n");
        }
    }

    // now unset all the properties...
    for (i=0;i<pl.size(); i++)
    {
        test->unset(pl[i]);
    }

    for (i=0;i<pl.size(); i++)
    {
        if (!pl[i].isMany() )
        {
            test->setNull(i);
        }
    }

    fprintf(f, "Should all be null, and default values...\n");
    for (i=0;i<pl.size(); i++)
    {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            // check for a null first!!
            fprintf(f, "%s isNull:%d set:%d value:",pl[i].getName(),
                                         test->isNull(i),test->isSet(pl[i]));
            if (test->getCString(pl[i])  == 0) fprintf(f, "\n");
            else fprintf(f, "%s\n", test->getCString(pl[i]));
        }
        else {
            fprintf(f, "%s isNull:%d set:%d value",pl[i].getName(),
                   test->isNull(i),test->isSet(pl[i]));
            if (test->getDataObject(pl[i]) == 0) fprintf(f, " empty\n");
            else fprintf(f, " dataobject\n");
        }
    }

    s->addBoolean(test->getType().getProperty("boolean")/*"boolean"*/, true);
    s->addByte(1/*"byte"*/,1);
    s->addCharacter(2/*"character"*/,1);
    s->addShort(3/*"short"*/,1);
    s->addInteger(4/*"long"*/,1);
    s->addLong(5/*"longlong"*/,1 );
    s->addFloat(6/*"float"*/,1.0);
    s->addDouble(7/*"longdouble"*/,1.0 );
    // bug ...s->setDate(8/*"date"*/,0  );
    wchar_t* chs = new wchar_t[5];
    chs[0] = 'h';
    chs[1] = 'e';
    chs[2] = 'l';
    chs[3] = 'l';
    chs[4] = 'o';
    s->addString(9/*"string"*/,chs,5);
    delete chs;
    s->addBytes(10/*"bytes"*/,"hello",5) ;
    s->addDataObject(11/*"object"*/,ob) ;

    fprintf(f, "Should all have values, and not be null..\n");
    for (i=0;i<pl.size(); i++)
    {
        
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            fprintf(f, "%s isNull:%d set:%d value:",pl[i].getName(),
                test->isNull(i),test->isSet(pl[i]));
            if (test->getCString(pl[i]) == 0) fprintf(f, "\n");
            else fprintf(f, "%s\n", test->getCString(pl[i]));

        }
        else {
            fprintf(f, "%s isNull%d set:%d value:", pl[i].getName(),
                test->isNull(i),test->isSet(pl[i]));
            if (test->getDataObject(pl[i]) == 0) fprintf(f, " empty\n");
            else fprintf(f,  " dataobject\n");
        }
    }

    for (i=0;i<pl.size(); i++)
    {
        if (!pl[i].isMany() )
        {
            test->setNull(pl[i]);
        }
    }

    fprintf(f, "Should all be null, and default values...\n");
    for (i=0;i<pl.size(); i++)
    {
        
        try {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            fprintf(f, "%s isNull:%d set:%d value:",pl[i].getName(),
                test->isNull(i),test->isSet(pl[i]));
            if (test->getCString(pl[i]) == 0) fprintf(f, "\n");
            else fprintf(f, "%s\n",test->getCString(pl[i]));

        }
        else {
            fprintf(f, "%s isNull;%d set:%d value:", pl[i].getName(),
                test->isNull(i),test->isSet(pl[i]));
            if (test->getDataObject(pl[i]) == 0) fprintf(f," empty\n");
            else fprintf(f, " dataobject\n");
        }
        }
        catch (SDOPropertyNotSetException)
        {
            if (!silent) cout << " WRONG - not set exception" << endl;
            return 0;
        }
    }

    cs->endLogging();
    fclose(f);

    return comparefiles("nulltest.dat","nulltest.txt");
    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << "Nulltest failed " << e << endl;
        if (f) fclose(f);
        return 0;
    }
}

int sdotest::maintest()
{

    int i;

    FILE *f;

    try{

        f = fopen("maintest.dat","w+");
        if (f == 0)
        {   
            if (!silent) cout << "Failed to open maintest.dat" << endl;
        }

        fprintf(f,"Test Program starting to create types ...\n");

        /* First create a DataFactory , then add some types and props...*/

        /* This is dms creation of metadata */



        DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
        mdg->addType("myspace","Company");
        mdg->addType("myspace","Department");
        // manager is a sequenced type...
        mdg->addType("myspace","Manager", true, false);

        mdg->addType("myspace","Employee");

        // tests of base types 

        // note - base type of a seq type must be seq...
        mdg->addType("myspace","Person", true, false);

        mdg->addType("myspace","DerivedString");
        mdg->addType("myspace","SubDepartment");

        /* Now add the properties to the types...*/

        const Type& tc = mdg->getType("myspace","Company");
        const Type& ts = mdg->getType("commonj.sdo","String");
        const Type& ti = mdg->getType("commonj.sdo","Integer");
        const Type& tf = mdg->getType("commonj.sdo","Float");
        const Type& tm = mdg->getType("myspace","Manager");

        fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());

        const Type& td = mdg->getType("myspace","Department");
        const Type& te = mdg->getType("myspace","Employee");
        const Type& tp = mdg->getType("myspace","Person");
        const Type& tds= mdg->getType("myspace","DerivedString");
        const Type& tsd= mdg->getType("myspace","SubDepartment");

        fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());





        /* By type, with a type */
        mdg->addPropertyToType(tc,"name",ts);

        /* by name/uri with a type */
        mdg->addPropertyToType("myspace","Company","address",ts);

        /* by type with name/uri */
        mdg->addPropertyToType(tc,"departments", "myspace","Department",
        true);


        // add a reference to employee of the month
        mdg->addPropertyToType(tc,"employee of the month", "myspace","Employee",
        false, false, false);



        /* A list of floats */

        mdg->addPropertyToType(tc,"floatlist", tf,true);

        mdg->addPropertyToType(tc,"pdg", tm);

        fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());

        mdg->addPropertyToType(td,"name", ts);
        
        mdg->addPropertyToType(td,"id", ti);

        mdg->addPropertyToType(td,"manager", tm);
    
        mdg->addPropertyToType(tm,"name", ts);


        mdg->addPropertyToType(te,"name",ts);
        mdg->addPropertyToType(td,"employees",te,true,false,true);



        // emps and managers are both people (in theory).
        mdg->setBaseType(te,tp);
        fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());
        mdg->setBaseType(tm,tp);
        fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());
        mdg->addPropertyToType(tp,"haircolour", ts);
        mdg->addPropertyToType(tp,"name", ts);
        mdg->addPropertyToType(tm,"officeid", ts);
        mdg->addPropertyToType(te,"cubelocation", ts);

        mdg->addPropertyToType(tc,"shareholder", tp);

        //derived string - derived from string - cannot have properties!!!
        mdg->setBaseType(tds,ts);

        // subdept derived from dept
        mdg->setBaseType(tsd,td);
        mdg->addPropertyToType(tsd,"subname",ts);

        mdg->addPropertyToType(tc, "substring",tds);

        /* by type with name/uri */
        mdg->addPropertyToType(tc,"subdepartments", "myspace","SubDepartment",
        true);

        /* This is the data structure */    

        

    /* company
            ----shareholder (Person)
            ----substring (DerivedString)
     *      ----name (String)
     *        ----pdg  (Manager)
                    (inherits haircolour from person,
                     has name as duplicate property - should use person.name
                     has officeid)
     *            ----name(String)
                ----haircolour(String)
                ----officeid(String)
     *
     *        ----depts[] (Department)
     *          ----name (String)
     *          ----id   (Integer)
     *          ----manager (Manager).
     *                ----name(String)
                    ----haircolour(String) from Person
     *          ----employees[] (Employee)
     *              ----name(String)
                    ----haircolour(String) - from Person
                    ----cubelocation(String)
            --subdepartments[] (SubDepartment)
                    ----name (String) from Department
                    ----id (String) from Department
                    ----manager (Manager) from Department
                    ----employees (from department)
     */
  
        /* Now add a primitive type test to the manager */
        mdg->addPropertyToType(tm,"string",ts);

         fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());

        mdg->addPropertyToType(tm,"boolean",    "commonj.sdo","Boolean");
        mdg->addPropertyToType(tm,"byte",        "commonj.sdo","Byte");
        mdg->addPropertyToType(tm,"character",  "commonj.sdo","Character");
        mdg->addPropertyToType(tm,"short",        "commonj.sdo","Short");
        mdg->addPropertyToType(tm,"long",        "commonj.sdo","Integer");
        mdg->addPropertyToType(tm,"longlong",   "commonj.sdo","Long");
        mdg->addPropertyToType(tm,"float",        "commonj.sdo","Float");
        mdg->addPropertyToType(tm,"longdouble", "commonj.sdo","Double");
        mdg->addPropertyToType(tm,"date",       "commonj.sdo","Date");
        // dead mdg->addPropertyToType(tm,"charptr",    "commonj.sdo","Bytes");
        mdg->addPropertyToType(tm,"string",       "commonj.sdo","String");
        mdg->addPropertyToType(tm,"bytes",       "commonj.sdo","Bytes");


        fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());

        // create a few aliases

        mdg->setAlias("myspace","Company","TheFirm");
        mdg->setAlias("myspace","Company","departments","TheDepartments");
        mdg->setAlias("myspace","Company","TheBigFirm");
        mdg->setAlias("myspace","Company","TheLittleFirm");

        const Type& tlf = mdg->getType("myspace","TheBigFirm");

        fprintf(f, "Should be Company :%s\n",  tlf.getName());

        fprintf(f, "Should be 3: %d\n", tlf.getAliasCount());

        for (int ai = 0; ai < tlf.getAliasCount(); ai++)
        {
            fprintf(f, "Alias: %s\n",tlf.getAlias(ai));
        }


        // Change summary test begins
        mdg->addPropertyToType(tc,"csumm","commonj.sdo","ChangeSummary");
        // should log an error - but not fail
        mdg->addPropertyToType(td,"csumm","commonj.sdo","ChangeSummary");

        fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());
 
        fprintf(f, "BEFORE RESOLUTION\n");
        printDataStructure(f, mdg);

          /* Now create some objects in the dg */
    
//        DataObjectPtr dor;

        const Type& tcc = mdg->getType("myspace","Company");
        DataObjectPtr dor = mdg->create((Type&)tcc);

        fprintf(f, "AFTER RESOLUTION\n");
        printDataStructure(f, mdg);

        fprintf(f, "Manager is sequenced?%d\n",tm.isSequencedType());

        dor->setCString("substring","This is the sub string - its primitive, but not a string");
        
        const char* subby = dor->getCString("substring");

        fprintf(f, "%s\n", subby);

        dor->setCString("name","acmecorp");
        const char* chnam = dor->getCString("name");
        fprintf(f, "%s\n", chnam);

        dor->unset("name");

        dor->setCString("name","acmecorp");

        // Set up the two departments before logging starts

        DataObjectPtr dep1 = dor->createDataObject("departments");
        dep1->setCString("name","Developement");

        DataObjectPtr dep2= dor->createDataObject("departments");
        dep2->setCString("name","Marketing");

        // add a couple of floats to the floatlist
        DataObjectList& dolist = dor->getList("floatlist");
        dolist.append((float)1.4);
        dolist.append((float)1.5);
        
        //TODO - change summaries should live on the DataObject
        // interface - can this be done?

        ChangeSummaryPtr cs = dor->getChangeSummary();
        cs->beginLogging();

        const char* sname = dor->getCString("name");

        fprintf(f, "%s\n", sname);

        // This should put a created entry in the cs.

        // This should be equivalent to the alternative below.........
        const Type& tcd = mdg->getType("myspace","Manager");
        DataObjectPtr pdg = mdg->create((Type&)tcd);

        // This set should put the entry into the change summary.
        // both in the creations and in the changes.
         dor->setDataObject("pdg",pdg);


        // try getting the boolean as a string - should be defaulted to false:
        const char *bol;
        try{
            bol = pdg->getCString("boolean");
            if (bol != 0)
            {
                fprintf(f, "Expected default boolean (false) : %s\n",bol);
            }
            else
            {
                fprintf(f, " is empty \n");
            }
        }
        catch (SDOPropertyNotSetException)
        {
            if (!silent)cout << "WRONG handled notset exception " << endl;
            fclose(f);
            return 0;
        }

        pdg->setBoolean("boolean", true);
        bol = pdg->getCString("boolean");
        fprintf(f, "Expected  boolean (true) : %s\n",bol);

        // and the widechars?
        const char* wdc;
        try {
            wdc = pdg->getCString("string");
            if (wdc != 0)
            {
                fprintf(f, "Expected default string (0) : ",wdc);
            }
            else
            {
                fprintf(f, " is empty \n");
            }
        }
        catch (SDOPropertyNotSetException)
        {
            if (!silent) cout << "WRONG handled notset exception" <<endl;
            fclose (f);
            return 0;
        }


        // this should get a valid sequence..

        SequencePtr sq =   pdg->getSequence();


        // The alternative.............................................
        // Here the create should put the entry into the change summary
        //DataObject* pdg = dor->createDataObject("pdg");
    
        // This should do nothing to the cs, as its in a created object
        pdg->setCString("name","Jacques LePlace");
        
        // This should modify the property, and add an item to the sequence.
        sq->addCString("name", "Jacques LePlagne");


        // The creation entry should be removed from the change summary
        // The entry should no longer exist and the pointer is invalid
        pdg->detach();

        pdg = dor->getDataObject("pdg");


        fprintf(f, " A deleted data object should be zero: %d\n", pdg);

        pdg = mdg->create((Type&)tcd);

        // A new creation in the change summary
        dor->setDataObject("pdg",pdg);

        // No modification as the object is created
        pdg->setCString("name", "Mr Horace Walker");

        sq = pdg->getSequence();

        try {
        // element 0 is the first setting - which we just deleted!
            sq->setCStringValue(0,"Unable to set");
        }
        catch (SDOIndexOutOfRangeException)
        {
            // thats OK
            sq->addCString("name","Now able to set");
        }


        sq->addText(" - should say now able to set\n");

        for (int ii=0;ii<sq->size();ii++)
        {
            fprintf(f, "%s\n", sq->getCStringValue(ii));
        }

        try {
            const char* n = pdg->getCString("name");
            fprintf(f, " Name from deleted item: %s\n",n);
        }
        catch (SDOPropertyNotFoundException e)
        {
        // thats ok
            fprintf(f, "Correctly caught prop not found\n");
        }


        // This should put a change in the cs.
        dor->setCString("name","mega-new-acmecorp");


        // At this point, we should know that pdg was empty and is now 
        // a created object. And that the old string for company name
        // was "acmecorp"

    //    cs->DebugPrint();


    /* Now set up the primitive type test on PDG */

        pdg->setCString("string","Hello I am a string");
        pdg->setBoolean("boolean",true);
        pdg->setByte("byte",23);
        pdg->setCharacter("character",45);
        pdg->setShort("short",34);
        pdg->setLong("long", 56);
        pdg->setDouble("longdouble",89.0);
        pdg->setFloat("float",90.0);
        pdg->setDate("date",(time_t)200);
        

          pdg->setLong("longlong",0xFFFFFFFFFFFFFFFF);

        // try reading the longlong as a string 
        const char *lls = pdg->getCString("longlong");
        fprintf(f, "0xffffffffffffffff = : %s\n", lls);

        pdg->setLong("longlong",0x7FFFFFFFFFFFFFFF);

        lls = pdg->getCString("longlong");
        fprintf(f, "0x7fffffffffffffff =  %s\n",lls);

        pdg->setLong("longlong",0x7FFFFFFF);
        lls = pdg->getCString("longlong");
        fprintf(f, "0x7fffffff =  %s\n",lls);

        pdg->setLong("longlong",0x80000000);
        lls = pdg->getCString("longlong");
        fprintf(f, "0x80000000 =  %s\n",lls);

        pdg->setLong("longlong",78);


        wchar_t* wide = new wchar_t[4];
        wide[0] = 'W';
        wide[1] = 'i';
        wide[2] = 'd';
        wide[3] = 'e';

        pdg->setString("string",wide,4);

        // and as a string?
        wdc = pdg->getCString("string");
        fprintf(f, " Expected Wide - got : %s\n",wdc);

        delete wide;

        char* thin = new char[4];
        thin[0] = 'T';
        thin[1] = 'h';
        thin[2] = 'i';
        thin[3] = 'n';

        pdg->setBytes("bytes",thin,4);

        const char* dc = pdg->getCString("bytes");
        fprintf(f, " Expected Thin - got : %s\n",dc);

        delete thin;

        const char* ps = pdg->getCString("string");
        fprintf(f, "Expecting string, got %s\n", ps);
    
        bool pb = pdg->getBoolean("boolean");
        fprintf(f, "Expected bool true, got %d\n",pb);

        char pc = pdg->getByte("byte");
        fprintf(f, "Expected char 23, got %d\n",pc);

        wchar_t pw = pdg->getCharacter("character");
        fprintf(f, "expected wchar 45, got %d",pw);

        short pss = pdg->getShort("short");
        fprintf(f, "Expected short 34, got %d\n",pss);

        long pl = pdg->getLong("long");
        fprintf(f, "Expected long 56 , got %ld\n",pl);

        int64_t pi = pdg->getLong("longlong");
        fprintf(f, "Expected long long 78, got %ld\n",(long)pi);

        long double ld = pdg->getDouble("longdouble");
        fprintf(f, "Expected long double 89, got %.3f\n",ld);

        float pf = pdg->getFloat("float");
        fprintf(f, "Expected float 90, got %.3f\n",pf);

        SDODate  pt = pdg->getDate("date");
        fprintf(f, "Expected time_t 200, got %d\n",pt.getTime());


        wchar_t* result;
        // get the length to allocate:
        unsigned int widelen = pdg->getString("string",0,0);
        if (widelen > 0) {
            result = new wchar_t[widelen];
            widelen = pdg->getString("string",result,widelen);
            for ( i=0;i<widelen;i++)
            {
                fprintf(f, "Wide[%d]=%d\n",i,result[i]);
            }
            delete result;
        }


        char* thinresult;

        unsigned int thinlen = pdg->getBytes("bytes",0,0);
        if (thinlen > 0) {
            thinresult = new char[thinlen];
            thinlen = pdg->getBytes("bytes",thinresult,thinlen);
            for ( i=0;i<thinlen;i++)
            {
                fprintf(f, "Thin[%d]=%d\n",i,thinresult[i]);
            }
            delete thinresult;
        }


        // add two more departments - the first should provoke the
        // saving of a list in a change summary

        DataObjectPtr dep3 = dor->createDataObject("departments");
        dep3->setCString("name","Architecture");

        // The second should do no logging in the change summary

        DataObjectPtr dep4= dor->createDataObject("departments");
        dep4->setCString("name","Pyrotechnics");


        // create another one using tha alias
        DataObjectPtr dep5= dor->createDataObject("TheDepartments");
        dep5->setCString("name","Accounts");

        DataObjectPtr emp1 = dep1->createDataObject("employees");
        emp1->setCString("name","Eric");
    

        DataObjectPtr emp2 = dep1->createDataObject("employees");
        emp2->setCString("name","Bill");

        // now lets make eric employee of the month
        dor->setDataObject("employee of the month",emp1);

        // no - actually it was bill
        dor->setDataObject("employee of the month",emp2);


        
        // Now add to the list of floats, hopefully the two old
        // values will get change summarised.

        dolist = dor->getList("floatlist");
        float f1 = dolist.getFloat(0);

        fprintf(f, "%.3f",f1);

        dolist.setFloat(0,(float)567.7);

        f1 = dolist.getFloat(0);

        fprintf(f, "%.3f",f1);

        dolist.insert(0,(float)34.56);


        DataObjectPtr ddd = dor->getDataObject("departments[1]/employees[2]");

        const char* ch = ddd->getCString("name");

        fprintf(f, "Should be Bill:%s\n",ch);

 
        DataObjectPtr ddd2 = dor->getDataObject("TheDepartments[1]/employees[2]");

        const char* ch2 = ddd2->getCString("name");

        fprintf(f, "Should be Bill:%s\n",ch2);


        DataObjectPtr dempofm = dor->getDataObject("employee of the month");

        const char* chh = dempofm->getCString("name");

        fprintf(f, "The employee of the month is %s\n",chh);

        // Suppose we delete bill 

        DataObjectList& dlo = dor->getList("departments[1]/employees");
        DataObjectPtr fred = dlo.remove(1);

        const char* chh2 = fred->getCString("name");

        dempofm = dor->getDataObject("employee of the month");

        // uncertain bahaviour - should the reference be zeroed 
        // becuase the item is removed from a list or not? 
        // Right now it doesnt get done, as the assumption is that
        // the object will be put somewhere else, or dropped.
        // fprintf(f, "Emp of month is now zero : %p",dempofm);


    /* "The client would create a data mediator and get graph which 
    would return the root data object " */

        const char* boss = dor->getCString("pdg/name");
    
        fprintf(f, "%s\n", boss);

        DataObjectPtr mypdg = dor->getDataObject("pdg");

        Type::Types t = mypdg->getTypeEnum();
    
        if (t != Type::OtherTypes) {
            fprintf(f, "pdg correctly identified\n");
        }

        const char* boss2 = mypdg->getCString("name");

        fprintf(f, "%s\n", boss2);

        /* getPrimitiveArrayListVectorThingy() */

        DataObjectList& deps = dor->getList("departments");
    
        fprintf(f, "size of list %d\n",deps.size());

        DataObjectPtr dout = deps[0];

        dout->setCString("name","Research And Development");

        const char* snew = dout->getCString("name");

        fprintf(f, "%s\n", snew);

        string snew2 = dor->getCString("departments[1 ] /name");

        fprintf(f, "%s\n", snew2.c_str());

        for (int lx = 0; lx < deps.size(); lx++)
        {
            fprintf(f, "Department: %s\n",deps[lx]->getCString("name"));
        }

        cs->endLogging();


        try {
            // should fail - if localtype not set
            dor->setCString("departments","department label");
            const char* slabel = dor->getCString("departments");
            fprintf(f, "String in list type:%s\n", slabel);
            return 0;
        }
        catch (SDORuntimeException e)
        {
            fprintf(f, "Normal expected exception\n");
        }

        try 
        {
            const char* snew3 = dor->getCString(" ]awep50wi4,0qw9q]45]#        q2345        -t        -v3lt6o        -56o        56=-o7nl        ewv/;sdl        f[vxddglh]px        dfju/    g#k./jh#.k./");
            fprintf(f, "%s\n", snew3);
            return 0;
        }
        catch (SDOPropertyNotFoundException e)
        {
            fprintf(f, "Normal Invalid path exception\n");
        }

        try 
        {

        // catch a and a property not found 
            const Property& pp = dor->getType().getProperty("notaproperty");
            return 0;
        }
        catch (SDOPropertyNotFoundException e)
        {
            fprintf(f, "Normal SDOPropertyNotFound exception\n");
        }
        fclose(f);
        return comparefiles("maintest.dat","maintest.txt");

    }

    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << " in "; 
        if (!silent) cout << e.getFileName() << " at line ";
        if (!silent) cout << e.getLineNumber() << endl;
        if (!silent) cout << e.getFunctionName() << " ";
        if (!silent) cout << e.getSeverity() << " "; 
        if (!silent) cout << e.getMessageText() << endl;
        return 0;
    }
}

int sdotest::getproptest()
{
    // should be able to get a property by xpath...

    FILE *f;

    f = fopen("getproptest.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open getproptest.dat" << endl;
        return 0;
    }

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","Company");
    mdg->addType("myspace","Department");
    // manager is a sequenced type...
    mdg->addType("myspace","Manager", true, false);
    mdg->addType("myspace","Employee");


    /* Now add the properties to the types...*/

    const Type& tc = mdg->getType("myspace","Company");
    const Type& ts = mdg->getType("commonj.sdo","String");
    const Type& ti = mdg->getType("commonj.sdo","Integer");
    const Type& tf = mdg->getType("commonj.sdo","Float");
    const Type& tm = mdg->getType("myspace","Manager");
    const Type& td = mdg->getType("myspace","Department");
    const Type& te = mdg->getType("myspace","Employee");

    mdg->addPropertyToType(tc,"name",ts);

    mdg->addPropertyToType(tc,"address",ts);

    mdg->addPropertyToType(tc,"departments", td,true);

    // add a reference to employee of the month
    mdg->addPropertyToType(tc,"employee of the month", te,    false, false, false);

    /* A list of floats */
    mdg->addPropertyToType(tc,"floatlist", tf,true);
    mdg->addPropertyToType(tc,"director", tm);

    mdg->addPropertyToType(td,"name", ts);
    mdg->addPropertyToType(td,"id", ti);

    mdg->addPropertyToType(td,"manager", tm);
    mdg->addPropertyToType(tm,"name", ts);

    mdg->addPropertyToType(te,"name",ts);
    mdg->addPropertyToType(td,"employees",te,true,false,true);

    //company/departments/employees.
    // company - name, address, floatlist
    // department name, id, manager
    // employee name


    const Type& tcc = mdg->getType("myspace","Company");
    DataObjectPtr dor = mdg->create((Type&)tcc);


    const Property& p = dor->getType().getProperty("name");
    fprintf(f, "Companys name property is: %s\n",p.getName());

    const Property& p1 = dor->getType().getProperty("departments");
    fprintf(f, "Companys dept property is:%s\n",p1.getName());

    // now try some xpaths...

    try {

    const Property& p2 = dor->getType().getProperty("departments/employees");
    fprintf(f,"Departments empl property is:%s\n",p2.getName());

    const Property& p3 = dor->getType().getProperty("departments[456]/employees[123]");
    fprintf(f,"Departments empl property is:%s\n", p3.getName());

    const Property& p4 = dor->getType().getProperty("departments.34/employees.123/name");
    fprintf(f,"Employees name property is:%s\n", p4.getName());

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout <<"Exceptions - xpath didnt work" << endl;
        return 0;
    }
    try {
    const Property& p5 = dor->getType().getProperty("departments.34/[]/name");
    fprintf(f,"Employees name property is:%s\n",p5.getName());
    return 0;
    }
    catch (SDORuntimeException e)
    {
        fprintf(f,"Normal exception caught\n");
    }

    try {
    const Property& p6 = dor->getType().getProperty("deptartments");
    fprintf(f,"Deptartments property is:%s\n",p6.getName());
    return 0;
    }
    catch (SDORuntimeException e)
    {
        fprintf(f,"Normal exception caught\n");
    }

    try {
    const Property& p7 = dor->getType().getProperty("../company");
    fprintf(f,"Company property is:%s\n",p7.getName());
    return 0;
    }
    catch (SDORuntimeException e)
    {
        fprintf(f,"Normal exception caught\n");
    }
    fclose (f);
    return comparefiles("getproptest.dat","getproptest.txt");

}

int sdotest::querytest()
{
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    FILE *f = fopen("querytest.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open querytest.dat" << endl;
        return 0;
    }

    mdg->addType("myspace","Company");
    mdg->addType("myspace","Department");
    // manager is a sequenced type...
    mdg->addType("myspace","Manager", true, false);
    mdg->addType("myspace","Employee");


    /* Now add the properties to the types...*/

    
    const Type& tstring = mdg->getType("commonj.sdo","String");
    const Type& tbool=   mdg->getType("commonj.sdo","Boolean");
    const Type& tbyte=   mdg->getType("commonj.sdo","Byte");
    const Type& tchar=   mdg->getType("commonj.sdo","Character");
    const Type& tint=    mdg->getType("commonj.sdo","Integer");
    const Type& tlong=   mdg->getType("commonj.sdo","Long");
    const Type& tfloat=  mdg->getType("commonj.sdo","Float");
    const Type& tbytes = mdg->getType("commonj.sdo","Bytes");
    const Type& tdouble= mdg->getType("commonj.sdo","Double");

    const Type& tc = mdg->getType("myspace","Company");
    const Type& tm = mdg->getType("myspace","Manager");
    const Type& td = mdg->getType("myspace","Department");
    const Type& te = mdg->getType("myspace","Employee");

    mdg->addPropertyToType(tc,"name",tstring);

    mdg->addPropertyToType(tc,"departments", td,true);

    // add a reference to employee of the month
    mdg->addPropertyToType(tc,"employee of the month", te,    false, false, false);

    mdg->addPropertyToType(tc,"director", tm);

    mdg->addPropertyToType(td,"name", tstring);

    mdg->addPropertyToType(td,"manager", tm);

    mdg->addPropertyToType(tm,"name", tstring);


    mdg->addPropertyToType(td,"employees",te,true,false,true);

    mdg->addPropertyToType(te,"bool",tbool);
    mdg->addPropertyToType(te,"byte",tbyte);
    mdg->addPropertyToType(te,"char",tchar);
    mdg->addPropertyToType(te,"int",tint);
    mdg->addPropertyToType(te,"float",tfloat);
    mdg->addPropertyToType(te,"long",tlong);
    mdg->addPropertyToType(te,"double",tdouble);
    mdg->addPropertyToType(te,"bytes",tbytes);
    mdg->addPropertyToType(te,"string",tstring);
    mdg->addPropertyToType(te,"name",tstring);

    //company/departments/employees.
    // company - name, address, floatlist
    // department name, id, manager
    // employee name

    const Type& tcc = mdg->getType("myspace","Company");
    DataObjectPtr dor = mdg->create((Type&)tcc);

    DataObjectPtr dept = dor->createDataObject("departments");

    DataObjectPtr emp1 = dept->createDataObject("employees");
    DataObjectPtr emp2 = dept->createDataObject("employees");
    DataObjectPtr emp3 = dept->createDataObject("employees");

    emp1->setCString("name","Employee1");
    emp2->setCString("name","Employee2");
    emp3->setCString("name","Employee3");


    emp1->setBoolean("bool",true);
    emp1->setByte("byte",'1');
    emp1->setCharacter("char",'1');
    emp1->setInteger("int",1);
    emp1->setFloat("float",1.0);
    emp1->setLong("long",1);
    emp1->setDouble("double",1.0);

    char* bytes = new char[6];
    bytes[0] = 'h';
    bytes[1] = 'e';
    bytes[2] = 'l';
    bytes[3] = 'l';
    bytes[4] = 'o';

    emp1->setBytes("bytes",bytes,5);

    wchar_t* mbytes = new wchar_t[6];
    mbytes[0] = 'h';
    mbytes[1] = 'e';
    mbytes[2] = 'l';
    mbytes[3] = 'l';
    mbytes[4] = 'o';

    emp1->setString("string",mbytes,5);


    emp2->setBoolean("bool",true);
    emp2->setByte("byte",'2');
    emp2->setCharacter("char",'2');
    emp2->setInteger("int",200);
    emp2->setFloat("float",200.0);
    emp2->setLong("long",200);
    emp2->setDouble("double",200.0);

    char* bytes2 = new char[6];
    bytes2[0] = 'h';
    bytes2[1] = 'e';
    bytes2[2] = 'l';
    bytes2[3] = 'p';
    bytes2[4] = '!';

    emp2->setBytes("bytes",bytes2,5);

    wchar_t* mbytes2 = new wchar_t[6];
    mbytes2[0] = 'h';
    mbytes2[1] = 'e';
    mbytes2[2] = 'l';
    mbytes2[3] = 'p';
    mbytes2[4] = '!';

    emp2->setString("string",mbytes2,5);

    emp3->setBoolean("bool",false);
    emp3->setByte("byte",'4');
    emp3->setCharacter("char",'4');
    emp3->setInteger("int",400);
    emp3->setFloat("float",400.0);
    emp3->setLong("long",400);
    emp3->setDouble("double",400.0);

    char* bytes3 = new char[6];
    bytes3[0] = 'w';
    bytes3[1] = 'o';
    bytes3[2] = 'm';
    bytes3[3] = 'p';
    bytes3[4] = '!';

    emp3->setBytes("bytes",bytes3,5);

    wchar_t* mbytes3 = new wchar_t[6];
    mbytes3[0] = 'w';
    mbytes3[1] = 'o';
    mbytes3[2] = 'm';
    mbytes3[3] = 'p';
    mbytes3[4] = '!';

    emp3->setString("string",mbytes3,5);


    
    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[1]/employees[bool=false]");
        fprintf(f,  "Expected employee3: got %s\n",dob1->getCString("name"));

        DataObjectPtr dob2 = dor->getDataObject("departments[1]/employees[bool=true]");
        fprintf(f, "Expected employee1: got %s\n",dob2->getCString("name"));

        DataObjectPtr dob3 = dor->getDataObject("departments[1]/employees[byte=2]");
        fprintf(f, "Expected employee2: got %s\n",dob3->getCString("name"));

        DataObjectPtr dob4 = dor->getDataObject("departments[1]/employees[byte=1]");
        fprintf(f, "Expected employee1: got %s\n",dob4->getCString("name"));

        DataObjectPtr dob5 = dor->getDataObject("departments[1]/employees[bytes=hello]");
        fprintf(f, "Expected employee1: got %s\n",dob5->getCString("name"));

        DataObjectPtr dob5a = dor->getDataObject("departments[1]/employees[bytes=\"hello\"]");
        fprintf(f, "Expected employee1: got %s\n",dob5a->getCString("name"));
        
        DataObjectPtr dob5b = dor->getDataObject("departments[1]/employees[bytes='hello']");
        fprintf(f, "Expected employee1: got %s\n",dob5b->getCString("name"));

        DataObjectPtr dob6 = dor->getDataObject("departments[1]/employees[bytes=womp!]");
        fprintf(f, "Expected employee3: got %s\n",dob6->getCString("name"));

        DataObjectPtr dob7 = dor->getDataObject("departments[1]/employees[double=200.0]");
        fprintf(f, "Expected employee2: got %s\n",dob7->getCString("name"));

        DataObjectPtr dob8 = dor->getDataObject("departments[1]/employees[string=help!]");
        fprintf(f, "Expected employee2: got %s\n",dob8->getCString("name"));

        DataObjectPtr dob8a = dor->getDataObject("departments[1]/employees[string=\"help!\"]");
        fprintf(f, "Expected employee2: got %s\n",dob8a->getCString("name"));

        DataObjectPtr dob8b = dor->getDataObject("departments[1]/employees[string= 'help!']");
        fprintf(f, "Expected employee2: got %s\n",dob8b->getCString("name"));
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << e.getEClassName() << " in "; 
        if (!silent)cout << e.getFileName() << " at line ";
        if (!silent)cout << e.getLineNumber() << endl;
        if (!silent)cout << e.getFunctionName() << " ";
        if (!silent)cout << e.getMessageText() << endl;
        return 0;
    }
    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[1]/employees[bool=doughnut]");
        return 0;
    }
    catch (SDORuntimeException e)
    {
        fprintf(f, "Normal exception caught\n");
    }

    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[4]/employees[bool=true]");
        return 0;
    }
    catch (SDORuntimeException e)
    {
        fprintf(f, "Normal exception caught\n");
    }

    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[1]/employees[bytes=whoomp!]");
        return 0;
    }
    catch (SDORuntimeException e)
    {
        fprintf(f, "Normal exception caught\n");
    }

    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[1]/blmployees[bool=true]");
        return 0;
    }
    catch (SDORuntimeException e)
    {
        fprintf(f, "Normal exception caught\n");
    }

    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[teapot]/employees[bool=true]");
        return 0;
    }

    catch (SDORuntimeException e)
    {
        fprintf(f, "Normal exception caught\n");
    }

    delete bytes;
    delete bytes2;
    delete bytes3;
    delete mbytes;
    delete mbytes2;
    delete mbytes3;

    fclose (f);
    return comparefiles("querytest.dat","querytest.txt");
}

int sdotest::setmany()
{

    FILE *f;
    f = fopen("setmany.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open setmany.dat" << endl;
    }

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","School");
    mdg->addType("myspace","Class");
    mdg->addType("myspace","Teacher");
    mdg->addType("myspace","Child");
    mdg->addType("myspace","Address");

    /* Now add the properties to the types...*/

    
    const Type& tstring = mdg->getType("commonj.sdo","String");
    const Type& tint = mdg->getType("commonj.sdo","Integer");
    const Type& ts = mdg->getType("myspace","School");
    const Type& tc = mdg->getType("myspace","Class");
    const Type& tch = mdg->getType("myspace","Child");
    const Type& tt = mdg->getType("myspace","Teacher");
    const Type& ta = mdg->getType("myspace","Address");

    mdg->addPropertyToType(tc,"name",tstring);
    mdg->addPropertyToType(ts,"name",tstring);
    mdg->addPropertyToType(tch,"name",tstring);
    mdg->addPropertyToType(tt,"name",tstring);

    mdg->addPropertyToType(tc,"number",tint);
    mdg->addPropertyToType(ts,"number",tint);
    mdg->addPropertyToType(tch,"number",tint);
    mdg->addPropertyToType(tt,"number",tint);

    mdg->addPropertyToType(ts,"classes", tc,true);
    mdg->addPropertyToType(tc,"children", tch,true);
    mdg->addPropertyToType(tc,"teacher", tt);

    mdg->addPropertyToType(tch,"address", ta);
    mdg->addPropertyToType(ts,"address", ta);
    mdg->addPropertyToType(tt,"address", ta);

    mdg->addPropertyToType(ta,"lines", tstring, true);

    DataObjectPtr school = mdg->create((Type&)ts);

    DataObjectPtr class1 = school->createDataObject("classes");
    DataObjectPtr class2 = school->createDataObject("classes");

    DataObjectPtr teach1 = class1->createDataObject("teacher");

    DataObjectPtr kid1 = class1->createDataObject("children");
    DataObjectPtr kid2 = class1->createDataObject("children");
    DataObjectPtr kid3 = class1->createDataObject("children");

    DataObjectPtr teach2 = class2->createDataObject("teacher");

    DataObjectPtr kid4 = class2->createDataObject("children");
    DataObjectPtr kid5 = class2->createDataObject("children");
    DataObjectPtr kid6 = class2->createDataObject("children");

    school->setCString("name","King Norberts");
    school->createDataObject("address");
    school->setCString("address/lines.0","The Place");
    school->setCString("address/lines.1","1 The Avenue");
    school->setCString("address/lines.2","Murchester");
    school->setCString("address/lines.3","England");

    class1->setCString("name","Primary");
    class2->setCString("name","Secondary");

    class1->setCString("teacher/name","Mr Philbert Chloroform");
    class2->setCString("teacher/name","Mr Brian Onasis");

    kid1->setCString("name", "Witherspoon Jnr");
    kid2->setCString("name", "Snape");
    kid3->setCString("name", "Thannet");
    kid4->setCString("name", "Grimbling");
    kid5->setCString("name", "Snagget Minor");
    kid6->setCString("name", "Van Den Bograt");

    kid1->setInteger("number", 1);
    kid2->setInteger("number", 2);
    kid3->setInteger("number", 3);
    kid4->setInteger("number", 4);
    kid5->setInteger("number", 5);
    kid6->setInteger("number", 6);

    fprintf(f," School  :%s\n", school->getCString("name"));

    fprintf(f," Address1:%s\n",school->getCString("address/lines.0"));
    fprintf(f," Address2:%s\n",school->getCString("address/lines.1"));
    fprintf(f," Address3:%s\n",school->getCString("address/lines.2"));
    fprintf(f," Address4:%s\n",school->getCString("address/lines.3"));

    fprintf(f," Class   :%s\n",class1->getCString("name")); 
    fprintf(f," Teacher :%s\n",class1->getCString("teacher/name")); 
    fprintf(f," Pupil1  :%s\n",class1->getCString("children.0/name"));
    fprintf(f," Pupil2  :%s\n",class1->getCString("children[number=2]/name"));
    fprintf(f," Pupil3  :%s\n",class1->getCString("children[3]/name"));

    fprintf(f," Class   :%s\n",class2->getCString("name"));
    fprintf(f," Teacher :%s\n",class2->getCString("teacher/name"));
    fprintf(f," Pupil1  :%s\n",class2->getCString("children[1]/name"));
    fprintf(f," Pupil2  :%s\n",class2->getCString("children.1/name"));
    fprintf(f," Pupil3  :%s\n",class2->getCString("children[number=6]/name"));

    try {
        fprintf(f," Pupil3  :%s\n",class2->getCString("children[4]/name"));
        return 0;
    }
    catch (SDOPathNotFoundException e)
    {
        fprintf(f,"Expected path not found exception caught\n");
    }
    if (XpathHelper::isIndexed("abc[2]")) {
        fprintf(f, "Indexed as expected\n");
    }
    else {
        fprintf(f, "Bad - not indexed\n");
    }

    if (XpathHelper::isIndexed("wibble/[2]")) {
        fprintf(f, "Indexed as expected\n");
    }
    else {
        fprintf(f, "Bad - not indexed\n");
    }

    if (XpathHelper::isIndexed("wibble/wobble[2]")) {
        fprintf(f, "Indexed as expected\n");
    }
    else {
        fprintf(f, "Bad - not indexed\n");
    }

    if (XpathHelper::isIndexed("wibble/wobble.2")) {
        fprintf(f, "Indexed as expected\n");
    }
    else {
        fprintf(f, "Bad - not indexed\n");
    }

    if (XpathHelper::isIndexed("wibble/wobble.2")) {
        fprintf(f, "Indexed as expected\n");
    }
    else {
        fprintf(f, "Bad - not indexed\n");
    }

    try {
        fprintf(f," Address5  :%s\n",school->getCString("address/lines[5]"));
        return 0;
    }
    catch (SDOIndexOutOfRangeException e)
    {
        fprintf(f,"Normal index out of range caught\n");
    }

    try {
        school->setCString("address/lines[6]","PostCode");
        fprintf(f, "Debatable behaviour - appended\n");
        fprintf(f," Address4  :%s\n",school->getCString("address/lines[5]"));
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout <<"Unexpected exception"<< endl;
        return 0;
    }

    fclose(f);
    return comparefiles("setmany.dat","setmany.txt");
}

int sdotest::carotest2()
{
    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","root");

    mdg->addPropertyToType("myspace","root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

    const Type& troot = mdg->getType("myspace","root");

    DataObjectPtr root = mdg->create((Type&)troot);
    return 1;
    }
    catch (SDORuntimeException e)
    {
        return 0;
    }
}

int sdotest::adddeletetest()
{
    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","root");
    mdg->addType("myspace","bill");

    mdg->addPropertyToType("myspace","root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addPropertyToType("myspace","root","bill",
                           "myspace","bill");

    const Type& troot = mdg->getType("myspace","root");

    DataObjectPtr root = mdg->create((Type&)troot);

    ChangeSummaryPtr cs = root->getChangeSummary();
    cs->beginLogging();
    DataObjectPtr dob = root->createDataObject("bill");
    root->unset("bill");
    return 1;
    }
    catch (SDORuntimeException e)
    {
        return 0;
    }
}

int sdotest::carotest()
{

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","root");
    mdg->addType("myspace","company");
    mdg->addType("myspace","department");
    mdg->addType("myspace","employee");

    mdg->addPropertyToType("myspace","root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addPropertyToType("myspace","company","name",
                           "commonj.sdo","String", false, false, false);
    

    mdg->addPropertyToType("myspace","department","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","employee","name",
                           "commonj.sdo","String", false, false, false);
    

    mdg->addPropertyToType("myspace","root","companies",
                           "myspace","company", true, false, true);

    mdg->addPropertyToType("myspace","company","departments",
                           "myspace","department", true, false, true);

    mdg->addPropertyToType("myspace","company","eotm",
                           "myspace","employee", false, false, false);

    mdg->addPropertyToType("myspace","department","employees",
                           "myspace","employee", true, false, true);

    const Type& troot = mdg->getType("myspace","root");


    DataObjectPtr root = mdg->create((Type&)troot);
    ChangeSummaryPtr cs =  root->getChangeSummary();

    cs->beginLogging();

    DataObjectPtr comp = root->createDataObject("companies");

    comp->setCString("name","Acme");

    //cout << "name of the company is " << comp->getCString("name") << endl;
    try {
            root->unset("companies[0]");
            return 1;
    }
    catch (SDOUnsupportedOperationException e)
    {
        if (!silent)cout <<  "Abnormal unsupported operation for unset of many valued item" << endl;
        return 0;
    }
}


int sdotest::bug2()
{

    FILE *f;

    f = fopen("bug2.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open bug2.dat" << endl;
        return 0;
    }

    try  {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","root");
    mdg->addType("myspace","company");

    mdg->addPropertyToType("myspace","root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addPropertyToType("myspace","company","name",
                           "commonj.sdo","String", false, false, false);
    

    mdg->addPropertyToType("myspace","root","companies",
                           "myspace","company", true, false, true);


    const Type& troot = mdg->getType("myspace","root");


    DataObjectPtr root = mdg->create((Type&)troot);

    ChangeSummaryPtr cs =  root->getChangeSummary();

    cs->beginLogging();

    DataObjectPtr comp = root->createDataObject("companies");

    comp->setCString("name","Acme");

    dumpproperties(f,root);

    DataObjectList& dl = root->getList("companies");
    dl.remove(0);

    dumpproperties(f,root);


    fprintf(f, "Change summary should have no entries...\n");

    dumpchangesummary(f,cs);

    dumpproperties(f,root);

    fprintf(f, "Change summary should have no entries...\n");

    dumpchangesummary(f,cs);

    fclose(f);
    return comparefiles("bug2.dat","bug2.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent) cout << e.getEClassName() << " in "; 
        if (!silent) cout << e.getFileName() << " at line ";
        if (!silent) cout << e.getLineNumber() << endl;
        if (!silent) cout << e.getFunctionName() << " ";
        if (!silent) cout << e.getMessageText() << endl;
        return 0;
    }
}


int sdotest::dumpproperties(FILE *f, DataObjectPtr root)
{
    PropertyList pl = root->getInstanceProperties();
    for (int i=0;i<pl.size();i++)
    {
        if (pl[i].isMany())
        {
            DataObjectList& dl = root->getList(pl[i]);
            if (pl[i].getType().isDataType())
            {
                char buf[10];
                for (int j=0;j<dl.size();j++)
                {

                    sprintf(buf,"%02d",j);
                    fprintf(f,"MProperty:%s[%s]:%s\n",
                        pl[i].getName(),buf,dl.getCString(j));
                }
            }
            else
            {
                fprintf(f,"MObject Property %s\n",pl[i].getName());
                for (int j=0;j<dl.size();j++)
                {
                    if (dl[j] != 0)
                    {
                        dumpproperties(f, dl[j]);
                    }
                }
                fprintf(f,"End of MObject Property %s\n",pl[i].getName());
            }
        }

        else
        {
            if (pl[i].getType().isDataType())
            {
                fprintf(f,"Property:%s:%s\n",pl[i].getName(),root->getCString(pl[i]));
            }
            else
            {
                fprintf(f,"Object Property%s\n:",pl[i].getName());
                DataObjectPtr d = root->getDataObject(pl[i]);
                if (d != 0)
                {
                    dumpproperties(f, d);
                }
                else
                {
                    fprintf(f,"  Value was not set or null\n");
                }
                fprintf(f,"End of Object Property %s\n",pl[i].getName());
            }
        }
    }
    return 1;
}


int sdotest::datetest()
{

    try  {

    FILE *f = fopen("datetest.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open datetest.dat" << endl;
        return 0;
    }

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","root");
    mdg->addType("myspace","company");

    mdg->addPropertyToType("myspace","root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addPropertyToType("myspace","company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","company","startupdate",
                           "commonj.sdo","Date", false, false, false);
    
    mdg->addPropertyToType("myspace","company","reviewdates",
                           "commonj.sdo","Date", true, false, false);


    mdg->addPropertyToType("myspace","root","company",
                           "myspace","company", false, false, true);



    const Type& troot = mdg->getType("myspace","root");


    DataObjectPtr root = mdg->create((Type&)troot);
    ChangeSummaryPtr cs =  root->getChangeSummary();

    cs->beginLogging();

    DataObjectPtr comp = root->createDataObject("company");


    comp->setCString("name","DateTest");

    fprintf(f,"Name of the company is %s\n",comp->getCString("name"));

    comp->setDate("startupdate",SDODate( 1000 ));

    fprintf(f,"Set start of the company is %d\n",comp->getDate("startupdate").getTime());
    
    DataObjectList& dol = comp->getList("reviewdates");
    
    dol.append(SDODate(2000));
    dol.append(SDODate(4000));

    for (int i=0;i < dol.size(); i++)
    {
        fprintf(f,"Review number:%d was:%d\n", i,dol.getDate(i).getTime());
        fprintf(f,"Formatted:%d was:%s\n",i,dol.getDate(i).ascTime());
    }

    cs->endLogging();

    fclose(f);

    return comparefiles("datetest.dat","datetest.txt"); 
    }

    catch (SDORuntimeException e)
    {
        if (!silent)cout << e.getEClassName() << " in "; 
        if (!silent)cout << e.getFileName() << " at line ";
        if (!silent)cout << e.getLineNumber() << endl;
        if (!silent)cout << e.getFunctionName() << " ";
        if (!silent)cout << e.getMessageText() << endl;
        return 0;
    }

}

int sdotest::matttest1()
{

    try  {
    FILE *f = fopen("matttest1.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open matttest1.dat" << endl;
        return 0;
    }

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","root");
    mdg->addType("myspace","company");
    mdg->addType("myspace","department");
    mdg->addType("myspace","employee");

    mdg->addPropertyToType("myspace","root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addPropertyToType("myspace","company","name",
                           "commonj.sdo","String", false, false, false);
    

    mdg->addPropertyToType("myspace","department","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","employee","name",
                           "commonj.sdo","String", false, false, false);
    

    mdg->addPropertyToType("myspace","root","companies",
                           "myspace","company", true, false, true);

    mdg->addPropertyToType("myspace","company","departments",
                           "myspace","department", true, false, true);

    mdg->addPropertyToType("myspace","company","eotm",
                           "myspace","employee", false, false, false);

    mdg->addPropertyToType("myspace","department","employees",
                           "myspace","employee", true, false, true);

    const Type& troot = mdg->getType("myspace","root");


    DataObjectPtr root = mdg->create((Type&)troot);

    DataObjectPtr comp = root->createDataObject("companies");

    comp->setCString("name","Acme");    

    DataObjectPtr dept1 = comp->createDataObject("departments");

    dept1->setCString("name","Shoe");    

    DataObjectPtr dept2 = comp->createDataObject("departments");

    dept2->setCString("name","IT");    

    DataObjectPtr sue = dept1->createDataObject("employees");

    sue->setCString("name","Sue");
    
    DataObjectPtr billy = dept2->createDataObject("employees");

    billy->setCString("name","Billy");

    // Want to swap Sue and Billy so, while holding them in variables, should be
    //able to unset from department then reinsert

    // method 1 - clear the lists...
    dept1->unset("employees");
    dept2->unset("employees");

    DataObjectPtr cont = billy->getContainer();

    if (cont != 0) 
    {
        if (!silent)cout << "Container of Billy should be zero, but is :" << cont->getCString("name") << endl;
        return 0;
    }

    DataObjectList& li1 = dept1->getList("employees");
    DataObjectList& li2 = dept2->getList("employees");
    li1.append(billy);
    li2.append(sue);

    DataObjectPtr cont2 = billy->getContainer();

    if (cont2 != 0) 
    {
        fprintf(f,"Container of Billy should be dept1, and is :%s\n", cont2->getCString("name"));
    }
    else {
        if (!silent)cout << "Problem - Billy is not contained." << endl;
        return 0;
    }

    li1.remove(0);
    li2.remove(0);

    DataObjectPtr cont3 = billy->getContainer();

    if (cont3 != 0) 
    {
        if (!silent)cout << "Container of Billy should be zero, but is :" << cont3->getCString("name") << endl;
        return 0;
    }

    li1.append(sue);
    li2.append(billy);

    DataObjectPtr cont4 = billy->getContainer();

    if (cont4 != 0) 
    {
        fprintf(f,"Container of Billy should be dept2, and is :%s\n", cont4->getCString("name"));
    }
    else {
        if (!silent)cout << "Problem - Billy is not contained." << endl;
        return 0;
    }

    fclose(f);

    return comparefiles("matttest1.dat","matttest1.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << e.getEClassName() << " in "; 
        if (!silent)cout << e.getFileName() << " at line ";
        if (!silent)cout << e.getLineNumber() << endl;
        if (!silent)cout << e.getFunctionName() << " ";
        if (!silent)cout << e.getMessageText() << endl;
        return 0;
    }
}

int sdotest::carotest3()
{
    // sequence new APIs 
    // data object list, new getLength(unsigned int)


    FILE *f = fopen("carotest3.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open carotest3.dat" << endl;
        return 0;
    }
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    // company is sequenced.
    mdg->addType("myspace","Company",true,false);
    
    mdg->addType("myspace","Department",true,false);
    // string is many

    mdg->addPropertyToType("myspace","Company","string","commonj.sdo","String",
        true);

    mdg->addPropertyToType("myspace","Company","departments","myspace","Department",
        true);
    
    mdg->addPropertyToType("myspace","Company","strings","commonj.sdo","String",
        true);

    mdg->addPropertyToType("myspace","Company","integers","commonj.sdo","Integer",
        true);


    mdg->addPropertyToType("myspace","Department","name","commonj.sdo","String");


    const Type& tcc = mdg->getType("myspace","Company");
    
    DataObjectPtr dor = mdg->create(tcc);
    const Property& sprop = dor->getType().getProperty("string");

    SequencePtr sptr = dor->getSequence();

    printseq(f,sptr);

    sptr->addCString(sprop,"I am Item 1 of string");
    
    printseq(f,sptr);

    sptr->addText("I am the first free text");
    
    printseq(f,sptr);

    sptr->addCString(sprop,"I am Item 2 of string");
    
    printseq(f,sptr);

    sptr->setText(1,"I am free text which has been modified");

    printseq(f,sptr);

    DataObjectPtr dep1 = dor->createDataObject("departments");

    printseq(f,sptr);

    dep1->setCString("name","department1");

    printseq(f,sptr);

    DataObjectList& dol = dor->getList("departments");

    unsigned int ii = dol.getLength(0);

    if (ii != 0)
    {
        if (!silent)cout << "Length of a data object should be zero:" << ii << endl;
        return 0;
    }

    DataObjectList& strl = dor->getList("strings");

    wchar_t * buf = new wchar_t[5];
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = 'd';
    buf[4] = 'e';

    strl.append(buf,1);
    strl.append(buf,2);
    strl.append(buf,3);
    strl.append(buf,4);
    strl.append(buf,5);

    fprintf(f,"Element zero, length 1: %d\n",strl.getLength(0));
    fprintf(f,"Element one,  length 2: %d\n",strl.getLength(1));
    fprintf(f,"Element two,  length 3: %d\n",strl.getLength(2));
    fprintf(f,"Element three,length 4: %d\n",strl.getLength(3));
    fprintf(f,"Element four, length 5: %d\n",strl.getLength(4));

    try {
        int xx = strl.getLength(5);
        if (!silent)cout << "problem - element 5 should not exist" << endl;
        return 0;
    }
    catch (SDOIndexOutOfRangeException e)
    {
        if (!silent)cout << "Normal out of range exception in test" << endl;
    }

    DataObjectList& numl = dor->getList("integers");

    try {
        int xx2 = numl.getLength(0);
        if (!silent)cout << "problem - element 0 should not exist" << endl;
        return 0;
    }
    catch (SDOIndexOutOfRangeException e)
    {
        if (!silent)cout << "Normal out of range exception in test" << endl;
    }
    fclose(f);
    return comparefiles("carotest3.dat","carotest3.txt");
}


int sdotest::eqhelpertest()
{

    // copy and equality helpers

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","Assembly");
    mdg->addType("myspace","SubAssembly");
    mdg->addType("myspace","Part");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tint     = mdg->getType("commonj.sdo","Integer");
    const Type& tassy    = mdg->getType("myspace","Assembly");
    const Type& tsubassy =  mdg->getType("myspace","SubAssembly");
    const Type& tpart    = mdg->getType("myspace","Part");

    
    mdg->addPropertyToType(tassy,"name",tstring);
    mdg->addPropertyToType(tsubassy,"name",tstring);
    mdg->addPropertyToType(tpart,"name",tstring);

    mdg->addPropertyToType(tassy,"number",tint);
    mdg->addPropertyToType(tsubassy,"number",tint);
    mdg->addPropertyToType(tpart,"number",tint);

    mdg->addPropertyToType(tassy,"subassemblies", tsubassy,true);
    mdg->addPropertyToType(tsubassy,"parts", tpart,true);


    // assy 1 

    DataObjectPtr assy1 = mdg->create((Type&)tassy);

    DataObjectPtr subassy1 = assy1->createDataObject("subassemblies");
    DataObjectPtr subassy2 = assy1->createDataObject("subassemblies");

    DataObjectPtr part1 = subassy1->createDataObject("parts");
    DataObjectPtr part2 = subassy1->createDataObject("parts");
    DataObjectPtr part3 = subassy1->createDataObject("parts");

    DataObjectPtr part4 = subassy2->createDataObject("parts");
    DataObjectPtr part5 = subassy2->createDataObject("parts");
    DataObjectPtr part6 = subassy2->createDataObject("parts");


    assy1->setCString("name","MicroFrame");
    assy1->setInteger("number",32768);

    subassy1->setCString("name","Legs");
    subassy2->setCString("name","Body");
    subassy1->setInteger("number",1);
    subassy2->setInteger("number",2);

    part1->setCString("name","Leg1");
    part2->setCString("name","Leg2");
    part3->setCString("name","Leg3");
    part1->setInteger("number",10001);
    part2->setInteger("number",10002);
    part3->setInteger("number",10003);


    part4->setCString("name","MainStrut");
    part5->setCString("name","Brace1");
    part6->setCString("name","Brace2");
    part4->setInteger("number",20001);
    part5->setInteger("number",20002);
    part6->setInteger("number",20003);

    // assy2 - shallow equal to assy, but not deep equal

    DataObjectPtr assy2 = mdg->create((Type&)tassy);

    DataObjectPtr subassy3 = assy2->createDataObject("subassemblies");
    DataObjectPtr subassy4 = assy2->createDataObject("subassemblies");

    DataObjectPtr part7 = subassy3->createDataObject("parts");
    DataObjectPtr part8 = subassy3->createDataObject("parts");
    DataObjectPtr part9 = subassy3->createDataObject("parts");

    DataObjectPtr part10 = subassy4->createDataObject("parts");
    DataObjectPtr part11 = subassy4->createDataObject("parts");
    DataObjectPtr part12 = subassy4->createDataObject("parts");


    assy2->setCString("name","MicroFrame");
    assy2->setInteger("number",32768);

    subassy3->setCString("name","Legs");
    subassy4->setCString("name","Body");
    subassy3->setInteger("number",1);
    subassy4->setInteger("number",2);

    part7->setCString("name","Leg1");
    part8->setCString("name","Leg2");
    part9->setCString("name","Leg3");
    part7->setInteger("number",10001);
    part8->setInteger("number",10002);
    part9->setInteger("number",10003);


    part10->setCString("name","MainStrut");
    part11->setCString("name","Brace1");
    part12->setCString("name","OddBrace2");
    part10->setInteger("number",20001);
    part11->setInteger("number",20002);
    part12->setInteger("number",20003);

    if (!EqualityHelper::equalShallow(assy1, assy2))
    {
        if (!silent) cout << "EQUALITY HELPER TEST - failure at first test" << endl;
        return 0;
    }
    if (EqualityHelper::equal(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at second test" << endl;
        return 0;
    }

    // Now alter assy2 to be deep equal...


    part12->setCString("name","Brace2");

    if (!EqualityHelper::equalShallow(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at third test" << endl;
        return 0;
    }
    if (!EqualityHelper::equal(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at fourth test" << endl;
        return 0;
    }
 
    // now add a part, so its not deep equal again
    DataObjectPtr part13 = subassy4->createDataObject("parts");

    if (!EqualityHelper::equalShallow(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at fifth test" << endl;
        return 0;
    }
    if (EqualityHelper::equal(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at sixth test" << endl;
        return 0;
    }
    
    // now remove that part again...
    DataObjectList& dl = subassy4->getList("parts");
    dl.remove(3);

    // should be deep equal again...

    if (!EqualityHelper::equalShallow(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at seventh test" << endl;
        return 0;
    }
    if (!EqualityHelper::equal(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at eighth test" << endl;
        return 0;
    }
    // now make them not shallow equal
    assy2->setInteger("number",32767);

    if (EqualityHelper::equalShallow(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at nineth test" << endl;
        return 0;
    }
    if (EqualityHelper::equal(assy1, assy2))
    {
        if (!silent)cout << "EQUALITY HELPER TEST - failure at tenth test" << endl;
        return 0;
    }
    return 1;
}

int sdotest::cohelpertest()
{

    // copy and equality helpers

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","Assembly");
    mdg->addType("myspace","SubAssembly");
    mdg->addType("myspace","Part");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tint     = mdg->getType("commonj.sdo","Integer");
    const Type& tassy    = mdg->getType("myspace","Assembly");
    const Type& tsubassy =  mdg->getType("myspace","SubAssembly");
    const Type& tpart    = mdg->getType("myspace","Part");

    
    mdg->addPropertyToType(tassy,"name",tstring);
    mdg->addPropertyToType(tsubassy,"name",tstring);
    mdg->addPropertyToType(tpart,"name",tstring);

    mdg->addPropertyToType(tassy,"number",tint);
    mdg->addPropertyToType(tsubassy,"number",tint);
    mdg->addPropertyToType(tpart,"number",tint);

    mdg->addPropertyToType(tassy,"subassemblies", tsubassy,true);
    mdg->addPropertyToType(tsubassy,"parts", tpart,true);


    // assy 1 

    DataObjectPtr assy1 = mdg->create((Type&)tassy);

    DataObjectPtr subassy1 = assy1->createDataObject("subassemblies");
    DataObjectPtr subassy2 = assy1->createDataObject("subassemblies");

    DataObjectPtr part1 = subassy1->createDataObject("parts");
    DataObjectPtr part2 = subassy1->createDataObject("parts");
    DataObjectPtr part3 = subassy1->createDataObject("parts");

    DataObjectPtr part4 = subassy2->createDataObject("parts");
    DataObjectPtr part5 = subassy2->createDataObject("parts");
    DataObjectPtr part6 = subassy2->createDataObject("parts");


    assy1->setCString("name","MicroFrame");
    assy1->setInteger("number",32768);

    subassy1->setCString("name","Legs");
    subassy2->setCString("name","Body");
    subassy1->setInteger("number",1);
    subassy2->setInteger("number",2);

    part1->setCString("name","Leg1");
    part2->setCString("name","Leg2");
    part3->setCString("name","Leg3");
    part1->setInteger("number",10001);
    part2->setInteger("number",10002);
    part3->setInteger("number",10003);


    part4->setCString("name","MainStrut");
    part5->setCString("name","Brace1");
    part6->setCString("name","Brace2");
    part4->setInteger("number",20001);
    part5->setInteger("number",20002);
    part6->setInteger("number",20003);

    // first lets shallow copy assy1:

    DataObjectPtr assy2 = CopyHelper::copyShallow(assy1);

    if (strcmp(assy2->getCString("name"),"MicroFrame"))
    {
        if (!silent)cout << "COPYHELPER shallow copy failed(1)" << endl;
        return 0;
    }
    else if (assy2->getInteger("number") != 32768)
    {
        if (!silent)cout << "COPYHELPER shallow copy 1 failed(2)" << endl;
        return 0;
    }
    else if (assy2->getList("subassemblies").size()  != 0)
    {
        if (!silent)cout << "COPYHELPER shallow copy 1 failed(3)" << endl;
        return 0;
    }
    else if (assy2->getContainer() != 0)
    {
        if (!silent)cout << "COPYHELPER shallow copy failed(4)" << endl;
        return 0;
    }

    // now deep copy it

    DataObjectPtr assy3 = CopyHelper::copy(assy1);

    if (!EqualityHelper::equal(assy1, assy3))
    {
        if (!silent)cout << "COPYHELPER deep copy failed(1)" << endl;
        return 0;
    }
    else if (assy3->getContainer() != 0)
    {
        if (!silent)cout << "COPYHELPER deep copy failed(2)" << endl;
        return 0;
    }
    return 1;
}


int sdotest::cssave()
{

    try {


    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    xsh->defineFile("company.xsd");

    //mdg->addType("companyNS","CompanyType");
    //mdg->addType("companyNS","EmployeeType");
    //mdg->addType("companyNS","DepartmentType");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    
    //mdg->addPropertyToType(tcomp,"name",tstring);
    // not containment...
    //mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp, false,false,false);
    //mdg->addPropertyToType(tcomp,"departments",tdept, true);
    //mdg->addPropertyToType(tcomp,"cs",tcs);

    //mdg->addPropertyToType(tdept,"name",tstring);
    //mdg->addPropertyToType(tdept,"location",tstring);
    //mdg->addPropertyToType(tdept,"number",tstring);
    //mdg->addPropertyToType(tdept,"employees",temp,true);
    
    //mdg->addPropertyToType(temp, "name",tstring);
    //mdg->addPropertyToType(temp, "SN",  tstring);
    //mdg->addPropertyToType(temp, "manager",  tbool);


    // create a graph, then save it

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","ACME");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","Advanced Technologies");
    dept->setCString("location","NY");
    dept->setCString("number","123");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);

    emp1->setCString("name","John Jones");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Mary Smith");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Jane Doe");
    emp3->setCString("SN","E0003");

    DataObjectList& dol2 = dept->getList("employees");
    dol2.append(emp1);
    dol2.append(emp2);
    dol2.append(emp3);

    comp->setDataObject("employeeOfTheMonth",emp2);

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    DataObjectPtr emp4 = mdg->create(temp);
    emp4->setCString("name","Al Smith");
    emp4->setCString("SN","E0004");
    emp4->setBoolean("manager",true);

    // first change - create employee 4
    dol2.append(emp4);

    dol2.remove(1); // element 1  is Mary

    DataObjectPtr emp5 = mdg->create(temp);
    emp5->setCString("name","Bill Withers");
    emp5->setCString("SN","E0005");

    dol2.append(emp5);


    comp->setCString("name","MegaCorp");
    comp->setDataObject("employeeOfTheMonth",emp4);

    // silly test - add mary back again
    dol2.append(emp2);




    cs->endLogging();


    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"cssave-output.xml");
    return comparefiles("cssave-output.xml","cssave-output.txt");


    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "CSSAVE FAILED" << e << endl;
        return 0;
    }
}

int sdotest::cssave2()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    xsh->defineFile("company.xsd");

   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    
    // create a graph, then save it

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","Eastleigh Borough Council");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","Waste Disposal");
    dept->setCString("location","Botley");
    dept->setCString("number","123");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);
    DataObjectPtr emp4 = mdg->create(temp);
    DataObjectPtr emp5 = mdg->create(temp);

    emp1->setCString("name","Alphonse Dodet");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Bridget Jones");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Colin Thorne");
    emp3->setCString("SN","E0003");

    emp4->setCString("name","Donald Trump");
    emp4->setCString("SN","E0004");
    
    emp5->setCString("name","Eddy the eagle");
    emp5->setCString("SN","E0005");

    DataObjectList& dol2 = dept->getList("employees");
    dol2.append(emp1);
    dol2.append(emp2);
    dol2.append(emp3);
    dol2.append(emp4);
    dol2.append(emp5);

    //comp->setDataObject("employeeOfTheMonth",emp2);

    DataObjectPtr dept2 = mdg->create((Type&)tdept);
    dol.append(dept2);

    dept2->setCString("name","Tax Collection");
    dept2->setCString("location","Winchester");
    dept2->setCString("number","666");

    DataObjectPtr empb1 = mdg->create(temp);
    DataObjectPtr empb2 = mdg->create(temp);
    DataObjectPtr empb3 = mdg->create(temp);
    DataObjectPtr empb4 = mdg->create(temp);
    DataObjectPtr empb5 = mdg->create(temp);

    empb1->setCString("name","Arch Meanie");
    empb1->setCString("SN","D0001");

    empb2->setCString("name","Boris the Spider");
    empb2->setCString("SN","D0002");
    empb2->setBoolean("manager",true);

    empb3->setCString("name","Cash Hoarder");
    empb3->setCString("SN","D0003");

    empb4->setCString("name","Dean Giyatoss");
    empb4->setCString("SN","D0004");
    
    empb5->setCString("name","Ebenezer Scrooge");
    empb5->setCString("SN","D0005");

    DataObjectList& dol3 = dept2->getList("employees");
    dol3.append(empb1);
    dol3.append(empb2);
    dol3.append(empb3);
    dol3.append(empb4);
    dol3.append(empb5);

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    // start by deleting a few employees from dept 1

    dol2.remove(1); // B 
    dol2.remove(1); // C
    dol2.remove(1); // D

    // Now delete the tax collectors as a whole

    dol.remove(1);

    // now perhaps add back one of the employees

    dol2.append(emp3); // C

    comp->setCString("name","MegaCorp");
    
    try {
        comp->setDataObject("employeeOfTheMonth",emp4);
        if (!silent)cout << "emp4 was not in the tree" << endl;
        return 0;
    }
    catch (SDORuntimeException e)
    {
    }

    comp->setDataObject("employeeOfTheMonth",emp5);

    cs->endLogging();


    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"cssave2-output.xml");
    return comparefiles("cssave2-output.xml","cssave2-output.txt");
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout <<  "CSSAVE FAILED" << e << endl;
        return 0;
    }
}





int sdotest::b45933()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();


    mdg->addType("companyNS","CompanyType");
    mdg->addType("companyNS","WombatType");

    /* Now add the properties to the types...*/

   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& twom     = mdg->getType("companyNS","WombatType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);
    mdg->addPropertyToType(tcomp,"bools",tbool, true);
    mdg->addPropertyToType(tcomp,"woms",twom, true);
    mdg->addPropertyToType(tcomp,"cs",tcs);


    // create a graph, then save it

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","Bug45933");

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    DataObjectList& dol = comp->getList("bools");
    dol.append(false);
    dol.remove(0);
    DataObjectPtr awom = mdg->create((Type&)twom);
    DataObjectList& dol2 = comp->getList("woms");
    dol2.append(awom);
    dol2.remove(0);

    cs->endLogging();

    //dumpchangesummary(cs);

    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"bug45933-output.xml");
    return comparefiles("bug45933-output.xml","bug45933-output.txt");
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "BUG45933 FAILED" << endl << e << endl;
        return 0;
    }
}

int sdotest::b46617()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    //XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    //xsh->defineFile("b46617.xsd");
    mdg->addType("companyNS","CompanyType");
    mdg->addType("companyNS","DepartmentType");
    mdg->addType("companyNS","EmployeeType");


   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    mdg->addPropertyToType(tcomp,"name",tstring);
    mdg->addPropertyToType(tcomp,"departments",tdept,true,false,true);
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp,false,false,false);
    mdg->addPropertyToType(tcomp,"CEO",temp,false,false,true);

    mdg->addPropertyToType(tdept,"employees",temp,true,false,true);
    mdg->addPropertyToType(tdept,"name",tstring,false,false,true);
    mdg->addPropertyToType(tdept,"location",tstring,false,false,true);
    mdg->addPropertyToType(tdept,"number",tstring,false,false,true);

    mdg->addPropertyToType(temp,"name",tstring,false,false,true);
    mdg->addPropertyToType(temp,"SN",tstring,false,false,true);
    mdg->addPropertyToType(temp,"manager",tbool,false,false,true);

    // create a graph, then save it

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","Eastleigh Borough Council");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","Waste Disposal");
    dept->setCString("location","Botley");
    dept->setCString("number","123");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);

    emp1->setCString("name","Alphonse Dodet");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Bridget Jones");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Carl Marx");
    emp3->setCString("SN","E0003");

    comp->setDataObject("CEO",emp2);

    DataObjectList& dol2 = dept->getList("employees");
    dol2.append(emp1);
    //dol2.append(emp2);
    dol2.append(emp3);

    comp->setDataObject("employeeOfTheMonth",emp2);

    XSDHelperPtr xmd = HelperProvider::getXSDHelper(mdg);
    xmd->generateFile(mdg->getTypes(),
                "b46617.xsd","companyNS");


    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"b46617.xml");

    // now try loading the xml, using the same schema, and see if we get the
    // same answer

    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("b46617.xml", "companyNS");
    DataObjectPtr newdob = myXMLDocument->getRootDataObject();
    

    if (strcmp(newdob->getDataObject("employeeOfTheMonth")->getCString("name"),
        "Bridget Jones"))
    {
        if (!silent)cout << "Employee of the month name has changed" << endl;
        return 0;
    }
     
    return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "b46617 failed" << e << endl;
        return 0;
    }
}

int sdotest::b46617b()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    xsh->defineFile("b46617b.xsd");

    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("b46617b.xml", "companyNS");
    DataObjectPtr newdob = myXMLDocument->getRootDataObject();
    
    if (strcmp(newdob->getDataObject("employeeOfTheMonth")->getCString("name"),
        "Fred Smith"))
    {
        if (!silent)cout << "Employee of the month name has changed" << endl;
        return 0;
    }
    
    return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "b46617b failed" << e << endl;
        return 0;
    }
}

int sdotest::notfound()
{

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    try {

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    xsh->defineFile("not_present.xsd");
    }
    catch (SDOFileNotFoundException e)
    {
        //cout << "NotFound threw correct exception " << endl;
    }

    catch (SDORuntimeException e)
    {
        if (!silent)cout << "NOTFOUND THREW THE WRONG EXCEPTION" << e << endl;
        return 0;
    }

    try {
    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("not-present.xml", "companyNS");

    }
    catch (SDOFileNotFoundException ee)
    {
        //cout << "NotFound threw the correct exception" << endl;
    }

    catch (SDORuntimeException e)
    {
        if (!silent)cout << "NOTFOUND THREW THE WRONG EXCEPTION" << e << endl;
        return 0;
    }
    return 1;
}

int sdotest::csload()
{
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("company.xsd");

        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;

        //TypeList tl = mdg->getTypes();
        //for (int i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("cssave-output.xml", "companyNS");
        //myXMLDocument->setRootElementName("company");
        //myXMLHelper->save(myXMLDocument,"csload-output.xml");
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "csload-output.xml");

        return comparefiles("csload-output.xml","csload-output.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "CSLOAD FAILED" << e << endl;
        return 0;
    }

}

int sdotest::testsubsload()
{
    try 
    {
        FILE *f = fopen("testsubsload.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open testsubsload.dat" << endl;
            return 0;
        }

        fprintf(f,"TEST: TestSubsLoad ==========================================\n");
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("companysubs.xsd");

        //cout << "TYPES LOADED FROM COMPANYSUBS XSD" << endl;

        //TypeList tl = mdg->getTypes();
        //for (int i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        // so now we should be able to put book or a magazine into
        // the publication under company...

        const Type& tstring  = mdg->getType("commonj.sdo","String");
        const Type& tcomp    = mdg->getType("companyNS","CompanyType");
        const Type& book     = mdg->getType("companyNS","BookType");
        const Type& mag      = mdg->getType("companyNS","MagazineType");
        const Type& pub      = mdg->getType("companyNS","PublicationType");

    
        // create a graph, then save it

        DataObjectPtr comp = mdg->create((Type&)tcomp);
        comp->setCString("name","Puflet Publishing");


        DataObjectPtr book1 = mdg->create(book);
        book1->setCString("author","Mr P B Writer");
        // inherted from publication
        book1->setCString("title","Nowhere Man");
        
        DataObjectPtr mag1 = mdg->create(mag);
        mag1->setCString("editor","Mr B Picky");
        // inherited from publication
        mag1->setCString("title","Bionicle Weekly");
    
        DataObjectPtr pub1 = mdg->create(pub);
        pub1->setCString("title","Noddy In Toyland");


        // publication should accept any of them...

        comp->setDataObject("Publication",pub1);
        const Type& tpub1 = comp->getDataObject("Publication")->getType();
        fprintf(f,"Publication is of type %s\n",tpub1.getName());

        comp->setDataObject("Publication",book1);
        const Type& tpub2 = comp->getDataObject("Publication")->getType();
        fprintf(f,"Publication is of type %s\n",tpub2.getName());

        comp->setDataObject("Publication",mag1);
        const Type& tpub3 = comp->getDataObject("Publication")->getType();
        fprintf(f,"Publication is of type %s\n",tpub3.getName());

        // should be able to address publication as book or magazine

        comp->setDataObject("Book",book1);
        const Type& tpub4 = comp->getDataObject("Book")->getType();
        fprintf(f, "Book is of type %s\n",tpub4.getName());

        comp->setDataObject("Magazine",mag1);
        const Type& tpub5 = comp->getDataObject("Magazine")->getType();
        fprintf(f,"Magazine is of type %s\n",tpub5.getName());
        fprintf(f,"END TEST: TestSubsLoad ======================================\n");
        fclose(f);
        return comparefiles("testsubsload.dat","testsubsload.txt");
    
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "ERROR in TeseSubsLoad" << e << endl;
        return 0;
    }
}

int sdotest::testSCA()
{
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("sca.xsd");
        return 1;
    
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "ERROR in TeseSCA" << e << endl;
        return 0;
    }
}

int sdotest::testabstract()
{
    try 
    {

        FILE *f = fopen("testabstract.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open testabstract.dat" << endl;
            return 0;
        }

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("companyabs.xsd");

        //cout << "TYPES LOADED FROM COMPANYABS XSD" << endl;

        //TypeList tl = mdg->getTypes();
        //for (int i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        // so now we should be able to put book or a magazine into
        // the publication under company...

        const Type& tstring  = mdg->getType("commonj.sdo","String");
        const Type& tcomp    = mdg->getType("companyNS","CompanyType");
        const Type& book     = mdg->getType("companyNS","BookType");
        const Type& mag      = mdg->getType("companyNS","MagazineType");
        const Type& pub      = mdg->getType("companyNS","PublicationType");

    
        // create a graph, then save it

        DataObjectPtr comp = mdg->create((Type&)tcomp);
        comp->setCString("name","Puflet Publishing");

        // should be allowed...
        DataObjectPtr book1 = mdg->create(book);
        book1->setCString("author","Mr P B Writer");
        // inherted from publication
        book1->setCString("title","Nowhere Man");
        
        DataObjectPtr mag1 = mdg->create(mag);
        mag1->setCString("editor","Mr B Picky");
        // inherited from publication
        mag1->setCString("title","Bionicle Weekly");
    
        try {
            DataObjectPtr pub1 = mdg->create(pub);
            pub1->setCString("title","Noddy In Toyland");
        }
        catch (SDOUnsupportedOperationException e)
        {
            fprintf(f,"Normal unsupportedoperation for creation of abstract type\n");
        }


        // publication should accept books and magazines...

        comp->setDataObject("Publication",book1);
        const Type& tpub2 = comp->getDataObject("Publication")->getType();
        fprintf(f,"Publication is of type %s\n",tpub2.getName());

        comp->setDataObject("Publication",mag1);
        const Type& tpub3 = comp->getDataObject("Publication")->getType();
        fprintf(f,"Publication is of type %s\n",tpub3.getName());

        // should be able to address publication as book or magazine

        comp->setDataObject("Book",book1);
        const Type& tpub4 = comp->getDataObject("Book")->getType();
        fprintf(f,"Book is of type %s\n",tpub4.getName());

        comp->setDataObject("Magazine",mag1);
        const Type& tpub5 = comp->getDataObject("Magazine")->getType();
        fprintf(f,"Magazine is of type %s\n",tpub5.getName());

        fclose(f);
        return comparefiles("testabstract.dat","testabstract.txt");
    
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "ERROR in TestAbstract" << e << endl;
        return 0;
    }
}

int sdotest::testOpen()
{
    try 
    {

    FILE *f = fopen("testopen.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open testopen.dat" << endl;
        return 0;
    }

    DataFactoryPtr mdg  = DataFactory::getDataFactory();


    mdg->addType("companyNS","CompanyType");
    // employee will be an open type...
    mdg->addType("companyNS","EmployeeType", /*seq*/false, 
                                             /*open*/true, 
                                             /*abs */ false, 
                                             /*data*/ false);

    mdg->addType("companyNS","DepartmentType");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);
    // not containment...
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp, false,false,false);
    mdg->addPropertyToType(tcomp,"departments",tdept, true);
    mdg->addPropertyToType(tcomp,"cs",tcs);

    mdg->addPropertyToType(tdept,"name",tstring);
    mdg->addPropertyToType(tdept,"location",tstring);
    mdg->addPropertyToType(tdept,"number",tstring);
    mdg->addPropertyToType(tdept,"employees",temp,true);
    
    mdg->addPropertyToType(temp, "name",tstring);
    mdg->addPropertyToType(temp, "SN",  tstring);
    mdg->addPropertyToType(temp, "manager",  tbool);



    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","ACME");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","Advanced Technologies");
    dept->setCString("location","NY");
    dept->setCString("number","123");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);

    emp1->setCString("name","Albert");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Boris");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Carl");
    emp3->setCString("SN","E0003");

    // now we should be able to add extra properties to employees,
    // and get them back again...

    try {
        emp1->setNull("openstring");
        return 0;
    }
    catch (SDOUnsupportedOperationException e)
    {
        fprintf(f,"Normal exception for setting null on undefined prop\n");
    }

    emp1->setCString("openstring","Value Of Open String");
    emp1->setBoolean("openboolean",true);

    const char* c = emp1->getCString("openstring");

    fprintf(f,"Open Type string value: %s\n",c);

    bool b = emp1->getBoolean("openboolean");

    fprintf(f,"Open Type boolean value: %d\n", b);

    // unknown list type at present..
    DataObjectList& dl = emp1->getList("opentypelist");
    
    int i = 45;
    dl.append((short)i); // now the list must be primitive

    // we should now have instance properties 
    PropertyList pl = emp1->getType().getProperties();

    for (i=0;i<pl.size();i++)
    {
        fprintf(f,"EMP1 type property: %s ( many? %d) of type %s\n" ,pl[i].getName()
            , pl[i].isMany(),
            pl[i].getType().getName());
    }

    PropertyList pli = emp1->getInstanceProperties();

    for (i=0;i<pli.size();i++)
    {
        fprintf(f, "EMP1 inst property: %s (many? %d) of type %s\n",
            pli[i].getName(), pli[i].isMany(), pli[i].getType().getName());
    }


    // now we try to set the string to null..
    
    emp1->setNull("openstring");

    emp1->setCString("openstring","New Value Of Open String");


    // now unset them and see them disappear - what happens to the indices?

    emp1->unset("openstring");

    PropertyList plk = emp1->getInstanceProperties();

    for (i=0;i<plk.size();i++)
    {
        fprintf(f,"EMP1 inst property: %s (many? %d) of type %s\n",
            plk[i].getName(), plk[i].isMany(), plk[i].getType().getName());
    }

    emp1->unset("openboolean");

    dl.remove(0); // so is this list gone, or empty - I vote for still there.

    PropertyList pll = emp1->getInstanceProperties();
    for (i=0;i<pll.size();i++)
    {
        fprintf(f,"EMP1 inst property: %s (many? %d) of type %s\n",
            pll[i].getName(), pll[i].isMany(),pll[i].getType().getName());
    }

    emp1->unset("opentypelist");

    // now I vote for gone...

    PropertyList plm = emp1->getInstanceProperties();
    for (i=0;i<plm.size();i++)
    {
        fprintf(f,"EMP1 inst property: %s (many? %d) of type %s\n", plm[i].getName()
            , plm[i].isMany() , plm[i].getType().getName());
    }

    fclose(f);
    return comparefiles("testopen.dat","testopen.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "ERROR in TestOpen" << e << endl;
        return 0;
    }
}

int sdotest::saveOpen()
{
    try 
    {

    DataFactoryPtr mdg  = DataFactory::getDataFactory();


    mdg->addType("companyNS","CompanyType");
    // employee will be an open type...
    mdg->addType("companyNS","EmployeeType", /*seq*/false, 
                                             /*open*/true, 
                                             /*abs */ false, 
                                             /*data*/ false);

    mdg->addType("companyNS","DepartmentType");

    mdg->addType("companyNS","OpenType");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");
    const Type& topen    = mdg->getType("companyNS","OpenType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);
    // not containment...
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp, false,false,false);
    mdg->addPropertyToType(tcomp,"departments",tdept, true);
    mdg->addPropertyToType(tcomp,"cs",tcs);

    mdg->addPropertyToType(tdept,"name",tstring);
    mdg->addPropertyToType(tdept,"location",tstring);
    mdg->addPropertyToType(tdept,"number",tstring);
    mdg->addPropertyToType(tdept,"employees",temp,true);
    
    mdg->addPropertyToType(temp, "name",tstring);
    mdg->addPropertyToType(temp, "SN",  tstring);
    mdg->addPropertyToType(temp, "manager",  tbool);

    mdg->setAlias("companyNS","CompanyType","TheFirm");
    mdg->setAlias("companyNS","CompanyType","departments","TheDepartments");
    mdg->setAlias("companyNS","CompanyType","departments","MYDepartments");
    mdg->setAlias("companyNS","CompanyType","TheBigFirm");

    mdg->addPropertyToType(topen,"name",tstring);

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","ACME");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    dept->setCString("name","Advanced Technologies");
    dept->setCString("location","NY");
    dept->setCString("number","123");

    DataObjectList& emps = dept->getList("employees");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);

    emps.append(emp1);
    emps.append(emp2);
    emps.append(emp3);

    emp1->setCString("name","Albert");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Boris");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Carl");
    emp3->setCString("SN","E0003");

    // now we should be able to add extra properties to employees,
    // and get them back again...


    emp1->setCString("openstring","Value Of Open String");
    emp1->setBoolean("openboolean",true);

    const char* c = emp1->getCString("openstring");

    //cout << "Open Type string value: " << c << endl;

    bool b = emp1->getBoolean("openboolean");

    //cout << "Open Type boolean value: " << b << endl;

    // unknown list type at present..
    DataObjectList& dl = emp1->getList("openintlist");
    
    int i = 45;
    dl.append((short)i); // now the list must be primitive and short!!

    // unknown list type at present..
    DataObjectList& dl2 = emp1->getList("opendataobjectlist");
    
    DataObjectPtr myopen = mdg->create("companyNS","OpenType");
    myopen->setCString("name","MyOpenName");
    dl2.append(myopen); // now the list must be of data objects
    

    // now a single valued data object - should have an xsi:type
    emp1->setDataObject("opendataobject", myopen);


    cs->endLogging();

    //dumpchangesummary(cs);

    XSDHelperPtr xdh = HelperProvider::getXSDHelper(mdg);

    xdh->generateFile(mdg->getTypes(),
                "saveopen-output.xsd","companyNS");

    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"saveopen-output.xml");

    return comparefiles("saveopen-output.xml","saveopen-output.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "ERROR in SaveOpen" << e << endl;
        return 0;
    }
}

int sdotest::loadOpen()
{
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("saveopen-output.xsd");


        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;
        //int i;
        //TypeList tl = mdg->getTypes();
        //for (i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("partial.xml", "companyNS");
        
        

        // so, we now expect that the openboolean and openstring props will
        // be of type boolean and Bytes, and the openlist will be a list of integers.
        // The dataobjectlist an dataobject will be of the correct OpenType

        // we need a test for data objects, and that we are writing out xsi:type
        // correctly.
        DataObjectPtr comp = myXMLDocument->getRootDataObject();
        DataObjectList& dl = comp->getList("departments");
        DataObjectPtr dept = dl[0];
        DataObjectList& dl2 = dept->getList("employees");
        DataObjectPtr emp = dl2[0];

        if (strcmp(emp->getProperty("openboolean").getType().getName(),
            "Boolean"))
        {
            if (!silent)cout << "OpenLoad failure - bool property not boolean" << endl;
            return 0;

        }
        if (emp->getBoolean("openboolean") != true)
        {
            if (!silent)cout << "OpenLoad failure - bool property not true" << endl;
            return 0;
        }

        if (strcmp(emp->getProperty("openstring").getType().getName(),
            "Bytes"))
        {
            if (!silent)cout << "OpenLoad failure - bytes property not bytes" << endl;
            return 0;

        }
        if (strcmp(emp->getCString("openstring"),"Value Of Open String"))
        {
            if (!silent)cout << "OpenLoad failure - string value is" << 
                emp->getCString("openstring") << endl;
            return 0;
        }


        DataObjectList& dl3 = emp->getList("openintlist");

        if (strcmp(emp->getProperty("openintlist").getType().getName(),
            "Short"))
        {
            if (!silent)cout << "OpenLoad failure - IntegerList not Short " <<
                emp->getProperty("openintlist").getType().getName() << endl;
            return 0;
        }

        if (dl3.getInteger(0) != 45)
        {
            if (!silent)cout << "OpenLoad failure - Integer value is" << 
                dl.getInteger(0) << endl;
            return 0;
        }

        if (strcmp(emp->getProperty("opendataobjectlist").getType().getName(),
            "OpenType"))
        {
            if (!silent)cout << "OpenLoad failure - OpenTypeList not OpenType" << endl;
            return 0;
        }

        DataObjectList& dl4 = emp->getList("opendataobjectlist");

        if (strcmp(dl4[0]->getCString("name"),"MyOpenName"))
        {
            if (!silent)cout << "OpenLoad failure - list element name is " <<
                dl4[0]->getCString("name") << endl;
            return 0;
        }

        if (strcmp(emp->getProperty("opendataobject").getType().getName(),
            "OpenType"))
        {
            if (!silent)cout << "OpenLoad failure - OpenType not OpenType" << endl;
            return 0;
        }

        DataObjectPtr dob2 = emp->getDataObject("opendataobject");

        if (strcmp(dob2->getCString("name"),"MyOpenName"))
        {
            if (!silent)cout << "OpenLoad failure - open value name is " <<
                dob2->getCString("name") << endl;
            return 0;
        }

        
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "loadload-output.xml");
        return comparefiles("loadload-output.xml","loadload-output.txt");


    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "LoadOpen FAILED" << e << endl;
        return 0;
    }

}

int sdotest::loadOpenNS()
{
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("openloadNS.xsd");
        xsh->defineFile("openloadNS2.xsd");


        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;
        //int i;
        //TypeList tl = mdg->getTypes();
        //for (i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("openloadNS.xml", "companyNS");
        
        

        // so, we now expect that the openboolean and openstring props will
        // be of type boolean and Bytes, and the openlist will be a list of integers.
        // The dataobjectlist an dataobject will be of the correct OpenType

        // we need a test for data objects, and that we are writing out xsi:type
        // correctly.
        DataObjectPtr comp = myXMLDocument->getRootDataObject();
        DataObjectList& dl = comp->getList("departments");
        DataObjectPtr dept = dl[0];
        DataObjectList& dl2 = dept->getList("employees");
        DataObjectPtr emp = dl2[0];

        if (strcmp(emp->getProperty("openboolean").getType().getName(),
            "Boolean"))
        {
            if (!silent)cout << "OpenLoadNS failure - bool property not boolean" << endl;
            return 0;

        }
        if (emp->getBoolean("openboolean") != true)
        {
            if (!silent)cout << "OpenLoadNS failure - bool property not true" << endl;
            return 0;
        }

        if (strcmp(emp->getProperty("openstring").getType().getName(),
            "Bytes"))
        {
            if (!silent)cout << "OpenLoadNS failure - bytes property not bytes" << endl;
            return 0;

        }
        if (strcmp(emp->getCString("openstring"),"Value Of Open String"))
        {
            if (!silent)cout << "OpenLoadNS failure - string value is" << 
                emp->getCString("openstring") << endl;
            return 0;
        }


        DataObjectList& dl3 = emp->getList("openintlist");

        if (strcmp(emp->getProperty("openintlist").getType().getName(),
            "Short"))
        {
            if (!silent)cout << "OpenLoadNS failure - IntegerList not Short " <<
                emp->getProperty("openintlist").getType().getName() << endl;
            return 0;
        }

        if (dl3.getInteger(0) != 45)
        {
            if (!silent)cout << "OpenLoadNS failure - Integer value is" << 
                dl.getInteger(0) << endl;
            return 0;
        }


        
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "openloadNSout.xml");

        if (!comparefiles("openloadNSout.xml","openloadNSout.txt"))return 0;


        // now try exactly the same, but with NS which doesnt exist
        XMLDocumentPtr myXMLDocument2 = myXMLHelper->loadFile("openloadNS2.xml", "companyNS");
        
        

        // so, we now expect that the openboolean and openstring props will
        // be of type boolean and Bytes, and the openlist will be a list of integers.
        // The dataobjectlist an dataobject will be of the correct OpenType

        // we need a test for data objects, and that we are writing out xsi:type
        // correctly.
        DataObjectPtr comp2 = myXMLDocument2->getRootDataObject();
        DataObjectList& dld = comp2->getList("departments");
        DataObjectPtr dept2 = dld[0];
        DataObjectList& dl22 = dept2->getList("employees");
        DataObjectPtr emp2 = dl22[0];

        if (strcmp(emp2->getProperty("openboolean").getType().getName(),
            "Bytes"))
        {
            if (!silent)cout << "OpenLoadNS2 failure - bool property not bytes" << endl;
            return 0;

        }
        if (emp2->getBoolean("openboolean") != true)
        {
            if (!silent)cout << "OpenLoadNS2 failure - bool property not true" << endl;
            return 0;
        }

        if (strcmp(emp2->getProperty("openstring").getType().getName(),
            "Bytes"))
        {
            if (!silent)cout << "OpenLoadNS2 failure - bytes property not bytes" << endl;
            return 0;

        }
        if (strcmp(emp2->getCString("openstring"),"Value Of Open String"))
        {
            if (!silent)cout << "OpenLoadNS failure - string value is" << 
                emp2->getCString("openstring") << endl;
            return 0;
        }


        DataObjectList& dl32 = emp2->getList("openintlist");

        if (strcmp(emp2->getProperty("openintlist").getType().getName(),
            "Bytes"))
        {
            if (!silent)cout << "OpenLoadNS2 failure - IntegerList not Bytes " <<
                emp2->getProperty("openintlist").getType().getName() << endl;
            return 0;
        }

        if (dl32.getInteger(0) != 45)
        {
            if (!silent)cout << "OpenLoadNS2 failure - Integer value is" << 
                dl32.getInteger(0) << endl;
            return 0;
        }
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "LoadOpenNS FAILED" << e << endl;
        return 0;
    }

}

int sdotest::loadManyOpen()
{
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("many.xsd");


        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;
        //int i;
        //TypeList tl = mdg->getTypes();
        //for (i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("many.xml", "companyNS");
        
        

        // so, we now expect that the openboolean and openstring props will
        // be of type boolean and Bytes, and the openlist will be a list of integers.
        // The dataobjectlist an dataobject will be of the correct OpenType

        // we need a test for data objects, and that we are writing out xsi:type
        // correctly.
        DataObjectPtr comp = myXMLDocument->getRootDataObject();
        DataObjectList& dl = comp->getList("departments");
        DataObjectPtr dept = dl[0];
        DataObjectList& dl2 = dept->getList("employees");
        DataObjectPtr emp = dl2[0];


        DataObjectList& dl3 = emp->getList("openlist");

        if (strcmp(emp->getProperty("openlist").getType().getName(),
            "Bytes"))
        {
            if (!silent)cout << "OpenManyLoad failure - BytesList not Bytes " <<
                emp->getProperty("openlist").getType().getName() << endl;
            return 0;
        }

        if (dl3.size() != 3)
        {
            if (!silent)cout << "OpenManyLoad failure - size is " << 
                dl3.size() << endl;
            return 0;
        }

        if (strcmp(dl3.getCString(0),"Three member")) return 0;
        if (strcmp(dl3.getCString(1),"Open bytes")) return 0;
        if (strcmp(dl3.getCString(2),"List is complete")) return 0;

        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "LoadOpen FAILED" << e << endl;
        return 0;
    }

}

int sdotest::csload2()
{
    //cout << "TEST: CSLoad2 ===============================================" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("company.xsd");

        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;

        //TypeList tl = mdg->getTypes();
        //for (int i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("cssave2-output.xml", "companyNS");
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "csload2-output.xml");

        if (!comparefiles("csload2-output.xml","csload2-output.txt")) return 0;

        XMLDocumentPtr myXMLDocument2 = myXMLHelper->loadFile("csload2-output.xml", "companyNS");
        myXMLHelper->save(myXMLDocument2->getRootDataObject(),
                          0, "company", "csload3-output.xml");

        return comparefiles("csload3-output.xml","csload3-output.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "ERROR in CsLoad2" << e << endl;
        return 0;
    }
}

int sdotest::bug1()
{

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    try {
    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    xsh->defineFile("bugs/1/company.xsd");

    //TypeList tl = mdg->getTypes();
    //for (int i=0;i< tl.size(); i++)
    //{
    //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
    //}


    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("bugs/1/company.xml", 
    "http://www.mycompanyinc.com");
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "ERROR in Bug1" << e << endl;
        return 0;
    }
    return 1;
}

int sdotest::setnull()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    mdg->addType("NullNS","NullMainType");
    mdg->addType("NullNS","NullSubType");

    /* Now add the properties to the types...*/
   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tint     = mdg->getType("commonj.sdo","Integer");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tmain    = mdg->getType("NullNS","NullMainType");
    const Type& tsub     = mdg->getType("NullNS","NullSubType");
 
    
    mdg->addPropertyToType(tmain,"name",tstring);
    mdg->addPropertyToType(tmain,"subs",tsub, true);  // many
    mdg->addPropertyToType(tmain,"asub",tsub, false); // single
    mdg->addPropertyToType(tmain,"bsub",tsub, false); // single

    mdg->addPropertyToType(tmain,"cs",tcs); // change summary

    mdg->addPropertyToType(tsub,"name",tstring);
    mdg->addPropertyToType(tsub,"number",tint);
    mdg->addPropertyToType(tsub,"bool",tbool);


    // create a graph, then save it

    DataObjectPtr main1 = mdg->create((Type&)tmain);
    main1->setCString("name","MainObject");

    // asub is initially set
    DataObjectPtr sub0 = main1->createDataObject("asub");
    // bsub is not set.

    DataObjectPtr sub1 = mdg->create((Type&)tsub);
    DataObjectPtr sub2 = mdg->create((Type&)tsub);
    DataObjectPtr sub3 = mdg->create((Type&)tsub);

    DataObjectList& dol = main1->getList("subs");

    dol.append(sub1);
    dol.append(sub2);
    dol.append(sub3);

    // sub1 has all values set initially

    sub1->setCString("name","All Initially set");
    sub1->setInteger("number",100);
    sub1->setBoolean("bool",false);

    // sub2 has no values set at all


    // sub3 has all values set to null
    sub3->setNull("name");
    sub3->setNull("number");
    sub3->setNull("bool");


    // report
    
    if (main1->isNull("asub")) return 0;

    if (!main1->isSet("asub")) return 0;
    
    if (main1->isNull("bsub")) return 0;
    
    if (main1->isSet("bsub")) return 0;

// dol0 is set, not null

    if (dol[0]->isNull("name")) return 0;

    if (dol[0]->isNull("number")) return 0;

    if (dol[0]->isNull("bool")) return 0;

    if (!dol[0]->isSet("name")) return 0;

    if (!dol[0]->isSet("number")) return 0;

    if (!dol[0]->isSet("bool")) return 0;

// dol1 is all unset , so no nulls
    
    if (dol[1]->isNull("name")) return 0;

    if (dol[1]->isNull("number")) return 0;

    if (dol[1]->isNull("bool")) return 0;

    if (dol[1]->isSet("name")) return 0;
    
    if (dol[1]->isSet("number")) return 0;

    if (dol[1]->isSet("bool")) return 0;

// dol2 is all nulls, so all set
    
    if (!dol[2]->isNull("name")) return 0;

    if (!dol[2]->isNull("number")) return 0;

    if (!dol[2]->isNull("bool")) return 0;

    if (!dol[2]->isSet("name")) return 0;

    if (!dol[2]->isSet("number")) return 0;

    if (!dol[2]->isSet("bool")) return 0;
    
    ChangeSummaryPtr cs = main1->getChangeSummary();

    cs->beginLogging();

    main1->setNull("asub");
    main1->setNull("bsub");

    dol[0]->setNull("name");
    dol[0]->setNull("number");
    dol[0]->setNull("bool");

    dol[1]->setNull("bool");

    dol[2]->setCString("name","NewName");
    dol[2]->setInteger("number",100);
    dol[2]->setBoolean("bool",true);


    cs->endLogging();

    // what would we expect from the change summary?

    //cout << "==============Expected in Change Summary=================" << endl;
    //cout << "change to main1 - object asub set to null, oldvalue asub (set)" << endl;
    //cout << "change to main1 - object bsub set to null, oldvalue null (unset)" << endl;
    //cout << "change to sub1  - name asub set to null, oldvalue \"All Initially Set\" (set)" << endl;
    //cout << "change to sub1  - number asub set to null, oldvalue 100 (set)" << endl;
    //cout << "change to sub1  - bool asub set to null, oldvalue false (set)" << endl;
    //cout << "change to sub2  - bool asub set to null, oldvalue false (unset)" << endl;
    //cout << "change to sub3  - name asub set to \"NewName\", oldvalue null, set" << endl;
    //cout << "change to sub3  - number asub set to 100, oldvalue null, set" << endl;
    //cout << "change to sub3  - bool asub set to true, oldvalue null, set" << endl;
    //cout << "==============End Expected in Change Summary=============" << endl;

    FILE* f = fopen("setnull.dat","w+");
    if (f == 0)
    {
        if (!silent) cout << "Unable to open setnull.dat" << endl;
        return 0;
    }

    dumpchangesummary(f, cs);

    fclose(f);

    if (!comparefiles("setnull.dat","setnull.txt")) return 0;
    
    // report

    if (!main1->isNull("asub")) return 0;

    if (!main1->isSet("asub")) return 0;

    if (!main1->isNull("bsub")) return 0;

    if (!main1->isSet("bsub")) return 0;

// dol0 is set to null

    if (!dol[0]->isNull("name")) return 0;

    if (!dol[0]->isNull("number")) return 0;

    if (!dol[0]->isNull("bool")) return 0;

    if (!dol[0]->isSet("name")) return 0;

    if (!dol[0]->isSet("number")) return 0;

    if (!dol[0]->isSet("bool")) return 0;

// dol1 is all unset except bool which is null 
    
    if (dol[1]->isNull("name")) return 0;

    if (dol[1]->isNull("number")) return 0;

    if (!dol[1]->isNull("bool")) return 0;

    if (dol[1]->isSet("name")) return 0;

    if (dol[1]->isSet("number")) return 0;

    if (!dol[1]->isSet("bool")) return 0;

// dol2 is all set non-null, so all set
    
        if (dol[2]->isNull("name")) return 0;

        if (dol[2]->isNull("number")) return 0;

        if (dol[2]->isNull("bool")) return 0;

        if (!dol[2]->isSet("name")) return 0;

        if (!dol[2]->isSet("number")) return 0;

        if (!dol[2]->isSet("bool")) return 0;


        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in setnull" << e << endl;
        return 0;
    }
}


int sdotest::testsubs()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    mdg->addType("companyNS","MainType");

    mdg->addType("companyNS","SubType");
    mdg->addType("companyNS","SuperType");
    mdg->addType("companyNS","SubstituteType");
    mdg->addType("companyNS","UnrelatedType");

    /* Now add the properties to the types...*/

   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tmain    = mdg->getType("companyNS","MainType");
    const Type& sub      = mdg->getType("companyNS","SubType");
    const Type& super    = mdg->getType("companyNS","SuperType");
    const Type& subst    = mdg->getType("companyNS","SubstituteType");
    const Type& unrel    = mdg->getType("companyNS","UnrelatedType");

    
    mdg->addPropertyToType(tmain,"name",tstring);
    mdg->addPropertyToType(tmain,"sub",super, false);
    mdg->addPropertyToType(tmain,"subs",super, true);


    mdg->setPropertySubstitute(tmain, "sub","wilbur", subst);
    mdg->setPropertySubstitute(tmain, "subs","wilburs", subst);

    mdg->setBaseType(sub,super);

    //now sub and subs should accept all the three types

    DataObjectPtr main = mdg->create((Type&)tmain);
    main->setCString("name","SubstituteTest");

    DataObjectList& dol = main->getList("subs");

    DataObjectPtr sub1 = mdg->create(sub);
    DataObjectPtr super1 = mdg->create(super);
    DataObjectPtr subst1 = mdg->create(subst);
    DataObjectPtr unrel1 = mdg->create(unrel);

    dol.append(sub1);
    dol.append(super1);
    dol.append(subst1);
    try 
    {
        dol.append(unrel1);
        return 0;
    }
    catch (SDOInvalidConversionException e)
    {
        //cout << "Normal failure to append wrong type" << endl;
    }

    dol.remove(0);
    dol.remove(0);
    dol.remove(0);

    DataObjectList& dol2 = main->getList("wilburs");

    dol2.append(sub1);
    dol2.append(super1);
    dol2.append(subst1);
    try 
    {
        dol2.append(unrel1);
        return 0;
    }
    catch (SDOInvalidConversionException e)
    {
        //cout << "Normal failure to append wrong type" << endl;
    }

    main->setDataObject("sub",sub1);
    main->setDataObject("sub",super1);
    main->setDataObject("sub",subst1);

    main->setDataObject("wilbur",sub1);
    main->setDataObject("wilbur",super1);
    main->setDataObject("wilbur",subst1);

    try 
    {
        main->setDataObject("sub", unrel1);
        return 0;
    }
    catch (SDOInvalidConversionException e)
    {
        //cout << "Normal failure to set wrong type" << endl;
    }
    return 1;
    }

    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Substitute test failed" << endl << e << endl;
        return 0;
    }
}


int sdotest::merle1()
{

    try {

        // Dynamic setup of the metadata 

        FILE *f = fopen("merle1.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open merle1.dat" << endl;
            return 0;
        }


        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("axis.xsd");

        // or dynamically create, but that would get attributes for the strings, not elements
        //mdg->addType("axis","ItemSearch",true); // sequenced
        //mdg->addType("axis","ItemSearchRequest",true); // sequenced
    
        const Type& tstring  = mdg->getType("commonj.sdo","String");
        const Type& tsearch  = mdg->getType("axis","ItemSearch");
        const Type& treq     = mdg->getType("axis","ItemSearchRequest");
    
        //mdg->addPropertyToType(tsearch,"SubscriptionId",tstring);
        //mdg->addPropertyToType(tsearch,"AssociateTag",tstring);
        //mdg->addPropertyToType(tsearch,"XMLEscaping",tstring);
        //mdg->addPropertyToType(tsearch,"Validate",tstring);
        //mdg->addPropertyToType(tsearch,"Shared",treq, false, false, true);
        //mdg->addPropertyToType(tsearch,"Request",treq, true, false, true);

        //mdg->addPropertyToType(treq,"Keywords",tstring);
        //mdg->addPropertyToType(treq,"SearchIndex",tstring);

        // dynamic creation of the data 

        DataObjectPtr search = mdg->create((Type&)tsearch);
    
        search->setCString("SubscriptionId","I am the subscription ID string");
        search->setCString("AssociateTag","I am the associate Tag string");
        search->setCString("XMLEscaping","I am the XML escaping string");
        search->setCString("Validate","I am the Validate string");

        DataObjectPtr shared = search->createDataObject("Shared");
        shared->setCString("Keywords","SharedKeywordsString");
        shared->setCString("SearchIndex","SearchIndexString");

        DataObjectList& requests = search->getList("Request");
        DataObjectPtr request1 = mdg->create(treq);
        requests.append(request1);
        request1->setCString("Keywords","RequestKeywords");
        request1->setCString("SearchIndex","RequestSearchIndex");

        // Read the data and put it into XML:

        PropertyList properties = search->getInstanceProperties();

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        for (int i=0;i<properties.size();i++)
        {
            Property& prop = properties[i];
            switch (prop.getTypeEnum())
            {

                case Type::StringType:
                    fprintf(f,"<%s>%s</%s>\n",prop.getName(), search->getCString(prop),
                        prop.getName());
                break;

                case Type::DataObjectType:
                    if (prop.isMany())
                    {
                        // many valued property, such as Requests , so get the list
                        DataObjectList& rq = search->getList(prop);
                        for (int j=0;j<rq.size();j++)
                        {
                            DataObjectPtr dob = rq[j];
                            if (dob != 0)
                            {
                                XMLDocumentPtr doc = myXMLHelper->createDocument(dob,
                                "axis", prop.getName());
                                doc->setXMLDeclaration(false);
                                char* dobXML = myXMLHelper->save(doc);
                                fprintf(f,"DataObject \"%s\" as XML:\n",prop.getName()); 
                                fprintf(f,"%s\n", dobXML);
                            } // if dob
                        } // for 
                    } // if many
                    else
                    {
                        DataObjectPtr dob = search->getDataObject(prop);
                        if (dob != 0)
                        {
                            XMLDocumentPtr doc = myXMLHelper->createDocument(dob,
                                "axis", prop.getName());
                            doc->setXMLDeclaration(false);
                            char* dobXML = myXMLHelper->save(doc);
                            fprintf(f,"DataObject \"%s\" as XML:\n",prop.getName());
                            fprintf(f, "%s\n", dobXML);
                        } // if
                    } // else
                break;

                default:
                break;

            } // switch
        } // for
        fclose(f);
        return comparefiles("merle1.dat","merle1.txt");
    } // try
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Merle 1 failed" << endl << e << endl;
        return 0;
    }
}


int sdotest::graham3()
{
    try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("graham3.xsd");
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Graham1 failed " << e << endl;
        return 0;
    }
}

int sdotest::graham1()
{
    try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("graham1.xsd");
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Graham1 failed " << e << endl;
        return 0;
    }
}
int sdotest::graham2()
{
    try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("graham2.xsd");
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Graham2 failed " << e << endl;
        return 0;
    }
}


int sdotest::b46613()
{
    try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        mdg->create("rubbish","MyObject");

        return 0;

    }
    catch (SDOTypeNotFoundException e)
    {
        //cout << "b46613 gave the correct exception" << endl;
        return 1;
    }
}

int sdotest::doctest()
{
    try {

        FILE *f = fopen("doctest.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open doctest.dat" << endl;
            return 0;
        }

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("doctest.xsd");
        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("doctest.xml", 
        "companyNS");
        fprintf(f,"RootURI=%s\n",myXMLDocument->getRootElementURI());
        fprintf(f,"RootName=%s\n",myXMLDocument->getRootElementName());
        fclose(f);
        return comparefiles("doctest.dat","doctest.txt");
    }
    catch (SDOTypeNotFoundException e)
    {
        if (!silent)cout << "doctest failed" << endl;
        return 0;
    }

}


int sdotest::b46634()
{
    try {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("company_with_nillable_SN.xsd");
        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("company_with_nillable_SN.xml", 
        "companyNS");

        DataObjectPtr dob = myXMLDocument->getRootDataObject();
        DataObjectList& dol = dob->getList("departments");
        DataObjectPtr dept = dol[0];
        DataObjectList& dol2 = dept->getList("employees");
        DataObjectPtr emp = dol2[0];

        if (!emp->isSet("SN"))
        {
            if (!silent)cout << "B46634 WRONG - isSet should be true" << endl;
            return 0;
        }
        if (!emp->isNull("SN"))
        {
            if (!silent)cout << "B46634 WRONG - isNull should be true" << endl;
            return 0;
        }

        // now write out the nill value and check its still nil
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "b46634_out.xml");
        return comparefiles("b46634_out.xml","b46634_out.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "b46634 failed" << endl;
        return 0;
    }

}

int sdotest::testErrors()
{
    try {

        int i,j;

        FILE *f = fopen("testerrors.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open testerrors.dat" << endl;
            return 0;
        }

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("error1.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            fprintf(f,"XSD reported some errors:\n");
            for (j=0;j<i;j++)
            {
                const char* m = xsh->getErrorMessage(j);
                if (m != 0) fprintf(f,"%s\n", m);
            }
        }
        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("error1.xml", 
        "companyNS");

        if ((i = myXMLHelper->getErrorCount()) > 0)
        {
            fprintf(f,"XML reported some errors:\n");
            for (j=0;j<i;j++)
            {
                const char *m = myXMLHelper->getErrorMessage(j);
                if (m != 0) fprintf(f,"%s\n",m);
            }
        }

        fclose(f);
        return comparefiles("testerrors.dat","testerrors.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "testErrors  failed" << endl << e << endl;
        return 0;
    }

}

int sdotest::b46734()
{
    try {

        int i,j;

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("wsdl.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "WSDL XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
        xsh->defineFile("wsdl-soap.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent) 
            {
                cout << "WSDL SOAP XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char* m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
        xsh->defineFile("wsdl-http.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "WSDL HTTP XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char* m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("StockQuoteService.wsdl"
            );

        if ((i = myXMLHelper->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "XML reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char * m = myXMLHelper->getErrorMessage(j);
                    if (m != 0)cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
        return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "b46734 failed" << endl << e << endl;
        return 0;
    }

}

int sdotest::b46693()
{
    try {

        int i,j;

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("b46693.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "b46693 XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char* m = xsh->getErrorMessage(j);
                    if (m != 0)cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

    //    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    //    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("b46693.xsd"
    //        );

    //    if ((i = myXMLHelper->getErrorCount()) > 0)
    //    {
    //        cout << "b46693 reported some errors:" << endl;
    //        for (j=0;j<i;j++)
    //        {
    //            const char* m = myXMLHelper->getErrorMessage(j);
    //            if (m != 0)cout << m;
    //          cout << endl;
    //        }
    //    }
        return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "b46693 failed" << endl << e << endl;
        return 0;
    }

}

int sdotest::clonetest()
{
    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    mdg->addType("companyNS","CompanyType");
    mdg->addType("companyNS","DepartmentType");
    mdg->addType("companyNS","EmployeeType");

   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    mdg->addPropertyToType(tcomp,"name",tstring);
    mdg->addPropertyToType(tcomp,"departments",tdept,true,false,true);
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp,false,false,false);
    mdg->addPropertyToType(tcomp,"CEO",temp,false,false,true);

    mdg->addPropertyToType(tdept,"employees",temp,true,false,true);
    mdg->addPropertyToType(tdept,"name",tstring,false,false,true);
    mdg->addPropertyToType(tdept,"location",tstring,false,false,true);
    mdg->addPropertyToType(tdept,"number",tstring,false,false,true);

    mdg->addPropertyToType(temp,"name",tstring,false,false,true);
    mdg->addPropertyToType(temp,"SN",tstring,false,false,true);
    mdg->addPropertyToType(temp,"manager",tbool,false,false,true);


    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","The Company");

    DataObjectPtr dept = mdg->create((Type&)tdept);

    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","The Department");
    dept->setCString("location","Botley");
    dept->setCString("number","123");


    // now clone the data factory, and add more types

    DataFactoryPtr df2 = mdg->clone();

    df2->addType("otherNS","DataSet");

    df2->addPropertyToType("companyNS","DepartmentType","dataset",
        "otherNS","DataSet", false, false, true);

    const Type& tstring2  = df2->getType("commonj.sdo","String");
    const Type& tbool2    = df2->getType("commonj.sdo","Boolean");
    const Type& tcs2      = df2->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp2   = df2->getType("companyNS","CompanyType");
    const Type& tdept2    = df2->getType("companyNS","DepartmentType");
    const Type& temp2     = df2->getType("companyNS","EmployeeType");

    DataObjectPtr comp2 = df2->create((Type&)tcomp2);
    comp->setCString("name","The Company");

    DataObjectPtr dept2 = df2->create((Type&)tdept2);

    DataObjectList& dol2 = comp2->getList("departments");
    dol2.append(dept2);

    dept2->setCString("name","The Other Department");
    dept2->setCString("location","Not Botley");
    dept2->setCString("number","321");

    return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "clone failed " << e << endl;
        return 0;
    }


}

int sdotest::testUtil()
{
    try {


    DataFactoryPtr mdg  = DataFactory::getDataFactory();


    mdg->addType("companyNS","CompanyType");
    // employee will be an open type...
    mdg->addType("companyNS","EmployeeType", /*seq*/false, 
                                             /*open*/true, 
                                             /*abs */ false, 
                                             /*data*/ false);

    mdg->addType("companyNS","DepartmentType");

    mdg->addType("companyNS","OpenType");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");
    const Type& topen    = mdg->getType("companyNS","OpenType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);
    // not containment...
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp, false,false,false);
    mdg->addPropertyToType(tcomp,"departments",tdept, true);
    mdg->addPropertyToType(tcomp,"cs",tcs);

    mdg->addPropertyToType(tdept,"name",tstring);
    mdg->addPropertyToType(tdept,"location",tstring);
    mdg->addPropertyToType(tdept,"number",tstring);
    mdg->addPropertyToType(tdept,"employees",temp,true);
    
    mdg->addPropertyToType(temp, "name",tstring);
    mdg->addPropertyToType(temp, "SN",  tstring);
    mdg->addPropertyToType(temp, "manager",  tbool);

    mdg->addPropertyToType(topen,"name",tstring);

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","ACME");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","Advanced Technologies");
    dept->setCString("location","NY");
    dept->setCString("number","123");

    DataObjectList& emps = dept->getList("employees");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);

    emps.append(emp1);
    emps.append(emp2);
    emps.append(emp3);

    emp1->setCString("name","Albert");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Boris");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Carl");
    emp3->setCString("SN","E0003");

    emp1->setCString("openstring","Value Of Open String");
    emp1->setBoolean("openboolean",true);

    const char* c = emp1->getCString("openstring");

    bool b = emp1->getBoolean("openboolean");

    // unknown list type at present..
    DataObjectList& dl = emp1->getList("openintlist");
    
    int i = 45;
    dl.append((short)i); // now the list must be primitive and short!!

    // unknown list type at present..
    DataObjectList& dl2 = emp1->getList("opendataobjectlist");
    
    DataObjectPtr myopen = mdg->create("companyNS","OpenType");
    myopen->setCString("name","MyOpenName");
    dl2.append(myopen); // now the list must be of data objects
    
    emp1->setDataObject("opendataobject", myopen);
    
    filebuf fb;
    fb.open ("testutils.dat",ios::out);
    ostream os(&fb);
    SDOUtils::printDataObject(os, comp);
    fb.close();
    return comparefiles("testutils.dat","testutils.txt");
    
 
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "ERROR in UTILS test" << e << endl;
        return 0;
    }

}

int sdotest::printset(FILE *f, ChangeSummaryPtr cs)
{
    ChangedDataObjectList& cdl = cs->getChangedDataObjects();

    // here there should be a creation of Ed, and a change to dept, which has a
    // bool previously unset, and a int list previuously unset and an employees list
    // previously unset
    
    for (int i=0;i< cdl.size();i++)
    {
        if (cs->isModified(cdl[i]))
        {

            fprintf(f,"A modified of type %s#%s\n",cdl[i]->getType().getURI()  
                 ,cdl[i]->getType().getName());

            SettingList& sl = cs->getOldValues(cdl[i]);
            if (sl.size() == 0) 
            {
                fprintf(f,"No Settings found\n");
            }
            else 
            {
                for (int j=0;j< sl.size(); j++)
                {
                    fprintf(f,"Property %s",sl[j].getProperty().getName());
                    // this could be a many-valued property, and could be one which is
                    // a data object or a data type...
                    if (sl[j].getProperty().isMany()) 
                    {
                        fprintf(f,"[%s]",sl[j].getIndex());
                    }
                    if (!sl[j].isSet())
                    {
                        fprintf(f," was unset before the change\n");
                    }
                    else
                    {
                        fprintf(f," was set before the change !!\n");
                    }
                }
            }
        }
    }
    return 1;
}
    

int sdotest::b46633()
{
    try {

        FILE *f = fopen("b46633.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open b46633.dat" << endl;
            return 0;
        }

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");
        mdg->addPropertyToType("myspace","Root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

        mdg->addType("myspace","Company");
        mdg->addType("myspace","Department");
        mdg->addType("myspace","Manager");
        mdg->addType("myspace","Employee");


        mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    

        mdg->addPropertyToType("myspace","Root","company",
                           "myspace","Company", false, false, true);

        mdg->addPropertyToType("myspace","Company","departments",
                           "myspace","Department", true, false, true);


        mdg->addPropertyToType("myspace","Department","name",
                           "commonj.sdo","String", false, false, false);

        mdg->addPropertyToType("myspace","Department","ints",
                           "commonj.sdo","Integer", true, false, false);


        mdg->addPropertyToType("myspace","Department","bool",
                           "commonj.sdo","Boolean", false, false, false);

        mdg->addPropertyToType("myspace","Department","manager",
                           "myspace","Manager", false, false, true);

        mdg->addPropertyToType("myspace","Department","employees",
                           "myspace","Employee", true, false, true);

        mdg->addPropertyToType("myspace","Manager","name",
                           "commonj.sdo","String", false, false, false);
    
        mdg->addPropertyToType("myspace","Employee","name",
                            "commonj.sdo","String", false, false, false);

        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    
        ChangeSummaryPtr cs = dor->getChangeSummary();

        DataObjectPtr com = dor->createDataObject("company");
        com->setCString("name","IBM");

        DataObjectPtr dept = com->createDataObject("departments");
        dept->setCString("name","JTC");

        // employees initally unset

        cs->beginLogging();

        DataObjectPtr emp = dept->createDataObject("employees");
        emp->setCString("name","Ed");



        DataObjectList& dl = dept->getList("ints");
        dl.append((long)400);
    
        dept->setBoolean("bool", true);

        printset(f,cs);

        cs->endLogging();

        // unset them all , restart logging, and get the same results
    
        dept->unset("bool");
        dept->unset("ints");
        dept->unset("employees");

        cs->beginLogging();

        DataObjectPtr emp2 = dept->createDataObject("employees");
        emp2->setCString("name","Ed2");

        dl.append((long)600);
    
        dept->setBoolean("bool", true);

        printset(f, cs);

        cs->endLogging();

        fclose(f);

        return comparefiles("b46633.dat","b46633.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "b46633 failed " << e << endl;
        return 0;
    }

}



int sdotest::testLoad()
{
    try 
    {
        int i,j;
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        FILE* f = fopen("company_with_nillable_SN.xsd","r+");
        char* buffer = new char[4000];
        j = 0;
        while ((i = fgetc(f)) != EOF)
        {
            buffer[j++] = (char)i;
        }
        buffer[j] = 0;
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->define(buffer);

        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent) {
                cout << "XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        FILE* f2 = fopen("company_with_nillable_SN.xml","r+");
        j = 0;
        while ((i = fgetc(f2)) != EOF)
        {
            buffer[j++] = (char)i;
        }
        buffer[j] = 0;

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->load(buffer);
        return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in testDAS " << e << endl;
        return 0;
    }
}

int sdotest::includetest()
{
    try 
    {
        int i,j;
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("include.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "INCLUDE XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        xsh->defineFile("../test/includeother.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "INCLUDE XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in include test" << e << endl;
        return 0;
    }
}

int sdotest::detachtest()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");

        mdg->addType("myspace","Company");
        mdg->addType("myspace","Department");
        mdg->addType("myspace","Manager");
        mdg->addType("myspace","Employee");


        mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    

        mdg->addPropertyToType("myspace","Root","company",
                           "myspace","Company", false, false, true);

        mdg->addPropertyToType("myspace","Company","departments",
                           "myspace","Department", true, false, true);


        mdg->addPropertyToType("myspace","Department","name",
                           "commonj.sdo","String", false, false, false);


        mdg->addPropertyToType("myspace","Department","manager",
                           "myspace","Manager", false, false, true);

        mdg->addPropertyToType("myspace","Department","employees",
                           "myspace","Employee", true, false, true);

        mdg->addPropertyToType("myspace","Manager","name",
                           "commonj.sdo","String", false, false, false);
    
        mdg->addPropertyToType("myspace","Employee","name",
                            "commonj.sdo","String", false, false, false);

        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    
        DataObjectPtr com = dor->createDataObject("company");
        com->setCString("name","IBM");

        DataObjectPtr dept = com->createDataObject("departments");

        dept->setCString("name","JTC");


        DataObjectPtr emp = dept->createDataObject("employees");
        emp->setCString("name","Ed");

        DataObjectPtr emp2 = dept->createDataObject("employees");
        emp2->setCString("name","Tom");
        
        DataObjectList& dol = dept->getList("employees");

        DataObjectPtr e = dol[0];
        e->detach();

        DataObjectPtr cont = emp->getContainer();

        if (cont != 0)
        {
            if (!silent)cout << "EMP STILL ATTACHED - problem" << endl;
            return 0;
        }

        DataObjectPtr cont2 = dol[0]->getContainer();

        if (cont2 == 0)
        {
            if (!silent)cout << "SECOND DETACHED - problem" << endl;
            return 0;
        }

        return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "detach failed " << e << endl;
        return 0;
    }

}


int sdotest::leak()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");


        mdg->addPropertyToType("myspace","Root","ints",
                           "commonj.sdo","Integer", true, false, false);


        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    

        if (dor->isSet("ints")) return 0;

        DataObjectList& dl = dor->getList("ints");
        
        if (dor->isSet("ints")) return 0;

        dl.append((long)400);
    
        if (!dor->isSet("ints")) return 0;

        dl.remove(0);
    
        if (dor->isSet("ints")) return 0;

        dl.append((long)400);

        if (!dor->isSet("ints")) return 0;

        dor->unset("ints");

        if (dor->isSet("ints")) return 0;

        dl.append((long)600);
    
        if (!dor->isSet("ints")) return 0;

        return 1;


    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "leak failed " << e << endl;
        return 0;
    }

}

int sdotest::twolists()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");

        mdg->addPropertyToType("myspace","Root","ints",
                           "commonj.sdo","Integer", true, false, false);

        mdg->addPropertyToType("myspace","Root","ints2",
                           "commonj.sdo","Integer", true, false, false);


        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    
        DataObjectList& dl2 = dor->getList("ints2");
        dl2.append((long)400);


        DataObjectList& dl = dor->getList("ints");
        dl.append((long)400);
        return 1;
        

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "twolists failed " << e << endl;
        return 0;
    }

}


int sdotest::graham4()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");
        mdg->addType("myspace","ListItem");


        mdg->addPropertyToType("myspace","ListItem","name",
                           "commonj.sdo","String", false, false, false);
    

        mdg->addPropertyToType("myspace","Root","ints",
                           "commonj.sdo","Integer", true, false, false);

        mdg->addPropertyToType("myspace","Root","items",
                           "myspace","ListItem", true, false, true);


        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    

        // first try with no elements in lists

        try {
            DataObjectPtr pitem = dor->getDataObject("items[name=\"hello\"]");
            return 0;
        }
        catch (SDOIndexOutOfRangeException e)
        {
            //cout << "Normal exception in Graham4" << endl;
        }

        try {
            int inty = dor->getInteger("ints[1]");
            return 0;
        }
        catch (SDOIndexOutOfRangeException e)
        {
            //cout << "Normal exception in Graham4" << endl;
        }

        // now populate them

        DataObjectPtr myitem = dor->createDataObject("items");
        myitem->setCString("name","not hello");

        try {
            DataObjectPtr pitem2 = dor->getDataObject("items[name=\"hello\"]");
            return 0;
        }
        catch (SDOIndexOutOfRangeException e)
        {
            //cout << "Normal exception in Graham4" << endl;
        }

        DataObjectList& dl = dor->getList("ints");
        dl.append((long)100);

        try {
            int inty2 = dor->getInteger("ints[2]");
            return 0;
        }
        catch (SDOIndexOutOfRangeException e)
        {
            //cout << "Normal exception in Graham4" << endl;
        }

        try 
        {
            DataObjectPtr pitem2 = dor->getDataObject("items[name=\"not hello\"]");
            if (!silent)cout << "item name is " << pitem2->getCString("name") << endl;
        }
        catch (SDORuntimeException e)
        {
            if (!silent)cout << "ABNORMAL exception in Graham4" << endl;
            return 0;
        }

        try {
            int inty2 = dor->getInteger("ints[1]");
            //cout << "Integer is " << inty2 << endl;
            return 1;
        }
        catch (SDOIndexOutOfRangeException e)
        {

            if (!silent)cout << "ABNORMAL exception in Graham4" << endl;
            return 0;
        }

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "twolists failed " << e << endl;
        return 0;
    }

}


int sdotest::graham5()
{
    try 
    {
        int i,j;
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("Order.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "INCLUDE XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
        return 1;
    }
    catch (SDORuntimeException r)
    {
        if (!silent)cout << "Graham5 failed " << r << endl;
        return 0;
    }
}

int sdotest::graham6()
{
    try 
    {
        int i,j;
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("g/Order.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "INCLUDE XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr doc = xmh->loadFile("g/order.xml", "orderNS");

        char* str = xmh->save(doc);
        FILE* f = fopen("g/string-output.xml","w+");
        if (f != 0)
        {
            fputs(str,f);
            fclose(f);
        }

        xmh->save(doc,"g/graham6-output.xml");

        // now get the root and try that 
        DataObjectPtr dob = doc->getRootDataObject();
        char* str2 = xmh->save(dob,"orderNS","order");
        FILE* f2 = fopen("g/string-output-2.xml","w+");
        if (f2 != 0)
        {
            fputs(str2,f2);
            fclose(f2);
        }

        xmh->save(dob,"orderNS","order", "g/graham6-output-2.xml");

        if (!comparefiles("g/string-output.xml","g/string-output.txt"))return 0;
        if (!comparefiles("g/graham6-output.xml","g/graham6-output.txt")) return 0;
        if (!comparefiles("g/string-output-2.xml","g/string-output-2.txt")) return 0;
        return 1;

    }
    catch (SDORuntimeException r)
    {
        if (!silent)cout << "Graham6 failed " << r << endl;
        return 0;
    }
}

int sdotest::b47137()
{

    try 
    {
        FILE *f = fopen("b47137.dat","w+");
        if (f == 0)
        {
            if (!silent)cout <<  "Unable to open b47137.dat" << endl;
        }

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("b47137.xsd");

        DataObjectPtr dob = mdg->create("","guid");

        const Type& t = dob->getType();

        PropertyList pl = t.getProperties();

        for (int i=0;i<pl.size();i++)
        {
            fprintf(f,"Property:%s\n", pl[i].getName());
        }

        dob->setCString("value","Hello");
        dob->setBoolean("isPermaLink",true);

        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);

        xmh->save(dob,"","fluid","b47137_out.xml");

        // now try to read back the file ..

        XMLDocumentPtr doc = xmh->loadFile("b47137_out.xml", "");

        DataObjectPtr dob2 = doc->getRootDataObject();

        const Type& t2 = dob2->getType();

        PropertyList pl2 = t2.getProperties();

        for (int j=0;j<pl2.size();j++)
        {
            fprintf(f,"Property:%s\n",pl2[j].getName());
            fprintf(f, "Value:%s\n", dob2->getCString(pl2[j]));
        }

        fclose(f);
        return comparefiles("b47137.dat","b47137.txt");
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "B47137 failed " << e << endl;
        return 0;
    }
}

int sdotest::b47137b()
{
    int i,j;
    try 
    {

        FILE *f = fopen("b47137b.dat","w+");
        if (f == 0)
        {
            if (!silent) cout << "Unable to open b47137b.dat" << endl;
            return 0;
        }

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("b47137b.xsd");

        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "INCLUDE XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        TypeList tl = mdg->getTypes();
        for (int k=0;k<tl.size();k++)
        {
            fprintf(f,"Type:%s#%s\n",tl[k].getURI(),tl[k].getName());
        }

        DataObjectPtr dob = mdg->create("companyNS","CompanyType");

        const Type& t = dob->getType();

        PropertyList pl = t.getProperties();

        for (int i=0;i<pl.size();i++)
        {
            fprintf(f, "Property:%s\n",pl[i].getName());
        }

        DataObjectPtr dobguid = dob->createDataObject("guid");

        dobguid->setCString("value","Wilbur");
        dobguid->setBoolean("isPermaLink",true);

        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);

        xmh->save(dob,"","company","b47137b_out.xml");

        // now try to read back the file ..

        XMLDocumentPtr doc = xmh->loadFile("b47137b_out.xml", "");

        DataObjectPtr dob2 = doc->getRootDataObject();

        const Type& t2 = dob2->getType();

        PropertyList pl2 = t2.getProperties();

        for (int j=0;j<pl2.size();j++)
        {

            fprintf(f,"Property:%s\n",pl2[j].getName());
            if (!strcmp(pl2[j].getName(),"guid"))
            {
                DataObjectPtr dobguid2 = dob2->getDataObject("guid");
                if (dobguid2 != 0)
                {
                    fprintf(f,"guid Value:%s\n",dobguid2->getCString("value"));
                    fprintf(f,"guid isPermaLink:",dobguid2->getCString("isPermaLink"));
                }
                else
                {
                    fprintf(f,"GUID is empty\n");
                }
            }
        }
        fclose(f);
        return comparefiles("b47137b.dat","b47137b.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "B47137b failed " << e << endl;
        return 0;
    }
}

int sdotest::b47293()
{
    int i,j,k;
    try 
    {
        FILE *f = fopen("b47293.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open b47293.dat" << endl;
            return 0;
        }

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("47293Catalog.xsd");

        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent) 
            {
                cout << "47293 XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                 const char *m = xsh->getErrorMessage(j);
                     if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        TypeList tl = mdg->getTypes();
        for ( k=0;k<tl.size();k++)
        {
            fprintf(f,"Type:%s#%s",tl[k].getURI(),tl[k].getName());
        }

        xsh->generateFile(tl,"47293Catalog-out.xsd","catalogNS",0);

        DataFactoryPtr mdg2  = DataFactory::getDataFactory();

        XSDHelperPtr xsh2 = HelperProvider::getXSDHelper(mdg2);
        
        xsh2->defineFile("47293Catalog-out.xsd");

        if ((i = xsh2->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "47293-out XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh2->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        TypeList tl2 = mdg2->getTypes();
        for (k=0;k<tl2.size();k++)
        {
            fprintf(f,"Type:%s#%s\n",tl2[k].getURI(),tl2[k].getName());
        }
        fclose(f);
        return comparefiles("b47293.dat","b47293.txt");
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "B47137b failed " << e << endl;
        return 0;
    }
}


int sdotest::b47802()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");
        mdg->addType("myspace","ListItem");

        mdg->addPropertyToType("myspace","Root","ints",
                           "commonj.sdo","Integer", true, false, false);

        mdg->addPropertyToType("myspace","Root","items",
                           "myspace","ListItem", true, false, true);


        const Type& tcc = mdg->getType("myspace","Root");
        const Type& tcl = mdg->getType("myspace","ListItem");

        DataObjectPtr dor = mdg->create((Type&)tcc);

        DataObjectList& dol1 = dor->getList("ints");

        DataObjectList& dol2 = dor->getList("items");
    

        dol1.append((long)34);
        dol1.append((long)56);

        DataObjectPtr li1 = mdg->create(tcl);
        DataObjectPtr li2 = mdg->create(tcl);

        dol2.append(li1);
        dol2.append(li2);

        dor->unset("ints[1]"); // should work
        dor->unset("items.0"); // should work

        try {

            dor->unset("ints[2]"); // should  not work
            return 0;
        }
        catch (SDOIndexOutOfRangeException e)
        {
            //cout << "Normal index oor in 47802" << endl;
        }

        try {
            dor->unset("items.1"); // should not work
            return 0;
        }    
        catch (SDOIndexOutOfRangeException e)
        {
            // cout << "Normal index oor in 47802" << endl;
        }
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Problem in b47802" << e << endl;
        return 0;
    }
}


int sdotest::b47663()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        DataFactoryPtr mdg2  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");
        mdg->addType("myspace","ListItem");
        mdg->addType("myspace","Item");

        mdg->addPropertyToType("myspace","Root","itemref",
                           "myspace","Item", false, false, false);

        mdg->addPropertyToType("myspace","Root","itemcont",
                           "myspace","Item", false, false, true);

        mdg->addPropertyToType("myspace","Root","items",
                           "myspace","ListItem", true, false, true);

        mdg2->addType("myspace","Root");
        mdg2->addType("myspace","ListItem");
        mdg2->addType("myspace","Item");

        mdg2->addPropertyToType("myspace","Root","itemref",
                           "myspace","Item", false, false, false);

        mdg2->addPropertyToType("myspace","Root","itemcont",
                           "myspace","Item", false, false, true);

        mdg2->addPropertyToType("myspace","Root","items",
                           "myspace","ListItem", true, false, true);



        const Type& tcc = mdg->getType("myspace","Root");
        const Type& tcl = mdg->getType("myspace","ListItem");
        const Type& tci = mdg->getType("myspace","Item");

        DataObjectPtr dor = mdg->create((Type&)tcc);
        DataObjectPtr it1 = mdg->create((Type&)tci);
        DataObjectList& dol1 = dor->getList("items");
        DataObjectPtr li11 = mdg->create(tcl);
        DataObjectPtr li12 = mdg->create(tcl);

        
        const Type& tcc2 = mdg2->getType("myspace","Root");
        const Type& tcl2 = mdg2->getType("myspace","ListItem");
        const Type& tci2 = mdg2->getType("myspace","Item");
        DataObjectPtr dor2 = mdg2->create((Type&)tcc2);
        DataObjectPtr it2 = mdg2->create((Type&)tci2);
        DataObjectList& dol2 = dor2->getList("items");
        DataObjectPtr li21 = mdg2->create(tcl2);
        DataObjectPtr li22 = mdg2->create(tcl2);


        try 
        {
            // append from wrong factory to list
            dol1.append(li21);
            //cout << "ERROR - 47764 should not append from wrong factory" << endl;
            //cout << "Updated test case: Correctly used data object from compatible factory" <<
            //    endl;
        }
        catch (SDOInvalidConversionException e)
        {
            if (!silent)cout << "ERROR: Incorrect invalid conversion exception" << e << endl;
            return 0;
        }

        try 
        {
            // add object from wrong factory to reference.
            dor2->setDataObject("itemref",it1);
            return 0;
        }
        catch (SDOUnsupportedOperationException e)
        {
            // expected ,as a reference cannot be outside the graph
        }

        try 
        {
            // add object from wrong factory to reference.
            dor2->setDataObject("itemcont",it1);
        }
        catch (SDORuntimeException e)
        {
            if (!silent)cout << "Add contained object from second factory not OK" << endl;
            return 0;
        }


        dol2.append(li22); // OK

        try 
        {
            dol2.insert(0,li11);
            //cout << "Updated test case: Correctly used data object from compatible factory" <<
            //    endl;
            //cout << "ERROR - 47764 should not insert from wrong factory" << endl;
        }
        catch (SDOInvalidConversionException e)
        {
            if (!silent)cout << "Incorrect invalid conversion exception" << e << endl;
            return 0;
        }

       try 
        {
            dol2.setDataObject(0,li12);
            //cout << "Updated test case: Correctly used data object from compatible factory" <<
            //    endl;
            // cout << "ERROR - 47764 should not set in list from wrong factory" << endl;
        }
        catch (SDOInvalidConversionException e)
        {
            if (!silent)cout << "Incorrect invalid conversion exception" << e << endl;
            return 0;
        }
        return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Problem in b47663" << e << endl;
        return 0;
    }
}


int sdotest::bunique()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("test","Root");
        mdg->addType("test","Duplicate");
        mdg->addType("nottest","Duplicate");
        mdg->addType("test","Unique");
        mdg->addType("test","Object");

        mdg->addPropertyToType("test","Root","duplicate",
                           "test","Duplicate", false, false, true);

        mdg->addPropertyToType("test","Root","duplicate2",
                           "nottest","Duplicate", false, false, true);
 
        mdg->addPropertyToType("test","Root","unique",
                            "test","Unique", false, false, true);

        mdg->addPropertyToType("test","Root","duplicatelist",
                           "test","Duplicate", true, false, true);

        mdg->addPropertyToType("test","Root","duplicatelist2",
                           "nottest","Duplicate", true, false, true);

        mdg->addPropertyToType("test","Root","uniquelist",
                           "test","Unique", true, false, true);

        mdg->addPropertyToType("test","Duplicate","id",
                           "commonj.sdo","Integer", false, false, false);
 
        mdg->addPropertyToType("nottest","Duplicate","id",
                           "commonj.sdo","Integer", false, false, false);
 
        mdg->addPropertyToType("test","Unique","id",
                           "commonj.sdo","Integer", false, false, false);

        mdg->addPropertyToType("test","Duplicate","ob",
                           "test","Object", false, false, true);
 
        mdg->addPropertyToType("nottest","Duplicate","ob",
                           "test","Object", false, false, true);
 
        mdg->addPropertyToType("test","Unique","ob",
                           "test","Object", false, false, true);

        const Type& troot = mdg->getType("test","Root");
        const Type& tdup1 = mdg->getType("test","Duplicate");
        const Type& tdup2 = mdg->getType("nottest","Duplicate");
        const Type& tuniq = mdg->getType("test","Unique");
        const Type& tob   = mdg->getType("test","Object");

        DataObjectPtr rooty = mdg->create(troot);

        DataObjectPtr dup1 = mdg->create(tdup1);
        dup1->setInteger("id",1);
        DataObjectPtr dup2 = mdg->create(tdup1);
        dup2->setInteger("id",2);
        DataObjectPtr dup3 = mdg->create(tdup2);
        dup3->setInteger("id",3);
        DataObjectPtr dup4 = mdg->create(tdup2);
        dup4->setInteger("id",4);
        DataObjectPtr uni1 = mdg->create(tuniq);
        uni1->setInteger("id",5);
        DataObjectPtr uni2 = mdg->create(tuniq);
        uni2->setInteger("id",6);

        DataObjectPtr ob1 = mdg->create(tob);
        dup1->setDataObject("ob",ob1);
        DataObjectPtr ob2 = mdg->create(tob);
        dup2->setDataObject("ob",ob2);
        DataObjectPtr ob3 = mdg->create(tob);
        uni1->setDataObject("ob",ob3);
 
        DataObjectList& dl1 = rooty->getList("duplicatelist");
        DataObjectList& dl2 = rooty->getList("duplicatelist2");
        DataObjectList& dl3 = rooty->getList("uniquelist");

        rooty->setDataObject("duplicate",dup1);
        dl1.append(dup2);

        rooty->setDataObject("duplicate2",dup3);
        dl2.append(dup4);
        
        rooty->setDataObject("unique",uni1);

        dl3.append(uni2);

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        TypeList tl = mdg->getTypes();

        xsh->generateFile(tl,"bunique-out.xsd","test");

        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);

        xmh->save(rooty,"","rooty","bunique-out.xml");

        if (!comparefiles("bunique-out.xsd","bunique-out.txt")) return 0;
        if (!comparefiles("bunique-out.xml","bunique-outxml.txt")) return 0;
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Problem in bunique" << e << endl;
        return 0;
    }
}

int sdotest::buniqueread()
{
    try {

        int i,j,k;

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("bunique-out.xsd");

        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "47293 XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
                return 0;
            }
        }

        TypeList tl = mdg->getTypes();
        for ( k=0;k<tl.size();k++)
        {
            //cout << "Type:" << tl[k].getURI() << "#" << tl[k].getName() << endl;
        }


        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);

        XMLDocumentPtr doc = xmh->loadFile("bunique-out.xml", "rooty");

        DataObjectPtr rooty = doc->getRootDataObject();
 
        xsh->generateFile(tl,"buniqueread-out.xsd","test");

        xmh->save(rooty,"","rooty","buniqueread-out.xml");

        return comparefiles("buniqueread-out.xml","buniqueread-out.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Problem in buniqueread" << e << endl;
        return 0;
    }
}


int sdotest::testwsdl()
{
    try {

        int i,j;

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("wsdl.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "WSDL XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }

            }
            return 0;
        }
        xsh->defineFile("wsdl-soap.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "WSDL SOAP XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char* m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }

            }
            return 0;
        }
        xsh->defineFile("wsdl-http.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "WSDL HTTP XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char* m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        TypeList tl = mdg->getTypes();

        for (i=0;i<tl.size();i++)
        {
            //cout << "TYPE:" << tl[i].getURI() << "#" << tl[i].getName() << endl;
        }

        DataObjectPtr root = mdg->create("http://schemas.xmlsoap.org/wsdl/","tDefinitions");

        
        DataObjectPtr mess = mdg->create("http://schemas.xmlsoap.org/wsdl/","tMessage");
        mess->setCString("name","testRequest");

        DataObjectPtr part = mdg->create("http://schemas.xmlsoap.org/wsdl/","tPart");
        part->setCString("name","name");
        part->setCString("type","http://www.w3.org/2001/XMLSchema-instance#string");

        DataObjectPtr part2 = mdg->create("http://schemas.xmlsoap.org/wsdl/","tPart");
        part2->setCString("name","name");
        part2->setCString("type","http://I want/this/added/to/the/urls#string");

 
        DataObjectList& dol = root->getList("message");

        dol.append(mess);

        DataObjectList& dol2 = mess->getList("part");

        dol2.append(part);
        dol2.append(part2);
 
        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);

        xmh->save(root,"","definitions","testwsdl.xml");

        return comparefiles("testwsdl.xml","testwsdl.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "testwsdl failed" << endl << e << endl;
        return 0;
    }

}



int sdotest::travel()
{
    try {

        int i,j;

        FILE *f = fopen("travel.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open travel.dat" << endl;
            return 0;
        }

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("TravelBookingSchema.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "WSDL XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
 
        TypeList tl = mdg->getTypes();

        fprintf(f, "*******************************TYPES**********************\n");

        for (i=0;i<tl.size();i++)
        {
            fprintf(f,"TYPE:%s#%s\n",tl[i].getURI(),tl[i].getName());
        }
        fprintf(f, "*******************************END TYPES******************\n");

 
        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
 
        XMLDocumentPtr doc = xmh->loadFile("TravelBookingUsingSchema.xml");

        DataObjectPtr rooty = doc->getRootDataObject();

        fprintf(f, "*******************************DATA**********************\n");
        printDataObject(f, rooty);
        fprintf(f,"***************************END DATA**********************\n");
       
        fclose(f);
        return comparefiles("travel.dat","travel.txt");
 

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "travel failed" << endl << e << endl;
        return 0;
    }

}


int sdotest::oddchars()
{
    try {

        int i,j;

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("OddChars.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "ODDCHARS XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
 
 
        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
 
        XMLDocumentPtr doc = xmh->loadFile("OddChars.xml");

        if ((i = xmh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "OddChars reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xmh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        DataObjectPtr rooty = doc->getRootDataObject();

        FILE *f = fopen ("oddchars.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open oddchars.dat" << endl;
            return 0;
        }
        printDataObject(f, rooty);
 
        fclose(f);
        return comparefiles("oddchars.dat","oddchars.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "oddchars failed" << endl << e << endl;
        return 0;
    }

}

int sdotest::unsetlist()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("test","Root");
        mdg->addType("test","ListItem");

        mdg->addPropertyToType("test","Root","list",
                           "test","ListItem", true, false, true);

        const Type& troot = mdg->getType("test","Root");
        const Type& tlist = mdg->getType("test","ListItem");

        DataObjectPtr rooty = mdg->create(troot);

        DataObjectList& dl = rooty->getList("list");


        DataObjectPtr li1 = mdg->create(tlist);
        DataObjectPtr li2 = mdg->create(tlist);
        DataObjectPtr li3 = mdg->create(tlist);

        
        dl.append(li1);
        dl.append(li2);
        dl.append(li3);

        rooty->unset("list[3]");

        dl.append(li3);

        rooty->unset("list.2");
       
        return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Problem in unsetlist" << e << endl;
        return 0;
    }
}


int sdotest::notns()
{
    try {


        DataFactoryPtr mdg  = DataFactory::getDataFactory();


        mdg->addType("","Root");
        mdg->addType("","ListItem");

        mdg->addPropertyToType("","Root","list",
                           "","ListItem", true, false, true);

        const Type& troot = mdg->getType("","Root");
        const Type& tlist = mdg->getType("","ListItem");

        DataObjectPtr rooty = mdg->create(troot);

        DataObjectList& dl = rooty->getList("list");


        DataObjectPtr li1 = mdg->create(tlist);
        DataObjectPtr li2 = mdg->create(tlist);
        DataObjectPtr li3 = mdg->create(tlist);

        
        dl.append(li1);
        dl.append(li2);
        dl.append(li3);

 
        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);

        xmh->save(rooty,"","root","notns.xml");

        return comparefiles("notns.xml","notns.txt");


    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "notns" << endl << e << endl;
        return 0;
    }

}

int sdotest::badelement()
{
    try {

        int i,j;
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("BadElement.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "PROBLEM: BADELEMENT XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
 
        XMLDocumentPtr doc = xmh->loadFile("BadElement.xml");

        if ((i = xmh->getErrorCount()) > 0)
        {
            FILE *f = fopen("badelement.dat","w+");
            if (f == 0)
            {
                if (!silent)cout << "Unable to open badelement.dat" << endl;
                return 0;
            }
            fprintf(f,"BADELEMENT correctly found errors:\n");
            for (j=0;j<i;j++)
            {
                const char *m = xmh->getErrorMessage(j);
                if (m != 0) fprintf(f,"%s\n", m);
            }
            fclose (f);
            return comparefiles("badelement.dat","badelement.txt");
        }
        else
        {
            if (!silent)cout << "Expected errors, got none" << endl;
            return 0;
        }

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "badelement" << endl << e << endl;
        return 0;
    }

}


int sdotest::testastyle(FILE *f, const char* style)
{

    int i,j;


    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    
    xsh->defineFile(style);

    if ((i = xsh->getErrorCount()) > 0)
    {
        if (!silent)
        {
            cout << "PROBLEM: TESTSTYLES XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        return 0;
    }

    TypeList tl = mdg->getTypes();

    fprintf(f,"*****STYLE %s\n",style);

    for (i=0;i<tl.size();i++)
    {
        fprintf(f,"Type: %s#%s\n", tl[i].getURI(),tl[i].getName());
        if (!strcmp(tl[i].getName(),"library"))
        {
            PropertyList pl = tl[i].getProperties();
            for (int j=0;j<pl.size();j++)
            {
                fprintf(f,"Property:%s\n",pl[j].getName());
            }
        }
    }

    fprintf(f, "*************\n");

    // now try creating elements...

    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
 
    XMLDocumentPtr doc = xmh->createDocument(); // with no name, URI - uses first in schema

    DataObjectPtr root = doc->getRootDataObject();

    fprintf(f,"Root is:%s#%s\n",root->getType().getURI(),
        root->getType().getName());


    XMLDocumentPtr doc1 = xmh->createDocument("library"); // with no URI - uses first in schema

    DataObjectPtr root1 = doc1->getRootDataObject();

    fprintf(f,"Root is:%s#%s\n",root1->getType().getURI(), 
        root1->getType().getName());

    XMLDocumentPtr doc2 = xmh->createDocument("library","libraryNS"); // with URI 

    DataObjectPtr root2 = doc2->getRootDataObject();

    fprintf(f, "Root is:%s#%s\n",root2->getType().getURI(), 
        root2->getType().getName());
    return 1;


}

int sdotest::teststyles()
{
    try 
    {

        FILE *f = fopen("teststyles.dat","w+");
        if (f == 0)
        {
            if (!silent)cout << "Unable to open teststyles.dat" << endl;
            return 0;
        }
        testastyle(f,"Style1.xsd");
        testastyle(f,"Style2.xsd");
        testastyle(f,"Style3.xsd");
        //testastyle("Style4.xsd"); = groups not supported yet.
        fclose(f);
        return comparefiles("teststyles.dat","teststyles.txt");
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "teststyles" << endl << e << endl;
        return 0;
    }

}


int sdotest::testinc2(const char* inc, const char* name1, const char* name2)
{


    int i,j;

    try {

    FILE *f = fopen(name1,"w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open " << name1 << endl;
        return 0;
    }

    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    
    xsh->defineFile(inc);

    if ((i = xsh->getErrorCount()) > 0)
    {
        if (!silent)
        {
            cout << "PROBLEM: TestInc2 XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        return 0;
    }

    TypeList tl = mdg->getTypes();

    //cout << "***** CALCULATOR ***************************************" << endl;

    for (i=0;i<tl.size();i++)
    {
        fprintf(f, "Type:%s#%s\n", tl[i].getURI(),tl[i].getName());
        PropertyList pl = tl[i].getProperties();
        for (int j=0;j<pl.size();j++)
        {
            fprintf(f,"Property:%s\n",pl[j].getName());
        }
    }

    //cout << "*******************************END TYPES******************" << endl;
    fclose(f);
    return comparefiles((char*)name1, (char*)name2);

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in testinc2" << endl;
        return 0;
    }

}



int sdotest::b48300()
{
    return testany("bug48300.xsd",0,"bug48300.xml",0);
}


int sdotest::testOrder(const char* x1, const char* x2, const char* name1,
                       const char* name2)
{

    int i,j;

    try {

    FILE *f = fopen(name1,"w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open " << name1 << endl;
        return 0;
    }

    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    
    xsh->defineFile(x1);

    if ((i = xsh->getErrorCount()) > 0)
    {
        if (!silent)
        {
            cout << "PROBLEM: Order XSD1 reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        fclose(f);
        return 0;
    }

   xsh->defineFile(x2);

    if ((i = xsh->getErrorCount()) > 0)
    {
        if (!silent)
        {
            cout << "PROBLEM: Order XSD2 reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        fclose(f);
        return 0;
    }

    TypeList tl = mdg->getTypes();

    //cout << "***** TESTORDER ****************************************" << endl;

    for (i=0;i<tl.size();i++)
    {
        if (!strcmp(tl[i].getURI(),"commonj.sdo")) continue;
        fprintf(f,"Type:%s#%s\n",tl[i].getURI(),tl[i].getName());
        PropertyList pl = tl[i].getProperties();
        for (int j=0;j<pl.size();j++)
        {
            fprintf(f,"Property:%s\n", pl[j].getName());
        }
    }

    //cout << "*******************************END TYPES******************" << endl;
    
    fclose(f);
    return comparefiles((char*)name1, (char*)name2);

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in TestOrder" << e << endl;
        return 0;
    }
}

int sdotest::simple()
{


    int i,j;

    try {

    FILE *f = fopen("simple.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open simple.dat" << endl;
        return 0;
    }

    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    
    xsh->defineFile("simple.xsd");

    if ((i = xsh->getErrorCount()) > 0)
    {
        if (!silent)
        {
            cout << "PROBLEM: Order simple.xsd reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        return 0;
    }


    TypeList tl = mdg->getTypes();

    //cout << "***** SIMPLE ****************************************" << endl;

    for (i=0;i<tl.size();i++)
    {
        if (!strcmp(tl[i].getURI(),"commonj.sdo")) continue;
        fprintf(f, "Type:%s#%s\n",tl[i].getURI(),tl[i].getName());
        PropertyList pl = tl[i].getProperties();
        for (int j=0;j<pl.size();j++)
        {
            fprintf(f,"Property:%s\n",pl[j].getName());
            fprintf(f, "Type of property:%s\n",pl[j].getType().getName());
            fprintf(f, "IsMany?%d\n",pl[j].isMany());
        }
    }

    //cout << "*******************************SIMPLE******************" << endl;
    
    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
 
    XMLDocumentPtr doc = xmh->loadFile("simple.xml");

    if ((i = xmh->getErrorCount()) > 0)
    {
        if (!silent)
        {
            cout << "Simple found errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xmh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        return 0;
    }

    DataObjectPtr dob = doc->getRootDataObject();

    printDataObject(f, dob);

    fclose(f);
    return comparefiles("simple.dat","simple.txt");

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in TestAny" << e << endl;
        return 0;
    }
}





