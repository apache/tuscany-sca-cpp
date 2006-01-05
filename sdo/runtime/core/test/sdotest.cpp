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

#include <stdio.h>

#pragma warning(disable:4786)

#include <iostream>
using namespace std;

// #include "TypeImpl.h"

#include "sdotest.h"



using namespace commonj::sdo;

// very basic print of a data graph

typedef struct {
    //zend_object         zo;            /* The standard zend_object */
    DataObjectPtr    dop;            /* The Hydra DataObject */
    //zend_object        *df;            /* The Data Factory */
} sdo_doimpl_object;


void  sdotest::rcptest()
{
    DataFactoryPtr mdg;
    sdo_doimpl_object* ptr;

    cout << "RCPtest" << endl;

    mdg  = DataFactory::getDataFactory();
    mdg = 0;
    mdg  = DataFactory::getDataFactory();
    mdg = NULL;    
    mdg  = DataFactory::getDataFactory();

    cout << "RCPtest 2" << endl;

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


    //delete ptr->dop;


    ptr->dop = 0; // null ;

}



void printDataStructure(DataFactory* dd)
{
    TypeList tt = dd->getTypes();
    cout << "Printing Types\n";
    for (int i = 0; i < tt.size(); ++i)
    {
        cout << "Type "  << tt[i].getName() << "\n";
        PropertyList pl = tt[i].getProperties();
        for (int j = 0; j < pl.size() ; j++)
        {
            cout << "Has Property " << pl[j].getName() << 
                " of type ";
            cout << pl[j].getType().getName() << "\n";
        }
    }
}


void sdotest::changesummarytest()
{
    scenario5();
    scenario1();
    scenario2();
    scenario3();
    scenario4();

}




void sdotest::printOldValues(ChangeSummaryPtr cs, DataObjectPtr dol)
{
    cout << "===== Old Property Values Begin=====================================" << endl;
    SettingList& sl = cs->getOldValues(dol);
    if (sl.size() == 0) 
    {
        cout << "No Settings found" << endl;
    }
    else 
    {
        for (int j=0;j< sl.size(); j++)
        {
            cout << "Property " << sl[j].getProperty().getName();
            // this could be a many-valued property, and could be one which is
            // a data object or a data type...
            if (sl[j].getProperty().isMany()) 
            {
                cout << "[" << sl[j].getIndex() << "]" ;
            }
            if (!sl[j].isSet())
            {
                cout << "(UNSET)";
            }
            if (sl[j].isNull())
            {
                cout << "(ISNULL)";
            }

            cout << " of type " ;
            switch (sl[j].getProperty().getTypeEnum())
            {
                case Type::BooleanType:
                    cout << "Boolean:" << sl[j].getBooleanValue();
                break;
                case Type::ByteType:
                    cout << "Byte:" << sl[j].getByteValue();
                break;
                case Type::CharacterType:
                    cout << "Character:" << sl[j].getCharacterValue();
                break;
                case Type::IntegerType: 
                    cout << "Integer:" << sl[j].getIntegerValue();
                break;
                case Type::ShortType:
                    cout << "Short:" << sl[j].getShortValue();
                break;
                case Type::DoubleType:
                    cout << "Double:" << sl[j].getDoubleValue();
                break;
                case Type::FloatType:
                    cout << "Float:" << sl[j].getFloatValue();
                break;
                case Type::LongType:
                    cout << "Long:" << sl[j].getIntegerValue();
                break;
                case Type::DateType:
                    cout << "Date:" << sl[j].getDateValue().getTime();
                break;
                case Type::BigDecimalType: 
                case Type::BigIntegerType: 
                case Type::StringType: 
                case Type::UriType:
                    cout << "String:" << sl[j].getCStringValue();
                    break;
                case Type::BytesType:
                    cout << "Bytes:" << sl[j].getCStringValue();
                break;
                case Type::OtherTypes:
                case Type::DataObjectType:
                case Type::ChangeSummaryType:
                {
                    cout << "DataObject " ;
                    DataObjectPtr dob = sl[j].getDataObjectValue();
                    if (!dob) 
                    {
                        cout << " - object null or unset" ;
                    }
                    else 
                    {
                        DataObjectPtr mydo = sl[j].getDataObjectValue();
                        if (cs->isDeleted(mydo))
                        {
                            cout << " - object deleted " << endl;
                            printOldValues(cs,mydo);
                        }
                        else 
                        {
                            cout << " object still exists " << endl;
                            //printDataObject(mydo);
                        }
                    }
                }
                break;
                default:
                {
                        cout << "Unknown object type";
                }
                break;
            }
            cout << endl;
        }
    }
    cout << "===== Old Property Values End ======================================" << endl;
}


void sdotest::printValue(DataObjectPtr dp, const Property& p)
{
    switch (p.getTypeEnum())
    {
        case Type::BooleanType:
            cout << "boolean:" << dp->getBoolean(p);
        break;
        case Type::ByteType:
            cout << "Byte:" << dp->getByte(p);
        break;
        case Type::CharacterType:
            cout << "character:" << dp->getCharacter(p);
        break;
        case Type::IntegerType: 
            cout << "integer:" << dp->getInteger(p);
        break;
        case Type::ShortType:
            cout << "short:" << dp->getShort(p);
        break;
        case Type::DoubleType:
            cout << "double:" << dp->getDouble(p);
        break;
        case Type::FloatType:
            cout << "float:" << dp->getFloat(p);
        break;
        case Type::LongType:
            cout << "long:" << "cheat"  << dp->getInteger(p);
        break;
        case Type::DateType:
            cout << "date:" << dp->getDate(p).getTime();
        break;
        case Type::BigDecimalType: 
        case Type::BigIntegerType: 
        case Type::StringType: 
        case Type::UriType:
            cout << "string:" << dp->getCString(p);
            break;
        case Type::BytesType:
            cout << "bytes:" << dp->getCString(p);
        break;
        case Type::DataObjectType:
        {
            cout << "dataObject" ;
            DataObjectPtr dob = dp->getDataObject(p);
            if (!dob) 
            {
                cout << " - null or unset" ;
            }
            else 
            {
                cout << endl;
                printDataObject(dob);
            }
        }
        break;
        case Type::OtherTypes:
        case Type::ChangeSummaryType:
        default:
        {
            cout << "Unknown object type";
        }
        break;
    }
    cout << endl;
}

void sdotest::printList(DataObjectPtr dp, const Property& p)
{
    DataObjectList& dobl = dp->getList(p);
    cout << " list ";
    
    if (dobl.size() ==0) {
        cout << "(empty)" << endl;
        return;
    }

    for (int i=0;i<dobl.size();i++) {

    switch (p.getTypeEnum())
    {
        case Type::BooleanType:
            cout << "boolean[" << i << "]=" << dobl.getBoolean(i);
        break;
        case Type::ByteType:
            cout << "byte[" << i << "]=" << dobl.getByte(i);
        break;
        case Type::CharacterType:
            cout << "character[" << i << "]=" << dobl.getCharacter(i);
        break;
        case Type::IntegerType: 
            cout << "integer[" << i << "]=" << dobl.getInteger(i);
        break;
        case Type::ShortType:
            cout << "short[" << i << "]=" << dobl.getShort(i);
        break;
        case Type::DoubleType:
            cout << "double[" << i << "]=" << dobl.getDouble(i);
        break;
        case Type::FloatType:
            cout << "float[" << i << "]=" << dobl.getFloat(i);
        break;
        case Type::LongType:
            cout << "long:" << i << "]=" << "cheat"  << dobl.getInteger(i);
        break;
        case Type::DateType:
            cout << "date[" << i << "]=" << dobl.getDate(i).getTime();
        break;
        case Type::BigDecimalType: 
        case Type::BigIntegerType: 
        case Type::StringType: 
        case Type::UriType:
            cout << "string:" << i << "]=" << dobl.getCString(i);
            break;
        case Type::BytesType:
            cout << "bytes[" << i << "]=" << dobl.getCString(i);
        break;
        case Type::DataObjectType:
        {
            cout << "dataObject["<< i << "]="  ;
            DataObjectPtr dob = dobl[i];
            if (!dob) 
            {
                cout << " null or unset" ;
            }
            else 
            {
                cout << endl;
                printDataObject(dob);
            }
        }
        break;
        case Type::OtherTypes:
        case Type::ChangeSummaryType:
        default:
        {
            cout << "Unknown object type";
        }
        break;
    }
    cout << endl;
    }
}

void sdotest::printDataObject(DataObjectPtr dol)
{
    cout << "DataObject Current Values ==========================================" << endl;
    PropertyList pl = dol->getInstanceProperties();

    for (int j=0;j< pl.size(); j++)
    {
        cout << "Property " << pl[j].getName() << " of type ";
        // this could be a many-valued property, and could be one which is
        // a data object or a data type...
        if (pl[j].isMany()) 
        {
            printList(dol,pl[j]);
        }
        else {
            printValue(dol,pl[j]);
        }
    }
    cout << "End DataObject Current Values ======================================" << endl;
}


void sdotest::dumpchangesummary(ChangeSummaryPtr cs)
{
    ChangedDataObjectList& cdol = cs->getChangedDataObjects();

    // the changed data object list contains only the objects whose properties have
    // been changed. 
    // a changed and subsequently deleted object will not appear, but necessarily its
    // container will appear, so we can rebuild it.

    for (int i=0;i< cdol.size();i++)
    {
        if (cs->isCreated(cdol[i]))
        {
            cout << "Created object in changed list: " << cdol[i] << endl;
            // So its in the created list, it must exist in the tree...
            cout << "The object is " << cdol[i]->getType().getURI()  
                 << "#" << cdol[i]->getType().getName() << endl;
        }
        if (cs->isModified(cdol[i]))
        {
            cout <<"===== Modified Object Starts " << cdol[i] << " ==============================" <<endl;
            // hack to get to a DAS data object...
            DataObject* temp = cdol[i];
            cout << "XPATH:" << ((DataObject*)temp)->objectToXPath() << endl;
            // end hack

            if (cs->isDeleted(cdol[i]))
            {
                cout << "PROBLEM: DELETED OBJECT IN CHANGED LIST: " << cdol[i] << endl;
                // As the item is in the deleted list - its still present..
                cout << "The type is " << cdol[i]->getType().getURI()  
                     << "#" << cdol[i]->getType().getName() << endl;
            }
            else {
                cout << "The modified objects type is " << cdol[i]->getType().getURI()  
                 << "#" << cdol[i]->getType().getName() << endl;

                printOldValues(cs, cdol[i]);
                //printDataObject(cdol[i]);
            }
            cout <<"=====Modified Object Ends " << cdol[i] << " =================================" <<endl;
        }
        if (cs->isDeleted(cdol[i]))
        {
            cout <<"=====Deleted Object Starts " << cdol[i] << " =================================" <<endl;
            printOldValues(cs, cdol[i]);
            cout <<"=====Deleted Object Ends " << cdol[i] << " =================================" <<endl;
        }
    }
}





void sdotest::scenario1()
{
    // scenario 1
    // create root type - root has cs
    // create a company type. and containment ref from root, many valued.
    // props on comp id, name both strings single v.
    // create root object
    // begin logging
    // create a company obj. name=acme id=123

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","RootOfAllEvil");
    mdg->addPropertyToType("myspace","RootOfAllEvil","whatever",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addType("myspace","Company");

    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);

    mdg->addPropertyToType("myspace","RootOfAllEvil","companies",
                           "myspace","Company", true, false, true);

    const Type& tcc = mdg->getType("myspace","RootOfAllEvil");
    DataObjectPtr dor = mdg->create((Type&)tcc);
    
    ChangeSummaryPtr cs = dor->getChangeSummary();
    cs->beginLogging();

    DataObjectPtr com = dor->createDataObject("companies");
    com->setCString("name","acme");
    com->setCString("id","123");

    dumpchangesummary(cs);

    cs->endLogging();

    //expect cdo:
    //root - isChanged
    //comp - isCreated


}


void sdotest::testui()
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
    }

    catch (SDORuntimeException e)
    {
        cout << "Unsigned int failed" << endl << e << endl;
    }
}



void sdotest::scenario5()
{
    // scenario 1
    // create root type - root has cs
    // create a company type. and containment ref from root, many valued.
    // props on comp id, name both strings single v.
    // create root object
    // begin logging
    // create a company obj. name=acme id=123

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","RootOfAllEvil");
    mdg->addPropertyToType("myspace","RootOfAllEvil","whatever",
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

    mdg->addPropertyToType("myspace","RootOfAllEvil","companies",
                           "myspace","Company", true, false, true);

    const Type& tcc = mdg->getType("myspace","RootOfAllEvil");
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

    dumpchangesummary(cs);

    cs->endLogging();

    //expect com to have change record, nothing for eotm:

}

void sdotest::scenario2()
{
    // create root type - root has cs
    // create a company type. and containment ref from root, many valued.
    // 2 props on comp id, name both strings single v.
    // create root object
    // create a company obj. name=acme id=123
    // begin logging()
    // name="megacorp"

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","RootOfAllEvil");
    mdg->addPropertyToType("myspace","RootOfAllEvil","whatever",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addType("myspace","Company");
    
    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);

    mdg->addPropertyToType("myspace","RootOfAllEvil","companies",
                           "myspace","Company", true, false, true);

    const Type& tcc = mdg->getType("myspace","RootOfAllEvil");
    DataObjectPtr dor = mdg->create((Type&)tcc);

    DataObjectPtr com = dor->createDataObject("companies");
    com->setCString("name","acme");
    com->setCString("id","123");
    
    ChangeSummaryPtr cs = dor->getChangeSummary();
    cs->beginLogging();

    com->setCString("name","megacorp");

    dumpchangesummary(cs);

    cs->endLogging();

    //expect cdo:
    //root- nothing
    // company- isChanged
    //setting prop=name, value="acme"

}

void sdotest::scenario3()
{

    //create root type - root has cs
    //create a company type. and containment ref from root, many valued.
    //2 props on comp id, name both strings single v.
    //create root object
    //create a company obj. name=acme id=123
    //begin logging()
    //delete company

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
    mdg->addType("myspace","RootOfAllEvil");
    mdg->addPropertyToType("myspace","RootOfAllEvil","whatever",
                           "commonj.sdo","ChangeSummary", false, false, false);

    mdg->addType("myspace","Company");

    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","id",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","RootOfAllEvil","companies",
                           "myspace","Company", true, false, true);

    const Type& tcc = mdg->getType("myspace","RootOfAllEvil");
    DataObjectPtr dor = mdg->create((Type&)tcc);

    DataObjectPtr com = dor->createDataObject("companies");
    com->setCString("name","acme");
    com->setCString("id","123");
    
    ChangeSummaryPtr cs = dor->getChangeSummary();
    cs->beginLogging();

    com->detach();

    dumpchangesummary(cs);

    cs->endLogging();

    // expect cdo
    // root - changed
    // company - deleted

}

void sdotest::scenario4()
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
    
    mdg->addType("myspace","RootOfAllEvil");
    mdg->addPropertyToType("myspace","RootOfAllEvil","whatever",
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

    mdg->addPropertyToType("myspace","RootOfAllEvil","companies",
                           "myspace","Company", true, false, true);

    mdg->addPropertyToType("myspace","Company","departments",
                           "myspace","Department", true, false, true);

    mdg->addPropertyToType("myspace","Department","employees",
                           "myspace","Employee", true, false, true);

    const Type& tcc = mdg->getType("myspace","RootOfAllEvil");
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
 
    cout << "-----------------------------------" << endl;
    cout << com->getCString("name") << endl;
    DataObjectList& deps = com->getList("departments");
    cout << "Depts[0]:" << deps[0]->getCString("name") << endl;
    DataObjectList& emps = deps[0]->getList("employees");
    cout << "Emps[0]:" << emps[0]->getCString("name") << endl;
    cout << "Emps[1]:" << emps[1]->getCString("name") << endl;
    cout << "Depts[1]:" << deps[1]->getCString("name") << endl;
    DataObjectList& emps2 = deps[1]->getList("employees");
    cout << "Emps[0]:" << emps2[0]->getCString("name") << endl;
    cout << "Emps[1]:" << emps2[1]->getCString("name") << endl;
    cout << "-----------------------------------" << endl;
    cout << com2->getCString("name") << endl;
    DataObjectList& deps2 = com2->getList("departments");
    cout << "Depts[0]:" << deps2[0]->getCString("name") << endl;
    DataObjectList& emps3 = deps2[0]->getList("employees");
    cout << "Emps[0]:" << emps3[0]->getCString("name") << endl;
    cout << "Emps[1]:" << emps3[1]->getCString("name") << endl;
    cout << "Depts[1]:" << deps2[1]->getCString("name") << endl;
    DataObjectList& emps4 = deps2[1]->getList("employees");
    cout << "Emps[0]:" << emps4[0]->getCString("name") << endl;
    cout << "Emps[1]:" << emps4[1]->getCString("name") << endl;
    cout << "-----------------------------------" << endl;

    // should be able to get the change summary property, but it
    // should be zero. 
    // should be able to find out if the type is a change summary type

    if (dor->getType().isChangeSummaryType())
    {
        cout << "The root object has a change summary" << endl;
    }
    else {
        cout << "The root object DOESNT HAVE a change summary !!!" << endl;
    }

    try {
        DataObjectPtr csptr = dor->getDataObject("whatever");

        if (!csptr) {
            cout << "CS property was zero - should be invisible!!!!" << endl;
        }
        else {
            cout << "CS property was NOT zero !!" << endl;
        } 
    }
    catch (SDOPropertyNotFoundException e)
    {
        cout << "No property shows - this is correct" << endl;
    }
    

    if (com->getType().isChangeSummaryType())
    {
        cout << "The company object has a change summary !!!!" << endl;
    }
    else {
        cout << "The company object is OK" << endl;
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

    dumpchangesummary(cs);

    cs->endLogging();

    // expect:
    // company acme, department widgets changed.
    // employee 3 of dept widgets of acme (Irma Newby) created.
    // employee 1 of dept gadgets or acme deleted
    // no change record for employee 1 of company megacorp, dept windows
    // deletion for dept windows of megacorp
    // deletion for emp 1 of windows - showing original value for name (Ivor Payne)
    // deletion for emp 2 of windows.

        
}

void sdotest::seqtest()
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

    printseq(sptr);

    sptr->addCString(sprop,"I am Item 1 of string");
    
    printseq(sptr);

    sptr->addText("I am the first free text");
    
    printseq(sptr);

    sptr->addCString(sprop,"I am Item 2 of string");
    
    printseq(sptr);

    sptr->setCStringValue(1,"I am free text which has been modified");

    DataObjectPtr dep1 = dor->createDataObject("departments");

    printseq(sptr);

    dep1->setCString("name","department1");

    printseq(sptr);

    DataObjectList& dol = dor->getList("departments");

    const Type& tcd = mdg->getType("myspace","Department");
    DataObjectPtr dep2 = mdg->create(tcd);

    printseq(sptr);

    dep2->setCString("name","department2");

    printseq(sptr);

    dol.append(dep2);
    
    printseq(sptr);

}

void sdotest::printseq(SequencePtr sptr)
{
    cout << "======================================" <<endl;
    for (int i=0;i< sptr->size();i++)
    {
        try {
            if (!sptr->isText(i)) 
            {
                const Property& prp = sptr->getProperty(i);
                if (!strcmp(prp.getType().getName(),"Department"))
                {
                    DataObjectPtr pdep = sptr->getDataObjectValue(i);
                    if (pdep != 0)
                    {
                        const char * cs = pdep->getCString("name");
                        if (cs != 0)
                        {
                            cout << cs << endl;
                        }
                        else
                        {
                            cout << " is empty " << endl;
                        }
                    }
                }
                else 
                {
                    const char* stx = sptr->getCStringValue(i);
                    if (stx != 0)
                    {
                        cout << stx << endl;
                    }
                    else
                    {
                        cout << " is empty " << endl;
                    }
                }
            }
            else 
            {
                const char* st = sptr->getCStringValue(i);
                if (st != 0)
                {
                    cout << st << endl;
                }
                else
                {
                    cout << " is empty " << endl;
                }
            }
        }
        catch (SDOPropertyNotSetException)
        {
            cout << "WRONG - got a property not set exception!!!" << endl;
            continue;
        }
    }
    cout << "======================================" << endl;
}

void sdotest::boolbug()
{
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    mdg->addType("myspace","Company");
    mdg->addPropertyToType("myspace","Company","bool","commonj.sdo","Boolean");
    const Type& tcc = mdg->getType("myspace","Company");
    DataObjectPtr dor = mdg->create((Type&)tcc);
    try {
        bool b = dor->getBoolean("bool");
        cout << "bool:" << b << endl;
    }
    catch (SDOPropertyNotSetException)
    {
        cout << "WRONG not set exception" << endl;
    }
}

void sdotest::scope1()
{
    cout << "SCOPE1 - Data Factory" << endl;

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
}

void sdotest::scope2()
{
    cout << "SCOPE2 - Create Data Object" << endl;

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    mdg->addType("myspace","Company");
    mdg->addPropertyToType("myspace","Company","csumm","commonj.sdo","ChangeSummary");
    const Type& tcc = mdg->getType("myspace","Company");
    DataObjectPtr dor = mdg->create((Type&)tcc);
}

void sdotest::scope3()
{
    cout << "SCOPE3 - Lists" << endl;

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
    

}

void sdotest::testGetters(DataObjectPtr dor)
{
    testGetter(dor,"boolean");
    testGetter(dor,"byte");
    testGetter(dor,"character");
    testGetter(dor,"short");
    testGetter(dor,"integer");
    testGetter(dor,"long");
    testGetter(dor,"double");
    testGetter(dor,"float");
    testGetter(dor,"date");
    testGetter(dor,"string");
    testGetter(dor,"bytes");
    testGetter(dor,"dataobject");
}

void sdotest::testGetter(DataObjectPtr dor, char* str)
{

    try {
        cout << "Boolean from " << str;
        bool b = dor->getBoolean(str);
        cout << " was " << b << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "Byte from " << str;
        char by = dor->getByte(str);
        cout << " was " << by <<endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG  unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "Character from " << str;
        wchar_t cy = dor->getCharacter(str);
        cout << " was " << cy << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG  unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "Short from " << str;
        short s = dor->getShort(str);
        cout << " was " << s << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG  unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "Int from " << str;
        int i = dor->getInteger(str);
        cout << " was " << i << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "Long from " << str;
        long l = dor->getLong(str);
        cout << " was " << l << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "LongLong from " << str;
        int64_t ll = dor->getLong(str);
        cout << " was got OK" << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG  unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "Float from " << str;
        float f = dor->getFloat(str);
        cout << " was " << f << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "Double from " << str;
        long double d = dor->getDouble(str);
        cout << " was " << d << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    unsigned int len;

    try {
        cout << "Length of " << str;
        len = dor->getLength(str);
        cout << " was " << len << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG  unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    if (len > 0) {
        try  {
            cout << "String from " << str;
            wchar_t * buf = new wchar_t[len];
            unsigned int gotlen = dor->getString(str,buf,len);
            cout << " size ";
            cout << gotlen << endl;
            for (int jj=0;jj<gotlen;jj++)
            {
                cout << buf[jj] << ":";
            }
            cout << endl;
        }
        catch (SDOPropertyNotSetException pe)
        {
            cout << "WRONG  unset and undefaulted" << endl; 
        }
        catch (SDORuntimeException e)
        {
            cout << e.getEClassName() << endl; 
        }
        try {
            cout << "Bytes from " << str;
            char * cbuf = new char[len];
            unsigned int gotlen = dor->getBytes(str,cbuf,len);
            cout << " size " << gotlen << endl;
            for (int jj=0;jj<gotlen;jj++)
            {
                cout << cbuf[jj]  << ":";
            }
            cout << endl;
        }
        catch (SDOPropertyNotSetException pe)
        {
            cout << "WRONG  unset and undefaulted" << endl; 
        }
        catch (SDORuntimeException e)
        {
            cout << e.getEClassName() << endl; 
        }
    }

    try {
        cout << "Date from " << str;
        SDODate t = dor->getDate(str);
        cout << " was " << t.getTime() << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << " WRONG unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "CString from " << str; 
        const char * string = dor->getCString(str);
        if ( string != 0) cout << " was " << string << endl;
        else cout << " was empty " << endl;
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG -  unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

    try {
        cout << "DataObject from " << str; 
        DataObjectPtr dob = dor->getDataObject(str);
        if (dob != 0)
        {
            cout << " was " << dob << endl;
        }
        else
        {
            cout << " was empty" << endl;
        }
    }
    catch (SDOPropertyNotSetException pe)
    {
        cout << "WRONG  unset and undefaulted" << endl; 
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << endl; 
    }

}

void sdotest::conversiontest()
{
    cout << "Conversion tests" << endl;

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

    cout << "+++++++++++++++PROPERTY VALUES UNSET ++++++++++++++++++" << endl;

    testGetters(dor);

    cout << "+++++++++++++++PROPERTY VALUES SET ++++++++++++++++++++" << endl;

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


    testGetters(dor);

    cout << "+++++++++++++++END OF TEST ++++++++++++++++++++++++++++" << endl;

    // phase 2 all set.....

}


void sdotest::usertest()
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

    void* value = (void*)0xFACC0FF;

    test->setUserData(value);

    cout << "I wanted 0xFACC0FF " << test->getUserData() << endl;

    root->setUserData("usertest",value);

    cout << "I wanted 0xFACC0FF and got " << root->getUserData("usertest") << endl;
    
    root->setUserData((unsigned int)0,(void*)0x20);

    cout << "I wanted 0x20 and got " << root->getUserData((unsigned int)0) << endl;

    const Property& prop = root->getType().getProperty("usertest");
    root->setUserData(prop,(void*)0x40020);

    cout << "I wanted 0x40020 and got " << root->getUserData(prop) << endl;
    
    test->setUserData("boolean", (void*)0x120);

    cout << "I wanted (graceful)0 and got " << test->getUserData("boolean") << endl;

    test->setUserData("unsetboolean", (void*)0x340);

    cout << "I wanted (graceful)0 and got " << test->getUserData("boolean") << endl;

    test->setUserData("object", (void*)0x120);

    cout << "I wanted 120 and got " << test->getUserData("object") << endl;

    test->setUserData("unsetobject", (void*)0x540);

    cout << "I wanted (graceful)0 and got " << test->getUserData("unsetobject") << endl;

    test->setUserData("objects", (void*)0x640);

    // TODO might be dodgy - this allows setting of user data on a base of a list
    cout << "I wanted 640 and got " << test->getUserData("objects") << endl;

    test->setUserData("objects[1]", (void*)0x740);

    cout << "I wanted 0x740 and got " << test->getUserData("objects[1]") << endl;
    }
    catch (SDORuntimeException e)
    {
        cout << "Exception in user test - unexpected" << endl;
    }

}

void sdotest::versiontest()
{
    cout << "The SDO version is :" <<    SdoRuntime::getVersion() << endl;
    cout << "The Major version is: " << SdoRuntime::getMajor() << endl;
    cout << "The Minor version is: " << SdoRuntime::getMinor() << endl;
    cout << "The Fix level     is: " << SdoRuntime::getFix() << endl;
}

void sdotest::noncontest()
{
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



}




void sdotest::defaulttest()
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

    cout << "Boolean default is true: " << test->getBoolean("boolean") << endl;

    cout << "Byte default is d: " << test->getByte("byte") << endl;
    
    cout << "Character default is e: " << test->getCharacter("character") << endl;

    cout << "Short default is 300: " << test->getShort("short") << endl;

    cout << "Long default is 400: " << test->getInteger("long") << endl;

    try {
    cout << "Longs default is 800: " << test->getInteger("longs[1]") << endl;
    }
    catch (SDOIndexOutOfRangeException ex)
    {
        cout << "Expected index out of range OK" << endl;
    }

    cout << "Float default is 600: " << test->getFloat("float") << endl;

    cout << "LongDouble default is 700: " << test->getDouble("longdouble") << endl;

    cout << "String default is HELP: ";
    unsigned int lenw = test->getLength("string");
    if (lenw > 0) {
        char* tw = new char[lenw];
        test->getBytes("string",tw,lenw);
        for (int i=0;i<lenw;i++)
        {
            cout << tw[i];
        }
        cout << endl;
    }

    cout << "Bytes default is HELP: ";
    unsigned int len = test->getLength("bytes");
    if (len > 0) {
        char* tc = new char[len];
        test->getBytes("bytes",tc,len);
        for (int i=0;i<len;i++)
        {
            cout << tc[i];
        }
        cout << endl;
    }
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << " in "; 
        cout << e.getFileName() << " at line ";
        cout << e.getLineNumber() << endl;
        cout << e.getFunctionName() << " ";
        cout << e.getMessageText() << endl;
    }



}


void sdotest::showdefault(const Type& tm)
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
    cout << "Boolean default is : " << bb << endl;

    char cc = pbyte.getByteDefault();
    cout << "Byte default is : " << cc << endl;

    wchar_t wc = pcharacter.getCharacterDefault();
    cout << "Character default is : " << wc << endl;

    short ss = pshort.getShortDefault();
    cout << "Short default is : " << ss << endl;

    long ll = plong.getIntegerDefault();
    cout << "Integer default is : " << ll << endl;
    long ll2 = plongs.getLongDefault();
    cout << "Integer many default is : " << ll2 << endl;

    int64_t llll = plonglong.getLongDefault();
    cout << "Long default is : " << (long)llll << endl;

    float ff = pfloat.getFloatDefault();
    cout << "Float default is : " << ff << endl;
    
    long double  dd = plongdouble.getDoubleDefault();
    cout << "Double default is : " << dd << endl;

    const SDODate& sd = pdate.getDateDefault();
    cout << "Date default is : " << sd.getTime() << endl;

    unsigned int l = pstring.getDefaultLength();
    if (l > 0)
    {
        wchar_t * buf = new wchar_t[l+1];
        l = pstring.getStringDefault(buf,l);
        cout << "String default length is" << l << endl;
        for (int i=0;i<l;i++)
        {
            cout << buf[i];
        }
        cout << endl;
        delete buf;
    }
    else
    {
        cout << "String default is zero length" << endl; 
    }

    l = pbytes.getDefaultLength();
    if (l > 0)
    {
        char * buf = new char[l+1];
        l = pbytes.getBytesDefault(buf,l);
        cout << "Bytes default length is" << l << endl;
        for (int i=0;i<l;i++)
        {
            cout << buf[i];
        }
        cout << endl;
        delete buf;
    }
    else
    {
        cout << "Bytes default is zero length" << endl; 
    }
}


void sdotest::propdefaulttest()
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

    cout << "Testing unset defaults....." << endl;

    showdefault(tm);

    
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

    cout << "Testing set defaults....." << endl;

    showdefault(tm);

    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << " in "; 
        cout << e.getFileName() << " at line ";
        cout << e.getLineNumber() << endl;
        cout << e.getFunctionName() << " ";
        cout << e.getMessageText() << endl;
    }

}

void sdotest::nulltest()
{
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

    // first check all props are unset

    ChangeSummaryPtr cs = test->getChangeSummary();

    cs->beginLogging();

    PropertyList pl = test->getInstanceProperties();

    cout << "Initially - all properties unset, and default values..." << endl;
    for (i=0;i<pl.size(); i++)
    {
        if (pl[i].isMany())continue;
        try {
        if (pl[i].getType().isDataType()) {
            cout << pl[i].getName() << " set:" << test->isSet(pl[i]) << " value:";
            const char *xx = test->getCString(pl[i]);
            if (xx != 0)
            {
                cout << xx << endl;
            }
            else
            {
                cout << " is empty " << endl;
            }
        }
        else {
            cout << pl[i].getName() << " set:" << test->isSet(pl[i]) << " value:";
            DataObjectPtr xy = test->getDataObject(pl[i]);
            if (xy != 0)
            {
                cout << xy << endl;
            }
            else
            {
                cout << " is empty " << endl;
            }
        }
        }
        catch (SDOPropertyNotSetException)
        {
            cout << " WRONG - got not set exception!!!";
            continue;
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
    


    cout << "Should now have all properties set, and zero values..." << endl;
    for (i=0;i<pl.size(); i++)
    {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            cout << pl[i].getName() << " set:" << test->isSet(pl[i]) << " value:" << test->getCString(pl[i]) << endl;
        }
        else {
            cout << pl[i].getName() <<" set:" << test->isSet(pl[i]) << " value:" << test->getDataObject(pl[i]) << endl;
        }

    }

    for (i=0;i<pl.size(); i++)
    {
        test->unset(pl[i]);
    }
    cout << "Should be back to having properties unset, and default values..." << endl;
    for (i=0;i<pl.size(); i++)
    {

        try {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            cout << pl[i].getName() << " set:" << test->isSet(pl[i]) << " value:";
            const char *xx = test->getCString(pl[i]);
            if (xx != 0)
            {
                cout << xx << endl;
            }
            else
            {
                cout << " is empty " << endl;
            }
        }
        else {
            cout << pl[i].getName() << " set:" << test->isSet(pl[i]) << " value:";
            DataObjectPtr dp = test->getDataObject(pl[i]);
            if (dp != 0)
            {
                cout << dp << endl;
            }
            else
            {
                cout << " is empty " << endl;
            }
        }
        }
        catch (SDOPropertyNotSetException)
        {
            cout << "WRONG  - not set exception" << endl;
            continue;
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
            cout << pl[i].getName() << " set:" << test->isSet(pl[i]) << " value:";
            const char*  xx = test->getCString(pl[i]);
            if ( xx != 0)
            {
                cout << xx << endl;    
            }
            else
            {
                cout << " is empty " << endl;
            }
        }
        else {
            cout << pl[i].getName() << " set:" << test->isSet(pl[i]) << " value:";
            DataObjectPtr db = test->getDataObject(pl[i]);
            if (db != 0)
            {
                cout << db << endl;
            }
            else
            {
                cout << " is empty " << endl;
            }
        }
        }
        catch (SDOPropertyNotSetException)
        {
            cout << "WRONG - not set exception " << endl;
            continue;
        }
    }
    ChangedDataObjectList& cl = cs->getChangedDataObjects();
    for ( i =0; i< cl.size() ; i++) 
    {
        if (cs->isCreated(cl[i])) {
            cout << "Created:" << cl[i] << endl;
        }
        if (cs->isDeleted(cl[i])) {
            cout << "Deleted:" << cl[i] << endl;
        }
        if (cs->isModified(cl[i])) {
            cout << "Modified:" << cl[i] << endl;
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

    cout << "Should all be null, and default values..." << endl;
    for (i=0;i<pl.size(); i++)
    {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            // check for a null first!!
            cout << pl[i].getName() << "isNull:" << test->isNull(i) <<" set:" << test->isSet(pl[i]) << " value:";
            if (test->getCString(pl[i])  == 0) cout << endl;
            else cout << test->getCString(pl[i]) << endl;
        }
        else {
            cout << pl[i].getName() << "isNull:" << test->isNull(i) <<" set:" << test->isSet(pl[i]) << " value:";
            if (test->getDataObject(pl[i]) == 0) cout << endl;
            else cout << test->getDataObject(pl[i]) << endl;
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

    cout << "Should all have values, and not be null.." << endl;
    for (i=0;i<pl.size(); i++)
    {
        
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            cout << pl[i].getName() << "isNull:" << test->isNull(i) <<" set:" << test->isSet(pl[i]) << " value:";
            if (test->getCString(pl[i]) == 0) cout << endl;
            else cout << test->getCString(pl[i]) << endl;

        }
        else {
            cout << pl[i].getName() << "isNull:" << test->isNull(i) <<" set:" << test->isSet(pl[i]) << " value:";
            if (test->getDataObject(pl[i]) == 0) cout << endl;
            else cout <<  test->getDataObject(pl[i]) << endl;
        }
    }

    for (i=0;i<pl.size(); i++)
    {
        if (!pl[i].isMany() )
        {
            test->setNull(pl[i]);
        }
    }

    cout << "Should all be null, and default values..." << endl;
    for (i=0;i<pl.size(); i++)
    {
        
        try {
        if (pl[i].isMany())continue;
        if (pl[i].getType().isDataType()) {
            cout << pl[i].getName() << "isNull:" << test->isNull(i) <<" set:" << test->isSet(pl[i]) << " value:";
            if (test->getCString(pl[i]) == 0) cout << endl;
            else cout << test->getCString(pl[i]) << endl;

        }
        else {
            cout << pl[i].getName() << "isNull:" << test->isNull(i) <<" set:" << test->isSet(pl[i]) << " value:";
            if (test->getDataObject(pl[i]) == 0) cout  << endl;
            else cout << test->getDataObject(pl[i]) << endl;
        }
        }
        catch (SDOPropertyNotSetException)
        {
            cout << " WRONG - not set exception" << endl;
            continue;
        }
    }

    cs->endLogging();
}

int sdotest::main(int argc, char** argv)
{

    int i;

    printf("Test Program starting to create types ...\n");

    /* First create a DataFactory , then add some types and props...*/

    /* This is dms creation of metadata */

    try {


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

        cout << "Manager is sequenced?" << tm.isSequencedType() << endl;

        const Type& td = mdg->getType("myspace","Department");
        const Type& te = mdg->getType("myspace","Employee");
        const Type& tp = mdg->getType("myspace","Person");
        const Type& tds= mdg->getType("myspace","DerivedString");
        const Type& tsd= mdg->getType("myspace","SubDepartment");

        cout << "Manager is sequenced?" << tm.isSequencedType() << endl;





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

        cout << "Manager is sequenced?" << tm.isSequencedType() << endl;

        mdg->addPropertyToType(td,"name", ts);
        
        mdg->addPropertyToType(td,"id", ti);

        mdg->addPropertyToType(td,"manager", tm);
    
        mdg->addPropertyToType(tm,"name", ts);


        mdg->addPropertyToType(te,"name",ts);
        mdg->addPropertyToType(td,"employees",te,true,false,true);



        // emps and managers are both people (in theory).
        mdg->setBaseType(te,tp);
        cout << "Manager is sequenced?" << tm.isSequencedType() << endl;
        mdg->setBaseType(tm,tp);
        cout << "Manager is sequenced?" << tm.isSequencedType() << endl;
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

         cout << "Manager is sequenced?" << tm.isSequencedType() << endl;

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


        cout << "Manager is sequenced?" << tm.isSequencedType() << endl;

        // create a few aliases

        mdg->setAlias("myspace","Company","TheFirm");
        mdg->setAlias("myspace","Company","departments","TheDepartments");
        mdg->setAlias("myspace","Company","TheBigFirm");
        mdg->setAlias("myspace","Company","TheLittleFirm");

        const Type& tlf = mdg->getType("myspace","TheBigFirm");

        cout << "I hope this says Company :" << tlf.getName() << endl;

        cout << "I hope this says 3 : " << tlf.getAliasCount() << endl;

        for (int ai = 0; ai < tlf.getAliasCount(); ai++)
        {
            cout << "AKA : " << tlf.getAlias(ai) << endl;
        }

        // just for a laugh - how about finding the TheDepartments?




    /* 
     * create an empty datagraph with a type system starting from
     * company
     */

    /* 
     *
     * start of dms getting datagraph 
     */

        // Change summary test begins
        mdg->addPropertyToType(tc,"csumm","commonj.sdo","ChangeSummary");
        // should log an error - but not fail
        mdg->addPropertyToType(td,"csumm","commonj.sdo","ChangeSummary");

        cout << "Manager is sequenced?" << tm.isSequencedType() << endl;
 
        cout << "BEFORE RESOLUTION" << endl;
        printDataStructure(mdg);

          /* Now create some objects in the dg */
    
//        DataObjectPtr dor;

        const Type& tcc = mdg->getType("myspace","Company");
        DataObjectPtr dor = mdg->create((Type&)tcc);

        cout << "AFTER RESOLUTION" << endl;
        printDataStructure(mdg);

        cout << "Manager is sequenced?" << tm.isSequencedType() << endl;

        dor->setCString("substring","This is the sub string - its primitive, but not a string");
        
        const char* subby = dor->getCString("substring");

        cout << subby << endl;

        dor->setCString("name","acmecorp");
        const char* chnam = dor->getCString("name");
        cout << chnam << endl;

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

        cout << sname << endl;

        // This should put a created entry in the cs.

        // This should be equivalent to the alternative below.........
        const Type& tcd = mdg->getType("myspace","Manager");
        DataObjectPtr pdg = mdg->create((Type&)tcd);

        // This set should put the entry into the change summary.
        // both in the creations and in the changes.
         dor->setDataObject("pdg",pdg);


        // try getting the boolean as a string - should be defalted to false:
        const char *bol;
        try{
            bol = pdg->getCString("boolean");
            if (bol != 0)
            {
                cout << "Expected default boolean (false) : " << bol << endl;
            }
            else
            {
                cout << " is empty " << endl;
            }
        }
        catch (SDOPropertyNotSetException)
        {
            cout << "WRONG handled notset exception " << endl;
        }

        pdg->setBoolean("boolean", true);
        bol = pdg->getCString("boolean");
        cout << "Expected  boolean (true) : " << bol << endl;

        // and the widechars?
        const char* wdc;
        try {
            wdc = pdg->getCString("string");
            if (wdc != 0)
            {
                cout << "Expected default string (0) : " << bol << endl;
            }
            else
            {
                cout << " is empty " << endl;
            }
        }
        catch (SDOPropertyNotSetException)
        {
            cout << "WRONG handled notset exception" << endl;
        }


        // this should get a valid sequence..

        SequencePtr sq =   pdg->getSequence();


        // The alternative.............................................
        // Here the create should put the entry into the change summary
        //DataObject* pdg = dor->createDataObject("pdg");
    
        // This should do nothing to the cs, as its in a created object
        pdg->setCString("name","Jacques LePlace");
        
        // This should modify the property, and add an item to the sequence.
        sq->addCString("name", "Jacques LeWrongPlace");


        // The creation entry should be removed from the change summary
        // The entry should no longer exist and the pointer is invalid
        pdg->detach();

        pdg = dor->getDataObject("pdg");


        cout << " A deleted data object should be zero: " << pdg << endl;

        pdg = mdg->create((Type&)tcd);

        // A new creation in the change summary
        dor->setDataObject("pdg",pdg);

        // No modification as the object is created
        pdg->setCString("name", "Mr Horace the snail");

        sq = pdg->getSequence();

        try {
        // element 0 is the first setting - which we just deleted!
            sq->setCStringValue(0,"Eric the half a bee");
        }
        catch (SDOIndexOutOfRangeException)
        {
            // thats OK
            sq->addCString("name","Eric the quarter bee");
        }


        sq->addText(" - only a quarter was expected \r\n");

        for (int ii=0;ii<sq->size();ii++)
        {
            cout << sq->getCStringValue(ii);
        }

        try {
            const char* n = pdg->getCString("name");
            cout << " Name from deleted item: " << n << endl;
        }
        catch (SDOPropertyNotFoundException e)
        {
        // thats ok
            cout << "caught prop not found" << endl;
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
        cout << "0xffffffffffffffff = : " << lls << endl;

        pdg->setLong("longlong",0x7FFFFFFFFFFFFFFF);

        lls = pdg->getCString("longlong");
        cout << "0x7fffffffffffffff =  " << lls << endl;

        pdg->setLong("longlong",0x7FFFFFFF);
        lls = pdg->getCString("longlong");
        cout << "0x7fffffff =  " << lls << endl;

        pdg->setLong("longlong",0x80000000);
        lls = pdg->getCString("longlong");
        cout << "0x80000000 =  " << lls << endl;

        pdg->setLong("longlong",78);

        // pdg->setCharPtr("charptr","Hello I am a char star");

        wchar_t* wide = new wchar_t[4];
        wide[0] = 'W';
        wide[1] = 'i';
        wide[2] = 'd';
        wide[3] = 'e';

        pdg->setString("string",wide,4);

        // and as a string?
        wdc = pdg->getCString("string");
        cout << " Expected Wide (fat chance) - got : " << wdc << endl;

        delete wide;

        char* thin = new char[4];
        thin[0] = 'T';
        thin[1] = 'h';
        thin[2] = 'i';
        thin[3] = 'n';

        pdg->setBytes("bytes",thin,4);

        delete thin;


        const char* ps = pdg->getCString("string");
        cout << "Expecting string, got " << ps << endl;
    
        bool pb = pdg->getBoolean("boolean");
        cout << "Expected bool true, got " << pb << endl;

        char pc = pdg->getByte("byte");
        cout << "Expected char 23, got " << pc << endl;

        wchar_t pw = pdg->getCharacter("character");
        cout << "expected wchar 45, got " << pw << endl;

        short pss = pdg->getShort("short");
        cout << "Expected short 34, got " << pss << endl;

        long pl = pdg->getLong("long");
        cout << "Expected long 56 , got " <<pl << endl;

        int64_t pi = pdg->getLong("longlong");
        cout << "Expected long long 78, got " << (long)pi << endl;

        long double ld = pdg->getDouble("longdouble");
        cout << "Expected long double 89, got " << ld << endl;

        float pf = pdg->getFloat("float");
        cout << "Expected float 90, got " << pf << endl;

        SDODate  pt = pdg->getDate("date");
        cout << "Expected time_t 200, got " << pt.getTime() << endl;

        //const char * pcs = pdg->getCharPtr("charptr");
        //cout <<"Expected charptr, got " << pcs << endl;

        wchar_t* result;
        // get the length to allocate:
        unsigned int widelen = pdg->getString("string",0,0);
        if (widelen > 0) {
            result = new wchar_t[widelen];
            widelen = pdg->getString("string",result,widelen);
            for ( i=0;i<widelen;i++)
            {
                cout << "Wide[" << i << "]=" << result[i] << endl;
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
                cout << "Thin[" << i << "]=" << thinresult[i] << endl;
            }
            delete thinresult;
        }


        //cout <<"Expected charptr, got " << pcs << endl;

        // add two more departments - the first should provoke the
        // saving of a list in a chanaeg summary

        DataObjectPtr dep3 = dor->createDataObject("departments");
        dep3->setCString("name","Architecture");

        // The second should do no logging in the change summary

        DataObjectPtr dep4= dor->createDataObject("departments");
        dep4->setCString("name","Pyrotechnics");


        // create another one using tha alias
        DataObjectPtr dep5= dor->createDataObject("TheDepartments");
        dep5->setCString("name","WibbleSmodging");

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
        //float f = dolist[0]->getFloat("");
        float f = dolist.getFloat(0);

        cout << f;

        dolist.setFloat(0,(float)567.7);

        f = dolist.getFloat(0);

        cout << f;

        dolist.insert(0,(float)34.56);




    //    cs->DebugPrint();

//    const char* ch = dor->getString("departments[1]/employees[2]/name");

        DataObjectPtr ddd = dor->getDataObject("departments[1]/employees[2]");

        const char* ch = ddd->getCString("name");

        cout << "Are you Bill?:" << ch << endl;

        // just for a laugh - how about finding the TheDepartments?

        DataObjectPtr ddd2 = dor->getDataObject("TheDepartments[1]/employees[2]");

        const char* ch2 = ddd2->getCString("name");

        cout << "Are you still Bill?:" << ch2 << endl;



        DataObjectPtr dempofm = dor->getDataObject("employee of the month");

        const char* chh = dempofm->getCString("name");

        cout << "The employee of the month is " << chh << endl;

        // Suppose we delete bill - I wonder what happens?

        // doesnt work - why not??dor->unSet("departments[1]/employees[2]");
        DataObjectList& dlo = dor->getList("departments[1]/employees");
        DataObjectPtr fred = dlo.remove(1);
        //delete fred;

        dempofm = dor->getDataObject("employee of the month");

        cout << "Hopefully emp of month is now zero : " << dempofm << endl;

    /* "The client would create a data mediator and get graph which 
    would return the root data object " says colin*/

        const char* boss = dor->getCString("pdg/name");
    
        cout << boss << endl;

        DataObjectPtr mypdg = dor->getDataObject("pdg");

        Type::Types t = mypdg->getTypeEnum();
    
        if (t != Type::OtherTypes) {
            cout << "MY pdg is not something!" << endl;
        }

        const char* boss2 = mypdg->getCString("name");

        cout << boss2 << endl;

    /* TODO1 ref or pointer to dol. 
       Manip done by methods of the
       list, reflected directly in the dg 
       need methods to create /insert dataobjects in lists*/


    /* getPrimitiveArrayListVectorThingy() */

        DataObjectList& deps = dor->getList("departments");
    
        cout << "size of list " << deps.size() << endl;

        DataObjectPtr dout = deps[0];

        dout->setCString("name","Research And Development");

        const char* snew = dout->getCString("name");

        cout << snew << endl;

        string snew2 = dor->getCString("departments[1 ] /name");

        cout << snew2 << endl;

        for (int lx = 0; lx < deps.size(); lx++)
        {
            cout << "Department: " << deps[lx]->getCString("name") << endl;
        }

        cs->endLogging();


        try {
            // should fail - if localtype not setstd::
            dor->setCString("departments","department label");
            const char* slabel = dor->getCString("departments");
            cout << "String in list type:" << slabel << endl;
        }
        catch (SDORuntimeException e)
        {
            cout << "Normal expected exception in test case" << endl;
        }

        try 
        {
    // The exception for path is caught by the SDO library - perhaps we should
    // pass it up?
            const char* snew3 = dor->getCString(" ]awep50wi4,0qw9q]45]#        q2345        -t        -v3lt6o        -56o        56=-o7nl        ewv/;sdl        f[vxddglh]px        dfju/    g#k./jh#.k./");
            cout << snew3 << endl;
        }
        catch (SDOPropertyNotFoundException e)
        {
            cout << "Normal Invalid path exception" << endl;
        }

        try 
        {

        // catch a and a property not found 
            const Property& pp = dor->getType().getProperty("notaproperty");
        }
        catch (SDOPropertyNotFoundException e)
        {
            cout << "Normal SDOPropertyNotFound exception" << endl;
        }

    }

    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << " in "; 
        cout << e.getFileName() << " at line ";
        cout << e.getLineNumber() << endl;
        cout << e.getFunctionName() << " ";
        cout << e.getSeverity() << " "; 
        cout << e.getMessageText() << endl;
    }


    return 0;
}

void sdotest::getproptest()
{
    // should be able to get a property by xpath...

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

    try 
    {

    const Property& p = dor->getType().getProperty("name");
    cout << "Companys name property is:" << p.getName() << endl;

    const Property& p1 = dor->getType().getProperty("departments");
    cout << "Companys dept property is:" << p1.getName() << endl;

    // now try some xpaths...

    const Property& p2 = dor->getType().getProperty("departments/employees");
    cout << "Departments empl property is:" << p2.getName() << endl;

    const Property& p3 = dor->getType().getProperty("departments[456]/employees[123]");
    cout << "Departments empl property is:" << p3.getName() << endl;

    const Property& p4 = dor->getType().getProperty("departments.34/employees.123/name");
    cout << "Employees name property is:" << p4.getName() << endl;

    }
    catch (SDORuntimeException e)
    {
        cout <<"Exceptions - xpath didnt work" << endl;
    }
    try {
    cout << "before p5" << endl;
    const Property& p5 = dor->getType().getProperty("departments.34/[]/name");
    cout << "after p5" << endl;
    cout << "Employees name property is:" << p5.getName() << endl;
    cout << "Expected error - didnt get one" << endl;
    }
    catch (SDORuntimeException e)
    {
    }

    try {
    const Property& p6 = dor->getType().getProperty("deptartments");
    cout << "Deptartments property is:" << p6.getName() << endl;
    cout << "Expected error - didnt get one" << endl;
    }
    catch (SDORuntimeException e)
    {
    }

    try {
    const Property& p7 = dor->getType().getProperty("../company");
    cout << "Company property is:" << p7.getName() << endl;
    cout << "Expected error - didnt get one" << endl;
    }
    catch (SDORuntimeException e)
    {
    }
}

void sdotest::querytest()
{
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
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
        cout << "Expected employee3: got " << dob1->getCString("name") << endl;

        DataObjectPtr dob2 = dor->getDataObject("departments[1]/employees[bool=true]");
        cout << "Expected employee1: got " << dob2->getCString("name") << endl;

        DataObjectPtr dob3 = dor->getDataObject("departments[1]/employees[byte=2]");
        cout << "Expected employee2: got " << dob3->getCString("name") << endl;

        DataObjectPtr dob4 = dor->getDataObject("departments[1]/employees[byte=1]");
        cout << "Expected employee1: got " << dob4->getCString("name") << endl;

        DataObjectPtr dob5 = dor->getDataObject("departments[1]/employees[bytes=hello]");
        cout << "Expected employee1: got " << dob5->getCString("name") << endl;

        DataObjectPtr dob5a = dor->getDataObject("departments[1]/employees[bytes=\"hello\"]");
        cout << "Expected employee1: got " << dob5a->getCString("name") << endl;
        
        DataObjectPtr dob5b = dor->getDataObject("departments[1]/employees[bytes='hello']");
        cout << "Expected employee1: got " << dob5b->getCString("name") << endl;

        DataObjectPtr dob6 = dor->getDataObject("departments[1]/employees[bytes=womp!]");
        cout << "Expected employee3: got " << dob6->getCString("name") << endl;

        DataObjectPtr dob7 = dor->getDataObject("departments[1]/employees[double=200.0]");
        cout << "Expected employee2: got " << dob7->getCString("name") << endl;

        DataObjectPtr dob8 = dor->getDataObject("departments[1]/employees[string=help!]");
        cout << "Expected employee2: got " << dob8->getCString("name") << endl;

        DataObjectPtr dob8a = dor->getDataObject("departments[1]/employees[string=\"help!\"]");
        cout << "Expected employee2: got " << dob8a->getCString("name") << endl;

        DataObjectPtr dob8b = dor->getDataObject("departments[1]/employees[string= 'help!']");
        cout << "Expected employee2: got " << dob8b->getCString("name") << endl;
    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << " in "; 
        cout << e.getFileName() << " at line ";
        cout << e.getLineNumber() << endl;
        cout << e.getFunctionName() << " ";
        cout << e.getMessageText() << endl;
    }
    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[1]/employees[bool=doughnut]");
        cout << "Expected to fail!" << endl;
    }
    catch (SDORuntimeException e)
    {
    }

    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[4]/employees[bool=true]");
        cout << "Expected to fail!" << endl;
    }
    catch (SDORuntimeException e)
    {
    }

    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[1]/employees[bytes=whoomp!]");
        cout << "Expected to fail!" << endl;
    }
    catch (SDORuntimeException e)
    {
    }

    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[1]/blmployees[bool=true]");
        cout << "Expected to fail!" << endl;
    }
    catch (SDORuntimeException e)
    {
    }

    try {
        DataObjectPtr dob1 = dor->getDataObject("departments[teapot]/employees[bool=true]");
        cout << "Expected to fail!" << endl;
    }

    catch (SDORuntimeException e)
    {
    }

    delete bytes;
    delete bytes2;
    delete bytes3;
    delete mbytes;
    delete mbytes2;
    delete mbytes3;
}

void sdotest::setmany()
{
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
    class2->setCString("teacher/name","Mr Brian Onastick");

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

    cout <<" School  :" << school->getCString("name") << endl;

    cout <<" Address1:" << school->getCString("address/lines.0") << endl;
    cout <<" Address2:" << school->getCString("address/lines.1") << endl;
    cout <<" Address3:" << school->getCString("address/lines.2") << endl;
    cout <<" Address4:" << school->getCString("address/lines.3") << endl;

    cout <<" Class   :" << class1->getCString("name") << endl;
    cout <<" Teacher :" << class1->getCString("teacher/name") << endl;
    cout <<" Pupil1  :" << class1->getCString("children.0/name") << endl;
    cout <<" Pupil2  :" << class1->getCString("children[number=2]/name") << endl;
    cout <<" Pupil3  :" << class1->getCString("children[3]/name") << endl;

    cout <<" Class   :" << class2->getCString("name") << endl;
    cout <<" Teacher :" << class2->getCString("teacher/name") << endl;
    cout <<" Pupil1  :" << class2->getCString("children[1]/name") << endl;
    cout <<" Pupil2  :" << class2->getCString("children.1/name") << endl;
    cout <<" Pupil3  :" << class2->getCString("children[number=6]/name") << endl;

    try {
        cout <<" Pupil3  :" << class2->getCString("children[4]/name") << endl;
        cout << "That should have failed with a path not found" << endl;
    }
    catch (SDOPathNotFoundException e)
    {

    }
    if (XpathHelper::isIndexed("abc[2]")) {
        cout << "Indexed as expected" << endl;
    }
    else {
        cout << "Bad - not indexed" << endl;
    }

    if (XpathHelper::isIndexed("wibble/[2]")) {
        cout << "Indexed as expected" << endl;
    }
    else {
        cout << "Bad - not indexed" << endl;
    }

    if (XpathHelper::isIndexed("wibble/wobble[2]")) {
        cout << "Indexed as expected" << endl;
    }
    else {
        cout << "Bad - not indexed" << endl;
    }

    if (XpathHelper::isIndexed("wibble/wobble.2")) {
        cout << "Indexed as expected" << endl;
    }
    else {
        cout << "Bad - not indexed" << endl;
    }

    if (XpathHelper::isIndexed("wibble/wobble.2")) {
        cout << "Indexed as expected" << endl;
    }
    else {
        cout << "Bad - not indexed" << endl;
    }

    try {
        cout <<" Address5  :" << school->getCString("address/lines[5]") << endl;
        cout << "Address5 should have failed with an index out of range" << endl;
    }
    catch (SDOIndexOutOfRangeException e)
    {

    }

    try {
        school->setCString("address/lines[6]","PostCode");
        cout << "Debatable behaviour - appended" << endl;
        cout <<" Address4  :" << school->getCString("address/lines[5]") << endl;
    }
    catch (SDORuntimeException e)
    {
        cout <<"Unexpected exception"<< endl;
    }


}

void sdotest::carotest2()
{
    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","root");

    mdg->addPropertyToType("myspace","root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

    const Type& troot = mdg->getType("myspace","root");

    DataObjectPtr root = mdg->create((Type&)troot);

}

void sdotest::adddeletetest()
{
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

}

void sdotest::carotest()
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

    cout << "name of the company is " << comp->getCString("name") << endl;
    try {
            root->unset("companies[0]");
    }
    catch (SDOUnsupportedOperationException e)
    {
        cout << "Normal unsupported operation for unset of many valued item" << endl;
    }
}


void sdotest::bug2()
{

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

    dumpproperties(root);

    DataObjectList& dl = root->getList("companies");
    dl.remove(0);

    dumpproperties(root);


    cout << "Change summary should have no entries..." << endl;

    dumpchangesummary(cs);

    dumpproperties(root);

    cout << "Change summary should have no entries..." << endl;

    dumpchangesummary(cs);

    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << " in "; 
        cout << e.getFileName() << " at line ";
        cout << e.getLineNumber() << endl;
        cout << e.getFunctionName() << " ";
        cout << e.getMessageText() << endl;
    }
}


void sdotest::dumpproperties(DataObjectPtr root)
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
                    cout << "MProperty:" << pl[i].getName() << "[" << buf << "]:" << dl.getCString(j) << endl;
                }
            }
            else
            {
                cout << "MObject Property" << pl[i].getName() << endl;
                for (int j=0;j<dl.size();j++)
                {
                    if (dl[j] != 0)
                    {
                        dumpproperties(dl[j]);
                    }
                }
                cout << "End of MObject Property " << pl[i].getName() << endl;
            }
        }

        else
        {
            if (pl[i].getType().isDataType())
            {
                cout << "Property:" << pl[i].getName() << ":" << root->getCString(pl[i]) << endl;
            }
            else
            {
                cout << "Object Property:" << pl[i].getName() << endl;
                DataObjectPtr d = root->getDataObject(pl[i]);
                if (d != 0)
                {
                    dumpproperties(d);
                }
                else
                {
                    cout << "  Value was not set or null" << endl;
                }
                cout << "End of Object Property " << pl[i].getName() << endl;
            }
        }
    }
}


void sdotest::datetest()
{

    try  {

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

    cout << "name of the company is " << comp->getCString("name") << endl;

    // cannot get undefaulted properties
    //cout << "unset start of the company is " << comp->getDate("startupdate").getTime() << endl;

    comp->setDate("startupdate",SDODate( 1000 ));

    cout << "set  start of the company is " << comp->getDate("startupdate").getTime() << endl;
    
    DataObjectList& dol = comp->getList("reviewdates");

    
    dol.append(SDODate(2000));
    dol.append(SDODate(4000));

    for (int i=0;i < dol.size(); i++)
    {
        cout << "Review number:" << i << " was:" << dol.getDate(i).getTime()  << endl;
        cout << "Formatted:" << i << " was:" << dol.getDate(i).ascTime()  << endl;
    }

    cs->endLogging();
    }

    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << " in "; 
        cout << e.getFileName() << " at line ";
        cout << e.getLineNumber() << endl;
        cout << e.getFunctionName() << " ";
        cout << e.getMessageText() << endl;
    }

}

void sdotest::matttest1()
{

    try  {
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
        cout << "Container of Billy should be zero, but is :" << cont->getCString("name") << endl;
    }
    else {
        cout << "OK, - Billy is not contained." << endl;
    }

    DataObjectList& li1 = dept1->getList("employees");
    DataObjectList& li2 = dept2->getList("employees");
    li1.append(billy);
    li2.append(sue);

    DataObjectPtr cont2 = billy->getContainer();

    if (cont2 != 0) 
    {
        cout << "Container of Billy should be dept1, and is :" << cont2->getCString("name") << endl;
    }
    else {
        cout << "Problem - Billy is not contained." << endl;
    }

    li1.remove(0);
    li2.remove(0);

    DataObjectPtr cont3 = billy->getContainer();

    if (cont3 != 0) 
    {
        cout << "Container of Billy should be zero, but is :" << cont3->getCString("name") << endl;
    }
    else {
        cout << "OK, - Billy is not contained." << endl;
    }


    li1.append(sue);
    li2.append(billy);

    DataObjectPtr cont4 = billy->getContainer();

    if (cont4 != 0) 
    {
        cout << "Container of Billy should be dept2, and is :" << cont4->getCString("name") << endl;
    }
    else {
        cout << "Problem - Billy is not contained." << endl;
    }

    }
    catch (SDORuntimeException e)
    {
        cout << e.getEClassName() << " in "; 
        cout << e.getFileName() << " at line ";
        cout << e.getLineNumber() << endl;
        cout << e.getFunctionName() << " ";
        cout << e.getMessageText() << endl;
    }
}

void sdotest::carotest3()
{
    // sequence new APIs 
    // data object list, new getLength(unsigned int)

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

    printseq(sptr);

    sptr->addCString(sprop,"I am Item 1 of string");
    
    printseq(sptr);

    sptr->addText("I am the first free text");
    
    printseq(sptr);

    sptr->addCString(sprop,"I am Item 2 of string");
    
    printseq(sptr);

    sptr->setText(1,"I am free text which has been modified");

    printseq(sptr);

    DataObjectPtr dep1 = dor->createDataObject("departments");

    printseq(sptr);

    dep1->setCString("name","department1");

    printseq(sptr);

    DataObjectList& dol = dor->getList("departments");

    unsigned int ii = dol.getLength(0);

    cout << "Length of a data object should be zero:" << ii << endl;

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

    cout << "Element zero, length 1: " << strl.getLength(0) << endl;
    cout << "Element one,  length 2: " << strl.getLength(1) << endl;
    cout << "Element two,  length 3: " << strl.getLength(2) << endl;
    cout << "Element three,length 4: " << strl.getLength(3) << endl;
    cout << "Element four, length 5: " << strl.getLength(4) << endl;

    try {
        cout << "Element five doesnt exist: " << strl.getLength(5);
    }
    catch (SDOIndexOutOfRangeException e)
    {
        cout << "Normal out of range exception in test" << endl;
    }

    DataObjectList& numl = dor->getList("integers");

    try {
        cout << "On an empty list? " << numl.getLength(0);
    }
    catch (SDOIndexOutOfRangeException e)
    {
        cout << "Normal out of range exception in test" << endl;
    }
}


int main (int argc, char** argv)
{
    Logger::setLogging(20);

    sdotest::b47293();


    sdotest::propdefaulttest();
    sdotest::graham5();
    sdotest::graham4();

    sdotest::detachtest();
    sdotest::includetest();
    sdotest::testLoad();
    sdotest::leak();
    sdotest::twolists();
    sdotest::b46633();

    sdotest::testUtil();
    
    sdotest::clonetest();

    sdotest::b46693();
    Logger::setLogging(0);
//    sdotest::b46734();
    sdotest::notfound();
    sdotest::testErrors();

    sdotest::b46634();
    sdotest::loadManyOpen();

    sdotest::doctest();
    sdotest::b46617b();
    sdotest::b46617();
    sdotest::b46613();

    sdotest::graham3();
    sdotest::graham1();
    sdotest::graham2();


    sdotest::merle1();
    sdotest::loadOpenNS();

    sdotest::saveOpen();
    sdotest::loadOpen();
    sdotest::testui();
    sdotest::testOpen();
    sdotest::testSCA();
    sdotest::testabstract();

    sdotest::testsubsload();
    sdotest::testsubs();
    sdotest::bug45933();
    sdotest::setnull();
    sdotest::bug2();
    sdotest::cssave2();
    sdotest::csload2();
    sdotest::cssave();
    sdotest::csload();
    sdotest::eqhelpertest();
    sdotest::cohelpertest();
    sdotest::datetest();
    sdotest::carotest3();
    sdotest::matttest1();
    sdotest::adddeletetest();
    sdotest::carotest2();
    sdotest::carotest();
    sdotest::setmany();
    sdotest::noncontest();
    sdotest::versiontest();
    sdotest::defaulttest();
    sdotest::nulltest();
    sdotest::usertest();
    sdotest::querytest();
    sdotest::getproptest();
    sdotest::rcptest();
    sdotest::seqtest();
    sdotest::changesummarytest();
    sdotest::conversiontest();
    sdotest::boolbug();
    sdotest::scope1();
    sdotest::scope2();
    sdotest::scope3();
    return sdotest::main(argc, argv);
    /* All objects freed ? */
return 0;
}


