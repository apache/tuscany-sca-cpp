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

#ifndef tuscany_sca_extension_ws_model_wsreferencebinding_h
#define tuscany_sca_extension_ws_model_wsreferencebinding_h

#include "tuscany/sca/model/ReferenceBinding.h"

using namespace tuscany::sca::model;

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
            class WSReferenceBinding : public ReferenceBinding
            {    
            public:

                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 * @param endpoint The definition of the endpoint to which the entrypoint
                 * or external service is to be bound. This is of the form
                 * "namespace"#endpoint("service"/"endpoint")
                 */
                WSReferenceBinding(Reference* reference, const string&uri, const string& endpoint, const string& version);  

                /**
                 * Destructor.
                 */
                virtual ~WSReferenceBinding();
                            
                /**
                 * Returns the type of binding.
                 */                
                virtual string getType() { return "http://www.osoa.org/xmlns/sca/1.0#WebServiceBinding"; };
                            
                 /**
                  * Configure this binding from a service binding.
                  */
                  virtual void configure(ServiceBinding* serviceBinding);
                                
                /**
                 * Create a proxy representing the reference to the
                 * client component.
                 */
                 virtual ServiceProxy* getServiceProxy();
                                
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
                
                 /**
                  * Returns the target service binding.
                  */
                  ServiceBinding* getTargetServiceBinding() const { return targetServiceBinding; };
                                
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
                 * Endpoint name from the endpoint.
                 */
                string endpointName;

                /**
                 * SOAP version.
                 */
                string soapVersion;

                /**
                 * The proxy representing the reference to the client
                 * component.
                 */
                ServiceProxy* serviceProxy;
                
                /**
                 * The service binding of the target
                 */
                ServiceBinding* targetServiceBinding; 
            };
            
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_ws_model_wsreferencebinding_h
