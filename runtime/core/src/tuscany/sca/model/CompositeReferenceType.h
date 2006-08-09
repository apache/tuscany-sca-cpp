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

#ifndef tuscany_sca_model_externalservice_h
#define tuscany_sca_model_externalservice_h

#include <string>

#include "tuscany/sca/model/WireTarget.h"
#include "tuscany/sca/model/Binding.h"

namespace tuscany
{
    namespace sca
    {

        namespace model
        {
            class Composite;

            /**
             * Information about an external service.
             */
            class CompositeReferenceType : public WireTarget
            {
            public:
                /**
                 * Constructor.
                 * @param name The name of the external service.
                 * @param composite The composite containing this external service.
                 */
                CompositeReferenceType(const std::string& name, Composite *composite);

                /**
                 * Destructor.
                 */
                virtual ~CompositeReferenceType();

                /**
                 * Return the type of service.
                 * @return Always returns CompositeReferenceTypeType
                 */
                virtual Type getServiceType() {return CompositeReferenceTypeType;}

                /**
                 * Set the binding for this external service.
                 * @param binding The binding to set.
                 */
                virtual void setBinding(Binding* binding);

                /**
                 * Get the binding set for this external service.
                 * @return The binding.
                 */
                virtual Binding* getBinding() {return binding;};

                /**
                 * Get the composite containing this external service.
                 * @return The containing composite.
                 */
                Composite* getContainingComposite() {return containingComposite;};

            private:
                /**
                 * The binding for this external service.
                 */
                Binding* binding;

                /**
                 * The composite containing this external service.
                 */
                Composite* containingComposite;


            };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_externalservice_h

