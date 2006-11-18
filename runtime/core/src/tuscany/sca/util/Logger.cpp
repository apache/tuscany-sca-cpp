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

#include <iostream>
#include <stdarg.h>

#include "tuscany/sca/util/Logger.h"
#include "tuscany/sca/util/DefaultLogWriter.h"
#include "tuscany/sca/util/FileLogWriter.h"

using namespace std;

namespace tuscany
{
    namespace sca
    {
        LogWriter* Logger::logWriter = getLogWriter();
        
        char* Logger::pid = NULL;

        LogWriter* Logger::getLogWriter()
        {
            if (logWriter == 0)
            {
                setLogWriter(0);
                
                pid = new char[10];
                sprintf(pid, "%d", getpid());
            }
            return logWriter;
        }

        void Logger::setLogWriter(LogWriter* writer)
        {
            if (logWriter != writer
            	&& logWriter != 0)
            {
                delete logWriter;
            }

            if (writer == 0)
            {
                char*  loggingVar = 0;
                loggingVar = getenv("TUSCANY_SCACPP_LOG");
                if (loggingVar == 0)
                    logWriter = new DefaultLogWriter;
                else
                    logWriter = new FileLogWriter(loggingVar);
            }
            else
            {
                logWriter = writer;
            }
        }

        int Logger::loggingLevel = setLogging();
        
        int Logger::setLogging()
        {
            char*  loggingVar = 0;
            loggingVar = getenv("TUSCANY_SCACPP_LOGGING");
            if (loggingVar == 0)
                return 0;
            else
                return atoi(loggingVar);
        }
        
        void Logger::setLogging(int level)
        {
            loggingLevel = level;
        }
            
        void Logger::log(int level, const char* msg)
        {
            if (level <= loggingLevel)
            {
                logWriter->log(level, pid, msg);
            }
        }

        void Logger::logArgs(int level, const char* msg, ...)
        {
            if (level <= loggingLevel)
            {
                va_list variableArguments;
                va_start(variableArguments, msg);
                char messageBuffer[1024];
                vsprintf(messageBuffer, msg, variableArguments);
                logWriter->log(level, pid, messageBuffer);
                va_end(variableArguments);
            }
        }
        
        void Logger::logArgs0(const char* msg, ...)
        {
            if (0 <= loggingLevel)
            {
                va_list variableArguments;
                va_start(variableArguments, msg);
                char messageBuffer[1024];
                vsprintf(messageBuffer, msg, variableArguments);
                logWriter->log(0, pid, messageBuffer);
                va_end(variableArguments);
            }
        }
        void Logger::logArgs1(const char* msg, ...)
        {
            if (1 <= loggingLevel)
            {
                va_list variableArguments;
                va_start(variableArguments, msg);
                char messageBuffer[1024];
                vsprintf(messageBuffer, msg, variableArguments);
                logWriter->log(1, pid, messageBuffer);
                va_end(variableArguments);
            }
        }
        void Logger::logArgs2(const char* msg, ...)
        {
            if (2 <= loggingLevel)
            {
                va_list variableArguments;
                va_start(variableArguments, msg);
                char messageBuffer[1024];
                vsprintf(messageBuffer, msg, variableArguments);
                logWriter->log(2, pid, messageBuffer);
                va_end(variableArguments);
            }
        }

    } // End namespace sca
} // End namespace tuscany
