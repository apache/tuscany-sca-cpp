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
#include "commonj/sdo/Type.h"
#include "commonj/sdo/TypeList.h"
#include "commonj/sdo/Logger.h"

using namespace std;

namespace commonj{
namespace sdo {


/** TypeList provides access to a list of Types.
 *
 * The data factory can return a list of types available. This
 * class wraps that list and provides access to Types by 
 * iteration
 */

SDO_API TypeList::TypeList(std::vector<const Type*> p) : plist (p)
{
}

SDO_API TypeList::TypeList(const TypeList &pin)
{
    plist = std::vector<const Type*>(pin.getVec());
}

SDO_API TypeList::TypeList()
{
}

SDO_API TypeList::~TypeList()
{
}


SDO_API const Type& TypeList::operator[] (int pos) const
{
    return *plist[pos];
}

SDO_API int TypeList::size () const
{
    return plist.size();
}

std::vector<const Type*> TypeList::getVec() const
{
    return plist;
}

SDO_API void TypeList::insert (const Type* t) 
{
    plist.insert(plist.end(),t);
}

};
};

