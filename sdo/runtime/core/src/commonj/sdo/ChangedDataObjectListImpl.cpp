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


/* $Rev$ $Date$ */


#include "commonj/sdo/ChangedDataObjectListImpl.h"


#include <iostream>
#include "commonj/sdo/Property.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/Logger.h"
#include "commonj/sdo/SDORuntimeException.h"
#include "commonj/sdo/DataObjectImpl.h"


namespace commonj{
namespace sdo {

    /**  CDOListElement
     *
     * constructor
     */

    CDOListElement::CDOListElement()
    {
        theObject = 0;
        theType = ChangedDataObjectList::Undefined;
    }

    /**  CDOListElement
     *
     * constructor from a data object and a change
     */

    CDOListElement::CDOListElement(DataObject* in, ChangedDataObjectList::ChangeType type)
    {
        theObject = in;
        theType = type;
    }

   /**  ~CDOListElement()
     *
     * destructor
     */

    CDOListElement::~CDOListElement()
    {
    }

   /**  getObject()
     *
     * return the changed data object
     */

    DataObject*    CDOListElement::getObject() const
    {
        return theObject;
    }

   /**  
     *
     * return the type of change getType()
     */

    ChangedDataObjectList::ChangeType CDOListElement::getType() const
    {
        return theType;
    }

   /**  ChangedDataObjectListImpl
     *
     * constructor
     */

    ChangedDataObjectListImpl::ChangedDataObjectListImpl(CHANGEDDATAOBJECT_VECTOR p) : plist (p)
    {
    }

   /**  ChangedDataObjectListImpl
     *
     * copy constructor 
     */

    ChangedDataObjectListImpl::ChangedDataObjectListImpl(const ChangedDataObjectListImpl &pin)
    {
    plist = std::vector< CDOListElement >(pin.getVec());
    }

   /**  ChangedDataObjectListImpl
     *
     * default constructor
     */

    ChangedDataObjectListImpl::ChangedDataObjectListImpl()
    {
    }


   /**  ~ChangedDataObjectListImpl
     *
     * destructor
     */

    ChangedDataObjectListImpl::~ChangedDataObjectListImpl()
    {
    }

   /**  [] index operator
     *
     *
     */

    RefCountingPointer<DataObject> ChangedDataObjectListImpl::operator[] (int pos)
    {
        validateIndex(pos);
        return plist[pos].getObject();
    }

   /**  [] index operator
     *
     *
     */

    const RefCountingPointer<DataObject> ChangedDataObjectListImpl::operator[] (int pos) const
    {    
        validateIndex(pos);
        return  plist[pos].getObject();
    }

   /**  get() gets an element at the index.
     *
     */

    DataObject* ChangedDataObjectListImpl::get(unsigned int pos)
    {
        validateIndex(pos);
        return plist[pos].getObject();
    }

   /**  size() returns the length of the list.
     *
     *
     */

    int ChangedDataObjectListImpl::size () const
    {
        return plist.size();
    }

   /** getVec()
     * gets the vector which actually stores the data objects.
     */

    CHANGEDDATAOBJECT_VECTOR ChangedDataObjectListImpl::getVec() const
    {
        return plist;
    }

   /**  getType()
     *  returns the type of the change.
     */

    ChangedDataObjectList::ChangeType ChangedDataObjectListImpl::getType(unsigned int pos) 
    {
        validateIndex(pos);
        return plist[pos].getType();
    }


   /**  insert()
     *  inserts an element into the list.
     */

    void ChangedDataObjectListImpl::insert (unsigned int index, 
                                        DataObject *d, ChangedDataObjectList::ChangeType type)
    {
        plist.insert(plist.begin()+index, CDOListElement((DataObject*)d,type));
    }

   /**  append()
     *  appends to the end of the list.
     */

    void ChangedDataObjectListImpl::append (DataObject *d, ChangedDataObjectList::ChangeType type)
    {
        plist.insert(plist.end(),CDOListElement((DataObject*)d, type));
    }

   /**  clear()
     *  empties the list.
     */

    void ChangedDataObjectListImpl::clear ()
    {
        plist.clear();
    }


   /**  remove()
     *  removes an item from the list.
     */

    void ChangedDataObjectListImpl::remove(unsigned int index)
    {
        validateIndex(index);
        plist.erase(plist.begin() +index);
        return ;
    }

   /**  validateIndex()
     *  checks that the index is in range.
     */

    void ChangedDataObjectListImpl::validateIndex(int index) const
    {
        if ((index < 0) || (index >= size()))
        {
            std::string msg("Invalid index : ");
            msg += index;
            SDO_THROW_EXCEPTION("(ChangeSummary)validateIndex", SDOIndexOutOfRangeException,
                msg.c_str());

        }

    }


};
};

