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
#include <process.h>
#endif

#include <errno.h>

#include <sstream>

#include "tuscany/sca/util/Thread.h"
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

#if defined(WIN32)  || defined (_WINDOWS)
            unsigned int __stdcall runThread(void *args)
            {
                Thread* thread = (Thread*)args;
                thread->run();
                return 0;
            }
#else
            void* runThread(void* args)
            {
                Thread* thread = (Thread*)args;
                thread->run();
                return NULL;
            }
#endif             

            Thread::Thread()
#if defined(WIN32)  || defined (_WINDOWS)
                : hthread(0)
#else
#endif             
            {
                logentry();
            }
            
            Thread::~Thread()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                if (hthread != 0)
                {
                    CloseHandle(hthread);
                }
#else
#endif             
            }
            
            void Thread::start()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
                hthread = (HANDLE)_beginthreadex(NULL, 0, runThread, this, 0, NULL); 
                if (hthread == 0)
                {
                    ostringstream msg;
                    msg << "Failed to create thread, errno: " << __doserrno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#else
                int rc =pthread_create(&thread, NULL, runThread, this);
                if (rc)
                {
                    ostringstream msg;
                    msg << "Failed to create thread, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }    
        
            void Thread::join()
            {
                logentry();
                
#if defined(WIN32)  || defined (_WINDOWS)
                WaitForSingleObject(hthread, INFINITE);
#else
                int rc =pthread_join(thread, NULL);
                if (rc)
                {
                    ostringstream msg;
                    msg << "Failed to join thread, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }    
        
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany
