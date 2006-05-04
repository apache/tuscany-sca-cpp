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

#ifndef _TYPEDefinitions_H_
#define _TYPEDefinitions_H_


#include "commonj/sdo/TypeDefinition.h"
#include "map"

namespace commonj
{
    namespace sdo
    {
        
    class TypeDefinitionsImpl;
    
/**
 * TypeDefinitionsImpl holds a list information gathered from parsing the
 * XSD and used for creating Types
 */
        class SDO_API TypeDefinitions
        {
            
        public:
            /*SDO_API*/ TypeDefinitions();
            TypeDefinitions(const TypeDefinitions& tds);
            TypeDefinitions& operator=(const TypeDefinitions& tds);

            /*SDO_API*/ virtual ~TypeDefinitions();

            /*SDO_API*/ void addTypeDefinition(TypeDefinition& t);

            TypeDefinitionsImpl& getTypeDefinitions();
 
        private:
            TypeDefinitionsImpl* typedefinitions;
            void copy(const TypeDefinitions& tds);

            
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_TYPEDefinitions_H_
