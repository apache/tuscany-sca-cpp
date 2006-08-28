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

#ifndef tuscany_sca_model_reference_h
#define tuscany_sca_model_reference_h

#include <string>
using std::string;
#include <vector>
using std::vector;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            class Component;
            class ReferenceType;
            class ReferenceBinding;
            class Service;

            /**
             * An addressable instance of a reference type associated with a particular component.
             * Each reference represents a configured version of a logical
             * reference type defined in the component type. If the logical reference
             * has a multiplicity greater than 1 (0..n or 1..n) then the configured
             * reference many have multiple targets.
             */
            class Reference
            {
            public:
                /**
                 * Constructor.
                 * @param component The component on which the reference is defined.
                 * @param referenceType The reference type defining the characteristics of the reference.
                 */
                 Reference(Component* component, ReferenceType* referenceType);
                 
                /**
                 * Destructor.
                 */
                 virtual ~Reference();

                /**
                 * Returns the component on which this reference is defined.
                 * @return The component on which this reference is defined.
                 */
                Component* getComponent() const { return component; }

                /**
                 * Returns the reference type defining the characteristics of the reference.
                 * @return The reference type defining the characteristics of the reference.
                 */
                ReferenceType* getType() const { return type; }

                /**
                 * Returns the binding supported by the reference.
                 * @return The binding supported by the reference.
                 */
                ReferenceBinding* getBinding() const;

                /**
                 * Sets the binding supported by the reference.
                 * @param binding The binding supported by the reference.
                 */
                void setBinding(ReferenceBinding* binding);

               /**
                 * Add a target for this reference. There may be more than
                 * one if the multiplicity is 0..n or 1..n.
                 * @param target The target of the reference.
                 */
                void addTarget(Service* target);

                typedef vector<Service*> TARGETS;

                /**
                 * Get a vector of all the targets from this reference.
                 * @return The targets of this reference.
                 */
                const TARGETS& getTargets() const { return targets; }

            private:

                /**
                 * The component on which this reference is defined.
                 */
                 Component* component;

                /**
                 * The reference type defining the characteristics of the reference.
                 */
                 ReferenceType* type;

                /**
                 * The binding supported by this reference
                 */
                 ReferenceBinding* binding;

                /**
                 * Vector of all the targets wired from this reference.
                 */
                TARGETS targets;
            };

         } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_reference_h
