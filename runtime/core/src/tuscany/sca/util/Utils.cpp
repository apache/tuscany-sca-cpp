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

/* $Rev$ $Date$ */

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#endif

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h> 
#else
#include <errno.h>
#endif

#include "tuscany/sca/util/Utils.h"

using namespace std;
using namespace commonj::sdo;

namespace tuscany
{
    namespace sca
    {
        namespace util
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
                    int l = separator.length();
                    token1 = str.substr(0, sep);
                    if ( (sep+l) < str.length())
                    {
                        token2 = str.substr(sep+l);
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
                    int l = separator.length();
                    token1 = str.substr(0, sep);
                    if ( (sep+l) < str.length())
                    {
                        token2 = str.substr(sep+l);
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
    
            const bool Utils::compareProperties(DataObjectPtr dataObject1, const Property& prop1, DataObjectPtr dataObject2, const Property& prop2, string& diff)
            {            
                if(strcmp(prop1.getName(),prop2.getName()) != 0)
                {
                    diff.append("Differing names for Properties:\n");
                    diff.append(prop1.getName());
                    diff.append("\n");
                    diff.append(prop2.getName());                        
                    return false;
                }
    
                const Type& propertyType1 = prop1.getType();
                const Type& propertyType2 = prop2.getType();
    
                if(strcmp(propertyType1.getName(), propertyType2.getName()) != 0 ||
                   strcmp(propertyType1.getURI(),propertyType2.getURI()) != 0 )
                {
                    diff.append("Differing types for Properties:\n");
                    diff.append(propertyType1.getName());
                    diff.append("#");
                    diff.append(propertyType1.getURI());
                    diff.append("\n");
                    diff.append(propertyType2.getName());
                    diff.append("#");
                    diff.append(propertyType2.getURI());
                    return false;
                }
                if (dataObject1->isSet(prop1) != dataObject2->isSet(prop2))
                {
                    diff.append("Property ");
                    diff.append(prop1.getName());
                    diff.append(" is set on one DataObject but not the other"); 
                    return false;
                }
                                    
                if (dataObject1->isSet(prop1))
                {
                    
                    if (prop1.isMany() != prop2.isMany())
                    {
                        diff.append("Property ");
                        diff.append(prop1.getName());
                        diff.append(" is many on one DataObject but not the other"); 
                        return false;
                    }
                    if (propertyType1.isDataType() != propertyType2.isDataType())
                    {
                        diff.append("Property ");
                        diff.append(prop1.getName());
                        diff.append(" is dataType on one DataObject but not the other"); 
                        return false;
                    }
    
                    //////////////////////////////////////////////////////////////////////
                    // For a many-valued property get the list of values
                    //////////////////////////////////////////////////////////////////////
                    if (prop1.isMany())
                    {
                        DataObjectList& dol1 = dataObject1->getList(prop1);
                        DataObjectList& dol2 = dataObject2->getList(prop2);
                        if (dol1.size() != dol2.size())
                        {
                            diff.append("Property ");
                            diff.append(prop1.getName());
                            diff.append(" is many but has differing number of elements"); 
                            return false;
                        }
    
                        for (unsigned int j = 0; j <dol1.size(); j++)
                        {                               
    
                            if (propertyType1.isDataType())
                            {
                                if( strcmp(dol1.getCString(j), dol2.getCString(j)) != 0)
                                {
                                    diff.append("Differing value for Property ");
                                    diff.append(prop1.getName());
                                    diff.append("[");                               
                                    diff += ((int)j);
                                    diff.append("]:\n");
                                    diff.append(dol1.getCString(j));
                                    diff.append("\n");
                                    diff.append(dol2.getCString(j));
                                    return false;
                                }
                            }
                            else
                            {
                                if(!compareDataObjects(dol1[j], dol2[j], diff))
                                {
                                    return false;
                                }
                            }
                        }
                    } // end IsMany
                    
                    
                    //////////////////////////////////////////////////////////////////////
                    // For a primitive data type compare the values
                    //////////////////////////////////////////////////////////////////////
                    else if (propertyType1.isDataType())
                    {
                        if( strcmp(dataObject1->getCString(prop1), dataObject2->getCString(prop2)) != 0)
                        {
                            diff.append("Differing value for Property ");
                            diff.append(prop1.getName());
                            diff.append(":\n");
                            diff.append(dataObject1->getCString(prop1));
                            diff.append("\n");
                            diff.append(dataObject2->getCString(prop2));
                            return false;
                        }
                    }
                    
                    //////////////////////////////////////////////////////////////////////
                    // For a dataobject compare the DOs
                    //////////////////////////////////////////////////////////////////////
                    else
                    {
                        if(!compareDataObjects(dataObject1->getDataObject(prop1), dataObject2->getDataObject(prop2), diff))
                        {
                            return false;
                        }
                    }
                }                    
                return true;
            }
    
            const bool Utils::compareDataObjects(DataObjectPtr dataObject1, DataObjectPtr dataObject2, string& diff)
            {
                if (!dataObject1 || !dataObject2)
                {
                    diff.append("Cannot compare null DataObjects");
                    return false;
                }
    
                const Type& dataObject1Type = dataObject1->getType();
                const Type& dataObject2Type = dataObject2->getType();
    
                if( strcmp(dataObject1Type.getURI(), dataObject2Type.getURI()) != 0 ||
                    strcmp(dataObject1Type.getName(), dataObject2Type.getName()) != 0 )
                {
                    diff.append("DataObject Types differ:\n");
                    diff.append(dataObject1Type.getURI());
                    diff.append("#");
                    diff.append(dataObject1Type.getName());
                    diff.append("\n");
                    diff.append(dataObject2Type.getURI());
                    diff.append("#");
                    diff.append(dataObject2Type.getName());
                    return false;
                }
                
                //////////////////////////////////////////////////////////////////////////
                // Iterate over all the properties
                //////////////////////////////////////////////////////////////////////////
                PropertyList pl1 = dataObject1->getInstanceProperties();
                PropertyList pl2 = dataObject2->getInstanceProperties();
                if (pl1.size() != pl2.size())
                {
                    diff.append("Differing number of properties");
                    return false;
                }
    
                if (pl1.size() != 0)
                {
                    for (unsigned int i = 0; i < pl1.size(); i++)
                    {
                        if(!compareProperties(dataObject1, pl1[i], dataObject2, pl2[i], diff))
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    if(dataObject1->getType().isOpenType() != dataObject2->getType().isOpenType() && 
                       dataObject1->getType().isDataObjectType() != dataObject2->getType().isDataObjectType())
                    {
                        diff.append("DataObject is open & DO type on one but not the other"); 
                        return false;
                    }
    
                    // Compare elements under an open DataObject 
                    if(dataObject1->getType().isOpenType() && dataObject1->getType().isDataObjectType())
                    {
                        SequencePtr sequence1 = dataObject1->getSequence();
                        SequencePtr sequence2 = dataObject2->getSequence();
    
                        if (sequence1 != NULL && sequence2 != NULL)
                        {
                            if (sequence1->size() != sequence1->size())
                            {
                                diff.append("Open DataObjects have differing number of elements"); 
                                return false;
                            }
    
                            for (unsigned int i = 0; i < sequence1->size(); i++)
                            {
                                if (sequence1->isText(i) != sequence2->isText(i))
                                {
                                    diff.append("Open DataObjects have differing element types at position "); 
                                    diff += ((int) i);
                                    return false;
                                }
                                if (sequence1->isText(i))
                                {                                
                                    if( strcmp(sequence1->getCStringValue(i), sequence2->getCStringValue(i)) != 0)
                                    {
                                        diff.append("Differing value for element at position ");
                                        diff += ((int) i);
                                        diff.append(":\n");
                                        diff.append(sequence1->getCStringValue(i));
                                        diff.append("\n");
                                        diff.append(sequence2->getCStringValue(i));
                                        return false;
                                    }
                                }
                                else 
                                {
                                    const Property& p1 = sequence1->getProperty(i);
                                    const Property& p2 = sequence2->getProperty(i);
    
                                    if(!compareProperties(dataObject1, p1, dataObject2, p2, diff))
                                    {
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                }
    
                return true;
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
                    for (unsigned int i = 0; i < pl.size(); i++)
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
                                for (unsigned int j = 0; j <dol.size(); j++)
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
                            for (unsigned int i = 0; i < sequence->size(); i++)
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
                                            for (unsigned int j = 0; j <dol.size(); j++)
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
                for (unsigned int i = 0; i < tl.size(); i++)
                {
                    cout << "Type: " << tl[i].getURI()<< "#" << tl[i].getName() << endl;
                    PropertyList pl = tl[i].getProperties();
                    for (unsigned int j = 0; j < pl.size(); j++)
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
                for (unsigned int j = 0; j < pl.size(); j++)
                {
                    tabs(inc);
                    cout << "\tProperty: " << pl[j].getName()
                        << " type: " <<pl[j].getType().getURI()<<"#"<<pl[j].getType().getName()<< endl;
                    inc++;
                    printType(pl[j].getType(), inc);
                    inc--;
                }
            }

            void Utils::printLastError(ostream& os)
            {
#if defined(WIN32)  || defined (_WINDOWS)
                DWORD err = GetLastError();
                LPTSTR buf;
                FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    err,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &buf,
                    0, NULL );

                os << (const char *)buf;
                LocalFree(buf);
#else
#endif
            }
        
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany

