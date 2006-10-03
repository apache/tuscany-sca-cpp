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

#include "commonj/sdo/XpathHelper.h"

#include <string.h>
using std::string;

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
   if (path == 0)
   {
      return false;
   }
   else
   {
      return isIndexed(SDOString(path));
   }
}

const bool XpathHelper::isIndexed(const SDOString& path)
{
    size_t lastSlash = path.rfind('/');

    if (lastSlash == string::npos)
    {
       // Search from start of path
       if (path.find('[') != string::npos)
       {
          return true;
       }
       if (path.find('.') != string::npos)
       {
          return true;
       }
    }
    else
    {
       // Search from lastSlash
       if (path.find('[', lastSlash) != string::npos)
       {
          return true;
       }
       if (path.find('.', lastSlash) != string::npos)
       {
          return true;
       }
    }

    return false;
}

};
};

