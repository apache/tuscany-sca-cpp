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

#ifndef _ChangedDataObjectListImplIMPL_H_
#define _ChangedDataObjectListImplIMPL_H_

#include "commonj/sdo/disable_warn.h"


#include <vector>

#include "commonj/sdo/ChangedDataObjectList.h"
namespace commonj{
namespace sdo{

class DataObjectImpl;
class DataObject;

/** 
 * CDOListElement is an element of the changed data object list
 * in a change summary,it may be a delete, addition or change.
 */

class CDOListElement 
{
public:
    CDOListElement ();
    CDOListElement (DataObject* in, ChangedDataObjectList::ChangeType type);
    virtual ~CDOListElement();
    virtual ChangedDataObjectList::ChangeType getType() const;
    virtual DataObject* getObject() const;
private:
    DataObject* theObject;
    ChangedDataObjectList::ChangeType    theType; 
};

typedef std::vector< CDOListElement  > CHANGEDDATAOBJECT_VECTOR;


/**
 * ChangedDataObjectListImpl implements ChangedDataObjectList, part of a 
 * change summary.
 * This is the list of objects which have been altered during
 * logging of a change summary.
 */

class ChangedDataObjectListImpl : public ChangedDataObjectList
{

public:
    ChangedDataObjectListImpl(CHANGEDDATAOBJECT_VECTOR p);
    ChangedDataObjectListImpl(const ChangedDataObjectListImpl &pin);
    ChangedDataObjectListImpl();

    virtual ~ChangedDataObjectListImpl();
    virtual DataObjectPtr operator[] (int pos);
    virtual const DataObjectPtr operator[] (int pos) const;
    virtual DataObject* get(unsigned int pos);
    virtual ChangedDataObjectList::ChangeType getType(unsigned int pos);


    virtual int size () const;

    virtual void insert (unsigned int index, DataObject *d, ChangedDataObjectList::ChangeType type);

    virtual void append (DataObject* d, ChangedDataObjectList::ChangeType type);
    
    virtual void clear();
    
    virtual void  remove (unsigned int index);


private: 
    CHANGEDDATAOBJECT_VECTOR plist;
    CHANGEDDATAOBJECT_VECTOR getVec() const;

    void validateIndex(int index) const;
};
};
};
#endif
