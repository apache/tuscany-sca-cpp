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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            // Constructor
            Reference::Reference(Component* component, ReferenceType* referenceType)
                : component(component), type(referenceType), binding(0)
            {
            }
            
            // Destructor
            Reference::~Reference()
            {
            }
            
            void Reference::setBinding(ReferenceBinding* binding)
            {
                this->binding = binding;
            }
            
            ReferenceBinding* Reference::getBinding() const
            {
                return binding;
            }
            
            void Reference::addTarget(Service* target)
            {
                if (type->getMultiplicity() == ReferenceType::ONE_ONE || type->getMultiplicity() == ReferenceType::ZERO_ONE)
                {
                    if (targets.size() > 0)
                    {
                        // throw exception
                        string message = "Duplicate wire for reference: " + type->getName();
                        throw SystemConfigurationException(message.c_str());
                    }
                }

                targets.push_back(target);
            }
            
       } // End namespace model
    } // End namespace sca
} // End namespace tuscany
