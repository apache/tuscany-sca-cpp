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

#ifndef _TYPELIST_H_
#define _TYPELIST_H_
#include "commonj/sdo/export.h"

#include <vector>
#include "commonj/sdo/Type.h"

namespace commonj{
namespace sdo
{
class Type;

/** TypeList - provides access to a list of Types
 *
 * The data factory can return a list of types available. This
 * class wraps that list and provides access to Types by 
 * iteration
 */

class TypeList
{
private: 
    std::vector<const Type*> plist;
    std::vector<const Type*> getVec() const;
public:
    SDO_API TypeList(const TypeList &pin);
    SDO_API TypeList(std::vector<const Type*> p);
    SDO_API TypeList();
    virtual SDO_API ~TypeList();
    SDO_API const Type& operator[] (int pos) const;
    SDO_API int size () const;
    SDO_API void insert (const Type* t);
};
};
};

#endif
