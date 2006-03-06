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
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/model/ServiceReference.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            
            // Constructor
            ServiceReference::ServiceReference(const std::string& referenceName)
                : name(referenceName), multiplicity(ONE_ONE)
            {
            }
            
            ServiceReference::~ServiceReference()
            {
            }
            
            void ServiceReference::addTarget(WireTarget* targ)
            {
                if (multiplicity == ONE_ONE || multiplicity == ZERO_ONE)
                {
                    if (targets.size() > 0)
                    {
                        // throw exception
                        string message = "Duplicate wire for reference: " + name;
                        throw SystemConfigurationException(message.c_str());
                    }
                }

                targets.push_back(targ);
            }
            
            void ServiceReference::setMultiplicity(const std::string& multip)
            {
                if (multip == "0..1")
                {
                    multiplicity = ZERO_ONE;
                }
                else if (multip == "1..1")
                {
                    multiplicity = ONE_ONE;
                }
                else if (multip == "0..n")
                {
                    multiplicity = ZERO_MANY;
                }
                else if (multip == "1..n")
                {
                    multiplicity = ONE_MANY;
                }
                else
                {
                    string msg = "Invalid multiplicity specified, " + multip + ", for reference: " +name;
                    throw SystemConfigurationException(msg.c_str());
                }
            }
            
             void ServiceReference::setInterface(Interface* interf)
            {
                iface = interf;
            }

       } // End namespace model
    } // End namespace sca
} // End namespace tuscany
