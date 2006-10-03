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

#ifndef tuscany_sca_model_contract_h
#define tuscany_sca_model_contract_h

#include <string>
using std::string;

#include "tuscany/sca/model/Interface.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Interface contracts define one or more business functions.  These business functions are
             * provided by services and are used by references.  Services are defined by the interface which
             * they implement.
             * This interface will typically be extended to support concrete interface type systems, such as
             * CPP classes, Java interfaces, WSDL 1.1 portTypes and WSDL 2.0 interfaces.
             */
            class Contract
            {
            public:
                
                /**
                 * Constructor.
                 */
                SCA_API Contract(Interface* intface, Interface* callbackInterface);

                /**
                 * Destructor.
                 */
                SCA_API virtual ~Contract();
                
                /**
                 * Returns the interface for invocations from the requestor to the provider.
                 * @return The interface for invocations from the requestor to the provider.
                 */
                SCA_API Interface* getInterface() const { return iface; }
                
                /**
                 * Returns the interface for invocations from the provider back to the requestor.
                 * @return The interface for invocations from the provider back to the requestor.
                 */
                SCA_API Interface* getCallbackInterface() const { return callbackInterface; }
                
            private:

                /**
                 * The interface for invocations from the requestor to the provider.
                 */
                Interface* iface;

                /**
                 * The interface for invocations from the provider back to the requestor.
                 */
                Interface* callbackInterface;
                
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_contract_h

