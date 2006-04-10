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
        Operation::Operation(const char* operationName)
        {
            LOGENTRY(1,"Operation::constructor");

            if (operationName != 0)
            {
                name = operationName;
            }
                        
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
        void* Operation::getParameterValue(unsigned int pos)
        {
            if (pos < parameters.size())
            {
                return parameters[pos].getValue();
            }

            return 0;
        }

        // ==============================================
        // getParameterType: return type of parameter
        // ==============================================
        Operation::ParameterType Operation::getParameterType(unsigned int pos)
        {
            if (pos < parameters.size())
            {
                return parameters[pos].getType();
            }

            return VOID_TYPE;
        }

        // ===========================================
        // addParameter: set parameter at position pos
        // ===========================================
        void Operation::addParameter(const void *parm)
        {
            LOGENTRY(1, "Operation::addParameter(void*)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, VOID_TYPE));
         }

        void Operation::addParameter(const bool *parm)
        {
            LOGENTRY(1, "Operation::addParameter(bool)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, BOOL));
        }

        void Operation::addParameter(const short *parm)
        {
            LOGENTRY(1, "Operation::addParameter(short)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, SHORT));
        }

        void Operation::addParameter(const long *parm)
        {
            LOGENTRY(1, "Operation::addParameter(long)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, LONG));
        }

        void Operation::addParameter(const unsigned short *parm)
        {
            LOGENTRY(1, "Operation::addParameter(unsigned short)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, USHORT));
        }

        void Operation::addParameter(const unsigned long *parm)
        {
            LOGENTRY(1, "Operation::addParameter(unsigned long)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, ULONG));
        }

       void Operation::addParameter(const float *parm)
        {
            LOGENTRY(1, "Operation::addParameter(float)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, FLOAT));
        }

       void Operation::addParameter(const double *parm)
        {
            LOGENTRY(1, "Operation::addParameter(double)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, DOUBLE));
        }

       void Operation::addParameter(const long double *parm)
        {
            LOGENTRY(1, "Operation::addParameter(long double)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, LONGDOUBLE));
        }

        void Operation::addParameter(const char* *parm)
        {
            LOGENTRY(1, "Operation::addParameter(char*)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, CHARS));
        }

        void Operation::addParameter(const string *parm)
        {
            LOGENTRY(1, "Operation::addParameter(string)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, STRING));
        }

        void Operation::addParameter(const DataObjectPtr *parm)
        {
            LOGENTRY(1, "Operation::addParameter(DataObjectPtr)");
            parameters.insert(parameters.begin(), Parameter((void*)parm, DATAOBJECT));
        }

        // ====================================
        // setReturnValue: set the return value
        // ====================================
        void Operation::setReturnValue(void* val)
        {
            returnValue = val;
        }

        Operation::Parameter::Parameter(void* val, Operation::ParameterType typ)
            : value(val), type(type)
        {
        }
 
    } // End namespace sca
} // End namespace tuscany
