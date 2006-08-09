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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/model/CompositeReferenceType.h"


namespace tuscany
{
    namespace sca
    {

        namespace model
        {

            // Constructor
            CompositeReferenceType::CompositeReferenceType(const std::string& serviceName, Composite *composite)
                : WireTarget(serviceName), containingComposite(composite)
            {
                LOGENTRY(1, "CompositeReferenceType::constructor");
                LOGINFO_1(2, "CompositeReferenceType::constructor: CompositeReferenceType name: %s", getName().c_str());
                LOGEXIT(1, "CompositeReferenceType::constructor");
            }

            CompositeReferenceType::~CompositeReferenceType()
            {
                delete binding;
            }

            void CompositeReferenceType::setBinding(Binding* bind)
            {
                binding = bind;
            }


        } // End namespace model

    } // End namespace sca
} // End namespace tuscany
