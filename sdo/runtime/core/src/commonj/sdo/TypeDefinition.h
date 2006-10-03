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

#ifndef _TYPEDefinition_H_
#define _TYPEDefinition_H_

#include "export.h"


#include "commonj/sdo/PropertyDefinition.h"
#include "commonj/sdo/SDOString.h"
#include "list"


namespace commonj
{
    namespace sdo
    {
        
        class TypeDefinitionImpl;

       
/**
 * TypeDefinitionImpl holds information gathered from parsing the
 * XSD and used for creating Types
 */
        class SDO_API TypeDefinition
        {
            
        public:


            /*SDO_API*/ TypeDefinition();
            TypeDefinition(const TypeDefinition& td);
            TypeDefinition& operator=(const TypeDefinition& td);

            /*SDO_API*/ virtual ~TypeDefinition();


            TypeDefinitionImpl* getTypeDefinition();       
        
            /*SDO_API*/ void setUri(const char * inuri) ;
            /*SDO_API*/ void setUri(const SDOString& inuri) ;

            /*SDO_API*/ const char * getUri() const;

            /*SDO_API*/ void setName(const char * inname) ;
            /*SDO_API*/ void setName(const SDOString& inname) ;
            /*SDO_API*/ const char * getName() const;

            /*SDO_API*/ void setLocalName(const char * inname) ;
            /*SDO_API*/ void setLocalName(const SDOString& inname) ;
            /*SDO_API*/ const char * getLocalName() const;

            /*SDO_API*/ void setAliases(const char * inaliases) ;
            /*SDO_API*/ void setAliases(const SDOString& inaliases) ;
            /*SDO_API*/ const char * getAliases() const;


            /*SDO_API*/ void setParentType(const char* uri, const char* name,
                bool isrestriction);
            /*SDO_API*/ void setParentType(const SDOString& uri,
                                           const SDOString& name,
                                           bool isrestriction);
            /*SDO_API*/ const char * getParentTypeUri() const;
            /*SDO_API*/ const char * getParentTypeName() const;

            /*SDO_API*/ void setIDPropertyName(const char * idpropname) ;
            /*SDO_API*/ void setIDPropertyName(const SDOString& idpropname) ;
            /*SDO_API*/ const char * getIDPropertyName() const;


            /*SDO_API*/ void addPropertyDefinition(PropertyDefinition& p);

            /*SDO_API*/ bool getIsDataType() const;
            /*SDO_API*/ bool getIsOpen() const;
            /*SDO_API*/ bool getIsSequenced() const;
            /*SDO_API*/ bool getIsAbstract() const;
            /*SDO_API*/ bool getIsExtendedPrimitive() const;
            /*SDO_API*/ bool getIsFromList() const;
            /*SDO_API*/ bool getIsQName() const;
            /*SDO_API*/ bool getIsMany() const;
            
            /*SDO_API*/ void setIsDataType(bool value);
            /*SDO_API*/ void setIsOpen(bool value);
            /*SDO_API*/ void setIsSequenced(bool value);
            /*SDO_API*/ void setIsAbstract(bool value);
            /*SDO_API*/ void setIsExtendedPrimitive(bool value);
            /*SDO_API*/ void setIsFromList(bool value);
            /*SDO_API*/ void setIsQName(bool value);
            /*SDO_API*/ void setIsMany(bool value);

            /*SDO_API*/ int getGroupElementCount() const;

        private :
            TypeDefinitionImpl *typedefinition;
            void copy(const TypeDefinition& td);
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_TYPEDefinition_H_
