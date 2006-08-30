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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/ws/model/WSReferenceBinding.h"
#include "tuscany/sca/core/ServiceProxy.h"
#include "tuscany/sca/ws/WSServiceProxy.h"

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {

            // Constructor
            WSReferenceBinding::WSReferenceBinding(Reference* reference, const string& uri, const string& port)
                : ReferenceBinding(reference, uri), port(port)
            {
                parsePort();
            }
            
            void WSReferenceBinding::parsePort()
            {
                // Port is of the form: <wsdl-namepace-uri>#wsdl.endpoint(<service-name>/<port-name>)
                string::size_type hash = port.find("#");
                if (hash != string::npos)
                {
                    // Found a hash

                    // Namepace is the part before the #
                    wsdlNamespaceURL = port.substr(0, hash);

                    
                    if ( (hash+1) < port.length())
                    {
                        // Check the next part is wsdl.endpoint( 
                        int ending = hash+15;
                        string check = port.substr(hash+1, 14);
                        if (check.compare("wsdl.endpoint(") == 0)
                        {
                            // Find the matching )
                            int endBracket = port.find(")",ending);
                            if (endBracket-1 > ending+1)
                            {
                                string serviceAndPort = port.substr(ending, endBracket-ending);
                                // Look for a '/'
                                string::size_type slash = serviceAndPort.find("/");
                                if (slash != string::npos)
                                {
                                    serviceName = serviceAndPort.substr(0, slash);

                                    if ( (slash+1) < serviceAndPort.length())
                                    {
                                        portName = serviceAndPort.substr(slash+1);
                                    }
                                    else
                                    {
                                        portName = "";
                                    }

                                }
                                else
                                {
                                    // No '/' so all of it is the service name
                                    serviceName = serviceAndPort;
                                    portName = "";

                                }
                            }
                            else
                            {
                                // Nothing between the ()
                                serviceName = "";
                                portName = "";
                            }
                        }
                        else
                        {
                            // not the correct characters after the #, ignore the rest
                            serviceName = "";
                            portName = "";
                        }
                        
                    }
                    else
                    {
                        // Nothing after the hash
                        serviceName = "";
                        portName = "";
                    }
                }
                else
                {
                    // No hash at all
                    wsdlNamespaceURL = port;
                    serviceName = "";
                    portName = "";
                }
            }

            // Destructor
            WSReferenceBinding::~WSReferenceBinding()
            {
            }
            
            void WSReferenceBinding::configure(ServiceBinding *binding)
            {
                targetServiceBinding = binding;
                
                serviceProxy = new WSServiceProxy(getReference());
            }
            
            ServiceProxy* WSReferenceBinding::getServiceProxy()
            {
                return serviceProxy;
            }
                
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany
