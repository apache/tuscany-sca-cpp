/*
 *
 *  *** GMW *** Copyright 2005 The Apache Software Foundation or its licensors, as applicable.
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

/* *** GMW *** $Rev$ $Date$ */

// This class probably needs to inherit from RefCountingObject to ensure that objects of this type get cleaned up correctly.

#include "commonj/sdo/SDOString.h"
#include <iostream>
// #include "libxml/globals.h"

#include <ctype.h>
#include <string>

// C6 duff compilation - see http://www.codecomments.com/message294418.html
using std::string;
#define std_string string

namespace commonj
{
  namespace sdo
  {
    inline SDOString::SDOString(const std::string& str)
      : std_string(str)
    {
    }

    inline SDOString::~SDOString()
    {

    }

    inline SDOString SDOString::substr(size_t i, size_t n) const
    {
      return SDOString(substr(i, n));
    }

//     inline SDOString::SDOString(const std::string& str, size_t pos)
//       : std_string(str, pos)
//     {
//     }

    inline SDOString::SDOString(const std::string& str, size_t pos, size_t n)
      : std_string(str, pos, n)
    {
    }

    inline SDOString::SDOString(const char* str)
      : std_string(str)
    {
    }

    inline SDOString::SDOString()
      : std_string()
    {
    }

    SDOString SDOString::toLower(unsigned int start, unsigned int length)
    {
      std::string result_string;
      result_string.reserve(size());

      for (unsigned int i = start; (i < size() && i < length); i++)
        {
          result_string[i] = tolower((*this)[i]);
        }

      return SDOString(result_string);
    }
  } // End - namespace sdo
} // End - namespace commonj
