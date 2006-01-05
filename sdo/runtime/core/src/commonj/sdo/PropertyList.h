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

#ifndef _PROPERTYLIST_H_
#define _PROPERTYLIST_H_


#include "commonj/sdo/export.h"

#include <vector>
#include <list>




namespace commonj{
namespace sdo{

    class PropertyImpl;
    class Property;
    typedef std::vector<PropertyImpl*> PROPERTY_VECTOR;
#ifndef PROPERTY_LIST
    typedef std::list<PropertyImpl*> PROPERTY_LIST;
#endif

/**  
 * This class provides for iteration over a collection of Properties
 */

class PropertyList
{
private: 
    PROPERTY_VECTOR plist;
    PROPERTY_VECTOR getVec() const;
public:
    SDO_API PropertyList(PROPERTY_VECTOR p);
    SDO_API PropertyList(PROPERTY_LIST p);
    SDO_API PropertyList(const PropertyList &pin);
    SDO_API PropertyList();
    virtual SDO_API ~PropertyList();

    /**  [] 
     *
     * Operator to allow reference to an element of the list.
     * There are separate versions for const and variable lists
     */

    SDO_API Property& operator[] (int pos);
    SDO_API const Property& operator[] (int pos) const;
    
    /**  size gives the number of elements.
     *
     * size() returns the number of elements in the list.
     */

    SDO_API int size ();
    
    /**  insert adds an element to the list
     *
     * insert appends an item to the list. This is not
     * of any use to a client application, as these lists
     * are always supplied by the library, and are const.
     */

    SDO_API void insert (const Property& p);
};
};
};


#endif
