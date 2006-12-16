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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/model/ReferenceType.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            
            // Constructor
            ReferenceType::ReferenceType(ComponentType* componentType, const string& name,
                Interface* intface, Interface* callbackInterface, Multiplicity multiplicity)
                : Contract(intface, callbackInterface),
                componentType(componentType), name(name), multiplicity(multiplicity)
            {
                logentry(); 
            }
            
            ReferenceType::~ReferenceType()
            {
                logentry(); 
            }
            
            ReferenceType::Multiplicity ReferenceType::getMultiplicityFromString(const string& multip)
            {
                logentry(); 

                if (multip == "0..1")
                {
                    return ReferenceType::ZERO_ONE;
                }
                else if (multip == "1..1")
                {
                    return ReferenceType::ONE_ONE;
                }
                else if (multip == "0..n")
                {
                    return ReferenceType::ZERO_MANY;
                }
                else if (multip == "1..n")
                {
                    return ReferenceType::ONE_MANY;
                }
                else
                {
                    return ReferenceType::UNKNOWN;
                }
            }
            
           void ReferenceType::setBinding(Binding* binding)
            {
                this->binding = binding;
            }
            
            Binding* ReferenceType::getBinding() const
            {
                return binding;
            }
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
