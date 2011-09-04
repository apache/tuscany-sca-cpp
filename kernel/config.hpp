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

#ifndef tuscany_config_hpp
#define tuscany_config_hpp

#include "ap_config.h"
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#include "../config.h"

/**
 * Platform configuration and debug functions.
 */
namespace tuscany
{

/**
 * Attribute used to mark unused parameters.
 */
#ifndef unused
#define unused __attribute__ ((unused))
#endif

/**
 * Compiler feature detection.
 */
#ifdef __clang__

#if __has_feature(cxx_lambdas)
#define HAS_CXX0X_LAMBDAS 1
#endif

#else

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 4)
#define HAS_CXX0X_LAMBDAS 1
#endif

#endif

/**
 * Debug utilities.
 */
#ifdef WANT_MAINTAINER_MODE

/**
 * Add string watch members to important classes to help watch them in a debugger.
 */
//#define WANT_MAINTAINER_WATCH

/**
 * Increment/decrement a debug counter.
 */
bool debug_inc(long int& c) {
    c++;
    return true;
}

bool debug_dec(long int& c) {
    c--;
    return true;
}

#else

#define debug_inc(c)
#define debug_dec(c)

#endif

}
#endif /* tuscany_config_hpp */
