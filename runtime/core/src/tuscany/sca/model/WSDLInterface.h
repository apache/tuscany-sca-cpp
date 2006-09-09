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

#ifndef tuscany_sca_model_wsdlinterface_h
#define tuscany_sca_model_wsdlinterface_h

#include "tuscany/sca/model/Interface.h"

#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Holds information about an interface described using a WSDL
             * port type.
             */
            class WSDLInterface : public Interface
            {
                
            public:    
                /**
                 * Constuctor.
                 * @param interfaceName Name of the WSDL interface.
                 */
                WSDLInterface(const string& qname, bool remotable, bool conversational);  

                /**
                 * Destructor.
                 */
                virtual ~WSDLInterface();
                
                /**
                 * Returns the WSDL namespace
                 */
                 string getNamespaceURI() const { return namespaceURI; }
                 
                 /**
                  * Returns the interface name
                  */
                  string getName() const { return name; }  

                /**
                * return the QName of the schema type for this interface type
                * (e.g. "http://www.osoa.org/xmlns/sca/1.0#interface.cpp")
                */
                virtual const string& getInterfaceTypeQName() { return typeQName; };
    
                /**
                 * The QName of the schema type for this interface type.
                 */
                static const string typeQName;
                
           private:
           
                /**
                 * Parse the WSDL qname
                 */
                 void parse(const string& qname);
           
                /**
                 * WSDL namespace.
                 */
                string namespaceURI;

                /**
                 * Name of the WSDL interface.
                 */
                string name;

            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_cpp_wsdlinterface_h

