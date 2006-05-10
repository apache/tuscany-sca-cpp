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

/* $Id: utils.cpp,v 1.1 2006/03/16 10:54:16 slattery Exp $ */

#include <stdio.h>

#pragma warning(disable:4786)

#include <iostream>
using namespace std;

#include "sdotest.h"

bool sdotest::silent = true;
bool sdotest::ramping = false;

void sdotest::printOldValues(FILE *f, ChangeSummaryPtr cs, DataObjectPtr dol)
{
    fprintf(f,"===== Old Property Values Begin\n");
    SettingList& sl = cs->getOldValues(dol);
    if (sl.size() == 0) 
    {
        fprintf(f,"No Settings found\n");
    }
    else 
    {
        for (int j=0;j< sl.size(); j++)
        {
            fprintf(f,"Property %s",sl[j].getProperty().getName());
            // this could be a many-valued property, and could be one which is
            // a data object or a data type...
            if (sl[j].getProperty().isMany()) 
            {
                fprintf(f,"[%d]",sl[j].getIndex());
            }
            if (!sl[j].isSet())
            {
                fprintf(f,"(UNSET)");
            }
            if (sl[j].isNull())
            {
                fprintf(f,"(ISNULL)");
            }

            fprintf(f," of type ");
            switch (sl[j].getProperty().getTypeEnum())
            {
                case Type::BooleanType:
                    fprintf(f,"Boolean:%d\n",sl[j].getBooleanValue());
                break;
                case Type::ByteType:
                    fprintf(f,"Byte:%d\n",sl[j].getByteValue());
                break;
                case Type::CharacterType:
                    fprintf(f,"Character:%d\n",sl[j].getCharacterValue());
                break;
                case Type::IntegerType: 
                    fprintf(f,"Integer:%d\n",sl[j].getIntegerValue());
                break;
                case Type::ShortType:
                    fprintf(f,"Short:%d\n",sl[j].getShortValue());
                break;
                case Type::DoubleType:
                    fprintf(f,"Double:%.3f\n",(float)sl[j].getDoubleValue());
                break;
                case Type::FloatType:
                    fprintf(f,"Float:%.3f\n",sl[j].getFloatValue());
                break;
                case Type::LongType:
                    fprintf(f,"Long:%ld\n",sl[j].getIntegerValue());
                break;
                case Type::DateType:
                    fprintf(f,"Date:%d\n",sl[j].getDateValue().getTime());
                break;
                case Type::BigDecimalType: 
                case Type::BigIntegerType: 
                case Type::StringType: 
                case Type::UriType:
                    fprintf(f,"String:%s\n",sl[j].getCStringValue());
                    break;
                case Type::BytesType:
                    fprintf(f,"Bytes:%s\n",sl[j].getCStringValue());
                break;
                case Type::OtherTypes:
                case Type::DataObjectType:
                case Type::ChangeSummaryType:
                {
                    fprintf(f,"DataObject ");
                    DataObjectPtr dob = sl[j].getDataObjectValue();
                    if (!dob) 
                    {
                        fprintf(f," - object null or unset\n");
                    }
                    else 
                    {
                        DataObjectPtr mydo = sl[j].getDataObjectValue();
                        if (cs->isDeleted(mydo))
                        {
                            fprintf(f," - object deleted \n");
                            printOldValues(f, cs,mydo);
                        }
                        else 
                        {
                            fprintf(f," object still exists \n");
                        }
                    }
                }
                break;
                default:
                {
                        fprintf(f,"Unknown object type\n");
                }
                break;
            }
            fprintf(f,"\n");
        }
    }
    fprintf(f,"===== Old Property Values End \n");
    return;
}


void sdotest::printDataStructure(FILE *f , DataFactory* dd)
{
    TypeList tt = dd->getTypes();
    fprintf(f,"Printing Types\n");
    for (int i = 0; i < tt.size(); ++i)
    {
        fprintf(f,"Type %s\n",tt[i].getName());
        PropertyList pl = tt[i].getProperties();
        for (int j = 0; j < pl.size() ; j++)
        {
            fprintf(f,"Has Property %s of type %s\n",
                pl[j].getName(),pl[j].getType().getName());
        }
    }
}


void sdotest::printValue(FILE *f, DataObjectPtr dp, const Property& p)
{
    switch (p.getTypeEnum())
    {
        case Type::BooleanType:
            fprintf(f,"boolean:%d\n",dp->getBoolean(p));
        break;
        case Type::ByteType:
            fprintf(f,"Byte:%d\n",dp->getByte(p));
        break;
        case Type::CharacterType:
            fprintf(f,"character:%d\n",dp->getCharacter(p));
        break;
        case Type::IntegerType: 
            fprintf(f,"integer:%d\n",dp->getInteger(p));
        break;
        case Type::ShortType:
            fprintf(f,"short:%d\n",dp->getShort(p));
        break;
        case Type::DoubleType:
            fprintf(f,"double:%.3f\n",(float)dp->getDouble(p));
        break;
        case Type::FloatType:
            fprintf(f,"float:%.3f\n", dp->getFloat(p));
        break;
        case Type::LongType:
            fprintf(f,"long:%ld\n",dp->getInteger(p));
        break;
        case Type::DateType:
            fprintf(f,"date:%d\n",dp->getDate(p).getTime());
        break;
        case Type::BigDecimalType: 
        case Type::BigIntegerType: 
        case Type::StringType: 
        case Type::UriType:
            fprintf(f,"string:%s\n", dp->getCString(p));
            break;
        case Type::BytesType:
            fprintf(f,"bytes:%s\n",dp->getCString(p));
        break;
        case Type::OpenDataObjectType:
        case Type::DataObjectType:
        {
            fprintf(f,"dataObject\n" );
            DataObjectPtr dob = dp->getDataObject(p);
            if (!dob) 
            {
                fprintf(f," - null or unset\n");
            }
            else 
            {
                printDataObject(f, dob);
            }
        }
        break;
        case Type::OtherTypes:
        case Type::ChangeSummaryType:
        default:
        {
            fprintf(f,"Unknown object type");
        }
        break;
    }
    fprintf(f,"\n");
}

void sdotest::printList(FILE *f, DataObjectPtr dp, const Property& p)
{
    DataObjectList& dobl = dp->getList(p);
    fprintf(f," list ");
    
    if (dobl.size() ==0) {
        fprintf(f,"(empty)\n");
        return;
    }

    for (int i=0;i<dobl.size();i++) {

    switch (p.getTypeEnum())
    {
        case Type::BooleanType:
            fprintf(f,"boolean[%d]=%d\n",i,dobl.getBoolean(i));
        break;
        case Type::ByteType:
            fprintf(f,"byte[%d]=%d\n",i,dobl.getByte(i));
        break;
        case Type::CharacterType:
            fprintf(f,"character[%d]=%d\n",i,dobl.getCharacter(i));
        break;
        case Type::IntegerType: 
            fprintf(f,"integer[%d]=%d\n",i,dobl.getInteger(i));
        break;
        case Type::ShortType:
            fprintf(f,"short[%d]=%d",i, dobl.getShort(i));
        break;
        case Type::DoubleType:
            fprintf(f,"double[%d]=%.3f\n",i,(float)dobl.getDouble(i));
        break;
        case Type::FloatType:
            fprintf(f,"float[%d]=%.3f\n",i,dobl.getFloat(i));
        break;
        case Type::LongType:
            fprintf(f,"long[%d]=%ld\n",i,dobl.getInteger(i));
        break;
        case Type::DateType:
            fprintf(f,"date[%d]=%d\n",i,dobl.getDate(i).getTime());
        break;
        case Type::BigDecimalType: 
        case Type::BigIntegerType: 
        case Type::StringType: 
        case Type::UriType:
            fprintf(f,"string[%d]=%s\n",i,dobl.getCString(i));
            break;
        case Type::BytesType:
            fprintf(f,"bytes[%d]=%s\n",i,dobl.getCString(i));
        break;
        case Type::OpenDataObjectType:
        case Type::DataObjectType:
        {
            fprintf(f,"dataObject[%d]=\n",i);
            DataObjectPtr dob = dobl[i];
            if (!dob) 
            {
                fprintf(f," null or unset\n");
            }
            else 
            {
                printDataObject(f, dob);
            }
        }
        break;
        case Type::OtherTypes:
        case Type::ChangeSummaryType:
        default:
        {
            fprintf(f,"Unknown object type\n");;
        }
        break;
    }
    fprintf(f,"\n");
    }
}

void sdotest::printDataObject(FILE *f, DataObjectPtr dol)
{
    fprintf(f,"===== DataObject contents =====\n");
    PropertyList pl = dol->getInstanceProperties();

    for (int j=0;j< pl.size(); j++)
    {
        fprintf(f,"%s:",pl[j].getName());
        // this could be a many-valued property, and could be one which is
        // a data object or a data type...
        if (pl[j].isMany()) 
        {
            printList(f, dol,pl[j]);
        }
        else {
            printValue(f, dol,pl[j]);
        }
    }
    fprintf(f,"===== End DataObject =====\n");
}



void sdotest::dumpchangesummary(FILE *f, ChangeSummaryPtr cs)
{

    ChangedDataObjectList& cdol = cs->getChangedDataObjects();

    // the changed data object list contains only the objects whose properties have
    // been changed. 
    // a changed and subsequently deleted object will not appear, but necessarily its
    // container will appear, so we can rebuild it.

    for (int i=0;i< cdol.size();i++)
    {
        if (cs->isCreated(cdol[i]))
        {
            fprintf(f,"Created object in changed list\n");
            // So its in the created list, it must exist in the tree...
            fprintf(f,"The object is %s#%s\n", 
                  cdol[i]->getType().getURI(),
                  cdol[i]->getType().getName());
        }
        if (cs->isModified(cdol[i]))
        {
            fprintf(f,"===== Modified Object Starts\n");
            // get to a DAS data object...
            DataObject* temp = cdol[i];
            fprintf(f,"Xpath:%s\n",((DataObject*)temp)->objectToXPath());
 
            if (cs->isDeleted(cdol[i]))
            {
                fprintf(f,"PROBLEM: DELETED OBJECT IN CHANGED LIST: \n");
                // As the item is in the deleted list - its still present..
                fprintf(f,"The type is %s#%s\n",
                        cdol[i]->getType().getURI(),  
                        cdol[i]->getType().getName());
            }
            else {
                fprintf(f,"The modified objects type is %s#%s\n",
                             cdol[i]->getType().getURI(),  
                             cdol[i]->getType().getName());

                printOldValues(f, cs, cdol[i]);
            }
            fprintf(f,"=====Modified Object Ends \n");
        }
        if (cs->isDeleted(cdol[i]))
        {
            fprintf(f,"=====Deleted Object Starts \n");
            printOldValues(f, cs, cdol[i]);
            fprintf(f,"=====Deleted Object Ends \n");
        }
    }
}


int sdotest::comparefiles(char* fn1, char*fn2)
{
    FILE *f1, *f2;
    f1 = fopen(fn1,"r+");
    if (f1 == 0)
    {
        if (!silent) cout << "Cannot open file:" << fn1 << endl;
        return 0;
    }
    f2 = fopen(fn2,"r+");
    if (f2 == 0)
    {
        if (!silent) cout << "Cannot open file:" << fn2 << endl;
        if (ramping)
        {
            cout << "Creating it" << endl;
            f2 = fopen(fn2,"w+");
            if (f2 == 0)
            {
                cout << "Cannot ramp up - failed on " << fn2 << endl;
                return 0;
            }
            int c;
            do
            {
                c = fgetc(f1);
                if (!feof(f1)) fputc(c,f2);
                else break;
            } while (1);
            fclose(f1);
            fclose(f2);
            return 1;
        }
        else 
        {
            fclose(f1);
            return 0;
        }
    }
   
    while (!feof(f1))
    {
        if (fgetc(f1) != fgetc(f2))
        {
            fclose(f1);
            fclose(f2);
            return 0;
        }
    }
    fclose (f1);

    if (!feof(f2))
    {
        fclose(f2);
        return 0;
    }

    fclose(f2);
    return 1;
}


int sdotest::printseq(FILE *f, SequencePtr sptr)
{
    fprintf(f, "======================================\n");
    for (int i=0;i< sptr->size();i++)
    {
        try {
            if (!sptr->isText(i)) 
            {
                const Property& prp = sptr->getProperty(i);
                if (!strcmp(prp.getType().getName(),"Department"))
                {
                    DataObjectPtr pdep = sptr->getDataObjectValue(i);
                    if (pdep != 0)
                    {
                        const char * cs = pdep->getCString("name");
                        if (cs != 0)
                        {
                            fprintf(f, "%s\n",cs);
                        }
                        else
                        {
                            fprintf(f," is empty \n");
                        }
                    }
                }
                else 
                {
                    const char* stx = sptr->getCStringValue(i);
                    if (stx != 0)
                    {
                        fprintf(f,"%s\n",stx);
                    }
                    else
                    {
                        fprintf(f," is empty \n");
                    }
                }
            }
            else 
            {
                const char* st = sptr->getCStringValue(i);
                if (st != 0)
                {
                    fprintf(f,"%s\n", st);
                }
                else
                {
                    fprintf(f," is empty \n");
                }
            }
        }
        catch (SDOPropertyNotSetException)
        {
            if (!silent) cout << "WRONG - got a property not set exception!!!" << endl;
            return 0;
        }
    }
    fprintf(f,  "======================================\n");
    return 1;
}
