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

/* $Rev$ $Date: 2006/02/17 16:01:05 $ */

#include "commonj/sdo/ChangeSummaryImpl.h"

#include "commonj/sdo/DataObjectImpl.h"

#include "commonj/sdo/Property.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/TypeList.h"
#include "commonj/sdo/Sequence.h"
#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/DataObjectList.h"
#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/Logging.h"

#include <stdlib.h>

namespace commonj{
namespace sdo{


    // Initializes an empty change summary, so we know one is required
    ChangeSummaryImpl::ChangeSummaryImpl()
    {
        logging = false;
    }

    ChangeSummaryImpl::~ChangeSummaryImpl()
    {
        //Force logging off or bad things will happen!
        logging = false;

        /**
         * These remove the logitems, so cause the
         *  refcounts of the data objects to drop, and
         * delete the settings lists.
         */
        deletedMap.clear();
        createdMap.clear();
        changedMap.clear();
        changedDataObjects.clear();
    }

    SequencePtr ChangeSummaryImpl::getOldSequence(DataObjectPtr dob)
    {
        DELETELOG_MAP::iterator deleteLogIter;

        DataObject* ob = dob;
        deleteLogIter = deletedMap.find((DataObjectImpl*)ob);

        if (deleteLogIter != deletedMap.end())
        {
            return (deleteLogIter->second).getSequence();
        }
    
        CHANGELOG_MAP::iterator changeLogIter;

        changeLogIter = changedMap.find((DataObjectImpl*)ob);

        if (changeLogIter != changedMap.end())
        {
            return (changeLogIter->second).getSequence();
        }
        return NULL;

    }

    void ChangeSummaryImpl::removeFromChanges(DataObjectImpl* ob)
    {
        int i = changedDataObjects.size();

       /**
        * this is used to remove only the most recent change.
        * A deletion has occurred, and there was a creation, so 
        * we remove the creation. There cannot be a change after the
        * creation, so the last entry in changes is the creation.
        * there could be a previous deletion of the same object, so
        * we should not clear the whole list.
        */
        while (i > 0)
        {
            i--;


            if (changedDataObjects.get(i) == ob)
            {
                changedDataObjects.remove(i);
                return;
            }
        }
    }

    DataObjectPtr ChangeSummaryImpl::matchDeletedObject(SDOXMLString path)
    {
        DELETELOG_MAP::iterator deleteLogIter;

        for (deleteLogIter = deletedMap.begin();
                deleteLogIter != deletedMap.end(); ++deleteLogIter)
        {
            if (
                path.equals(
                SDOXMLString((deleteLogIter->second).getOldXpath())
                )
                )
            {
                return deleteLogIter->first;
            }
        }
        return DataObjectPtr(0);
    }



    void ChangeSummaryImpl::logDeletion(DataObjectImpl* ob,
        DataObjectImpl* container, const Property& prop,
        const char* oldpath, bool loggingChildren)
    {
        LOGINFO_1(INFO,"ChangeSummary:LogDelete:%s",
            ob->getType().getName());

        /**
         * The object is about to be deleted, so we need
         * all its property Settings recorded in the list
         * of changed data objects. We also need to know
         * its old container, container property, and 
         * value.
         */

        // find any properties which are data objects, log their
        // deletion first.

        int i;

        // Trace for change summaries

        LOGINFO_2(INFO,"ChangeSummary logs a deletion of property %s type %s", 
             prop.getName(), prop.getType().getName());


        CREATELOG_MAP::iterator createLogIter;
        CHANGELOG_MAP::iterator changeLogIter;

        createLogIter = createdMap.find(ob);
        if (createLogIter != createdMap.end())
        {
            // would need to remove it from the created list.
            LOGINFO(INFO,"ChangeSummary: The deletion was already created - just removing it");

            removeFromChanges(ob);
            createdMap.erase(ob);
            // find the container in the change map.

            changeLogIter = changedMap.find(container);
            if (changeLogIter != changedMap.end())
            {
                // would hope there was an entry
                int index;
                if (prop.isMany())
                {
                    DataObjectList& dl = container->getList(prop);
                    for (index=0;index<dl.size();index++)
                    {
                        if (dl[index] == ob)
                        {
                            break;
                        }
                    }
                }
                else 
                {
                    index = 0;
                }

                SettingList& slist = (changeLogIter->second).getSettings();

                for (int j=0 ; j < slist.size();j++)
                {
                    if (!strcmp(slist.get(j)->getProperty().getName(),
                                    prop.getName())
                                   
                                   
                        && slist.get(j)->getIndex() == 
                              index)
                    {
                        // these are settings of the same prop/index, we
                        // need to remove this entry
                        slist.remove(j);
                        // and if this was the only change, then we
                        // need to remoe the container from the changes
                        if (slist.size() == 0)
                        {
                            changedMap.erase(container);
                            removeFromChanges(container);
                        }
                        break; // must not continue - size is now wrong
                    }
                }
            }
            return;
        }

        DELETELOG_MAP::iterator deleteLogIter;

        deleteLogIter = deletedMap.find(ob);

        if (deleteLogIter != deletedMap.end())
        {
            LOGINFO(INFO,"ChangeSummary - delete an already deleted object");
            return;
        }

         PropertyList pl = ob->getInstanceProperties();
        DataObject* dob;

        if (loggingChildren)
        {
            for (i=0; i < pl.size(); i++) 
            {
                if (pl[i].getType().isDataObjectType())
                {
                    if (pl[i].isMany()) {
                        DataObjectList& dl = ob->getList(pl[i]);
                        for (int j = dl.size() - 1; j>=0; j--)
                        {
                            dob = dl[j];
                            if (dob)logDeletion((DataObjectImpl*)dob,ob,pl[i],
                                dob->objectToXPath(), true);
                        }
                    }
                    else {
                        dob = ob->getDataObject(pl[i]);
                        if (dob)logDeletion((DataObjectImpl*)(dob),ob,pl[i],
                            dob->objectToXPath(), true);
                    }
                }
            }
        }


         // build a Setting list for the set properties

        deletedMap.insert(std::make_pair(ob,deleteLogItem(
            (DataObject*)ob, prop,ob->getSequence(),
            oldpath,
            container)));
        deleteLogIter = deletedMap.find(ob);
        SettingList& sl = (deleteLogIter->second).getSettings();

         void* value;
        unsigned int len;

        for (i=0; i < pl.size(); i++) 
        {
            //if (!pl[i].getType().isDataType()) continue;

            if (!ob->isSet(pl[i]))
            { 
                sl.append(new Setting(false,false, 0,0,pl[i],0));
                continue;
            }
            if (pl[i].isMany())
            {

                DataObjectList& dol = ob->getList(pl[i]);
                for (int j=dol.size()-1;j >= 0; j--)
                {
                    // needs to be the data object in cases where...
                    if (pl[i].getType().isDataType()) {
                        setPropValue(&value,&len,ob,pl[j]);
                        sl.append(new Setting(true,false,value,len,pl[i],j));
                    }
                    else {
                        value = (void*)dol[j];
                        sl.append(new Setting(true,false,value,0,pl[i],j));
                    }
                }
            }
            else 
            {
                setPropValue(&value,&len,ob,pl[i]);
                sl.append(new Setting(true,ob->isNull(pl[i]),value,len,pl[i],0));
            }

        }


        changeLogIter = changedMap.find(ob);
        if (changeLogIter != changedMap.end())
        {
            // we have already changed this object, so we need the old values
            // from the change, and to remove the changed map entry
            LOGINFO(INFO,"ChangeSummary found a deleted item in then changes");

            (deleteLogIter->second).setSequence((changeLogIter->second).getSequence());

            SettingList& slist = (changeLogIter->second).getSettings();

            for (int j=0 ; j < slist.size();j++)
            {
                for (int i=0;i<sl.size();i++)
                {
                    if (!strcmp(slist.get(j)->getProperty().getName(),
                                   sl.get(i)->getProperty().getName())
                                   
                                   
                        && slist.get(j)->getIndex() == 
                              sl.get(i)->getIndex())
                    {
                    // these are settings of the same prop/index, we
                    // need the old value to get transferred.
                        sl.remove(i);
                        sl.insert(i,new Setting(*(slist.get(j))));
                  
                    }
                }
            }
            LOGINFO(INFO, "ChangeSummary: Erasing from changes");
            changedMap.erase(ob);
        }
        // We append deleted objects to the changed list - this list gives 
        // those data objects which have been affected - we then look at their
        // current and old property values to find out whether they have been
        // deleted or created.
        else 
        {
            changedDataObjects.append(ob, ChangedDataObjectList::Delete);
        }
        
        LOGINFO(INFO,"ChangeSummary: Deletion being added to the list");

        return;
    }

    void ChangeSummaryImpl::logCreation(DataObjectImpl* ob,
        DataObjectImpl* container, const Property& prop)
    {

        LOGINFO_1(INFO,"ChangeSummary:LogCreate of type:%s",ob->getType().getName());

        // These we just need to record the fact that they
        // are present. We dont have an original value to
        // store.
        // log a change to the container of this object
        //

        DELETELOG_MAP::iterator deleteLogIter;

        deleteLogIter = deletedMap.find(ob);
        if (deleteLogIter != deletedMap.end())
        {
            // can happen - a delete is really a removal from the tree.
            // Adding back an object which you held a pointer to is just
            // a re-creation.
            LOGINFO(INFO,"ChangeSummary: Adding in a pre-deleted object");
        }

        // we should check if this object has sub-objects, they will
        // need to be created too

        PropertyList pl = ob->getInstanceProperties();
        for (int p=0;p<pl.size();p++)
        {
            Property& thisprop = pl[p];
            if (!thisprop.getType().isDataType())
            {
                if (ob->isSet(thisprop)) 
                {
                    DataObject* dp;

                    if (thisprop.isMany())
                    {
                        DataObjectList& dol = ob->getList(thisprop);
                        for (int ds = 0; ds < dol.size(); ds++) 
                        {
                            dp = dol[ds];
                            if (!dp) continue; 
                            logCreation((DataObjectImpl*)dp,ob,thisprop);
                        }
                    }
                    else 
                    {
                        dp = ob->getDataObject(thisprop); 
                        if (dp)
                        {  
                            logCreation((DataObjectImpl*)dp,ob,thisprop);
                        }
                    }
                }
            }
        }

        CREATELOG_MAP::iterator createLogIter;

        createLogIter = createdMap.find(ob);
        if (createLogIter != createdMap.end())
        {
            // this could be a reference - we dont add it twice.
            LOGINFO(INFO,"ChangeSummary: No need to log creation twice");
            return;
        }

        // We append created objects to the changed list - this list gives 
        // those data objects which have been affected - we then look at their
        // current and old property values to find out whether they have been
        // deleted or created.
        changedDataObjects.append(ob, ChangedDataObjectList::Create);

        createdMap.insert(std::make_pair(ob,createLogItem(ob->getType(),prop,container)));
        return;
    }


    void ChangeSummaryImpl::setPropValue(void** value, unsigned int* len, DataObjectImpl* ob, const Property& prop)
    {

		*len = 0;
        switch (prop.getTypeEnum())
        {
            case Type::BooleanType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = 0; 
                }
                else
                {
                    *(long*)*value = (long)ob->getBoolean(prop);
                }
                break;
            case Type::ByteType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = 0; 
                }
                else
                {
                    *(long*)*value = (long)ob->getByte(prop);
                }
                break;
            case Type::CharacterType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = 0; 
                }
                else
                {
                    *(long*)*value = (long)ob->getCharacter(prop);
                }
                break;
            case Type::IntegerType: 
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = 0; 
                }
                else
                {
                    *(long*)*value = (long)ob->getInteger(prop);
                }
                break;
            case Type::ShortType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = 0; 
                }
                else
                {
                    *(long*)*value = (long)ob->getShort(prop);
                }
                break;
            case Type::DoubleType:
                *value = new long double;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long double*)value = (long double)0; 
                }
                else
                {
                    *(long double*)*value = (long double)ob->getDouble(prop);
                }
                break;
            case Type::FloatType:
                *value = new float;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(float*)value = (float)0; 
                }
                else
                {
                    *(float*)*value = (float)ob->getFloat(prop);
                }
                break;
            case Type::LongType:
                *value = new int64_t;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(int64_t*)value = (int64_t)0; 
                }
                else
                {
                    *(int64_t*)*value = (int64_t)ob->getLong(prop);
                }
                break;
            case Type::DateType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = (long)0; 
                }
                else
                {
                    *(long*)*value = (long)(ob->getDate(prop).getTime());
                }
                break;
            case Type::BigDecimalType: 
            case Type::BigIntegerType: 
            case Type::StringType: 
            case Type::UriType:
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *value = 0;
                    *len = 0;
                }
                else
                {
                    unsigned int siz = ob->getLength(prop);
                    if (siz > 0) {
                        *value = new wchar_t[siz];
                        *len = ob->getString(prop,(wchar_t*)*value, siz);
                    }    
                    else {
                        *value = 0;
                        *len = 0;
                    }
                }
                break;
            case Type::BytesType:
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *value = 0;
                    *len = 0;
                }
                else
                {
                    unsigned int siz = ob->getLength(prop);
                    if (siz > 0) {
                        *value = new char[siz];
                        *len = ob->getBytes(prop,(char*)*value, siz);
                    }
                    else {
                        *value = 0;
                        *len = 0;
                    }
                }
                break;
            case Type::OtherTypes:
            case Type::DataObjectType:
            case Type::ChangeSummaryType:
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *value = 0; 
                }
                else
                {
                    *value = (void*)ob->getDataObject(prop);
                }
                break;
            default:
                SDO_THROW_EXCEPTION("(ChangeSummary)setPropValue" ,
                SDOUnsupportedOperationException, "Type is not recognised and cannot be saved");
                break;
        }
    }

    void ChangeSummaryImpl::setManyPropValue(void** value, unsigned int *len, DataObjectImpl* ob, 
                                                DataObjectImpl* listob, const Property& prop)
    {

		*len = 0;
        switch (prop.getTypeEnum())
        {
            case Type::BooleanType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = (long)0; 
                }
                else
                {
                    *(long*)*value = (long)listob->getBoolean();
                }
                break;
            case Type::ByteType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = (long)0; 
                }
                else
                {
                    *(long*)*value = (long)listob->getByte();
                }
                break;
            case Type::CharacterType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = (long)0; 
                }
                else
                {
                    *(long*)*value = (long)listob->getCharacter();
                }
                break;
            case Type::IntegerType: 
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = (long)0; 
                }
                else
                {
                    *(long*)*value = (long)listob->getInteger();
                }
                break;
            case Type::ShortType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = (long)0; 
                }
                else
                {
                    *(long*)*value = (long)listob->getShort();
                }
                break;
            case Type::DoubleType:
                *value = new long double;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long double*)value = (long double)0; 
                }
                else
                {
                    *(long double*)*value = (long double)listob->getDouble();
                }
                break;
            case Type::FloatType:
                *value = new float;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(float*)value = (float)0; 
                }
                else
                {
                    *(float*)*value = (float)listob->getFloat();
                }
                break;
            case Type::LongType:
                *value = new int64_t;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(int64_t*)value = (int64_t)0; 
                }
                else
                {
                    *(int64_t*)*value = (int64_t)listob->getLong();
                }
                break;
            case Type::DateType:
                *value = new long;
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *(long*)value = (long)0; 
                }
                else
                {
                    *(long*)*value = (long)(listob->getDate().getTime());
                }
                break;
            case Type::BigDecimalType: 
            case Type::BigIntegerType: 
            case Type::StringType: 
            case Type::UriType:
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *value = 0;
                    *len = 0; 
                }
                else
                {
                    unsigned int siz = listob->getLength(prop);
                    if (siz > 0) 
                    {
                        *value = new wchar_t[siz];
                        *len = listob->getString((wchar_t*)*value, siz);
                    }
                }
                break;
            case Type::BytesType:
                if (!ob->isSet(prop) && !prop.isDefaulted())
                {
                    *value = 0;
                    *len = 0; 
                }
                else
                {
                    unsigned int siz = listob->getLength(prop);
                    if (siz > 0) 
                    {
                        *value = new char[siz];
                        *len = listob->getBytes((char*)*value, siz);
                    }
                }
                break;

            case Type::OtherTypes:
            case Type::DataObjectType:
            case Type::ChangeSummaryType:
                SDO_THROW_EXCEPTION("(ChangeSummary)setManyPropValue" ,
                SDOUnsupportedOperationException, "A many prop data object value is being set");
                //*value = (void*)ob;
                break;
            default:
                SDO_THROW_EXCEPTION("(ChangeSummary)setManyPropValue" ,
                SDOUnsupportedOperationException, "Type is not recognised and cannot be saved");
                break;
        }
    }

    void ChangeSummaryImpl::logChange(DataObjectImpl* ob,
         const Property& prop)
    {
        // need to record the old value, unless the object
        // is in the created list, in which case we store 
        // nothing.

        LOGINFO_1(INFO,"ChangeSummary:LogChange %s",prop.getName());

        CREATELOG_MAP::iterator createLogIter;

        unsigned int len;

        createLogIter = createdMap.find(ob);
        if (createLogIter != createdMap.end())
        {
            LOGINFO(INFO,"ChangeSummary: no need to log change");
            return;
        }

        DELETELOG_MAP::iterator deleteLogIter;

        deleteLogIter = deletedMap.find(ob);
        if (deleteLogIter != deletedMap.end())
        {
            LOGINFO(INFO,"ChangeSummary: no need to log change - already deleted");
            return;
        }

        CHANGELOG_MAP::iterator changeLogIter;

        changeLogIter = changedMap.find(ob);
        if (changeLogIter == changedMap.end())
        {
            LOGINFO(INFO,"ChangeSummary: A change to an object which was not previously changed");
            changedMap.insert(std::make_pair(ob, changeLogItem(ob->getType(),prop, 
                              ob->getSequence(), ob)));
            changedDataObjects.append(ob, ChangedDataObjectList::Change);
        }
        else 
        {
            LOGINFO(INFO,"ChangeSummary: A change to an object which has already been changed");
        }

        changeLogIter = changedMap.find(ob);
        if (changeLogIter == changedMap.end())
        {
            LOGINFO(INFO,"ChangeSummary: Problem - no changes to append to");
            return;
        }

        SettingList& slist = (changeLogIter->second).getSettings();
    
        void* value;

        for (int i=0;i<slist.size();i++)
        {
            if (!strcmp(slist.get(i)->getProperty().getName(),prop.getName()))
            {
                LOGINFO(INFO,"ChangeSummary: Change of a property which was already changed - ignore");
                return;
            }
        }

        // need to check if the property has already been set,
        // There could be many Settings if the item was a list,
        // but we dont care here about that.

        if (!ob->isSet(prop))
        { 
            slist.append(new Setting(false,false,0,0,prop,0));
            return;
        }

        if (prop.isMany())
        {
            //  We are appending, not modifying
            //    we need to store the list as it was.

            LOGINFO(INFO,"ChangeSummary: logging a change to a many valued property");

            DataObjectList& dol = ob->getList(prop);
            if (dol.size() == 0)
            {
                slist.append(new Setting(false,false,0,0,prop,0));
                return;
            }
            for (int i=0;i< dol.size(); i++)
            {
                DataObject* dob = dol[i];
                if (prop.getType().isDataType()) {
                    setManyPropValue(&value, &len, (DataObjectImpl*)ob,
                        (DataObjectImpl*)dob, prop);
                    slist.append(new Setting(true,false,value,len,prop,i));
                }
                else{
                    value = (void*)dob;
                    slist.append(new Setting(true,false,value,0,prop,i));
                }
            }
        }
        else 
        {
            setPropValue(&value,&len,ob,prop);
            slist.append(new Setting(true,ob->isNull(prop),value,len,prop,0));
        }

        return;
    }



    void ChangeSummaryImpl::undoChanges()
    {
        // TODO 
        /* Plan for undoChanges:
        
        There are three lists of information. 
        The created list is a list of data objects which were created during logging.
        These data objects will have current state, and will represent the value of a 
        property of their containing object. As they had no state before they were 
        created, the undoChanges needs to unSet the property values for these first.

        The deleted list contains a list of objects which were deleted. The objects 
        themselves are not valid, but can be used to obtain a list of settings
        representing the values of the properties at the time the object was
        deleted. Here are recursive create is needed to re-create an object 
        similar to the deleted one, and set its properties using the settings. 
        The deleted object may well contain other deleted objects as its 
        property values, so these too will have entries in the deleted list, and
        need to be found and re-created.

        The changed list holds a list of settings for properties of data objects
        which have been changed. These objects may also be in the deleted list, so#
        may not be valid. They will not be in the created list. First check that
        the object is not in the deleted list. If it is, then the changes need to
        be applied to the data object which we created when undoing the deleted list,
        otherwise we just apply the change to a data object which exists.
        */

        /* what about items in many-valued properties? I guess we need to check and
        search the values returned for the list to find the object to delete*/
        LOGINFO(INFO,"ChangeSummary UndoChanges not implemented");
        return;
    }

    
    void ChangeSummaryImpl::beginLogging()
    {

        // Clear down the lists and their contents, if there was already
        // some activity.

        if (logging) endLogging();
        changedMap.clear();
        deletedMap.clear();
        createdMap.clear();
        changedDataObjects.clear();

        LOGINFO(INFO,"ChangeSummary: logging switched on");

        logging = true;
          return;
    }

    void ChangeSummaryImpl::endLogging()
    {
        logging = false;

        LOGINFO(INFO,"ChangeSummary: logging switched off");

          return;
    }

    bool ChangeSummaryImpl::isLogging()
    {
          return logging;
    }

///////////////////////////////////////////////////////////////////////////////////////
    void ChangeSummaryImpl::debugPrint(){}
//    {
//        ChangedDataObjectList& dol = getChangedDataObjects();
//
//        CREATELOG_MAP::iterator createLogIter;
//        DELETELOG_MAP::iterator deleteLogIter;
//        CHANGELOG_MAP::iterator changeLogIter;
//
//        for (int i=0;i<dol.size(); i++)
//        {
//            DataObject* dob = dol[i];
//            createLogIter = createdMap.find((DataObjectImpl*)dob);
//            if (createLogIter != createdMap.end())
//            {
//                cout << "Found a created object " << dol[i] << endl;
//                return;
//            }
//            deleteLogIter = deletedMap.find((DataObjectImpl*)dob);
//            if (deleteLogIter != deletedMap.end())
//            {
//                cout << "Found a deleted object " << dol[i] << endl;
//                return;
//            }
//            changeLogIter = changedMap.find((DataObjectImpl*)dob);
//            if (changeLogIter != changedMap.end())
//            {
//                cout << "Found a modified object " << dol[i] << endl;
//                SettingList& sl = getOldValues(dob);
//                for (int j=0; j < sl.size(); j++)
//                {
//                    cout << "Old Value of " << sl[j].getProperty().getName();
//                    if (sl[j].getProperty().isMany())
//                    {
//                        cout << "[" << sl[j].getIndex() << "]" ;
//                    }
//                    cout <<  endl;
//                    switch (sl[j].getProperty().getTypeEnum()) 
//                    {
//                        case Type::BooleanType:
//                        cout << "Boolean:" << sl[j].getBooleanValue();
//                        break;
//                        case Type::ByteType:
//                        cout << "Byte:" << sl[j].getByteValue();
//                        break;
//                        case Type::CharacterType:
//                            cout << "Character:" << sl[j].getCharacterValue();
//                        break;
//                        case Type::IntegerType: 
//                            cout << "Integer:" << sl[j].getIntegerValue();
//                        break;
//                        case Type::ShortType:
//                            cout << "Short:" << sl[j].getShortValue();
//                        break;
//                        case Type::DoubleType:
//                            cout << "Double:" << sl[j].getDoubleValue();
//                        break;
//                        case Type::FloatType:
//                            cout << "Float:" << sl[j].getFloatValue();
//                        break;
//                        case Type::LongType:
//                            cout << "Int64: (cant print)"; // << (*sl)[j]->getLongValue();
//                        break;
//                        case Type::DateType:
//                            cout << "Date:" << sl[j].getDateValue().getTime();
//                        break;
//                        case Type::BigDecimalType: 
//                        case Type::BigIntegerType: 
//                        case Type::StringType: 
//                        case Type::UriType:
//                            {
//                            unsigned int len = sl[j].getLength();
//                            if (len > 0) {
//                                wchar_t* buf = new wchar_t[len];
//                                len = sl[j].getStringValue(buf,len);
//                                cout <<"String type" ;
//                                for (int i=0;i<len;i++) 
//                                {
//                                    cout << ":" << buf[i];
//                                }
//                                delete buf;
//                            }
//                            break;
//                            }
//                        case Type::BytesType:
//                            {
//                            unsigned int len = sl[j].getLength();
//                            if (len > 0) {
//                                char* buf = new char[len];
//                                len = sl[j].getBytesValue(buf,len);
//                                cout <<"Bytes type" ;
//                                for (int i=0;i<len;i++) 
//                                {
//                                    cout << buf[i];
//                                }
//                                delete buf;
//                            }
//                            break;
//                            }
//                        case Type::OtherTypes:
//                        case Type::DataObjectType:
//                        case Type::ChangeSummaryType:
//                            cout << "DataObject:" << sl[j].getDataObjectValue();
//                        break;
//                        default:
//                            cout << "Unspecified type found in setting";
//                        break;
//                    }
//                    cout << endl;
//                }
//                return;
//            }
//            cout << "Found an object which was not in the changes " << dol[i] << endl;
//
//        }
//    }
///////////////////////////////////////////////////////////////////////////////////////
 
    ChangedDataObjectList&  ChangeSummaryImpl::getChangedDataObjects()
    {
        // build a list of all the changes, in the same order
        // as the actions occured,

        return changedDataObjects;

    }

    bool ChangeSummaryImpl::isCreated(DataObjectPtr dol)
    {
        CREATELOG_MAP::iterator createLogIter;
        DataObject* dob = dol;
        createLogIter = createdMap.find((DataObjectImpl*)dob);
        
        if (createLogIter != createdMap.end())
            return true;
        return false;
    }

    bool ChangeSummaryImpl::isDeleted(DataObjectPtr dol)
    {
        DELETELOG_MAP::iterator deleteLogIter;
        DataObject* dob = dol;
        deleteLogIter = deletedMap.find((DataObjectImpl*)dob);
        
        if (deleteLogIter != deletedMap.end())
            return true;
        return false;
    }

    bool ChangeSummaryImpl::isModified(DataObjectPtr dol)
    {
        CHANGELOG_MAP::iterator changeLogIter;
        DataObject* dob = dol;
        changeLogIter = changedMap.find((DataObjectImpl*)dob);
        
        if (changeLogIter != changedMap.end())
            return true;
        return false;
    }  
    

 
    SettingList& ChangeSummaryImpl::getOldValues(DataObjectPtr dol)
    {
        CHANGELOG_MAP::iterator changeLogIter;
        DELETELOG_MAP::iterator deleteLogIter;
        DataObject* dob = dol;
        // start with the deleted map...
        deleteLogIter = deletedMap.find((DataObjectImpl*)dob);
        if (deleteLogIter != deletedMap.end()){
            return ((deleteLogIter->second).getSettings());
        }
        changeLogIter = changedMap.find((DataObjectImpl*)dob);
        if (changeLogIter != changedMap.end()){
            return ((changeLogIter->second).getSettings());
        }
        SDO_THROW_EXCEPTION("(ChangeSummary(getOldValues" ,
        SDOIndexOutOfRangeException, "Data object is not in the change summary");
    } 



    const char* ChangeSummaryImpl::getOldXpath(RefCountingPointer<commonj::sdo::DataObject> dol)
    {
        DELETELOG_MAP::iterator deleteLogIter;
        DataObject* dob = dol;
        deleteLogIter = deletedMap.find((DataObjectImpl*)dob);
        if (deleteLogIter != deletedMap.end()){
            return ((deleteLogIter->second).getOldXpath());
        }
        return 0;
    } 


    const Setting& ChangeSummaryImpl::getOldValue(DataObjectPtr dol, const Property& property)
    { 
        DELETELOG_MAP::iterator deleteLogIter;
        CHANGELOG_MAP::iterator changeLogIter;
        DataObject* dob = dol;

        deleteLogIter = deletedMap.find((DataObjectImpl*)dob);
        if (deleteLogIter != deletedMap.end())
        {
            SettingList& sl = (deleteLogIter->second).getSettings();
            for (int i=0;i < sl.size(); i++)
            {
                if (!strcmp(property.getName(),
                    sl.get(i)->getProperty().getName()))
                    return (*sl.get(i));
            }
        }

        changeLogIter = changedMap.find((DataObjectImpl*)dob);
        if (changeLogIter != changedMap.end())
        {
            SettingList& sl = (changeLogIter->second).getSettings();
            for (int i=0;i < sl.size(); i++)
            {
                if (!strcmp(property.getName(),
                    sl.get(i)->getProperty().getName()))
                    return (*sl.get(i));
            }
        }

        SDO_THROW_EXCEPTION("(ChangeSummary(getOldValue)" ,
        SDOIndexOutOfRangeException, "Data object is not in the change summary");
    }

    DataObjectPtr ChangeSummaryImpl::getOldContainer(DataObjectPtr dol)
    {
        CHANGELOG_MAP::iterator changeLogIter;
        DataObject* dob = dol;
        changeLogIter = changedMap.find((DataObjectImpl*)dob);
       
        if (changeLogIter != changedMap.end())
        {
            return (changeLogIter->second).getOldContainer();
        }
          return 0;
    }

    const Property& ChangeSummaryImpl::getOldContainmentProperty(DataObjectPtr dol)
      {
        CHANGELOG_MAP::iterator changeLogIter;
        DataObject* dob = dol;
        changeLogIter = changedMap.find((DataObjectImpl*)dob);
       
        if (changeLogIter == changedMap.end())
        {
            SDO_THROW_EXCEPTION("(ChangeSummary(getOldContainmentProperty)" ,
            SDOIndexOutOfRangeException, "Data object is not in the change summary");
        }
        return (changeLogIter->second).getOldContainmentProperty();
    }


    // 
    // Creations and deletions can be just logged in the normal
    // way. Changes have to be handled specially, because we
    // need the old value, but the data object now contains the
    // current value.
    //

    void ChangeSummaryImpl::appendToCreations(const Property& p, 
        DataObjectPtr    dob, DataObjectPtr cont)
    {
        LOGINFO(INFO,"ChangeSummary: Appending to creations list");
        DataObject* pdob = (DataObject*)dob;
        DataObject* pcont = (DataObject*)cont;
        logCreation((DataObjectImpl*)pdob,(DataObjectImpl*)pcont,p);
        return;
    }


    void ChangeSummaryImpl::appendToDeletions(const Property& p,
        DataObjectPtr    dob, DataObjectPtr cont, const char* oldpath )
    {
        DELETELOG_MAP::iterator deleteLogIter;

        LOGINFO_1(INFO,"ChangeSummary:Appending to deletions list %s",oldpath);

        DataObject* pdob = (DataObject*)dob;
        DataObject* pcont = (DataObject*)cont;
        logDeletion((DataObjectImpl*)pdob,(DataObjectImpl*)pcont,p,
            oldpath, false);
        return;
    }


    unsigned int ChangeSummaryImpl::stringConvert(char** value, const char* c, const Property& p)
    {
    // code duplicated from typeimpl
    switch (p.getTypeEnum())
        {
        case Type::BooleanType:
            *value = new char[sizeof(long)];
            if ((c == 0) || strcmp(c,"true"))
            {
                *(long*)*value = 0;
            }
            else 
            {
                *(long*)*value = 1;
            }
            return sizeof(long);

        case Type::ByteType:
        case Type::CharacterType: 
        case Type::IntegerType: 
        case Type::ShortType:
            *value = new char[sizeof(long)];
              *(long*)*value = atoi(c);
            return sizeof(long);

        case Type::DoubleType:
            *value = new char[sizeof(long double)];
            // TODO - atof not suitable here
            *(long double*)*value = (long double)atof(c);
            return sizeof(long double);

        case Type::FloatType:    
            *value = new char[sizeof(float)];
            *(float*)*value = (float)atof(c);
            return sizeof(float);

        case Type::LongType:   
            *value = new char[sizeof(int64_t)];
#if defined(WIN32)  || defined (_WINDOWS)
            *(int64_t*)*value = (int64_t)_atoi64(c);
#else 
            *(int64_t*)*value = (int64_t)strtoll(c, NULL, 0);
#endif
            return sizeof(int64_t);

        case Type::DateType:
            *value = new char[sizeof(time_t)];
            *(time_t*)*value = (time_t)atoi(c);
            return sizeof(time_t);
            
        case Type::BigDecimalType: 
        case Type::BigIntegerType: 
        case Type::StringType:    
        case Type::UriType:
            {
                char* vw = new char[(strlen(c)+1)*sizeof(wchar_t)];                
                wchar_t* pvw = (wchar_t*)vw;
                for (int i=0;i< strlen(c);i++)
                {
                    pvw[i] = (wchar_t)c[i];
                }
                pvw[strlen(c)] = 0;
                *value = vw;
                return strlen(c);
            }
            break;
        case Type::BytesType:
            {
                char* vc = new char[strlen(c)+1];
                for (int i=0;i< strlen(c);i++)
                {
                    vc[i] =  (char)c[i];
                }
                vc[strlen(c)] = 0;
                *value = vc;
                return strlen(c);
            }
            break;
             
        case Type::OtherTypes:
        case Type::DataObjectType: 
        case Type::ChangeSummaryType:
        default:
            {
            std::string msg("Cannot set CString on object of type:");
            msg += p.getType().getName();
            SDO_THROW_EXCEPTION("setString" ,
              SDOInvalidConversionException, msg.c_str());
            break;
            }
        }
        return 0;
    }

    void ChangeSummaryImpl::appendToChanges(const Property& p, 
        DataObjectPtr dob , SDOXMLString value, int index)
    {
        CHANGELOG_MAP::iterator changeLogIter;

        LOGINFO(INFO,"ChangeSummary Appending to changes list");

        DataObject* pdob = (DataObject*)dob;
        

        changeLogIter = changedMap.find((DataObjectImpl*)pdob);
        if (changeLogIter == changedMap.end())
        {

            LOGINFO(INFO,"ChangeSummary: A change to an object which was not previously changed");
            
            changedMap.insert(std::make_pair((DataObjectImpl*)pdob, 
                              changeLogItem(dob->getType(),p, 
                              dob->getSequence(), (DataObjectImpl*)pdob)));
            changedDataObjects.append((DataObjectImpl*)pdob, 
                                       ChangedDataObjectList::Change);
        }

        changeLogIter = changedMap.find((DataObjectImpl*)pdob);
        if (changeLogIter == changedMap.end())
        {

            LOGINFO(INFO,"ChangeSummary: no changes to append to");

            return;
        }

        SettingList& slist = (changeLogIter->second).getSettings();

        // simply need to insert a setting - no requirement to validate
        // against existing settings

        char* datavalue = 0;

        unsigned int len = stringConvert(&datavalue, (const char*)value , p);

        slist.append(new Setting(true,false,datavalue,len,p,index));
        
        // The datavalue will be freed by the deletion of the setting later
     }


    void ChangeSummaryImpl::appendToChanges(const Property& p, 
        DataObjectPtr dob , DataObjectPtr indob, int index)
    {
        CHANGELOG_MAP::iterator changeLogIter;

        LOGINFO(INFO,"ChangeSummary Appending to changes list (2)");

        DataObject* pdob = (DataObject*)dob;

        changeLogIter = changedMap.find((DataObjectImpl*)pdob);
        if (changeLogIter == changedMap.end())
        {
            LOGINFO(INFO,"ChangeSummary: A change to an object which was not previously changed");
            
            changedMap.insert(std::make_pair((DataObjectImpl*)pdob, 
                              changeLogItem(dob->getType(),p, 
                              dob->getSequence(), (DataObjectImpl*)pdob)));
            changedDataObjects.append((DataObjectImpl*)pdob, 
                                       ChangedDataObjectList::Change);
        }

        changeLogIter = changedMap.find((DataObjectImpl*)pdob);
        if (changeLogIter == changedMap.end())
        {
            LOGINFO(INFO,"ChangeSummary: Problem - no changes to append to");

            return;
        }

        SettingList& slist = (changeLogIter->second).getSettings();

        // simply need to insert a setting - no requirement to validate
        // against existing settings


        slist.append(new Setting(true,false,(void*)indob,0,p,index));
    
     }



    
    /////////////////////////////////////////////////////////////
    // log items are for all lists, but contain different things
    // depending on whether they are deletions, additions...
    /////////////////////////////////////////////////////////////

    createLogItem::createLogItem(const Type& tp, const Property& prop,
         DataObjectImpl* cont) :
            theOldContainmentProperty(prop), theOldContainer(cont),
            theType(tp)
    {
    }

    DataObjectImpl* createLogItem::getOldContainer()
    {
        return theOldContainer;
    }

    const Type& createLogItem::getOldType()
    {
        return theType;
    }

    const Property& createLogItem::getOldContainmentProperty()
    {
        return  theOldContainmentProperty;
    }



    changeLogItem::changeLogItem(const Type& tp, 
        const Property& prop,
        SequencePtr seq, 
        DataObjectImpl* cont) :
            theOldContainmentProperty(prop), theOldContainer(cont),
            theType(tp)
    {
        if (seq) 
        {
            theSequence = new SequenceImpl((SequenceImpl*)seq);
        }
        else 
        {
            theSequence = 0;
        }
    }

    changeLogItem::changeLogItem(const changeLogItem& cin) :
        theOldContainmentProperty(cin.theOldContainmentProperty),
        theType(cin.theType)
    {
        if (cin.theSequence) theSequence = new SequenceImpl((SequenceImpl*)cin.theSequence);
        else theSequence = 0;
        theOldContainer = cin.theOldContainer;
        theSettings = cin.theSettings;

    }

    changeLogItem::~changeLogItem()
    {
        if (theSequence) delete theSequence;
    }

    DataObjectImpl* changeLogItem::getOldContainer()
    {
        return theOldContainer;
    }

    const Type& changeLogItem::getOldType()
    {
        return theType;
    }

    const Property& changeLogItem::getOldContainmentProperty()
    {
        return  theOldContainmentProperty;
    }

    SettingList& changeLogItem::getSettings()
    {
        return theSettings;
    }

    SequencePtr changeLogItem::getSequence()
    {
        return theSequence;
    }


    deleteLogItem::deleteLogItem(DataObject* dob, 
         const Property& prop, SequencePtr seq ,
         const char* oldpath, DataObjectImpl* cont
         ) :
            theOldContainmentProperty(prop), theOldContainer(cont),
            theActualObject(dob),
            theType(dob->getType())
    {
        if (seq) 
        {
            theSequence = new SequenceImpl((SequenceImpl*)seq);
        }
        else 
        {
            theSequence = 0;
        }
        if (oldpath !=0  && strlen(oldpath) != 0) 
        {
            theOldXpath = new char[strlen(oldpath)+1];
            strcpy(theOldXpath,oldpath);
        }
        else 
        {
            theOldXpath = 0;
        }
    }

    const char* deleteLogItem::getOldXpath()
    {
        return theOldXpath;
    }

    DataObjectImpl* deleteLogItem::getOldContainer()
    {
        return theOldContainer;
    }

    deleteLogItem::deleteLogItem(const deleteLogItem& cin):
        theOldContainmentProperty(cin.theOldContainmentProperty),
        theType(cin.theType), theActualObject(cin.theActualObject)
    {
        if (cin.theSequence) theSequence = new SequenceImpl((SequenceImpl*)cin.theSequence);
        else theSequence = 0;
        theOldContainer = cin.theOldContainer;
        theSettings = cin.theSettings;
        if (cin.theOldXpath != 0)
        {
            theOldXpath = new char[strlen(cin.theOldXpath) + 1];
            strcpy(theOldXpath, cin.theOldXpath);
        }
        else
        {
            theOldXpath = 0;
        }
    }

    deleteLogItem::~deleteLogItem()
    {
        theActualObject = 0;
        if (theSequence) delete theSequence;
        if (theOldXpath != 0) 
        {
            delete theOldXpath;
        }
    }

    const Type& deleteLogItem::getOldType()
    {
        return theType;
    }

    const Property& deleteLogItem::getOldContainmentProperty()
    {
        return  theOldContainmentProperty;
    }

    SettingList& deleteLogItem::getSettings()
    {
        return theSettings;
    }

    SequencePtr deleteLogItem::getSequence()
    {
        return theSequence;
    }

    void deleteLogItem::setSequence(SequencePtr s)
    {
        if (theSequence) delete theSequence;
        if (s) {
            theSequence = new SequenceImpl((SequenceImpl*)s);
        }
        else {
            theSequence = 0;
        }
        return; 
    }

};
};

