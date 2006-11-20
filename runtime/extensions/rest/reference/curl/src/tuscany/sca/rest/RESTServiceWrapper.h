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

#ifndef tuscany_sca_extension_rest_restservicewrapper_h
#define tuscany_sca_extension_rest_restservicewrapper_h

#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/Service.h"
#include "commonj/sdo/SDO.h"

using namespace tuscany::sca::model;

#include <sstream>


namespace tuscany
{
    namespace sca
    {
        
        namespace rest
        {
        
            class RESTServiceWrapper : public ServiceWrapper
            {
            public:

                /**
                 * Constructor.
                 * @param target The service wrapper represents a Web service.
                 */
                RESTServiceWrapper(Service* service);
                
                /**
                 * Destructor
                 */
                virtual    ~RESTServiceWrapper();

                /**
                 * All business method calls on the target service are performed through
                 * this invoke method.
                 * @param operation The details of the method, paramaters and return value for the
                 * business method to be called on the target service.
                 */
                virtual void invoke(Operation& operation);
                
            private:

                /**
                 * Used to track initialization of the CURL library
                 */            
                static bool initialized;
                
                /**
                 * Write a parameter into a URL
                 */
                void writeParameter(commonj::sdo::XMLHelper* xmlHelper, std::ostringstream& os, const Operation::Parameter& parm);
                
                /**
                 * Set the return value on the given operation
                 */
                void setReturn(commonj::sdo::XMLHelper* xmlHelper, string& payload, Operation& operation);
                
            };
        } // End namespace rest
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_rest_restservicewrapper_h
