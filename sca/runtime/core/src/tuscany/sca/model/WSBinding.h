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

#ifndef tuscany_sca_model_wsbinding_h
#define tuscany_sca_model_wsbinding_h

#include "tuscany/sca/model/Binding.h"

#include <string>

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Information about a web service binding for an entry point or an
             * external service.
             */
            class WSBinding : public Binding
            {    
            public:
                /**
                 * Constructor.
                 * @param uri The uri of the binding.
                 * @param port The definition of the port to which the entrypoint
                 * or external service is to be bound. This is of the form
                 * "namespace"#endpoint("service"/"port")
                 */
                WSBinding(const string&uri, const string& port);  

                /**
                 * Destructor.
                 */
                virtual ~WSBinding();
                            
                /**
                 * Return the type of the binding.
                 * @return Always returns WS.
                 */
                virtual Type getBindingType() {return WS;};

                /**
                 * Return the part of the port definition describing the wsdl 
                 * namespace.
                 * @return The wsdl namespace.
                 */
                string getWSDLNamespaceURL() {return wsdlNamespaceURL;};

                /**
                 * Return the service part of the port definition.
                 * @return The service to use.
                 */
                string getServiceName() {return serviceName;};

                /**
                 * Return the port name part of the port definition.
                 * @return The port name to use.
                 */
                string getPortName() {return portName;};
                
            private:
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
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wsbinding_h

