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

#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_


#include "commonj/sdo/export.h"
#include "commonj/sdo/RefCountingObject.h"
#include "commonj/sdo/RefCountingPointer.h"
#include "commonj/sdo/SDODate.h"
#include "commonj/sdo/Type.h"


namespace commonj{
namespace sdo{

class Property; /* forward declaration */
class DataObject;

/**  
 * A sequence is a heterogeneous list of properties and corresponding values.
 * It represents an ordered arbitrary mixture of data values from more 
 * than one property of a {@link DataObject data object}.
 */

class Sequence : public RefCountingObject
{
    public:
    /**  size() returns the number of elements 
     *
     * Returns the number of entries in the sequence.
     */

    SDO_API virtual unsigned int size() = 0;

    /**
     *
     * A Sequence element is either a free text item, or represents a property
     * setting. If the element is a property setting, then the information 
     * held in the element consists of:
     * -# The property which was set.
     * -# The element of that property if it was many valued.
     * -# The value which was set.
     *
     * If the sequence element is text, then there is no property, and only the
     * methods for text retrieval will work. Use the isText() method to check
     * before calling getProperty().
     */

    SDO_API virtual const Property& getProperty(unsigned int index) = 0;

    /**
     *
     * The element of that property if it was many valued. Zero if the 
     * property was single valued.
     */

    SDO_API virtual unsigned int getListIndex(unsigned int index) = 0;

    /**
     *
     * The element contains the actual value set within the property. The
     * method required to get that value will vary depending on the type
     * of the property. This is shorthand for getProperty.getTypeEnum().
     */

    SDO_API virtual Type::Types getTypeEnum(unsigned int index) = 0;

    /**
     *
     * This method searched the sequence for a setting which corresponds
     * to the specified index of the specified property. Searches for single
     * valued property settings may omit the pindex parameter.
     */


    SDO_API virtual unsigned int getIndex(const Property& p, unsigned int pindex=0) = 0;
    SDO_API virtual unsigned int getIndex(const char* propName, unsigned int pindex=0) = 0;

    /**
     *
     * These methods retrieve the setting according to the type of the
     * property concerned. If a retrieval method is used which does not correspond to
     * the correct type, a conversion will be performed, or an illegal conversion
     * exception will be thrown.
     */

    SDO_API virtual const char* getCStringValue(unsigned int index) = 0;
    SDO_API virtual bool        getBooleanValue(unsigned int index) = 0;
    SDO_API virtual char        getByteValue(unsigned int index) = 0;
    SDO_API virtual wchar_t     getCharacterValue(unsigned int index) = 0;
    SDO_API virtual unsigned int getStringValue(unsigned int index, wchar_t* val, unsigned int max) = 0;
    SDO_API virtual unsigned int getBytesValue(unsigned int index, char* val, unsigned int max) = 0;
    SDO_API virtual short       getShortValue(unsigned int index) = 0;
    SDO_API virtual long         getIntegerValue(unsigned int index) = 0;    
    SDO_API virtual int64_t     getLongValue(unsigned int index) = 0;
    SDO_API virtual float       getFloatValue(unsigned int index) = 0;
    SDO_API virtual long double getDoubleValue(unsigned int index) = 0;
    SDO_API virtual const SDODate      getDateValue(unsigned int index) = 0;
    SDO_API virtual DataObjectPtr getDataObjectValue(unsigned int index) = 0;

    /**
     *
     * getlength applies to string and byte buffer types, and allows the
     * client to allocate the correct size of buffer before retrieving the 
     * value.
     */

    SDO_API virtual unsigned int getLength(unsigned int index) = 0;


    /**
     *
     * The setter methods use an element of the sequence to perform a 
     * property setting. The value in the sequence is updated, and so is the
     * rel value of the property in the data object.
     */

    SDO_API virtual void setCStringValue(   unsigned int index, const char* s ) = 0;
    SDO_API virtual void setBooleanValue(   unsigned int index, bool        b ) = 0;
    SDO_API virtual void setByteValue(      unsigned int index, char        c ) = 0;
    SDO_API virtual void setCharacterValue( unsigned int index, wchar_t     c ) = 0;
    SDO_API virtual void setStringValue(    unsigned int index, const wchar_t* s , unsigned int len) = 0;
    SDO_API virtual void setBytesValue(     unsigned int index, const char* s , unsigned int len) = 0;
    SDO_API virtual void setShortValue(     unsigned int index, short       s ) = 0;
    SDO_API virtual void setIntegerValue(       unsigned int index, long         i ) = 0;    
    SDO_API virtual void setLongValue(  unsigned int index, int64_t     l ) = 0;
    SDO_API virtual void setFloatValue(     unsigned int index, float       f ) = 0;
    SDO_API virtual void setDoubleValue(unsigned int index, long double d ) = 0;
    SDO_API virtual void setDateValue(unsigned int index, const SDODate t ) = 0;
    SDO_API virtual void setDataObjectValue(unsigned int index, DataObjectPtr d ) = 0;


    /**
     *
     * Sequences may be added to by usign the addXXX methods. These provoke
     * a setting of the real value of the property concerned, and also append
     * an element to the sequence to reflect that setting.
     * Additions may be performed based on property name, property index or
     * the Property itself.
     */
    
    SDO_API virtual bool addCString(   const char* propertyName,const char* s ) = 0;
    SDO_API virtual bool addBoolean(   const char* propertyName,bool        b ) = 0;
    SDO_API virtual bool addByte(      const char* propertyName,char        c ) = 0;
    SDO_API virtual bool addCharacter( const char* propertyName,wchar_t     c ) = 0;
    SDO_API virtual bool addString(    const char* propertyName,const wchar_t* s , unsigned int len) = 0;
    SDO_API virtual bool addBytes (    const char* propertyName,const char* s , unsigned int len) = 0;
    SDO_API virtual bool addShort(     const char* propertyName,short       s ) = 0;
    SDO_API virtual bool addInteger(       const char* propertyName,long         i ) = 0;    
    SDO_API virtual bool addLong(  const char* propertyName,int64_t     l ) = 0;
    SDO_API virtual bool addFloat(     const char* propertyName,float       f ) = 0;
    SDO_API virtual bool addDouble(const char* propertyName,long double d ) = 0;
    SDO_API virtual bool addDate(const char* propertyName,const SDODate t ) = 0;
    SDO_API virtual bool addDataObject(const char* propertyName,DataObjectPtr d ) = 0;

 
    SDO_API virtual bool addCString(   unsigned int propertyIndex,const char* s ) = 0;
    SDO_API virtual bool addBoolean(   unsigned int propertyIndex,bool        b ) = 0;
    SDO_API virtual bool addByte(      unsigned int propertyIndex,char        c ) = 0;
    SDO_API virtual bool addCharacter( unsigned int propertyIndex,wchar_t     c ) = 0;
    SDO_API virtual bool addString(    unsigned int propertyIndex,const wchar_t* s , unsigned int len) = 0;
    SDO_API virtual bool addBytes(     unsigned int propertyIndex,const char* s , unsigned int len) = 0;
    SDO_API virtual bool addShort(     unsigned int propertyIndex,short       s ) = 0;
    SDO_API virtual bool addInteger(       unsigned int propertyIndex,long         i ) = 0;    
    SDO_API virtual bool addLong(  unsigned int propertyIndex,int64_t     l ) = 0;
    SDO_API virtual bool addFloat(     unsigned int propertyIndex,float       f ) = 0;
    SDO_API virtual bool addDouble(unsigned int propertyIndex,long double d ) = 0;
    SDO_API virtual bool addDate(unsigned int propertyIndex,const SDODate t ) = 0;
    SDO_API virtual bool addDataObject(unsigned int propertyIndex,DataObjectPtr d ) = 0;



    SDO_API virtual bool addCString(   const Property& property,const char* s ) = 0;
    SDO_API virtual bool addBoolean(   const Property& property,bool        b ) = 0;
    SDO_API virtual bool addByte(      const Property& property,char        c ) = 0;
    SDO_API virtual bool addCharacter( const Property& property,wchar_t     c ) = 0;
    SDO_API virtual bool addString   ( const Property& property,const wchar_t* s , unsigned int len) = 0;
    SDO_API virtual bool addBytes(     const Property& property,const char* s , unsigned int len) = 0;
    SDO_API virtual bool addShort(     const Property& property,short       s ) = 0;
    SDO_API virtual bool addInteger(       const Property& property,long         i ) = 0;    
    SDO_API virtual bool addLong(  const Property& property,int64_t     l ) = 0;
    SDO_API virtual bool addFloat(     const Property& property,float       f ) = 0;
    SDO_API virtual bool addDouble(const Property& property,long double d ) = 0;
    SDO_API virtual bool addDate(const Property& property,const SDODate t ) = 0;
    SDO_API virtual bool addDataObject(const Property& property,DataObjectPtr d ) = 0;


    /**
     *
     * Sequences may be added to by usign the addXXX methods. These provoke
     * a setting of the real value of the property concerned, and also append
     * an element to the sequence to reflect that setting.
     * Additions may be performed based on property name, property index or
     * the Property itself. 
     * The add method also has an override to allow insertion of a sequence element
     * before any index.
     */

    SDO_API virtual bool addCString(   unsigned int index,const char* propertyName,const char* s ) = 0;
    SDO_API virtual bool addBoolean(   unsigned int index,const char* propertyName,bool        b ) = 0;
    SDO_API virtual bool addByte(      unsigned int index,const char* propertyName,char        c ) = 0;
    SDO_API virtual bool addCharacter( unsigned int index,const char* propertyName,wchar_t     c ) = 0;
    SDO_API virtual bool addString(    unsigned int index,const char* propertyName,const wchar_t* s, unsigned int len ) = 0;
    SDO_API virtual bool addBytes(     unsigned int index,const char* propertyName,const char* s , unsigned int len) = 0;
    SDO_API virtual bool addShort(     unsigned int index,const char* propertyName,short       s ) = 0;
    SDO_API virtual bool addInteger(       unsigned int index,const char* propertyName,long         i ) = 0;    
    SDO_API virtual bool addLong(  unsigned int index,const char* propertyName,int64_t     l ) = 0;
    SDO_API virtual bool addFloat(     unsigned int index,const char* propertyName,float       f ) = 0;
    SDO_API virtual bool addDouble(unsigned int index,const char* propertyName,long double d ) = 0;
    SDO_API virtual bool addDate(unsigned int index,const char* propertyName,const SDODate t ) = 0;
    SDO_API virtual bool addDataObject(unsigned int index,const char* propertyName,DataObjectPtr d ) = 0;


    SDO_API virtual bool addCString(   unsigned int index,unsigned int propertyIndex,const char* s ) = 0;
    SDO_API virtual bool addBoolean(   unsigned int index,unsigned int propertyIndex,bool        b ) = 0;
    SDO_API virtual bool addByte(      unsigned int index,unsigned int propertyIndex,char        c ) = 0;
    SDO_API virtual bool addCharacter( unsigned int index,unsigned int propertyIndex,wchar_t     c ) = 0;
    SDO_API virtual bool addString(    unsigned int index,unsigned int propertyIndex,const wchar_t* s , unsigned int len) = 0;
    SDO_API virtual bool addBytes(     unsigned int index,unsigned int propertyIndex,const char* s , unsigned int len) = 0;
    SDO_API virtual bool addShort(     unsigned int index,unsigned int propertyIndex,short       s ) = 0;
    SDO_API virtual bool addInteger(       unsigned int index,unsigned int propertyIndex,long         i ) = 0;    
    SDO_API virtual bool addLong(  unsigned int index,unsigned int propertyIndex,int64_t     l ) = 0;
    SDO_API virtual bool addFloat(     unsigned int index,unsigned int propertyIndex,float       f ) = 0;
    SDO_API virtual bool addDouble(unsigned int index,unsigned int propertyIndex,long double d ) = 0;
    SDO_API virtual bool addDate(unsigned int index,unsigned int propertyIndex,const SDODate t ) = 0;
    SDO_API virtual bool addDataObject(unsigned int index,unsigned int propertyIndex,DataObjectPtr d ) = 0;


    SDO_API virtual bool addCString(   unsigned int index,const Property& property,const char* s ) = 0;
    SDO_API virtual bool addBoolean(   unsigned int index,const Property& property,bool        b ) = 0;
    SDO_API virtual bool addByte(      unsigned int index,const Property& property,char        c ) = 0;
    SDO_API virtual bool addCharacter( unsigned int index,const Property& property,wchar_t     c ) = 0;
    SDO_API virtual bool addString(    unsigned int index,const Property& property,const wchar_t* s , unsigned int len) = 0;
    SDO_API virtual bool addBytes(     unsigned int index,const Property& property,const char* s , unsigned int len) = 0;
    SDO_API virtual bool addShort(     unsigned int index,const Property& property,short       s ) = 0;
    SDO_API virtual bool addInteger(       unsigned int index,const Property& property,long         i ) = 0;    
    SDO_API virtual bool addLong(  unsigned int index,const Property& property,int64_t     l ) = 0;
    SDO_API virtual bool addFloat(     unsigned int index,const Property& property,float       f ) = 0;
    SDO_API virtual bool addDouble(unsigned int index,const Property& property,long double d ) = 0;
    SDO_API virtual bool addDate(unsigned int index,const Property& property,const SDODate t ) = 0;
    SDO_API virtual bool addDataObject(unsigned int index,const Property& property,DataObjectPtr d ) = 0;

 
    /**  remove() removes an item.
     *
     * Remove the item at the specified index in the sequence. 
     * This has no effect on the value of the property in the data object,
     * but the setting is no longer recorded.
     */
    SDO_API virtual void remove(unsigned int index) = 0;
    SDO_API virtual void removeAll(const Property& p) = 0;

    /**
     * Moves an entry in the sequence from one index to another. This 
     * makes the entry appear to have been set at a different point in
     * the sequence, but does not alter the actual value set.
     */

    SDO_API virtual void move(unsigned int toIndex, unsigned int fromIndex) = 0;

    /**
     * The text setting APIs add an element to the sequence which 
     * is not a property setting. 
     * This is text which may be retieved in sequence with the settings.
     */

    SDO_API virtual bool addText(const char* text) = 0;
    SDO_API virtual bool addText(unsigned int index, const char* text) = 0;
    SDO_API virtual bool setText(unsigned int index, const char* text) = 0;

    /**
     * isText returns true if the element is not a property setting.
     * This method should be used before getting the Property, to avoid an
     * exception
     */
    
    SDO_API virtual bool isText(unsigned int index) = 0;

};


};
};

#endif //_SEQUENCE_H_
