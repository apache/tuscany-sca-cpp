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

/* $Rev$ $Date$ */

#include "samples.h"
using namespace std;

void Query::sample()
{
    cout << " ********** Query sample *******************" << endl;

    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    /**
     * Create some types 
     */

    mdg->addType("myspace","Company");
    mdg->addType("myspace","Department");
    mdg->addType("myspace","Employee");


    /* Now add the properties to the types...*/

    
    const Type& tstring = mdg->getType("commonj.sdo","String");
    const Type& tbool=   mdg->getType("commonj.sdo","Boolean");
    const Type& tint=    mdg->getType("commonj.sdo","Integer");
 
    const Type& tc = mdg->getType("myspace","Company");
    const Type& td = mdg->getType("myspace","Department");
    const Type& te = mdg->getType("myspace","Employee");

    /**
     * The company
     */

    mdg->addPropertyToType(tc,"name",tstring); // single string name
    mdg->addPropertyToType(tc,"departments", 
                                     td,true); // many departments
    mdg->addPropertyToType(tc,"employee of the month"
        , te,    false, false, false);          // reference to employee

    /**
     * The department
     */

    mdg->addPropertyToType(td,"name", tstring); // single string name 
    mdg->addPropertyToType(td,"employees",te,
                              true,false,true); // many employees


    /**
     * The employee
     */

    mdg->addPropertyToType(te,"isFullTime",tbool);
    mdg->addPropertyToType(te,"employeeNumber",tint);
    mdg->addPropertyToType(te,"name",tstring);


    const Type& tcc = mdg->getType("myspace","Company");

    DataObjectPtr dor = mdg->create((Type&)tcc);

    // The departments

    DataObjectPtr dept = dor->createDataObject("departments");
    dept->setCString("name","Shipping");

    DataObjectPtr dept2 = dor->createDataObject("departments");
    dept2->setCString("name","Buying");

    // The employees

    DataObjectPtr emp1 = dept->createDataObject("employees");
    DataObjectPtr emp2 = dept->createDataObject("employees");
    DataObjectPtr emp3 = dept->createDataObject("employees");
    DataObjectPtr emp4 = dept2->createDataObject("employees");


    emp1->setBoolean("isFullTime",true);
    emp1->setInteger("employeeNumber",65443);
    emp1->setCString("name","Norman");


    emp2->setBoolean("isFullTime",false);
    emp2->setInteger("employeeNumber",64778);
    emp2->setCString("name","Carl");

    emp3->setBoolean("isFullTime",true);
    emp3->setInteger("employeeNumber",61990);
    emp3->setCString("name","Amanda");

    emp4->setBoolean("isFullTime",true);
    emp4->setInteger("employeeNumber",56789);
    emp4->setCString("name","Donna");

    dor->setDataObject("employee of the month",emp4); // Donna is referenced.


    try {

        // access the first employee of the first department who is not full time

        DataObjectPtr dob1 = dor->getDataObject("departments[1]/employees[isFullTime=false]");
        cout << "Carl should be the first part timer: " << dob1->getCString("name") << " is." <<endl;

        // get the same employee by index
        
        DataObjectPtr dob2 = dor->getDataObject("departments[1]/employees[2]");
        cout << "Carl should be employees[2]:" << dob2->getCString("name") << " is." << endl;

        // use the dot notation to get the same employee 

        DataObjectPtr dob3 = dor->getDataObject("departments.0/employees.1");
        cout << "Carl should be employees.1:" << dob3->getCString("name") << " is." << endl;

        // get the reference...

        DataObjectPtr dob4 = dor->getDataObject("employee of the month");
        cout << "Donna should be employee of the month:" << dob4->getCString("name") << " is." << endl;

        // And by employee number...

        DataObjectPtr dob5 = dor->getDataObject("departments[2]/employees[employeeNumber=56789]");
        cout << "Donna should be employee 56789:" << dob5->getCString("name") << " is." << endl;

        // If the query yields no value because the element doesnt exist...

        try 
        {
            DataObjectPtr dob6 = dor->getDataObject("departments[1]/employees[employeeNumber=56789]");
            cout << "Did not get the expected exception" << endl;
        }
        catch (SDORuntimeException e)
        {
            cout << "Expected an IndexOutOfRangeException and got " << e.getEClassName() << endl;
        }

        // If the query yields no value because the path is invalid...


        try 
        {
            DataObjectPtr dob7 = dor->getDataObject("departments[fish]/employees[0]");
            cout << "Did not get the expected exception" << endl;
        }
        catch (SDORuntimeException e)
        {
            cout << "Expected an PathNotFoundException and got " << e.getEClassName() << endl;
        }
    }
    catch (SDORuntimeException e)
    {
        cout << "Unexpected error in Query " << e << endl;
    }


    cout << " ********** End  Query Sample **************" << endl;
}
