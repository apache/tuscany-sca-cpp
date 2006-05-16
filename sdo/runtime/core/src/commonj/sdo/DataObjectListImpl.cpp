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

/* $Rev$ $Date: 2006/04/07 13:35:33 $ */

#include "commonj/sdo/DataObjectListImpl.h"


#include <iostream>
#include "commonj/sdo/Property.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/Logger.h"
#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/DataFactory.h"
#include "commonj/sdo/DataObjectImpl.h"
#include "commonj/sdo/DataFactoryImpl.h"

#include <stdio.h>

namespace commonj{
namespace sdo {

/** 
 * DataObjectListImpl implements DataObjectList.
 * List for retrieving many valued property values.
 *
 * DataObjectList provides an API for getting and setting values in
 * many valued properties. The list deals with both DataType and 
 * DataObjectType properties.
 */
DataObjectListImpl::DataObjectListImpl(DATAOBJECT_VECTOR p) : plist (p)
{
    theFactory = 0;
    container  = 0;
    pindex     = 0;
    isReference = false;
}

DataObjectListImpl::DataObjectListImpl(const DataObjectListImpl &pin)
{
    plist = std::vector<RefCountingPointer<DataObjectImpl> >(pin.getVec());
    theFactory = pin.theFactory;
    container = pin.container;
    pindex = pin.pindex;
    typeUnset = pin.typeUnset;
    isReference = pin.isReference;
    if (pin.typeURI != 0) {
        typeURI = new char[strlen(pin.typeURI) +1];
        strcpy(typeURI, pin.typeURI);
    }
    if (pin.typeName != 0) {
        typeName = new char[strlen(pin.typeName) +1];
        strcpy(typeName, pin.typeName);
    }
}

DataObjectListImpl::DataObjectListImpl()
{
    theFactory = 0;
    typeURI    = 0;
    typeName   = 0;
    theFactory = 0;
    container  = 0;
    pindex     = 0;
    typeUnset  = false;
    isReference = false;
}

DataObjectListImpl::DataObjectListImpl(DataFactory* df, 
                                       DataObjectImpl* cont,
                                       unsigned int inpindex,
                                       const char* intypeURI,
                                       const char* intypeName)
{
    container = cont;
    pindex = inpindex;
    theFactory = df;


    isReference = false;
    if (container->getProperty(pindex).isReference())
    {
        isReference = true;
    }
    typeUnset = false;

    if (container->getType().isOpenType())
    {
        if (!strcmp(intypeURI,Type::SDOTypeNamespaceURI) &&
            !strcmp(intypeName,"OpenDataObject"))
        {
            typeUnset = true;
        }
    }


    if (intypeURI != 0) {
        typeURI = new char[strlen(intypeURI) +1];
        strcpy(typeURI, intypeURI);
    }
    else {
        typeURI = 0;
    }
    if (intypeName != 0) {
        typeName = new char[strlen(intypeName) +1];
        strcpy(typeName, intypeName);
    }
    else {
        typeName = 0;
        theFactory = 0;
    }
}

DataObjectListImpl::~DataObjectListImpl()
{
    if (typeURI != 0) {
        delete typeURI;
        typeURI = 0;
    }
    if (typeName != 0) {
        delete typeName;
        typeName = 0;
    }
}

RefCountingPointer<DataObject> DataObjectListImpl::operator[] (int pos)
{
    validateIndex(pos);
    return plist[pos];
}

const RefCountingPointer<DataObject> DataObjectListImpl::operator[] (int pos) const
{
    validateIndex(pos);
    RefCountingPointer<DataObjectImpl> d = plist[pos];
    DataObjectImpl* dob = d;
    return  RefCountingPointer<DataObject>((DataObject*)dob);
}


int DataObjectListImpl::size () const
{
    return plist.size();
}

DATAOBJECT_VECTOR DataObjectListImpl::getVec() const
{
    return plist;
}



const Type& DataObjectListImpl::getType()
{
    if (typeUnset)
    {
        string msg("The list property is open, and the type of the contents has not bee determined yet.");
        SDO_THROW_EXCEPTION("Get Type", SDOTypeNotFoundException,
            msg.c_str());
    }
    return theFactory->getType(typeURI, typeName);
}


const Type::Types DataObjectListImpl::getTypeEnum()
{
    if (typeUnset)
    {
        return Type::OpenDataObjectType;

    }
    return theFactory->getType(typeURI, typeName).getTypeEnum();
}


void DataObjectListImpl::insert (unsigned int index, DataObjectPtr d)
{
    if (typeUnset)setType(d->getType().getURI(),d->getType().getName());

    checkFactory(d);

    checkType(theFactory->getType(typeURI,typeName),
                d->getType());

    if (container != 0)
    {
        container->logChange(pindex);
    }
    for (int i=0;i < plist.size(); i++)
    {
        if (plist[i] == d)
        {
        string msg("Insertion of object which already exists in the list:");
        msg += typeURI;
        msg += " ";
        msg += typeName;
        SDO_THROW_EXCEPTION("List insert", SDOUnsupportedOperationException,
            msg.c_str());
        }
    }
    if (strcmp(typeURI,d->getType().getURI()) 
        || 
        strcmp(typeName,d->getType().getName()))
    {
        string msg("Insertion of object of the wrong type to a list:");
        msg += typeURI;
        msg += " ";
        msg += typeName;
        msg += " not compatible with ";
        msg += d->getType().getURI();
        msg += " ";
        msg += d->getType().getName();
        SDO_THROW_EXCEPTION("List append", SDOInvalidConversionException,
            msg.c_str());
    }

    DataObject* dob = d; // unwrap the data object ready for a downcasting hack.
    DataObjectImpl* con  = ((DataObjectImpl*)dob)->getContainerImpl();  
    if (!isReference)
    {
        if (con != 0)
        {
            if (con != container)
            {
                /* this data object is already contained somewhere else */
                string msg("Insertion of object to list, object is already contained:");
                msg += d->getType().getURI();
                msg += " ";
                msg += d->getType().getName();
                SDO_THROW_EXCEPTION("List append", SDOInvalidConversionException,
                    msg.c_str());
            }
        }
        else 
        {
            ((DataObjectImpl*)dob)->setContainer(container);
            ((DataObjectImpl*)dob)->setApplicableChangeSummary();
            ((DataObjectImpl*)dob)->logCreation((DataObjectImpl*)dob,
                (DataObjectImpl*)container,
                container->getProperty(pindex));
        }
    }

    plist.insert(plist.begin()+index, RefCountingPointer<DataObjectImpl>((DataObjectImpl*)dob));

    if (container != 0) 
    {
        if (container->getType().isSequencedType())
        {
            SequenceImpl* sq = container->getSequenceImpl();
            if (sq)sq->push(container->getProperty(pindex),index);
        }
    }

}



  void DataObjectListImpl::checkFactory(DataObjectPtr dob)
    {
        
        DataObjectImpl* d = (DataObjectImpl*)(DataObject*)dob;

        if (d->getDataFactory() == theFactory) return;

        // temporary experiment with allowing data objects
        // to move from factory to factory if the type is 
        // nominally present, and the type systems match

        DataFactoryImpl* f = (DataFactoryImpl*)theFactory;


        if (d->getContainer() != 0)
        {
           string msg("Insertion of object into list from another factory is only allowed if the parent is null: ");
        
            const Type& t = d->getType();
            msg += t.getURI();
            msg += "#";
            msg += t.getName();
            msg += " into property  ";
            msg += container->getProperty(pindex).getName();
            msg += " of type ";
            msg += typeURI;
            msg += "#";
            msg += typeName;
            SDO_THROW_EXCEPTION("checkFactory", SDOInvalidConversionException,
            msg.c_str());
        }

        if (f->isCompatible(d->getDataFactory(),d))
        {
            d->setDataFactory(theFactory);
            // we will also need to transfer any children - assuming they
            // are ok in the new factory!!
            d->transferChildren(d,theFactory);
            return;
        }

        string msg("Insertion into list fromm incompatible factory:");
        
        const Type& t = d->getType();
        msg += t.getURI();
        msg += "#";
        msg += t.getName();
        msg += " into property  ";
        msg += container->getProperty(pindex).getName();
        msg += " of type ";
        msg += typeURI;
        msg += "#";
        msg += typeName;
        SDO_THROW_EXCEPTION("checkFactory", SDOInvalidConversionException,
            msg.c_str());

    }


void DataObjectListImpl::checkType(const Type& listType, const Type& objectType)
    {
        if (listType.equals(objectType)) return;

        const TypeImpl* ti = ((DataFactoryImpl*)theFactory)->findTypeImpl
            (objectType.getURI(),objectType.getName());
        if (ti != 0)
        {
            do  
            {
                ti  = (const TypeImpl*)ti->getBaseType();
                if (ti == 0) break;
                if (listType.equals(*ti)) return;
            } while (ti != 0);

            // allow types of any substitutes
            if (container != 0)
            {
                PropertyImpl* pi = 
                    container->getPropertyImpl(pindex);
                if (pi != 0)
                {
                    unsigned int subcount = pi->getSubstitutionCount();
                    for (int i=0;i<subcount;i++)
                    {
                        const Type* tsub = pi->getSubstitutionType(i);
                        if (tsub != 0 && tsub->equals(objectType)) return;
                    }
                }
            }
        }

        // no match..
        string msg("Insertion of object of incompatible type ");
        msg += objectType.getURI();
        msg += "#";
        msg += objectType.getName();
        msg += " into list of type ";
        msg += listType.getURI();
        msg += "#";
        msg += listType.getName();
        SDO_THROW_EXCEPTION("TypeCheck", SDOInvalidConversionException,
            msg.c_str());
    }


void DataObjectListImpl::setType(const char* uri, const char* name)    
{
    // need to check for an opentype list which has not been set up yet
    if (name == 0) return;

    const TypeImpl* t = ((DataFactoryImpl*)theFactory)->findTypeImpl(uri,name);
    if (t == 0) return; // cannot set to a type which is not avilable

    // need to modify the instance property of the container
    container->setInstancePropertyType(pindex,t);

    delete typeName;
    typeName = new char[strlen(name)+1];
    strcpy(typeName, name);
    delete typeURI;
    if (uri == 0) 
    {
        typeURI = new char[1];
        typeURI[0] = 0;
    }
    else 
    {
        typeURI = new char[strlen(uri)+1];
        strcpy(typeURI, uri);
    }
    typeUnset = false;
}


void DataObjectListImpl::append (DataObjectPtr d)
{

    if (typeUnset)setType(d->getType().getURI(),d->getType().getName());

    if (container != 0)
    {
        container->logChange(pindex);
    }

    for (int i=0;i < plist.size(); i++)
    {
        if (plist[i] == d)
        {
        string msg("Append of object which already exists in the list:");
        msg += typeURI;
        msg += " ";
        msg += typeName;
        SDO_THROW_EXCEPTION("List append", SDOUnsupportedOperationException,
            msg.c_str());
        }
    }

    checkFactory(d);

    checkType(theFactory->getType(typeURI,typeName),
                d->getType());

    DataObject* dob = d; // unwrap the data object ready for a downcasting hack.
    DataObjectImpl* con  = ((DataObjectImpl*)dob)->getContainerImpl();  
    
    if (!isReference)
    {
        if (con != 0)
        {
            if (con != container)
            {
                /* this data object is already contained somewhere else */
                string msg("Append of object to list, object is already contained:");
                msg += d->getType().getURI();
                msg += " ";
                msg += d->getType().getName();
                SDO_THROW_EXCEPTION("List append", SDOInvalidConversionException,
                    msg.c_str());
            }
        }
        else 
        {
            ((DataObjectImpl*)dob)->setContainer(container);
            ((DataObjectImpl*)dob)->setApplicableChangeSummary();
            if (!container->getProperty(pindex).getType().isDataType())
            {
                ((DataObjectImpl*)dob)->logCreation((DataObjectImpl*)dob,
                    container,container->getProperty(pindex));
            }

        }
    }
    plist.insert(plist.end(),RefCountingPointer<DataObjectImpl>((DataObjectImpl*)dob));

    if (container != 0) {
        if (container->getType().isSequencedType())
        {
            SequenceImpl* sq = container->getSequenceImpl();
            if (sq)sq->push(container->getProperty(pindex),plist.size()-1);
        }
    }
}

void DataObjectListImpl::insert (unsigned int index, bool d)
{
    if (theFactory == 0) return;
    
    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Boolean");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setBoolean(d);
    insert(index, dol);
}

void DataObjectListImpl::append (bool d) 
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Boolean");
    
    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setBoolean(d);
    append( dol);
}

void DataObjectListImpl::insert (unsigned int index, char d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Byte");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setByte(d);
    insert(index, dol);
}

void DataObjectListImpl::append (char d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Byte");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setByte(d);
    append( dol);
}

void DataObjectListImpl::insert (unsigned int index, wchar_t d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Character");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setCharacter(d);
    insert(index, dol);
}

void DataObjectListImpl::append (wchar_t d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Character");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setCharacter(d);
    append( dol);
}

void DataObjectListImpl::insert (unsigned int index, const wchar_t* d, unsigned int length)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "String");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setString(d, length);
    insert(index, dol);
}

void DataObjectListImpl::append (const wchar_t* d, unsigned int length)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "String");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setString(d, length);
    append( dol);
}
void DataObjectListImpl::insert (unsigned int index, const char* d, unsigned int length)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Bytes");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setBytes(d, length);
    insert(index, dol);
}

void DataObjectListImpl::append (const char* d, unsigned int length)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Bytes");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setBytes(d, length);
    append( dol);
}
void DataObjectListImpl::insert (unsigned int index, const char* d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Bytes");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setCString(d);
    insert(index, dol);
}

void DataObjectListImpl::append (const char* d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Bytes");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setCString(d);
    append( dol);
}

void DataObjectListImpl::insert (unsigned int index, short d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Short");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setShort(d);
    insert(index, dol);
}

void DataObjectListImpl::append (short d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Short");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setShort(d);
    append( dol);
}


#if __WORDSIZE !=64
void DataObjectListImpl::insert (unsigned int index, long d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Integer");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setInteger(d);
    insert(index, dol);
}

void DataObjectListImpl::append (long d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Integer");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setInteger(d);
    append( dol);
}
#endif

void DataObjectListImpl::insert (unsigned int index, const SDODate d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Date");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setDate(d);
    insert(index, dol);
}

void DataObjectListImpl::append (const SDODate d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Date");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setDate(d);
    append( dol);
}

void DataObjectListImpl::insert (unsigned int index, int64_t d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Long");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setLong(d);
    insert(index, dol);
}

void DataObjectListImpl::append (int64_t d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Long");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setLong(d);
    append( dol);
}

void DataObjectListImpl::insert (unsigned int index, float d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Float");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setFloat(d);
    insert(index, dol);
}

void DataObjectListImpl::append (float d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Float");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setFloat(d);
    append( dol);
}

void DataObjectListImpl::insert (unsigned int index, long double d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Double");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setDouble(d);
    insert(index, dol);
}

void DataObjectListImpl::append (long double d)
{
    if (theFactory == 0) return;

    if (typeUnset)setType(Type::SDOTypeNamespaceURI, "Double");

    RefCountingPointer<DataObject> dol = theFactory->create(typeURI, typeName);
    DataObject* dob = dol;
    ((DataObjectImpl*)dob)->setDouble(d);
    append( dol);
}


void DataObjectListImpl::decrementPindex()
{
    pindex--;
}


RefCountingPointer<DataObject> DataObjectListImpl::remove(unsigned int index)
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> d = (*this)[index];
    
    // log deletion only if the list is of data objects.
    if (theFactory != 0) 
    {
        const Type& t = theFactory->getType(typeURI,typeName);
        const Property& p = container->getProperty(pindex);
        if (!t.isDataType() && !p.isReference())
        {
            (getVec()[index])->logDeletion();
        }
    }
    plist.erase(plist.begin()+index);
    DataObject* dob = d;
    ((DataObjectImpl*)dob)->setContainer(0);
    return d;
}

void DataObjectListImpl::validateIndex(int index) const
{
    if ((index < 0) || (index >= size()))
    {
        char val[32];
        string msg("Index out of range:");
        sprintf(val,"%d",index);
        msg += val;
        SDO_THROW_EXCEPTION("validateIndex", SDOIndexOutOfRangeException,
            msg.c_str());

    }

}

bool        DataObjectListImpl::getBoolean(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getBoolean();
}
char        DataObjectListImpl::getByte(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getByte();
}
wchar_t     DataObjectListImpl::getCharacter(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getCharacter();
}
unsigned int  DataObjectListImpl::getBytes(unsigned int index, char* value, unsigned int max) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getBytes(value, max);
}
unsigned int  DataObjectListImpl::getString(unsigned int index, wchar_t* value, unsigned int max) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getString(value, max);
}
short       DataObjectListImpl::getShort(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getShort();
}
long         DataObjectListImpl::getInteger(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getInteger();
}
int64_t     DataObjectListImpl::getLong(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getLong();
}
float       DataObjectListImpl::getFloat(unsigned int index) const 
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]); 
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getFloat();
}
long double DataObjectListImpl::getDouble(unsigned int index) const 
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getDouble();
}
const SDODate      DataObjectListImpl::getDate(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getDate();
}
const char* DataObjectListImpl::getCString(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> d = ((*this)[index]);
    DataObject* dob = d;
    return ((DataObjectImpl*)dob)->getCString();
}

DataObjectPtr DataObjectListImpl::getDataObject(unsigned int index) const
{
    validateIndex(index);
    return (*this)[index];
}

void DataObjectListImpl::setBoolean(unsigned int index, bool d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setBoolean(d);
}
void DataObjectListImpl::setByte(unsigned int index, char d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setByte(d);
}
void DataObjectListImpl::setCharacter(unsigned int index, wchar_t d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setCharacter(d);
}

void DataObjectListImpl::setString(unsigned int index, const wchar_t* d, unsigned int len) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setString(d, len);
}
void DataObjectListImpl::setBytes(unsigned int index, const char* d, unsigned int len) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setBytes(d, len);
}

void DataObjectListImpl::setShort(unsigned int index, short d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setShort(d);
}
void DataObjectListImpl::setInteger(unsigned int index, long d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setInteger(d);
}
void DataObjectListImpl::setLong(unsigned int index, int64_t d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setLong(d);
}
void DataObjectListImpl::setFloat(unsigned int index, float d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setFloat(d);
}
void DataObjectListImpl::setDouble(unsigned int index, long double d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setDouble(d);
}
void DataObjectListImpl::setDate(unsigned int index, const SDODate d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setDate(d);
}
void DataObjectListImpl::setCString(unsigned int index, char* d) 
{
    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    ((DataObjectImpl*)dob)->setCString(d);
}

void DataObjectListImpl::setDataObject(unsigned int index, DataObjectPtr dob) 
{

    if (dob != 0)
    {
        checkFactory(dob);

        checkType(theFactory->getType(typeURI,typeName),
                dob->getType());
    }

    validateIndex(index);
    if (container != 0)
    {
        container->logChange(pindex);
    }

    remove(index);
    insert(index,dob);
}

unsigned int DataObjectListImpl::getLength(unsigned int index) const
{
    validateIndex(index);
    RefCountingPointer<DataObject> dd = ((*this)[index]);
    DataObject* dob = dd;
    return dob->getLength();
}

};
};
