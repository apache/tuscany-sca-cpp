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

#ifndef tuscany_sca_util_logger_h
#define tuscany_sca_util_logger_h

#include "tuscany/sca/export.h"
#include "tuscany/sca/util/LogWriter.h"
namespace tuscany
{
    namespace sca
    {
        /**
         * Provide a logging interface.
         */
        class Logger {

        public:
            /**
             * Set the log writer to use.
             * @param writer The writer to use for all subsequent logging.
             */
            SCA_API static void setLogWriter(LogWriter* writer);

            /**
             * Set or reset the logging level. Any message with a higher logging
             * level than this value will be filtered (i.e. not shown).
             * @param level The level of logging to use for all subsequent logging.
             */
            SCA_API static void setLogging(int level);

            /**
             * Log a message.
             * @param level The log level of this message.
             * @param msg The message to be logged.
             */
            SCA_API static void log(int level, const char* msg);

            /**
             * Log a message with variable arguments.
             * @param level The log level of this message.
             * @param msg The message to be logged. Must include template 
             * characters as described in printf.
             * @param ... Variable arguments.
             */
            SCA_API static void logArgs(int level, const char* msg, ...);

            /**
             * The currently set logging level
             */
            SCA_API static int loggingLevel;

        private:
            /** 
             * The current log writer.
             */
            static LogWriter* logWriter;

            /**
             * Get the current log writer.
             * @return The current log writer.
             */
            static LogWriter* getLogWriter();

            /**
             * Retrieves the logging level set as an environment variable.
             */
            static int setLogging();
        };
        
    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_util_logger_h
