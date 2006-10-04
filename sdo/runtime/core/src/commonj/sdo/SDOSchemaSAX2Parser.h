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

#ifndef _SDOSCHEMASAX2PARSER_H_
#define _SDOSCHEMASAX2PARSER_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SAX2Parser.h"
#include "commonj/sdo/SchemaInfo.h"
#include "stack"
#include "commonj/sdo/TypeDefinitionsImpl.h"
#include "commonj/sdo/GroupDefinition.h"
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
                ParserErrorSetter* insetter,
                bool loadImportNamespace = false);
            
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


            virtual void stream(std::istream& input);

            virtual int parse(const char* filename);
 
            virtual void endDocument();

            
            const SDOXMLString& getTargetNamespaceURI() const {return schemaInfo.getTargetNamespaceURI();}
            
            TypeDefinitionsImpl& getTypeDefinitions() {return typeDefinitions;}
            
            friend std::istream& operator>>(std::istream& input, SDOSchemaSAX2Parser& parser);
            friend std::istringstream& operator>>(std::istringstream& input, SDOSchemaSAX2Parser& parser);


        private:

            // group handling.

            int inGroup;
            bool preParsing;
            GroupDefinition* currentGroup;
            std::vector<GroupDefinition>    groupList;

            bool bInSchema; // only parse when within a schema
            bool bInvalidElement; // where element content is not valid
            bool bInInvalidContent; // actually inside invalid content.
            bool bInvalidList; // a list with no type - not supported.

            virtual void replayEvents(
                const SDOXMLString& uri,
                const SDOXMLString& name,
                bool isGroup,
                const SAX2Attributes& groupAttributes);

            virtual void storeStartElementEvent(
                                const SDOXMLString& localname,
                                const SDOXMLString& prefix,
                                const SDOXMLString& URI,
                                const SAX2Namespaces& namespaces,
                                const SAX2Attributes& attributes);

            virtual void  storeEndElementEvent(
                                const SDOXMLString& localname,
                                const SDOXMLString& prefix,
                                const SDOXMLString& URI
                                );

            virtual int    startSecondaryParse(
                                SDOSchemaSAX2Parser& schemaParser,
                                SDOXMLString& schemaLocation);

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
            
            virtual void startList(
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
                PropertyDefinitionImpl& property,
                const SAX2Attributes& attributes,
                const SAX2Namespaces& namespaces);
            
            void setTypeName(
                TypeDefinitionImpl& type,
                const SAX2Attributes& attributes,
                const SAX2Namespaces& namespaces);
            
            void setDefault(
                PropertyDefinitionImpl& thisProperty,
                const SAX2Attributes& attributes);
            
            
            SchemaInfo& schemaInfo;
            
            
            PropertyDefinitionImpl currentProperty;
            std::stack<PropertyDefinitionImpl>    propertyStack;
            void             setCurrentProperty(const PropertyDefinitionImpl& property);
            void             defineProperty();
            
            TypeDefinitionImpl   currentType;
            std::stack<TypeDefinitionImpl>    typeStack;
            void             setCurrentType(const TypeDefinitionImpl& type);
            void             defineType();
            
            TypeDefinitionsImpl typeDefinitions;

            bool loadImportNamespace;
            
        };


    } // End - namespace sdo
} // End - namespace commonj
#endif //_SDOSCHEMASAX2PARSER_H_
