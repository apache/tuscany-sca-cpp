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
#include "tuscany/sca/model/CompositeService.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ComponentType.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/ServiceType.h"

namespace tuscany
{
    namespace sca
    {

        namespace model
        {

            // Constructor
            CompositeService::CompositeService(Composite* composite, const string& name,
                    Interface* intface, Interface* callbackInterface, bool conversational,
                    ReferenceType::Multiplicity multiplicity) :
                Component(composite, name, new ComponentType(composite, name))
            {
                LOGENTRY(1, "CompositeService::constructor");
                LOGINFO_1(2, "CompositeService::constructor: CompositeService name: %s", name.c_str());
                
                // Initialize the component type, reference type and reference
                ComponentType* componentType = getType();
                ReferenceType* referenceType = new ReferenceType(
                    componentType, "", intface, callbackInterface, multiplicity);
                componentType->addReferenceType(referenceType);
                
                reference = new Reference(this, referenceType);
                addReference(reference);
                
                LOGEXIT(1, "CompositeService::constructor");
            }

            // Destructor
            CompositeService::~CompositeService()
            {
            }

        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
