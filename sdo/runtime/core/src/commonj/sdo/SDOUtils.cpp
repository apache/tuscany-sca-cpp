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

/* $Rev$ $Date: 2005/12/22 16:54:15 $ */

#include "commonj/sdo/SDOUtils.h"

using namespace std;

//
// Utility methods to print a DataObject tree
//

namespace commonj {
    namespace sdo {



//////////////////////////////////////////////////////////////////////////
// Print Tabs
//////////////////////////////////////////////////////////////////////////

        void SDOUtils::printTabs(unsigned int incr)
        {
            for (int ind=0; ind < incr; ind++)
            {
                cout << "  ";
            }
        }

//////////////////////////////////////////////////////////////////////////
// Print a DatObject tree
//////////////////////////////////////////////////////////////////////////

        void SDOUtils::printDataObject(DataObjectPtr dataObject)
        {
            printDataObject(dataObject,0);
        }


        void SDOUtils::printDataObject(DataObjectPtr dataObject,
            unsigned int incr)
        {
    
            cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> start of DO" 
                 << endl;
    
            if (!dataObject)return;

            const Type& dataObjectType = dataObject->getType();
            printTabs(incr);
            cout << "DataObject type: " 
                 << dataObjectType.getURI() 
                 << "#" << dataObjectType.getName() << endl;
    
            incr++;
    
            //////////////////////////////////////////////////////////////
            // Iterate over all the properties
            //////////////////////////////////////////////////////////////
            PropertyList pl = dataObject->getInstanceProperties();
            for (int i = 0; i < pl.size(); i++)
            {
                printTabs(incr);
                cout << "Property: " << pl[i].getName() << endl;
        
                const Type& propertyType = pl[i].getType();
        
                printTabs(incr);

                cout << "Property Type: " 
                     << propertyType.getURI() 
                     << "#" << propertyType.getName() << endl;
        
                if (dataObject->isSet(pl[i]))
                {
            
                    ///////////////////////////////////////////////////////////
                    // For a many-valued property get the list of values
                    ///////////////////////////////////////////////////////////
                    if (pl[i].isMany())
                    {
                        incr++;
                        DataObjectList& dol = dataObject->getList(pl[i]);
                        for (int j = 0; j <dol.size(); j++)
                        {
                            printTabs(incr);
                            cout << "Value " << j <<endl;
                            incr++;
                            printDataObject(dol[j],incr);
                            incr--;
                            cout << endl;
                        }
                        incr--;
                    } // end IsMany
        
                    //////////////////////////////////////////////////////////////////////
                    // For a primitive data type print the value
                    //////////////////////////////////////////////////////////////////////
                    else if (propertyType.isDataType())
                    {
                        printTabs(incr);
                        cout<< "Property Value: " 
                            << dataObject->getCString(pl[i]) <<endl ; 
                    }
            
                    //////////////////////////////////////////////////////////////////////
                    // For a dataobject print the do
                    //////////////////////////////////////////////////////////////////////
                    else
                    {
                        incr++;
                        printDataObject(dataObject->getDataObject(pl[i]),incr);
                        incr--;
                    }
                }
                else
                {
                    printTabs(incr);
                    cout << "Property Value: not set" <<endl ; 
                }
        
            }
            incr--;
            cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< end of do" << endl;
        }
    };
};
