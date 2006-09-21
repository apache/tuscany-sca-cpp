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
            WSReferenceBinding::WSReferenceBinding(Reference* reference, const string& uri, const string& endpoint, const string&version)
                : ReferenceBinding(reference, uri), endpoint(endpoint), soapVersion(version)
            {
                parseEndpoint();
            }
            
            void WSReferenceBinding::parseEndpoint()
            {
                // Endpoint is of the form: <wsdl-namepace-uri>#wsdl.endpoint(<service-name>/<endpoint-name>)
                string::size_type hash = endpoint.find("#");
                if (hash != string::npos)
                {
                    // Found a hash

                    // Namepace is the part before the #
                    wsdlNamespaceURL = endpoint.substr(0, hash);

                    
                    if ( (hash+1) < endpoint.length())
                    {
                        // Check the next part is wsdl.endpoint( 
                        int ending = hash+15;
                        string check = endpoint.substr(hash+1, 14);
                        if (check.compare("wsdl.endpoint(") == 0)
                        {
                            // Find the matching )
                            int endBracket = endpoint.find(")",ending);
                            if (endBracket-1 > ending+1)
                            {
                                string serviceAndEndpoint = endpoint.substr(ending, endBracket-ending);
                                // Look for a '/'
                                string::size_type slash = serviceAndEndpoint.find("/");
                                if (slash != string::npos)
                                {
                                    serviceName = serviceAndEndpoint.substr(0, slash);

                                    if ( (slash+1) < serviceAndEndpoint.length())
                                    {
                                        endpointName = serviceAndEndpoint.substr(slash+1);
                                    }
                                    else
                                    {
                                        endpointName = "";
                                    }

                                }
                                else
                                {
                                    // No '/' so all of it is the service name
                                    serviceName = serviceAndEndpoint;
                                    endpointName = "";

                                }
                            }
                            else
                            {
                                // Nothing between the ()
                                serviceName = "";
                                endpointName = "";
                            }
                        }
                        else
                        {
                            // not the correct characters after the #, ignore the rest
                            serviceName = "";
                            endpointName = "";
                        }
                        
                    }
                    else
                    {
                        // Nothing after the hash
                        serviceName = "";
                        endpointName = "";
                    }
                }
                else
                {
                    // No hash at all
                    wsdlNamespaceURL = endpoint;
                    serviceName = "";
                    endpointName = "";
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
