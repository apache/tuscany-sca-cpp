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

#ifndef _SEQUENCEIMPL_H_
#define _SEQUENCEIMPL_H_

#include "commonj/sdo/disable_warn.h"

#include <vector>
using namespace std;

#include "commonj/sdo/Sequence.h"
#include "commonj/sdo/SDODate.h"

#define SequenceImplPtr RefCountingPointer<SequenceImpl> 

namespace commonj{
namespace sdo{

class Property; /* forward declaration */
class DataObjectImpl;

/**  SequenceImpl implements the abstract class Sequence.
 *
 * A sequence is a heterogeneous list of properties and corresponding values.
 * It represents an ordered arbitrary mixture of data values from more 
 * than one property of a {@link DataObject data object}.
 */

class SequenceImpl : public Sequence 
{
    public:
    ///////////////////////////////////////////////////////////////////////////
    // Returns the number of entries in the sequence.
    // @return the number of entries.
    ///////////////////////////////////////////////////////////////////////////
    virtual unsigned int size();

    ///////////////////////////////////////////////////////////////////////////
    // Returns the property for the given entry index.
    ///////////////////////////////////////////////////////////////////////////
    virtual const Property& getProperty(unsigned int index);

    /**
     * Return the data object associated with this sequence
     */

    SDO_API virtual const DataObjectPtr getDataObject();

    ///////////////////////////////////////////////////////////////////////////
    // Returns the property for the given entry index.
    ///////////////////////////////////////////////////////////////////////////
    virtual unsigned int getIndex(const Property& p, unsigned int pindex=0);
    virtual unsigned int getIndex(const char* propName, unsigned int pindex=0);

    virtual unsigned int getListIndex(unsigned int index);
    
    Type::Types getTypeEnum(unsigned int index);

    
    ///////////////////////////////////////////////////////////////////////////
    // Returns the property value for the given entry index.
    // @param index the index of the entry.
    // @return the value for the given entry index..
    ///////////////////////////////////////////////////////////////////////////
    virtual const char* getCStringValue(unsigned int index);
    virtual bool        getBooleanValue(unsigned int index);
    virtual char        getByteValue(unsigned int index);
    virtual wchar_t     getCharacterValue(unsigned int index);
    virtual unsigned int getStringValue(unsigned int index, wchar_t* val, unsigned int max);
    virtual unsigned int getBytesValue(unsigned int index, char* val, unsigned int max);
    virtual short       getShortValue(unsigned int index);
    virtual long         getIntegerValue(unsigned int index);    
    virtual int64_t     getLongValue(unsigned int index);
    virtual float       getFloatValue(unsigned int index);
    virtual long double getDoubleValue(unsigned int index);
    virtual const SDODate   getDateValue(unsigned int index);
    virtual DataObjectPtr getDataObjectValue(unsigned int index);

    virtual unsigned int getLength(unsigned int index);
 
    ///////////////////////////////////////////////////////////////////////////
    // sets the entry at a specified index to the new value.
    // @param index the index of the entry.
    // @param value the new value for the entry.
    ///////////////////////////////////////////////////////////////////////////

    virtual void setCStringValue(   unsigned int index, const char* s );
    virtual void setBooleanValue(   unsigned int index, bool        b );
    virtual void setByteValue(      unsigned int index, char        c );
    virtual void setCharacterValue( unsigned int index, wchar_t     c );
    virtual void setBytesValue(     unsigned int index, const char* s , unsigned int len);
    virtual void setStringValue(    unsigned int index, const wchar_t* s , unsigned int len);
    virtual void setShortValue(     unsigned int index, short       s );
    virtual void setIntegerValue(       unsigned int index, long         i );    
    virtual void setLongValue(  unsigned int index, int64_t     l );
    virtual void setFloatValue(     unsigned int index, float       f );
    virtual void setDoubleValue(    unsigned int index, long double d );
    virtual void setDateValue(    unsigned int index, const SDODate t );
    virtual void setDataObjectValue(unsigned int index, DataObjectPtr d );
    

        ///////////////////////////////////////////////////////////////////////////
    // adds a new entry with the specified property name and value
    // to the end of the entries.
    // @param propertyName the name of the entry's property.
    // @param value the value for the entry.
    ///////////////////////////////////////////////////////////////////////////
    virtual bool addCString(   const char* propertyName,const char* s );
    virtual bool addBoolean(   const char* propertyName,bool        b );
    virtual bool addByte(      const char* propertyName,char        c );
    virtual bool addCharacter( const char* propertyName,wchar_t     c );
    virtual bool addString(    const char* propertyName,const wchar_t* s , unsigned int len);
    virtual bool addBytes(     const char* propertyName,const char* s , unsigned int len);
    virtual bool addShort(     const char* propertyName,short       s );
    virtual bool addInteger(   const char* propertyName,long         i );    
    virtual bool addLong(      const char* propertyName,int64_t     l );
    virtual bool addFloat(     const char* propertyName,float       f );
    virtual bool addDouble(    const char* propertyName,long double d );
    virtual bool addDate(      const char* propertyName,const SDODate t );
    virtual bool addDataObject(const char* propertyName,DataObjectPtr d );

    
    ///////////////////////////////////////////////////////////////////////////
    // adds a new entry with the specified property index and value
    // to the end of the entries.
    // @param propertyIndex the index of the entry's property.
    // @param value the value for the entry.
    ///////////////////////////////////////////////////////////////////////////
    virtual bool addCString(   unsigned int propertyIndex,const char* s );
    virtual bool addBoolean(   unsigned int propertyIndex,bool        b );
    virtual bool addByte(      unsigned int propertyIndex,char        c );
    virtual bool addCharacter( unsigned int propertyIndex,wchar_t     c );
    virtual bool addString(    unsigned int propertyIndex,const wchar_t* s , unsigned int len);
    virtual bool addBytes(     unsigned int propertyIndex,const char* s , unsigned int len);
    virtual bool addShort(     unsigned int propertyIndex,short       s );
    virtual bool addInteger(   unsigned int propertyIndex,long         i );    
    virtual bool addLong(      unsigned int propertyIndex,int64_t     l );
    virtual bool addFloat(     unsigned int propertyIndex,float       f );
    virtual bool addDouble(    unsigned int propertyIndex,long double d );
    virtual bool addDate(      unsigned int propertyIndex,const SDODate t );
    virtual bool addDataObject(unsigned int propertyIndex,DataObjectPtr d );


    ///////////////////////////////////////////////////////////////////////////
    // adds a new entry with the specified property and value
    // to the end of the entries.
    // @param property the property of the entry.
    // @param value the value for the entry.
    ///////////////////////////////////////////////////////////////////////////

    virtual bool addCString(   const Property& property,const char* s );
    virtual bool addBoolean(   const Property& property,bool        b );
    virtual bool addByte(      const Property& property,char        c );
    virtual bool addCharacter( const Property& property,wchar_t     c );
    virtual bool addString(    const Property& property,const wchar_t* s , unsigned int len);
    virtual bool addBytes(     const Property& property,const char* s , unsigned int len);
    virtual bool addShort(     const Property& property,short       s );
    virtual bool addInteger(   const Property& property,long         i );    
    virtual bool addLong(      const Property& property,int64_t     l );
    virtual bool addFloat(     const Property& property,float       f );
    virtual bool addDouble(    const Property& property,long double d );
    virtual bool addDate(      const Property& property,const SDODate t );
    virtual bool addDataObject(const Property& property,DataObjectPtr d );


    ///////////////////////////////////////////////////////////////////////////
    // adds a new entry with the specified property name and value
    // at the specified entry index.
    // @param index the index at which to add the entry.
    // @param propertyName the name of the entry's property.
    // @param value the value for the entry.
    ///////////////////////////////////////////////////////////////////////////
    virtual bool addCString(   unsigned int index,const char* propertyName,const char* s );
    virtual bool addBoolean(   unsigned int index,const char* propertyName,bool        b );
    virtual bool addByte(      unsigned int index,const char* propertyName,char        c );
    virtual bool addCharacter( unsigned int index,const char* propertyName,wchar_t     c );
    virtual bool addString(    unsigned int index,const char* propertyName,const wchar_t* s , unsigned int len);
    virtual bool addBytes(     unsigned int index,const char* propertyName,const char* s , unsigned int len);
    virtual bool addShort(     unsigned int index,const char* propertyName,short       s );
    virtual bool addInteger(   unsigned int index,const char* propertyName,long         i );    
    virtual bool addLong(      unsigned int index,const char* propertyName,int64_t     l );
    virtual bool addFloat(     unsigned int index,const char* propertyName,float       f );
    virtual bool addDouble(    unsigned int index,const char* propertyName,long double d );
    virtual bool addDate(      unsigned int index,const char* propertyName,const SDODate t );
    virtual bool addDataObject(unsigned int index,const char* propertyName,DataObjectPtr d );


    ///////////////////////////////////////////////////////////////////////////
    // adds a new entry with the specified property index and value
    // at the specified entry index.
    // @param index the index at which to add the entry.
    // @param propertyIndex the index of the entry's property.
    // @param value the value for the entry.
    ///////////////////////////////////////////////////////////////////////////
    virtual bool addCString(   unsigned int index,unsigned int propertyIndex,const char* s );
    virtual bool addBoolean(   unsigned int index,unsigned int propertyIndex,bool        b );
    virtual bool addByte(      unsigned int index,unsigned int propertyIndex,char        c );
    virtual bool addCharacter( unsigned int index,unsigned int propertyIndex,wchar_t     c );
    virtual bool addString(    unsigned int index,unsigned int propertyIndex,const wchar_t* s , unsigned int len);
    virtual bool addBytes(     unsigned int index,unsigned int propertyIndex,const char* s , unsigned int len);
    virtual bool addShort(     unsigned int index,unsigned int propertyIndex,short       s );
    virtual bool addInteger(   unsigned int index,unsigned int propertyIndex,long         i );    
    virtual bool addLong(      unsigned int index,unsigned int propertyIndex,int64_t     l );
    virtual bool addFloat(     unsigned int index,unsigned int propertyIndex,float       f );
    virtual bool addDouble(    unsigned int index,unsigned int propertyIndex,long double d );
    virtual bool addDate(      unsigned int index,unsigned int propertyIndex,const SDODate t );
    virtual bool addDataObject(unsigned int index,unsigned int propertyIndex,DataObjectPtr d );


    ///////////////////////////////////////////////////////////////////////////
    // adds a new entry with the specified property and value
    // at the specified entry index.
    // @param index the index at which to add the entry.
    // @param property the property of the entry.
    // @param value the value for the entry.
    ///////////////////////////////////////////////////////////////////////////
    virtual bool addCString(   unsigned int index,const Property& property,const char* s );
    virtual bool addBoolean(   unsigned int index,const Property& property,bool        b );
    virtual bool addByte(      unsigned int index,const Property& property,char        c );
    virtual bool addCharacter( unsigned int index,const Property& property,wchar_t     c );
    virtual bool addString(    unsigned int index,const Property& property,const wchar_t* s , unsigned int len);
    virtual bool addBytes(     unsigned int index,const Property& property,const char* s , unsigned int len);
    virtual bool addShort(     unsigned int index,const Property& property,short       s );
    virtual bool addInteger(   unsigned int index,const Property& property,long         i );    
    virtual bool addLong(      unsigned int index,const Property& property,int64_t     l );
    virtual bool addFloat(     unsigned int index,const Property& property,float       f );
    virtual bool addDouble(    unsigned int index,const Property& property,long double d );
    virtual bool addDate(      unsigned int index,const Property& property,const SDODate t );
    virtual bool addDataObject(unsigned int index,const Property& property,DataObjectPtr d );

 
    ///////////////////////////////////////////////////////////////////////////
    // removes the entry at the given entry index.
    // @param index the index of the entry
    ///////////////////////////////////////////////////////////////////////////
    virtual void remove(unsigned int index);
    virtual void removeAll(const Property& p);

    ///////////////////////////////////////////////////////////////////////////
    // pushes a new entry without setting the property.
    // @param index the index of the entry
    ///////////////////////////////////////////////////////////////////////////
    virtual void push(const Property& p, unsigned int index);

    ///////////////////////////////////////////////////////////////////////////
    // Moves the entry at <code>fromIndex</code> to <code>toIndex</code>.
    // @param toIndex the index of the entry destination.
    // @param fromIndex the index of the entry to move.
    ///////////////////////////////////////////////////////////////////////////
    virtual void move(unsigned int toIndex, unsigned int fromIndex);

    ///////////////////////////////////////////////////////////////////////////
    // adds a new Setting with the SDO text Property
    // to the end of the Settings.
    // @param text value of the Setting.
    ///////////////////////////////////////////////////////////////////////////
    virtual bool addText(const char* text);

    ///////////////////////////////////////////////////////////////////////////
    // adds a new Setting with the SDO text Property
    // to the Settings.
    // @param index the index at which to add the entry.
    // @param text value of the Setting.
    ///////////////////////////////////////////////////////////////////////////
    virtual bool addText(unsigned int index, const char* text);

    ///////////////////////////////////////////////////////////////////////////
    // sets a text Property
    // @param index the index at which to set the entry.
    // @param text value of the Setting.
    ///////////////////////////////////////////////////////////////////////////
    bool setText(unsigned int index, const char* text);

    ///////////////////////////////////////////////////////////////////////////
    // Checks if an entry is text or a setting.
    ///////////////////////////////////////////////////////////////////////////
    virtual bool isText(unsigned int index);

    SequenceImpl(DataObject* the_do);
    SequenceImpl(SequenceImpl* s);
  
    private:

    SequenceImpl();

    DataObjectImpl* the_do;

    class seq_item {
       public:
           seq_item(const Property* p, unsigned int i):
             the_prop(p)
            {
                 index = i;
                 text = 0;
            }
           seq_item(const char* t)
           {
               text = new char[strlen(t) + 1];
               strcpy(text,t);
               the_prop = 0;
           }

           ~seq_item()
           {
                if (text) {
                    delete text;
                }
           }

           seq_item(const seq_item& sin)
           {
               the_prop = sin.the_prop;
               index = sin.index;
               if (sin.text) {
                   text = new char[strlen(sin.text) +1];
                   strcpy(text, sin.text);
               }
               else
               {
                  text =0;
               }
           }

           const Property* getProp() {return the_prop;}
           unsigned int getIndex() { return index;}
           char* getText() { return text;}
           void setProp(Property* p) { the_prop = p;}
           void setText(const char* intext)
           {
               if (intext != 0)
               {
                   if (text != 0)
                   {
                       delete text;
                   }
                   text = new char[strlen(intext) +1];
                   strcpy(text,intext);
               }
           }
           void setIndex(unsigned int i)
           {
               index = i;
           }
       private:
       const Property* the_prop;
       unsigned int index;
       char* text;
    };

    typedef std::list<seq_item> SEQUENCE_ITEM_LIST;

    SEQUENCE_ITEM_LIST the_list;

};
};
};

#endif //_SEQUENCE_H_
