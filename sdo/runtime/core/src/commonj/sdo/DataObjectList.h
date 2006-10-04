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

#ifndef _DATAOBJECTLIST_H_
#define _DATAOBJECTLIST_H_


#include "commonj/sdo/export.h"
#include "commonj/sdo/RefCountingPointer.h"
#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/SDODate.h"
#include <wchar.h>


namespace commonj{
namespace sdo{

/** 
 * 
 * List for retrieving many valued property values, DataType or DataObjectType.
 *
 * DataObjectList provides an API for getting and setting values in
 * many valued properties. The list deals with both DataType and 
 * DataObjectType properties.
 */

class DataObjectList
{
public:
    virtual  ~DataObjectList();

    /**  [] is a index operator, and returns a dataobject.
     *
     * The index operator returns a data object, even if the 
     * list is of a DataType. The data object can yield its true value using
     * getInteger() etc. Alternatively, list elements can be accessed
     * with their correct type using the list getInteger(index) api.
     */

    virtual SDO_API DataObjectPtr operator[] (int pos) = 0;
    virtual SDO_API const DataObjectPtr operator[] (int pos) const = 0;

    /**  size returns the number of elements.
     *
     * The size method returns the number of elements in the list
     */

    virtual SDO_API int size () const = 0;

    /**  getBoolean returns a boolean at an index
     *
     * Each of the DataTypes has a corresponding getter and setter
     * overload. getBoolean returns a bool, getByte returns a char
     * etc
     */

    virtual SDO_API bool getBoolean(unsigned int index) const = 0;
    virtual SDO_API char getByte(unsigned int index) const = 0;
    virtual SDO_API wchar_t getCharacter(unsigned int index) const = 0;
    virtual SDO_API unsigned int getString(unsigned int index, wchar_t* value,
        unsigned int max) const = 0;
    virtual SDO_API unsigned int getBytes(unsigned int index, char* value,
        unsigned int max) const = 0;
    virtual SDO_API short getShort(unsigned int index) const = 0;
    virtual SDO_API long getInteger(unsigned int index) const = 0;
    virtual SDO_API int64_t getLong(unsigned int index) const = 0;
    virtual SDO_API float getFloat(unsigned int index) const = 0;
    virtual SDO_API long double getDouble(unsigned int index) const = 0;
    virtual SDO_API const SDODate  getDate(unsigned int index) const = 0;
    virtual SDO_API const char*  getCString(unsigned int index) const = 0;
    virtual SDO_API DataObjectPtr  getDataObject(unsigned int index) const = 0;

    virtual SDO_API void setBoolean(unsigned int index, bool d)  = 0;
    virtual SDO_API void setByte(unsigned int index, char d)  = 0;
    virtual SDO_API void setCharacter(unsigned int index, wchar_t d)  = 0;
    virtual SDO_API void setString(unsigned int index, const wchar_t* d, unsigned int len)  = 0;
    virtual SDO_API void setBytes(unsigned int index, const char* d, unsigned int len)  = 0;
    virtual SDO_API void setShort(unsigned int index, short d)  = 0;
    virtual SDO_API void setInteger(unsigned int index, long d)  = 0;
    virtual SDO_API void setLong(unsigned int index, int64_t d)  = 0;
    virtual SDO_API void setFloat(unsigned int index, float d)  = 0;
    virtual SDO_API void setDouble(unsigned int index, long double d)  = 0;
    virtual SDO_API void setDate(unsigned int index, const SDODate d)  = 0;
    virtual SDO_API void setCString(unsigned int index, char* d)  = 0;
    virtual SDO_API void setCString(unsigned int index, const SDOString& d)  = 0;
    virtual SDO_API void setDataObject(unsigned int index, DataObjectPtr dob)  = 0;

    /**  getLength returns the length of a string element
     *
     * getLength behaves like DataObject::getlength. It returns the length
     * of a bytes or string buffer such that space can be allocated for the
     * values from getBytes() and getString()
     */

    virtual SDO_API unsigned int getLength(unsigned int index) const = 0;

    /**  insert and append put items into the list
     *
     * insert and append have overrides for each of the DataTypes, and 
     * for DataObject. Insert inserts before the element number given. If the insertion 
     * point is off the end of the list, then an append is performed.
     * Append puts the element on the end of the list.
     */


    virtual SDO_API void insert (unsigned int index, DataObjectPtr d) = 0;
    virtual SDO_API void append (DataObjectPtr d) = 0;
    
    virtual SDO_API void insert (unsigned int index, bool d) = 0;
    virtual SDO_API void append (bool d) = 0;

    virtual SDO_API void insert (unsigned int index, char d) = 0;
    virtual SDO_API void append (char d) = 0;

    virtual SDO_API void insert (unsigned int index, wchar_t d) = 0;
    virtual SDO_API void append (wchar_t d) = 0;

    virtual SDO_API void insert (unsigned int index, const wchar_t* d, unsigned int len) = 0;
    virtual SDO_API void append (const wchar_t* d, unsigned int len) = 0;

    virtual SDO_API void insert (unsigned int index, const char* d, unsigned int len) = 0;
    virtual SDO_API void append (const char* d, unsigned int len) = 0;

    virtual SDO_API void insert (unsigned int index, const char* d) = 0;
    virtual SDO_API void insert (unsigned int index, const SDOString& d) = 0;
    virtual SDO_API void append (const char* d) = 0;
    virtual SDO_API void append (const SDOString& d) = 0;

    virtual SDO_API void insert (unsigned int index, short d) = 0;
    virtual SDO_API void append (short d) = 0;

    virtual SDO_API void insert (unsigned int index, const SDODate d) = 0;
    virtual SDO_API void append (const SDODate d) = 0;

#if __WORDSIZE !=64
    virtual SDO_API void insert (unsigned int index, long d) = 0;
    virtual SDO_API void append (long d) = 0;
#endif

    virtual SDO_API void insert (unsigned int index, int64_t d) = 0;
    virtual SDO_API void append (int64_t d) = 0;
    
    virtual SDO_API void insert (unsigned int index, float d) = 0;
    virtual SDO_API void append (float d) = 0;


    virtual SDO_API void insert (unsigned int index, long double d) = 0;
    virtual SDO_API void append (long double d) = 0;

    /**  remove removes an element from the list.
     * 
     * Remove removes the element from the list, and passes out a 
     * DataObjectPtr to the old value. This can be attached to the list
     * somewhere else, or discarded.
     */

    virtual SDO_API DataObjectPtr  remove (unsigned int index) = 0;

    /*  getType returns type
     *
     * Returns the type.
     * May throw SDOTYpeNotFoundException for unset open types
     */


   virtual SDO_API const Type& getType() = 0;

    /*  getTypeEnum returns an enumerator for the type
     *
     * Returns an enumerator for the type for easy switching on basic types.
     * The enumerator is part of the Type class
     * May throw SDOTypeNotFoundException for open types
     */

    virtual SDO_API const Type::Types getTypeEnum() = 0;

};
};
};

#endif
