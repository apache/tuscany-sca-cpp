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

// #include "commonj/sdo/disable_warn.h"

#ifndef _SDOString_H_
#define _SDOString_H_
#include "commonj/sdo/export.h"

// #include <libxml/xmlstring.h>
#include <iostream>

namespace commonj
{
    namespace sdo
    {
    /** 
    * SDOString - utility class for string handling.
    * Keeps the string handling local to one place, so we can 
    * replace it with a more NLS friendly version later.
    */
            
      class SDOString
        {
        public:
            SDO_SPI SDOString();
            SDO_SPI SDOString(const char* localString);
            SDO_SPI SDOString(const std::string& str);    
            SDO_SPI SDOString(const SDOString& str);    
            SDO_SPI virtual ~SDOString();
            
            SDO_SPI SDOString& operator=(const SDOString& str);
            SDO_SPI SDOString operator+(const SDOString& str) const;
            SDO_SPI SDOString& operator+=(const SDOString& str);
            
    
            SDO_SPI bool operator== (const SDOString& str) const;
            SDO_SPI bool equals(const char* localString) const;
            SDO_SPI bool equals(const SDOString& str) const;
            SDO_SPI bool equalsIgnoreCase(const char* localString) const;
            SDO_SPI bool equalsIgnoreCase(const SDOString& str) const;

            SDO_SPI bool operator< (const SDOString& str) const;
            
          //            SDO_SPI operator const char*() const {return (const char*) xmlForm;}
            
            SDO_SPI friend std::ostream& operator<<(std::ostream& output, const SDOString& str);
            
            SDO_SPI bool isNull() const;

            SDO_SPI SDOString toLower(
                unsigned int start = 0, 
                unsigned int length = 0);


            SDO_SPI int firstIndexOf(const char ch) const;
            SDO_SPI int lastIndexOf(const char ch) const;
            SDO_SPI SDOString substring(int start, int length) const;
            SDO_SPI SDOString substring(int start) const;

			SDO_SPI const char* c_str() const;
        private :
          std::string characters;
            void release();                
        };
    } // End - namespace sdo
} // End - namespace commonj

#endif // _SDOString_H_
