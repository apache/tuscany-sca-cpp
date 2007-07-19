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

#include <errno.h>

#include <sstream>

#include "tuscany/sca/util/Mutex.h"
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
            Mutex::Mutex()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                InitializeCriticalSection(&section);
#else
                int rc = pthread_mutex_init(&mutex, NULL);
                if (rc)
                {
                    ostringstream msg;
                    msg << "Failed to create mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }
            
            Mutex::~Mutex()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                DeleteCriticalSection(&section);
#else
                int rc = pthread_mutex_destroy(&mutex);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to destroy mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }
            
            void Mutex::lock()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                EnterCriticalSection(&section);
#else
                int rc = pthread_mutex_lock(&mutex);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to lock mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }    
        
            void Mutex::unlock()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                LeaveCriticalSection(&section);
#else
                int rc = pthread_mutex_unlock(&mutex);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to unlock mutex, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }    
        
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany
