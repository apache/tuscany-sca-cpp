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

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#include "tuscany/sca/util/Logger.h"
#include "tuscany/sca/util/DefaultLogWriter.h"
#include "tuscany/sca/util/FileLogWriter.h"

using namespace std;

namespace tuscany
{
    namespace sca
    {
        namespace util
        {
            LogWriter* Logger::logWriter = getLogWriter();
            
            char* Logger::pid = NULL;
    
            LogWriter* Logger::getLogWriter()
            {
                if (logWriter == 0)
                {
                    setLogWriter(0);
                    
                    pid = new char[21];
#if defined(WIN32)  || defined (_WINDOWS)
                    sprintf(pid, "%lu:%lu", (unsigned long)_getpid(), (unsigned long)GetCurrentThreadId());
#else
                    sprintf(pid, "%lu:%lu", (unsigned long)getpid(), (unsigned long)pthread_self());
#endif
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
                    char messageBuffer[256];
                    int size = vsnprintf(messageBuffer, 255, msg, variableArguments);
#if defined(WIN32)  || defined (_WINDOWS)
                    if (size == -1)
                    {
                        size = _vscprintf(msg, variableArguments);
                        char* bigMessageBuffer = new char[size+1];
                        vsnprintf(bigMessageBuffer, size, msg, variableArguments);
                        bigMessageBuffer[size] = '\0';
                        logWriter->log(level, pid, bigMessageBuffer);
                        delete bigMessageBuffer; 
                    }
#else
                    if (size > 255)
                    {
                        char* bigMessageBuffer = new char[size+1];
                        vsnprintf(bigMessageBuffer, size, msg, variableArguments);
                        bigMessageBuffer[size] = '\0';
                        logWriter->log(level, pid, bigMessageBuffer);
                        delete bigMessageBuffer; 
                    }
#endif
                    else
                    {
                        messageBuffer[255] = '\0';
                        logWriter->log(level, pid, messageBuffer);
                    }
                    va_end(variableArguments);
                }
            }
            
            void Logger::logArgs0(const char* msg, ...)
            {
                if (0 <= loggingLevel)
                {
                    va_list variableArguments;
                    va_start(variableArguments, msg);
                    char messageBuffer[256];
                    int size = vsnprintf(messageBuffer, 255, msg, variableArguments);
#if defined(WIN32)  || defined (_WINDOWS)
                    if (size == -1)
                    {
                        size = _vscprintf(msg, variableArguments);
                        char* bigMessageBuffer = new char[size+1];
                        vsnprintf(bigMessageBuffer, size, msg, variableArguments);
                        bigMessageBuffer[size] = '\0';
                        logWriter->log(0, pid, bigMessageBuffer);
                        delete bigMessageBuffer; 
                    }
#else
                    if (size > 255)
                    {
                        char* bigMessageBuffer = new char[size+1];
                        vsnprintf(bigMessageBuffer, size, msg, variableArguments);
                        bigMessageBuffer[size] = '\0';
                        logWriter->log(0, pid, bigMessageBuffer);
                        delete bigMessageBuffer; 
                    }
#endif
                    else
                    {
                        messageBuffer[255] = '\0';
                        logWriter->log(0, pid, messageBuffer);
                    }
                    va_end(variableArguments);
                }
            }
            void Logger::logArgs1(const char* msg, ...)
            {
                if (1 <= loggingLevel)
                {
                    va_list variableArguments;
                    va_start(variableArguments, msg);
                    char messageBuffer[256];
                    int size = vsnprintf(messageBuffer, 255, msg, variableArguments);
#if defined(WIN32)  || defined (_WINDOWS)
                    if (size == -1)
                    {
                        size = _vscprintf(msg, variableArguments);
                        char* bigMessageBuffer = new char[size+1];
                        vsnprintf(bigMessageBuffer, size, msg, variableArguments);
                        bigMessageBuffer[size] = '\0';
                        logWriter->log(1, pid, bigMessageBuffer);
                        delete bigMessageBuffer; 
                    }
#else
                    if (size > 255)
                    {
                        char* bigMessageBuffer = new char[size+1];
                        vsnprintf(bigMessageBuffer, size, msg, variableArguments);
                        bigMessageBuffer[size] = '\0';
                        logWriter->log(1, pid, bigMessageBuffer);
                        delete bigMessageBuffer; 
                    }
#endif
                    else
                    {
                        messageBuffer[255] = '\0';
                        logWriter->log(1, pid, messageBuffer);
                    }
                    va_end(variableArguments);
                }
            }
            void Logger::logArgs2(const char* msg, ...)
            {
                if (2 <= loggingLevel)
                {
                    va_list variableArguments;
                    va_start(variableArguments, msg);
                    char messageBuffer[256];
                    int size = vsnprintf(messageBuffer, 255, msg, variableArguments);
#if defined(WIN32)  || defined (_WINDOWS)
                    if (size == -1)
                    {
                        size = _vscprintf(msg, variableArguments);
                        char* bigMessageBuffer = new char[size+1];
                        vsnprintf(bigMessageBuffer, size, msg, variableArguments);
                        bigMessageBuffer[size] = '\0';
                        logWriter->log(2, pid, bigMessageBuffer);
                        delete bigMessageBuffer; 
                    }
#else
                    if (size > 255)
                    {
                        char* bigMessageBuffer = new char[size+1];
                        vsnprintf(bigMessageBuffer, size, msg, variableArguments);
                        bigMessageBuffer[size] = '\0';
                        logWriter->log(2, pid, bigMessageBuffer);
                        delete bigMessageBuffer; 
                    }
#endif
                    else
                    {
                        messageBuffer[255] = '\0';
                        logWriter->log(2, pid, messageBuffer);
                    }
                    va_end(variableArguments);
                }
            }

        } // End namespace util
    } // End namespace sca
} // End namespace tuscany
