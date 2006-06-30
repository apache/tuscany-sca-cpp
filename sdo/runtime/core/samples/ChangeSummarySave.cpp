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

void ChangeSummarySave::sample()
{
    cout << " ********** ChangeSummarySave sample********" << endl;

    try {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("ChangeSummarySave.xsd");

        /**
         * Load the schema from ChangeSummarySave.xsd
         */

        const Type& tstring  = mdg->getType("commonj.sdo","String");
        const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
        const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
        const Type& tcomp    = mdg->getType("companyNS","CompanyType");
        const Type& tdept    = mdg->getType("companyNS","DepartmentType");
        const Type& temp     = mdg->getType("companyNS","EmployeeType");

    
        /**
         * create a graph, set the change summary logging, modify the
         * data, then save it to a file
         */

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

        
        /**
         * Set the employee of the month - which is a reference, not
         * a containment value
         */

        comp->setDataObject("employeeOfTheMonth",emp2);

        /** 
         * The XSD defined the company type as having a change summary,
         * so we can get it...
         */
          
        ChangeSummaryPtr cs = comp->getChangeSummary();

        /**
         * And ask it to start logging...
         */

        cs->beginLogging();

        /**
         * With logging on, create a new employee
         */

        DataObjectPtr emp4 = mdg->create(temp);
        emp4->setCString("name","Al Smith");
        emp4->setCString("SN","E0004");
        emp4->setBoolean("manager",true);

        /**
         * The first recorded change happens now, as the employee is
         * added into the data graph. Emp4 (Al Smith) will appear in the
         * change summary as a creation.  There will also be a change
         * record for the list "employees" of this department, holding the
         * values before Al was added.
         */ 
        dol2.append(emp4);

        /** 
         * The second change is to remove element 1 from the
         * same list - Thats Mary Smith. 
         * Mary will appear as a deletion, but there will be no extra
         * change record for "employees", as its already been changed.
         * Mary was employee of the month, so that reference gets 
         * emptied, and a change record is set up for it, recording
         * Mary as the old value.
         */

        dol2.remove(1); // element 1  is Mary

        DataObjectPtr emp5 = mdg->create(temp);
        emp5->setCString("name","Bill Withers");
        emp5->setCString("SN","E0005");

        
        /**
         * The third change is to  append Bill to the same list.
         * Bill appears as a creation, but there is no change recorded to
         * the employees list.
         */

        dol2.append(emp5);


        /**
         * The company name is changed. A change record is set up for
         * the property "name" of this company. It stores the old value
         * "ACME"
         */

        comp->setCString("name","MegaCorp");

       /**
         * The company employee of the month is changed.  The old
         * value has already been changed from Mary to NULL, so no change
         * record is created here at all
         */

        comp->setDataObject("employeeOfTheMonth",emp4);

  
        /** 
         * Stop logging changes
         */

        cs->endLogging();


        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
        xmh->save(doc,"ChangeSummarySave-output.xml");

        /**
         * Have a look in the file and see if you can recognise the changes
         * above
         */

    }
    catch (SDORuntimeException e)
    {
        cout << "Exception in ChangeSummarySave" << endl;
        cout << e;
    }
    cout << " ********** Sample ends ********************" << endl;
}
