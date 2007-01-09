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

#ifndef tuscany_sca_util_queue_h
#define tuscany_sca_util_queue_h

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
             * A thread safe FIFO queue. 
             */
            class SCA_API Queue
            {
            public:
                /** 
                 * Constructor.
                 */
                Queue();
    
                /**
                 * Destructor.
                 */ 
                virtual ~Queue();

                /**
                 * Dequeue an element
                 */
                void* dequeue();
                
                /**
                 * Enqueue an element
                 */    
                void enqueue(void* element);
                
            private:
            
                /**
                 * The STL queue used to hold elements.
                 */
                std::queue<void*> queue;
    
                /**
                 * Handles to the mutex and condition variable
                 * used to synchronize access to the queue.
                 */ 
#if defined(WIN32)  || defined (_WINDOWS)
                CRITICAL_SECTION section;
                HANDLE hevent;
#else
                pthread_mutex_t mutex;
                pthread_cond_t cond;
#endif
    
            };
                
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_util_queue_h
