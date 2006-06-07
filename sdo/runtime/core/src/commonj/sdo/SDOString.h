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

// #include "commonj/sdo/disable_warn.h"

#ifndef _SDOString_H_
#define _SDOString_H_
#include "commonj/sdo/export.h"

#include <iostream>
using std::string;

namespace commonj
{
    namespace sdo
    {
    /** 
    * SDOString - utility class for string handling.
    * Keeps the string handling local to one place, so we can 
    * replace it with a more NLS friendly version later.
    */
            
      class SDOString : public string
      {
      public:
        SDO_API SDOString substr(size_t i = 0, size_t n = std::string::npos) const
        {
            return SDOString(substr(i, n));
        }

        SDO_API SDOString toLower(unsigned int start = 0,
                                  unsigned int length = 0);
        SDO_API SDOString(const string& str) : string(str) {}
        //        SDO_SPI SDOString(const std::string& str, size_t pos);
        SDO_API SDOString(const std::string& str, size_t pos, size_t n = std::string::npos)
            : string(str, pos, n) {}
        SDO_API SDOString(const char* str) : string(str) {}
        SDO_API SDOString() : string() {}
        SDO_API virtual ~SDOString() {}


        // User defined conversion so that the compiler will automatically convert SDOString to const char *

        // This is currently omitted because it can lead to confusion over
        // which method should be called. For example, suppose we have two
        // (overloaded) methods
        // 1. void fn(const char* first, const char* second)
        // 2. void fn(const SDOString& first, const SDOString& second)
        // and we make a call passing a C style string as the first parameter and an SDOString
        // as the second parameter. Currently, the first parameter is
        // converted to an SDOString and the second method is called. If the
        // conversion below is uncommented, then the compiler also has the
        // option to convert the second parameter to a C style string and then
        // call the first method - and it is ambiguous which of these to choose,
        // leading to a compile time error.

        // operator const char*() const { return c_str(); }

      };
    } // End - namespace sdo
} // End - namespace commonj

#endif // _SDOString_H_
