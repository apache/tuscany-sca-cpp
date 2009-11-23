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

#ifndef tuscany_sca_ruby_ruby_h
#define tuscany_sca_ruby_ruby_h

// Ruby inists on MS VC6 ... we'll see about that!
#if defined(WIN32)  || defined (_WINDOWS)
#define _REAL_MSC_VER _MSC_VER
#undef _MSC_VER
#define _MSC_VER 1200
#endif

#include <ruby.h>

#if defined(WIN32)  || defined (_WINDOWS)
#undef _MSC_VER
#define _MSC_VER _REAL_MSC_VER
#endif

#endif // tuscany_sca_ruby_ruby_h
