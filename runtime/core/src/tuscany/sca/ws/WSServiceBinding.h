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

#ifndef tuscany_sca_ws_wsservicebinding_h
#define tuscany_sca_ws_wsservicebinding_h

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
                 * @param port The definition of the port to which the entrypoint
                 * or external service is to be bound. This is of the form
                 * "namespace"#endpoint("service"/"port")
                 */
                WSServiceBinding(model::Service* service, const string& uri, const string& port);  

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
                 * Return the part of the port definition describing the wsdl 
                 * namespace.
                 * @return The wsdl namespace.
                 */
                string getWSDLNamespaceURL() const { return wsdlNamespaceURL; };

                /**
                 * Return the service part of the port definition.
                 * @return The service to use.
                 */
                string getServiceName() const { return serviceName; };

                /**
                 * Return the port name part of the port definition.
                 * @return The port name to use.
                 */
                string getPortName() const { return portName; };
                
            private:
            
                /**
                 * Parse the port specification.
                 */
                void parsePort();
            
                /**
                 * The full port string.
                 */
                string port;

                /**
                 * Namespace from the port.
                 */
                string wsdlNamespaceURL;

                /**
                 * Service name from the port.
                 */
                string serviceName;

                /**
                 * Port name from the port.
                 */
                string portName;

                /**
                 * The wrapper for the service configured by this binding.
                 */            
                ServiceWrapper* serviceWrapper;
            
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_ws_wsservicebinding_h
