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

#ifndef _XSDTypeInfo_H_
#define _XSDTypeInfo_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/DASValue.h"
#include "commonj/sdo/TypeDefinitionImpl.h"

namespace commonj
{
    namespace sdo
    {
        /**  
         *
         * This holds a type definition during the parsing process
         * when all types need to be read and stored prior to creation
         * within the data factory.
         */
        
        class XSDTypeInfo : public DASValue
        {
        public:
            XSDTypeInfo();
            XSDTypeInfo(const TypeDefinitionImpl& typeDef);
            virtual ~XSDTypeInfo();
            const TypeDefinitionImpl& getTypeDefinition() {return typeDefinition;}

        private:
            TypeDefinitionImpl typeDefinition;            
        };
    }
}
#endif //_XSDTypeInfo_H_
