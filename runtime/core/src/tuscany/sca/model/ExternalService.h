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
            class Module;

            /**
             * Information about an external service.
             */
            class ExternalService : public WireTarget
            {
            public:
                /**
                 * Constructor.
                 * @param name The name of the external service.
                 * @param module The module containing this external service.
                 */
                ExternalService(const std::string& name, Module *module);

                /**
                 * Destructor.
                 */
                virtual ~ExternalService();

                /**
                 * Return the type of service.
                 * @return Always returns ExternalServiceType
                 */
                virtual Type getServiceType() {return ExternalServiceType;}

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
                 * Get the module containing this external service.
                 * @return The containing module.
                 */
                Module* getContainingModule() {return containingModule;};

            private:
                /**
                 * The binding for this external service.
                 */
                Binding* binding;

                /**
                 * The module containing this external service.
                 */
                Module* containingModule;


            };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_externalservice_h

