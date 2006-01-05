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

#ifndef SDO_EXPORTS
    #define SDO_EXPORTS
#endif

#include "commonj/sdo/export.h"
#include <iostream>
#include "commonj/sdo/Property.h"
#include "commonj/sdo/PropertyImpl.h"
#include "commonj/sdo/Type.h"
#include "commonj/sdo/PropertyList.h"

#include "commonj/sdo/Logger.h"

namespace commonj{
namespace sdo {

class Property;

SDO_API PropertyList::PropertyList(PROPERTY_LIST p) 
{
    PROPERTY_LIST::iterator i;
    for (i = p.begin(); i != p.end(); ++i)
    {
        plist.insert(plist.end(),(PropertyImpl*)(*i));
    }
}

SDO_API PropertyList::PropertyList(PROPERTY_VECTOR p) 
{
    plist = PROPERTY_VECTOR(p);
}

SDO_API PropertyList::PropertyList(const PropertyList &pin)
{
    plist = PROPERTY_VECTOR(pin.getVec());
}

SDO_API PropertyList::PropertyList()
{
}

SDO_API PropertyList::~PropertyList()
{
}

SDO_API Property& PropertyList::operator[] (int pos)
{
    return *(plist[pos]);
}

SDO_API const Property& PropertyList::operator[] (int pos) const
{
    return *(plist[pos]);
}

SDO_API int PropertyList::size () 
{
    return plist.size();
}

SDO_API void PropertyList::insert(const Property& p) 
{
    PropertyImpl* pi = (PropertyImpl*)&p;
    plist.insert(plist.end(),new PropertyImpl(*pi));
}

PROPERTY_VECTOR PropertyList::getVec() const
{
    return plist;
}

};
};

