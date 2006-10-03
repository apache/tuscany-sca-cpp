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

#ifndef _XSDHELPERIMPL_H_
#define _XSDHELPERIMPL_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/XSDHelper.h"
#include "commonj/sdo/export.h"
#include "commonj/sdo/SDOXMLString.h"
#include "commonj/sdo/SAX2Namespaces.h"
#include "commonj/sdo/SchemaInfo.h"
#include "commonj/sdo/TypeDefinitionsImpl.h"
#include "commonj/sdo/ParserErrorSetter.h"

namespace commonj
{
    namespace sdo
    {
        
        /**  
         *
         * XSDHelperImpl is the implementation of the abstract class
         * XSDHelper.
         * Provides the means of loading and saving XSD information 
         * from/to the metadata (Types and properties)
         */
        
        class XSDHelperImpl : public XSDHelper, ParserErrorSetter
        {
        public:
            
            // Constructor
            XSDHelperImpl(DataFactoryPtr dataFactory);
            
            // Destructor
            virtual ~XSDHelperImpl();
            
            /**  define builds types/properties from file or stream
             *
             * define/defineFile
             *
             * Populates the data factory with Types and Properties from the schema
             * Loads from file, stream or char* buffer.
             * The return value is the URI of the root Type
             *
             */
            virtual const char* defineFile(const char* schemaFile, bool loadImportNamespace = false);
            virtual const char* defineFile(const SDOString& schemaFile, bool loadImportNamespace = false);
            virtual const char* define(std::istream& schema, bool loadImportNamespace = false);
            virtual const char* define(const char* schema, bool loadImportNamespace = false);
            virtual const char* define(const SDOString& schema, bool loadImportNamespace = false);
            
            /** getErrorCount gets number of parse errors
             *
             * Parser error count - the parse may have 
             * succeeded or partially succeeded or failed. There
             * may be errors to report or handle.
             */

            virtual int  getErrorCount() const;

            /** getErrorMessage gets the nth error message
             *
             * Each error has a message, usually giving the line and file
             * in which the parser error occurred.
             */

            virtual const char* getErrorMessage(int errnum) const;
            virtual void setError(const char* error);
            
            /**  generate buildsXSD from types/properties
             *
             * generate/generateFile
             *
             * Saves the types/properties to an XSD stream or file
             *
             */

            virtual char* generate(
                const TypeList& types,
                const char* targetNamespaceURI = "",
                int indent = -1
                );
            virtual char* generate(
                const TypeList& types,
                const SDOString& targetNamespaceURI = "",
                int indent = -1
                );
            void generate(
                const TypeList& types,
                std::ostream& outXsd,
                const char* targetNamespaceURI = "", 
                int indent = -1
                );
            void generate(
                const TypeList& types,
                std::ostream& outXsd,
                const SDOString& targetNamespaceURI = "",
                int indent = -1
                );
            virtual void generateFile(
                const TypeList& types,
                const char* fileName,
                const char* targetNamespaceURI = "", 
                int indent = -1);
            virtual void generateFile(
                const TypeList& types,
                const SDOString& fileName,
                const SDOString& targetNamespaceURI = "", 
                int indent = -1);
            
            virtual DataFactoryPtr getDataFactory();
            
            // Return the URI for the root Type
            virtual const char* getRootTypeURI()
            {
                return schemaInfo.getTargetNamespaceURI();
            }
 
            virtual void defineTypes(TypeDefinitions& types);

        private:
            virtual void clearErrors();

            void newSubstitute(const char* entryName,
                               PropertyDefinitionImpl& prop);

            void addSubstitutes(PropertyDefinitionImpl& prop,
                                TypeDefinitionImpl& ty);

            void defineTypes(TypeDefinitionsImpl& types);
            int     parse(const char* source);
            
            // Instance variables
            DataFactoryPtr    dataFactory;    // metadata
            SchemaInfo        schemaInfo;

            std::vector<char*> parseErrors;
            
        };
        
    } // End - namespace sdo
} // End - namespace commonj

#endif // _XSDHELPERIMPL_H_
