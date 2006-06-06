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

/* $Rev$ $Date: 2006/04/06 12:58:18 $ */

#ifndef _DATAOBJECTLISTIMPL_H_
#define _DATAOBJECTLISTIMPL_H_

#include "commonj/sdo/DataObjectList.h"
#include "commonj/sdo/SDODate.h"

#include "commonj/sdo/disable_warn.h"

#include <vector>


namespace commonj{
namespace sdo{

class DataObjectImpl;
class DataFactory;

typedef std::vector< RefCountingPointer<DataObjectImpl> > DATAOBJECT_VECTOR;

/** 
 * DataObjectListImpl implements the abstract class DataObjectList.
 * List for retrieving many valued property values.
 *
 * DataObjectList provides an API for getting and setting values in
 * many valued properties. The list deals with both DataType and 
 * DataObjectType properties.
 */

class DataObjectListImpl : public DataObjectList
{

public:
    DataObjectListImpl(DATAOBJECT_VECTOR p);
    DataObjectListImpl(const DataObjectListImpl &pin);
    DataObjectListImpl();
    DataObjectListImpl(DataFactory* df, DataObjectImpl* cont, unsigned int inpindex, 
                       const char* tURI, const char* tName);

    virtual ~DataObjectListImpl();

    // open type support
    virtual void decrementPindex();

    virtual DataObjectPtr operator[] (int pos);
    virtual const DataObjectPtr operator[] (int pos) const;

    // set/get primitive values 
    virtual bool getBoolean(unsigned int index) const;
    virtual char getByte(unsigned int index) const;
    virtual wchar_t getCharacter(unsigned int index) const;
    virtual unsigned int getString(unsigned int index, wchar_t* buf,
        unsigned int max) const;
    virtual unsigned int getBytes(unsigned int index, char* buf, 
        unsigned int max) const;
    virtual short getShort(unsigned int index) const;
    virtual long getInteger(unsigned int index) const;
    virtual int64_t getLong(unsigned int index) const;
    virtual float getFloat(unsigned int index) const;
    virtual long double getDouble(unsigned int index) const;
    virtual const SDODate  getDate(unsigned int index) const;
    virtual const char*  getCString(unsigned int index) const;
    virtual DataObjectPtr getDataObject(unsigned int index) const;

    virtual void setBoolean(unsigned int index, bool d);
    virtual void setByte(unsigned int index, char d);
    virtual void setCharacter(unsigned int index, wchar_t d);
    virtual void setString(unsigned int index, const wchar_t* d, unsigned int len);
    virtual void setBytes(unsigned int index, const char* d, unsigned int len);
    virtual void setShort(unsigned int index, short d);
    virtual void setInteger(unsigned int index, long d);
    virtual void setLong(unsigned int index, int64_t d);
    virtual void setFloat(unsigned int index, float d);
    virtual void setDouble(unsigned int index, long double d);
    virtual void setDate(unsigned int index, const SDODate d);
    virtual void setCString(unsigned int index, char* d);
    virtual void setDataObject(unsigned int index, DataObjectPtr dob);

    virtual unsigned int getLength(unsigned int index) const;

    virtual int size () const;

    virtual void insert (unsigned int index, DataObjectPtr d);
    virtual void append (DataObjectPtr d);

    virtual  void insert (unsigned int index, bool d) ;
    virtual  void append (bool d) ;

    virtual  void insert (unsigned int index, char d) ;
    virtual  void append (char d) ;

    virtual  void insert (unsigned int index, wchar_t d) ;
    virtual  void append (wchar_t d) ;

    virtual  void insert (unsigned int index, const wchar_t* d, unsigned int len) ;
    virtual  void append (const wchar_t* d, unsigned int len) ;

    virtual  void insert (unsigned int index, const char* d, unsigned int len) ;
    virtual  void append (const char* d, unsigned int len) ;

    virtual  void insert (unsigned int index, const char* d) ;
    virtual  void append (const char* d) ;

    virtual  void insert (unsigned int index, short d) ;
    virtual  void append (short d) ;

    virtual  void insert (unsigned int index, const SDODate d) ;
    virtual  void append (const SDODate d) ;

#if __WORDSIZE !=64
    virtual  void insert (unsigned int index, long d) ;
    virtual  void append (long d) ;
#endif

    virtual  void insert (unsigned int index, int64_t d) ;
    virtual  void append (int64_t d) ;
    
    virtual  void insert (unsigned int index, float d) ;
    virtual  void append (float d) ;

    virtual  void insert (unsigned int index, long double d) ;
    virtual  void append (long double d) ;
    
    
    virtual DataObjectPtr  remove (unsigned int index);

    virtual void checkFactory(DataObjectPtr dob);
    virtual void checkType(const Type& listType, const Type& objectType);
    virtual void setType(const char* uri, const char* name);    
    virtual void setType(const SDOString& uri, const SDOString& name);    

   /*  getType returns type
     *
     * Returns the type.
     * May throw SDOTYpeNotFoundException for unset open types
     */


   virtual const Type& getType();

    /*  getTypeEnum returns an enumerator for the type
     *
     * Returns an enumerator for the type for easy switching on basic types.
     * The enumerator is part of the Type class
     * May throw SDOTypeNotFoundException for open types
     */

    virtual const Type::Types getTypeEnum();


private: 
    DATAOBJECT_VECTOR plist;
    DATAOBJECT_VECTOR getVec() const;

    // For creation of items via the insert/append api.
    char* typeURI;
    char* typeName;

    // for open types, the list may not have a type until the
    // first item is entered

    bool typeUnset;

    // No reference count held on the factory
    DataFactory* theFactory;

    // For logging a change in the change summary when appending
    DataObjectImpl* container;
    unsigned int pindex;
    bool isReference;

    void validateIndex(int index) const;

    static const SDOString BooleanLiteral;
    static const SDOString ByteLiteral;
    static const SDOString CharacterLiteral;
    static const SDOString BytesLiteral;
	static const SDOString StringLiteral;
	static const SDOString IntegerLiteral;
	static const SDOString ShortLiteral;
	static const SDOString DateLiteral;
	static const SDOString LongLiteral;
	static const SDOString FloatLiteral;
	static const SDOString DoubleLiteral;

};
};
};
#endif
