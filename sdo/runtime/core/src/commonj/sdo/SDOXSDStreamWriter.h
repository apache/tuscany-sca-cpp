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

#ifndef _SDOXSDSTREAMWRITER_H_
#define _SDOXSDSTREAMWRITER_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SDOXSDBufferWriter.h"
#include <iostream>


namespace commonj
{
    namespace sdo
    {
        
/** 
 * SDOXSDStreamWriter extends SDOXSDWriter and writes a stream.
 */
        class SDOXSDStreamWriter : public SDOXSDBufferWriter
        {
            
        public:
            
            SDOXSDStreamWriter(std::ostream& outXML);                
            virtual ~SDOXSDStreamWriter();
            
            int write(const TypeList& types, const SDOXMLString& targetNamespaceURI,
                const propertyMap& openprops, int indent = -1);
        private:
            std::ostream& outXmlStream;
            
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_SDOXSDSTREAMWRITER_H_
