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

#include "sdo_axiom_test.h"


void axiomtest::axiom_test()
{

    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    mdg->addType("myspace","Company");
    
    mdg->addType("myspace","Department");

    mdg->addType("myspace","Employee");
    
    mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
     
    mdg->addPropertyToType("myspace","Company","norbert",
                           "commonj.sdo","String", false, false, false);
    
    mdg->addPropertyToType("myspace","Company","departments",
                           "myspace","Department", true, false, true);
    
    mdg->addPropertyToType("myspace","Department","name",
                           "commonj.sdo","String", false, false, false);

    mdg->addPropertyToType("myspace","Department","employees",
                           "myspace","Employee", true, false, true);

    mdg->addPropertyToType("myspace","Employee","name",
                           "commonj.sdo","String", false, false, false);

    const Type& tcc = mdg->getType("myspace","Company");

    DataObjectPtr company = mdg->create((Type&)tcc);
    company->setCString("name","The VDB Company Inc");
    company->setCString("norbert","Spandex Courvette");


    DataObjectPtr dept1 = company->createDataObject("departments");
    dept1->setCString("name","Accounts");
    
    DataObjectPtr dept2 = company->createDataObject("departments");
    dept2->setCString("name","Billing");

    DataObjectPtr emp1 = dept1->createDataObject("employees");
    emp1->setCString("name","Albert");
    
    DataObjectPtr emp2 = dept1->createDataObject("employees");
    emp2->setCString("name","Ben");
    
    DataObjectPtr emp3 = dept2->createDataObject("employees");
    emp3->setCString("name","Cyril");
    
    DataObjectPtr emp4 = dept2->createDataObject("employees");
    emp4->setCString("name","Dominic");

    AxiomHelper* ax = AxiomHelper::getHelper();

    if (ax)
    {
        axiom_document_t* doc = ax->toAxiomDoc(company);

        if (doc)
        {
            ax->output(doc);

            DataObjectPtr newdob = ax->toSdo(doc,mdg);
             
            if (newdob)
            {
                cout << "Printing first data object..." << endl;
                SDOUtils::printDataObject(cout,newdob);
                cout << "Finished" << endl;
            }

            axiom_node_t* node = 
                            AXIOM_DOCUMENT_GET_ROOT_ELEMENT(doc, ax->getEnv());
 

            DataObjectPtr newdob2 = ax->toSdo(node,mdg);

            if (newdob2)
            {
                cout << "Printing second data object..." << endl;
                SDOUtils::printDataObject(cout,newdob2);
                cout << "Finished" << endl;
            }


        }

    }

    return;

}

extern "C"{

int main (int argc, char** argv)
{
    axiomtest::axiom_test();
    return 0;
}

}
