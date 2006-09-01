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

#ifndef tuscany_sca_model_service_h
#define tuscany_sca_model_service_h

#include <string>
using std::string;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            class Component;
            class ServiceType;
            class ServiceBinding;

            /**
             * An addressable instance of a service type associated with a particular component.
             */
            class Service
            {
            public:
                /**
                 * Constructor.
                 * @param component The component on which the service is defined.
                 * @param serviceType The service type defining the characteristics of the service.
                 */
                 SCA_API Service(Component* component, ServiceType* serviceType);
                 
                /**
                 * Destructor.
                 */
                 SCA_API virtual ~Service();

                /**
                 * Returns the component on which this service is defined.
                 * @return The component on which this service is defined.
                 */
                SCA_API Component* getComponent() const { return component; }

                /**
                 * Returns the service type defining the characteristics of the service.
                 * @return The service type defining the characteristics of the service.
                 */
                SCA_API ServiceType* getType() const { return type; }

                /**
                 * Returns the binding supported by the service.
                 * @return The binding supported by the service.
                 */
                SCA_API ServiceBinding* getBinding() const;

                /**
                 * Sets the binding supported by the service.
                 * @param binding The binding supported by the service.
                 */
                SCA_API void setBinding(ServiceBinding* binding);

            private:

                /**
                 * The component on which this service is defined.
                 */
                 Component* component;

                /**
                 * The service type defining the characteristics of the service.
                 */
                 ServiceType* type;

                /**
                 * The binding supported by this service
                 */
                 ServiceBinding* binding;

            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_service_h
