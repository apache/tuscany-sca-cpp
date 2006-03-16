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

/* $Id: SdoTest2.cpp,v 1.2 2006/03/16 12:53:57 slattery Exp $ */

#include <stdio.h>

#pragma warning(disable:4786)

#include <iostream>
using namespace std;



#include "sdotest.h"



using namespace commonj::sdo;


int sdotest::compatiblefactory()
{
    DataFactoryPtr f1 = DataFactory::getDataFactory();

    f1->addType("Namespace","Project");
    f1->addType("Namespace","WorkPackage");
    f1->addType("Namespace","LineItem");
    f1->addType("Namespace","StringHolder");
 
    f1->addType("Namespace","Root");
    f1->addPropertyToType("Namespace","Root","project",
                           "Namespace","Project", false, false, true);

    f1->addPropertyToType("Namespace","StringHolder","value",
                           "commonj.sdo","String", false, false, false);
   
    f1->addPropertyToType("Namespace","Project","id",
                           "commonj.sdo","String", false, false, false);
    
    f1->addPropertyToType("Namespace","Project","string",
                           "Namespace","StringHolder", false, false, true);
 
    f1->addPropertyToType("Namespace","WorkPackage","name",
                           "commonj.sdo","String", false, false, false);
    

    f1->addPropertyToType("Namespace","WorkPackage","string",
                           "Namespace","StringHolder", false, false, true);

    f1->addPropertyToType("Namespace","LineItem","itemname",
                           "commonj.sdo","String", false, false, false);

    f1->addPropertyToType("Namespace","LineItem","string",
                           "Namespace","StringHolder", false, false, true);

    f1->addPropertyToType("Namespace","Project","packages",
                           "Namespace","WorkPackage", true, false, true);

    f1->addPropertyToType("Namespace","WorkPackage","lineitems",
                           "Namespace","LineItem", true, false, true);

  
    // project
    //    id (string)
    //    string (StringHolder)
    //    workpackages (WorkPackage)
    
    // workpackage
    //     name (string)
    //     string (StringHolder)
    //     lineitems (LineItem)

    // lineitem
    //     itemname (string)
    

    // factories 1 and 2 are compatible

    DataFactoryPtr f2 = DataFactory::getDataFactory();
    
    f2->addType("Namespace","Project");
    f2->addType("Namespace","WorkPackage");
    f2->addType("Namespace","LineItem");
    f2->addType("Namespace","StringHolder");
 
    f2->addType("Namespace","Root");
    f2->addPropertyToType("Namespace","Root","project",
                           "Namespace","Project", false, false, true);

 
    f2->addPropertyToType("Namespace","StringHolder","value",
                           "commonj.sdo","String", false, false, false);
   
    f2->addPropertyToType("Namespace","Project","id",
                           "commonj.sdo","String", false, false, false);
    
    f2->addPropertyToType("Namespace","Project","string",
                           "Namespace","StringHolder", false, false, true);
 
    f2->addPropertyToType("Namespace","WorkPackage","name",
                           "commonj.sdo","String", false, false, false);
    

    f2->addPropertyToType("Namespace","WorkPackage","string",
                           "Namespace","StringHolder", false, false, true);

    f2->addPropertyToType("Namespace","LineItem","itemname",
                           "commonj.sdo","String", false, false, false);

    f2->addPropertyToType("Namespace","LineItem","string",
                           "Namespace","StringHolder", false, false, true);

    f2->addPropertyToType("Namespace","Project","packages",
                           "Namespace","WorkPackage", true, false, true);

    f2->addPropertyToType("Namespace","WorkPackage","lineitems",
                           "Namespace","LineItem", true, false, true);


    // factory 3 has no project type

    DataFactoryPtr f3 = DataFactory::getDataFactory();

    f3->addType("Namespace","WorkPackage");
    f3->addType("Namespace","LineItem");
    f3->addType("Namespace","StringHolder");
 
    f3->addType("Namespace","Root");
    f3->addPropertyToType("Namespace","Root","project",
                           "Namespace","WorkPackage", false, false, true);

 
    f3->addPropertyToType("Namespace","StringHolder","value",
                           "commonj.sdo","String", false, false, false);
   
    f3->addPropertyToType("Namespace","WorkPackage","name",
                           "commonj.sdo","String", false, false, false);
    

    f3->addPropertyToType("Namespace","WorkPackage","string",
                           "Namespace","StringHolder", false, false, true);

    f3->addPropertyToType("Namespace","LineItem","itemname",
                           "commonj.sdo","String", false, false, false);

    f3->addPropertyToType("Namespace","LineItem","string",
                           "Namespace","StringHolder", false, false, true);


    f3->addPropertyToType("Namespace","WorkPackage","lineitems",
                           "Namespace","LineItem", true, false, true);


    // factory 4 looks compatible - but stringholder is missing

    DataFactoryPtr f4 = DataFactory::getDataFactory();
    
    f4->addType("Namespace","Project");
    f4->addType("Namespace","WorkPackage");
    f4->addType("Namespace","LineItem");
 
    f4->addType("Namespace","Root");
    f4->addPropertyToType("Namespace","Root","project",
                           "Namespace","Project", false, false, true);

 
    f4->addPropertyToType("Namespace","Project","id",
                           "commonj.sdo","String", false, false, false);
    
 
    f4->addPropertyToType("Namespace","WorkPackage","name",
                           "commonj.sdo","String", false, false, false);
    

    f4->addPropertyToType("Namespace","LineItem","itemname",
                           "commonj.sdo","String", false, false, false);

    f4->addPropertyToType("Namespace","Project","packages",
                           "Namespace","WorkPackage", true, false, true);

    f4->addPropertyToType("Namespace","WorkPackage","lineitems",
                           "Namespace","LineItem", true, false, true);


    // factory 5 looks compatible, but the lists are not lists

    DataFactoryPtr f5 = DataFactory::getDataFactory();
    
    f5->addType("Namespace","Project");
    f5->addType("Namespace","WorkPackage");
    f5->addType("Namespace","LineItem");
    f5->addType("Namespace","StringHolder");
 
    f5->addType("Namespace","Root");
    f5->addPropertyToType("Namespace","Root","project",
                           "Namespace","Project", false, false, true);

 
    f5->addPropertyToType("Namespace","StringHolder","value",
                           "commonj.sdo","String", false, false, false);
   
    f5->addPropertyToType("Namespace","Project","id",
                           "commonj.sdo","String", false, false, false);
    
    f5->addPropertyToType("Namespace","Project","string",
                           "Namespace","StringHolder", false, false, true);
 
    f5->addPropertyToType("Namespace","WorkPackage","name",
                           "commonj.sdo","String", false, false, false);
    

    f5->addPropertyToType("Namespace","WorkPackage","string",
                           "Namespace","StringHolder", false, false, true);

    f5->addPropertyToType("Namespace","LineItem","itemname",
                           "commonj.sdo","String", false, false, false);

    f5->addPropertyToType("Namespace","LineItem","string",
                           "Namespace","StringHolder", false, false, true);

    f5->addPropertyToType("Namespace","Project","packages",
                           "Namespace","WorkPackage", false, false, true);

    f5->addPropertyToType("Namespace","WorkPackage","lineitems",
                           "Namespace","LineItem", false, false, true);

    // factory 6 looks OK, but the lineitems have no itemname field

    DataFactoryPtr f6 = DataFactory::getDataFactory();
    
    f6->addType("Namespace","Project");
    f6->addType("Namespace","WorkPackage");
    f6->addType("Namespace","LineItem");
    f6->addType("Namespace","StringHolder");
 
    f6->addType("Namespace","Root");
    f6->addPropertyToType("Namespace","Root","project",
                           "Namespace","Project", false, false, true);

 
    f6->addPropertyToType("Namespace","StringHolder","value",
                           "commonj.sdo","String", false, false, false);
   
    f6->addPropertyToType("Namespace","Project","id",
                           "commonj.sdo","String", false, false, false);
    
    f6->addPropertyToType("Namespace","Project","string",
                           "Namespace","StringHolder", false, false, true);
 
    f6->addPropertyToType("Namespace","WorkPackage","name",
                           "commonj.sdo","String", false, false, false);
    

    f6->addPropertyToType("Namespace","WorkPackage","string",
                           "Namespace","StringHolder", false, false, true);

    f6->addPropertyToType("Namespace","LineItem","string",
                           "Namespace","StringHolder", false, false, true);

    f6->addPropertyToType("Namespace","Project","packages",
                           "Namespace","WorkPackage", true, false, true);

    f6->addPropertyToType("Namespace","WorkPackage","lineitems",
                           "Namespace","LineItem", true, false, true);


    // factory 7 looks OK, but the identifiers are ints
    
    
    DataFactoryPtr f7 = DataFactory::getDataFactory();

    f7->addType("Namespace","Project");
    f7->addType("Namespace","WorkPackage");
    f7->addType("Namespace","LineItem");
    f7->addType("Namespace","StringHolder");
 
    f7->addType("Namespace","Root");
    f7->addPropertyToType("Namespace","Root","project",
                           "Namespace","Project", false, false, true);

    f7->addPropertyToType("Namespace","StringHolder","value",
                           "commonj.sdo","String", false, false, true);
   
    f7->addPropertyToType("Namespace","Project","id",
                           "commonj.sdo","Integer", false, false, false);
    
    f7->addPropertyToType("Namespace","Project","string",
                           "Namespace","StringHolder", false, false, true);
 
    f7->addPropertyToType("Namespace","WorkPackage","name",
                           "commonj.sdo","String", false, false, false);
    

    f7->addPropertyToType("Namespace","WorkPackage","string",
                           "Namespace","StringHolder", false, false, true);

    f7->addPropertyToType("Namespace","LineItem","itemname",
                           "commonj.sdo","Integer", false, false, false);

    f7->addPropertyToType("Namespace","LineItem","string",
                           "Namespace","StringHolder", false, false, true);

    f7->addPropertyToType("Namespace","Project","packages",
                           "Namespace","WorkPackage", true, false, true);

    f7->addPropertyToType("Namespace","WorkPackage","lineitems",
                           "Namespace","LineItem", true, false, true);


    // create a tree from Root in factory 1

    DataObjectPtr root = f1->create("Namespace","Root");
    DataObjectPtr project = root->createDataObject("project");
    project->setCString("id","The TTP Project");
    DataObjectPtr str = project->createDataObject("string");
    str->setCString("value","The Recursive Acronym Project");
    DataObjectPtr wp1 = project->createDataObject("packages");
    DataObjectPtr wp2 = project->createDataObject("packages");
    wp1->setCString("name","Work Package 1");
    wp2->setCString("name","Work Package 2");
    DataObjectPtr li1 = wp1->createDataObject("lineitems");
    DataObjectPtr li2 = wp1->createDataObject("lineitems");
    DataObjectPtr li3 = wp2->createDataObject("lineitems");
    DataObjectPtr li4 = wp2->createDataObject("lineitems");
    li1->setCString("itemname","LineItem 1");
    li2->setCString("itemname","LineItem 2");
    li3->setCString("itemname","LineItem 3");
    li4->setCString("itemname","LineItem 4");
    DataObjectPtr str1 = li1->createDataObject("string");
    DataObjectPtr str2 = li2->createDataObject("string");
    DataObjectPtr str3 = li3->createDataObject("string");
    DataObjectPtr str4 = li4->createDataObject("string");
    str1->setCString("value","String1");
    str2->setCString("value","String2");
    str3->setCString("value","String3");
    str4->setCString("value","String4");


//    cout << "DATA OBJECTS FROM FACTORY 1" << endl;
//    printDataObject(root);

    if (!transferto(root,f2, false)) return 0;
    if (!transferto(root,f3, true)) return 0;
    if (!transferto(root,f4, true)) return 0;
    if (!transferto(root,f5, true)) return 0;
    if (!transferto(root,f6, true)) return 0;
    if (!transferto(root,f7, true)) return 0;

    // finally, lets move one which has a parent and cant be moved.

    try {
        DataObjectPtr project = root->getDataObject("project");

        DataObjectPtr root2 = f2->create("Namespace","Root");
    
        root2->setDataObject("project", project);
        return 0;
    }
    catch (SDORuntimeException e)
    {
        //cout << "Compatibility test correctly returned an error :" ;
        //cout << e.getMessageText() << endl;
        return 1;
    }
  
}

int sdotest::transferto(DataObjectPtr root, DataFactoryPtr f2, bool expecterror)
{
    try {

        // transfer the tree to factory 2...
        DataObjectPtr rproject = root->getDataObject("project");

        DataObjectPtr project = CopyHelper::copy(rproject);

        DataObjectPtr root2 = f2->create("Namespace","Root");
    
        root2->setDataObject("project", project);

        //cout << "DATA OBJECTS TRANSFERRED" << endl;

        //printDataObject(root2);
        
        if (expecterror)
        {
            return 0;
        }
        return 1;
    }
    catch (SDORuntimeException e)
    {
        if (expecterror){
            //cout << "Compatibility test correctly returned an error :" ;
            //cout << e.getMessageText() << endl;
            return 1;
        }
        else
        {
            if (!silent)cout << "Compatibility test failed whilst transferring " << e << endl;
            return 0;
        }
    }
}


int sdotest::b48602()
{


    int i,j;

    try {


        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    
        xsh->defineFile("b48602.xsd");

        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "PROBLEM: b48602 XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
        return 0;
    }
    catch (SDOTypeNotFoundException e)
    {
        return 1;
    }
    catch (SDORuntimeException e)
    {
        return 1;
    }

}

int sdotest::testany(const char* xsd, 
                     const char* acceptedxsderror,
                     const char* xml,
                     const char* acceptedxmlerror)
{


    int i,j,rc;

    try {

        char *c;
        

 
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    
        if (xsd)
        {
            char * name1 = new char[strlen(xsd)+5];
            char * name3 = new char[strlen(xsd)+5];
            strcpy(name1,xsd);
            while ((c = strchr(name1,'.')) != 0)*c = '_';
            strcpy(name3,name1);
            strcat(name1,".dat");
            strcat(name3,".txt");


            FILE *f1 = fopen(name1,"w+");
            if (f1 == 0)
            {
                if (!silent)cout << "Unable to open " << name1 << endl;
                delete name1;
                delete name3;
                return 0;
            }

            xsh->defineFile(xsd);

            if ((i = xsh->getErrorCount()) > 0)
            {
                if (acceptedxsderror != 0)
                {
                    for (j=0;j<i;j++)
                    {
                        if (!strcmp(acceptedxsderror,xsh->getErrorMessage(j)))
                        {
                            fclose(f1);
                            delete name1;
                            delete name3;
                            return 1;
                        }
                    }
                }
                if (!silent)
                {
                    cout << "PROBLEM: Testany XSD reported some errors:" << endl;
                    for (j=0;j<i;j++)
                    {
                        const char *m = xsh->getErrorMessage(j);
                        if (m != 0) cout << m;
                        cout << endl;
                    }
                }
                fclose(f1);
                delete name1;
                delete name3;
                return 0;
            }


            TypeList& tl = mdg->getTypes();

            fprintf(f1, "***** TYPES BEFORE RESOLVE **********************************\n");

            for (i=0;i<tl.size();i++)
            {
                fprintf(f1,"Type:%s#%s\n",tl[i].getURI(),tl[i].getName());
                PropertyList& pl = tl[i].getProperties();
                for (int j=0;j<pl.size();j++)
                {
                    fprintf(f1,"Property:%s ",pl[j].getName());
                    if (pl[j].isMany())
                         fprintf(f1, "(many) ");
                    fprintf(f1,  " of type %s\n",pl[j].getType().getName());
                }
            }

            fprintf(f1, "*******************************END TYPES******************\n");

            fclose(f1);
        

            rc = comparefiles(name1,name3);
            
            delete name1;
            delete name3;
            
            return rc;
    
        }

        if (xml == 0 || strlen(xml) == 0) return 1;

        char * name2 = new char[strlen(xml)+5];
        char * name4 = new char[strlen(xml)+5];
        strcpy(name2,xml);
        while ((c = strchr(name2,'.')) != 0)*c = '_';
        strcpy(name4,name2);
      
        strcat(name2,".dat");
        strcat(name4,".txt");

        FILE *f2 = fopen(name2,"w+");
        if (f2 == 0)
        {
            if (!silent)cout << "Unable to open " << name2 << endl;
            delete name2;
            delete name4;
            return 0;
        }

        XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
 
        XMLDocumentPtr doc = xmh->loadFile(xml);

        if ((i = xmh->getErrorCount()) > 0)
        {
            if (acceptedxmlerror != 0)
            {
                for (j=0;j<i;j++)
                {
                    if (!strcmp(acceptedxmlerror,xmh->getErrorMessage(j)))
                    {
                        fclose(f2);
                        delete name2;
                        delete name4;
                        return 1;
                    }
                }
            }
            if (!silent)
            {
                cout << "TestAny XML found errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xmh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            fclose(f2);
            delete name2;
            delete name4;
            return 0;
        }

        else {
            TypeList& tl = mdg->getTypes();

            fprintf(f2, "***** TYPES AFTER RESOLVE*********************************\n");

            for (i=0;i<tl.size();i++)
            {
                fprintf(f2,"Type:%s#%s\n",tl[i].getURI(),tl[i].getName());
                PropertyList& pl = tl[i].getProperties();
                for (int j=0;j<pl.size();j++)
                {
                    fprintf(f2, "Property:%s ",pl[j].getName());
                    if (pl[j].isMany())
                        fprintf(f2, "(many) ");
                    fprintf(f2,  " of type %s\n",pl[j].getType().getName());
                }
            }

            fprintf(f2, "*******************************END TYPES******************\n");
    

            DataObjectPtr dob = doc->getRootDataObject();
            printDataObject(f2, dob);

            fclose(f2);

            rc = comparefiles(name2,name4);

            delete name2;
            delete name4;

            return rc;
        }

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in TestAny" << e << endl;
        return 0;
    }
}



int sdotest::testanytwo(const char* xsd, const char* xsd2,
                         const char* xml)
{


    int i,j;

    try {

    char * name1 = new char[strlen(xsd)+5];
    char * name2 = new char[strlen(xml)+5];
    char * name3 = new char[strlen(xsd)+5];
    char * name4 = new char[strlen(xml)+5];
    strcpy(name1,xsd);
    strcpy(name2,xml);
    char *c;
    while ((c = strchr(name1,'.')) != 0)*c = '_';
    while ((c = strchr(name2,'.')) != 0)*c = '_';
    strcpy(name3,name1);
    strcpy(name4,name2);
    
    strcat(name1,".dat");
    strcat(name2,".dat");
    strcat(name3,".txt");
    strcat(name4,".txt");

    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    
    if (xsd)
    {
        xsh->defineFile(xsd);

        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "PROBLEM: Testany XSD reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }

    }

    if (xsd2)
    {
        xsh->defineFile(xsd2);

        if ((i = xsh->getErrorCount()) > 0)
        {
            if (!silent)
            {
                cout << "PROBLEM: Testany XSD2 reported some errors:" << endl;
                for (j=0;j<i;j++)
                {
                    const char *m = xsh->getErrorMessage(j);
                    if (m != 0) cout << m;
                    cout << endl;
                }
            }
            return 0;
        }
    }

    if (xsd)
    {


        FILE *f1 = fopen(name1,"w+");
        if (f1 == 0)
        {
            if (!silent)cout << "Unable to open " << name1 << endl;
            return 0;
        }

        TypeList& tl = mdg->getTypes();

        fprintf(f1,"***** TESTANY ******************************************\n");

        for (i=0;i<tl.size();i++)
        {
            fprintf(f1,"Type:%s#%s\n",tl[i].getURI(),tl[i].getName());
            PropertyList& pl = tl[i].getProperties();
            for (int j=0;j<pl.size();j++)
            {
                fprintf(f1,"Property:%s ",pl[j].getName());
                if (pl[j].isMany())
                     fprintf(f1, "(many) ");
                fprintf(f1,  " of type %s\n",pl[j].getType().getName());
            }
        }

        fprintf(f1,"*******************************END TYPES******************\n");
    
        fclose(f1);

        if (!comparefiles(name1,name3)) return 0;

    }
    

    if (xml == 0 || strlen(xml) == 0) return 1;


    FILE *f2 = fopen(name2,"w+");
    if (f2 == 0)
    {
        if (!silent)cout << "Unable to open " << name2 << endl;
        return 0;
    }

    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
 
    XMLDocumentPtr doc = xmh->loadFile(xml);

    if ((i = xmh->getErrorCount()) > 0)
    {
        if (!silent)
        {
            cout << "TestAny XML found errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xmh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        return 0;
    }
    else 
    {
        DataObjectPtr dob = doc->getRootDataObject();
        printDataObject(f2, dob);
    }

    fclose(f2);
    return comparefiles(name2,name4);

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in TestAnyTwo" << e << endl;
        return 0;
    }
}

int sdotest::openseq()

{

    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    FILE *f = fopen("openseq.dat","w+");
    if (f == 0)
    {
        if (!silent)cout << "Unable to open openseq.dat" << endl;
        return 0;
    }


    mdg->addType("companyNS","CompanyType");
    // employee will be an open sequenced type...
    mdg->addType("companyNS","EmployeeType", /*seq*/true, 
                                             /*open*/true, 
                                             /*abs */ false, 
                                             /*data*/ false);


    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);

    mdg->addPropertyToType(tcomp,"employees",temp,true);
    
    mdg->addPropertyToType(temp, "name",tstring);
 
    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","ACME");

    DataObjectPtr emp = mdg->create(temp);

    emp->setCString("name","Albert");
 
    emp->setCString("openstring","Value Of Open String");

    /* now do the same with the employee seq...*/

    SequencePtr seq = emp->getSequence();

    seq->addCString("opensequencedstring","Value of sequenced string");

    const char* c = emp->getCString("openstring");

    fprintf(f, "Open Type string value: %s\n",c);

    c = emp->getCString("opensequencedstring");

    fprintf(f, "Open Sequenced string value: %s\n",c);

    c = seq->getCStringValue(0);

    fprintf(f, "Open Sequenced string value from seq: %s\n",c);

    //cout << "END TEST: OpenSeq ======================================" << endl;
    fclose(f);
    return comparefiles("openseq.dat","openseq.txt");
}

int sdotest::b48601()
{
    return testany("48601.xsd",
        "Schema contains a union which is not yet implemented",0,0);
}

int sdotest::b48686()
{
    return testany("48686.xsd",0,"48686.xml",0);
}

int sdotest::b48736()
{
    return testany("48736.xsd",0,"48736.xml",0);
}


int sdotest::testgenerate(const char* xsd, const char* output)
{

    int i,j;

    try {

        char * name1 = new char[strlen(output) + 5];
        char * name2 = new char[strlen(output) + 5];
        char * c;
        strcpy(name1,output);
        while ((c = strchr(name1,'.')) != 0)*c='_';
        strcpy(name2,name1); 
        strcat(name1,".dat");
        strcat(name2,".txt");


        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    
        if (xsd)
        {
            xsh->defineFile(xsd);

            if ((i = xsh->getErrorCount()) > 0)
            {

                if (!silent)
                {
                    cout << "PROBLEM: generation XSD reported some errors:" << endl;
                    for (j=0;j<i;j++)
                    {
                        const char *m = xsh->getErrorMessage(j);
                        if (m != 0) cout << m;
                        cout << endl;
                    }
                }
                return 0;
            }
            else
            {
                mdg->generateInterface(name1,"Test");
                return comparefiles(name1,name2);
            }
        }
        return 0;
    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "Exception in TestGenerate" << e << endl;
        return 0;
    }
}


int sdotest::emptycs()
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

    // right now, there is no change summary, and logging is
    // off - we expect an empty change summary element

    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    
    xmh->save(doc,"emptycs1.xml");

    if (!comparefiles("emptycs1.xml","emptycs1.txt"))return 0;

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    xmh->save(doc,"emptycs2.xml");

    if (!comparefiles("emptycs2.xml","emptycs2.txt"))return 0;

    // now we expect and empty change summary with logging true

    DataObjectPtr emp4 = mdg->create(temp);
    emp4->setCString("name","Al Smith");
    emp4->setCString("SN","E0004");
    emp4->setBoolean("manager",true);

    // first change - create employee 4
    dol2.append(emp4);

    cs->endLogging();

    //serializeChangeSummary(cs);

    xmh->save(doc,"emptycs3.xml");

    if (!comparefiles("emptycs3.xml","emptycs3.txt"))return 0;

    return 1;

    }
    catch (SDORuntimeException e)
    {
        if (!silent)cout << "empty change summary save failed" << e << endl;
        return 0;
    }
}




