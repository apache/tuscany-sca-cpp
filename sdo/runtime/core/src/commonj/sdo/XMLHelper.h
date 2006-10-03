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

#ifndef _XMLHELPER_H_
#define _XMLHELPER_H_


#include "commonj/sdo/export.h"
#include "commonj/sdo/XMLDocument.h"
#include "commonj/sdo/RefCountingObject.h"

namespace commonj
{
    namespace sdo
    {
        /**  
         *
         * XMLHelper provides the ability to load XML data into 
         * a data object graph, according to  a schema
         */

        class XMLHelper : public RefCountingObject
        {
        public:
            
            /**  load/loadFile - loads xml data
             *
             * De-serializes the specified XML file building a graph of DataObjects.
             * Returns a pointer to the root data object
             */

           SDO_API virtual XMLDocumentPtr createDocument(
               const char* elementname = 0,
               const char* rootElementURI=0) = 0;
           SDO_API virtual XMLDocumentPtr createDocument(
               const SDOString& elementname,
               const SDOString& rootElementURI) = 0;

           SDO_API virtual XMLDocumentPtr loadFile(
                const char* xmlFile,
                const char* targetNamespaceURI=0) = 0;
           SDO_API virtual XMLDocumentPtr loadFile(
                const SDOString& xmlFile,
                const SDOString& targetNamespaceURI = "") = 0;

            SDO_API virtual XMLDocumentPtr load(
                std::istream& inXml,
                const char* targetNamespaceURI=0) = 0;
            SDO_API virtual XMLDocumentPtr load(
                std::istream& inXml,
                const SDOString& targetNamespaceURI = "") = 0;

            SDO_API virtual XMLDocumentPtr load(
                const char* inXml,
                const char* targetNamespaceURI=0) = 0;
            SDO_API virtual XMLDocumentPtr load(
                const SDOString& inXml,
                const SDOString& targetNamespaceURI = "") = 0;
            
            /**  save saves the graph to XML
             *
             * save - Serializes the datagraph to the XML file
             */

              SDO_API virtual void save(XMLDocumentPtr doc, 
                                        const char* xmlFile,
                                        int indent = -1) = 0;
              SDO_API virtual void save(XMLDocumentPtr doc,
                                        const SDOString& xmlFile,
                                        int indent = -1) = 0;

            SDO_API virtual void save(
                DataObjectPtr dataObject,
                const char* rootElementURI,
                const char* rootElementName,
                const char* xmlFile,
                int indent = -1) = 0;
            SDO_API virtual void save(
                DataObjectPtr dataObject,
                const SDOString& rootElementURI,
                const SDOString& rootElementName,
                const SDOString& xmlFile,
                int indent = -1) = 0;

            /**  save saves the graph to XML
             *
             * save - Serializes the datagraph to the XML stream
             */

            SDO_API virtual void save(XMLDocumentPtr doc, std::ostream& outXml,
                int indent = -1) = 0;

            SDO_API virtual void save(
                DataObjectPtr dataObject,
                const char* rootElementURI,
                const char* rootElementName,
                std::ostream& outXml,
                int indent = -1) = 0;
            SDO_API virtual void save(
                DataObjectPtr dataObject,
                const SDOString& rootElementURI,
                const SDOString& rootElementName,
                std::ostream& outXml,
                int indent = -1) = 0;
            
            /**  save saves the graph to XML
             *
             * save - Serializes the datagraph to a string
             */

            SDO_API virtual char* save(XMLDocumentPtr doc, int indent = -1) = 0;
            SDO_API virtual char* save(
                DataObjectPtr dataObject,
                const char* rootElementURI,
                const char* rootElementName,
                int indent = -1) = 0;
            SDO_API virtual char* save(
                DataObjectPtr dataObject,
                const SDOString& rootElementURI,
                const SDOString& rootElementName,
                int indent = -1) = 0;
            
            /**  createDocument creates an XMLDocument
             *
             * An XMLDocument holds a root data object and all its
             * tree of children. This provides the means of storing the
             * name to be applied to the root element when serializing
             */

            SDO_API virtual XMLDocumentPtr createDocument(
                DataObjectPtr dataObject,
                const char* rootElementURI,
                const char* rootElementName) = 0;
            SDO_API virtual XMLDocumentPtr createDocument(
                DataObjectPtr dataObject,
                const SDOString& rootElementURI,
                const SDOString& rootElementName) = 0;
            
            /***********************************/
            /* Destructor */
            /***********************************/
            SDO_API virtual ~XMLHelper();

            /** getErrorCount gets number of parse errors
             *
             * Parser error count - the parse may have 
             * succeeded or partially succeeded or failed. There
             * may be errors to report or handle.
             */

            virtual int  getErrorCount() const = 0;

            /** getErrorMessage gets the nth error message
             *
             * Each error has a message, usually giving the line and file
             * in which the parser error occurred.
             */

            virtual const char* getErrorMessage(int errnum) const = 0;

            
        };
    } // End - namespace sdo
} // End - namespace commonj

#endif //_XMLHELPER_H_
