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

#ifndef _SAX2ATTRIBUTES_H_
#define _SAX2ATTRIBUTES_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SAX2Attribute.h"

#include "vector"


namespace commonj
{
    namespace sdo
    {
        
/**
 * SAX2Attributes holds a list of attributes supplied by the SAX2 parser.
 */        
        class SAX2Attributes
        {
            
        public:

            SAX2Attributes();
            
            SAX2Attributes(
                int nb_attributes,
                int nb_defaulted,
                const xmlChar **attributes);
            
            virtual ~SAX2Attributes();
            
            const SAX2Attribute& operator[] (int pos) const;
            int size() const;
            
            const SDOXMLString& getValue(
                const SDOXMLString& attributeUri,
                const SDOXMLString& attributeName) const; 

            const SDOXMLString& getValue(
                const SDOXMLString& attributeName) const; 

            const SAX2Attribute* getAttribute(
                const SDOXMLString& attributeName) const; 

            void addAttribute(const SAX2Attribute& attr);
        private:
            typedef std::vector<SAX2Attribute> ATTRIBUTE_LIST;
            ATTRIBUTE_LIST    attributes;

            static const SDOXMLString nullValue;
            
            
            
        };
    } // End - namespace sdo
} // End - namespace commonj

#endif //_SAX2ATTRIBUTES_H_
