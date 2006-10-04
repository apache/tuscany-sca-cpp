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

#ifndef _XSDPropertyInfo_H_
#define _XSDPropertyInfo_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/DASValue.h"
#include "commonj/sdo/PropertyDefinitionImpl.h"
#include "commonj/sdo/SDOXMLString.h"

namespace commonj
{
    namespace sdo
    {
            
        /**  
         *
         * This holds a property definition during the parsing process.
         * This holds a property definition during the parsing process
         * when all properties need to be read and stored prior to creation
         * within the data facttory.
         */

        class XSDPropertyInfo : public DASValue
        {
        public:
            
            XSDPropertyInfo();
            XSDPropertyInfo(const PropertyDefinitionImpl& prop);
            
            virtual ~XSDPropertyInfo();
            
            const PropertyDefinitionImpl& getPropertyDefinition() {return property;}
            
            
        private:
            PropertyDefinitionImpl property;                
        };
    }
}
#endif //_XSDPropertyInfo_H_
