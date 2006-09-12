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

#ifndef tuscany_sca_model_referencetype_h
#define tuscany_sca_model_referencetype_h

#include <string>
using std::string;

#include "tuscany/sca/model/Contract.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            class ComponentType;
            class Interface;
            class Binding;
            
            /**
             * Represents an SCA reference. SCA references within an implementation represent
             * links to services that the implementation uses that must be provided by other components
             * in the SCA system.
             */
            class ReferenceType : public Contract
            {
            public:

                /**
                 * Multiplicity (how many wires can be connected to this
                 * reference)
                 */
                enum Multiplicity
                {
                    ZERO_ONE = 1,
                    ONE_ONE = 2,
                    ZERO_MANY = 3,
                    ONE_MANY = 4,
                    UNKNOWN = 0,
                };

                /**
                 * Constructor.
                 * @param name The name of the reference.
                 */
                SCA_API ReferenceType(ComponentType* componentType, const string& name,
                    Interface* intface, Interface* callbackInterface, Multiplicity multiplicity);
                    
                /**
                 * Destructor.
                 */;
                SCA_API virtual ~ReferenceType();
                
                /**
                 * Returns the component type on which this reference is defined.
                 * @return The component type on which this reference is defined.
                 */
                SCA_API ComponentType* getComponentType() const { return componentType; }

                /**
                 * Returns the name of the reference.
                 * @return The name of the reference.
                 */
                SCA_API const string& getName() const { return name; }

                /**
                 * Returns the multiplicity allowed for wires connected to this reference.
                 * @return The multiplicity allowed for wires connected to this reference
                 */
                SCA_API Multiplicity getMultiplicity() const { return multiplicity; }

                /**
                 * Returns the binding supported by the reference.
                 * @return The binding supported by the reference.
                 */
                SCA_API Binding* getBinding() const;

                /**
                 * Sets the binding supported by the reference.
                 * @param binding The binding supported by the reference.
                 */
                SCA_API void setBinding(Binding* binding);
                
                /**
                 * Get the multiplicity corresponding to the given
                 * string.
                 */
                static Multiplicity getMultiplicityFromString(const string& multip);

            private:
            
                /**
                 * The component type on which this reference is defined.
                 */
                 ComponentType *componentType;
            
                /**
                 * The name of the reference type.
                 */
                string name;
                
                /**
                 * The multiplicity allowed for wires connected to this reference.
                 */
                Multiplicity multiplicity;

                /**
                 * The binding supported by this reference type.
                 */
                 Binding* binding;

             };
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_referencetype_h

