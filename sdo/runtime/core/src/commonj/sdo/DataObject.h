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

/* $Rev$ $Date: 2006/02/02 16:41:30 $ */

#ifndef _DATAOBJECT_H_
#define _DATAOBJECT_H_
#include "commonj/sdo/export.h"


#include "commonj/sdo/Property.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/DataFactory.h"
#include "commonj/sdo/Sequence.h"
#include "commonj/sdo/DataObjectList.h"
#include "commonj/sdo/PropertyList.h"


namespace commonj{
namespace sdo{
    
class DataGraph; 
class DataObjectList;
class DataFactory;
class ChangeSummary;


 /**  
  *  DataObjects are the non-primitive members of a Data graph.
  *<p>
  * A data object is a representation of some structured data. 
  * it is the fundamental component in the SDO (Service Data Objects) package.
  * Data objects support reflection, path-based accesss, convenience creation 
  * and deletion methods,and the ability to be part of a data graph.
  * Each data object holds its data as a series of properties. 
  * Properties can be accessed by name, property index, or using the property 
  * meta object itself. 
  * <p>
  * A data object can also contain references to other data objects, through 
  * reference-type properties.
  * <p>
  * A data object has a series of convenience accessors for its properties. 
  * These methods either use a path (String), a property index, 
  * or the property's meta object itself, to identify the property.
  * Some examples of the path-based accessors are as follows:
  * <p>
  * DataObjectPtr company = ...;<br>
  * company->getString("name");<br>                   
  * company->setString("name", "acme");<br>
  * company->getString("department.0/name")<br>       
  * company->getString("department[1]/name")<br>        
  * (Note .n indexes from 0, whilst [] indexes from 1.<br>
  * company->getDataObject("department[number=123]")  returns the department where number=123<br>
  * company->getDataObject("..")                      returns the containing data object<br>
  * company->getDataObject("/")                       returns the root containing data object<br>
  * There are specific accessors for the primitive types and commonly used 
  * data types like String.
  */

class DataObject : public RefCountingObject
{
     public:
        virtual ~DataObject();

    /**
     *
     *    Introspection - properties and types
     */

    /**  getPropertyIndex gets the unique index of a property
     *
     * A property of a data object has a unique index associated with it. 
     * This method gets a property index for this object from the property, 
     * or throw SDOPropertyNotFoundException if the property is not part 
     * of this data object.
     */

    virtual unsigned int SDO_API getPropertyIndex(const Property& p) = 0;
    
    
    /**  getInstanceProperties gets the props of the current object.
     *
     * Returns a read-only List of the Properties currently used in this DataObject.
     * This list will contain all of the properties in getType().getProperties()
     * and any properties where isSet(property) is true.
     * For example, properties resulting from the use of
     * open or mixed XML content are present if allowed by the Type.
     * The list does not contain duplicates. 
     * The order of the properties in the list begins with getType().getProperties()
     * and the order of the remaining properties is determined by the implementation.
     * The same list will be returned unless the DataObject is updated so that 
     * the contents of the list change
     * Returns the list of Properties currently used in this DataObject.
     */
   
    virtual SDO_API PropertyList getInstanceProperties() = 0;

    /**
     * See if the property currently exists
     */

    virtual SDO_API bool hasProperty(const char* name) = 0;
    virtual SDO_API bool hasProperty(const SDOString& name) = 0;
    /**
     * These are just like getType().getProperty(), but may return
     * values other than the property list for open types.
     */

     virtual const Property& getProperty(unsigned int index) = 0;
    
    virtual const Property& getProperty(const char* prop) = 0;
    virtual const Property& getProperty(const SDOString& prop) = 0;
 
 
    /**
     * return the data factory which created this object
     */

    virtual SDO_API DataFactory* getDataFactory() = 0;

    /**  getContainer get the containing object
     *
     * Returns the containing data object
     * or 0 if there is no container.
     */

    virtual SDO_API DataObjectPtr getContainer() = 0;

    /**  getContainmentProperty returns the property containing this object
     *
     *  Return the Property of the data object containing this data object
     *  or throw an SDOPropertyNotFoundException if there is no container.
     */

    virtual SDO_API const Property& getContainmentProperty() = 0;
    
    /**  getType  returns the data object's type.
     *
     * getType returns the data object's type.
     * The type defines the properties available for reflective access.
     */

    virtual SDO_API const Type& getType() = 0;

    /*  getTypeEnum returns an enumerator for the type
     *
     * Returns an enumerator for the type for easy switching on basic types.
     * The enumerator is part of the Type class
     */

    virtual SDO_API const Type::Types getTypeEnum() = 0;



 
    
    /**  getDataObject returns a data object by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API DataObjectPtr getDataObject(const SDOString& path) = 0;
    virtual SDO_API DataObjectPtr getDataObject(const char* path) = 0; 
    virtual SDO_API DataObjectPtr getDataObject(unsigned int propertyIndex) = 0; 
    virtual SDO_API DataObjectPtr getDataObject(const Property& property) = 0; 

    /**  setDataObject sets a value by path, index or property
     *
     * Sets a property of either this object or an object reachable from it,
     * as identified by the specified path,
     * to the specified value.
     */

    
    virtual SDO_API void setDataObject(const char* path, DataObjectPtr value) = 0; 
    virtual SDO_API void setDataObject(const SDOString& path, DataObjectPtr value) = 0; 
    virtual SDO_API void setDataObject(unsigned int propertyIndex, DataObjectPtr value) = 0; 
    virtual SDO_API void setDataObject(const Property& property, DataObjectPtr value) = 0; 

    /**  getBoolean returns a boolean by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API bool getBoolean(const char* path) = 0;
    virtual SDO_API bool getBoolean(const SDOString& path) = 0;
    virtual SDO_API bool getBoolean(unsigned int propindex) = 0;
    virtual SDO_API bool getBoolean(const Property& p) = 0;

    virtual SDO_API void setBoolean(const char* path, bool b) = 0;
    virtual SDO_API void setBoolean(const SDOString& path, bool b) = 0;
    virtual SDO_API void setBoolean(unsigned int propindex, bool b) = 0;
    virtual SDO_API void setBoolean(const Property& p, bool b) = 0;

    /**  getByte returns a char by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API char getByte(const char* path) = 0;
    virtual SDO_API char getByte(const SDOString& path) = 0;
    virtual SDO_API char getByte(unsigned int propindex) = 0;
    virtual SDO_API char getByte(const Property& p) = 0;
    
    virtual SDO_API void setByte(const char* path, char c) = 0;
    virtual SDO_API void setByte(const SDOString& path, char c) = 0;
    virtual SDO_API void setByte(unsigned int propindex, char c) = 0;
    virtual SDO_API void setByte(const Property& p, char c) = 0;

    /**  getCharacter returns a wchar_t by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API wchar_t getCharacter(const char* path) = 0;
    virtual SDO_API wchar_t getCharacter(const SDOString& path) = 0;
    virtual SDO_API wchar_t getCharacter(unsigned int propindex) = 0;
    virtual SDO_API wchar_t getCharacter(const Property& p) = 0;

    virtual SDO_API void setCharacter(const char* path, wchar_t c) = 0;
    virtual SDO_API void setCharacter(const SDOString& path, wchar_t c) = 0;
    virtual SDO_API void setCharacter(unsigned int propindex, wchar_t c) = 0;
    virtual SDO_API void setCharacter(const Property& p, wchar_t c) = 0;


    /**  getlength returns the length of a string
     *
     * Specific to Bytes and Characters data objects. This returns the 
     * length of the buffer required to hold the contents of the object.
     * len = do->getLength("name");
     * buf = new char[len];
     * reallen = do->getBytes("name",buf,len);
     */
    virtual SDO_API unsigned int getLength(const char* path) = 0;
    virtual SDO_API unsigned int getLength(const SDOString& path) = 0;
    virtual SDO_API unsigned int getLength(unsigned int propindex) = 0;
    virtual SDO_API unsigned int getLength(const Property& p) = 0;
    virtual SDO_API unsigned int getLength() = 0;


    /**  getBytes returns a byte buffer
     *
     * A DataObject of type Bytes holds an array of bytes as its value. These
     * methods transfer the contents of that buffer into an array of chars allocated
     * by the users program. The return value is the number of bytes actually 
     * copied.
     * The byte array is not necessarily null terminated. If a null terminated
     * C style string is required, then getCString is an alternative.
     * The third paarameter is the length of the allocated buffer, which may be more
     * than the length of the byte array. If the length specified is less than the
     * length of the byte array, then only a portion of the 
     * byte array is returned.
     */
    

    virtual SDO_API unsigned int getBytes(const char* path, char* buf, unsigned int max) = 0;
    virtual SDO_API unsigned int getBytes(const SDOString& path, char* buf, unsigned int max) = 0;
    virtual SDO_API unsigned int getBytes(unsigned int propindex, char* buf, unsigned int max) = 0;
    virtual SDO_API unsigned int getBytes(const Property& p, char* buf, unsigned int max) = 0;

    virtual SDO_API void setBytes(const char* path, const char* c, unsigned int length) = 0;
    virtual SDO_API void setBytes(const SDOString& path, const char* c, unsigned int length) = 0;
    virtual SDO_API void setBytes(unsigned int propindex, const char* c, unsigned int length) = 0;
    virtual SDO_API void setBytes(const Property& p, const char* c, unsigned int length) = 0;

    /**  getString returns a wide char buffer
     *
     * A DataObject of type String holds an array of wide characters as its value. These
     * methods transfer the contents of that buffer into an array of wchar_t allocated
     * by the users program. The return value is the number of wchar_t actually 
     * copied.
     * The array is not necessarily null terminated. 
     * The third paarameter is the length of the allocated buffer, which may be more
     * than the length of the array. If the length specified is less than the
     * length of the array, then only a portion of the array is returned.
     */

    virtual SDO_API unsigned int getString(const SDOString& path , wchar_t* c, unsigned int max) = 0;
    virtual SDO_API unsigned int getString(const char* path , wchar_t* c, unsigned int max) = 0;
    virtual SDO_API unsigned int getString(unsigned int propindex, wchar_t* c, unsigned int max) = 0;
    virtual SDO_API unsigned int getString(const Property& p, wchar_t* c, unsigned int max) = 0;
    
    virtual SDO_API void setString(const char* path, const wchar_t* c, unsigned int length) = 0;
    virtual SDO_API void setString(const SDOString& path, const wchar_t* c, unsigned int length) = 0;
    virtual SDO_API void setString(unsigned int propindex, const wchar_t* c, unsigned int length) = 0;
    virtual SDO_API void setString(const Property& p, const wchar_t* c, unsigned int length) = 0;

    /**  getDate returns an SDODate by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */
    virtual SDO_API const SDODate getDate(const char* path) = 0;
    virtual SDO_API const SDODate getDate(const SDOString& path) = 0;
    virtual SDO_API const SDODate getDate(unsigned int propindex) = 0;
    virtual SDO_API const SDODate getDate(const Property& p) = 0;
    
    virtual SDO_API void setDate(const char* path, const SDODate d) = 0;
    virtual SDO_API void setDate(const SDOString& path, const SDODate d) = 0;
    virtual SDO_API void setDate(unsigned int propindex, const SDODate d) = 0;
    virtual SDO_API void setDate(const Property& p, const SDODate d) = 0;

    /**  getDouble returns a long double by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API long double getDouble(const SDOString& path) = 0;
    virtual SDO_API long double getDouble(const char* path) = 0;
    virtual SDO_API long double getDouble(unsigned int propindex) = 0;
    virtual SDO_API long double getDouble(const Property& p) = 0;
    
    virtual SDO_API void setDouble(const SDOString& path, long double d) = 0;
    virtual SDO_API void setDouble(const char* path, long double d) = 0;
    virtual SDO_API void setDouble(unsigned int propindex, long double d) = 0;
    virtual SDO_API void setDouble(const Property& p, long double d) = 0;

    /**  getFloat returns a float by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API float getFloat(const SDOString& path) = 0;
    virtual SDO_API float getFloat(const char* path) = 0;
    virtual SDO_API float getFloat(unsigned int propindex) = 0;
    virtual SDO_API float getFloat(const Property& p) = 0;
    
    virtual SDO_API void setFloat(const SDOString& path, float f) = 0;
    virtual SDO_API void setFloat(const char* path, float f) = 0;
    virtual SDO_API void setFloat(unsigned int propindex, float f) = 0;
    virtual SDO_API void setFloat(const Property& p, float f) = 0;

    /**  getInteger returns a long by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API long getInteger(const SDOString& path) = 0;
    virtual SDO_API long getInteger(const char* path) = 0;
    virtual SDO_API long getInteger(unsigned int propindex) = 0;
    virtual SDO_API long getInteger(const Property& p) = 0;
    
    virtual SDO_API void setInteger(const SDOString& path, long i) = 0;
    virtual SDO_API void setInteger(const char* path, long i) = 0;
    virtual SDO_API void setInteger(unsigned int propindex, long i) = 0;
    virtual SDO_API void setInteger(const Property& p, long i) = 0;

    /**  getLong returns a int64_t by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API int64_t getLong(const SDOString& path) = 0;
    virtual SDO_API int64_t getLong(const char* path) = 0;
    virtual SDO_API int64_t getLong(unsigned int propindex) = 0;
    virtual SDO_API int64_t getLong(const Property& p) = 0;

    virtual SDO_API void setLong(const SDOString& path, int64_t l) = 0;
    virtual SDO_API void setLong(const char* path, int64_t l) = 0;
    virtual SDO_API void setLong(unsigned int propindex, int64_t l) = 0;
    virtual SDO_API void setLong(const Property& p, int64_t l) = 0;

    /**  getShort returns a short by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API short getShort(const SDOString& path) = 0;
    virtual SDO_API short getShort(const char* path) = 0;
    virtual SDO_API short getShort(unsigned int propindex) = 0;
    virtual SDO_API short getShort(const Property& p) = 0;
    
    virtual SDO_API void setShort(const SDOString& path, short s) = 0;
    virtual SDO_API void setShort(const char* path, short s) = 0;
    virtual SDO_API void setShort(unsigned int propindex, short s) = 0;
    virtual SDO_API void setShort(const Property& p, short s) = 0;


    /**  getCString returns a null terminated string by path, index or property
     *
     * Returns the value of a property of either this object or an object 
     * reachable from it, as identified by the specified path.
     */

    virtual SDO_API const char* getCString(const char* path) = 0;
    virtual SDO_API const char* getCString(const SDOString& path) = 0;
    virtual SDO_API const char* getCString(unsigned int propertyIndex) = 0;
    virtual SDO_API const char* getCString(const Property& prop) = 0;
    
    virtual SDO_API void setCString(const char* path, const char* value) = 0;
    virtual SDO_API void setCString(unsigned int propertyIndex, const char* value) = 0;
    virtual SDO_API void setCString (const Property& prop, const char* value) = 0;

    virtual SDO_API void setCString(const SDOString& path, const SDOString& value) = 0;
    virtual SDO_API void setCString(unsigned int propertyIndex, const SDOString& value) = 0;
    virtual SDO_API void setCString (const Property& prop, const SDOString& value) = 0;
    

    /**  setNull sets a data object value to null.
     *
     * A DataObjectType or DataType value may be set or unset. If it is set, then
     * it may have a value, or it may be set to null. A distinction is drawn between
     * being unset, having the default value, being set and being null.
     * When the value of an integer (for example) is returned as zero, it could have
     * been set to zero, or it could be null. Use isNull() to verify.
        */

    virtual SDO_API void setNull(const char* path) = 0;
    virtual SDO_API void setNull(const SDOString& path) = 0;
    virtual SDO_API void setNull(unsigned int propertyIndex) = 0;
    virtual SDO_API void setNull(const Property& prop) = 0;
    
    virtual SDO_API bool isNull(const char* path) = 0;
    virtual SDO_API bool isNull(const SDOString& path) = 0;
    virtual SDO_API bool isNull(unsigned int propertyIndex) = 0;
    virtual SDO_API bool isNull(const Property& prop) = 0;

    /**  isSet test whether the value has been set
     *
     * Returns whether a property of either this object or an object reachable 
     * from it, as identified by the specified path,
     * is considered to be set.
     */
    
    virtual SDO_API bool isSet(const char* path) = 0;
    virtual SDO_API bool isSet(const SDOString& path) = 0;
    virtual SDO_API bool isSet(unsigned int propertyIndex) = 0;
    virtual SDO_API bool isSet(const Property& property) = 0;


    /** isValid shows true if the value is set or defaulted.
     *
     * A property can be set, null, or have a default value - all
     * of these are valid. It can also be unset, and have no default
     * in which case this returns false.
     */

    virtual SDO_API bool isValid(const char* path) = 0;
    virtual SDO_API bool isValid(const SDOString& path) = 0;
    virtual SDO_API bool isValid(unsigned int propertyIndex) = 0;
    virtual SDO_API bool isValid(const Property& property ) =0;
    
    /** unset unsets a value previously set.
     *
     * unsets a property of either this object or an object reachable 
     * from it, as identified by the specified path.
     */

    virtual SDO_API void unset(const char* path) = 0;
    virtual SDO_API void unset(const SDOString& path) = 0;
    virtual SDO_API void unset(unsigned int propertyIndex) = 0;
    virtual SDO_API void unset(const Property& property) = 0;

    /** setUserData sets a reserved field in the data object.
     *
     * Each data object has precisely one 32 bit slot available to 
     * be used by applications. This is not part of the data, its
     * just a place to store anything for later retrieval. 
     */

    virtual SDO_API void setUserData(const char* path,void* value) = 0;
    virtual SDO_API void setUserData(const SDOString& path, void* value) = 0;
    virtual SDO_API void setUserData(unsigned int propertyIndex, void* value) = 0;
    virtual SDO_API void setUserData(const Property& property, void* value) = 0;
    virtual SDO_API void setUserData(void* value) = 0;
    virtual SDO_API void* getUserData(const char* path) = 0;
    virtual SDO_API void* getUserData(const SDOString& path) = 0;
    virtual SDO_API void* getUserData(unsigned int propertyIndex) = 0;
    virtual SDO_API void* getUserData(const Property& property) = 0;
    virtual SDO_API void* getUserData() = 0;


    /**  getSequence returns the sequence for a data object
     *
     * Returns the value of a Sequence property identified by 
     * the specified path. See Sequence.
     */

    virtual SDO_API SequencePtr getSequence() = 0;
    virtual SDO_API SequencePtr getSequence(const char* path) = 0;
    virtual SDO_API SequencePtr getSequence(const SDOString& path) = 0;
    virtual SDO_API SequencePtr getSequence(unsigned int propertyIndex) = 0;
    virtual SDO_API SequencePtr getSequence(const Property& property) = 0;



    /**  createDataObject creates a data object value
     *
     * Returns a new data object contained by this object using the 
     * specified property,which must be a containment property.
     * The type of the created object is the declared type
     * of the specified property.
     * If the property is many valued, this method adds an element to the
     * list, otherwise it sets the value, removing any old value.
     */


    virtual SDO_API DataObjectPtr createDataObject(const char* propertyName) = 0;
    virtual SDO_API DataObjectPtr createDataObject(const SDOString& propertyName) = 0;
    virtual SDO_API DataObjectPtr createDataObject(unsigned int propertyIndex) = 0;
    virtual SDO_API DataObjectPtr createDataObject(const Property& property) = 0;



    /**  detach detaches an object from the graph
     *
     * This method removes the current data object from the graph, but does
     * not destroy it. The DataObject can be re-attached to the graph later.
     */

    virtual SDO_API void detach() = 0;

    /**  clear unsets all the properties 
     *
     * This method unsets all the properties, and deletes all the data object 
     * propertiy values from this data object.
     */
 
    virtual SDO_API void clear() = 0;


    /**  getList gets the value of a many-valued property
     *
     * Many valued properties are returned as lists of DataObjects.
     * These lists may contain primitives or data objects, but they behave
     * like data objects.
     * Getting a many valued integer consists of getting the list, then
     * using the DataObjectList API to getInteger() for each list element.
     */

    virtual SDO_API DataObjectList& getList(const char* path) = 0;
    virtual SDO_API DataObjectList& getList(const SDOString& path) = 0;
    virtual SDO_API DataObjectList& getList(unsigned int propIndex) = 0;
    virtual SDO_API DataObjectList& getList(const Property& p) = 0;
    virtual DataObjectList& getList() = 0;


    /**  getChangeSummary get the applicable change summary
     *
     * This method gets the applicable change summary for a data object.
     * The summary is not necessarily attached to the data object, it may be
     * the summary for a parent data object. No object with a summary attached
     * may be a child of another object with a summary attached. 
     * See the ChangeSummary API for details of using the change sumamry.
     */

    virtual SDO_SPI ChangeSummaryPtr getChangeSummary() = 0;
    virtual SDO_SPI ChangeSummaryPtr getChangeSummary(const char* path) = 0;
    virtual SDO_SPI ChangeSummaryPtr getChangeSummary(const SDOString& path) = 0;
    virtual SDO_SPI ChangeSummaryPtr getChangeSummary(unsigned int propIndex) = 0;
    virtual SDO_SPI ChangeSummaryPtr getChangeSummary(const Property& prop) = 0;

     /**  objectToXPath - utility to find the xpath from the root.
     *
     * objectToXPath returns a string which could be used to locate this data 
     * object from the root data object of the graph.
     */

    virtual SDO_SPI const char* objectToXPath() = 0;


};
};
};
 
#endif //_DATAOBJECT_H_
