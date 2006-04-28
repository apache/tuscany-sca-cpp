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

/* $Rev$ $Date$ */

#ifndef _TYPEDefinitionImpl_H_
#define _TYPEDefinitionImpl_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/PropertyDefinitionImpl.h"
#include "list"


namespace commonj
{
    namespace sdo
    {
        
        typedef std::list<PropertyDefinitionImpl> XmlDasPropertyDefs;
        
/**
 * TypeDefinitionImpl holds information gathered from parsing the
 * XSD and used for creating Types
 */
        class TypeDefinitionImpl
        {
            
        public:
            TypeDefinitionImpl();
            virtual ~TypeDefinitionImpl();
        
    
            SDOXMLString uri;
            SDOXMLString name;
            SDOXMLString localname;

            SDOXMLString aliases;

            SDOXMLString parentTypeUri;
            SDOXMLString parentTypeName;
            bool isRestriction;

            SDOXMLString IDPropertyName;

            bool dataType;
            XmlDasPropertyDefs properties;

            bool isOpen;
            bool isSequenced;
            bool isAbstract;

            bool isExtendedPrimitive;
            
            bool isFromList;


            bool isMany;
            int groupElementCount;

            bool isQName;
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_TYPEDefinitionImpl_H_
