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
#include "tuscany/sca/model/Subsystem.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            // Constructor
            Subsystem::Subsystem(const std::string& subsystemName) : name(subsystemName)
            {
                LOGENTRY(1, "Subsystem::constructor");
                
                
                LOGEXIT(1, "Subsystem::constructor");
            }
            
            Subsystem::~Subsystem()
            {
            }

            ///
            /// Add a new module component
            ///
            Module* Subsystem::addModuleComponent(const std::string& name, const std::string& moduleName)
            {
                LOGENTRY(1, "Subsystem::addModuleComponent");
                
                Module* newModule = new Module(moduleName);
                
                // TODO: Should check for duplicates and throw exception
                moduleComponents[name] = newModule;
                
                LOGEXIT(1, "Subsystem::addModuleComponent");
                
                return newModule;
            }
            
            Module* Subsystem::findModule(const std::string& moduleName)
            {
                for (MODULECOMPONENT_MAP::iterator moduleComponentIter = moduleComponents.begin();
                moduleComponentIter != moduleComponents.end();
                moduleComponentIter++)
                {
                    if (moduleName == moduleComponentIter->second->getName())
                    {
                        return (Module*)moduleComponentIter->second;
                    } 
                }
                return 0;
            }
            
            Module* Subsystem::findModuleByComponentName(const std::string& moduleComponentName)
            {
                return moduleComponents[moduleComponentName];
            }
            
            Module* Subsystem::getDefaultModule()
            {
                if (moduleComponents.size() == 1)
                {
                    MODULECOMPONENT_MAP::iterator moduleComponentIter = moduleComponents.begin();
                    return moduleComponentIter->second;
                }
                return 0;
            }
            
            ///
            /// Add a new entrypoint 
            ///
            EntryPoint* Subsystem::addEntryPoint(const std::string& name)
            {
                LOGENTRY(1, "Subsystem::addEntryPoint");
                
                EntryPoint* newEntryPoint = new EntryPoint(name);
                entryPoints[name] = newEntryPoint;
                LOGEXIT(1, "Subsystem::addEntryPoint");
                return findEntryPoint(name);
           }
            
            EntryPoint* Subsystem::findEntryPoint(const std::string& name)
            {
                return entryPoints[name];
            }

            void Subsystem::resolveWires()
            {
                for (MODULECOMPONENT_MAP::iterator moduleComponentIter = moduleComponents.begin();
                moduleComponentIter != moduleComponents.end();
                moduleComponentIter++)
                {
                    moduleComponentIter->second->resolveWires();
                }

                // Resolve EntryPoint wires
            }

        } // End namespace model
        
    } // End namespace sca
} // End namespace tuscany
