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

#ifndef _XMLHELPERIMPL_H_
#define _XMLHELPERIMPL_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/XMLHelper.h"
#include "commonj/sdo/export.h"
#include "commonj/sdo/SDOXMLString.h"
#include "commonj/sdo/SAX2Namespaces.h"
#include "commonj/sdo/SchemaInfo.h"
#include "commonj/sdo/TypeDefinitions.h"
#include "commonj/sdo/ParserErrorSetter.h"

namespace commonj
{
    namespace sdo
    {
        
        /**  
         *
         * XMLHelperImpl is the implementation of the abstract class
         * XMLHelper.
         * Provides the ability to load XML data into 
         * a data object graph, according to  a schema
         */
        
        class XMLHelperImpl : public XMLHelper, ParserErrorSetter
        {
        public:            
            // Constructor
            XMLHelperImpl(DataFactoryPtr dataFactory);
            
            // Destructor
            virtual ~XMLHelperImpl();
            
            virtual int  getErrorCount() const;
            virtual const char*  getErrorMessage(int errnum) const;
            virtual void setError(const char* error);


            /**  load/loadFile - loads xml data
             *
             * De-serializes the specified XML file building a graph of DataObjects.
             * Returns a pointer to the root data object
             */

            virtual XMLDocumentPtr loadFile(
                const char* xmlFile,
                const char* targetNamespaceURI = 0);
            virtual XMLDocumentPtr load(
                istream& inXml,
                const char* targetNamespaceURI = 0);
            virtual XMLDocumentPtr load(
                const char* inXml,
                const char* targetNamespaceURI = 0);
            
            virtual XMLDocumentPtr createDocument(
                DataObjectPtr dataObject,
                const char* rootElementURI,
                const char* rootElementName);

            /**  save saves the graph to XML
             *
             * save - Serializes the datagraph to the XML file
             */

            void save(XMLDocumentPtr doc, const char* xmlFile);
            void save(
                DataObjectPtr dataObject,
                const char* rootElementURI,
                const char* rootElementName,
                const char* xmlFile);

            /**  save saves the graph to XML
             *
             * save - Serializes the datagraph to a stream
             */

            void save(XMLDocumentPtr doc, std::ostream& outXml);
            void save(
                DataObjectPtr dataObject,
                const char* rootElementURI,
                const char* rootElementName,
                std::ostream& outXml);

            /**  save saves the graph to XML
             *
             * save - Serializes the datagraph to a string
             */
            char* save(XMLDocumentPtr doc);
            char* save(
                DataObjectPtr dataObject,
                const char* rootElementURI,
                const char* rootElementName);
                            
        private:
            int     parse(const char* source);
            void clearErrors();

            std::vector<char*> parseErrors;
            
            // Instance variables
            DataFactoryPtr    dataFactory;
            SDOXMLString targetNamespaceURI;

            XMLDocumentPtr createDocument(DataObjectPtr dataObject);

            DataFactoryPtr getDataFactory();
        };
        
    } // End - namespace sdo
} // End - namespace commonj

#endif // _XMLHELPERIMPL_H_
