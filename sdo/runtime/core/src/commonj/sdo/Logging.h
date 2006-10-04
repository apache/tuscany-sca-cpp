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

#include "commonj/sdo/Logger.h"
#include "commonj/sdo/SDORuntimeException.h"

#ifndef SDO_LOGGING_H
#define SDO_LOGGING_H

/**
 * logging level for stuff you rarely want to see, like 
 * memory allocations.
 */

#define HIGHVOLUME 40
/**
 * logging level for normal path information
 */
#define INFO 30

 /**
 * logging level for something which is unusual. 
 */

#define WARNING 20
/**
 * logging level for a major problem
 */

#define ERROR 10 

#define INDENT 1
#define OUTDENT -1
#define NODENT 0


/**
 * Macro for simplifying addition of trace points
 */

#ifdef _DEBUG
#define LOGENTRY(level, methodName) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(INDENT, level, "Entering: %s", methodName);

/**
 * Macro for simplifying addition of trace points
 */
#define LOGEXIT(level, methodName) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(OUTDENT, level, "Exiting: %s" ,methodName);

/**
 * Macro for simplifying addition of trace points
 */
#define LOGINFO(level, message) \
if (Logger::loggingLevel >= level) \
Logger::log(NODENT, level, message);

/**
 * Macro for simplifying addition of trace points
 */
#define LOGINFO_1(level, message, arg1) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(NODENT,level, message, arg1);

/**
 * Macro for simplifying addition of trace points
 */
#define LOGINFO_2(level, message, arg1, arg2) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(NODENT,level, message, arg1, arg2);

/**
 * Macro for simplifying addition of trace points
 */
#define LOGERROR(level, message) \
if (Logger::loggingLevel >= level) \
Logger::log(NODENT,level, message);

/**
 * Macro for simplifying addition of trace points
 */
#define LOGERROR_1(level, message, arg1) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(NODENT,level, message, arg1);

/**
 * Macro for simplifying addition of trace points
 */
#define LOGERROR_2(level, message, arg1, arg2) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(NODENT,level, message, arg1, arg2);

/**
 * Macro for simplifying addition of trace points
 */
#define LOGSDOEXCEPTION(level, message, arg1) \
if (Logger::loggingLevel >= level) \
Logger::log(NODENT,level, message);\
Logger::logArgs(NODENT,level, "%s:%s\nIn %s\nAt %s line %ld\n",\
                ((SDORuntimeException)arg1).getEClassName(),\
                ((SDORuntimeException)arg1).getMessageText(),\
                ((SDORuntimeException)arg1).getFunctionName(),\
                ((SDORuntimeException)arg1).getFileName(),\
                ((SDORuntimeException)arg1).getLineNumber());

#else // Not DEBUG

/**
 * Macro for simplifying addition of trace points
 */
#define LOGSDOEXCEPTION(level, message, arg1) 

/**
 * Macro for simplifying addition of trace points
 */
#define LOGENTRY(level, methodName)

/**
 * Macro for simplifying addition of trace points
 */
#define LOGEXIT(level, methodName)

/**
 * Macro for simplifying addition of trace points
 */
#define LOGINFO(level, message)

/**
 * Macro for simplifying addition of trace points
 */
#define LOGINFO_1(level, message, arg1)

/**
 * Macro for simplifying addition of trace points
 */
#define LOGINFO_2(level, message, arg1, arg2)
/**
 * Macro for simplifying addition of trace points
 */
#define LOGERROR(level, message)
/**
 * Macro for simplifying addition of trace points
 */
#define LOGERROR_1(level, message, arg1)
/**
 * Macro for simplifying addition of trace points
 */
#define LOGERROR_2(level, message, arg1, arg2)

#endif
#endif // SDO_LOGGING_H
