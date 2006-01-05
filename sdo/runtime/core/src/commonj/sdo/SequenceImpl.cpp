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

#include "commonj/sdo/disable_warn.h"
#include "commonj/sdo/Property.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/TypeList.h"

#include "commonj/sdo/SequenceImpl.h"

#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/DataObjectImpl.h"
#include "commonj/sdo/SDORuntimeException.h"



#define CHECK_RANGE\
        if (index >= the_list.size()){\
            string msg("Index out of range:");\
            msg += index;\
            SDO_THROW_EXCEPTION("Sequence API", SDOIndexOutOfRangeException,\
            msg.c_str());\
        }\
        int j = 0;\
        for (i = the_list.begin(); (j < index) && (i != the_list.end()); ++i)\
        {\
            j++;\
        }
    

namespace commonj{
namespace sdo{

    SequenceImpl::SequenceImpl()
    {
        SDO_THROW_EXCEPTION("Sequence constructor", SDORuntimeException,\
        "SequenceImpl::construction without a data object");\
    }

    SequenceImpl::SequenceImpl(DataObject* indo)
    {
        the_do = indo;
    }

    SequenceImpl::SequenceImpl(SequenceImpl* inseq)
    {
        // take a copy of the_list
        the_list = inseq->the_list;
    }

    unsigned int SequenceImpl::size()
    {
          return the_list.size();
    }

    const Property& SequenceImpl::getProperty(unsigned int index)
    {
        SEQUENCE_ITEM_LIST::iterator i;

        if (isText(index))
        {
            string msg("Cannot get property of a text item");
            msg += index;
            SDO_THROW_EXCEPTION("getProperty", SDOUnsupportedOperationException,
            msg.c_str());
        }

        unsigned int j = 0;
        for (i = the_list.begin() ; (j < index) && (i != the_list.end()); ++i){j++;}
        
        if (i != the_list.end()) {
            return *((*i).getProp()); 
        }
        string msg("Index out of range:");
        msg += index;
        SDO_THROW_EXCEPTION("getProperty", SDOIndexOutOfRangeException,
        msg.c_str());
    }

    Type::Types SequenceImpl::getTypeEnum(unsigned int index)
    {
        SEQUENCE_ITEM_LIST::iterator i;
        
        unsigned int j = 0;
        for (i = the_list.begin() ; (j < index) && (i != the_list.end()); ++i){j++;}

        if (i != the_list.end()) {
            if ((*i).getProp() == 0) {
                // text type
                return Type::TextType;
            }
            return (*i).getProp()->getTypeEnum(); 
        }
        string msg("Index out of range:");
        msg += index;
        SDO_THROW_EXCEPTION("getTypeEnum", SDOIndexOutOfRangeException,
        msg.c_str());
    }

    unsigned int SequenceImpl::getListIndex(unsigned int index)
    {
        SEQUENCE_ITEM_LIST::iterator i;
        unsigned int j = 0;
        for (i = the_list.begin() ; (j < index) && (i != the_list.end()); ++i){j++;}

        if (i != the_list.end()) {
            if ((*i).getProp() == 0) 
            {
                string msg("Get list index on text property");
                SDO_THROW_EXCEPTION("getListIndex", SDOUnsupportedOperationException,
                msg.c_str());
            }
            if ( (*i).getProp()->isMany())
            {
                return (*i).getIndex();
            }
            else 
            {
                string msg("Get list index on text single valued property");
                SDO_THROW_EXCEPTION("getListIndex", SDOUnsupportedOperationException,
                msg.c_str());
            }
        }
        string msg("Index out of range:");
        msg += index;
        SDO_THROW_EXCEPTION("getListIndex", SDOIndexOutOfRangeException,
        msg.c_str());
    }

    unsigned int SequenceImpl::getIndex(const char* propName, unsigned int pindex)
    {
        SEQUENCE_ITEM_LIST::iterator i;
        unsigned int j = 0;
        for (i=the_list.begin(), j=0;i != the_list.end();++i,++j) {
            const Property* p = (*i).getProp();
            if (p == 0 ) continue; // avoid text
            if (!strcmp(p->getName(), propName)) {
                if (p->isMany())
                {
                    if( pindex == (*i).getIndex())return j;
                }
                else {
                    return j;
                }
            }
        }
        SDO_THROW_EXCEPTION("getIndex", SDOIndexOutOfRangeException,
        "SequenceImpl::getIndex - property Setting not in sequence");
    }

    unsigned int SequenceImpl::getIndex(const Property& p, unsigned int pindex)
    {
        return getIndex(p.getName(), pindex);
    }

    ///////////////////////////////////////////////////////////////////////////
    // specific getters for prims and data objects
    ///////////////////////////////////////////////////////////////////////////


#define StringGetter(returntype,primtype,defval)\
    returntype SequenceImpl::get##primtype##Value(unsigned int index)\
    {\
        SEQUENCE_ITEM_LIST::iterator i;\
        CHECK_RANGE;\
        const Property* p = (*i).getProp();\
        if (p == 0) {\
            return (*i).getText();\
        }\
        if (p->isMany())\
        {\
            DataObjectList& dol = the_do->getList(*p);\
            DataObject* list_do = dol[(*i).getIndex()];\
            if (list_do != 0)\
            {\
                return ((DataObjectImpl*)list_do)->get##primtype();\
            }\
            return defval;\
        }\
        return the_do->get##primtype(*((*i).getProp()));\
    }

#define getter(returntype,primtype,defval)\
    returntype SequenceImpl::get##primtype##Value(unsigned int index)\
    {\
        SEQUENCE_ITEM_LIST::iterator i;\
        CHECK_RANGE;\
        const Property* p = (*i).getProp();\
        if (p == 0) {\
            return defval;\
        }\
        if (p->isMany())\
        {\
            DataObjectList& dol = the_do->getList(*p);\
            DataObject* list_do = dol[(*i).getIndex()];\
            if (list_do != 0)\
            {\
                return ((DataObjectImpl*)list_do)->get##primtype();\
            }\
            return defval;\
        }\
        return the_do->get##primtype(*((*i).getProp()));\
    }

#define ObjectGetter(returntype,primtype,defval)\
    returntype SequenceImpl::get##primtype##Value(unsigned int index)\
    {\
        SEQUENCE_ITEM_LIST::iterator i;\
        CHECK_RANGE;\
        const Property* p = (*i).getProp();\
        if (p == 0) {\
            return defval;\
        }\
        if (p->isMany())\
        {\
            DataObjectList& dol = the_do->getList(*p);\
            DataObject* list_do = dol[(*i).getIndex()];\
            if (list_do != 0)\
            {\
                    return list_do;\
            }\
            return defval;\
        }\
        return the_do->get##primtype(*((*i).getProp()));\
    }

#define CharGetter(returntype,primtype,defval)\
    unsigned int SequenceImpl::get##primtype##Value(unsigned int index, returntype ptr, unsigned int max)\
    {\
        SEQUENCE_ITEM_LIST::iterator i;\
        CHECK_RANGE;\
        const Property* p = (*i).getProp();\
        if (p == 0) {\
            return defval;\
        }\
        if (p->isMany())\
        {\
            DataObjectList& dol = the_do->getList(*p);\
            DataObject* list_do = dol[(*i).getIndex()];\
            if (list_do != 0)\
            {\
                return ((DataObjectImpl*)list_do)->get##primtype(ptr, max);\
            }\
            return defval;\
        }\
        return the_do->get##primtype(*((*i).getProp()), ptr, max);\
    }


StringGetter(const char* , CString, 0);

CharGetter( wchar_t* , String, 0);
CharGetter( char*, Bytes, 0);

unsigned int SequenceImpl::getLength(unsigned int index)
{
    SEQUENCE_ITEM_LIST::iterator i;
    CHECK_RANGE;
    const Property* p = (*i).getProp();
    switch (p->getTypeEnum())
    {
    case Type::StringType:
        return getStringValue(index,0,0);
    case Type::BytesType:
        return getBytesValue(index,0,0);
    default:
        return 0;
    }
}

getter(bool, Boolean, false);
getter(char, Byte, (char)0);
getter(wchar_t, Character, 0);
getter(short, Short, 0);
getter(long, Integer, 0L);
getter(int64_t, Long, 0);
getter(float, Float, (float)0);
getter(long double, Double, 0);
getter(const SDODate, Date, 0);

ObjectGetter(RefCountingPointer<DataObject> , DataObject, 0);


    ///////////////////////////////////////////////////////////////////////////
    // generic getter for those types which support it
    ///////////////////////////////////////////////////////////////////////////
    
    void SequenceImpl::setCStringValue(    unsigned int index, const char* s )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            (*i).setText(s);
            return;
        }
        the_do->setCString(*((*i).getProp()),s);

    }
    void SequenceImpl::setBooleanValue(   unsigned int index, bool        b )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setBoolean(*((*i).getProp()),b);
    }

    void SequenceImpl::setByteValue(      unsigned int index, char        c )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setByte(*((*i).getProp()),c);
    }

    void SequenceImpl::setCharacterValue(     unsigned int index, wchar_t     c )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setCharacter(*((*i).getProp()),c);
    }
    void SequenceImpl::setStringValue(   unsigned int index, const wchar_t* s , unsigned int len)
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setString(*((*i).getProp()),s, len);
    }
    void SequenceImpl::setBytesValue(   unsigned int index, const char* s , unsigned int len)
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setBytes(*((*i).getProp()),s, len);
    }
    void SequenceImpl::setShortValue(     unsigned int index, short       s )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setShort(*((*i).getProp()),s);
    }

    void SequenceImpl::setIntegerValue(       unsigned int index, long         l)
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setInteger(*((*i).getProp()),l);
    }


    void SequenceImpl::setLongValue(  unsigned int index, int64_t     l )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setLong(*((*i).getProp()),l);
    }

    void SequenceImpl::setFloatValue(     unsigned int index, float       f )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setFloat(*((*i).getProp()),f);
    }


    void SequenceImpl::setDoubleValue(unsigned int index, long double d )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setDouble(*((*i).getProp()),d);
    }

    void SequenceImpl::setDateValue(unsigned int index, const SDODate t )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setDate(*((*i).getProp()),t);
    }

    void SequenceImpl::setDataObjectValue(unsigned int index, DataObjectPtr d )
    {
        SEQUENCE_ITEM_LIST::iterator i;
        CHECK_RANGE;
        if ((*i).getProp() == 0) {
            return;
        }
        the_do->setDataObject(*((*i).getProp()),d);
    }
  


#define adder(primtype,primval)\
    bool SequenceImpl::add##primtype(const char* propertyName, primval v)\
    {\
        return add##primtype(the_do->getProperty(propertyName),v);\
    }\
    bool SequenceImpl::add##primtype(unsigned int propertyIndex, primval v)\
    {\
        return add##primtype(the_do->getProperty(propertyIndex), v);\
    }\
    bool SequenceImpl::add##primtype(const Property& p, primval v)\
    {\
        if (p.isMany())\
        {\
            DataObjectList& dol = the_do->getList(p);\
            dol.append((primval)v);\
            /* the_list.push_back(seq_item(&p,dol.size()-1));*/\
            return true;\
        }\
        SEQUENCE_ITEM_LIST::iterator i;\
        for (i=the_list.begin();i != the_list.end();++i) {\
            const Property* pp = (*i).getProp();\
            if (pp == 0) continue;\
            if (!strcmp(pp->getName(),p.getName()))\
            {\
            SDO_THROW_EXCEPTION("add", SDOUnsupportedOperationException,\
            "Sequence::add of property which already exists in sequence");\
            }\
        }\
        the_do->set##primtype(p,v);\
        the_list.push_back(seq_item(&p,0));\
        return true;\
    }

adder( CString , const char* );
adder( Boolean, bool);
adder( Byte, char );
adder(Character, wchar_t);
adder(Short, short);
adder(Integer, long);
adder(Long, int64_t);
adder(Float, float);
adder(Double, long double);
adder(Date, const SDODate);
adder(DataObject, RefCountingPointer<DataObject>);


#define charAdder(primtype,primval)\
    bool SequenceImpl::add##primtype(const char* propertyName, primval v, unsigned int len)\
    {\
        return add##primtype(the_do->getProperty(propertyName),v, len);\
    }\
    bool SequenceImpl::add##primtype(unsigned int propertyIndex, primval v, unsigned int len)\
    {\
        return add##primtype(the_do->getProperty(propertyIndex), v, len);\
    }\
    bool SequenceImpl::add##primtype(const Property& p, primval v, unsigned int len)\
    {\
        if (p.isMany())\
        {\
            DataObjectList& dol = the_do->getList(p);\
            dol.append((primval)v, len);\
            /*the_list.push_back(seq_item(&p,dol.size()-1));*/\
            return true;\
        }\
        SEQUENCE_ITEM_LIST::iterator i;\
        for (i=the_list.begin();i != the_list.end();++i) {\
            const Property* pp = (*i).getProp();\
            if (pp == 0) continue;\
            if (!strcmp(pp->getName(),p.getName()))\
            {\
            SDO_THROW_EXCEPTION("add", SDOUnsupportedOperationException,\
            "Sequence::add of property which already exists in sequence");\
            }\
        }\
        the_do->set##primtype(p,v, len);\
        the_list.push_back(seq_item(&p,0));\
        return true;\
    }


charAdder ( String, const wchar_t* );
charAdder ( Bytes , const char* );



#define inserter(primtype,primval)\
    bool SequenceImpl::add##primtype(unsigned int index, const char* propertyName, primval v)\
    {\
        return add##primtype(index,the_do->getProperty(propertyName),v);\
    }\
    bool SequenceImpl::add##primtype(unsigned int index, unsigned int propertyIndex, primval v)\
    {\
        return add##primtype(index,the_do->getProperty(propertyIndex), v);\
    }\
    bool SequenceImpl::add##primtype(unsigned int index, const Property& p, primval v)\
    {\
        SEQUENCE_ITEM_LIST::iterator i;\
        SEQUENCE_ITEM_LIST::iterator i2 = the_list.end();\
        int j;\
        if (index >= the_list.size()) {\
            return add##primtype(p,v);\
        }\
        if (p.isMany())\
        {\
            DataObjectList& dol = the_do->getList(p);\
            dol.append((primval)v);\
            j = 0;\
            for (i=the_list.begin();(j < index) && (i != the_list.end());++i) {\
                j++;\
            }\
            /*the_list.insert(i,seq_item(&p,dol.size()-1));*/\
            return true;\
        }\
        j = 0;\
        for (i=the_list.begin();i != the_list.end();++i) {\
            const Property* pp = (*i).getProp();\
            if (pp == 0) continue;\
            if (!strcmp(pp->getName(),p.getName()))\
            {\
                SDO_THROW_EXCEPTION("Insert", SDOUnsupportedOperationException,\
                "Sequence::insert of property which already exists in sequence");\
            }\
            if (j == index) {\
                i2 = i;\
            }\
            j++;\
        }\
        the_do->set##primtype(p,v);\
        the_list.insert(i2,seq_item(&p,0));\
        return true;\
    }

inserter( CString , const char* );
inserter( Boolean, bool);
inserter( Byte, char );
inserter(Character, wchar_t);
inserter(Short, short);
inserter(Integer, long);
inserter(Long, int64_t);
inserter(Float, float);
inserter(Double, long double);
inserter(Date, const SDODate);
inserter(DataObject, RefCountingPointer<DataObject>);



#define charInserter(primtype,primval)\
    bool SequenceImpl::add##primtype(unsigned int index, const char* propertyName, primval v, unsigned int len)\
    {\
        return add##primtype(index,the_do->getProperty(propertyName),v, len);\
    }\
    bool SequenceImpl::add##primtype(unsigned int index, unsigned int propertyIndex, primval v, unsigned int len)\
    {\
        return add##primtype(index,the_do->getProperty(propertyIndex), v, len);\
    }\
    bool SequenceImpl::add##primtype(unsigned int index, const Property& p, primval v, unsigned int len)\
    {\
        SEQUENCE_ITEM_LIST::iterator i;\
        SEQUENCE_ITEM_LIST::iterator i2 = the_list.end();\
        unsigned int j;\
        if (index >= the_list.size()) {\
            return add##primtype(p,v, len);\
        }\
        if (p.isMany())\
        {\
            DataObjectList& dol = the_do->getList(p);\
            dol.append((primval)v, len);\
            j = 0;\
            for (i=the_list.begin();(j < index) && (i != the_list.end());++i) {\
                j++;\
            }\
            /*the_list.insert(i,seq_item(&p,dol.size()-1));*/\
            return true;\
        }\
        j = 0;\
        for (i=the_list.begin();i != the_list.end();++i) {\
            const Property* pp = (*i).getProp();\
            if (pp == 0) continue;\
            if (!strcmp(pp->getName(),p.getName()))\
            {\
                SDO_THROW_EXCEPTION("Insert", SDOUnsupportedOperationException,\
                "Sequence::insert of property which already exists in sequence");\
            }\
            if (j == index) {\
                i2 = i;\
            }\
            j++;\
        }\
        the_do->set##primtype(p,v, len);\
        the_list.insert(i2,seq_item(&p,0));\
        return true;\
    }


charInserter( String , const wchar_t* );
charInserter (Bytes , const char* );





    void SequenceImpl::push(const Property& p, unsigned int index)
    {
        the_list.push_back(seq_item(&p,index));
    }

    void SequenceImpl::remove(unsigned int index)
    {
        if (index >= the_list.size()) {
            string msg("Index out of range:");
            msg += index;
            SDO_THROW_EXCEPTION("Sequence remove", SDOIndexOutOfRangeException,
            msg.c_str());
        }
        SEQUENCE_ITEM_LIST::iterator i;
        int j = 0;
        for (i=the_list.begin();(j < index) && (i != the_list.end()) ; ++i)
        {
            j++;
        }
        the_list.erase(i);
        return;
    }

    void SequenceImpl::removeAll(const Property& p)
    {
        int j = 0;
        const Property* prop;
        SEQUENCE_ITEM_LIST::iterator i;
        for (i=the_list.begin();i != the_list.end(); ++i)
        {
            prop = (*i).getProp();
            if (prop != 0) {
                if (!strcmp(prop->getName(),
                            p.getName()))
                {
                    // not sure this works - does i still equate
                    // to an iterator over the list after the remove?
                    i = the_list.erase(i);
                }
            }
        }
    
        return;
    }

    void SequenceImpl::move(unsigned int toIndex, unsigned int fromIndex)
    {
        if (fromIndex >= the_list.size()) {
            string msg("Index out of range:");
            msg += fromIndex;
            SDO_THROW_EXCEPTION("Sequence Move", SDOIndexOutOfRangeException,
            msg.c_str());
        }

        if (toIndex == fromIndex) return;

        SEQUENCE_ITEM_LIST::iterator i1,
                            i2 = the_list.end(), 
                            i3 = the_list.end();
        int j = 0;
        for (i3 = the_list.begin(); 
             j < toIndex && j < fromIndex && 
                 i3 != the_list.end() ; ++i3);
        {
            if (j == toIndex)   i1 = i3;
            if (j == fromIndex) i2 = i3;
            j++;
        }

        if (toIndex < fromIndex) 
        {
            the_list.insert( i1, *i2);
            the_list.erase(i2);
        }
        else 
        {
            if (toIndex + 1 == the_list.size()) 
            {
                the_list.push_back(*i2);
            }
            else
            {
                the_list.insert(++i1,*i2);
            }
            the_list.erase(i2);
        }
        return;
    }

    bool SequenceImpl::addText(const char* text)
    {
        the_list.push_back(seq_item(text));
        return true;
    }

    bool SequenceImpl::isText(unsigned int index)
    {
        if (index >= the_list.size()) {
            return false;
        }
        SEQUENCE_ITEM_LIST::iterator i;
        int j = 0;
        for (i= the_list.begin(); (j < index) && (i != the_list.end()); ++i)
        {
            j++;
        }
        if ((*i).getProp() == 0)
        {
            return true;
        }

        return false;
    }

    bool SequenceImpl::addText(unsigned int index, const char* text)
    {
        if (index >= the_list.size()) {
            return addText(text);
        }

        SEQUENCE_ITEM_LIST::iterator i;
        int j = 0;
        for (i= the_list.begin(); (j < index) && (i != the_list.end()); ++i)
        {
            j++;
        }
        the_list.insert(i,seq_item(text));
        return true;
    }

    bool SequenceImpl::setText(unsigned int index, const char* text)
    {
        if (index >= the_list.size()) {
            return false;
        }

        if (!isText(index))
        {
            return false;
        }

        SEQUENCE_ITEM_LIST::iterator i;
        int j = 0;
        for (i= the_list.begin(); (j < index) && (i != the_list.end()); ++i)
        {
            j++;
        }
        (*i).setText(text);
        return true;
    }

};
};

