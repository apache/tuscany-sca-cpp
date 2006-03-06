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

#ifndef _TYPEDEFINITIONS_H_
#define _TYPEDEFINITIONS_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/TypeDefinition.h"
#include "map"

namespace commonj
{
    namespace sdo
    {
        typedef std::map<SDOXMLString, TypeDefinition> XMLDAS_TypeDefs;
        
/**
 * TypeDefinitions holds a list information gathered from parsing the
 * XSD and used for creating Types
 */
        class TypeDefinitions
        {
            
        public:
            TypeDefinitions();
            virtual ~TypeDefinitions();
            static SDOXMLString getTypeQName(const SDOXMLString& typeUri, const SDOXMLString& typeName);

            XMLDAS_TypeDefs types;
            
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_TYPEDEFINITIONS_H_
