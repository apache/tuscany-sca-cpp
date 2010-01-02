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

#ifndef tuscany_debug_hpp
#define tuscany_debug_hpp

/**
 * Debug functions and macros.
 */

namespace tuscany
{

#ifdef _DEBUG

//#define _DEBUG_WATCH

/**
 * Increment / decrement a debug counter.
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

/**
 * Attribute used to mark unused parameters.
 */
#ifndef unused
#define unused __attribute__ ((unused))
#endif

}
#endif /* tuscany_debug_hpp */
