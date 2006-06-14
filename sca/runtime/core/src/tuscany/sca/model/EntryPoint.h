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

#ifndef tuscany_sca_model_entrypoint_h
#define tuscany_sca_model_entrypoint_h

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/model/Binding.h"
#include "tuscany/sca/model/WireTarget.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Represents the information about an entry point.
             */
            class EntryPoint 
            {

            public:
                /**
                 * Constructor.
                 * @param name The name of the entry point.
                 */
                EntryPoint(const std::string& name);  

                /**
                 * Destructor.
                 */
                virtual ~EntryPoint();

                /**
                 * Return the name of the entry point.
                 * @return The name of the entry point.
                 */
                const string& getName() {return name;}

                /**
                 * How many wires can be wired from this entry point.
                 */
                enum Multiplicity
                {
                    ZERO_ONE,
                    ONE_ONE,
                    ZERO_MANY,
                    ONE_MANY
                };

                /**
                 * Set the multiplicity of this entry point.
                 * @param multiplicity One of 0..1, 1..1, 0..n, 1..n
                 */
                void setMultiplicity(const std::string& multiplicity);

                /**
                 * Return the multiplicity of this entry point (how
                 * many wires can be wired from this entry point).
                 * @return The multiplicity.
                 */
                Multiplicity getMultiplicity() {return multiplicity;}

                /**
                 * Set the interface describing this entry point.
                 * @param iface The interface.
                 */
                void setInterface(Interface* iface);

                /**
                 * Get the interface describing this entry point. Use
                 * Interface#getType to find out the type of interface 
                 * describing this entry point.
                 * @return iface The interface.
                 */
                Interface* getInterface() {return iface;}

				/**
                 * Set the binding connecting this entry point.
                 * @param binding The Binding.
                 */
                void setBinding(Binding* binding);

                /**
                 * Get the binding connecting this entry point. Use
                 * Binding#getType to find out the type of binding 
                 * connecting this entry point.
                 * @return binding The binding.
                 */
                Binding* getBinding() {return binding;}

                /**
                 * Add a target to the entry point.
                 * @param target Add a target (derived from a wire) to
                 * the entry point.
                 */
                void addTarget(WireTarget* target);
                typedef vector<WireTarget*> TARGETS;

                /**
                 * Get a vector of targets added to this entry point.
                 */
                const TARGETS& getTargets() {return targets;}
            private:
                /**
                 * Name of the entry point.
                 */
                string name;            
                
                /**
                 * The interface describing this entry point.
                 */
                Interface* iface;

                /**
                 * The binding connecting this entry point.
                 */
                Binding* binding;

                /**
                 * The multiplicity of this entry point..
                 */
                Multiplicity multiplicity;

                /**
                 * The vector of targets added to this entry point.
                 */
                TARGETS targets;
           };

        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_entrypoint_h

