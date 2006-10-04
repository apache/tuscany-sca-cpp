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

#ifndef _PROPERTYDefinitionImpl_H_
#define _PROPERTYDefinitionImpl_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SDOXMLString.h"
#include <vector>



namespace commonj
{
    namespace sdo
    {
        
        
/**
 * PropertyDefinitionImpl holds a property as defined by an XSD.
 * The XSD holds more information than does the SDO Property.
 * This class holds the extra information found as the XSD is
 * parsed.
 */
        class PropertyDefinitionImpl
        {
            
        public:
            PropertyDefinitionImpl();
            virtual ~PropertyDefinitionImpl();
            
            // where this property is to be substituted for another
            SDOXMLString substituteName;
            SDOXMLString substituteUri;

            // where there are substitute names for this property.
            std::vector<SDOXMLString> substituteNames;
            std::vector<SDOXMLString> substituteLocalNames;

            SDOXMLString aliases;

            SDOXMLString name;
            SDOXMLString localname;
            
            SDOXMLString typeUri;
            SDOXMLString typeName;
            SDOXMLString fullTypeName;

            SDOXMLString fullLocalTypeName;

            SDOXMLString defaultValue;

            bool isMany;
            bool isContainment;
            bool isReadOnly;

            bool isID;
            bool isIDREF;
            bool isReference;
            bool isElement;
                        
            bool isQName;

            bool isSubstitute;
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_PROPERTYDefinitionImpl_H_
