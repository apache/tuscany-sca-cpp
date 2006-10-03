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

#ifndef _PROPERTYDefinition_H_
#define _PROPERTYDefinition_H_

#include "export.h"

#include <vector>
#include "commonj/sdo/SDOString.h"



namespace commonj
{
    namespace sdo
    {
     
        class PropertyDefinitionImpl;
        
/**
 * PropertyDefinitionImpl holds a property as defined by an XSD.
 * The XSD holds more information than does the SDO Property.
 * This class holds the extra information found as the XSD is
 * parsed.
 */
        class SDO_API PropertyDefinition
        {
            
        public:


            /*SDO_API*/ PropertyDefinition();
            PropertyDefinition(const PropertyDefinition& pd);
            PropertyDefinition& operator=(const PropertyDefinition& pd);

            /*SDO_API*/ virtual ~PropertyDefinition();

            PropertyDefinitionImpl* getPropertyDefinition();


                       
            // where this property is to be substituted for another
            /*SDO_API*/ void setSubstituteName(const char * name);
            /*SDO_API*/ void setSubstituteName(const SDOString& name);
            /*SDO_API*/ const char* getSubstituteName() const;

            /*SDO_API*/ void setSubstituteUri(const char* name);
            /*SDO_API*/ void setSubstituteUri(const SDOString& name);
            /*SDO_API*/ const char* getSubstituteUri() const;

            // where there are substitute names for this property.
            /*SDO_API*/ void addSubstitute(const char* name,
                const char* localname);
            /*SDO_API*/ void addSubstitute(const SDOString& name,
                const SDOString& localname);
            /*SDO_API*/ int getSubstituteCount() const;
            /*SDO_API*/ const char* getSubstituteNames(int index) const;
            /*SDO_API*/ const char* getSubstituteLocalNames(int index) const;

            
            /*SDO_API*/ void setAliases(const char* aliases);
            /*SDO_API*/ void setAliases(const SDOString& aliases);
            /*SDO_API*/ const char* getAliases() const;

            /*SDO_API*/ void setName(const char* name);
            /*SDO_API*/ void setName(const SDOString& name);
            /*SDO_API*/ const char* getName() const;
            
            /*SDO_API*/ void setLocalName(const char* name);
            /*SDO_API*/ void setLocalName(const SDOString& name);
            /*SDO_API*/ const char* getLocalName() const;

            /*SDO_API*/ void setType(const char* uri, const char* name);
            /*SDO_API*/ void setType(const SDOString& uri, const SDOString& name);
            /*SDO_API*/ const char * getTypeName() const;
            /*SDO_API*/ const char * getTypeUri() const;
            /*SDO_API*/ const char * getTypeFullName() const;
            /*SDO_API*/ const char * getTypeFullLocalName() const ;
            
            /*SDO_API*/ void setTypeFullLocalName(const char* name);
            /*SDO_API*/ void setTypeFullLocalName(const SDOString& name);

            /*SDO_API*/ void setDefaultValue(const char* value);
            /*SDO_API*/ void setDefaultValue(const SDOString& value);
            /*SDO_API*/ const char* getDefaultValue() const;

            /*SDO_API*/ bool getIsMany() const;
            /*SDO_API*/ bool getIsContainment() const;
            /*SDO_API*/ bool getIsReadOnly() const;
            /*SDO_API*/ bool getIsID() const;
            /*SDO_API*/ bool getIsIDREF() const;
            /*SDO_API*/ bool getIsReference() const;
            /*SDO_API*/ bool getIsElement() const;
            /*SDO_API*/ bool getIsQName() const;
            /*SDO_API*/ bool getIsSubstitute() const;

            /*SDO_API*/ void setIsMany(bool value);
            /*SDO_API*/ void setIsContainment(bool value);
            /*SDO_API*/ void setIsReadOnly(bool value);
            /*SDO_API*/ void setIsID(bool value);
            /*SDO_API*/ void setIsIDREF(bool value);
            /*SDO_API*/ void setIsReference(bool value);
            /*SDO_API*/ void setIsElement(bool value);
            /*SDO_API*/ void setIsQName(bool value);
            /*SDO_API*/ void setIsSubstitute(bool value);


        private:
            PropertyDefinitionImpl* propertydefinition;
            void copy(const PropertyDefinition& pd);
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_PROPERTYDefinitionImpl_H_
