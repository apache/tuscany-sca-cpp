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

#ifndef tuscany_sca_util_logging_h
#define tuscany_sca_util_logging_h

#include "tuscany/sca/util/Logger.h"

#ifdef _DEBUG
#define LOGENTRY(level, methodName) \
if (Logger::loggingLevel >= level) \
Logger::log(level, "Entering: " methodName);

#define LOGEXIT(level, methodName) \
if (Logger::loggingLevel >= level) \
Logger::log(level, "Exiting: " methodName);

#define LOGINFO(level, message) \
if (Logger::loggingLevel >= level) \
Logger::log(level, message);

#define LOGINFO_1(level, message, arg1) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(level, message, arg1);

#define LOGINFO_2(level, message, arg1, arg2) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(level, message, arg1, arg2);

#else // Not DEBUG

#define LOGENTRY(level, methodName)

#define LOGEXIT(level, methodName)

#define LOGINFO(level, message)

#define LOGINFO_1(level, message, arg1)

#define LOGINFO_2(level, message, arg1, arg2)

#define LOGERROR(level, message) \
if (Logger::loggingLevel >= level) \
Logger::log(level, message);

#define LOGERROR_1(level, message, arg1) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(level, message, arg1);

#define LOGERROR_2(level, message, arg1, arg2) \
if (Logger::loggingLevel >= level) \
Logger::logArgs(level, message, arg1, arg2);
#endif
#endif // tuscany_sca_util_logging_h
