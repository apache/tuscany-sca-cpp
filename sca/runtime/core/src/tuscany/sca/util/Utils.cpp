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

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */
#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#include "tuscany/sca/util/Utils.h"

using namespace std;
using namespace commonj::sdo;
namespace tuscany
{
    namespace sca
    {
        void Utils::tokeniseUri(const string& uri, string& token1, string& token2)
        {
            tokeniseString("/", uri, token1, token2);
        }
    
        void Utils::tokeniseQName(const string& qname, string& uri, string& name)
        {
            tokeniseString("#", qname, uri, name);
            if (name == "")
            {
                name =  uri;
                uri = "";
            }
        }

        void Utils::tokeniseString(
            const string& separator,
            const string& str,
            string& token1,
            string& token2)
        {
            string::size_type sep = str.find(separator);
            if (sep != string::npos)
            {
                token1 = str.substr(0, sep);
                if ( (sep+1) < str.length())
                {
                    token2 = str.substr(sep+1);
                }
                else
                {
                    token2 = "";
                }
            }
            else
            {
                token1 = str;
                token2 = "";
            }
        }

        void Utils::rTokeniseString(
            const string& separator,
            const string& str,
            string& token1,
            string& token2)
        {
            string::size_type sep = str.rfind(separator);
            if (sep != string::npos)
            {
                token1 = str.substr(0, sep);
                if ( (sep+1) < str.length())
                {
                    token2 = str.substr(sep+1);
                }
                else
                {
                    token2 = "";
                }
            }
            else
            {
                token1 = "";
                token2 = str;
            }
        }
        
        void Utils::breakpoint() {
            // dummy method used to set breakpoints
        }
        
        //////////////////////////////////////////////////////////////////////////
        // Print a DatObject tree
        //////////////////////////////////////////////////////////////////////////
        void Utils::tabs(int inc)
        {
            for (int ind=0; ind <inc; ind++)
            {
                cout << "  ";
            }            
        }        
    
        void Utils::printDO(DataObjectPtr dataObject, int increment)
        {
            int inc=increment;
            if (!dataObject)
                return;
            const Type& dataObjectType = dataObject->getType();
            tabs(inc);
            cout << "DataObject type: " << dataObjectType.getURI()<< "#" << dataObjectType.getName() << endl;
            inc++;
            
            //////////////////////////////////////////////////////////////////////////
            // Iterate over all the properties
            //////////////////////////////////////////////////////////////////////////
            PropertyList pl = dataObject->getInstanceProperties();
            if (pl.size() != 0)
            {
                for (int i = 0; i < pl.size(); i++)
                {
                    tabs(inc);
                    cout << "Property: " << pl[i].getName() << endl;
                    
                    const Type& propertyType = pl[i].getType();
                    
                    tabs(inc);
                    cout << "Property Type: " << propertyType.getURI()<< "#" << propertyType.getName() << endl;
                    
                    if (dataObject->isSet(pl[i]))
                    {
                        
                        //////////////////////////////////////////////////////////////////////
                        // For a many-valued property get the list of values
                        //////////////////////////////////////////////////////////////////////
                        if (pl[i].isMany())
                        {
                            inc++;
                            DataObjectList& dol = dataObject->getList(pl[i]);
                            for (int j = 0; j <dol.size(); j++)
                            {
                                tabs(inc);
                                cout << "Value " << j <<endl;
                                inc++;
                                
                                if (propertyType.isDataType())
                                {
                                    tabs(inc);
                                    cout<< "Property Value: " << dol.getCString(j) <<endl ; 
                                }
                                else
                                    printDO(dol[j], inc);
                                inc--;
                            }
                            inc--;
                        } // end IsMany
                        
                        
                        //////////////////////////////////////////////////////////////////////
                        // For a primitive data type print the value
                        //////////////////////////////////////////////////////////////////////
                        else if (propertyType.isDataType())
                        {
                            tabs(inc);
                            cout<< "Property Value: " << dataObject->getCString(pl[i]) <<endl ; 
                        }
                        
                        //////////////////////////////////////////////////////////////////////
                        // For a dataobject print the do
                        //////////////////////////////////////////////////////////////////////
                        else
                        {
                            inc++;
                            printDO(dataObject->getDataObject(pl[i]), inc);
                            inc--;
                        }
                    }
                    else
                    {
                        tabs(inc);
                        cout<< "Property Value: not set" <<endl ; 
                    }
                    
                }
            }
            else
            {
                // Print elements under an open DataObject 
                if(dataObject->getType().isOpenType() && dataObject->getType().isDataObjectType())
                {
                    SequencePtr sequence = dataObject->getSequence();
                    if (sequence != NULL)
                    {
                        for (int i = 0; i < sequence->size(); i++)
                        {
                            if (sequence->isText(i))
                            {
                                tabs(inc);
                                cout<< "Text Value: " << sequence->getCStringValue(i) <<endl ; 
                            }
                            else {
                                const Property& p = sequence->getProperty(i);
                                
                                tabs(inc);
                                cout << "Property: " << p.getName() << endl;
                                
                                const Type& propertyType = p.getType();
                                
                                tabs(inc);
                                cout << "Property Type: " << propertyType.getURI()<< "#" << propertyType.getName() << endl;
                                
                                if (dataObject->isSet(p))
                                {
                                    
                                    //////////////////////////////////////////////////////////////////////
                                    // For a many-valued property get the list of values
                                    //////////////////////////////////////////////////////////////////////
                                    if (p.isMany())
                                    {
                                        inc++;
                                        DataObjectList& dol = dataObject->getList(p);
                                        for (int j = 0; j <dol.size(); j++)
                                        {
                                            tabs(inc);
                                            cout << "Value " << j <<endl;
                                            inc++;
                                            
                                            if (propertyType.isDataType())
                                            {
                                                tabs(inc);
                                                cout<< "Property Value: " << dol.getCString(j) <<endl ; 
                                            }
                                            else
                                                printDO(dol[j], inc);
                                            inc--;
                                        }
                                        inc--;
                                    } // end IsMany
                                    
                                    
                                    //////////////////////////////////////////////////////////////////////
                                    // For a primitive data type print the value
                                    //////////////////////////////////////////////////////////////////////
                                    else if (propertyType.isDataType())
                                    {
                                        tabs(inc);
                                        cout<< "Property Value: " << dataObject->getCString(p) <<endl ; 
                                    }
                                    
                                    //////////////////////////////////////////////////////////////////////
                                    // For a dataobject print the do
                                    //////////////////////////////////////////////////////////////////////
                                    else
                                    {
                                        inc++;
                                        printDO(dataObject->getDataObject(p), inc);
                                        inc--;
                                    }
                                }
                                else
                                {
                                    tabs(inc);
                                    cout<< "Property Value: not set" <<endl ; 
                                }
                            }
                        }
                    }
                }
            }
            inc--;
        }
        
        void Utils::printTypes(DataFactoryPtr df) 
        {
            //////////////////////////////////////////////////////////////////////////
            // Retrieve the DataFactory from the mediator
            // get the list of Types in the DataFactory and list them
            //////////////////////////////////////////////////////////////////////////
            TypeList tl = df->getTypes();
            for (int i = 0; i < tl.size(); i++)
            {
                cout << "Type: " << tl[i].getURI()<< "#" << tl[i].getName() << endl;
                PropertyList pl = tl[i].getProperties();
                for (int j = 0; j < pl.size(); j++)
                {
                    cout << "\tProperty: " << pl[j].getName()
                        << " type: " <<pl[j].getType().getURI()<<"#"<<pl[j].getType().getName()<< endl;
                    
                }
            }
            
        }
        
        void Utils::printType(const Type& type, int increment) 
        {
            int inc = increment;
            tabs(inc);
            cout << "Type: " << type.getURI()<< "#" << type.getName() << endl;
            inc++;
            PropertyList pl = type.getProperties();
            for (int j = 0; j < pl.size(); j++)
            {
                tabs(inc);
                cout << "\tProperty: " << pl[j].getName()
                    << " type: " <<pl[j].getType().getURI()<<"#"<<pl[j].getType().getName()<< endl;
                inc++;
                printType(pl[j].getType(), inc);
                inc--;
            }
        }
        
    } // End namespace sca
} // End namespace tuscany

