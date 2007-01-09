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

/* $Rev: 492011 $ $Date: 2007-01-02 18:15:11 -0800 (Tue, 02 Jan 2007) $ */

#ifndef tuscany_sca_util_thread_h
#define tuscany_sca_util_thread_h

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h> 
#else
#include <pthread.h>
#endif

#include <queue>

#include "tuscany/sca/export.h"


namespace tuscany
{
    namespace sca
    {
        namespace util
        {
            /**
             * A portable wrapper for native threads. 
             */
            class SCA_API Thread
            {
            public:
                /** 
                 * Constructor.
                 */
                Thread();
    
                /**
                 * Destructor.
                 */ 
                virtual ~Thread();

                /**
                 * The method that will be run in the thread.
                 */
                virtual void run() = 0;

                /**
                 * Start the thread.
                 */
                void start();
                
                /**
                 * Join the thread.
                 */    
                void join();

            private:
            
                /**
                 * Native thread handle.
                 */ 
#if defined(WIN32)  || defined (_WINDOWS)
                HANDLE hthread;
#else
                pthread_t thread;
#endif
    
            };
                
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_util_thread_h
