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

/* $Rev$ $Date: 2006/04/13 08:35:04 $ */

#include "commonj/sdo/TypeDefinition.h"
namespace commonj
{
    namespace sdo
    {
        TypeDefinition::TypeDefinition()
            : dataType(false),
              isOpen(false),
              isSequenced(false),
              isMany(false),
              isQName(false),
              isAbstract(false),
              groupElementCount(0),
              isExtendedPrimitive(false),
              isFromList(false)
        {
        }
        
        TypeDefinition::~TypeDefinition()
        {
        }
        
    } // End - namespace sdo
} // End - namespace commonj
