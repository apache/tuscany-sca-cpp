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

#ifndef _SDOXMLWRITER_H_
#define _SDOXMLWRITER_H_

#include "commonj/sdo/disable_warn.h"

#include <libxml/xmlwriter.h>
#include "commonj/sdo/XMLDocument.h"
#include "commonj/sdo/SDOXMLString.h"
#include "commonj/sdo/SchemaInfo.h"
#include "commonj/sdo/DataFactory.h"
#include "commonj/sdo/XSDPropertyInfo.h"
#include <stack>
#include "commonj/sdo/SAX2Namespaces.h"
#include "commonj/sdo/DataObjectImpl.h"


namespace commonj
{
    namespace sdo
    {
            
/**
 * SDOXMLWriter writes a data object tree to XML
 */
        class SDOXMLWriter
        {
            
        public:
            
            SDOXMLWriter(DataFactoryPtr dataFactory = NULL);
            
            virtual ~SDOXMLWriter();
            
            int write(XMLDocumentPtr doc, int indent=-1);

        protected:
            void setWriter(xmlTextWriterPtr textWriter);
            void freeWriter();
            
        private:
            xmlTextWriterPtr writer;

            void handleChangeSummaryAttributes(
                ChangeSummaryPtr cs,
                DataObjectPtr doB);

            void handleChangeSummaryElements(
                ChangeSummaryPtr cs, 
                DataObjectPtr dob);

            void handleChangeSummaryDeletedObject(
                const char* name, 
                ChangeSummaryPtr cs, 
                DataObjectPtr dob);

            void handleSummaryChange(
                const SDOXMLString& elementName, 
                ChangeSummaryPtr cs, 
                DataObjectPtr dob);

            void handleChangeSummary(
                const SDOXMLString& elementName,
                ChangeSummaryPtr cs);

            void addToNamespaces(DataObjectImpl* dob);
 
            int writeDO(
                DataObjectPtr dataObject,
                const SDOXMLString& elementURI,
                const SDOXMLString& elementName,
                bool writeXSIType = false,
                bool isRoot = false);

            SchemaInfo* schemaInfo;
            DataFactoryPtr    dataFactory;

            XSDPropertyInfo* getPropertyInfo(const Type& type, const Property& property);
            
            //std::stack<SDOXMLString>    namespaceUriStack;
            //SAX2Namespaces namespaces;
            //std::stack<SAX2Namespaces>    namespaceStack;
            int spacescount;
            std::map<SDOXMLString,SDOXMLString> namespaceMap;
            SDOXMLString tnsURI;
           

            void writeReference(
                DataObjectPtr dataObject, 
                const Property& property,
                bool isElement,
                DataObjectPtr refferedToObject = 0);

            void writeXmlnsXsi();

        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_SDOXMLWRITER_H_
