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

/* $Rev$ $Date: 2006/01/23 16:39:24 $ */

#include "commonj/sdo/Logger.h"
#include <iostream>
#include <stdarg.h>
#include "commonj/sdo/DefaultLogWriter.h"
#include <stdio.h>

using namespace std;

namespace commonj
{
    namespace sdo
    {
        LogWriter* Logger::logWriter = getLogWriter();

        LogWriter* Logger::getLogWriter()
        {
            if (logWriter == 0)
            {
                setLogWriter(0);
            }
            return logWriter;
        }

        void Logger::setLogWriter(LogWriter* writer)
        {
            if (logWriter != writer)
            {
                delete logWriter;
            }

            if (writer == 0)
            {
                logWriter = new DefaultLogWriter;
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
            loggingVar = getenv("TUSCANY_SDOCPP_LOGGING");
            if (loggingVar == 0)
                return 0;
            else
                return atoi(loggingVar);
        }
        
        void Logger::setLogging(int level)
        {
            loggingLevel = level;
        }
            
        void Logger::log(int indent, int level, const char* msg)
        {
            if (level <= loggingLevel)
            {
                logWriter->log(indent, level, msg);
            }
        }

        void Logger::logArgs(int indent, int level, const char* msg, ...)
        {
            if (level <= loggingLevel)
            {
                va_list variableArguments;
                va_start(variableArguments, msg);
                char messageBuffer[1024];
                vsprintf(messageBuffer, msg, variableArguments);
                logWriter->log(indent, level, messageBuffer);
                va_end(variableArguments);
            }
        }
        
    } // End namespace sdo
} // End namespace commonj

