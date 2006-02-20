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

/* $Rev$ $Date: 2006/02/01 12:53:49 $ */

#ifndef _SDOXSDWRITER_H_
#define _SDOXSDWRITER_H_

#include "commonj/sdo/disable_warn.h"

#include <libxml/xmlwriter.h>
#include "commonj/sdo/SDOXMLString.h"
#include "commonj/sdo/DataObject.h"
#include "commonj/sdo/DataFactoryImpl.h"


namespace commonj
{
    namespace sdo
    {

/** 
 * SDOXSDWriter writes XSD from types and properties.
 * The writer takes the contents of a data factory and writes an
 * XSD which will be usable to serialize data from the graph.
 */
        class SDOXSDWriter
        {

        public:

            SDOXSDWriter(DataFactoryPtr dataFactory = NULL);

            virtual ~SDOXSDWriter();

            int write(const TypeList& types, const SDOXMLString& targetNamespaceURI,
                const propertyMap& openprops, int indent = -1);

        protected:
            void setWriter(xmlTextWriterPtr textWriter);
            void freeWriter();

        private:
            xmlTextWriterPtr writer;

            std::map<SDOXMLString,SDOXMLString> namespaceMap;

            void writeProps( const propertyMap& pl, const SDOXMLString& targetNamespaceURI,
                SDOXMLString& xsd);
            int writeDO(DataObjectPtr dataObject, const SDOXMLString& elementName);

            DataFactoryPtr    dataFactory;

            SDOXMLString resolveName(const SDOXMLString& uri, const SDOXMLString& name, const SDOXMLString& targetNamespaceURI);

        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_SDOXSDWRITER_H_
