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

#ifndef tuscany_sca_model_wsdldefinition_h
#define tuscany_sca_model_wsdldefinition_h

#include <string>
#include <map>
#include <vector>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/export.h"
#include "tuscany/sca/model/WSDLOperation.h"


namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            class WSDLOperation;

            /**
             * Holds information about a WSDL definition loaded into the runtime.
             */ 
            class WSDLDefinition
            {
            public:
                /**
                 * Constructor.
                 * @param wsdlModel The data object representing the WSDL document
                 * defining a web service.
                 */
                 SCA_API WSDLDefinition(commonj::sdo::DataObjectPtr wsdlModel);

                /**
                 * Destructor.
                 */ 
                SCA_API virtual ~WSDLDefinition();

                /**
                 * Returns the target namespace of the WSDL definitions.
                 * @return The target namespace.
                 */
                SCA_API std::string getNamespace(void);

                /**
                 * Add a WSDL model.
                 */
                SCA_API void addWSDLModel(commonj::sdo::DataObjectPtr wsdlModel);

                /**
                 * Find an operation in the WSDL definitions.
                 * @param serviceName The name of the service on which this
                 * operation is defined.
                 * @param portName The name of the port in the service to
                 * use.
                 * @param operationName The name of the operation to find.
                 * @return The operation if found. Exception thrown if not found.
                 */
                SCA_API const WSDLOperation& findOperation(const std::string& serviceName, 
                                        const std::string& portName, 
                                        const std::string& operationName);
                
                /**
                 * Find an operation in the WSDL definitions.
                 * @param portTypeName The name of the portType on which this
                 * operation is defined.
                 * @param operationName The name of the operation to find.
                 * @return The operation if found. Exception thrown if not found.
                 */
                SCA_API const WSDLOperation& findOperation(const std::string& portTypeName, 
                                        const std::string& operationName);

                /**
                 * Get all of the PortTypes in this WSDL definition.
                 * @return A standard list of standard strings, each being a different PortType
                 */
                SCA_API std::list<std::string> getPortTypes();

                /**
                 * Get all of the Operations for a particular PortType in this WSDL definition.
                 * @param portTypeName Get all of the operations for this portTypeName
                 * @return A standard list of standard strings, each of which is a different operation
                 */
                SCA_API std::list<std::string> getOperations( const std::string &portTypeName );
                
            private:

                /**
                 * Find a service in the wsdl definition.
                 * @param serviceName The name of the service.
                 * @return A data object describing the service if found, otherwise
                 * a 0 if not found.
                 */
                commonj::sdo::DataObjectPtr findService(const std::string& serviceName);

                /**
                 * Find a binding in the wsdl definition.
                 * @param bindingName The name of the binding to find.
                 * @return A data object describing the binding if found, otherwise
                 * a 0 if not found.
                 */
                commonj::sdo::DataObjectPtr findBinding(const std::string& bindingName);

                /**
                 * Find a portType in the wsdl definition.
                 * @param portTypeName The name of the portType.
                 * @return A data object describing the portType if found, otherwise
                 * a 0 if not found.
                 */
                commonj::sdo::DataObjectPtr findPortType(const std::string& portTypeName);

                /**
                 * Find a message in the wsdl definition.
                 * @param messageName The name of the message.
                 * @return A data object describing the message if found, otherwise
                 * a 0 if not found.
                 */
                commonj::sdo::DataObjectPtr findMessage(const std::string& messageName);

                /**
                 * Given a wsdl represented by an SDO, traverse the SDO and
                 * extract all of the necessary information to populate the
                 * operationMap with WSDLOperation objects.
                 * This method is called each time a wsdl model is added: so
                 * that would be be in the constructor and in addWSDLModel().
                 * @param wsdlModel A wsdl represented by an SDO 
                 */
                void mapOperations( commonj::sdo::DataObjectPtr wsdlModel );

                /**
                 * The data object representation of the WSDL document.
                 */
                typedef std::vector<commonj::sdo::DataObjectPtr> MODEL_VECTOR;
                MODEL_VECTOR wsdlModels;

                typedef std::map<std::string, WSDLOperation> OPERATION_MAP;
                typedef std::map<std::string, OPERATION_MAP> STR_OPERATION_MAP;
                STR_OPERATION_MAP portTypeMap;
                STR_OPERATION_MAP servicePortMap;
            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wsdldefinition_h

