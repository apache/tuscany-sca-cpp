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

/* $Rev$ $Date: 2005/12/22 16:54:31 $ */

#include <stdio.h>

#pragma warning(disable:4786)

#include <iostream>
using namespace std;

// #include "TypeImpl.h"

#include "sdotest.h"



using namespace commonj::sdo;

void sdotest::eqhelpertest()
{

    // copy and equality helpers

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","Assembly");
    mdg->addType("myspace","SubAssembly");
    mdg->addType("myspace","Part");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tint     = mdg->getType("commonj.sdo","Integer");
    const Type& tassy    = mdg->getType("myspace","Assembly");
    const Type& tsubassy =  mdg->getType("myspace","SubAssembly");
    const Type& tpart    = mdg->getType("myspace","Part");

    
    mdg->addPropertyToType(tassy,"name",tstring);
    mdg->addPropertyToType(tsubassy,"name",tstring);
    mdg->addPropertyToType(tpart,"name",tstring);

    mdg->addPropertyToType(tassy,"number",tint);
    mdg->addPropertyToType(tsubassy,"number",tint);
    mdg->addPropertyToType(tpart,"number",tint);

    mdg->addPropertyToType(tassy,"subassemblies", tsubassy,true);
    mdg->addPropertyToType(tsubassy,"parts", tpart,true);


    // assy 1 

    DataObjectPtr assy1 = mdg->create((Type&)tassy);

    DataObjectPtr subassy1 = assy1->createDataObject("subassemblies");
    DataObjectPtr subassy2 = assy1->createDataObject("subassemblies");

    DataObjectPtr part1 = subassy1->createDataObject("parts");
    DataObjectPtr part2 = subassy1->createDataObject("parts");
    DataObjectPtr part3 = subassy1->createDataObject("parts");

    DataObjectPtr part4 = subassy2->createDataObject("parts");
    DataObjectPtr part5 = subassy2->createDataObject("parts");
    DataObjectPtr part6 = subassy2->createDataObject("parts");


    assy1->setCString("name","MicroFrame");
    assy1->setInteger("number",32768);

    subassy1->setCString("name","Legs");
    subassy2->setCString("name","Body");
    subassy1->setInteger("number",1);
    subassy2->setInteger("number",2);

    part1->setCString("name","Leg1");
    part2->setCString("name","Leg2");
    part3->setCString("name","Leg3");
    part1->setInteger("number",10001);
    part2->setInteger("number",10002);
    part3->setInteger("number",10003);


    part4->setCString("name","MainStrut");
    part5->setCString("name","Brace1");
    part6->setCString("name","Brace2");
    part4->setInteger("number",20001);
    part5->setInteger("number",20002);
    part6->setInteger("number",20003);

    // assy2 - shallow equal to assy, but not deep equal

    DataObjectPtr assy2 = mdg->create((Type&)tassy);

    DataObjectPtr subassy3 = assy2->createDataObject("subassemblies");
    DataObjectPtr subassy4 = assy2->createDataObject("subassemblies");

    DataObjectPtr part7 = subassy3->createDataObject("parts");
    DataObjectPtr part8 = subassy3->createDataObject("parts");
    DataObjectPtr part9 = subassy3->createDataObject("parts");

    DataObjectPtr part10 = subassy4->createDataObject("parts");
    DataObjectPtr part11 = subassy4->createDataObject("parts");
    DataObjectPtr part12 = subassy4->createDataObject("parts");


    assy2->setCString("name","MicroFrame");
    assy2->setInteger("number",32768);

    subassy3->setCString("name","Legs");
    subassy4->setCString("name","Body");
    subassy3->setInteger("number",1);
    subassy4->setInteger("number",2);

    part7->setCString("name","Leg1");
    part8->setCString("name","Leg2");
    part9->setCString("name","Leg3");
    part7->setInteger("number",10001);
    part8->setInteger("number",10002);
    part9->setInteger("number",10003);


    part10->setCString("name","MainStrut");
    part11->setCString("name","Brace1");
    part12->setCString("name","OddBrace2");
    part10->setInteger("number",20001);
    part11->setInteger("number",20002);
    part12->setInteger("number",20003);

    if (!EqualityHelper::equalShallow(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at first test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 1 passed" << endl;
    }
    if (EqualityHelper::equal(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at second test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 2 passed" << endl;
    }

    // Now alter assy2 to be deep equal...


    part12->setCString("name","Brace2");

    if (!EqualityHelper::equalShallow(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at third test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 3 passed" << endl;
    }
    if (!EqualityHelper::equal(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at fourth test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 4 passed" << endl;
    }

    // now add a part, so its not deep equal again
    DataObjectPtr part13 = subassy4->createDataObject("parts");

    if (!EqualityHelper::equalShallow(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at fifth test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 5 passed" << endl;
    }
    if (EqualityHelper::equal(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at sixth test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 6 passed" << endl;
    }
    
    // now remove that part again...
    DataObjectList& dl = subassy4->getList("parts");
    dl.remove(3);

    // should be deep equal again...

    if (!EqualityHelper::equalShallow(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at seventh test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 7 passed" << endl;
    }
    if (!EqualityHelper::equal(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at eighth test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 8 passed" << endl;
    }
    // now make them not shallow equal
    assy2->setInteger("number",32767);

    if (EqualityHelper::equalShallow(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at nineth test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 9 passed" << endl;
    }
    if (EqualityHelper::equal(assy1, assy2))
    {
        cout << "EQUALITY HELPER TEST - failure at tenth test" << endl;
    }
    else 
    {
        cout << "Equality Helper test 10 passed" << endl;
    }

}

void sdotest::cohelpertest()
{

    // copy and equality helpers

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
 
    mdg->addType("myspace","Assembly");
    mdg->addType("myspace","SubAssembly");
    mdg->addType("myspace","Part");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tint     = mdg->getType("commonj.sdo","Integer");
    const Type& tassy    = mdg->getType("myspace","Assembly");
    const Type& tsubassy =  mdg->getType("myspace","SubAssembly");
    const Type& tpart    = mdg->getType("myspace","Part");

    
    mdg->addPropertyToType(tassy,"name",tstring);
    mdg->addPropertyToType(tsubassy,"name",tstring);
    mdg->addPropertyToType(tpart,"name",tstring);

    mdg->addPropertyToType(tassy,"number",tint);
    mdg->addPropertyToType(tsubassy,"number",tint);
    mdg->addPropertyToType(tpart,"number",tint);

    mdg->addPropertyToType(tassy,"subassemblies", tsubassy,true);
    mdg->addPropertyToType(tsubassy,"parts", tpart,true);


    // assy 1 

    DataObjectPtr assy1 = mdg->create((Type&)tassy);

    DataObjectPtr subassy1 = assy1->createDataObject("subassemblies");
    DataObjectPtr subassy2 = assy1->createDataObject("subassemblies");

    DataObjectPtr part1 = subassy1->createDataObject("parts");
    DataObjectPtr part2 = subassy1->createDataObject("parts");
    DataObjectPtr part3 = subassy1->createDataObject("parts");

    DataObjectPtr part4 = subassy2->createDataObject("parts");
    DataObjectPtr part5 = subassy2->createDataObject("parts");
    DataObjectPtr part6 = subassy2->createDataObject("parts");


    assy1->setCString("name","MicroFrame");
    assy1->setInteger("number",32768);

    subassy1->setCString("name","Legs");
    subassy2->setCString("name","Body");
    subassy1->setInteger("number",1);
    subassy2->setInteger("number",2);

    part1->setCString("name","Leg1");
    part2->setCString("name","Leg2");
    part3->setCString("name","Leg3");
    part1->setInteger("number",10001);
    part2->setInteger("number",10002);
    part3->setInteger("number",10003);


    part4->setCString("name","MainStrut");
    part5->setCString("name","Brace1");
    part6->setCString("name","Brace2");
    part4->setInteger("number",20001);
    part5->setInteger("number",20002);
    part6->setInteger("number",20003);

    // first lets shallow copy assy1:

    DataObjectPtr assy2 = CopyHelper::copyShallow(assy1);

    if (strcmp(assy2->getCString("name"),"MicroFrame"))
    {
        cout << "COPYHELPER shallow copy failed(1)" << endl;
    }
    else if (assy2->getInteger("number") != 32768)
    {
        cout << "COPYHELPER shallow copy 1 failed(2)" << endl;
    }
    else if (assy2->getList("subassemblies").size()  != 0)
    {
        cout << "COPYHELPER shallow copy 1 failed(3)" << endl;
    }
    else if (assy2->getContainer() != 0)
    {
        cout << "COPYHELPER shallow copy failed(4)" << endl;
    }
    else 
    {
        cout << "CopyHelper test1 passed" << endl;
    }

    // now deep copy it

    DataObjectPtr assy3 = CopyHelper::copy(assy1);

    if (!EqualityHelper::equal(assy1, assy3))
    {
        cout << "COPYHELPER deep copy failed(1)" << endl;
    }
    else if (assy3->getContainer() != 0)
    {
        cout << "COPYHELPER deep copy failed(2)" << endl;
    }
    else
    {
        cout << "CopyHelper test 2 passed" << endl;
    }

}


/////////////////////////////////////////////////////////////////////////////
///test code for serialization...........
/////////////////////////////////////////////////////////////////////////////
//
//bool sdotest::serializeOldSingleValues(int indent, ChangeSummaryPtr cs, DataObjectPtr dol)
//{
//    bool onlySingle = true;
//    SettingList& sl = cs->getOldValues(dol);
//    if (sl.size() == 0) 
//    {
//        return onlySingle;
//    }
//
//    for (int k=0;k<indent;k++)cout << "  ";
//
//    for (int j=0;j< sl.size(); j++)
//    {
//        if (sl[j].getProperty().isMany()) 
//        {
//            onlySingle = false;
//            continue;
//        }
//        
//        cout << " " << sl[j].getProperty().getName() << "=";
//
//        switch (sl[j].getProperty().getTypeEnum())
//        {
//            case Type::BooleanType:
//                cout <<  "\"" << sl[j].getBooleanValue() << "\"";
//            break;
//            case Type::ByteType:
//                cout << "\"" << sl[j].getByteValue() << "\"";
//            break;
//            case Type::CharacterType:
//                cout << "\"" << sl[j].getCharacterValue() << "\"";
//            break;
//            case Type::IntegerType: 
//                cout << "\"" << sl[j].getIntegerValue() << "\"";
//            break;
//            case Type::ShortType:
//                cout << "\"" << sl[j].getShortValue() << "\"";
//            break;
//            case Type::DoubleType:
//                cout << "\"" << sl[j].getDoubleValue() << "\"";
//            break;
//            case Type::FloatType:
//                cout << "\"" << sl[j].getFloatValue() << "\"";
//            break;
//            case Type::LongType:
//                cout << "\"!" << sl[j].getIntegerValue() << "\"";
//            break;
//            case Type::DateType:
//                cout << "\"" << sl[j].getDateValue().ascTime() << "\"";
//            break;
//            case Type::BigDecimalType: 
//            case Type::BigIntegerType: 
//            case Type::StringType: 
//            case Type::UriType:
//                cout << "\"" << sl[j].getCStringValue() << "\"";
//                break;
//            case Type::BytesType:
//                cout << "\"" << sl[j].getCStringValue() << "\"";
//            break;
//            case Type::OtherTypes:
//            case Type::DataObjectType:
//            case Type::ChangeSummaryType:
//            {
//                DataObjectPtr dob = sl[j].getDataObjectValue();
//                if (!dob) 
//                {
//                    cout << "";
//                }
//                else 
//                {
//                    if (cs->isDeleted(dob))
//                    {
//                        cout << "\"" << cs->getOldXpath(dob) << "\"";
//                    }
//                    else 
//                    {
//                        cout << "\"" << dob->objectToXPath() << "\"";
//                    }
//                }
//            }
//            break;
//            default:
//            {
//            }
//            break;
//        }
//    }
//    return onlySingle;
//}
//
//
//
//
//void sdotest::serializeOldManyValues(int indent, ChangeSummaryPtr cs, DataObjectPtr dol)
//{
//    SettingList& sl = cs->getOldValues(dol);
//    if (sl.size() == 0) 
//    {
//        return;
//    }
//    for (int j=0;j< sl.size(); j++)
//    {
//        if (!sl[j].getProperty().isMany()) continue;
//        
//        for (int k=0;k<indent;k++)cout << "  ";
//
//        cout << "<" ;
//        cout << sl[j].getProperty().getName() << " ";
//
//        switch (sl[j].getProperty().getTypeEnum())
//        {
//            case Type::BooleanType:
//                cout << " value=\"" << sl[j].getBooleanValue() << "\"";
//            break;
//            case Type::ByteType:
//                cout << " value=\"" << sl[j].getByteValue() << "\"";
//            break;
//            case Type::CharacterType:
//                cout << " value=\"" << sl[j].getCharacterValue() << "\"";
//            break;
//            case Type::IntegerType: 
//                cout << " value=\"" << sl[j].getIntegerValue() << "\"";
//            break;
//            case Type::ShortType:
//                cout << " value=\"" << sl[j].getShortValue() << "\"";
//            break;
//            case Type::DoubleType:
//                cout << " value=\"" << sl[j].getDoubleValue() << "\"";
//            break;
//            case Type::FloatType:
//                cout << " value=\"" << sl[j].getFloatValue() << "\"";
//            break;
//            case Type::LongType:
//                cout << " value=\"" << "!"  << sl[j].getIntegerValue() << "\"";
//            break;
//            case Type::DateType:
//                cout << " value=\"" << sl[j].getDateValue().getTime() << "\"";
//            break;
//            case Type::BigDecimalType: 
//            case Type::BigIntegerType: 
//            case Type::StringType: 
//            case Type::UriType:
//                cout << " value=\"" << sl[j].getCStringValue() << "\"";
//                break;
//            case Type::BytesType:
//                cout << " value=\"" << sl[j].getCStringValue() << "\"";
//            break;
//            case Type::OtherTypes:
//            case Type::DataObjectType:
//            case Type::ChangeSummaryType:
//            {
//                DataObjectPtr dob = sl[j].getDataObjectValue();
//                if (!dob) 
//                {
//                    // what to do?;
//                }
//                else 
//                {
//                    if (cs->isDeleted(dob))
//                    {
//                        // TODO
//                        cout << " sdo:ref=\"" << "deleted" << "\"";
//                    }
//                    else 
//                    {
//                        cout << " sdo:ref=\"" << dob->objectToXPath() << "\"";
//                    }
//                }
//            }
//            break;
//            default:
//            {
//            }
//            break;
//        } // switch
//        cout << "/>" << endl;
//    } // for
//} 
//
//
//
//void sdotest::serializechangesummary(ChangeSummaryPtr cs)
//{
//    changeSummaryHeader(cs);
//
//    ChangedDataObjectList& cdol = cs->getChangedDataObjects();
//
//    for (i=0;i< cdol.size();i++)
//    {
//        if (cs->isModified(cdol[i]))
//        {
//            changeSummaryChange(cs,cdol[i]);
//        }
//    }
//
//    changeSummaryFooter();
//
//}
//
///////////////////////////////////////////////////////////////////////////////
/////end code for serialization...........
///////////////////////////////////////////////////////////////////////////////
//
//


bool sdotest::changeSummaryAttributes(ChangeSummaryPtr cs, DataObjectPtr dol)
{
    bool onlySingle = true;
    SettingList& sl = cs->getOldValues(dol);
    if (sl.size() == 0) 
    {
        return onlySingle;
    }

    for (int j=0;j< sl.size(); j++)
    {
        if (sl[j].getProperty().isMany()) 
        {
            onlySingle = false;
            continue;
        }
        
        cout << " " << sl[j].getProperty().getName() << "=\"";

        if (sl[j].getProperty().getType().isDataType())
        {
            changeSummarySetting(sl[j]);
        }
        else 
        {
            DataObjectPtr dob = sl[j].getDataObjectValue();
            if (dob) 
            {
                if (cs->isDeleted(dob))
                {
                    cout << cs->getOldXpath(dob);
                }
                else 
                {
                    cout << dob->objectToXPath();
                }
            }
        }
        cout << "\" ";
    }
    return onlySingle;
}


void sdotest::changeSummarySetting(Setting& s)
{

    switch (s.getProperty().getTypeEnum())
    {
    case Type::BooleanType:
        cout << s.getCStringValue(); 
    break;
    case Type::ByteType:
        cout << s.getByteValue();
    break;
    case Type::CharacterType:
        cout << s.getCharacterValue();
    break;
    case Type::IntegerType: 
        cout << s.getIntegerValue();
    break;
    case Type::ShortType:
        cout << s.getShortValue();
    break;
    case Type::DoubleType:
        cout << s.getDoubleValue();
    break;
    case Type::FloatType:
        cout << s.getFloatValue();
    break;
    case Type::LongType:
        cout << s.getIntegerValue();
    break;
    case Type::DateType:
        cout << s.getDateValue().getTime(); 
    break;
    case Type::BigDecimalType: 
    case Type::BigIntegerType: 
    case Type::StringType: 
    case Type::UriType:
        cout << s.getCStringValue();
        break;
    case Type::BytesType:
        cout << s.getCStringValue();
    break;
    default:
    break;
    } // switch
}


void sdotest::changeSummaryElements(int indent, ChangeSummaryPtr cs, DataObjectPtr dob)
{
    SettingList& sl = cs->getOldValues(dob);

    if (sl.size() == 0) 
    {
        // there are no setting for this element.
        return;
    }

    for (int j=0;j< sl.size(); j++)
    {
        // single values will have been covered by the attributes.
        if (!sl[j].getProperty().isMany()) continue;

        if (sl[j].getProperty().getType().isDataType())
        {

            // indent the line 
            for (int k=0;k<indent;k++)cout << "  ";

            cout << "<" << sl[j].getProperty().getName() << ">\""; 

            changeSummarySetting(sl[j]);

            cout << "\"</" << sl[j].getProperty().getName() << ">" << endl;
        } // if datatype
        else
        {
            DataObjectPtr dob2 = sl[j].getDataObjectValue();
            if (!dob2) 
            {
                return;
            }
            if (cs->isDeleted(dob2))
            {
                changeSummaryDeletedObject(indent,sl[j].getProperty().getName(), cs,dob2);
            }
            else
            {
                // indent the line 
                for (int k=0;k<indent;k++)cout << "  ";
                cout << "<" << sl[j].getProperty().getName(); 
                cout << " sdo:ref=\"" << dob2->objectToXPath() << "\"/>" << endl;
            }
        } 
    } // for
} 



void sdotest::changeSummaryDeletedObject(int indent, const char* name, ChangeSummaryPtr cs, DataObjectPtr dob)
{

    bool elements = false; // are there any elements to process?
    int ind;

    // indent the line 
    for (ind=0;ind<indent;ind++)cout << "  ";

    cout << "<" << name; 

    SettingList& sl = cs->getOldValues(dob);

    if (sl.size() == 0) 
    {
        // there are no setting for this element.
        cout << "/>";
        return;
    }

    cout << " ";

    // print single valued datatypes as attributes

    for (int j=0;j< sl.size(); j++)
    {
        // single values will have been covered by the attributes.
        if ( sl[j].getProperty().isMany()) 
        {
            elements = true;
            continue;
        }
        if (!sl[j].getProperty().getType().isDataType())
        {
            elements = true;
            continue;
        }

        cout << sl[j].getProperty().getName() << "=\""; 

        changeSummarySetting(sl[j]);

        cout << "\" ";
    } // for attributes

    if (!elements)
    {
        cout << "/>" << endl;
        return;
    }

    // now we are onto the many-values, and dataobject single values.
    for (int k=0;k< sl.size(); k++)
    {
        if ( !sl[k].getProperty().getType().isDataType()) 
        {
            // its a dataobject type
            DataObjectPtr dob2 = sl[k].getDataObjectValue();
            if (!dob2) continue;
            if (!cs->isDeleted(dob2)) continue;
            changeSummaryDeletedObject(indent+1,sl[k].getProperty().getName(),cs,dob2);
        }
        else 
        {
            // could only be many valued data type

            for (ind=0;ind<indent;ind++)cout << "  ";
            cout << "<" << sl[k].getProperty().getName() << ">\""; 

            changeSummarySetting(sl[k]);

            cout << "</" << sl[k].getProperty().getName() << ">" << endl;
        }
    } // for attributes

    // indent the line 
    for (ind=0;ind<indent;ind++)cout << "  ";
    cout << "</" << name << ">" << endl; 
} 



void sdotest::changeSummaryHeader(ChangeSummaryPtr cs)
{
    int i;

    ChangedDataObjectList& cdol = cs->getChangedDataObjects();

    cout << "<changeSummary ";

    if (cs->isLogging())
    {
        cout << " logging=\"true\" ";
    }

    cout << ">" << endl;


    for (i=0;i< cdol.size();i++)
    {
        if (cs->isCreated(cdol[i]) && cdol.getType(i) == ChangedDataObjectList::Change)
        {
            cout << "  <create>";
            // TODO is there an IDREF for this created/deleted object?
            DataObject* temp = cdol[i];
            cout << ((DataObject*)temp)->objectToXPath();
            cout << "</create>" << endl;
        }
        if (cs->isDeleted(cdol[i]) && cdol.getType(i) == ChangedDataObjectList::Delete)
        {
            cout  << "  <delete>";
            // TODO is there an IDREF for this created/deleted object?
            const char* path = cs->getOldXpath(cdol[i]);
            if (path != 0) 
            {
                cout << path;
            }
            cout << "</delete>" << endl;
        }
    }
}

void sdotest::changeSummaryFooter()
{
    cout << "</changeSummary>" << endl;
}


void sdotest::changeSummaryChange(int level, ChangeSummaryPtr cs, DataObjectPtr dob)
{
    DataObject* temp = dob;

    // indent by level.
    for (int l=0;l<level;l++)cout << "  ";

    cout << "<";

    try 
    {
        cout << temp->getContainmentProperty().getName();
    }
    catch (SDORuntimeException e)
    {
        // the object which was changed has no name, 
        // so its the root object of a data object tree.
        cout << "root";
    }

    cout << " ";

    try 
    {
        cout << "sdo:ref=\"" << temp->objectToXPath() << "\" ";
    }
    catch (SDORuntimeException e)
    {
        // TODO there was no xpath - thats not good
    }

    // returns true if there were only single values - attributes only.
    if (changeSummaryAttributes(cs, temp))
    {
        cout << "/>" << endl;
        return;
    }

    // there are some elements
    cout << ">" << endl;


    changeSummaryElements(level+1, cs, temp);


    // indent by level.

    for (int m=0;m<level;m++)cout << "  ";
    cout << "</";

    try 
    {
        cout << temp->getContainmentProperty().getName();
    }
    catch (SDORuntimeException e)
    {
        // the object which was changed has no name, 
        // so its the root object of a data object tree.
        cout << "root";
    }

    cout << ">" << endl;

}



void sdotest::serializeChangeSummary(ChangeSummaryPtr cs)
{
    changeSummaryHeader(cs);

    ChangedDataObjectList& cdol = cs->getChangedDataObjects();

    for (int i=0;i< cdol.size();i++)
    {
        if (cs->isModified(cdol[i]))
        {
            changeSummaryChange(1, cs,cdol[i]);
        }
    }

    changeSummaryFooter();
}


void sdotest::cssave()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    xsh->defineFile("company.xsd");

    //mdg->addType("companyNS","CompanyType");
    //mdg->addType("companyNS","EmployeeType");
    //mdg->addType("companyNS","DepartmentType");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    
    //mdg->addPropertyToType(tcomp,"name",tstring);
    // not containment...
    //mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp, false,false,false);
    //mdg->addPropertyToType(tcomp,"departments",tdept, true);
    //mdg->addPropertyToType(tcomp,"cs",tcs);

    //mdg->addPropertyToType(tdept,"name",tstring);
    //mdg->addPropertyToType(tdept,"location",tstring);
    //mdg->addPropertyToType(tdept,"number",tstring);
    //mdg->addPropertyToType(tdept,"employees",temp,true);
    
    //mdg->addPropertyToType(temp, "name",tstring);
    //mdg->addPropertyToType(temp, "SN",  tstring);
    //mdg->addPropertyToType(temp, "manager",  tbool);


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

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    DataObjectPtr emp4 = mdg->create(temp);
    emp4->setCString("name","Al Smith");
    emp4->setCString("SN","E0004");
    emp4->setBoolean("manager",true);

    // first change - create employee 4
    dol2.append(emp4);

    dol2.remove(1); // element 1  is Mary

    DataObjectPtr emp5 = mdg->create(temp);
    emp5->setCString("name","Bill Withers");
    emp5->setCString("SN","E0005");

    dol2.append(emp5);


    comp->setCString("name","MegaCorp");
    comp->setDataObject("employeeOfTheMonth",emp4);

    // silly test - add mary back again
    dol2.append(emp2);

    // and why not delete her again?
    //dol2.remove(4);



    cs->endLogging();

    //serializeChangeSummary(cs);

    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"cssave-output.xml");
    }
    catch (SDORuntimeException e)
    {
        cout << "CSSAVE FAILED" << e << endl;
    }
}

void sdotest::cssave2()
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
    comp->setCString("name","Eastleigh Borough Council");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","Waste Disposal");
    dept->setCString("location","Botley");
    dept->setCString("number","123");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);
    DataObjectPtr emp4 = mdg->create(temp);
    DataObjectPtr emp5 = mdg->create(temp);

    emp1->setCString("name","Alphonse Dodet");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Bridget Jones");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Colin Thorne");
    emp3->setCString("SN","E0003");

    emp4->setCString("name","Donald Trump");
    emp4->setCString("SN","E0004");
    
    emp5->setCString("name","Eddy the eagle");
    emp5->setCString("SN","E0005");

    DataObjectList& dol2 = dept->getList("employees");
    dol2.append(emp1);
    dol2.append(emp2);
    dol2.append(emp3);
    dol2.append(emp4);
    dol2.append(emp5);

    //comp->setDataObject("employeeOfTheMonth",emp2);

    DataObjectPtr dept2 = mdg->create((Type&)tdept);
    dol.append(dept2);

    dept2->setCString("name","Tax Collection");
    dept2->setCString("location","Winchester");
    dept2->setCString("number","666");

    DataObjectPtr empb1 = mdg->create(temp);
    DataObjectPtr empb2 = mdg->create(temp);
    DataObjectPtr empb3 = mdg->create(temp);
    DataObjectPtr empb4 = mdg->create(temp);
    DataObjectPtr empb5 = mdg->create(temp);

    empb1->setCString("name","Arch Meanie");
    empb1->setCString("SN","D0001");

    empb2->setCString("name","Boris the Spider");
    empb2->setCString("SN","D0002");
    empb2->setBoolean("manager",true);

    empb3->setCString("name","Cash Hoarder");
    empb3->setCString("SN","D0003");

    empb4->setCString("name","Dean Giyatoss");
    empb4->setCString("SN","D0004");
    
    empb5->setCString("name","Ebenezer Scrooge");
    empb5->setCString("SN","D0005");

    DataObjectList& dol3 = dept2->getList("employees");
    dol3.append(empb1);
    dol3.append(empb2);
    dol3.append(empb3);
    dol3.append(empb4);
    dol3.append(empb5);

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    // start by deleting a few employees from dept 1

    dol2.remove(1); // B 
    dol2.remove(1); // C
    dol2.remove(1); // D

    // Now delete the tax collectors as a whole

    dol.remove(1);

    // now perhaps add back one of the employees

    dol2.append(emp3); // C

    comp->setCString("name","MegaCorp");
    
    try {
        comp->setDataObject("employeeOfTheMonth",emp4);
        cout << "THIS ISNT RIGHT - emp4 was nolt in the tree" << endl;
    }
    catch (SDORuntimeException e)
    {
    }

    comp->setDataObject("employeeOfTheMonth",emp5);

    cs->endLogging();


    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"cssave2-output.xml");
    }
    catch (SDORuntimeException e)
    {
        cout << "CSSAVE FAILED" << e << endl;
    }
}

void sdotest::bug45933()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();


    mdg->addType("companyNS","CompanyType");
    mdg->addType("companyNS","WombatType");

    /* Now add the properties to the types...*/

   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& twom     = mdg->getType("companyNS","WombatType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);
    mdg->addPropertyToType(tcomp,"bools",tbool, true);
    mdg->addPropertyToType(tcomp,"woms",twom, true);
    mdg->addPropertyToType(tcomp,"cs",tcs);


    // create a graph, then save it

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","Bug45933");

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    DataObjectList& dol = comp->getList("bools");
    dol.append(false);
    dol.remove(0);
    DataObjectPtr awom = mdg->create((Type&)twom);
    DataObjectList& dol2 = comp->getList("woms");
    dol2.append(awom);
    dol2.remove(0);

    cs->endLogging();

    dumpchangesummary(cs);

    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"bug45933-output.xml");
    }
    catch (SDORuntimeException e)
    {
        cout << "BUG45933 FAILED" << endl << e << endl;
    }
}

void sdotest::b46617()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    //XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    //xsh->defineFile("b46617.xsd");
    mdg->addType("companyNS","CompanyType");
    mdg->addType("companyNS","DepartmentType");
    mdg->addType("companyNS","EmployeeType");


   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    mdg->addPropertyToType(tcomp,"name",tstring);
    mdg->addPropertyToType(tcomp,"departments",tdept,true,false,true);
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp,false,false,false);
    mdg->addPropertyToType(tcomp,"CEO",temp,false,false,true);

    mdg->addPropertyToType(tdept,"employees",temp,true,false,true);
    mdg->addPropertyToType(tdept,"name",tstring,false,false,true);
    mdg->addPropertyToType(tdept,"location",tstring,false,false,true);
    mdg->addPropertyToType(tdept,"number",tstring,false,false,true);

    mdg->addPropertyToType(temp,"name",tstring,false,false,true);
    mdg->addPropertyToType(temp,"SN",tstring,false,false,true);
    mdg->addPropertyToType(temp,"manager",tbool,false,false,true);

    // create a graph, then save it

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","Eastleigh Borough Council");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","Waste Disposal");
    dept->setCString("location","Botley");
    dept->setCString("number","123");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);

    emp1->setCString("name","Alphonse Dodet");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Bridget Jones");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Carl Marx");
    emp3->setCString("SN","E0003");

    comp->setDataObject("CEO",emp2);

    DataObjectList& dol2 = dept->getList("employees");
    dol2.append(emp1);
    //dol2.append(emp2);
    dol2.append(emp3);

    comp->setDataObject("employeeOfTheMonth",emp2);

    XSDHelperPtr xmd = HelperProvider::getXSDHelper(mdg);
    xmd->generateFile(mdg->getTypes(),
                "b46617.xsd","companyNS");


    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"b46617.xml");

    // now try loading the xml, using the same schema, and see if we get the
    // same answer

    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("b46617.xml", "companyNS");
    DataObjectPtr newdob = myXMLDocument->getRootDataObject();
    
    cout << "EOMS name is " 
         << newdob->getDataObject("employeeOfTheMonth")->getCString("name")
         << endl;


    }
    catch (SDORuntimeException e)
    {
        cout << "b46617 failed" << e << endl;
    }
}

void sdotest::b46617b()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    xsh->defineFile("b46617b.xsd");

    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("b46617b.xml", "companyNS");
    DataObjectPtr newdob = myXMLDocument->getRootDataObject();
    
    cout << "EOMS name is " 
         << newdob->getDataObject("employeeOfTheMonth")->getCString("name")
         << endl;


    }
    catch (SDORuntimeException e)
    {
        cout << "b46617b failed" << e << endl;
    }
}

void sdotest::notfound()
{

    DataFactoryPtr mdg  = DataFactory::getDataFactory();
    try {

    XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
    xsh->defineFile("not_present.xsd");
    }
    catch (SDOFileNotFoundException e)
    {
        cout << "NotFound threw correct exception " << endl;
    }

    catch (SDORuntimeException e)
    {
        cout << "NOTFOUND THREW THE WRONG EXCEPTION" << e << endl;
    }

    try {
    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("not-present.xml", "companyNS");

    }
    catch (SDOFileNotFoundException ee)
    {
        cout << "NotFound threw the correct exception" << endl;
    }

    catch (SDORuntimeException e)
    {
        cout << "NOTFOUND THREW THE WRONG EXCEPTION" << e << endl;
    }
}

void sdotest::csload()
{
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("company.xsd");

        cout << "TYPES LOADED FROM COMPANY XSD" << endl;

        TypeList tl = mdg->getTypes();
        for (int i=0;i< tl.size(); i++)
        {
            cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        }

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("cssave-output.xml", "companyNS");
        //myXMLDocument->setRootElementName("company");
        //myXMLHelper->save(myXMLDocument,"csload-output.xml");
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "csload-output.xml");
    }
    catch (SDORuntimeException e)
    {
        cout << "CSLOAD FAILED" << e << endl;
    }

}

void sdotest::testsubsload()
{
    cout << "TEST: TestSubsLoad ==========================================" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("companysubs.xsd");

        //cout << "TYPES LOADED FROM COMPANYSUBS XSD" << endl;

        //TypeList tl = mdg->getTypes();
        //for (int i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        // so now we should be able to put book or a magazine into
        // the publication under company...

        const Type& tstring  = mdg->getType("commonj.sdo","String");
        const Type& tcomp    = mdg->getType("companyNS","CompanyType");
        const Type& book     = mdg->getType("companyNS","BookType");
        const Type& mag      = mdg->getType("companyNS","MagazineType");
        const Type& pub      = mdg->getType("companyNS","PublicationType");

    
        // create a graph, then save it

        DataObjectPtr comp = mdg->create((Type&)tcomp);
        comp->setCString("name","Puflet Publishing");


        DataObjectPtr book1 = mdg->create(book);
        book1->setCString("author","Mr P B Writer");
        // inherted from publication
        book1->setCString("title","Nowhere Man");
        
        DataObjectPtr mag1 = mdg->create(mag);
        mag1->setCString("editor","Mr B Picky");
        // inherited from publication
        mag1->setCString("title","Bionicle Weekly");
    
        DataObjectPtr pub1 = mdg->create(pub);
        pub1->setCString("title","Noddy In Toyland");


        // publication should accept any of them...

        comp->setDataObject("Publication",pub1);
        const Type& tpub1 = comp->getDataObject("Publication")->getType();
        cout << "Publication is of type " << tpub1.getName() << endl;

        comp->setDataObject("Publication",book1);
        const Type& tpub2 = comp->getDataObject("Publication")->getType();
        cout << "Publication is of type " << tpub2.getName() << endl;

        comp->setDataObject("Publication",mag1);
        const Type& tpub3 = comp->getDataObject("Publication")->getType();
        cout << "Publication is of type " << tpub3.getName() << endl;

        // should be able to address publication as book or magazine

        comp->setDataObject("Book",book1);
        const Type& tpub4 = comp->getDataObject("Book")->getType();
        cout << "Book is of type " << tpub4.getName() << endl;

        comp->setDataObject("Magazine",mag1);
        const Type& tpub5 = comp->getDataObject("Magazine")->getType();
        cout << "Magazine is of type " << tpub5.getName() << endl;
    
    }
    catch (SDORuntimeException e)
    {
        cout << "ERROR in TeseSubsLoad" << e << endl;
    }
    cout << "END TEST: TestSubsLoad ======================================" << endl;
}

void sdotest::testSCA()
{
    cout << "TEST: TestSCA ==========================================" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("sca.xsd");
    
    }
    catch (SDORuntimeException e)
    {
        cout << "ERROR in TeseSCA" << e << endl;
    }
    cout << "END TEST: TestSCA ======================================" << endl;
}

void sdotest::testabstract()
{
    cout << "TEST: TestAbstract ==========================================" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("companyabs.xsd");

        //cout << "TYPES LOADED FROM COMPANYABS XSD" << endl;

        //TypeList tl = mdg->getTypes();
        //for (int i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        // so now we should be able to put book or a magazine into
        // the publication under company...

        const Type& tstring  = mdg->getType("commonj.sdo","String");
        const Type& tcomp    = mdg->getType("companyNS","CompanyType");
        const Type& book     = mdg->getType("companyNS","BookType");
        const Type& mag      = mdg->getType("companyNS","MagazineType");
        const Type& pub      = mdg->getType("companyNS","PublicationType");

    
        // create a graph, then save it

        DataObjectPtr comp = mdg->create((Type&)tcomp);
        comp->setCString("name","Puflet Publishing");

        // should be allowed...
        DataObjectPtr book1 = mdg->create(book);
        book1->setCString("author","Mr P B Writer");
        // inherted from publication
        book1->setCString("title","Nowhere Man");
        
        DataObjectPtr mag1 = mdg->create(mag);
        mag1->setCString("editor","Mr B Picky");
        // inherited from publication
        mag1->setCString("title","Bionicle Weekly");
    
        try {
            DataObjectPtr pub1 = mdg->create(pub);
            pub1->setCString("title","Noddy In Toyland");
        }
        catch (SDOUnsupportedOperationException e)
        {
            cout << "Normal unsupportedoperation for creation of abstract type" << endl;
        }


        // publication should accept books and magazines...

        comp->setDataObject("Publication",book1);
        const Type& tpub2 = comp->getDataObject("Publication")->getType();
        cout << "Publication is of type " << tpub2.getName() << endl;

        comp->setDataObject("Publication",mag1);
        const Type& tpub3 = comp->getDataObject("Publication")->getType();
        cout << "Publication is of type " << tpub3.getName() << endl;

        // should be able to address publication as book or magazine

        comp->setDataObject("Book",book1);
        const Type& tpub4 = comp->getDataObject("Book")->getType();
        cout << "Book is of type " << tpub4.getName() << endl;

        comp->setDataObject("Magazine",mag1);
        const Type& tpub5 = comp->getDataObject("Magazine")->getType();
        cout << "Magazine is of type " << tpub5.getName() << endl;
    
    }
    catch (SDORuntimeException e)
    {
        cout << "ERROR in TestAbstract" << e << endl;
    }
    cout << "END TEST: TestAbstract ======================================" << endl;
}

void sdotest::testOpen()
{
    cout << "TEST: TestOpen ==========================================" << endl;
    //try 
    //{

    DataFactoryPtr mdg  = DataFactory::getDataFactory();


    mdg->addType("companyNS","CompanyType");
    // employee will be an open type...
    mdg->addType("companyNS","EmployeeType", /*seq*/false, 
                                             /*open*/true, 
                                             /*abs */ false, 
                                             /*data*/ false);

    mdg->addType("companyNS","DepartmentType");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);
    // not containment...
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp, false,false,false);
    mdg->addPropertyToType(tcomp,"departments",tdept, true);
    mdg->addPropertyToType(tcomp,"cs",tcs);

    mdg->addPropertyToType(tdept,"name",tstring);
    mdg->addPropertyToType(tdept,"location",tstring);
    mdg->addPropertyToType(tdept,"number",tstring);
    mdg->addPropertyToType(tdept,"employees",temp,true);
    
    mdg->addPropertyToType(temp, "name",tstring);
    mdg->addPropertyToType(temp, "SN",  tstring);
    mdg->addPropertyToType(temp, "manager",  tbool);



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

    emp1->setCString("name","Albert");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Boris");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Carl");
    emp3->setCString("SN","E0003");

    // now we should be able to add extra properties to employees,
    // and get them back again...


    emp1->setCString("openstring","Value Of Open String");
    emp1->setBoolean("openboolean",true);

    const char* c = emp1->getCString("openstring");

    cout << "Open Type string value: " << c << endl;

    bool b = emp1->getBoolean("openboolean");

    cout << "Open Type boolean value: " << b << endl;

    // unknown list type at present..
    DataObjectList& dl = emp1->getList("opentypelist");
    
    int i = 45;
    dl.append((short)i); // now the list must be primitive

    // we should now have instance properties 
    PropertyList pl = emp1->getType().getProperties();

    for (i=0;i<pl.size();i++)
    {
        cout << "EMP1 type property: " << pl[i].getName() << "( many? " <<
            pl[i].isMany() << ")" << " of type " <<
            pl[i].getType().getName() << endl;
    }

    PropertyList pli = emp1->getInstanceProperties();

    for (i=0;i<pli.size();i++)
    {
        cout << "EMP1 inst property: " << pli[i].getName() << "( many? " <<
            pli[i].isMany() << ")" << " of type " <<
            pli[i].getType().getName() << endl;
    }

    // now unset them and see them disappear - what happens to the indices?

    emp1->unset("openstring");

    PropertyList plk = emp1->getInstanceProperties();

    for (i=0;i<plk.size();i++)
    {
        cout << "EMP1 inst property: " << plk[i].getName() << "( many? " <<
            plk[i].isMany() << ")" << " of type " <<
            plk[i].getType().getName() << endl;
    }

    emp1->unset("openboolean");

    dl.remove(0); // so is this list gone, or empty - I vote for still there.

    PropertyList pll = emp1->getInstanceProperties();
    for (i=0;i<pll.size();i++)
    {
        cout << "EMP1 inst property: " << pll[i].getName() << "( many? " <<
            pll[i].isMany() << ")" << " of type " <<
            pll[i].getType().getName() << endl;
    }

    emp1->unset("opentypelist");

    // now I vote for gone...

    PropertyList plm = emp1->getInstanceProperties();
    for (i=0;i<plm.size();i++)
    {
        cout << "EMP1 inst property: " << plm[i].getName() << "( many? " <<
            plm[i].isMany() << ")" << " of type " <<
            plm[i].getType().getName() << endl;
    }

    //}
    //catch (SDORuntimeException e)
    //{
    //    cout << "ERROR in TestOpen" << e << endl;
    //}
    cout << "END TEST: TestOpen ======================================" << endl;
}

void sdotest::saveOpen()
{
    cout << "TEST: SaveOpen ==========================================" << endl;
    try 
    {

    DataFactoryPtr mdg  = DataFactory::getDataFactory();


    mdg->addType("companyNS","CompanyType");
    // employee will be an open type...
    mdg->addType("companyNS","EmployeeType", /*seq*/false, 
                                             /*open*/true, 
                                             /*abs */ false, 
                                             /*data*/ false);

    mdg->addType("companyNS","DepartmentType");

    mdg->addType("companyNS","OpenType");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");
    const Type& topen    = mdg->getType("companyNS","OpenType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);
    // not containment...
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp, false,false,false);
    mdg->addPropertyToType(tcomp,"departments",tdept, true);
    mdg->addPropertyToType(tcomp,"cs",tcs);

    mdg->addPropertyToType(tdept,"name",tstring);
    mdg->addPropertyToType(tdept,"location",tstring);
    mdg->addPropertyToType(tdept,"number",tstring);
    mdg->addPropertyToType(tdept,"employees",temp,true);
    
    mdg->addPropertyToType(temp, "name",tstring);
    mdg->addPropertyToType(temp, "SN",  tstring);
    mdg->addPropertyToType(temp, "manager",  tbool);

    mdg->setAlias("companyNS","CompanyType","TheFirm");
    mdg->setAlias("companyNS","CompanyType","departments","TheDepartments");
    mdg->setAlias("companyNS","CompanyType","departments","MYDepartments");
    mdg->setAlias("companyNS","CompanyType","TheBigFirm");

    mdg->addPropertyToType(topen,"name",tstring);

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","ACME");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    ChangeSummaryPtr cs = comp->getChangeSummary();

    cs->beginLogging();

    dept->setCString("name","Advanced Technologies");
    dept->setCString("location","NY");
    dept->setCString("number","123");

    DataObjectList& emps = dept->getList("employees");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);

    emps.append(emp1);
    emps.append(emp2);
    emps.append(emp3);

    emp1->setCString("name","Albert");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Boris");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Carl");
    emp3->setCString("SN","E0003");

    // now we should be able to add extra properties to employees,
    // and get them back again...


    emp1->setCString("openstring","Value Of Open String");
    emp1->setBoolean("openboolean",true);

    const char* c = emp1->getCString("openstring");

    cout << "Open Type string value: " << c << endl;

    bool b = emp1->getBoolean("openboolean");

    cout << "Open Type boolean value: " << b << endl;

    // unknown list type at present..
    DataObjectList& dl = emp1->getList("openintlist");
    
    int i = 45;
    dl.append((short)i); // now the list must be primitive and short!!

    // unknown list type at present..
    DataObjectList& dl2 = emp1->getList("opendataobjectlist");
    
    DataObjectPtr myopen = mdg->create("companyNS","OpenType");
    myopen->setCString("name","MyOpenName");
    dl2.append(myopen); // now the list must be of data objects
    

    // now a single valued data object - should have an xsi:type
    emp1->setDataObject("opendataobject", myopen);


    cs->endLogging();

    dumpchangesummary(cs);

    XSDHelperPtr xdh = HelperProvider::getXSDHelper(mdg);

    xdh->generateFile(mdg->getTypes(),
                "saveopen-output.xsd","companyNS");

    XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);
    XMLDocumentPtr doc = xmh->createDocument(comp,"companyNS","company");
    xmh->save(doc,"saveopen-output.xml");

    }
    catch (SDORuntimeException e)
    {
        cout << "ERROR in SaveOpen" << e << endl;
    }
    cout << "END TEST: SaveOpen ======================================" << endl;
}

void sdotest::loadOpen()
{
    cout << "TEST: LoadOpen ==========================================" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("saveopen-output.xsd");


        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;
        //int i;
        //TypeList tl = mdg->getTypes();
        //for (i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("partial.xml", "companyNS");
        
        

        // so, we now expect that the openboolean and openstring props will
        // be of type boolean and Bytes, and the openlist will be a list of integers.
        // The dataobjectlist an dataobject will be of the correct OpenType

        // we need a test for data objects, and that we are writing out xsi:type
        // correctly.
        DataObjectPtr comp = myXMLDocument->getRootDataObject();
        DataObjectList& dl = comp->getList("departments");
        DataObjectPtr dept = dl[0];
        DataObjectList& dl2 = dept->getList("employees");
        DataObjectPtr emp = dl2[0];

        if (strcmp(emp->getProperty("openboolean").getType().getName(),
            "Boolean"))
        {
            cout << "OpenLoad failure - bool property not boolean" << endl;

        }
        if (emp->getBoolean("openboolean") != true)
        {
            cout << "OpenLoad failure - bool property not true" << endl;
        }

        if (strcmp(emp->getProperty("openstring").getType().getName(),
            "Bytes"))
        {
            cout << "OpenLoad failure - bytes property not bytes" << endl;

        }
        if (strcmp(emp->getCString("openstring"),"Value Of Open String"))
        {
            cout << "OpenLoad failure - string value is" << 
                emp->getCString("openstring") << endl;
        }


        DataObjectList& dl3 = emp->getList("openintlist");

        if (strcmp(emp->getProperty("openintlist").getType().getName(),
            "Short"))
        {
            cout << "OpenLoad failure - IntegerList not Short " <<
                emp->getProperty("openintlist").getType().getName() << endl;
        }

        if (dl3.getInteger(0) != 45)
        {
            cout << "OpenLoad failure - Integer value is" << 
                dl.getInteger(0) << endl;
        }

        if (strcmp(emp->getProperty("opendataobjectlist").getType().getName(),
            "OpenType"))
        {
            cout << "OpenLoad failure - OpenTypeList not OpenType" << endl;
        }

        DataObjectList& dl4 = emp->getList("opendataobjectlist");

        if (strcmp(dl4[0]->getCString("name"),"MyOpenName"))
        {
            cout << "OpenLoad failure - list element name is " <<
                dl4[0]->getCString("name") << endl;
        }

        if (strcmp(emp->getProperty("opendataobject").getType().getName(),
            "OpenType"))
        {
            cout << "OpenLoad failure - OpenType not OpenType" << endl;
        }

        DataObjectPtr dob2 = emp->getDataObject("opendataobject");

        if (strcmp(dob2->getCString("name"),"MyOpenName"))
        {
            cout << "OpenLoad failure - open value name is " <<
                dob2->getCString("name") << endl;
        }

        
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "loadload-output.xml");


    }
    catch (SDORuntimeException e)
    {
        cout << "LoadOpen FAILED" << e << endl;
    }

}

void sdotest::loadOpenNS()
{
    cout << "TEST: LoadOpenNS ==========================================" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("openloadNS.xsd");
        xsh->defineFile("openloadNS2.xsd");


        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;
        //int i;
        //TypeList tl = mdg->getTypes();
        //for (i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("openloadNS.xml", "companyNS");
        
        

        // so, we now expect that the openboolean and openstring props will
        // be of type boolean and Bytes, and the openlist will be a list of integers.
        // The dataobjectlist an dataobject will be of the correct OpenType

        // we need a test for data objects, and that we are writing out xsi:type
        // correctly.
        DataObjectPtr comp = myXMLDocument->getRootDataObject();
        DataObjectList& dl = comp->getList("departments");
        DataObjectPtr dept = dl[0];
        DataObjectList& dl2 = dept->getList("employees");
        DataObjectPtr emp = dl2[0];

        if (strcmp(emp->getProperty("openboolean").getType().getName(),
            "Boolean"))
        {
            cout << "OpenLoadNS failure - bool property not boolean" << endl;

        }
        if (emp->getBoolean("openboolean") != true)
        {
            cout << "OpenLoadNS failure - bool property not true" << endl;
        }

        if (strcmp(emp->getProperty("openstring").getType().getName(),
            "Bytes"))
        {
            cout << "OpenLoadNS failure - bytes property not bytes" << endl;

        }
        if (strcmp(emp->getCString("openstring"),"Value Of Open String"))
        {
            cout << "OpenLoadNS failure - string value is" << 
                emp->getCString("openstring") << endl;
        }


        DataObjectList& dl3 = emp->getList("openintlist");

        if (strcmp(emp->getProperty("openintlist").getType().getName(),
            "Short"))
        {
            cout << "OpenLoadNS failure - IntegerList not Short " <<
                emp->getProperty("openintlist").getType().getName() << endl;
        }

        if (dl3.getInteger(0) != 45)
        {
            cout << "OpenLoadNS failure - Integer value is" << 
                dl.getInteger(0) << endl;
        }


        
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "openloadNSout.xml");


        // now try exactly the same, but with NS which doesnt exist
        XMLDocumentPtr myXMLDocument2 = myXMLHelper->loadFile("openloadNS2.xml", "companyNS");
        
        

        // so, we now expect that the openboolean and openstring props will
        // be of type boolean and Bytes, and the openlist will be a list of integers.
        // The dataobjectlist an dataobject will be of the correct OpenType

        // we need a test for data objects, and that we are writing out xsi:type
        // correctly.
        DataObjectPtr comp2 = myXMLDocument2->getRootDataObject();
        DataObjectList& dld = comp2->getList("departments");
        DataObjectPtr dept2 = dld[0];
        DataObjectList& dl22 = dept2->getList("employees");
        DataObjectPtr emp2 = dl22[0];

        if (strcmp(emp2->getProperty("openboolean").getType().getName(),
            "Bytes"))
        {
            cout << "OpenLoadNS2 failure - bool property not bytes" << endl;

        }
        if (emp2->getBoolean("openboolean") != true)
        {
            cout << "OpenLoadNS2 failure - bool property not true" << endl;
        }

        if (strcmp(emp2->getProperty("openstring").getType().getName(),
            "Bytes"))
        {
            cout << "OpenLoadNS2 failure - bytes property not bytes" << endl;

        }
        if (strcmp(emp2->getCString("openstring"),"Value Of Open String"))
        {
            cout << "OpenLoadNS failure - string value is" << 
                emp2->getCString("openstring") << endl;
        }


        DataObjectList& dl32 = emp2->getList("openintlist");

        if (strcmp(emp2->getProperty("openintlist").getType().getName(),
            "Bytes"))
        {
            cout << "OpenLoadNS2 failure - IntegerList not Bytes " <<
                emp2->getProperty("openintlist").getType().getName() << endl;
        }

        if (dl32.getInteger(0) != 45)
        {
            cout << "OpenLoadNS2 failure - Integer value is" << 
                dl32.getInteger(0) << endl;
        }


    }
    catch (SDORuntimeException e)
    {
        cout << "LoadOpenNS FAILED" << e << endl;
    }

}

void sdotest::loadManyOpen()
{
    cout << "TEST: LoadManyOpen ==========================================" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("many.xsd");


        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;
        //int i;
        //TypeList tl = mdg->getTypes();
        //for (i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("many.xml", "companyNS");
        
        

        // so, we now expect that the openboolean and openstring props will
        // be of type boolean and Bytes, and the openlist will be a list of integers.
        // The dataobjectlist an dataobject will be of the correct OpenType

        // we need a test for data objects, and that we are writing out xsi:type
        // correctly.
        DataObjectPtr comp = myXMLDocument->getRootDataObject();
        DataObjectList& dl = comp->getList("departments");
        DataObjectPtr dept = dl[0];
        DataObjectList& dl2 = dept->getList("employees");
        DataObjectPtr emp = dl2[0];


        DataObjectList& dl3 = emp->getList("openlist");

        if (strcmp(emp->getProperty("openlist").getType().getName(),
            "Bytes"))
        {
            cout << "OpenManyLoad failure - BytesList not Bytes " <<
                emp->getProperty("openlist").getType().getName() << endl;
        }

        if (dl3.size() != 3)
        {
            cout << "OpenManyLoad failure - size is " << 
                dl3.size() << endl;
        }

        for (int i=0;i< dl3.size();i++)
        {
            cout << "Value:" << 
                dl3.getCString(i) << endl;
        }

    }
    catch (SDORuntimeException e)
    {
        cout << "LoadOpen FAILED" << e << endl;
    }

}

void sdotest::csload2()
{
    cout << "TEST: CSLoad2 ===============================================" << endl;
    try 
    {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("company.xsd");

        //cout << "TYPES LOADED FROM COMPANY XSD" << endl;

        //TypeList tl = mdg->getTypes();
        //for (int i=0;i< tl.size(); i++)
        //{
        //    cout << tl[i].getURI() << ":" << tl[i].getName() << endl;
        //}

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("cssave2-output.xml", "companyNS");
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "csload2-output.xml");
        XMLDocumentPtr myXMLDocument2 = myXMLHelper->loadFile("csload2-output.xml", "companyNS");
        myXMLHelper->save(myXMLDocument2->getRootDataObject(),
                          0, "company", "csload3-output.xml");
    }
    catch (SDORuntimeException e)
    {
        cout << "ERROR in CsLoad2" << e << endl;
    }
    cout << "END TEST: CSLoad2 ===========================================" << endl;

}



void sdotest::setnull()
{

//    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    mdg->addType("NullNS","NullMainType");
    mdg->addType("NullNS","NullSubType");

    /* Now add the properties to the types...*/
   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tint     = mdg->getType("commonj.sdo","Integer");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tmain    = mdg->getType("NullNS","NullMainType");
    const Type& tsub     = mdg->getType("NullNS","NullSubType");
 
    
    mdg->addPropertyToType(tmain,"name",tstring);
    mdg->addPropertyToType(tmain,"subs",tsub, true);  // many
    mdg->addPropertyToType(tmain,"asub",tsub, false); // single
    mdg->addPropertyToType(tmain,"bsub",tsub, false); // single

    mdg->addPropertyToType(tmain,"cs",tcs); // change summary

    mdg->addPropertyToType(tsub,"name",tstring);
    mdg->addPropertyToType(tsub,"number",tint);
    mdg->addPropertyToType(tsub,"bool",tbool);


    // create a graph, then save it

    DataObjectPtr main1 = mdg->create((Type&)tmain);
    main1->setCString("name","MainObject");

    // asub is initially set
    DataObjectPtr sub0 = main1->createDataObject("asub");
    // bsub is not set.

    DataObjectPtr sub1 = mdg->create((Type&)tsub);
    DataObjectPtr sub2 = mdg->create((Type&)tsub);
    DataObjectPtr sub3 = mdg->create((Type&)tsub);

    DataObjectList& dol = main1->getList("subs");

    dol.append(sub1);
    dol.append(sub2);
    dol.append(sub3);

    // sub1 has all values set initially

    sub1->setCString("name","All Initially set");
    sub1->setInteger("number",100);
    sub1->setBoolean("bool",false);

    // sub2 has no values set at all


    // sub3 has all values set to null
    sub3->setNull("name");
    sub3->setNull("number");
    sub3->setNull("bool");


    // report

    cout << "is ASUB null? (0 expected) " << 
        main1->isNull("asub") << endl;

    cout << "is ASUB set?  (1 expected) " << 
        main1->isSet("asub") << endl;

    cout << "is BSUB null? (0 expected) " << 
        main1->isNull("bsub") << endl;

    cout << "is BSUB set?  (0 expected) " << 
        main1->isSet("bsub") << endl;

// dol0 is set, not null

    cout << "dol[0] name is null? (0 expected) " <<
        dol[0]->isNull("name") << endl;

    cout << "dol[0] number is null? (0 expected) " <<
        dol[0]->isNull("number") << endl;

    cout << "dol[0] bool is null? (0 expected) " <<
        dol[0]->isNull("bool") << endl;

    cout << "dol[0] name is set? (1 expected) " <<
        dol[0]->isSet("name") << endl;

    cout << "dol[0] number is set? (1 expected) " <<
        dol[0]->isSet("number") << endl;

    cout << "dol[0] bool is set? (1 expected) " <<
        dol[0]->isSet("bool") << endl;

// dol1 is all unset , so no nulls
    
    cout << "dol[1] name is null? (0 expected) " <<
        dol[1]->isNull("name") << endl;

    cout << "dol[1] number is null? (0 expected) " <<
        dol[1]->isNull("number") << endl;

    cout << "dol[1] bool is null? (0 expected) " <<
        dol[1]->isNull("bool") << endl;

    cout << "dol[1] name is set? (0 expected) " <<
        dol[1]->isSet("name") << endl;

    cout << "dol[1] number is set? (0 expected) " <<
        dol[1]->isSet("number") << endl;

    cout << "dol[1] bool is set? (0 expected) " <<
        dol[1]->isSet("bool") << endl;

// dol2 is all nulls, so all set
    
    cout << "dol[2] name is null? (1 expected) " <<
        dol[2]->isNull("name") << endl;

    cout << "dol[2] number is null? (1 expected) " <<
        dol[2]->isNull("number") << endl;

    cout << "dol[2] bool is null? (1 expected) " <<
        dol[2]->isNull("bool") << endl;

    cout << "dol[2] name is set? (1 expected) " <<
        dol[2]->isSet("name") << endl;

    cout << "dol[2] number is set? (1 expected) " <<
        dol[2]->isSet("number") << endl;

    cout << "dol[2] bool is set? (1 expected) " <<
        dol[2]->isSet("bool") << endl;
    
    ChangeSummaryPtr cs = main1->getChangeSummary();

    cs->beginLogging();

    main1->setNull("asub");
    main1->setNull("bsub");

    dol[0]->setNull("name");
    dol[0]->setNull("number");
    dol[0]->setNull("bool");

    dol[1]->setNull("bool");

    dol[2]->setCString("name","NewName");
    dol[2]->setInteger("number",100);
    dol[2]->setBoolean("bool",true);


    cs->endLogging();

    // what would we expect from the change summary?

    cout << "==============Expected in Change Summary=================" << endl;
    cout << "change to main1 - object asub set to null, oldvalue asub (set)" << endl;
    cout << "change to main1 - object bsub set to null, oldvalue null (unset)" << endl;
       cout << "change to sub1  - name asub set to null, oldvalue \"All Initially Set\" (set)" << endl;
       cout << "change to sub1  - number asub set to null, oldvalue 100 (set)" << endl;
       cout << "change to sub1  - bool asub set to null, oldvalue false (set)" << endl;
       cout << "change to sub2  - bool asub set to null, oldvalue false (unset)" << endl;
       cout << "change to sub3  - name asub set to \"NewName\", oldvalue null, set" << endl;
       cout << "change to sub3  - number asub set to 100, oldvalue null, set" << endl;
       cout << "change to sub3  - bool asub set to true, oldvalue null, set" << endl;
    cout << "==============End Expected in Change Summary=============" << endl;


    dumpchangesummary(cs);
 
    // report

    cout << "is ASUB null? (1 expected) " << 
        main1->isNull("asub") << endl;

    cout << "is ASUB set?  (1 expected) " << 
        main1->isSet("asub") << endl;

    cout << "is BSUB null? (1 expected) " << 
        main1->isNull("bsub") << endl;

    cout << "is BSUB set?  (1 expected) " << 
        main1->isSet("bsub") << endl;

// dol0 is set to null

    cout << "dol[0] name is null? (1 expected) " <<
        dol[0]->isNull("name") << endl;

    cout << "dol[0] number is null? (1 expected) " <<
        dol[0]->isNull("number") << endl;

    cout << "dol[0] bool is null? (1 expected) " <<
        dol[0]->isNull("bool") << endl;

    cout << "dol[0] name is set? (1 expected) " <<
        dol[0]->isSet("name") << endl;

    cout << "dol[0] number is set? (1 expected) " <<
        dol[0]->isSet("number") << endl;

    cout << "dol[0] bool is set? (1 expected) " <<
        dol[0]->isSet("bool") << endl;

// dol1 is all unset except bool which is null 
    
    cout << "dol[1] name is null? (0 expected) " <<
        dol[1]->isNull("name") << endl;

    cout << "dol[1] number is null? (0 expected) " <<
        dol[1]->isNull("number") << endl;

    cout << "dol[1] bool is null? (1 expected) " <<
        dol[1]->isNull("bool") << endl;

    cout << "dol[1] name is set? (0 expected) " <<
        dol[1]->isSet("name") << endl;

    cout << "dol[1] number is set? (0 expected) " <<
        dol[1]->isSet("number") << endl;

    cout << "dol[1] bool is set? (1 expected) " <<
        dol[1]->isSet("bool") << endl;

// dol2 is all set non-null, so all set
    
    cout << "dol[2] name is null? (0 expected) " <<
        dol[2]->isNull("name") << endl;

    cout << "dol[2] number is null? (0 expected) " <<
        dol[2]->isNull("number") << endl;

    cout << "dol[2] bool is null? (0 expected) " <<
        dol[2]->isNull("bool") << endl;

    cout << "dol[2] name is set? (1 expected) " <<
        dol[2]->isSet("name") << endl;

    cout << "dol[2] number is set? (1 expected) " <<
        dol[2]->isSet("number") << endl;

    cout << "dol[2] bool is set? (1 expected) " <<
        dol[2]->isSet("bool") << endl;


    // what would we expect from the change summary?
//    }
//    catch (SDORuntimeException e)
//    {
//        cout << "Exception in setnull" << endl;
//        cout << e;
//    }

}

void sdotest::testsubs()
{

    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    mdg->addType("companyNS","MainType");

    mdg->addType("companyNS","SubType");
    mdg->addType("companyNS","SuperType");
    mdg->addType("companyNS","SubstituteType");
    mdg->addType("companyNS","UnrelatedType");

    /* Now add the properties to the types...*/

   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tmain    = mdg->getType("companyNS","MainType");
    const Type& sub      = mdg->getType("companyNS","SubType");
    const Type& super    = mdg->getType("companyNS","SuperType");
    const Type& subst    = mdg->getType("companyNS","SubstituteType");
    const Type& unrel    = mdg->getType("companyNS","UnrelatedType");

    
    mdg->addPropertyToType(tmain,"name",tstring);
    mdg->addPropertyToType(tmain,"sub",super, false);
    mdg->addPropertyToType(tmain,"subs",super, true);


    mdg->setPropertySubstitute(tmain, "sub","wilbur", subst);
    mdg->setPropertySubstitute(tmain, "subs","wilburs", subst);

    mdg->setBaseType(sub,super);

    //now sub and subs should accept all the three types

    DataObjectPtr main = mdg->create((Type&)tmain);
    main->setCString("name","SubstituteTest");

    DataObjectList& dol = main->getList("subs");

    DataObjectPtr sub1 = mdg->create(sub);
    DataObjectPtr super1 = mdg->create(super);
    DataObjectPtr subst1 = mdg->create(subst);
    DataObjectPtr unrel1 = mdg->create(unrel);

    dol.append(sub1);
    dol.append(super1);
    dol.append(subst1);
    try 
    {
        dol.append(unrel1);
    }
    catch (SDOInvalidConversionException e)
    {
        cout << "Normal failure to append wrong type" << endl;
    }

    dol.remove(0);
    dol.remove(0);
    dol.remove(0);

    DataObjectList& dol2 = main->getList("wilburs");

    dol2.append(sub1);
    dol2.append(super1);
    dol2.append(subst1);
    try 
    {
        dol2.append(unrel1);
    }
    catch (SDOInvalidConversionException e)
    {
        cout << "Normal failure to append wrong type" << endl;
    }

    main->setDataObject("sub",sub1);
    main->setDataObject("sub",super1);
    main->setDataObject("sub",subst1);

    main->setDataObject("wilbur",sub1);
    main->setDataObject("wilbur",super1);
    main->setDataObject("wilbur",subst1);

    try 
    {
        main->setDataObject("sub", unrel1);
    }
    catch (SDOInvalidConversionException e)
    {
        cout << "Normal failure to set wrong type" << endl;
    }
    }

    catch (SDORuntimeException e)
    {
        cout << "Substitute test failed" << endl << e << endl;
    }
}


void sdotest::merle1()
{

    try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("axis.xsd");

        // or dynamically create, but that would get attributes for the strings, not elements
        //mdg->addType("axis","ItemSearch",true); // sequenced
        //mdg->addType("axis","ItemSearchRequest",true); // sequenced
    
        const Type& tstring  = mdg->getType("commonj.sdo","String");
        const Type& tsearch  = mdg->getType("axis","ItemSearch");
        const Type& treq     = mdg->getType("axis","ItemSearchRequest");
    
        //mdg->addPropertyToType(tsearch,"SubscriptionId",tstring);
        //mdg->addPropertyToType(tsearch,"AssociateTag",tstring);
        //mdg->addPropertyToType(tsearch,"XMLEscaping",tstring);
        //mdg->addPropertyToType(tsearch,"Validate",tstring);
        //mdg->addPropertyToType(tsearch,"Shared",treq, false, false, true);
        //mdg->addPropertyToType(tsearch,"Request",treq, true, false, true);

        //mdg->addPropertyToType(treq,"Keywords",tstring);
        //mdg->addPropertyToType(treq,"SearchIndex",tstring);

        // dynamic creation of the data 

        DataObjectPtr search = mdg->create((Type&)tsearch);
    
        search->setCString("SubscriptionId","I am the subscription ID string");
        search->setCString("AssociateTag","I am the associate Tag string");
        search->setCString("XMLEscaping","I am the XML escaping string");
        search->setCString("Validate","I am the Validate string");

        DataObjectPtr shared = search->createDataObject("Shared");
        shared->setCString("Keywords","SharedKeywordsString");
        shared->setCString("SearchIndex","SearchIndexString");

        DataObjectList& requests = search->getList("Request");
        DataObjectPtr request1 = mdg->create(treq);
        requests.append(request1);
        request1->setCString("Keywords","RequestKeywords");
        request1->setCString("SearchIndex","RequestSearchIndex");

        // Read the data and put it into XML:

        PropertyList properties = search->getInstanceProperties();

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);

        for (int i=0;i<properties.size();i++)
        {
            Property& prop = properties[i];
            switch (prop.getTypeEnum())
            {

                case Type::StringType:
                    cout << "<" << prop.getName() << ">" <<
                        search->getCString(prop) << "</" <<
                        prop.getName() << ">" << endl;
                break;

                case Type::DataObjectType:
                    if (prop.isMany())
                    {
                        // many valued property, such as Requests , so get the list
                        DataObjectList& rq = search->getList(prop);
                        for (int j=0;j<rq.size();j++)
                        {
                            DataObjectPtr dob = rq[j];
                            if (dob != 0)
                            {
                                XMLDocumentPtr doc = myXMLHelper->createDocument(dob,
                                "axis", prop.getName());
                                doc->setXMLDeclaration(false);
                                char* dobXML = myXMLHelper->save(doc);
                                cout << "DataObject \"" << prop.getName() << 
                                        "\" as XML:" << endl;
                                cout << dobXML << endl;
                            } // if dob
                        } // for 
                    } // if many
                    else
                    {
                        DataObjectPtr dob = search->getDataObject(prop);
                        if (dob != 0)
                        {
                            XMLDocumentPtr doc = myXMLHelper->createDocument(dob,
                                "axis", prop.getName());
                            doc->setXMLDeclaration(false);
                            char* dobXML = myXMLHelper->save(doc);
                            cout << "DataObject \"" << prop.getName() << 
                                    "\" as XML:" << endl;
                            cout  << dobXML << endl;
                        } // if
                    } // else
                break;

                default:
                break;

            } // switch
        } // for
    } // try
    catch (SDORuntimeException e)
    {
        cout << "Merle 1 failed" << endl << e << endl;
    }
}


void sdotest::graham3()
{
    //try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("graham3.xsd");
    //}
    //catch (SDORuntimeException e)
    //{
//        cout << "Graham1 failed " << e << endl;
//    }
}

void sdotest::graham1()
{
    try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("graham1.xsd");
    }
    catch (SDORuntimeException e)
    {
        cout << "Graham1 failed " << e << endl;
    }
}
void sdotest::graham2()
{
    try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("graham2.xsd");
    }
    catch (SDORuntimeException e)
    {
        cout << "Graham2 failed " << e << endl;
    }
}


void sdotest::b46613()
{
    try {

        // Dynamic setup of the metadata 

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        mdg->create("rubbish","MyObject");

    }
    catch (SDOTypeNotFoundException e)
    {
        cout << "b46613 gave the correct exception" << endl;
    }
}

void sdotest::doctest()
{
    try {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("doctest.xsd");
        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("doctest.xml", 
        "companyNS");
        cout << "RootURI=" << myXMLDocument->getRootElementURI();
        cout << "RootName=" << myXMLDocument->getRootElementName();
    }
    catch (SDOTypeNotFoundException e)
    {
        cout << "doctest failed" << endl;
    }

}


void sdotest::b46634()
{
    try {

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("company_with_nillable_SN.xsd");
        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("company_with_nillable_SN.xml", 
        "companyNS");

        DataObjectPtr dob = myXMLDocument->getRootDataObject();
        DataObjectList& dol = dob->getList("departments");
        DataObjectPtr dept = dol[0];
        DataObjectList& dol2 = dept->getList("employees");
        DataObjectPtr emp = dol2[0];

        if (!emp->isSet("SN"))
        {
            cout << "B46634 WRONG - isSet should be true" << endl;
        }
        if (!emp->isNull("SN"))
        {
            cout << "B46634 WRONG - isNull should be true" << endl;
        }

        // now write out the nill value and check its still nil
        myXMLHelper->save(myXMLDocument->getRootDataObject(),
                          0, "company", "b46634_out.xml");

    }
    catch (SDORuntimeException e)
    {
        cout << "b46634 failed" << endl;
    }

}

void sdotest::testErrors()
{
    try {

        int i,j;

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->defineFile("error1.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char* m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("error1.xml", 
        "companyNS");

        if ((i = myXMLHelper->getErrorCount()) > 0)
        {
            cout << "XML reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = myXMLHelper->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }

    }
    catch (SDORuntimeException e)
    {
        cout << "testErrors  failed" << endl << e << endl;
    }

}

void sdotest::b46734()
{
    try {

        int i,j;

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("wsdl.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "WSDL XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        xsh->defineFile("wsdl-soap.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "WSDL SOAP XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char* m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
        xsh->defineFile("wsdl-http.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "WSDL HTTP XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char* m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
    //    xsh->defineFile("StockQuoteService.wsdl");
    //    if ((i = xsh->getErrorCount()) > 0)
    //    {
    //        cout << "SQ WSDL reported some errors:" << endl;
    //        for (j=0;j<i;j++)
    //        {
    //            const char *m = xsh->getErrorMessage(j);
    //            if (m != 0) cout << m;
    //            cout << endl;
    //        }
    //    }
        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("StockQuoteService.wsdl"
            );
            //, "http://swanandmokashi.com");

        if ((i = myXMLHelper->getErrorCount()) > 0)
        {
            cout << "XML reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char * m = myXMLHelper->getErrorMessage(j);
                if (m != 0)cout << m;
                cout << endl;
            }
        }

    }
    catch (SDORuntimeException e)
    {
        cout << "b46734 failed" << endl << e << endl;
    }

}

void sdotest::b46693()
{
    try {

        int i,j;

        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("b46693.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "b46693 XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char* m = xsh->getErrorMessage(j);
                if (m != 0)cout << m;
                cout << endl;
            }
        }

    //    XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
    //    XMLDocumentPtr myXMLDocument = myXMLHelper->loadFile("b46693.xsd"
    //        );

    //    if ((i = myXMLHelper->getErrorCount()) > 0)
    //    {
    //        cout << "b46693 reported some errors:" << endl;
    //        for (j=0;j<i;j++)
    //        {
    //            const char* m = myXMLHelper->getErrorMessage(j);
    //            if (m != 0)cout << m;
    //          cout << endl;
    //        }
    //    }

    }
    catch (SDORuntimeException e)
    {
        cout << "b46693 failed" << endl << e << endl;
    }

}

void sdotest::clonetest()
{
    try {
    DataFactoryPtr mdg  = DataFactory::getDataFactory();

    mdg->addType("companyNS","CompanyType");
    mdg->addType("companyNS","DepartmentType");
    mdg->addType("companyNS","EmployeeType");

   
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");

    mdg->addPropertyToType(tcomp,"name",tstring);
    mdg->addPropertyToType(tcomp,"departments",tdept,true,false,true);
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp,false,false,false);
    mdg->addPropertyToType(tcomp,"CEO",temp,false,false,true);

    mdg->addPropertyToType(tdept,"employees",temp,true,false,true);
    mdg->addPropertyToType(tdept,"name",tstring,false,false,true);
    mdg->addPropertyToType(tdept,"location",tstring,false,false,true);
    mdg->addPropertyToType(tdept,"number",tstring,false,false,true);

    mdg->addPropertyToType(temp,"name",tstring,false,false,true);
    mdg->addPropertyToType(temp,"SN",tstring,false,false,true);
    mdg->addPropertyToType(temp,"manager",tbool,false,false,true);


    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","The Company");

    DataObjectPtr dept = mdg->create((Type&)tdept);

    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","The Department");
    dept->setCString("location","Botley");
    dept->setCString("number","123");


    // now clone the data factory, and add more types

    DataFactoryPtr df2 = mdg->clone();

    df2->addType("otherNS","DataSet");

    df2->addPropertyToType("companyNS","DepartmentType","dataset",
        "otherNS","DataSet", false, false, true);

    const Type& tstring2  = df2->getType("commonj.sdo","String");
    const Type& tbool2    = df2->getType("commonj.sdo","Boolean");
    const Type& tcs2      = df2->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp2   = df2->getType("companyNS","CompanyType");
    const Type& tdept2    = df2->getType("companyNS","DepartmentType");
    const Type& temp2     = df2->getType("companyNS","EmployeeType");

    DataObjectPtr comp2 = df2->create((Type&)tcomp2);
    comp->setCString("name","The Company");

    DataObjectPtr dept2 = df2->create((Type&)tdept2);

    DataObjectList& dol2 = comp2->getList("departments");
    dol2.append(dept2);

    dept2->setCString("name","The Other Department");
    dept2->setCString("location","Not Botley");
    dept2->setCString("number","321");
    }
    catch (SDORuntimeException e)
    {
        cout << "clone failed " << e << endl;
    }


}

void sdotest::testUtil()
{
    try {

    cout << "START TEST: UTILS ======================================" << endl;

    DataFactoryPtr mdg  = DataFactory::getDataFactory();


    mdg->addType("companyNS","CompanyType");
    // employee will be an open type...
    mdg->addType("companyNS","EmployeeType", /*seq*/false, 
                                             /*open*/true, 
                                             /*abs */ false, 
                                             /*data*/ false);

    mdg->addType("companyNS","DepartmentType");

    mdg->addType("companyNS","OpenType");

    /* Now add the properties to the types...*/

    
    const Type& tstring  = mdg->getType("commonj.sdo","String");
    const Type& tbool    = mdg->getType("commonj.sdo","Boolean");
    const Type& tcs      = mdg->getType("commonj.sdo","ChangeSummary");
    const Type& tcomp    = mdg->getType("companyNS","CompanyType");
    const Type& tdept    = mdg->getType("companyNS","DepartmentType");
    const Type& temp     = mdg->getType("companyNS","EmployeeType");
    const Type& topen    = mdg->getType("companyNS","OpenType");

    
    mdg->addPropertyToType(tcomp,"name",tstring);
    // not containment...
    mdg->addPropertyToType(tcomp,"employeeOfTheMonth",temp, false,false,false);
    mdg->addPropertyToType(tcomp,"departments",tdept, true);
    mdg->addPropertyToType(tcomp,"cs",tcs);

    mdg->addPropertyToType(tdept,"name",tstring);
    mdg->addPropertyToType(tdept,"location",tstring);
    mdg->addPropertyToType(tdept,"number",tstring);
    mdg->addPropertyToType(tdept,"employees",temp,true);
    
    mdg->addPropertyToType(temp, "name",tstring);
    mdg->addPropertyToType(temp, "SN",  tstring);
    mdg->addPropertyToType(temp, "manager",  tbool);

    mdg->addPropertyToType(topen,"name",tstring);

    DataObjectPtr comp = mdg->create((Type&)tcomp);
    comp->setCString("name","ACME");

    DataObjectPtr dept = mdg->create((Type&)tdept);
    DataObjectList& dol = comp->getList("departments");
    dol.append(dept);

    dept->setCString("name","Advanced Technologies");
    dept->setCString("location","NY");
    dept->setCString("number","123");

    DataObjectList& emps = dept->getList("employees");

    DataObjectPtr emp1 = mdg->create(temp);
    DataObjectPtr emp2 = mdg->create(temp);
    DataObjectPtr emp3 = mdg->create(temp);

    emps.append(emp1);
    emps.append(emp2);
    emps.append(emp3);

    emp1->setCString("name","Albert");
    emp1->setCString("SN","E0001");

    emp2->setCString("name","Boris");
    emp2->setCString("SN","E0002");
    emp2->setBoolean("manager",true);

    emp3->setCString("name","Carl");
    emp3->setCString("SN","E0003");

    emp1->setCString("openstring","Value Of Open String");
    emp1->setBoolean("openboolean",true);

    const char* c = emp1->getCString("openstring");

    bool b = emp1->getBoolean("openboolean");

    // unknown list type at present..
    DataObjectList& dl = emp1->getList("openintlist");
    
    int i = 45;
    dl.append((short)i); // now the list must be primitive and short!!

    // unknown list type at present..
    DataObjectList& dl2 = emp1->getList("opendataobjectlist");
    
    DataObjectPtr myopen = mdg->create("companyNS","OpenType");
    myopen->setCString("name","MyOpenName");
    dl2.append(myopen); // now the list must be of data objects
    
    emp1->setDataObject("opendataobject", myopen);

    SDOUtils::printDataObject(comp);

    }
    catch (SDORuntimeException e)
    {
        cout << "ERROR in UTILS test" << e << endl;
    }
    cout << "END TEST: UTILS ======================================" << endl;

}

void sdotest::printset(ChangeSummaryPtr cs)
{
    ChangedDataObjectList& cdl = cs->getChangedDataObjects();

    // here there should be a creation of Ed, and a change to dept, which has a
    // bool previously unset, and a int list previuously unset and an employees list
    // previously unset
    
    for (int i=0;i< cdl.size();i++)
    {
        if (cs->isModified(cdl[i]))
        {

            cout << "A modified of type " << cdl[i]->getType().getURI()  
                 << "#" << cdl[i]->getType().getName() << endl;

            SettingList& sl = cs->getOldValues(cdl[i]);
            if (sl.size() == 0) 
            {
                cout << "No Settings found" << endl;
            }
            else 
            {
                for (int j=0;j< sl.size(); j++)
                {
                    cout << "Property " << sl[j].getProperty().getName();
                    // this could be a many-valued property, and could be one which is
                    // a data object or a data type...
                    if (sl[j].getProperty().isMany()) 
                    {
                        cout << "[" << sl[j].getIndex() << "]" ;
                    }
                    if (!sl[j].isSet())
                    {
                        cout << " was unset before the change" << endl;
                    }
                    else
                    {
                        cout << " was set before the change !!" << endl;
                    }
                }
            }
        }
    }
}
    

void sdotest::b46633()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");
        mdg->addPropertyToType("myspace","Root","cs",
                           "commonj.sdo","ChangeSummary", false, false, false);

        mdg->addType("myspace","Company");
        mdg->addType("myspace","Department");
        mdg->addType("myspace","Manager");
        mdg->addType("myspace","Employee");


        mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    

        mdg->addPropertyToType("myspace","Root","company",
                           "myspace","Company", false, false, true);

        mdg->addPropertyToType("myspace","Company","departments",
                           "myspace","Department", true, false, true);


        mdg->addPropertyToType("myspace","Department","name",
                           "commonj.sdo","String", false, false, false);

        mdg->addPropertyToType("myspace","Department","ints",
                           "commonj.sdo","Integer", true, false, false);


        mdg->addPropertyToType("myspace","Department","bool",
                           "commonj.sdo","Boolean", false, false, false);

        mdg->addPropertyToType("myspace","Department","manager",
                           "myspace","Manager", false, false, true);

        mdg->addPropertyToType("myspace","Department","employees",
                           "myspace","Employee", true, false, true);

        mdg->addPropertyToType("myspace","Manager","name",
                           "commonj.sdo","String", false, false, false);
    
        mdg->addPropertyToType("myspace","Employee","name",
                            "commonj.sdo","String", false, false, false);

        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    
        ChangeSummaryPtr cs = dor->getChangeSummary();

        DataObjectPtr com = dor->createDataObject("company");
        com->setCString("name","IBM");

        DataObjectPtr dept = com->createDataObject("departments");
        dept->setCString("name","JTC");

        // employees initally unset

        cs->beginLogging();

        DataObjectPtr emp = dept->createDataObject("employees");
        emp->setCString("name","Ed");



        DataObjectList& dl = dept->getList("ints");
        dl.append((long)400);
    
        dept->setBoolean("bool", true);

        printset(cs);

        cs->endLogging();

        // unset them all , restart logging, and get the same results
    
        dept->unset("bool");
        dept->unset("ints");
        dept->unset("employees");

        cs->beginLogging();

        DataObjectPtr emp2 = dept->createDataObject("employees");
        emp2->setCString("name","Ed2");

        dl.append((long)600);
    
        dept->setBoolean("bool", true);

        printset(cs);

        cs->endLogging();

    }
    catch (SDORuntimeException e)
    {
        cout << "b46633 failed " << e << endl;
    }

}



void sdotest::testLoad()
{
    try 
    {
        int i,j;
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
        FILE* f = fopen("company_with_nillable_SN.xsd","r+");
        char* buffer = new char[4000];
        j = 0;
        while ((i = fgetc(f)) != EOF)
        {
            buffer[j++] = (char)i;
        }
        buffer[j] = 0;
        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        xsh->define(buffer);

        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }

        FILE* f2 = fopen("company_with_nillable_SN.xml","r+");
        j = 0;
        while ((i = fgetc(f2)) != EOF)
        {
            buffer[j++] = (char)i;
        }
        buffer[j] = 0;

        XMLHelperPtr myXMLHelper = HelperProvider::getXMLHelper(mdg);
        XMLDocumentPtr myXMLDocument = myXMLHelper->load(buffer);

    }
    catch (SDORuntimeException e)
    {
        cout << "Exception in testDAS " << e << endl;
    }
}

void sdotest::includetest()
{
    try 
    {
        int i,j;
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("include.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "INCLUDE XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }

        xsh->defineFile("../t2/includeother.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "INCLUDE XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
    }
    catch (SDORuntimeException e)
    {
        cout << "Exception in include test" << e << endl;
    }
}

void sdotest::detachtest()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");

        mdg->addType("myspace","Company");
        mdg->addType("myspace","Department");
        mdg->addType("myspace","Manager");
        mdg->addType("myspace","Employee");


        mdg->addPropertyToType("myspace","Company","name",
                           "commonj.sdo","String", false, false, false);
    

        mdg->addPropertyToType("myspace","Root","company",
                           "myspace","Company", false, false, true);

        mdg->addPropertyToType("myspace","Company","departments",
                           "myspace","Department", true, false, true);


        mdg->addPropertyToType("myspace","Department","name",
                           "commonj.sdo","String", false, false, false);


        mdg->addPropertyToType("myspace","Department","manager",
                           "myspace","Manager", false, false, true);

        mdg->addPropertyToType("myspace","Department","employees",
                           "myspace","Employee", true, false, true);

        mdg->addPropertyToType("myspace","Manager","name",
                           "commonj.sdo","String", false, false, false);
    
        mdg->addPropertyToType("myspace","Employee","name",
                            "commonj.sdo","String", false, false, false);

        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    
        DataObjectPtr com = dor->createDataObject("company");
        com->setCString("name","IBM");

        DataObjectPtr dept = com->createDataObject("departments");

        dept->setCString("name","JTC");


        DataObjectPtr emp = dept->createDataObject("employees");
        emp->setCString("name","Ed");

        DataObjectPtr emp2 = dept->createDataObject("employees");
        emp2->setCString("name","Tom");
        
        DataObjectList& dol = dept->getList("employees");

        DataObjectPtr e = dol[0];
        e->detach();

        DataObjectPtr cont = emp->getContainer();

        if (cont != 0)
        {
            cout << "EMP STILL ATTACHED - problem" << endl;
        }

        DataObjectPtr cont2 = dol[0]->getContainer();

        if (cont2 == 0)
        {
            cout << "SECOND DETACHED - problem" << endl;
        }

    }
    catch (SDORuntimeException e)
    {
        cout << "detach failed " << e << endl;
    }

}


void sdotest::leak()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");


        mdg->addPropertyToType("myspace","Root","ints",
                           "commonj.sdo","Integer", true, false, false);


        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    

        cout << "List set should be false and is " << dor->isSet("ints") << endl;

        DataObjectList& dl = dor->getList("ints");
        
        cout << "List set should be false and is " << dor->isSet("ints") << endl;

        dl.append((long)400);
    
        cout << "List set should be true and is " << dor->isSet("ints") << endl;

        dl.remove(0);
    
        cout << "List set should be false and is " << dor->isSet("ints") << endl;

        dl.append((long)400);

        cout << "List set should be true and is " << dor->isSet("ints") << endl;

        dor->unset("ints");

        cout << "List set should be false and is " << dor->isSet("ints") << endl;

        dl.append((long)600);
    
        cout << "List set should be true and is " << dor->isSet("ints") << endl;


    }
    catch (SDORuntimeException e)
    {
        cout << "leak failed " << e << endl;
    }

}

void sdotest::twolists()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");

        //mdg->addType("myspace","Company");
        //mdg->addType("myspace","Department");
        //mdg->addType("myspace","Manager");
        //mdg->addType("myspace","Employee");


        //mdg->addPropertyToType("myspace","Company","name",
        //                   "commonj.sdo","String", false, false, false);
    

        //mdg->addPropertyToType("myspace","Root","company",
        //                   "myspace","Company", false, false, true);

        //mdg->addPropertyToType("myspace","Company","departments",
        //                   "myspace","Department", true, false, true);


        //mdg->addPropertyToType("myspace","Department","name",
        //                   "commonj.sdo","String", false, false, false);

        mdg->addPropertyToType("myspace","Root","ints",
                           "commonj.sdo","Integer", true, false, false);

        mdg->addPropertyToType("myspace","Root","ints2",
                           "commonj.sdo","Integer", true, false, false);

        //mdg->addPropertyToType("myspace","Department","bool",
        //                   "commonj.sdo","Boolean", false, false, false);

        //mdg->addPropertyToType("myspace","Department","manager",
        //                   "myspace","Manager", false, false, true);

        //mdg->addPropertyToType("myspace","Department","employees",
        //                   "myspace","Employee", true, false, true);

        //mdg->addPropertyToType("myspace","Manager","name",
        //                   "commonj.sdo","String", false, false, false);
    
        //mdg->addPropertyToType("myspace","Employee","name",
        //                    "commonj.sdo","String", false, false, false);

        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    
        //ChangeSummaryPtr cs = dor->getChangeSummary();

        //DataObjectPtr com = dor->createDataObject("company");
        //com->setCString("name","IBM");

        //DataObjectPtr dept = com->createDataObject("departments");
        //dept->setCString("name","JTC");

        // employees initally unset

        //cs->beginLogging();

        //DataObjectPtr emp = dept->createDataObject("employees");
        //emp->setCString("name","Ed");

        DataObjectList& dl2 = dor->getList("ints2");
        dl2.append((long)400);


        DataObjectList& dl = dor->getList("ints");
        dl.append((long)400);
    
        //dept->setBoolean("bool", true);

        //printset(cs);

        //cs->endLogging();

        // unset them all , restart logging, and get the same results
    
        //dept->unset("bool");
        //dept->unset("ints");
        //dept->unset("employees");

        //cs->beginLogging();

        //DataObjectPtr emp2 = dept->createDataObject("employees");
        //emp2->setCString("name","Ed2");

        //dl.append((long)600);
    
        //dept->setBoolean("bool", true);

       // printset(cs);

        //cs->endLogging();

    }
    catch (SDORuntimeException e)
    {
        cout << "twolists failed " << e << endl;
    }

}


void sdotest::graham4()
{
    try {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();
    
        mdg->addType("myspace","Root");
        mdg->addType("myspace","ListItem");


        mdg->addPropertyToType("myspace","ListItem","name",
                           "commonj.sdo","String", false, false, false);
    

        mdg->addPropertyToType("myspace","Root","ints",
                           "commonj.sdo","Integer", true, false, false);

        mdg->addPropertyToType("myspace","Root","items",
                           "myspace","ListItem", true, false, true);


        const Type& tcc = mdg->getType("myspace","Root");

        DataObjectPtr dor = mdg->create((Type&)tcc);
    

        // first try with no elements in lists

        try {
            DataObjectPtr pitem = dor->getDataObject("items[name=\"hello\"]");
        }
        catch (SDOIndexOutOfRangeException e)
        {
            cout << "Normal exception in Graham4" << endl;
        }

        try {
            int inty = dor->getInteger("ints[1]");
        }
        catch (SDOIndexOutOfRangeException e)
        {
            cout << "Normal exception in Graham4" << endl;
        }

        // now populate them

        DataObjectPtr myitem = dor->createDataObject("items");
        myitem->setCString("name","not hello");

        try {
            DataObjectPtr pitem2 = dor->getDataObject("items[name=\"hello\"]");
        }
        catch (SDOIndexOutOfRangeException e)
        {
            cout << "Normal exception in Graham4" << endl;
        }

        DataObjectList& dl = dor->getList("ints");
        dl.append((long)100);

        try {
            int inty2 = dor->getInteger("ints[2]");
        }
        catch (SDOIndexOutOfRangeException e)
        {
            cout << "Normal exception in Graham4" << endl;
        }

        try 
        {
            DataObjectPtr pitem2 = dor->getDataObject("items[name=\"not hello\"]");
            cout << "item name is " << pitem2->getCString("name") << endl;
        }
        catch (SDORuntimeException e)
        {
            cout << "ABNORMAL exception in Graham4" << endl;
        }

        try {
            int inty2 = dor->getInteger("ints[1]");
            cout << "Integer is " << inty2 << endl;
        }
        catch (SDOIndexOutOfRangeException e)
        {
            cout << "Normal exception in Graham4" << endl;
        }

    }
    catch (SDORuntimeException e)
    {
        cout << "twolists failed " << e << endl;
    }

}


void sdotest::graham5()
{
    try 
    {
        int i,j;
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("Order.xsd");
        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "INCLUDE XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }
    }
    catch (SDORuntimeException r)
    {
        cout << "Graham5 failed " << r << endl;
    }
}



void sdotest::b47293()
{
    int i,j,k;
    try 
    {
        DataFactoryPtr mdg  = DataFactory::getDataFactory();

        XSDHelperPtr xsh = HelperProvider::getXSDHelper(mdg);
        
        xsh->defineFile("47293Catalog.xsd");

        if ((i = xsh->getErrorCount()) > 0)
        {
            cout << "47293 XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }

        TypeList tl = mdg->getTypes();
        for ( k=0;k<tl.size();k++)
        {
            cout << "Type:" << tl[k].getURI() << "#" << tl[k].getName() << endl;
        }

        xsh->generateFile(tl,"47293Catalog-out.xsd","catalogNS");

        DataFactoryPtr mdg2  = DataFactory::getDataFactory();

        XSDHelperPtr xsh2 = HelperProvider::getXSDHelper(mdg2);
        
        xsh2->defineFile("47293Catalog-out.xsd");

        if ((i = xsh2->getErrorCount()) > 0)
        {
            cout << "47293-out XSD reported some errors:" << endl;
            for (j=0;j<i;j++)
            {
                const char *m = xsh2->getErrorMessage(j);
                if (m != 0) cout << m;
                cout << endl;
            }
        }

        TypeList tl2 = mdg2->getTypes();
        for (k=0;k<tl2.size();k++)
        {
            cout << "Type:" << tl2[k].getURI() << "#" << tl2[k].getName() << endl;
        }



        //XMLHelperPtr xmh = HelperProvider::getXMLHelper(mdg);

        //XMLDocumentPtr doc = xmh->loadFile("47293Catalog.xml", "catalog");

        //DataObjectPtr dob = doc->getRootDataObject();
        
        //xmh->save(dob,"","catalog","47293Catalog-out.xml");


    }
    catch (SDORuntimeException e)
    {
        cout << "B47137 failed " << e << endl;
    }
}

