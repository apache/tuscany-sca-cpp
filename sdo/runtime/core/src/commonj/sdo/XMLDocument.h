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

#ifndef _XMLDOCUMENT_H_
#define _XMLDOCUMENT_H_


#include "commonj/sdo/export.h"

#include "commonj/sdo/DataObject.h"

namespace commonj
{
    namespace sdo
    {
        /**  XMLDocument - place for holding a graph
         *
         * The XMLDocument class provides a place to hold a loaded
         * graph of data. The root element name is maintained here so that
         * the graph can be serialized to the same name later.
         */

        
        class XMLDocument : public RefCountingObject
        {
            
        public:
            
            SDO_API virtual ~XMLDocument();
            
        /**  getRootDataObject- return the topmost data object
         *
         * When XML data is loaded, there is one base element which becomes
         * the root data object. As this data object is not a property of any
         * other data object, it has no name
         * The XMLDocument stores the name which was in the XML file.
         * This root data object has a URI and a Name only within this
         * XMLDocument
         */

            SDO_API virtual DataObjectPtr getRootDataObject() const = 0;

        /**  getRootElementName- return the topmost data objects name
         *
         * When XML data is loaded, there is one base element which becomes
         * the root data object. As this data object is not a property of any
         * other data object, it has no name
         * The XMLDocument stores the name/uri which was in the XML file.
         */
            SDO_API virtual const char* getRootElementURI() const = 0;

        /**  getRootElementURI- return the topmost data objects uri
         *
         * When XML data is loaded, there is one base element which becomes
         * the root data object. As this data object is not a property of any
         * other data object, it has no name
         * The XMLDocument stores the name/uri which was in the XML file.
         */
            SDO_API virtual const char* getRootElementName() const = 0;

        /**  getEncoding- return the encoding for this document
         *
         * NOT IMPLEMENTED
         * This returns the encoding specified in the original XML.
         */
            SDO_API virtual const char* getEncoding() const = 0;

        /**  setEncoding- set the encoding to save this document
         *
         * This sets the encoding to save this data to XML.
         */
            SDO_API virtual void setEncoding(const char* encoding) = 0;
            SDO_API virtual void setEncoding(const SDOString& encoding) = 0;

        /**  getXMLDeclaration
         *
         * NOT IMPLEMENTED
         */
            SDO_API virtual bool getXMLDeclaration() const = 0;

        /**  setXMLDeclaration
         *
         * Sets the XMLDeclaration
         */
            SDO_API virtual void setXMLDeclaration(bool xmlDeclaration) = 0;

        /**  getXMLVersion- return the xml version for this document
         *
         * NOT IMPLEMENTED
         */
            SDO_API virtual const char* getXMLVersion() const = 0;

        /**  setXMLVersion- sets the version for this document
         *
         * NOT IMPLEMENTED
         */
            SDO_API virtual void setXMLVersion(const char* xmlVersion) = 0;
            SDO_API virtual void setXMLVersion(const SDOString& xmlVersion) = 0;

        /**  getSchemaLocation- return the schema location
         *
         * NOT IMPLEMENTED
         */
            SDO_API virtual const char* getSchemaLocation() const = 0;

        /**  setSchemaLocation
         *
         * Sets the XML Schema location.
         */
            SDO_API virtual void setSchemaLocation(const char* schemaLocation) = 0;
            SDO_API virtual void setSchemaLocation(const SDOString& schemaLocation) = 0;

        /**  getNoNamespaceSchemaLocation
         *
         * NOT IMPLEMENTED
         */
            SDO_API virtual const char* getNoNamespaceSchemaLocation() const = 0;

        /**  setNoNamespaceSchemaLocation
         *
         * Sets the nonamespace schema location
         */
            SDO_API virtual void setNoNamespaceSchemaLocation(const char* noNamespaceSchemaLocation) = 0;        
            SDO_API virtual void setNoNamespaceSchemaLocation(const SDOString& noNamespaceSchemaLocation) = 0;        
            
            SDO_API friend std::istream& operator>>(std::istream& input, XMLDocument& doc);
            
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_XMLDOCUMENT_H_
