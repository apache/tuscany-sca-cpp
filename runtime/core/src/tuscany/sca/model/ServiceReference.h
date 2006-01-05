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

#ifndef tuscany_sca_model_servicereference_h
#define tuscany_sca_model_servicereference_h

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/model/WireTarget.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Information about a reference on a serivce.
             */
            class ServiceReference
            {
            public:
                /**
                 * Constructor.
                 * @param name The name of the reference.
                 */
                ServiceReference(const std::string& name)
                    
                /**
                 * Destructor.
                 */;
                virtual ~ServiceReference();

                /**
                 * Get the name of the service.
                 * @return The name of the service.
                 */
                const std::string& getName() {return name;}

                /**
                 * Multiplicity (how many wires can be attached to this
                 * reference as their source)
                 */
                enum Multiplicity
                {
                    ZERO_ONE,
                    ONE_ONE,
                    ZERO_MANY,
                    ONE_MANY
                };

                /**
                 * Set the multiplicity of this reference.
                 * @param multiplicity One of "0..1", "1..1", "0..n", "1..n"
                 */
                void setMultiplicity(const std::string& multiplicity);

                /**
                 * Get the multiplicity of this reference.
                 * @return The multiplicity.
                 */
                Multiplicity getMultiplicity() {return multiplicity;}

                /**
                 * Set the interface required by this reference.
                 * @param iface The interface.
                 */
                void setInterface(Interface* iface);

                /**
                 * Get the interface.
                 * @return The interface.
                 */
                Interface* getInterface() {return iface;}

                /**
                 * Add a target for a reference. There may be more than
                 * one if the multiplicity is 0..n or 1..n.
                 * @param target The target for a wire.
                 */
                void addTarget(WireTarget* target);
                typedef vector<WireTarget*> TARGETS;

                /**
                 * Get a vector of all the targets from this reference.
                 * @return The targets of this reference.
                 */
                const TARGETS& getTargets() {return targets;}
            private:
                /**
                 * Name of this reference.
                 */
                string name;            
                
                /**
                 * The interface defining this reference.
                 */
                Interface* iface;

                /**
                 * The multiplicity of this reference.
                 */
                Multiplicity multiplicity;

                /**
                 * Vector of all the targets wired from this reference.
                 */
                TARGETS targets;
             };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_servicereference_h

