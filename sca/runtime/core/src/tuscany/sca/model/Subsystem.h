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

#include "tuscany/sca/model/Module.h"
#include "tuscany/sca/model/EntryPoint.h"

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
                 * Add a new module component to the subsystem.
                 * @param name The name of the module component.
                 * @param moduleName The name of the module.
                 * @return The new Module added to the subsystem.
                 */
                Module* addModuleComponent(const std::string& name, const std::string& moduleName);

                /**
                 * Find a module by module name.
                 * @param moduleName The name of the module to be found.
                 * @return The Module that was found, or 0 if not found.
                 */
                Module* findModule(const std::string& moduleName);

                /**
                 * Find a module by the module component name.
                 * @param moduleComponentName The module component name to be used to find
                 * the module.
                 * @return The Module that was found, or 0 if not found.
                 */
                Module* findModuleByComponentName(const std::string& moduleComponentName);

                /**
                 * Get the default module set for this subsystem.
                 * @return The default module.
                 */
                Module* getDefaultModule();
                
                /**
                 * Add an new entry point to the subsystem.
                 * @param name The name of the entry point.
                 * @return The new EntryPoint added to the subsystem.
                 */
                EntryPoint* addEntryPoint(const std::string& name);

                /**
                 * Find an entry point by name.
                 * @param name The name of the entry point to be found.
                 * @return The EntryPoint that was found, or 0 if not found.
                 */
                EntryPoint* findEntryPoint(const std::string& name);

                /**
                 * Return the name of the subsystem.
                 * @return The name.
                 */
                const std::string& getName() {return name;}

                /**
                 * Resolve all the wires configured in all the modules in this
                 * subsystem.
                 */
                void resolveWires();
  
            private:    
                /**
                 * The name of the subsystem.
                 */
                std::string name;

                typedef std::map<std::string, Module*> MODULECOMPONENT_MAP;
                /**
                 * A map by module component name of modules.
                 */
                MODULECOMPONENT_MAP moduleComponents; 

                typedef map<string, EntryPoint*> ENTRYPOINT_MAP;
                /**
                 * A map of entry points defined in this subsystem.
                 */
                ENTRYPOINT_MAP entryPoints;
            };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_h

