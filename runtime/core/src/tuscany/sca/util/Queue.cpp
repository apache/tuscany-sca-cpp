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

/* $Rev: 491752 $ $Date: 2007-01-01 22:22:23 -0800 (Mon, 01 Jan 2007) $ */

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#pragma warning(disable: 4251)
#else
#include "tuscany_sca_config.h"
#endif

#include <errno.h>

#include <sstream>

#include "tuscany/sca/util/Queue.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"

using namespace std;


namespace tuscany
{
    namespace sca
    {
        namespace util
        {
            Queue::Queue()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                InitializeCriticalSection(&section);
                hevent = CreateEvent (NULL, TRUE, FALSE, NULL);
#else
                int rc = pthread_mutex_init(&mutex, NULL);
                if (rc)
                {
                    ostringstream msg;
                    msg << "Failed to create mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }

                rc = pthread_cond_init(&cond, NULL);
                if (rc)
                {
                    ostringstream msg;
                    msg << "Failed to create condition variable, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }
            
            Queue::~Queue()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                DeleteCriticalSection(&section);
                CloseHandle(hevent);
#else
                int rc = pthread_mutex_destroy(&mutex);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to destroy mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }

                rc = pthread_cond_destroy(&cond);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to destroy condition variable, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }
            
            void Queue::enqueue(void* element)
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                EnterCriticalSection(&section);

                try
                {
                    queue.push(element);
                    
                    if (queue.size() == 1)
                    {
                        SetEvent(hevent);
                    }
                }
                catch (...)
                {
                    LeaveCriticalSection(&section);
                    throw;
                }
                LeaveCriticalSection(&section);
#else
                int rc = pthread_mutex_lock(&mutex);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to lock mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
                try
                {
                    queue.push(element);
                    
                    if (queue.size() == 1)
                    {
                        rc = pthread_cond_signal(&cond);
                        if (rc) {
                            ostringstream msg;
                            msg << "Failed to broadcast condition variable, errno: " << errno;
                            throwException(TuscanyRuntimeException, msg.str().c_str());
                        }
                    }
    
                }
                catch(...)
                {
                    pthread_mutex_unlock(&mutex);
                    throw;
                }
                rc = pthread_mutex_unlock(&mutex);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to unlock mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }    
        
            void* Queue::dequeue()
            {
                logentry();
                
                void* element = NULL;
                
#if defined(WIN32)  || defined (_WINDOWS)

                EnterCriticalSection(&section);
                try
                {
                    while (queue.size() == 0) {
                        LeaveCriticalSection(&section);
                        WaitForSingleObject(hevent, INFINITE);
                        EnterCriticalSection(&section);
                    }
                    
                    element = queue.front();
                    queue.pop();

                    if (queue.size() == 0)
                    {
                        ResetEvent(hevent);
                    }
                }
                catch(...)
                {
                    LeaveCriticalSection(&section);
                    throw;
                }
                LeaveCriticalSection(&section);
                
#else
                int rc = pthread_mutex_lock(&mutex);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to lock mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
                try
                {
                    while (queue.size() == 0) {
                        rc = pthread_cond_wait(&cond, &mutex);
                        if (rc) {
                            ostringstream msg;
                            msg << "Failed to wait for condition variable, errno: " << errno;
                            throwException(TuscanyRuntimeException, msg.str().c_str());
                        }
                    }
                    
                    element = queue.front();
                    queue.pop();
                }
                catch(...)
                {
                    pthread_mutex_unlock(&mutex);
                    throw;
                }
                rc = pthread_mutex_unlock(&mutex);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to unlock mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
                
#endif             

                return element;
            }    
        
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany
