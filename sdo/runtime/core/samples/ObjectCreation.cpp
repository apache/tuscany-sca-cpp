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

/* $Rev$ $Date: 2005/12/22 16:54:14 $ */

#include "samples.h"
using namespace std;

void ObjectCreation::sample()
{


    cout << " ********** Sample ObjectCreation **********" << endl;

    try {


        /** 
         * Get a data factory. With it we can either create metadata
         * or load it from an XSD.
         */

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
        /**
         * Add some Types to the data factory..
         * The booleans at on addType are:
         * "isSequenced", "isOpen" "isAbstract" and "isDataType"
         */

        mdg->addType("myspace","Company");

        mdg->addType("myspace","Department");
        
        /**
         * Manager is going to be a sequenced type...
         */
        
        mdg->addType("myspace","Manager", true, false);

        mdg->addType("myspace","Employee");


        /**
         * We will make employee and manager sub-types of 'person'
         */

        mdg->addType("myspace","Person", true, false);


        /**
         * having all the types defined, we can now define the tree
         * by giving properties to the types.
         */

        /** 
         * We could use the API passing in name and URI for each
         * Type , or get the Types back and use them directly.
         * Here we get back the types to use...
         */

        const Type& tc = mdg->getType("myspace","Company");
        const Type& ts = mdg->getType("commonj.sdo","String");
        const Type& ti = mdg->getType("commonj.sdo","Integer");
        const Type& tm = mdg->getType("myspace","Manager");
        const Type& td = mdg->getType("myspace","Department");
        const Type& te = mdg->getType("myspace","Employee");
        const Type& tp = mdg->getType("myspace","Person");


        /**
         * Example 1 - add a property of type String to type company
         */

        mdg->addPropertyToType(tc,"name",ts);

        /** 
         * Example 2 - add using the name of the company instead of the
         * type...
         */

        mdg->addPropertyToType("myspace","Company","address",ts);

        /**
         * Example 3 - add a many valued property 
         */

        mdg->addPropertyToType(tc,"departments", "myspace","Department",
        true);


        /**
         * Example 4 - add a reference property
         */

        mdg->addPropertyToType(tc,"employee of the month", "myspace",
             "Employee",false, false, false);


        /**
         * Add other department properties...
         */

        mdg->addPropertyToType(td,"name", ts);
        mdg->addPropertyToType(td,"id", ti);
        mdg->addPropertyToType(td,"manager", tm);
        mdg->addPropertyToType(td,"employees",te,true,false,true);

        /**
         * Add a name to the person
         */

        mdg->addPropertyToType(tp,"name", ts);



        /**
         * Make employees and mamagers both substypes of person
         */

        mdg->setBaseType(te,tp);
          mdg->setBaseType(tm,tp);
  
        /**
         * And give them different properties of their own.
         */

        mdg->addPropertyToType(tm,"officeid", ts);
        mdg->addPropertyToType(te,"cubelocation", ts);

        
      
        /**
         * The data structure looks like this:
     
         *  Company
         *  ----name (String)
         *  ----address *String)
         *  ----departments (Department, many valued)
         *  ----employee of the month ( Employee - reference)
     
         * Person
         * ----name (String)

         * Employee
         * ----name (String - inherited from Person)
         * ----cubelocation (String)

         * Manager
         * ----name (String - inherited from Person)
         * ----officeid (String)

         * Department
         * ----name (String)
         * ----id (Integer)
         * ---- manager (Manager)
         * ---- employees (Employee - many valued)

    
        /** 
         * create an object of type Company using the DataFactory
         */

        DataObjectPtr dor = mdg->create((Type&)tc);

        /**
         * Set the company name to Acme
         */

        dor->setCString("name","Acme");

        /**
         * Set up the two departments  - using the 
         * DataObject createDataObject API
         */

        DataObjectPtr dep1 = dor->createDataObject("departments");
        dep1->setCString("name","Development");
        dep1->setInteger("id",100);

        DataObjectPtr man1 = dep1->createDataObject("manager");
        man1->setCString("name","Herve Jones");

        DataObjectPtr dep2= dor->createDataObject("departments");
        dep2->setCString("name","Marketing");
        dep2->setInteger("id",200);

        DataObjectPtr man2 = dep2->createDataObject("manager");
        man1->setCString("name","August Phan");

        /**
         * Give the departments some employees
         */

        DataObjectPtr emp1 = dep1->createDataObject("employees");
        emp1->setCString("name","Fred Appleby");
        emp1->setCString("cubelocation","100-A");

        DataObjectPtr emp2 = dep1->createDataObject("employees");
        emp2->setCString("name","Jane Bloggs");
        emp2->setCString("cubelocation","100-B");

        DataObjectPtr emp3 = dep2->createDataObject("employees");
        emp3->setCString("name","Robin Corbet");
        emp3->setCString("cubelocation","200-A");

        DataObjectPtr emp4 = dep2->createDataObject("employees");
        emp4->setCString("name","Martha Denby");
        emp4->setCString("cubelocation","200-B");

        cout << "Company Name:" << dor->getCString("name") << endl;
        
        DataObjectList& depts = dor->getList("departments");
        for (int i=0;i<depts.size();i++)
        {
            cout << "  Department Name:" << depts[i]->getCString("name") << endl;
            
            DataObjectList& emps = depts[i]->getList("employees");
 
            for (int j=0;j<emps.size();j++)
            {
                cout << "    Employee Name:" << emps[j]->getCString("name") << endl;
            }
        }
    }
    catch (SDORuntimeException e)
    {
        cout << "Exception in ObjectCreation" <<endl;
        cout << e;
    }
    cout << " ********** Sample ends ********************" << endl;
    return;
}


