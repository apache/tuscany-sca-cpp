/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 *   
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* $Rev$ $Date$ */

#include "commonj/sdo/SDOXMLString.h"
#include <iostream>
#include "libxml/globals.h"

#include <ctype.h>

namespace commonj
{
    namespace sdo
    {
        
        SDOXMLString::SDOXMLString()
            : xmlForm(0)
        {
        }
        
        SDOXMLString::SDOXMLString(const xmlChar* xmlString)
        {
            xmlForm = xmlStrdup(xmlString);
        }
        
        SDOXMLString::SDOXMLString(const char* localString)
        {
            xmlForm = xmlCharStrdup(localString);
        }
        
        SDOXMLString::SDOXMLString(const SDOXMLString& str)
        {
            xmlForm = xmlStrdup(str.xmlForm);
        }
        
        SDOXMLString::SDOXMLString(const xmlChar* str, int start, int len)
        {
            xmlForm = xmlStrsub(str, start, len);
        }
        
        SDOXMLString& SDOXMLString::operator=(const SDOXMLString& str)
        {
            if (this != &str)
            {
                release();
                xmlForm = xmlStrdup(str.xmlForm);
            }
            return *this;
        }
        
        SDOXMLString::operator std::string() const
        {
            if (xmlForm)
            {
                return (const char *)xmlForm;
            }
            return "";
        }

        SDOXMLString SDOXMLString::operator+(const SDOXMLString& str) const
        {
            xmlChar* newString = xmlStrncatNew(xmlForm, str.xmlForm, xmlStrlen(str.xmlForm));
            SDOXMLString retString(newString);
            if (newString)
                xmlFree(newString);
            return retString;
        }

        SDOXMLString& SDOXMLString::operator+=(const SDOXMLString& str)
        {
            xmlForm = xmlStrncat(xmlForm, str.xmlForm, xmlStrlen(str.xmlForm));
            return *this;
        }
        
        SDOXMLString::~SDOXMLString()
        {
            release();
        }
        
        void SDOXMLString::release()
        {
            if (xmlForm)
                xmlFree((void*)xmlForm);
        }
        
        bool SDOXMLString::operator< (const SDOXMLString& str) const
        {
            if (xmlStrcmp(xmlForm, str.xmlForm) <0)
                return true;
            else
                return false;
        }
        
        bool SDOXMLString::operator==(const SDOXMLString& str) const
        {
            if (xmlStrEqual(xmlForm, str.xmlForm))
                return true;
            else
                return false;
        }

        bool SDOXMLString::equals(const SDOXMLString& str) const
        {
            return equals(str.xmlForm);
        }
        
        bool SDOXMLString::equals(const char* localString) const
        {
            return equals(SDOXMLString(localString));
        }
        

        bool SDOXMLString::equals(const xmlChar* xmlString) const
        {
            if (xmlStrEqual(xmlForm, xmlString))
                return true;
            else
                return false;
        }
        
        bool SDOXMLString::equalsIgnoreCase(const SDOXMLString& str) const
        {
            return equalsIgnoreCase(str.xmlForm);
        }
        
        bool SDOXMLString::equalsIgnoreCase(const char* localString) const
        {
            return equalsIgnoreCase(SDOXMLString(localString));
        }
        

        bool SDOXMLString::equalsIgnoreCase(const xmlChar* xmlString) const
        {
            if (xmlStrcasecmp(xmlForm, xmlString) == 0)
                return true;
            else
                return false;
        }
        
        
        std::ostream& operator<<(std::ostream& output, const SDOXMLString& str)
        {
            if (str.xmlForm)
                output << str.xmlForm;
            return output;
        }
        
        bool SDOXMLString::isNull() const
        {
            if (xmlForm)
                return false;
            else
                return true;
        }

        SDOXMLString SDOXMLString::toLower(unsigned int start, unsigned int length)
        {
            char* newString = new char[strlen(*this)+1];
            strcpy(newString, *this);
            for (int i = start; (i<=(strlen(newString)) && i < length); i++)
            {
                newString[i] = tolower(newString[i]);
            }
            SDOXMLString retString(newString);
            delete newString;
            return retString;
        }

        int SDOXMLString::firstIndexOf(const char ch) const
        {
            const xmlChar* loc = xmlStrchr(xmlForm, ch);
            if (loc == 0)
            {
                return -1;
            }
            
            return int(loc-xmlForm);
        }
        
        int SDOXMLString::lastIndexOf(const char ch) const
        {
            const xmlChar* index = 0;
            const xmlChar* loc = xmlStrchr(xmlForm, ch);
            while (loc !=0)
            {
                index = loc;
                loc = xmlStrchr(loc+1, ch);
            }
            
            if (index == 0)
                return -1;

            return int(index-xmlForm);
        }
        
        SDOXMLString SDOXMLString::substring(int start, int length) const
        {
            if (length >0)
            {
                return SDOXMLString(xmlForm, start, length);
            }
            else
                return SDOXMLString();
        }
        
        SDOXMLString SDOXMLString::substring(int start) const
        {
            
            return SDOXMLString(xmlForm, start, xmlStrlen(xmlForm) - start);
        }
        
        
    } // End - namespace sdo
} // End - namespace commonj



