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
#include "libxml/globals.h"

#include <ctype.h>
#include <string>

namespace commonj
{
  namespace sdo
  {

    SDOString::SDOString()
      : characters()
    {
    }

    SDOString::SDOString(const char* localString)
    {
      characters = localString;
    }

    SDOString::SDOString(const std::string& str)
      : characters(str)
    {
    }

    SDOString::SDOString(const SDOString& str)
    {
      characters = str.characters;
    }
        
    SDOString& SDOString::operator=(const SDOString& str)
    {
      if (this != &str)
      {
        characters = str.characters;
      }
      return *this;
    }

    SDOString SDOString::operator+(const SDOString& str) const
    {
      std::string temp1 = characters;
      temp1 += str.characters;
      SDOString returnString = temp1.c_str();

      return returnString;
    }

    SDOString& SDOString::operator+=(const SDOString& str)
    {
      characters += str.characters;
      return *this;
    }
        
    SDOString::~SDOString()
    {
    }

    bool SDOString::operator< (const SDOString& str) const
    {
      if (characters.compare(str.characters) < 0)
        return true;
      else
        return false;
    }

    bool SDOString::operator==(const SDOString& str) const
    {
      if (characters.compare(str.characters) == 0)
        return true;
      else
        return false;
    }

    bool SDOString::equals(const SDOString& str) const
    {
      //      return (characters.equals(str.characters));
      return (characters == str.characters);
    }
        
    bool SDOString::equals(const char* localString) const
    {
      return equals(SDOString(localString));
    }

//         bool SDOString::equalsIgnoreCase(const SDOString& str) const
//         {
//             return equalsIgnoreCase(str.xmlForm);
//         }

//         bool SDOString::equalsIgnoreCase(const char* localString) const
//         {
//             return equalsIgnoreCase(SDOString(localString));
//         }

    std::ostream& operator<<(std::ostream& output, const SDOString& str)
    {
      if (!str.characters.empty())
        output << str.characters;
      return output;
    }

    bool SDOString::isNull() const
    {
      return (characters.empty());
    }

    SDOString SDOString::toLower(unsigned int start, unsigned int length)
    {
      std::string result_string;
      result_string.reserve(characters.length());

      for (unsigned int i = start; (i < characters.length() && i < length); i++)
      {
        result_string[i] = tolower(characters[i]);
      }

      return SDOString(result_string);
    }

    int SDOString::firstIndexOf(const char ch) const
    {
      return int(characters.find_first_of(ch));
    }

    int SDOString::lastIndexOf(const char ch) const
    {
      return int(characters.find_last_of(ch));
    }

    SDOString SDOString::substring(int start, int length) const
    {
      if (characters.length() > 0)
        {
          return SDOString(characters.substr(start, length));
        }
      else
        return SDOString();
    }

    SDOString SDOString::substring(int start) const
    {
      return substring(start, (characters.length() - start));
    }

	SDO_SPI const char* SDOString::c_str() const
	{
	  return characters.c_str();
	}

  } // End - namespace sdo
} // End - namespace commonj
