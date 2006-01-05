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

#ifndef tuscany_sca_model_wiretarget_h
#define tuscany_sca_model_wiretarget_h

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
             * Represents the target of a wire. This could be a service on a 
             * component or an external service.
             */
            class WireTarget
            {
            public:
                /**
                 * Supported types of wire target.
                 */
                enum Type
                {
                    ComponentServiceType,
                    ExternalServiceType
                };
                
                /**
                 * Constructor.
                 * @param name The name of the wire target.
                 */
                WireTarget(const std::string& name);

                /**
                 * Destructor.
                 */
                virtual ~WireTarget();
                
                /**
                 * Return the name of the wire target.
                 */
                string getName() {return name;}
                
                /**
                 * Get the type of the wire target. 
                 * @return Depends on the type of the wire target.
                 */
                virtual Type getServiceType() = 0;

                /**
                 * Set the interface describing the wire target.
                 * @param iface The interface.
                 */
                void setInterface(Interface* iface);

                /**
                 * Get the interface describing the wire target.
                 * @return The interface.
                 */
                Interface* getInterface() {return iface;}
                
            private:
                /**
                 * Name of the wire target (service name or external service name)
                 */
                string name;

                /**
                 * Type of the service.
                 */
                Type serviceType;

                /**
                 * Interface describing the wire target.
                 */
                Interface* iface;
            };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_wiretarget_h

