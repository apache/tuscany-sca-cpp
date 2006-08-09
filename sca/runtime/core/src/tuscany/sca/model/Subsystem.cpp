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
            /// Add a new composite component
            ///
            Composite* Subsystem::addCompositeComponent(const std::string& name, const std::string& compositeName)
            {
                LOGENTRY(1, "Subsystem::addCompositeComponent");
                
                Composite* newComposite = new Composite(compositeName);
                
                // TODO: Should check for duplicates and throw exception
                compositeComponents[name] = newComposite;
                
                LOGEXIT(1, "Subsystem::addCompositeComponent");
                
                return newComposite;
            }
            
            Composite* Subsystem::findComposite(const std::string& compositeName)
            {
                for (COMPOSITECOMPONENT_MAP::iterator compositeComponentIter = compositeComponents.begin();
                compositeComponentIter != compositeComponents.end();
                compositeComponentIter++)
                {
                    if (compositeName == compositeComponentIter->second->getName())
                    {
                        return (Composite*)compositeComponentIter->second;
                    } 
                }
                return 0;
            }
            
            Composite* Subsystem::findCompositeByComponentName(const std::string& compositeComponentName)
            {
                return compositeComponents[compositeComponentName];
            }
            
            Composite* Subsystem::getDefaultComposite()
            {
                if (compositeComponents.size() == 1)
                {
                    COMPOSITECOMPONENT_MAP::iterator compositeComponentIter = compositeComponents.begin();
                    return compositeComponentIter->second;
                }
                return 0;
            }
            
            ///
            /// Add a new entrypoint 
            ///
            CompositeServiceType* Subsystem::addCompositeServiceType(const std::string& name)
            {
                LOGENTRY(1, "Subsystem::addCompositeServiceType");
                
                CompositeServiceType* newCompositeServiceType = new CompositeServiceType(name);
                compositeServices[name] = newCompositeServiceType;
                LOGEXIT(1, "Subsystem::addCompositeServiceType");
                return findCompositeServiceType(name);
           }
            
            CompositeServiceType* Subsystem::findCompositeServiceType(const std::string& name)
            {
                return compositeServices[name];
            }

            void Subsystem::resolveWires()
            {
                for (COMPOSITECOMPONENT_MAP::iterator compositeComponentIter = compositeComponents.begin();
                compositeComponentIter != compositeComponents.end();
                compositeComponentIter++)
                {
                    compositeComponentIter->second->resolveWires();
                }

                // Resolve CompositeServiceType wires
            }

        } // End namespace model
        
    } // End namespace sca
} // End namespace tuscany
