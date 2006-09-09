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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/WSDLInterface.h"


namespace tuscany
{
    namespace sca
    {
        namespace model
        {

            const string WSDLInterface::typeQName("http://www.osoa.org/xmlns/sca/1.0#WSDLPortType");
            
            // Constructor
            WSDLInterface::WSDLInterface(
                    const string& qname, bool remotable, bool conversational) 
                    : Interface(remotable, conversational)
            {
                parse(qname);
           }
           
            void WSDLInterface::parse(const string& qname)
            {
                // PortType is of the form: <wsdl-namepace-uri>#wsdl.interface(<portType-name>)
                string::size_type hash = qname.find("#");
                if (hash != string::npos)
                {
                    // Found a hash

                    // Namepace is the part before the #
                    namespaceURI = qname.substr(0, hash);
                    
                    if ( (hash+1) < qname.length())
                    {
                        // Check the next part is wsdl.interface( 
                        int ending = hash+16;
                        string check = qname.substr(hash+1, 15);
                        if (check.compare("wsdl.interface(") == 0)
                        {
                            // Find the matching )
                            int endBracket = qname.find(")",ending);
                            if (endBracket-1 > ending+1)
                            {
                                name = qname.substr(ending, endBracket-ending);
                            }
                            else
                            {
                                // Nothing between the ()
                                name = "";
                            }
                        }
                        else
                        {
                            // not the correct characters after the #, ignore the rest
                            name = "";
                        }
                        
                    }
                    else
                    {
                        // Nothing after the hash
                        name = "";
                    }
                }
                else
                {
                    // No hash at all
                    namespaceURI = qname;
                    name = "";
                }
            }

           
            WSDLInterface::~WSDLInterface()
            {
            }

        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
