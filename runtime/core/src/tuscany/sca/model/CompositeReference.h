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

#ifndef tuscany_sca_model_compositereferencetype_h
#define tuscany_sca_model_compositereferencetype_h

#include <string>

#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ReferenceType.h"

namespace tuscany
{
    namespace sca
    {

        namespace model
        {
            class ComponentType;
            class Composite;
            class Interface;
            class Service;

            /**
             * Represents a reference in a composite.
             * Composite references represent dependencies that the composite has on services provided elsewhere,
             * outside the composite.
             */
            class CompositeReference : public Component
            {
            public:

                /**
                 * Constructor.
                 * @param name The name of the reference.
                 */
                CompositeReference(Composite* composite, const string& name,
                    Interface* interface, Interface* callbackInterface, bool conversational, ReferenceType::Multiplicity multiplicity);

                /**
                 * Destructor.
                 */
                virtual ~CompositeReference();
                
                /**
                 * Returns the service exposed by this composite reference.
                 * @return The service exposed by this composite reference.
                 */
                Service* getService() const { return service; };

            private:
            
                /**
                 * The service exposed by this composite reference.
                 */
                Service* service;

            };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_compositereferencetype_h

