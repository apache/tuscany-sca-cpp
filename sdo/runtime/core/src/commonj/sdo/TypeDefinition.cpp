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

        void TypeDefinition::copy (const TypeDefinition& td)
        {
            if (typedefinition != 0) delete typedefinition;
            typedefinition = new TypeDefinitionImpl(*(td.typedefinition));
        }
 
        TypeDefinition::TypeDefinition(const TypeDefinition& td)
        {
            copy(td);
        }
        
        TypeDefinition& TypeDefinition::operator=(const TypeDefinition& td)
        {
            if (this != &td)
            {
                copy(td);
            }
            return *this;
        }

        TypeDefinitionImpl* TypeDefinition::getTypeDefinition()
        {
            return typedefinition;
        }
        
        void TypeDefinition::setUri(const char * inuri) 
        {
            typedefinition->uri = inuri;
        }
        void TypeDefinition::setUri(const SDOString& inuri) 
        {
            typedefinition->uri = inuri.c_str();
        }

        const char * TypeDefinition::getUri() const
        {
            return (const char *)typedefinition->uri;
        }


        void TypeDefinition::setName(const char * inname)
        {
            typedefinition->name = inname;
        }
        void TypeDefinition::setName(const SDOString& inname)
        {
            typedefinition->name = inname.c_str();
        }

        const char * TypeDefinition::getName() const
        {
            return (const char *)typedefinition->name;
        }


        void TypeDefinition::setLocalName(const char * inname) 
        {
            typedefinition->localname = inname;
        }
        void TypeDefinition::setLocalName(const SDOString& inname) 
        {
            typedefinition->localname = inname.c_str();
        }

        const char * TypeDefinition::getLocalName() const
        {
            return (const char *)typedefinition->localname;
        }


        void TypeDefinition::setAliases(const char * inaliases)
        {
            typedefinition->aliases = inaliases;
        }
        void TypeDefinition::setAliases(const SDOString& inaliases)
        {
            typedefinition->aliases = inaliases.c_str();
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
        void TypeDefinition::setParentType(const SDOString& uri,
                                           const SDOString& name,
                                           bool isrestriction)
        {
            typedefinition->parentTypeUri = uri.c_str();
            typedefinition->parentTypeName = name.c_str();
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
        void TypeDefinition::setIDPropertyName(const SDOString& idpropname)
        {
            typedefinition->IDPropertyName = idpropname.c_str();
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
