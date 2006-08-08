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
            // Get a map of all the compositeComponents that use a particular composite
            // ====================================================================
            COMPOSITE_LIST System::findComposites(const std::string& name)
            {
                LOGENTRY(1, "System::findComposites");
                COMPOSITE_LIST foundComposites;
                
                SUBSYSTEM_MAP::iterator subsystemsIter;
                for (subsystemsIter = subsystemMap.begin(); 
                subsystemsIter != subsystemMap.end(); subsystemsIter++ ) {
                    LOGINFO_1(2, "System::findComposites: Found subsystem: %s", subsystemsIter->second->getName().c_str());
                    // In each subsystem, find composite components with this composite defined
                    
                    Composite* composite = subsystemsIter->second->findComposite(name);
                    if (composite)
                    {
                        LOGINFO_1(2, "System::findComposites: Found composite: %s", composite->getName().c_str());
                        foundComposites.push_back(composite);
                    }
                }   
                LOGEXIT(1, "System::findComposites");
                
                return foundComposites;
            }

            
            // ====================================================================
            // Get a map of all the compositeComponents that use a particular composite
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
