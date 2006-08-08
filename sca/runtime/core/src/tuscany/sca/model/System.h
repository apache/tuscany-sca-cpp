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

#ifndef tuscany_sca_model_system_h
#define tuscany_sca_model_system_h

#include <map>
#include <string>
#include <vector>

#include "tuscany/sca/model/Subsystem.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            typedef std::vector<Composite*> COMPOSITE_LIST;

            /**
             * The root of the runtime model supported by a Tuscany runtime.
             */
            class System {
            public:
                System();
                virtual ~System();

                /**
                 * Add a new subsystem to the system.
                 * @param subsystemName The name of the new subsystem to add.
                 * @return The new Subsystem added to the system.
                 */
                Subsystem* addSubsystem(const std::string& subsystemName);

                /**
                 * Find subsystem by name.
                 * @param subsystemName The name of the subsystem to be found.
                 * @return The Subsystem found, or 0 if not found.
                 */
                Subsystem* findSubsystem(const std::string& subsystemName);

                typedef std::vector<Composite*> COMPOSITE_LIST;

                /**
                 * Return a vector of all the composites with a given composite name.
                 * @param compositeName The name of the composite to find
                 * @return A vector of all the times the composite appears in the 
                 * system. It can be more than once because the same composite name
                 * is used by more than one composite component.
                 */
                COMPOSITE_LIST findComposites(const std::string& compositeName);

                /**
                 * Resolve all the wires defined in the system.
                 */
                void resolveWires();
 
            private:

                typedef std::map<std::string, Subsystem*>    SUBSYSTEM_MAP;

                /**
                 * Map by name of subsystems.
                 */
                SUBSYSTEM_MAP subsystemMap;
           };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_system_h

