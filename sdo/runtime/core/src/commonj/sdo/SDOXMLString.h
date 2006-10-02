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

#include "commonj/sdo/disable_warn.h"

#ifndef _SDOXMLString_H_
#define _SDOXMLString_H_
#include "commonj/sdo/export.h"

#include <libxml/xmlstring.h>
#include <iostream>

namespace commonj
{
    namespace sdo
    {
    /** 
    * SDOXMLString - utility class for string handling.
    * Keeps the string handling local to one place, so we can 
    * replace it with a more NLS friendly version later.
    * Many places within the core should use this too instead
    * of allocated char* buffers. (DataObject etc...)
    */
            
        class SDOXMLString
        {
        public:
            SDO_SPI SDOXMLString();
            SDO_SPI SDOXMLString(const xmlChar* xmlString);
            SDO_SPI SDOXMLString(const char* localString);
            SDO_SPI SDOXMLString(const SDOXMLString& str);    
            SDO_SPI SDOXMLString(const xmlChar* str, int start, int len);    
            SDO_SPI virtual ~SDOXMLString();
            
            SDO_SPI SDOXMLString& operator=(const SDOXMLString& str);
            SDO_SPI SDOXMLString operator+(const SDOXMLString& str) const;
            SDO_SPI SDOXMLString& operator+=(const SDOXMLString& str);
            
    
            SDO_SPI bool operator== (const SDOXMLString& str) const;
            SDO_SPI bool equals(const xmlChar* xmlString) const;
            SDO_SPI bool equals(const char* localString) const;
            SDO_SPI bool equals(const SDOXMLString& str) const;
            SDO_SPI bool equalsIgnoreCase(const xmlChar* xmlString) const;
            SDO_SPI bool equalsIgnoreCase(const char* localString) const;
            SDO_SPI bool equalsIgnoreCase(const SDOXMLString& str) const;

            SDO_SPI bool operator< (const SDOXMLString& str) const;
            
            SDO_SPI operator std::string() const;
            SDO_SPI operator const char*() const {return (const char*) xmlForm;}
            SDO_SPI operator const xmlChar*() const {return xmlForm;}
            
            SDO_SPI friend std::ostream& operator<<(std::ostream& output, const SDOXMLString& str);
            
            SDO_SPI bool isNull() const;

            SDO_SPI SDOXMLString toLower(
                unsigned int start = 0, 
                unsigned int length = 0);


            SDO_SPI int firstIndexOf(const char ch) const;
            SDO_SPI int lastIndexOf(const char ch) const;
            SDO_SPI SDOXMLString substring(int start, int length) const;
            SDO_SPI SDOXMLString substring(int start) const;
        private :
            xmlChar* xmlForm;
            void release();                
        };
    } // End - namespace sdo
} // End - namespace commonj



#endif // _SDOXMLString_H_
