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
            clean();
            LOGEXIT(1,"Operation::destructor");
        }

        // ==========
        // Copy Constructor
        // ==========
        Operation::Operation(const Operation& op)
        {
            LOGENTRY(1,"Operation::copy contructor");
          	copy(op);
            LOGEXIT(1,"Operation::copy contructor");
        }

        // ==========
        // operator=
        // ==========
        Operation& Operation::operator=(const Operation& op)
        {
            LOGENTRY(1,"Operation::operator=");
            if (&op != this)
            {
            	copy(op);
            }
            LOGEXIT(1,"Operation::operator=");
            return *this;
        }
    
        // ==========
        // Clean 
        // ==========
        void Operation::clean()
        {
            LOGENTRY(1,"Operation::clean");
            for (unsigned int i = 0; i < getNParms(); i++)
            {
            	const Parameter& parm = getParameter(i);
            	if (parm.getType() == DATAOBJECT)
            	{
            		delete (DataObjectPtr*)parm.getValue();
            	}
            }
            
            parameters.empty();
            
            if (getReturnType() == DATAOBJECT)
            {
            	delete (DataObjectPtr*)getReturnValue();
            }
          	
            LOGEXIT(1,"Operation::clean");
        }
    
        // ==========
        // Copy 
        // ==========
        void Operation::copy(const Operation& op)
        {
            LOGENTRY(1,"Operation::copy");
            clean();
            for (unsigned int i = 0; i < op.getNParms(); i++)
            {
            	const Parameter& parm = op.getParameter(i);
            	if (parm.getType() == DATAOBJECT)
            	{
            		addParameter((const DataObject*)parm.getValue());
            	}
            	else
            	{
            		parameters.insert(parameters.end(), parm);
            	}          	
            }
            
            if (getReturnType() == DATAOBJECT)
            {
            	setReturnValue((const DataObjectPtr*)op.getReturnValue());
            }
          	
            LOGEXIT(1,"Operation::copy");
        }

        // ==============================================
        // getParameter: return parameter at position pos
        // ==============================================
        void* Operation::getParameterValue(unsigned int pos) const
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
        const Operation::Parameter& Operation::getParameter(unsigned int pos) const
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
        Operation::ParameterType Operation::getParameterType(unsigned int pos) const
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

        void Operation::addParameter(const int *parm)
        {
            LOGINFO(4, "Operation::addParameter(int)");
            parameters.insert(parameters.end(), Parameter((void*)parm, INT));
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

        void Operation::addParameter(const unsigned int *parm)
        {
            LOGINFO(4, "Operation::addParameter(unsigned int)");
            parameters.insert(parameters.end(), Parameter((void*)parm, UINT));
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
            parameters.insert(parameters.end(), Parameter((void*)new DataObjectPtr(*parm), DATAOBJECT));
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

        void Operation::setReturnValue(const int *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(int)");
            returnValue = Parameter((void*)parm, INT);
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

        void Operation::setReturnValue(const unsigned int *parm)
        {
            LOGINFO(4, "Operation::setReturnValue(unsigned int)");
            returnValue = Parameter((void*)parm, UINT);
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
            returnValue = Parameter((void*)new DataObjectPtr(*parm), DATAOBJECT);
        }

 
    } // End namespace sca
} // End namespace tuscany
