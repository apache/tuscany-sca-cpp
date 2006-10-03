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

#define SDO4CPP_VERSION 20051202

#if defined(WIN32)  || defined (_WINDOWS)

#define int64_t __int64

#ifdef SDO_EXPORTS
#include "commonj/sdo/disable_warn.h"
#    define SDO_API __declspec(dllexport)
#    define SDO_SPI __declspec(dllexport)
#    define EXPIMP
#else
#    define SDO_API __declspec(dllimport)
#    define SDO_SPI __declspec(dllimport)
#    define EXPIMP extern
#endif

#else
#include <sys/time.h>
#include <inttypes.h> 
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#    define SDO_API
#    define SDO_SPI
#    define EXPIMP
#endif

