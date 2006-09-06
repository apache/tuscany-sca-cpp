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

#include "commonj/sdo/XMLDocumentImpl.h"

namespace commonj
{
    namespace sdo
    {
        XMLDocumentImpl::XMLDocumentImpl(
            DataObjectPtr dob,
            const char* rootURI,
            const char* rootName)
            : dataObject(dob), 
              rootElementURI(rootURI),
              rootElementName(rootName),
              xmlDeclaration(true),
              encoding("UTF-8"),
              xmlVersion("1.0")
        {
        }
        
        XMLDocumentImpl::~XMLDocumentImpl()
        {
        }

        void XMLDocumentImpl::setEncoding(const char* enc)
        {
            encoding = enc;
        }
        void XMLDocumentImpl::setEncoding(const SDOString& enc)
        {
            encoding = enc.c_str();
        }
        
        void XMLDocumentImpl::setXMLDeclaration(bool xmlDecl)
        {
            xmlDeclaration = xmlDecl;
        }
        
        void XMLDocumentImpl::setXMLVersion(const char* xmlVer)
        {
            xmlVersion = xmlVer;
        }
        void XMLDocumentImpl::setXMLVersion(const SDOString& xmlVer)
        {
            xmlVersion = xmlVer.c_str();
        }
        
        void XMLDocumentImpl::setSchemaLocation(const char* schemaLoc)
        {
            schemaLocation = schemaLoc;
        }
        void XMLDocumentImpl::setSchemaLocation(const SDOString& schemaLoc)
        {
            schemaLocation = schemaLoc.c_str();
        }
        
        void XMLDocumentImpl::setNoNamespaceSchemaLocation(const char* noNamespaceSchemaLoc)
        {
            noNamespaceSchemaLocation = noNamespaceSchemaLoc;
        }
        void XMLDocumentImpl::setNoNamespaceSchemaLocation(const SDOString& noNamespaceSchemaLoc)
        {
            noNamespaceSchemaLocation = noNamespaceSchemaLoc.c_str();
        }
        
    } // End - namespace sdo
} // End - namespace commonj
