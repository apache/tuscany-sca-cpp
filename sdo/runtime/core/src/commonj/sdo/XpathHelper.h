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

#ifndef XPATH_HELPER_H
#define XPATH_HELPER_H

#include "commonj/sdo/export.h"
#include "commonj/sdo/SDOString.h"

namespace commonj{
namespace sdo{
    
/**  
 * 
 * XpathHelper provides utilities to help with xpath parsing
 */

class XpathHelper
{
     public:

    virtual ~XpathHelper();

    /**  isIndexed - true if [] or .
     * 
     * Returns true if the path has an index, indicating a many valued
     * property at the end of the path.
     */

    static SDO_API const bool isIndexed(const char * path);
    static SDO_API const bool isIndexed(const SDOString& path);
 
};
};
};


#endif

