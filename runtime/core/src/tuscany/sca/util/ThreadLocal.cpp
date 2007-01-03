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

#if defined(WIN32)  || defined (_WINDOWS)
#pragma warning(disable: 4786)
#else
#include "tuscany_sca_config.h"
#endif

#include <errno.h>

#include <sstream>

#include "tuscany/sca/util/ThreadLocal.h"
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
            ThreadLocal::ThreadLocal()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
#else
                int rc = pthread_key_create(&key, NULL);
                if (rc)
                {
                    ostringstream msg;
                    msg << "Failed to create thread local key, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }
            
            ThreadLocal::~ThreadLocal()
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
#else
                int rc = pthread_key_delete(key);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to destroy thread local key, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }
            
            void ThreadLocal::setValue(const void* value)
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
#else
                int rc = pthread_setspecific(key, value);
                if (rc) {
                    ostringstream msg;
                    msg << "Failed to set thread local value, errno: " << errno;
                    throwException(TuscanyRuntimeException, msg.str().c_str());
                }
#endif             
            }    
        
            void* ThreadLocal::getValue() const
            {
                logentry();
#if defined(WIN32)  || defined (_WINDOWS)
#else
                return pthread_getspecific(key);
#endif             
            }    
        
        } // End namespace util
    } // End namespace sca
} // End namespace tuscany
