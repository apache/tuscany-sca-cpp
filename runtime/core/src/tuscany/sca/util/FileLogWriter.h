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

#ifndef tuscany_sca_util_filelogwriter_h
#define tuscany_sca_util_filelogwriter_h

#include <iostream>
#include <fstream>

#include "tuscany/sca/util/LogWriter.h"

namespace tuscany
{
    namespace sca
    {
        namespace util
        {
            /**
             * Log writer to write out to standard out.
             */
            class FileLogWriter : public LogWriter
            {
            public:
            	FileLogWriter(const char* logfile);
            	
                virtual ~FileLogWriter();
    
                /**
                 * Will write to the console.
                 * See LogWriter#log.
                 */
                virtual void log(int level, const char* tid, const char* msg);
            private:
                std::ofstream logFile;
            };
        
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_util_defaultlogwriter_h
