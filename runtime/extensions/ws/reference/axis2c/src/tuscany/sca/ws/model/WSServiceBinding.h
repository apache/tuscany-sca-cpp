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

#ifndef tuscany_sca_extension_ws_model_wsservicebinding_h
#define tuscany_sca_extension_ws_model_wsservicebinding_h

#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/model/Service.h"

#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
            /**
             * Information about a web service binding for service or a reference.
             */
            class WSServiceBinding : public model::ServiceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 * @param endpoint The definition of the endpoint to which the service
                 * or reference is to be bound. This is of the form
                 * "namespace"#endpoint("service"/"endpoint")
                 */
                WSServiceBinding(model::Service* service, const string& uri, const string& endpoint, const string& version);  

                /**
                 * Destructor.
                 */
                virtual ~WSServiceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual string getType() { return "http://www.osoa.org/xmlns/sca/1.0#WebServiceBinding"; };
                            
                /**
                 * Create a wrapper for the service configured by this
                 * binding.
                 */
                 virtual ServiceWrapper* getServiceWrapper();
                                
                /**
                 * Return the part of the endpoint definition describing the wsdl 
                 * namespace.
                 * @return The wsdl namespace.
                 */
                string getWSDLNamespaceURL() const { return wsdlNamespaceURL; };

                /**
                 * Return the service part of the endpoint definition.
                 * @return The service to use.
                 */
                string getServiceName() const { return serviceName; };

                /**
                 * Return the endpoint name part of the endpoint definition.
                 * @return The endpoint name to use.
                 */
                string getEndpointName() const { return endpointName; };
                
                /**
                 * Return the SOAP version.
                 * @return The SOAP version to use.
                 */
                string getSOAPVersion() const { return soapVersion; };
                
            private:
            
                /**
                 * Parse the endpoint specification.
                 */
                void parseEndpoint();
            
                /**
                 * The full endpoint string.
                 */
                string endpoint;

                /**
                 * Namespace from the endpoint.
                 */
                string wsdlNamespaceURL;

                /**
                 * Service name from the endpoint.
                 */
                string serviceName;

                /**
                 * WSDL Endpoint name.
                 */
                string endpointName;

                /**
                 * SOAP version.
                 */
                string soapVersion;

                /**
                 * The wrapper for the service configured by this binding.
                 */            
                ServiceWrapper* serviceWrapper;
            
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_ws_model_wsservicebinding_h
