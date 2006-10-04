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

#ifndef _CHANGESUMMARY_H_
#define _CHANGESUMMARY_H_
#include "commonj/sdo/export.h"


#include "commonj/sdo/SettingList.h"
#include "commonj/sdo/ChangedDataObjectList.h"


namespace commonj{
namespace sdo {

    /** 
     *
     * A change summary is used to record changes to the objects in a data graph.
     * This allows applications to efficiently and incrementally update 
     * back-end storage when required.
      */
 
    class ChangeSummary 
    {
      public:

    /**  getChangedDataObjects() returns a list of changed objects.    
     * 
     * The new and modified objects in the list are references to objects that
     * are associated with this change summary. 
     * The deleted objects in the list are references to copies of the objects 
     * as they appeared at the time that event logging was enabled; 
     * if the deleted objects have references to other objects, 
     * the references will also refer to copies of the target objects.
     * Return a list of changed data objects.
        */

    virtual SDO_API ChangedDataObjectList&  getChangedDataObjects() = 0;


    /**  getOldValues returns a list of old values as settings.
     *
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

    virtual SDO_API SettingList& getOldValues(DataObjectPtr dataObject) = 0;
    
    /**  getOldXpath returns the old path before the setting.
     *
     * Returns a path where the object was located before the setting took place.
       */
    
    
    virtual SDO_API const char* getOldXpath(RefCountingPointer<commonj::sdo::DataObject> dol) = 0;

     /**  beginLogging() sets the change summary in logging mode.
     *
     * The change summary is initially inactive. Setting it to logging mode
     * starts the process of recording all the settings made.
     */

    virtual SDO_API void beginLogging() = 0;

     /**  endLogging() stops logging.
     *
     * This method stops logging, but does not clear out the change summary.
     * The changes are valid until the summary is next set to logging, at which time
     * all old settings are lost.
     */

    virtual SDO_API void endLogging() = 0;

  
  
     /**  isLogging() returns true if logging is enabled.
     *
     * Indicates whether change logging is on (true) or off (false).
       */
    
    virtual SDO_API bool isLogging() = 0;


  
   /**  isCreated() returns true if the object was created during logging.
    *
    * Returns whether or not the specified data object was created while logging.
    * Any object that was added to the data graph
    * but was not in the data graph when logging began, 
    * will be considered created.
    * Param dataObject the data object in question.
    * Return true if the specified data object was created.
     */

    virtual SDO_API bool isCreated(DataObjectPtr dataObject) = 0;

     /**  isDeleted() - true if the object was deleted during logging.
     *
     * Returns whether or not the specified data object was deleted while logging.
     * Any object that is not contained by the data graph will be considered 
     * deleted.
     * Param dataObject the data object in question.
     * Return true if the specified data object was deleted.
      */

    virtual SDO_API bool isDeleted(DataObjectPtr dataObject) = 0;


     /**  isModified() - true if the object was changed during logging.
     *
     * Returns whether or not the specified data object was updated while logging.
     * An object that was contained in the data graph when logging began, 
     * and remains in the graph when logging ends will be considered for changes.
     * An object considered modified must have at least one old value Setting.
     * Param dataObject the data object in question.
     * Return true if the specified data object was modified.
      */

    virtual SDO_API bool isModified(DataObjectPtr dataObject) = 0;   

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

    virtual SDO_API const Setting& getOldValue(DataObjectPtr dataObject, const Property& property) = 0;

     /**  getOldContainer() gets the container before the setting.
     *
     * Returns the value of the container data object
     * at the point when logging began.
     * Param dataObject the object in question.
     * Return the old container data object.
        */

    virtual SDO_API DataObjectPtr getOldContainer(DataObjectPtr dataObject) = 0;

     /**  getOldContainmentProperty() - gives the property before setting.
     *
     * Returns the value of the containment property data object property
     * at the point when logging began.
     * Param dataObject the object in question.
     * Return the old containment property.
     */

    virtual SDO_API const Property& getOldContainmentProperty(DataObjectPtr dataObject) = 0;  

     /**  undoChanges() - Not Implemented.
     *
     * NOT IMPLEMENTED
     *
     * Undoes all changes in the log to restore the tree of 
     * DataObjects to its original state when logging began.
     * isLogging() is unchanged.  The log is cleared.
      */

    virtual SDO_API void undoChanges() = 0;

    /**  getOldSequence() - Not Implemented.
     *
     * NOT IMPLEMENTED
     *
     * This method gives back the sequence of a data object as it
     * appeared when logging was switched on. The data object may be
     * a deleted data object or a changed data object. If the
     * data object was not sequenced, this returns null.
      */

    virtual SDO_API SequencePtr getOldSequence(DataObjectPtr dataObject) = 0;

};
};
};
#endif //_CHANGESUMMARY_H_
