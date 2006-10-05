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

#ifndef SDO_LOGGER_H
#define SDO_LOGGER_H



#include "commonj/sdo/export.h"
#include "commonj/sdo/LogWriter.h"
namespace commonj
{
    namespace sdo
    {
        
/**
 * Logger is used for debug style logging such as method trace.
 * The logger can log at different levels, and can log entry, exit
 * and events. The log entries can take arguments or be just 
 * strings.
 */
        class Logger {

        public:
/**
 * attach a log writer.
 */
            SDO_API static void setLogWriter(LogWriter* writer);
/**
 * switch logging to a level.
 */
            SDO_API static void setLogging(int level);

/**
 * log a message.
 */
            SDO_API static void log(int indent, int level, const char* msg);
/**
 * log a message with parameters.
 */
            SDO_API static void logArgs(int indent, int level, const char* msg, ...);

            static int loggingLevel;

        private:
            static LogWriter* logWriter;
            static LogWriter* getLogWriter();
            static int setLogging();
        };
        
    } // End namespace sdo
} // End namespace commonj
#endif // SDO_LOGGER_H
