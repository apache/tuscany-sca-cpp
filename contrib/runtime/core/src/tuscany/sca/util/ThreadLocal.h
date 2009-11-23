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

#ifndef tuscany_sca_util_threadlocal_h
#define tuscany_sca_util_threadlocal_h

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h> 
#else
#include <pthread.h>
#endif

#include "tuscany/sca/export.h"


namespace tuscany
{
    namespace sca
    {
        namespace util
        {
            /**
             * Information about shared libraries and methods to 
             * access these shared libraries.
             */
            class SCA_API ThreadLocal
            {
            public:
                /** 
                 * Constructor.
                 */
                ThreadLocal();
    
                /**
                 * Destructor.
                 */ 
                virtual ~ThreadLocal();
    
                /**
                 * Set the ThreadLocal value.
                 */ 
                void setValue(void *value);

                /**
                 * Get the ThreadLocal value.
                 */ 
                void* getValue() const;

            private:
    
                /**
                 * Handle to the thread local key.
                 */ 
#if defined(WIN32)  || defined (_WINDOWS)
                DWORD index;
#else
                pthread_key_t key;
#endif
    
            };
                
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_util_threadlocal_h
