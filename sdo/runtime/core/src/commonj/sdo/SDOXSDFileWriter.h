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

#ifndef _SDOXSDFILEWRITER_H_
#define _SDOXSDFILEWRITER_H_

#include "commonj/sdo/disable_warn.h"

#include "commonj/sdo/SDOXSDWriter.h"


namespace commonj
{
    namespace sdo
    {
        
/** 
 * SDOXSDFileWriter extends SDOXSDWriter and writes a file.
 */
        class SDOXSDFileWriter : public SDOXSDWriter
        {
            
        public:
            
            SDOXSDFileWriter(const char* xsdFile);
            
            virtual ~SDOXSDFileWriter();
    
            
        };
    } // End - namespace sdo
} // End - namespace commonj


#endif //_SDOXSDFILEWRITER_H_
