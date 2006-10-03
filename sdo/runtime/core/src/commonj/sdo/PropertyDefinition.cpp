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

#include "commonj/sdo/PropertyDefinition.h"
#include "commonj/sdo/PropertyDefinitionImpl.h"

namespace commonj
{
    namespace sdo
    {

        PropertyDefinition::PropertyDefinition()
        {
            propertydefinition = new PropertyDefinitionImpl();
        }
                        
        PropertyDefinition::~PropertyDefinition()
        {
            if (propertydefinition) delete propertydefinition;
        }
 
        void PropertyDefinition::copy (const PropertyDefinition& pd)
        {
            if (propertydefinition != 0) delete propertydefinition;
            propertydefinition = new PropertyDefinitionImpl(*(pd.propertydefinition));
        }
 
        PropertyDefinition::PropertyDefinition(const PropertyDefinition& pd)
        {
            copy(pd);
        }
        
        PropertyDefinition& PropertyDefinition::operator=(const PropertyDefinition& pd)
        {
            if (this != &pd)
            {
                copy(pd);
            }
            return *this;
        }

        PropertyDefinitionImpl* PropertyDefinition::getPropertyDefinition()
        {
            return propertydefinition;
        }
                 
        // where this property is to be substituted for another
         void PropertyDefinition::setSubstituteName(const char * name)
         {
             propertydefinition->substituteName = name;
         }
         void PropertyDefinition::setSubstituteName(const SDOString& name)
         {
             propertydefinition->substituteName = name.c_str();
         }

         const char* PropertyDefinition::getSubstituteName() const
         {
             return (const char *)propertydefinition->substituteName;
         }


         void PropertyDefinition::setSubstituteUri(const char* name)
         {
             propertydefinition->substituteUri = name;
         }
         void PropertyDefinition::setSubstituteUri(const SDOString& name)
         {
             propertydefinition->substituteUri = name.c_str();
         }

         const char* PropertyDefinition::getSubstituteUri() const
         {
             return (const char*)propertydefinition->substituteUri;
         }

        // where there are substitute names for this property.
         void PropertyDefinition::addSubstitute(const SDOString& name,
            const SDOString& localname)
         {
             propertydefinition->substituteNames.push_back(name.c_str());
             propertydefinition->substituteLocalNames.push_back(localname.c_str());
         }

         int PropertyDefinition::getSubstituteCount() const
         {
             return propertydefinition->substituteNames.size();
         }

         const char* PropertyDefinition::getSubstituteNames(int index) const
         {
             if (index >= 0 && index < propertydefinition->substituteNames.size())
             {
                return propertydefinition->substituteNames[index];;
             }
             return 0;
         }

         const char* PropertyDefinition::getSubstituteLocalNames(int index) const
         {
             if (index >= 0 && index < propertydefinition->substituteLocalNames.size())
             {
                return propertydefinition->substituteLocalNames[index];;
             }
             return 0;
         }

        
         void PropertyDefinition::setAliases(const char* aliases)
         {
             propertydefinition->aliases = aliases;
         }
         void PropertyDefinition::setAliases(const SDOString& aliases)
         {
             propertydefinition->aliases = aliases.c_str();
         }

         const char* PropertyDefinition::getAliases() const
         {
             return (const char *)propertydefinition->aliases;
         }

         void PropertyDefinition::setName(const char* name)
         {
             propertydefinition->name = name;
         }
         void PropertyDefinition::setName(const SDOString& name)
         {
             propertydefinition->name = name.c_str();
         }

         const char* PropertyDefinition::getName() const
         {
             return (const char *)propertydefinition->name;
         }
        
         void PropertyDefinition::setLocalName(const char* name)
         {
             propertydefinition->localname = name;
         }
         void PropertyDefinition::setLocalName(const SDOString& name)
         {
             propertydefinition->localname = name.c_str();
         }

         const char* PropertyDefinition::getLocalName() const
         {
             return (const char*)propertydefinition->localname;
         }
        
         void PropertyDefinition::setType(const char* uri, const char* name)
         {
             propertydefinition->typeUri = uri;
             propertydefinition->typeName = name;
             propertydefinition->fullTypeName = uri;
             propertydefinition->fullTypeName += "#";
             propertydefinition->fullTypeName += name;
         }
         void PropertyDefinition::setType(const SDOString& uri, const SDOString& name)
         {
             propertydefinition->typeUri = uri.c_str();
             propertydefinition->typeName = name.c_str();
             propertydefinition->fullTypeName = uri.c_str();
             propertydefinition->fullTypeName += "#";
             propertydefinition->fullTypeName += name.c_str();
         }

         const char * PropertyDefinition::getTypeName() const
         {
             return propertydefinition->typeName;
         }

         const char * PropertyDefinition::getTypeUri() const
         {
             return propertydefinition->typeUri;
         }

         const char * PropertyDefinition::getTypeFullName() const
         {
             return propertydefinition->fullTypeName;
         }

         const char * PropertyDefinition::getTypeFullLocalName() const 
         {
             return propertydefinition->fullLocalTypeName;
         }

         void PropertyDefinition::setTypeFullLocalName(const char* name)
         {
             propertydefinition->fullLocalTypeName = name;
         }
         void PropertyDefinition::setTypeFullLocalName(const SDOString& name)
         {
             propertydefinition->fullLocalTypeName = name.c_str();
         }


         void PropertyDefinition::setDefaultValue(const char* value)
         {
             propertydefinition->defaultValue = value;
         }
         void PropertyDefinition::setDefaultValue(const SDOString& value)
         {
             propertydefinition->defaultValue = value.c_str();
         }

         const char* PropertyDefinition::getDefaultValue() const
         {
             return propertydefinition->defaultValue;
         }

         bool PropertyDefinition::getIsMany() const
         {
             return propertydefinition->isMany;
         }

         bool PropertyDefinition::getIsContainment() const
         {
             return propertydefinition->isContainment;
         }

         bool PropertyDefinition::getIsReadOnly() const
         {
             return propertydefinition->isReadOnly;
         }

         bool PropertyDefinition::getIsID() const
         {
             return propertydefinition->isID;
         }

         bool PropertyDefinition::getIsIDREF() const
         {
             return propertydefinition->isIDREF;
         }

         bool PropertyDefinition::getIsReference() const
         {
             return propertydefinition->isReference;
         }

         bool PropertyDefinition::getIsElement() const
         {
             return propertydefinition->isElement;
         }

         bool PropertyDefinition::getIsQName() const
         {
             return propertydefinition->isQName;
         }

         bool PropertyDefinition::getIsSubstitute() const
         {
             return propertydefinition->isSubstitute;
         }


         void PropertyDefinition::setIsMany(bool value)
         {
             propertydefinition->isMany = value;
         }

         void PropertyDefinition::setIsContainment(bool value)
         {
             propertydefinition->isContainment = value;
         }

         void PropertyDefinition::setIsReadOnly(bool value)
         {
             propertydefinition->isReadOnly = value;
         }

         void PropertyDefinition::setIsID(bool value)
         {
             propertydefinition->isID = value;
         }

         void PropertyDefinition::setIsIDREF(bool value)
         {
             propertydefinition->isIDREF = value;
         }

         void PropertyDefinition::setIsReference(bool value)
         {
             propertydefinition->isReference = value;
         }

         void PropertyDefinition::setIsElement(bool value)
         {
             propertydefinition->isElement = value;
         }

         void PropertyDefinition::setIsQName(bool value)
         {
             propertydefinition->isQName = value;
         }

         void PropertyDefinition::setIsSubstitute(bool value)
         {
             propertydefinition->isSubstitute = value;
         }


    } // End - namespace sdo
} // End - namespace commonj
