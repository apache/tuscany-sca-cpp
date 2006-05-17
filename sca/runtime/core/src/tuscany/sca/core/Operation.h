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

#ifndef tuscany_sca_core_operation_h
#define tuscany_sca_core_operation_h
#include "osoa/sca/export.h"
#include "commonj/sdo/SDO.h"
using commonj::sdo::DataObjectPtr;
#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        /**
         * Holds the details of a single invocation of a business method.
         * This class is used to pass the parameters and operation name from the
         * client to a service. It will also hold the return value on the 
         * return from the business method.
         */
        class Operation 
        {
        public:
            /**
             * Create a new operation.
             * @param operationName The method name of the business method to be invoked.
             * @param numParameters The number of parameters to be passed.
             */
            SCA_API Operation(const char* operationName = 0);

            /**
             * Destructor.
             */
            SCA_API virtual    ~Operation();

            /**
             * Return the operation name.
             * @return The name of the operation.
             */
            SCA_API const string& getName() {return name;}


            enum ParameterType
            {
            	UNSET = 0,
                VOID_TYPE,
                BOOL,
                SHORT,
                LONG,
                USHORT,
                ULONG,
                FLOAT,
                DOUBLE,
                LONGDOUBLE,
                CHARS,
                STRING,
                DATAOBJECT
            };

            class Parameter
            {
                public:
                    SCA_API Parameter(void* value = NULL, ParameterType type = VOID_TYPE);
                    SCA_API void* getValue() {return value;}
                    SCA_API ParameterType getType() {return type;}

                private:
                    void* value;
                    ParameterType type;
            };

            /**
             * Set a return value for the operation.
             * @param retVal Pointer to the return value.
             */
            SCA_API void setReturnValue(const void *retVal);
            SCA_API void setReturnValue(const bool *retVal);
            SCA_API void setReturnValue(const short *retVal);
            SCA_API void setReturnValue(const long *retVal);
            SCA_API void setReturnValue(const unsigned short *retVal);
            SCA_API void setReturnValue(const unsigned long *retVal);
            SCA_API void setReturnValue(const float *retVal);
            SCA_API void setReturnValue(const double *retVal);
            SCA_API void setReturnValue(const long double *retVal);
            SCA_API void setReturnValue(const char* *retVal);
            SCA_API void setReturnValue(const string *retVal);
            SCA_API void setReturnValue(const DataObjectPtr *retVal);

            /**
             * Set a parameter on the operation.
             * @param pos The position of the parameter in the parameter list.
             * @param parm Pointer to the parameter to be passed.
             */
            SCA_API void addParameter(const void *parm);
            SCA_API void addParameter(const bool *parm);
            SCA_API void addParameter(const short *parm);
            SCA_API void addParameter(const long *parm);
            SCA_API void addParameter(const unsigned short *parm);
            SCA_API void addParameter(const unsigned long *parm);
            SCA_API void addParameter(const float *parm);
            SCA_API void addParameter(const double *parm);
            SCA_API void addParameter(const long double *parm);
            SCA_API void addParameter(const char* *parm);
            SCA_API void addParameter(const string *parm);
            SCA_API void addParameter(const DataObjectPtr *parm);
            
            SCA_API unsigned int getNParms() {return parameters.size();}

            /**
             * Get a parameter from the operation.
             * @param pos The position of the parameter in the parameter list.
             * @return Pointer to the paramter at the given postion. Should be
             * cast to the appropriate type.
             */
            SCA_API Parameter& getParameter(unsigned int pos);

            /**
             * Get a parameter type from the operation.
             * @param pos The position of the parameter in the parameter list.
             * @return Pointer to the paramter at the given postion. Should be
             * cast to the appropriate type.
             */
            SCA_API ParameterType getParameterType(unsigned int pos);

            /**
             * Get a parameter from the operation.
             * @param pos The position of the parameter in the parameter list.
             * @return Pointer to the paramter at the given postion. Should be
             * cast to the appropriate type.
             */
            SCA_API void* getParameterValue(unsigned int pos);

            SCA_API ParameterType getReturnType() {return returnValue.getType();}
            SCA_API void* getReturnValue() {return returnValue.getValue();}

        private:
            /**
             * Operation name (method name).
             */ 
            string name;

            /**
             * Array of parameters.
             */
            typedef vector<Parameter> PARAMETER_VECTOR;
            
            PARAMETER_VECTOR parameters;

            Parameter returnValue;

        };
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_core_operation_h
