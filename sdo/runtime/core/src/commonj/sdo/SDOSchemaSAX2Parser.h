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

#ifndef _SDOSCHEMASAX2PARSER_H_
#define _SDOSCHEMASAX2PARSER_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SAX2Parser.h"
#include "commonj/sdo/SchemaInfo.h"
#include "stack"
#include "commonj/sdo/TypeDefinitions.h"
#include "commonj/sdo/XMLQName.h"
#include "commonj/sdo/ParserErrorSetter.h"

namespace commonj
{
    namespace sdo
    {
        
/**
 * SDOSAX2Parser implements SAX2Parser.
 * This class gets called back by the libxml library, and
 * deals with the creation of metadata objects from XSD.
 */
        
        class SDOSchemaSAX2Parser : public SAX2Parser
        {
            
        public:
            
            SDOSchemaSAX2Parser(SchemaInfo& schemaInfo,
                ParserErrorSetter* insetter);
            
            virtual ~SDOSchemaSAX2Parser();

        
            virtual void startElementNs(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            
            
            virtual void endElementNs(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI);
            
            const SDOXMLString& getTargetNamespaceURI() const {return schemaInfo.getTargetNamespaceURI();}
            
            TypeDefinitions& getTypeDefinitions() {return typeDefinitions;}
            
            friend std::istream& operator>>(std::istream& input, SDOSchemaSAX2Parser& parser);
            friend std::istringstream& operator>>(std::istringstream& input, SDOSchemaSAX2Parser& parser);


        private:

            const char* currentFile;
            bool bInSchema; // only parse when within a schema

        
            virtual void startInclude(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            

            virtual void startElement(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            
            
            virtual void startAttribute(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            
            
            virtual void startComplexType(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            
            
            virtual void startSimpleType(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            
            
            virtual void startRestriction(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            
            
            virtual void startExtension(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            
            
            virtual void startGroup(
                const SDOXMLString& localname,
                const SDOXMLString& prefix,
                const SDOXMLString& URI,
                const SAX2Namespaces& namespaces,
                const SAX2Attributes& attributes);            
            
            XMLQName resolveTypeName(
                const SDOXMLString& fullTypeName,
                const SAX2Namespaces& namespaces,
                SDOXMLString& uri,
                SDOXMLString& name);
            
            void setName(
                const SAX2Attributes& attributes,
                SDOXMLString& sdoname,
                SDOXMLString& localname
                );
            
            void setType(
                PropertyDefinition& property,
                const SAX2Attributes& attributes,
                const SAX2Namespaces& namespaces);
            
            void setTypeName(
                TypeDefinition& type,
                const SAX2Attributes& attributes,
                const SAX2Namespaces& namespaces);
            
            void setDefault(
                PropertyDefinition& thisProperty,
                const SAX2Attributes& attributes);
            
            
            SchemaInfo& schemaInfo;
            
            
            PropertyDefinition currentProperty;
            std::stack<PropertyDefinition>    propertyStack;
            void             setCurrentProperty(const PropertyDefinition& property);
            void             defineProperty();
            
            TypeDefinition   currentType;
            std::stack<TypeDefinition>    typeStack;
            void             setCurrentType(const TypeDefinition& type);
            void             defineType();
            
            TypeDefinitions typeDefinitions;
            
        };
    } // End - namespace sdo
} // End - namespace commonj
#endif //_SDOSCHEMASAX2PARSER_H_
