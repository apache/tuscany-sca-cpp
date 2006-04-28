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


#include "commonj/sdo/TypeDefinition.h"
#include "commonj/sdo/TypeDefinitionImpl.h"

namespace commonj
{
    namespace sdo
    {
        TypeDefinition::TypeDefinition()
        {
            typedefinition = new TypeDefinitionImpl();
        }
        
        TypeDefinition::~TypeDefinition()
        {
            if (typedefinition != 0) delete typedefinition;
        }
        
        TypeDefinitionImpl* TypeDefinition::getTypeDefinition()
        {
            return typedefinition;
        }
        
        void TypeDefinition::setUri(const char * inuri) 
        {
            typedefinition->uri = inuri;
        }

        const char * TypeDefinition::getUri() const
        {
            return (const char *)typedefinition->uri;
        }


        void TypeDefinition::setName(const char * inname)
        {
            typedefinition->name = inname;
        }

        const char * TypeDefinition::getName() const
        {
            return (const char *)typedefinition->name;
        }


        void TypeDefinition::setLocalName(const char * inname) 
        {
            typedefinition->localname = inname;
        }

        const char * TypeDefinition::getLocalName() const
        {
            return (const char *)typedefinition->localname;
        }


        void TypeDefinition::setAliases(const char * inaliases)
        {
            typedefinition->aliases = inaliases;
        }
        
        const char * TypeDefinition::getAliases() const
        {
            return (const char*)typedefinition->aliases;
        }

        void TypeDefinition::setParentType(const char* uri, const char* name,
            bool isrestriction)
        {
            typedefinition->parentTypeUri = uri;
            typedefinition->parentTypeName = name;
            typedefinition->isRestriction = isrestriction;
        }

        const char * TypeDefinition::getParentTypeUri() const
        {
            return (const char *)typedefinition->parentTypeUri;
        }

        const char * TypeDefinition::getParentTypeName() const
        {
            return (const char *)typedefinition->parentTypeName;
        }

        void TypeDefinition::setIDPropertyName(const char * idpropname)
        {
            typedefinition->IDPropertyName = idpropname;
        }
        
        const char * TypeDefinition::getIDPropertyName() const
        {
            return (const char *)typedefinition->IDPropertyName;
        }

        void TypeDefinition::addPropertyDefinition(PropertyDefinition& p)
        {
            typedefinition->properties.push_back((PropertyDefinitionImpl&)(*(p.getPropertyDefinition())));
        }

        bool TypeDefinition::getIsDataType() const
        {
            return typedefinition->dataType;
        }

        bool TypeDefinition::getIsOpen() const
        {
            return typedefinition->isOpen;
        }

        bool TypeDefinition::getIsSequenced() const
        {
            return typedefinition->isSequenced;
        }

        bool TypeDefinition::getIsAbstract() const
        {
            return typedefinition->isAbstract;
        }

        bool TypeDefinition::getIsExtendedPrimitive() const
        {
            return typedefinition->isExtendedPrimitive;
        }

        bool TypeDefinition::getIsFromList() const
        {
            return typedefinition->isFromList;
        }

        bool TypeDefinition::getIsQName() const
        {
            return typedefinition->isQName;
        }

        bool TypeDefinition::getIsMany() const
        {
            return typedefinition->isMany;
        }

        void TypeDefinition::setIsDataType(bool value)
        {
            typedefinition->dataType = value;
        }

        void TypeDefinition::setIsOpen(bool value)
        {
            typedefinition->isOpen = value;
        }

        void TypeDefinition::setIsSequenced(bool value)
        {
            typedefinition->isSequenced = value;
        }

        void TypeDefinition::setIsAbstract(bool value)
        {
            typedefinition->isAbstract = value;
        }

        void TypeDefinition::setIsExtendedPrimitive(bool value)
        {
            typedefinition->isExtendedPrimitive = value;
        }

        void TypeDefinition::setIsFromList(bool value)
        {
            typedefinition->isFromList = value;
        }

        void TypeDefinition::setIsQName(bool value)
        {
            typedefinition->isQName = value;
        }

        void TypeDefinition::setIsMany(bool value)
        {
            typedefinition->isMany = value;
        }


        int TypeDefinition::getGroupElementCount() const
         {
             return typedefinition->groupElementCount;
         }

        
    } // End - namespace sdo
} // End - namespace commonj
