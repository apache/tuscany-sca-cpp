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
#include "tuscany/sca/model/System.h"


namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
            // ============
            // Constructor
            // ============
            System::System()
            {
                LOGENTRY(1, "System::constructor");
                
                
                LOGEXIT(1, "System::constructor");
            }
            
            System::~System()
            {
                SUBSYSTEM_MAP::iterator subsystemsIter;
                for (subsystemsIter = subsystemMap.begin();
                subsystemsIter != subsystemMap.end(); 
                subsystemsIter++ ) 
                {                    
                    delete subsystemsIter->second;
                }                
            }
            
            // =================================
            // Add a new subsystem to the system
            // =================================
            Subsystem* System::addSubsystem(const std::string& name)
            {
                LOGENTRY(1, "System::addSubsystem");
                
                Subsystem* newSubsystem = new Subsystem(name);
                
                subsystemMap[name] = newSubsystem;
                
                LOGEXIT(1, "System::addSubsystem");
                
                return newSubsystem;
            }
            
            // ====================================================================
            // Get a map of all the moduleComponents that use a particular module
            // ====================================================================
            MODULE_LIST System::findModules(const std::string& name)
            {
                LOGENTRY(1, "System::findModules");
                MODULE_LIST foundModules;
                
                SUBSYSTEM_MAP::iterator subsystemsIter;
                for (subsystemsIter = subsystemMap.begin(); 
                subsystemsIter != subsystemMap.end(); subsystemsIter++ ) {
                    LOGINFO_1(2, "System::findModules: Found subsystem: %s", subsystemsIter->second->getName().c_str());
                    // In each subsystem, find module components with this module defined
                    
                    Module* module = subsystemsIter->second->findModule(name);
                    if (module)
                    {
                        LOGINFO_1(2, "System::findModules: Found module: %s", module->getName().c_str());
                        foundModules.push_back(module);
                    }
                }   
                LOGEXIT(1, "System::findModules");
                
                return foundModules;
            }

            
            // ====================================================================
            // Get a map of all the moduleComponents that use a particular module
            // ====================================================================
            Subsystem* System::findSubsystem(const std::string& subsystem)
            {
                return subsystemMap[subsystem];
            }
            
            void System::resolveWires()
            {
                SUBSYSTEM_MAP::iterator subsystemsIter;
                for (subsystemsIter = subsystemMap.begin();
                subsystemsIter != subsystemMap.end(); 
                subsystemsIter++ ) 
                {                    
                    subsystemsIter->second->resolveWires();
                }                
            }

     } // End namespace model
        
    } // End namespace sca
} // End namespace tuscany
