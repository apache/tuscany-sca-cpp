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

#ifndef tuscany_sca_model_h
#define tuscany_sca_model_h

#include <string>
using std::string;
#include <map>
using std::map;

#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/CompositeServiceType.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Information about a subsystem.
             */
            class Subsystem {
            public:
                /**
                 * Constructor.
                 * @param name The name of the subsystem.
                 */
                Subsystem(const std::string& name);

                /**
                 * Destructor.
                 */
                virtual ~Subsystem();

                /**
                 * Add a new composite component to the subsystem.
                 * @param name The name of the composite component.
                 * @param compositeName The name of the composite.
                 * @return The new Composite added to the subsystem.
                 */
                Composite* addCompositeComponent(const std::string& name, const std::string& compositeName);

                /**
                 * Find a composite by composite name.
                 * @param compositeName The name of the composite to be found.
                 * @return The Composite that was found, or 0 if not found.
                 */
                Composite* findComposite(const std::string& compositeName);

                /**
                 * Find a composite by the composite component name.
                 * @param compositeComponentName The composite component name to be used to find
                 * the composite.
                 * @return The Composite that was found, or 0 if not found.
                 */
                Composite* findCompositeByComponentName(const std::string& compositeComponentName);

                /**
                 * Get the default composite set for this subsystem.
                 * @return The default composite.
                 */
                Composite* getDefaultComposite();
                
                /**
                 * Add an new entry point to the subsystem.
                 * @param name The name of the entry point.
                 * @return The new CompositeServiceType added to the subsystem.
                 */
                CompositeServiceType* addCompositeServiceType(const std::string& name);

                /**
                 * Find an entry point by name.
                 * @param name The name of the entry point to be found.
                 * @return The CompositeServiceType that was found, or 0 if not found.
                 */
                CompositeServiceType* findCompositeServiceType(const std::string& name);

                /**
                 * Return the name of the subsystem.
                 * @return The name.
                 */
                const std::string& getName() {return name;}

                /**
                 * Resolve all the wires configured in all the composites in this
                 * subsystem.
                 */
                void resolveWires();
  
            private:    
                /**
                 * The name of the subsystem.
                 */
                std::string name;

                typedef std::map<std::string, Composite*> COMPOSITECOMPONENT_MAP;
                /**
                 * A map by composite component name of composites.
                 */
                COMPOSITECOMPONENT_MAP compositeComponents; 

                typedef map<string, CompositeServiceType*> ENTRYPOINT_MAP;
                /**
                 * A map of entry points defined in this subsystem.
                 */
                ENTRYPOINT_MAP compositeServices;
            };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_h

