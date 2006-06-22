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

#include "commonj/sdo/SAX2Attribute.h"


namespace commonj
{
    namespace sdo
    {
            
/**
 * SAX2Attribute holds an attribute supplied by the SAX2 parser.
 */
        SAX2Attribute::SAX2Attribute()
        {
        }
        
        SAX2Attribute::SAX2Attribute(const xmlChar **attribute) // localname/prefix/URI/value/end
        {
            name = attribute[0];
            prefix = attribute[1];
            uri = attribute[2];
            value = SDOXMLString(attribute[3],0, (int)(attribute[4] - attribute[3]));
        }
        
        SAX2Attribute::~SAX2Attribute()
        {    
        }
        
        
    } // End - namespace sdo
} // End - namespace commonj