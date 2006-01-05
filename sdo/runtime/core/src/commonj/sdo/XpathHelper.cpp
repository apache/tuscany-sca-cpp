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

#include "commonj/sdo/XpathHelper.h"

#include <string.h>

namespace commonj{
namespace sdo{

/** 
 * 
 * will provide utilities to help with xpath parsing
 */

/** isIndexed - true if [] or .
 * 
 * Returns true if the path has an index, indicating a many valued
 * property at the end of the path.
 */


const bool XpathHelper::isIndexed(const char* path)
{
    const char * c = strrchr(path,'/');
    if (c == 0)c = path;
    if (strchr(c,'[')) return true;
    if (strchr(c,'.')) return true;
    return false;
}

};
};

