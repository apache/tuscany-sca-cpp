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

#ifndef SDO_DefaultLOGWriter_H
#define SDO_DefaultLOGWriter_H

#include "commonj/sdo/LogWriter.h"



namespace commonj
{
    namespace sdo
    {
        
/** 
 * DefaultLogWriter is a basic log writer, part of the logger.
 * The logging API holds a log writer , and uses it to send
 * output whereever it is needed. The application writer would
 * hook their own LogWriter in place of this simple one.
 */
        class DefaultLogWriter : public LogWriter
        {
        public:

            DefaultLogWriter();
            virtual ~DefaultLogWriter();

            virtual void log(int indent, int level, const char* msg);
        private:
            int formatindent;
        };
        
    } // End namespace sca
} // End namespace commonj
#endif // SDO_DefaultLOGWriter_H
