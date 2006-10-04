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

#ifndef _SAX2NAMESPACES_H_
#define _SAX2NAMESPACES_H_
#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SDOXMLString.h"
#include "map"


namespace commonj
{
    namespace sdo
    {
        
/**
 * SAX2Namespaces holds a list of namespaces supplied by the SAX2 parser.
 */
        
        class SAX2Namespaces
        {
            
        public:
            
            SAX2Namespaces();
            
            SAX2Namespaces(int nb_namespaces, const xmlChar** namespaces);
            
            virtual ~SAX2Namespaces();

            void add(const SDOXMLString& prefix, const SDOXMLString& uri);

            void merge(const SAX2Namespaces& inspaces);
            
            const SDOXMLString* find(const SDOXMLString& prefix) const;
            const SDOXMLString* findPrefix(const SDOXMLString& uri) const;

            void empty();
            
        private:
            typedef std::map<SDOXMLString, SDOXMLString> NAMESPACE_MAP;
            NAMESPACE_MAP    namespaceMap;
            
            
            
        };
    } // End - namespace sdo
} // End - namespace commonj

#endif //_SAX2NAMESPACES_H_
