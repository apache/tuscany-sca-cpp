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

#ifndef tuscany_sca_model_wsdl_h
#define tuscany_sca_model_wsdl_h

#include "osoa/sca/export.h"
#include <string>
using std::string;

#include "tuscany/sca/model/WsdlOperation.h"

#include "commonj/sdo/SDO.h"
using commonj::sdo::DataObjectPtr;

#include <map>
using std::map;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {

            /**
             * Holds information about a WSDL definition loaded into the runtime.
             */ 
            class Wsdl
            {
            public:
                /**
                 * Constructor.
                 * @param wsdlModel The data object representing the WSDL document
                 * defining a web service.
                 */
                 Wsdl(DataObjectPtr wsdlModel);

                /**
                 * Destructor.
                 */ 
                ~Wsdl();


                /**
                 * Returns the target namespace of the WSDL definitions.
                 * @return The target namespace.
                 */
                string getNamespace(void);

                /**
                 * Find an operation in the WSDL definitions.
                 * @param serviceName The name of the service on which this
                 * operation is defined.
                 * @param portName The name of the port in the service to
                 * use.
                 * @param operationName The name of the operation to find.
                 * @return The operation if found. Exception thrown if not found.
                 */
                const WsdlOperation& findOperation(const string& serviceName, 
                                        const string& portName, 
                                        const string& operationName);
                
            private:

                /**
                 * Find a service in the wsdl definition.
                 * @param serviceName The name of the service.
                 * @return A data object describing the service if found, otherwise
                 * a 0 if not found.
                 */
                DataObjectPtr findService(const string& serviceName);

                /**
                 * Find a binding in the wsdl definition.
                 * @param bindingName The name of the binding to find.
                 * @return A data object describing the binding if found, otherwise
                 * a 0 if not found.
                 */
                DataObjectPtr findBinding(const string& bindingName);

                /**
                 * Find a portType in the wsdl definition.
                 * @param portTypeName The name of the portType.
                 * @return A data object describing the portType if found, otherwise
                 * a 0 if not found.
                 */
                DataObjectPtr findPortType(const string& portTypeName);

                /**
                 * Find a message in the wsdl definition.
                 * @param messageName The name of the message.
                 * @return A data object describing the message if found, otherwise
                 * a 0 if not found.
                 */
                DataObjectPtr findMessage(const string& messageName);


                /**
                 * The data object representation of the WSDL document.
                 */
                DataObjectPtr wsdlModel;
                
                typedef map<string, WsdlOperation> OperationMap;
                OperationMap operationMap;
            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wsdl_h

