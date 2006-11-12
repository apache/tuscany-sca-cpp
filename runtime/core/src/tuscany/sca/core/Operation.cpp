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
#include "tuscany/sca/util/Exceptions.h"
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
            logentry();
            if (operationName != 0)
            {
                name = operationName;
            }
        }

        // ==========
        // Destructor
        // ==========
        Operation::~Operation()
        {
            logentry();
            clean();
        }

        // ==========
        // Copy Constructor
        // ==========
        Operation::Operation(const Operation& op)
        {
            logentry();
          	copy(op);
        }

        // ==========
        // operator=
        // ==========
        Operation& Operation::operator=(const Operation& op)
        {
            logentry();
            if (&op != this)
            {
            	copy(op);
            }
            return *this;
        }
    
        // ==========
        // Clean 
        // ==========
        void Operation::clean()
        {
            logentry();
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
        }
    
        // ==========
        // Copy 
        // ==========
        void Operation::copy(const Operation& op)
        {
            logentry();
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
            else
            {
            	returnValue = op.returnValue;
            }
        }

        // ==============================================
        // getParameter: return parameter at position pos
        // ==============================================
        void* Operation::getParameterValue(unsigned int pos) const
        {
            logentry();
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
            logentry();
            if (pos < parameters.size())
            {
                return parameters[pos];
            }
            
            throwException(ServiceInvocationException, "Index out of range");
        }


        // ==============================================
        // getParameterType: return type of parameter
        // ==============================================
        Operation::ParameterType Operation::getParameterType(unsigned int pos) const
        {
            logentry();
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
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, VOID_TYPE));
         }

        void Operation::addParameter(const bool *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, BOOL));
        }

        void Operation::addParameter(const short *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, SHORT));
        }

        void Operation::addParameter(const int *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, INT));
        }

        void Operation::addParameter(const long *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, LONG));
        }

        void Operation::addParameter(const unsigned short *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, USHORT));
        }

        void Operation::addParameter(const unsigned int *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, UINT));
        }

        void Operation::addParameter(const unsigned long *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, ULONG));
        }

       void Operation::addParameter(const float *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, FLOAT));
        }

       void Operation::addParameter(const double *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, DOUBLE));
        }

       void Operation::addParameter(const long double *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, LONGDOUBLE));
        }

        void Operation::addParameter(const char* *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, CHARS));
        }

        void Operation::addParameter(const char *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, CHAR));
        }

        void Operation::addParameter(const string *parm)
        {
            logentry();
            parameters.insert(parameters.end(), Parameter((void*)parm, STRING));
        }

        void Operation::addParameter(const DataObjectPtr *parm)
        {
            logentry();
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
            logentry();
            returnValue = Parameter((void*)parm, VOID_TYPE);
        }

        void Operation::setReturnValue(const bool *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, BOOL);
        }

        void Operation::setReturnValue(const short *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, SHORT);
        }

        void Operation::setReturnValue(const int *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, INT);
        }

        void Operation::setReturnValue(const long *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, LONG);
        }

        void Operation::setReturnValue(const unsigned short *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, USHORT);
        }

        void Operation::setReturnValue(const unsigned int *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, UINT);
        }

        void Operation::setReturnValue(const unsigned long *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, ULONG);
        }

       void Operation::setReturnValue(const float *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, FLOAT);
        }

       void Operation::setReturnValue(const double *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, DOUBLE);
        }

       void Operation::setReturnValue(const long double *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, LONGDOUBLE);
        }

        void Operation::setReturnValue(const char *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, CHAR);
        }

        void Operation::setReturnValue(const char* *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, CHARS);
        }

        void Operation::setReturnValue(const string *parm)
        {
            logentry();
            returnValue = Parameter((void*)parm, STRING);
        }

        void Operation::setReturnValue(const DataObjectPtr *parm)
        {
            logentry();
            returnValue = Parameter((void*)new DataObjectPtr(*parm), DATAOBJECT);
        }

 
    } // End namespace sca
} // End namespace tuscany
