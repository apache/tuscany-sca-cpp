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

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#include <iostream>
#include <stdarg.h>

#include "tuscany/sca/util/Logger.h"
#include "tuscany/sca/util/DefaultLogWriter.h"

using namespace std;

namespace tuscany
{
    namespace sca
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
            loggingVar = getenv("SCA4CPP_LOGGING");
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
                logWriter->log(level, msg);
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
                logWriter->log(level, messageBuffer);
                va_end(variableArguments);
            }
        }
        
    } // End namespace sca
} // End namespace tuscany

