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

#ifndef _SCHEMAINFO_H_
#define _SCHEMAINFO_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SAX2Namespaces.h"
#include "commonj/sdo/DataObject.h"

namespace commonj
{
    namespace sdo
    {
        
/**
 *
 * SchemaInfo holds information supplied by the parser on the schema
 * held in an XSD
 */        
        class SchemaInfo
        {
            
        public:
            SDO_SPI SchemaInfo();
            SDO_SPI virtual ~SchemaInfo();
            
            SDO_SPI SAX2Namespaces& getSchemaNamespaces() {return schemaNamespaces;}
            
            SDO_SPI const SDOXMLString& getTargetNamespaceURI() const {return targetNamespaceURI;}
            SDO_SPI void setTargetNamespaceURI(const SDOXMLString& URI) {targetNamespaceURI = URI;}            
            
        private:
            SAX2Namespaces    schemaNamespaces;
            SDOXMLString    targetNamespaceURI;            
            
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_SCHEMAINFO_H_
