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

#ifndef _CHANGEDDATAOBJECTLIST_H_
#define _CHANGEDDATAOBJECTLIST_H_


#include "commonj/sdo/RefCountingPointer.h"

#include <vector>

/**
 * namespace commonj is the root namespace for SDO
 */

namespace commonj{

/**
 * sdo is the namespace holding the SDO core
 */

namespace sdo{

class DataObject;

/** 
 * 
 * A list for holding a record of changes which have happened 
 * during logging in a change summary.
 */

class ChangedDataObjectList
{

public:

/** \enum ChangeType
 * 
 * An enum for create, change or delete types in a change record
 */
    enum ChangeType
    {
        Undefined,
        Create,
        Change,
        Delete
    };

/**  [] is the index operator.
 * 
 * Gets the nth element of a list
 */
    virtual SDO_API DataObjectPtr operator[] (int pos) = 0;
    virtual SDO_API const DataObjectPtr operator[] (int pos) const = 0;

/**  size() returns the number of list elements.
 * 
 * Returns the number of list elements
 */

    virtual SDO_API int size () const = 0;

/**  getType() returns the type of a list element.
 * 
 * The type of change is one of the enum ChangeType, being a 
 * creation, a deletion or an update.
 */

    virtual SDO_API ChangeType getType(unsigned int index) = 0;

};
};
};
#endif

