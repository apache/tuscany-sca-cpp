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

/* $Rev$ $Date: 2005/12/22 11:33:21 $ */

#include "tuscany/sca/core/Operation.h"
#include <stdarg.h>

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"



namespace tuscany
{
    namespace sca
    {
        // ===========
        // Constructor
        // ===========
        Operation::Operation(const char* operationName, unsigned int numParameters)
            : name(operationName), nparms(numParameters)
        {
            LOGENTRY(1,"Operation::constructor");

            parameters = new void*[nparms];
            
            LOGEXIT(1,"Operation::constructor");
        }

        // ==========
        // Destructor
        // ==========
        Operation::~Operation()
        {
            LOGENTRY(1,"Operation::destructor");
            LOGEXIT(1,"Operation::destructor");
        }
    

        // ==============================================
        // getParameter: return parameter at position pos
        // ==============================================
        void* Operation::getParameter(unsigned int pos)
        {
            if (pos < nparms)
            {
                return parameters[pos];
            }

            return 0;
        }

        // ===========================================
        // setParameter: set parameter at position pos
        // ===========================================
        void Operation::setParameter(unsigned int pos, void* parm)
        {
            if (pos < nparms)
            {
                parameters[pos] = parm;
            }
        }

        // ====================================
        // setReturnValue: set the return value
        // ====================================
        void Operation::setReturnValue(void* val)
        {
            returnType = val;
        }
        
    } // End namespace sca
} // End namespace tuscany
