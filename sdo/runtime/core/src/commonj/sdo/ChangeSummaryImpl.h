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

#ifndef _CHANGESUMMARYIMPL_H_
#define _CHANGESUMMARYIMPL_H_

#include "commonj/sdo/disable_warn.h"


#include "commonj/sdo/ChangedDataObjectListImpl.h"
#include "commonj/sdo/SettingList.h"
#include "commonj/sdo/ChangeSummary.h"
#include "commonj/sdo/RefCountingPointer.h"
#include "commonj/sdo/SDOXMLString.h"

#include <map>

using namespace std;
namespace commonj{
namespace sdo {


    /** 
     * ChangeLogItem is a part of a change summary.
     * The change summary consisists of change items, 
     * deletion items and creation items. They are held in three
     * lists as they have slightly different contents.
     * The change log items hold old values for all the changes to
     * properties of the data object concerned.
     */

    class changeLogItem {

    public:

        changeLogItem(const Type& tp, const Property& prop,SequencePtr seq, 
            DataObjectImpl* cont = 0 );
        changeLogItem(const changeLogItem& cin); 
        ~changeLogItem();
        DataObjectImpl* getOldContainer();
        const Property& getOldContainmentProperty();
        const Type& getOldType();
        SettingList& getSettings();
        SequencePtr getSequence();

    private:

        const Property& theOldContainmentProperty;
        DataObjectImpl* theOldContainer;
        const Type& theType;
        SettingList theSettings;
        SequencePtr theSequence;
    };

    /** 
     * CreateLogItem is a part of a change summary.
     * The change summary consisists of change items, 
     * deletion items and creation items. They are held in three
     * lists as they have slightly different contents.
     * The create log items hold the data object itself,
     * and details of its container.
     */


    class createLogItem {

    public:

        createLogItem(const Type& tp, const Property& prop, DataObjectImpl* cont = 0);


        DataObjectImpl* getOldContainer();
        const Property& getOldContainmentProperty();
        const Type& getOldType();

    private:

        const Property& theOldContainmentProperty;
        DataObjectImpl* theOldContainer;
        const Type& theType;
    };

    /** 
     * DeleteLogItem is a part of a change summary.
     * The change summary consisists of change items, 
     * deletion items and creation items. They are held in three
     * lists as they have slightly different contents.
     * The delete log items hold the old data object , and
     * details of the properties as they were before deletion
     */

    class deleteLogItem {

    public:


        deleteLogItem(DataObject* dob,  const Property& prop,
                        SequencePtr seq,const char *oldpath,
                        DataObjectImpl* cont = 0);


        deleteLogItem(const deleteLogItem& cin); 
        ~deleteLogItem();
        DataObjectImpl* getOldContainer();
        const Property& getOldContainmentProperty();
        const Type& getOldType();
        const char* getOldXpath();
        SettingList& getSettings();
        SequencePtr getSequence();
        void setSequence(SequencePtr s);

    private:

        const Property& theOldContainmentProperty;
        DataObjectImpl* theOldContainer;

        /** theActualObject
         * A counting pointer to the object is held, such that the 
         * object is not freed, even if deleted. This rcp will not
         * be used to refer to the object, but makes sure that the
         * object does not reuse a memory address of a previously
         * deleted object until the change summary is destroyed.
         */

        RefCountingPointer<DataObject> theActualObject;
        const Type& theType;
        SettingList theSettings;
        SequencePtr theSequence;
        char * theOldXpath;
    };


    /** ChangeSummaryImpl implements the abstract class ChangeSummary.
     * The change summary consisists of change items, 
     * deletion items and creation items. They are held in three
     * lists as they have slightly different contents.
     */
 
    class ChangeSummaryImpl : public ChangeSummary
    {
      public:
        ChangeSummaryImpl();

        virtual ~ChangeSummaryImpl();

    /** 
    * The new and modified objects in the list are references to objects that
    * are associated with this change summary. 
    * The deleted objects in the list are references to copies of the objects 
    * as they appeared at the time that event logging was enabled; 
    * if the deleted objects have references to other objects, 
    * the references will also refer to copies of the target objects.
    * Return a list of changed data objects.
       */
    
    virtual ChangedDataObjectList&  getChangedDataObjects();


    /** getOldSettings returns a list of settings.
    * Returns a list of Settings 
    * that represent the property values of the given dataObject
    * at the point when logging began.
    * In the case of a deleted object, 
    * the list will include Settings for all the properties.
    * An old value Setting indicates the value at the
    * point logging begins.  A setting is only produced for 
    * modified objects if either the old value differs from the current value or
    * if the isSet differs from the current value. 
    * No Settings are produced for created objects.
    * Param dataObject the object in question.
    * Return a list of settings.
      */
    virtual SettingList& getOldValues(DataObjectPtr dataObject);

    /**  getOldXpath returns the old path before the setting.
     *
     * Returns a path where the object was located before the setting took place.
       */
    virtual const char* getOldXpath(RefCountingPointer<commonj::sdo::DataObject> dol);


    /**  beginLogging() sets the change summary in logging mode.
     *
     * The change summary is initially inactive. Setting it to logging mode
     * starts the process of recording all the settings made.
     */
    
    virtual void beginLogging();

     /**  endLogging() stops logging.
     *
     * This method stops logging, but does not clear out the change summary.
     * The changes are valid until the summary is next set to logging, at which time
     * all old settings are lost.
     */
    virtual void endLogging();

  
  
    /**  isLogging() returns true if logging is enabled.
     *
     * Indicates whether change logging is on (true) or off (false).
       */

    virtual bool isLogging();


  
   /**  isCreated() returns true if the object was created during logging.
    *
    * Returns whether or not the specified data object was created while logging.
    * Any object that was added to the data graph
    * but was not in the data graph when logging began, 
    * will be considered created.
    * Param dataObject the data object in question.
    * Return true if the specified data object was created.
     */
    virtual bool isCreated(DataObjectPtr dataObject);

    /**  isDeleted() - true if the object was deleted during logging.
     *
     * Returns whether or not the specified data object was deleted while logging.
     * Any object that is not contained by the data graph will be considered 
     * deleted.
     * Param dataObject the data object in question.
     * Return true if the specified data object was deleted.
      */
    virtual bool isDeleted(DataObjectPtr dataObject);


     /**  isModified() - true if the object was changed during logging.
     *
     * Returns whether or not the specified data object was updated while logging.
     * An object that was contained in the data graph when logging began, 
     * and remains in the graph when logging ends will be considered for changes.
     * An object considered modified must have at least one old value Setting.
     * Param dataObject the data object in question.
     * Return true if the specified data object was modified.
      */
    virtual bool isModified(DataObjectPtr dataObject);   

    /**  getOldValue() gets the setting showing the old value.
     *
     * Returns a setting for the specified property
     * representing the property value of the given dataObject
     * at the point when logging began.
     * Returns null if the property has not changed and 
     * has not been deleted. 
     * Param dataObject the object in question.
     * Param property the property of the object.
     * Return the Setting for the specified property.
      */

    virtual const Setting& getOldValue(DataObjectPtr dataObject, const Property& property);

     /**  getOldContainer() gets the container before the setting.
     *
     * Returns the value of the container data object
     * at the point when logging began.
     * Param dataObject the object in question.
     * Return the old container data object.
        */
    virtual DataObjectPtr getOldContainer(DataObjectPtr dataObject);

    /**  getOldContainmentProperty() - gives the property before setting.
     *
     * Returns the value of the containment property data object property
     * at the point when logging began.
     * Param dataObject the object in question.
     * Return the old containment property.
     */
    virtual const Property& getOldContainmentProperty(DataObjectPtr dataObject);  

     /**  undoChanges() - Not Implemented.
     *
     * NOT IMPLEMENTED
     *
     * Undoes all changes in the log to restore the tree of 
     * DataObjects to its original state when logging began.
     * isLogging() is unchanged.  The log is cleared.
      */
    virtual void undoChanges();

    /**  getOldSequence() - Not Implemented.
     *
     * NOT IMPLEMENTED
     *
     * This method gives back the sequence of a data object as it
     * appeared when logging was switched on. The data object may be
     * a deleted data object or a changed data object. If the
     * data object was not sequenced, this returns null.
      */

    virtual SDO_API SequencePtr getOldSequence(DataObjectPtr dataObject);

    bool isInCreatedMap(DataObjectImpl* ob);

    void logDeletion(DataObjectImpl* ob,
                     DataObjectImpl* cont, const Property&  prop,
                     const char* oldpath,
                     bool loggingChildren = true
                     );

    void logCreation(DataObjectImpl* ob,
                    DataObjectImpl* cont, const Property& prop
                    );

    void logChange(DataObjectImpl* ob, const Property& prop
                    );


    SDO_API void debugPrint();

    void removeFromChanges(DataObjectImpl* ob);


    /**
    * appendToCreations is used by deserialization
    * This method helps to rebuild a change summary by populating the 
    * lists without the changes actually being logged.
    */

    void appendToCreations(const Property& p, 
                            DataObjectPtr    dob,
                            DataObjectPtr    cont);


    /**
    * appendToDeletions is used by deserialization
    * This method helps to rebuild a change summary by populating the 
    * lists without the changes actually being logged.
    */
    void appendToDeletions(const Property& p,
                            DataObjectPtr dob, 
                            DataObjectPtr cont,
                            const char* oldpath);


    unsigned int stringConvert(
                            char** value, 
                            const char* c, 
                            const Property& p);

    /**
    * appendToChangesis used by deserialization
    * This method helps to rebuild a change summary by populating the 
    * lists without the changes actually being logged.
    */

    void appendToChanges(const Property& p, 
                            DataObjectPtr dob, 
                            SDOXMLString value,
                            int index);

    void appendToChanges(const Property& p, 
                            DataObjectPtr dob, 
                            DataObjectPtr pdob,
                            int index);

    DataObjectPtr matchDeletedObject(SDOXMLString path);


    private:


        void setPropValue(void** value, unsigned int *len, DataObjectImpl* ob, const Property& prop);
        void setManyPropValue(void** value, unsigned int *len, DataObjectImpl* ob, 
            DataObjectImpl* listob, const Property& prop);
        bool logging;

        typedef std::map<DataObjectImpl*, createLogItem>    CREATELOG_MAP;
        typedef std::map<DataObjectImpl*, deleteLogItem>    DELETELOG_MAP;
        typedef std::map<DataObjectImpl*, changeLogItem>    CHANGELOG_MAP;

        CHANGELOG_MAP changedMap;
        CREATELOG_MAP createdMap;
        DELETELOG_MAP deletedMap;

        ChangedDataObjectListImpl changedDataObjects;





};
};
};
#endif //_CHANGESUMMARYIMPL_H_
