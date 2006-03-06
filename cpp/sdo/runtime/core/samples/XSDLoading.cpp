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


void XSDLoading::sample()
{
    int i,j;

    cout << " ********** XSDLoading Sample **************" << endl;
    try 
    {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        /**
         * Get an XSD helper to load XSD information into the
         * data factory
         */

        XSDHelperPtr myXSDHelper = HelperProvider::getXSDHelper(mdg);
        myXSDHelper->defineFile("XSDLoading.xsd");

        /**
         * Check if there were any errors. The parse may still
         * succeed, but errors indicate some elements were not
         * understood 
         */

        if ((i = myXSDHelper->getErrorCount()) > 0)
        {
            cout << "XSD Loading reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = myXSDHelper->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }

        /** 
         * Use the same data factory to load XML corresponding to
         * data objects adhering to the previously loaded schema
         */

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("XSDLoading.xml", "companyNS");
    
       /**
         * Check if there were any errors. The parse may still
         * succeed, but errors indicate some elements did not match
         * the schema, or were malformed.
         * 
         */

        if ((i = myXMLHelper->getErrorCount()) > 0)
        {
            cout << "XML Loading reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = myXMLHelper->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }

        DataObjectPtr newdob = myXMLDocument->getRootDataObject();

        cout << "Company Name:" << newdob->getCString("name") << endl;

        DataObjectList& depts = newdob->getList("departments");
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
        cout << "Exception in XSD Loading test" <<  endl;
        cout << e;
    }
    cout << " ********** Sample ends ********************" << endl;
}
