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


#ifndef tuscany_sca_extension_ws_wsservicebindingextension_h
#define tuscany_sca_extension_ws_wsservicebindingextension_h

#include "tuscany/sca/extension/ServiceBindingExtension.h"

namespace tuscany
{
    namespace sca
    {
        namespace ws
        {
            
            class WSServiceBindingExtension : public ServiceBindingExtension 
            {
            public:
            /**
            * Default constructor
                */
                WSServiceBindingExtension();
                
                /**
                * Destructor
                */
                virtual ~WSServiceBindingExtension();            
                
                /**
                * return the name of the extension
                */
                virtual const string& getExtensionName() {return extensionName;}
                
                /**
                * return the QName of schema elemant for this implementation extension
                * (e.g. "http://www.osoa.org/xmlns/sca/1.0#binding.ws")
                */
                virtual const string& getExtensionTypeQName() {return typeQName;}
                
                virtual ServiceBinding* getServiceBinding(Composite* composite, Service* service, DataObjectPtr scdlBinding);
                
                static void initialize();

            private:
                static const string extensionName;
                static const string typeQName;
                
            };
            
            
        } // End namespace ws
    } // End namespace sca
} // End namespace tuscany

#endif //tuscany_sca_extension_ws_wsservicebindingextension_h

