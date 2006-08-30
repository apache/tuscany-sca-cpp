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

#ifndef tuscany_sca_model_servicetype_h
#define tuscany_sca_model_servicetype_h

#include <string>
using std::string;

#include "tuscany/sca/model/Contract.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            class ComponentType;
            class Binding;
            class Interface;

            /**
             * Represents a service type. Services are used to publish services provided by
             * implementations, so that they are addressable by other components.
             */
            class ServiceType : public Contract
            {
            public:
            
                /**
                 * Constructor.
                 * @param componentType The component type on which this service is defined.
                 * @param name The name of the service.
                 */
                SCA_API ServiceType(ComponentType* componentType, const string& name,
                    Interface* interface, Interface* callbackInterface, bool conversational);

                /**
                 * Destructor.
                 */
                 SCA_API virtual ~ServiceType();
                 
                /**
                 * Returns the component type on which this service is defined.
                 * @return The component type on which this service is defined.
                 */
                SCA_API ComponentType* getComponentType() const { return componentType; }

                /**
                 * Returns the name of the service type.
                 * @return The name of the service type
                 */
                 SCA_API const string& getName() const { return name; }

                /**
                 * Returns the binding supported by this service type.
                 * @return The binding supported by this service type
                 */
                SCA_API Binding* getBinding() const;
                
                /**
                 * Sets the binding supported by this service type.
                 * @param binding the binding supported by this service type
                 */
                SCA_API void setBinding(Binding* binding);

            private:

                /**
                 * The component type on which this service is defined.
                 */
                 ComponentType* componentType;
                 
                 /**
                  * The name of the service type.
                  */
                 string name;

                /**
                 * The binding supported by this service
                 */
                 Binding* binding;

            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_servicetype_h

