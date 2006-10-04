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

#ifndef _TYPEDefinitionsImpl_H_
#define _TYPEDefinitionsImpl_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/TypeDefinitionImpl.h"
#include "map"

namespace commonj
{
    namespace sdo
    {
        typedef std::map<SDOXMLString, TypeDefinitionImpl> XMLDAS_TypeDefs;
        
/**
 * TypeDefinitionsImpl holds a list information gathered from parsing the
 * XSD and used for creating Types
 */
        class TypeDefinitionsImpl
        {
            
        public:
            TypeDefinitionsImpl();
            virtual ~TypeDefinitionsImpl();
            static SDOXMLString getTypeQName(const SDOXMLString& typeUri, const SDOXMLString& typeName);


            XMLDAS_TypeDefs types;
            
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_TYPEDefinitionsImpl_H_
