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
        // getParameter: return of parameter
        // ==============================================
        Operation::Parameter& Operation::getParameter(unsigned int pos)
        {
            if (pos < parameters.size())
            {
                return parameters[pos];
            }
            
            throw "index out of range";
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
            LOGINFO(4, "Operation::addParameter(void*)");
            parameters.insert(parameters.end(), Parameter((void*)parm, VOID_TYPE));
         }

        void Operation::addParameter(const bool *parm)
        {
            LOGINFO(4, "Operation::addParameter(bool)");
            parameters.insert(parameters.end(), Parameter((void*)parm, BOOL));
        }

        void Operation::addParameter(const short *parm)
        {
            LOGINFO(4, "Operation::addParameter(short)");
            parameters.insert(parameters.end(), Parameter((void*)parm, SHORT));
        }

        void Operation::addParameter(const long *parm)
        {
            LOGINFO(4, "Operation::addParameter(long)");
            parameters.insert(parameters.end(), Parameter((void*)parm, LONG));
        }

        void Operation::addParameter(const unsigned short *parm)
        {
            LOGINFO(4, "Operation::addParameter(unsigned short)");
            parameters.insert(parameters.end(), Parameter((void*)parm, USHORT));
        }

        void Operation::addParameter(const unsigned long *parm)
        {
            LOGINFO(4, "Operation::addParameter(unsigned long)");
            parameters.insert(parameters.end(), Parameter((void*)parm, ULONG));
        }

       void Operation::addParameter(const float *parm)
        {
            LOGINFO(4, "Operation::addParameter(float)");
            parameters.insert(parameters.end(), Parameter((void*)parm, FLOAT));
        }

       void Operation::addParameter(const double *parm)
        {
            LOGINFO(4, "Operation::addParameter(double)");
            parameters.insert(parameters.end(), Parameter((void*)parm, DOUBLE));
        }

       void Operation::addParameter(const long double *parm)
        {
            LOGINFO(4, "Operation::addParameter(long double)");
            parameters.insert(parameters.end(), Parameter((void*)parm, LONGDOUBLE));
        }

        void Operation::addParameter(const char* *parm)
        {
            LOGINFO(4, "Operation::addParameter(char*)");
            parameters.insert(parameters.end(), Parameter((void*)parm, CHARS));
        }

        void Operation::addParameter(const char *parm)
        {
            LOGINFO(4, "Operation::addParameter(char)");
            parameters.insert(parameters.end(), Parameter((void*)parm, CHAR));
        }

        void Operation::addParameter(const string *parm)
        {
            LOGINFO(4, "Operation::addParameter(string)");
            parameters.insert(parameters.end(), Parameter((void*)parm, STRING));
        }

        void Operation::addParameter(const DataObjectPtr *parm)
        {
            LOGINFO(4, "Operation::addParameter(DataObjectPtr)");
            parameters.insert(parameters.end(), Parameter((void*)parm, DATAOBJECT));
        }

        Operation::Parameter::Parameter(void* val, Operation::ParameterType typ)
            : value(val), type(typ)
        {
        }

        // ===========================================
        // setReturnValue
        // ===========================================
        void Operation::setReturnValue(const void *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(void*)");
            returnValue = Parameter((void*)parm, VOID_TYPE);
        }

        void Operation::setReturnValue(const bool *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(bool*)");
            returnValue = Parameter((void*)parm, BOOL);
        }

        void Operation::setReturnValue(const short *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(short*)");
            returnValue = Parameter((void*)parm, SHORT);
        }

        void Operation::setReturnValue(const long *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(long*)");
            returnValue = Parameter((void*)parm, LONG);
        }

        void Operation::setReturnValue(const unsigned short *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(unsigned short*)");
            returnValue = Parameter((void*)parm, USHORT);
        }

        void Operation::setReturnValue(const unsigned long *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(unsigned long*)");
            returnValue = Parameter((void*)parm, ULONG);
        }

       void Operation::setReturnValue(const float *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(float*)");
            returnValue = Parameter((void*)parm, FLOAT);
        }

       void Operation::setReturnValue(const double *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(double*)");
            returnValue = Parameter((void*)parm, DOUBLE);
        }

       void Operation::setReturnValue(const long double *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(long double*)");
            returnValue = Parameter((void*)parm, LONGDOUBLE);
        }

        void Operation::setReturnValue(const char *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(char)");
            returnValue = Parameter((void*)parm, CHAR);
        }

        void Operation::setReturnValue(const char* *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(char*)");
            returnValue = Parameter((void*)parm, CHARS);
        }

        void Operation::setReturnValue(const string *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(string*)");
            returnValue = Parameter((void*)parm, STRING);
        }

        void Operation::setReturnValue(const DataObjectPtr *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(DataObjectPtr*)");
            returnValue = Parameter((void*)parm, DATAOBJECT);
        }

 
    } // End namespace sca
} // End namespace tuscany
