/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 *   
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "interop.h"

using namespace std;

/*
 * Ensure that SDO can read an XML file and 
 * write it back out again without changing the XML. The 
 * test XML files can be found in tuscany/interop/src/main/resources
 */ 
void Interop::test1ReadAndWriteXML(char *commonDir, char *filename)
{
    int i,j;

	cout << " \nC++ interop test1 - read and write XML: " << filename << endl;
    try 
    {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        /**
         * Get an XSD helper to load XSD information into the
         * data factory
         */
        XSDHelperPtr myXSDHelper = HelperProvider::getXSDHelper(mdg);
		char fullxsdfilename[200];
		strcpy (fullxsdfilename, commonDir );
		strcat (fullxsdfilename, filename );
		strcat (fullxsdfilename,".xsd");
        cout << "Read the XSD file " << fullxsdfilename <<  endl;
		myXSDHelper->defineFile(fullxsdfilename);

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
		char fullxmlfilename[200];
		strcpy (fullxmlfilename, commonDir );
		strcat (fullxmlfilename, filename );
		strcat(fullxmlfilename,"-in.xml");
        cout << "Read the XML file " << fullxmlfilename << endl;
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile(fullxmlfilename, "http://www.apache.org/tuscany/interop");
    
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
		cout << "Get current state of DataObject\n";
		DataObjectPtr dc = newdob->getContainer();

		if ( !dc )
		{
			cout << "There is no container" << endl;
		}
		else
		{
			cout <<" Container of type " << dc->getType().getName();
		}

		PropertyList pl = newdob->getInstanceProperties();
/*
		// Print out the top level elements of the document
		for ( int i=0; i < pl.size(); i++ )
		{
			const Property& p = pl[i];
			if ( newdob->isSet(p))
			{
				if ( p.getType().isDataType() )
				{
					cout << "isDataType = TRUE \n";
					cout << p.getName() << "=" << newdob->getCString(i) << endl;
				}
				else
				{
					cout << "isDataType = FALSE \n";
					if ( p.isMany )
					{
						cout << "isMany = TRUE \n";
						DataObjectList& dol= newdob->getList (i);

						for ( int i=0; i < dol.size(); i++ )
						{
							cout << p.getName() << "[" << i << "] " << "=" << dol[i] << endl;
						}
					}
					else
					{
						cout << "isMany = FALSE \n";
						cout << p.getName() << "=" << newdob->getDataObject(i) << endl;
					}
				}
			}
		}
*/
		// write the XML element back out to a file
		char outxmlfilename[200];
		strcpy (outxmlfilename, filename );
		strcat (outxmlfilename, "-cpp-out.xml" );
		cout << "Write the XML file " << outxmlfilename << endl;
		myXMLHelper->save(myXMLDocument, outxmlfilename);

    }
    catch (SDORuntimeException e)
    {
        cout << "Exception in interop test" <<  endl;
        cout << e.getMessageText();
		cout << e;
    }
}

/*
 * Ensure that SDO can read an XSD file and 
 * write it back out again without changing the XSD. The 
 * test XML files can be found in tuscany/interop/src/main/resources
 */ 
void Interop::test4ReadAndWriteXSD(char *commonDir, char *filename)
{
        /**
         * Get an XSD helper to load XSD information into the
         * data factory
         */

		DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr myXSDHelper = HelperProvider::getXSDHelper(mdg);

		char fullxsdfilename[200];
		strcpy (fullxsdfilename, commonDir );
		strcat (fullxsdfilename, filename );
		strcat (fullxsdfilename,".xsd");
	    cout << "Read the XSD file " << fullxsdfilename << endl;
		const char * URIOfRootType = myXSDHelper->defineFile(fullxsdfilename);

        cout << "URI of root type" << URIOfRootType << endl;

		/**
		 * Now write the XSD back again
		 */
		DataFactoryPtr df = myXSDHelper->getDataFactory();
		TypeList typeList = df->getTypes();

	    char outxsdfilename[200];
		strcpy (outxsdfilename, filename );
		strcat(outxsdfilename,"-cpp-out.xsd");
		cout << "Write an XSD back out " << outxsdfilename << endl;
		myXSDHelper->generateFile(typeList, outxsdfilename);

}
