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

#include <stdarg.h>
#include <sstream>

#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/core/SCARuntime.h"

using namespace std;
using namespace commonj::sdo;


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

        // ==================================================
        // getParameter: return parameter value based on name
        // ==================================================
        void* Operation::getParameterValue(const string& name) const
        {
            logentry();
            try
            {
                return getParameter(name).getValue();
            }
            catch(ServiceInvocationException)
            {}

            return 0;
        }

        // ======================================================
        // getParameter: return parameter based on index position
        // ======================================================
        const Operation::Parameter& Operation::getParameter(unsigned int pos) const
        {
            logentry();
            if (pos < parameters.size())
            {
                return parameters[pos];
            }
            
            throwException(ServiceInvocationException, "Index out of range");
        }

        // ===============================================
        // getParameter: return of parameter based on name
        // ===============================================
        const Operation::Parameter& Operation::getParameter(const string& name) const
        {
            logentry();
            for(unsigned int pos=0; pos < parameters.size(); pos++)
            {
                if(parameters[pos].getName() == name)
                {
                    return parameters[pos];
                }
            }
            
            throwException(ServiceInvocationException, "Parameter name not found");
        }


        // ============================================================
        // getParameterType: return type of parameter based on position
        // ============================================================
        Operation::ParameterType Operation::getParameterType(unsigned int pos) const
        {
            logentry();
            if (pos < parameters.size())
            {
                return parameters[pos].getType();
            }

            return VOID_TYPE;
        }

        // ========================================================
        // getParameterType: return type of parameter based on name
        // ========================================================
        Operation::ParameterType Operation::getParameterType(const string& name) const
        {
            logentry();
            try
            {
                return getParameter(name).getType();
            }
            catch(ServiceInvocationException)
            {}

            return VOID_TYPE;
        }

        const string emptyString = string();

        // ==============================================
        // getParameterName: return name of parameter
        // ==============================================
        const string& Operation::getParameterName(unsigned int pos) const
        {
            logentry();
            if (pos < parameters.size())
            {
                return parameters[pos].getName();
            }
            
            return emptyString;
        }

        // ===========================================
        // addParameter: set parameter at position pos
        // ===========================================
        void Operation::addParameter(const void *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: void, value: %p", parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, VOID_TYPE));
         }

        void Operation::addParameter(const bool *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: bool, value: %d", (int)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, BOOL));
        }

        void Operation::addParameter(const short *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: short, value: %hd", (short)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, SHORT));
        }

        void Operation::addParameter(const int *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: int, value: %d", (int)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, INT));
        }

        void Operation::addParameter(const long *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: long, value: %ld", (long)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, LONG));
        }

        void Operation::addParameter(const unsigned short *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: unsigned short, value: %hu", (unsigned short)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, USHORT));
        }

        void Operation::addParameter(const unsigned int *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: unsigned int, value: %u", (unsigned int)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, UINT));
        }

        void Operation::addParameter(const unsigned long *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: unsigned long, value: %lu", (unsigned long)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, ULONG));
        }

       void Operation::addParameter(const float *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: float, value: %f", (float)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, FLOAT));
        }

       void Operation::addParameter(const double *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: double, value: %lf", (double)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, DOUBLE));
        }

       void Operation::addParameter(const long double *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: long double, value: %Lf", (long double)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, LONGDOUBLE));
        }

        void Operation::addParameter(const char* *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: char*, value: %s", (const char*)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, CHARS));
        }

        void Operation::addParameter(const char *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: char, value: %d", (int)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, CHAR));
        }

        void Operation::addParameter(const string *parm)
        {
            logentry();
            loginfo("Adding operation parameter, type: string, value: %s", (const char*)(*parm).c_str());
            parameters.insert(parameters.end(), Parameter((void*)parm, STRING));
        }

        void Operation::addParameter(const DataObjectPtr *parm)
        {
            logentry();
            ostringstream os;
            os << *parm;
            loginfo("Adding operation parameter, type: DataObject, value: %s", os.str().c_str());
            parameters.insert(parameters.end(), Parameter((void*)new DataObjectPtr(*parm), DATAOBJECT));
        }

        // =======================================================
        // addParameter: set parameter at position pos with a name
        // =======================================================
        void Operation::addParameter(const string& name, const void *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: void, value: %p", name.c_str(), parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, VOID_TYPE, (string&) name));
         }

        void Operation::addParameter(const string& name, const bool *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: bool, value: %d", name.c_str(), (int)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, BOOL, (string&) name));
        }

        void Operation::addParameter(const string& name, const short *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: short, value: %hd", name.c_str(), (short)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, SHORT, (string&) name));
        }

        void Operation::addParameter(const string& name, const int *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: int, value: %d", name.c_str(), (int)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, INT, (string&) name));
        }

        void Operation::addParameter(const string& name, const long *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: long, value: %ld", name.c_str(), (long)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, LONG, (string&) name));
        }

        void Operation::addParameter(const string& name, const unsigned short *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: unsigned short, value: %hu", (unsigned short)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, USHORT, (string&) name));
        }

        void Operation::addParameter(const string& name, const unsigned int *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: unsigned int, value: %u", name.c_str(), (unsigned int)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, UINT, (string&) name));
        }

        void Operation::addParameter(const string& name, const unsigned long *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: unsigned long, value: %lu", name.c_str(), (unsigned long)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, ULONG, (string&) name));
        }

       void Operation::addParameter(const string& name, const float *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: float, value: %f", name.c_str(), (float)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, FLOAT, (string&) name));
        }

       void Operation::addParameter(const string& name, const double *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: double, value: %lf", name.c_str(), (double)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, DOUBLE, (string&) name));
        }

       void Operation::addParameter(const string& name, const long double *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: long double, value: %Lf", name.c_str(), (long double)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, LONGDOUBLE, (string&) name));
        }

        void Operation::addParameter(const string& name, const char* *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: char*, value: %s", name.c_str(), (const char*)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, CHARS, (string&) name));
        }

        void Operation::addParameter(const string& name, const char *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: char, value: %d", name.c_str(), (int)*parm);
            parameters.insert(parameters.end(), Parameter((void*)parm, CHAR, (string&) name));
        }

        void Operation::addParameter(const string& name, const string *parm)
        {
            logentry();
            loginfo("Adding operation parameter, name: %s, type: string, value: %s", (const char*)(*parm).c_str());
            parameters.insert(parameters.end(), Parameter((void*)parm, STRING, (string&) name));
        }

        void Operation::addParameter(const string& name, const DataObjectPtr *parm)
        {
            logentry();
            ostringstream os;
            os << *parm;
            loginfo("Adding operation parameter, name: %s, type: DataObject, value: %s", name.c_str(), os.str().c_str());
            parameters.insert(parameters.end(), Parameter((void*)new DataObjectPtr(*parm), DATAOBJECT, (string&) name));
        }

        Operation::Parameter::Parameter(void* val, Operation::ParameterType typ, string& nam)
            : value(val), type(typ), name(nam)
        {
        }

        // ===========================================
        // setReturnValue
        // ===========================================
        void Operation::setReturnValue(const void *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: void, value: %p", parm);
            returnValue = Parameter((void*)parm, VOID_TYPE);
        }

        void Operation::setReturnValue(const bool *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: bool, value: %d", (bool)*parm);
            returnValue = Parameter((void*)parm, BOOL);
        }

        void Operation::setReturnValue(const short *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: short, value: %h", (short)*parm);
            returnValue = Parameter((void*)parm, SHORT);
        }

        void Operation::setReturnValue(const int *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: int, value: %d", (int)*parm);
            returnValue = Parameter((void*)parm, INT);
        }

        void Operation::setReturnValue(const long *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: long, value: %l", (long)*parm);
            returnValue = Parameter((void*)parm, LONG);
        }

        void Operation::setReturnValue(const unsigned short *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: unsigned short, value: %hu", (unsigned short)*parm);
            returnValue = Parameter((void*)parm, USHORT);
        }

        void Operation::setReturnValue(const unsigned int *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: unsigned short, value: %u", (unsigned int)*parm);
            returnValue = Parameter((void*)parm, UINT);
        }

        void Operation::setReturnValue(const unsigned long *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: unsigned long, value: %lu", (unsigned long)*parm);
            returnValue = Parameter((void*)parm, ULONG);
        }

       void Operation::setReturnValue(const float *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: float, value: %f", (float)*parm);
            returnValue = Parameter((void*)parm, FLOAT);
        }

       void Operation::setReturnValue(const double *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: double, value: %lf", (double)*parm);
            returnValue = Parameter((void*)parm, DOUBLE);
        }

       void Operation::setReturnValue(const long double *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: long double, value: %Lf", (long double)*parm);
            returnValue = Parameter((void*)parm, LONGDOUBLE);
        }

        void Operation::setReturnValue(const char *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: char, value: %d", (char)*parm);
            returnValue = Parameter((void*)parm, CHAR);
        }

        void Operation::setReturnValue(const char* *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: char*, value: %s", (char*)*parm);
            returnValue = Parameter((void*)parm, CHARS);
        }

        void Operation::setReturnValue(const string *parm)
        {
            logentry();
            loginfo("Setting operation return value, type: string, value: %s", (*parm).c_str());
            returnValue = Parameter((void*)parm, STRING);
        }

        void Operation::setReturnValue(const DataObjectPtr *parm)
        {
            logentry();
            ostringstream os;
            os << *parm;
            loginfo("Setting operation return value, type: DataObject, value: %s", os.str().c_str());
            returnValue = Parameter((void*)new DataObjectPtr(*parm), DATAOBJECT);
        }
 
    } // End namespace sca
} // End namespace tuscany
