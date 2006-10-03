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

#ifndef tuscany_sca_util_logwriter_h
#define tuscany_sca_util_logwriter_h

#include "tuscany/sca/export.h"

namespace tuscany
{
    namespace sca
    {
        /**
         * Abstract class for extending logging to other destinations.
         */
        class SCA_API LogWriter 
        {
        public:
            virtual ~LogWriter();
            
            /**
             * Log a message.
             * @param level The level of logging for this message.
             * @param msg The message to log.
             */
            virtual void log(int level, const char* msg) = 0;
        };
        
    } // End namespace sca
} // End namespace tuscany
#endif // tuscany_sca_util_logwriter_h
