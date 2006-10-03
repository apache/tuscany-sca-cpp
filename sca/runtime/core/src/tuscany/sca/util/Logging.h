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

/* $Rev$ $Date$ */

#ifndef tuscany_sca_util_logging_h
#define tuscany_sca_util_logging_h

#include "tuscany/sca/util/Logger.h"

#define LOGENTRY(level, methodName) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::log(level, "Entering: " methodName);

#define LOGEXIT(level, methodName) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::log(level, "Exiting: " methodName);

#define LOGINFO(level, message) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::log(level, message);

#define LOGINFO_1(level, message, arg1) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::logArgs(level, message, arg1);

#define LOGINFO_2(level, message, arg1, arg2) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::logArgs(level, message, arg1, arg2);

#define LOGINFO_3(level, message, arg1, arg2, arg3) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::logArgs(level, message, arg1, arg2, arg3);

#define LOGERROR(level, message) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::log(level, message);

#define LOGERROR_1(level, message, arg1) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::logArgs(level, message, arg1);

#define LOGERROR_2(level, message, arg1, arg2) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::logArgs(level, message, arg1, arg2);

#define LOGERROR_3(level, message, arg1, arg2, arg3) \
if (tuscany::sca::Logger::loggingLevel >= level) \
tuscany::sca::Logger::logArgs(level, message, arg1, arg2, arg3);

#endif // tuscany_sca_util_logging_h
