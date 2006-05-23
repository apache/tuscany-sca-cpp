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

#include "tuscany/sca/core/TuscanyRuntime.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/core/SCARuntime.h"

#include "tuscany/sca/model/System.h"

namespace tuscany
{
    namespace sca
    {
            
        // ==========================================================
        // Set the default ModuleComponent name
        // ==========================================================
        void TuscanyRuntime::setSystemRoot(const string& root)
        {
            LOGENTRY(1, "TuscanyRuntime::setSystemRoot");
            systemRoot = root;
            LOGINFO_1(3, "TuscanyRuntime::setSystemRoot - set to %s", root.c_str());
            LOGEXIT(1, "TuscanyRuntime::setSystemRoot");
        }

        // ==========================================================
        // Set the default ModuleComponent name
        // ==========================================================
        void TuscanyRuntime::setDefaultModuleComponent(const string& moduleComponent)
        {
            LOGENTRY(1, "TuscanyRuntime::setDefaultModuleComponent");
            defaultModuleComponent = moduleComponent;
            LOGINFO_1(3, "TuscanyRuntime::setDefaultModuleComponent - set to %s", moduleComponent.c_str());
            LOGEXIT(1, "TuscanyRuntime::setDefaultModuleComponent");
        }

        // ===================================================================
        // Constructor for the TuscanyRuntime class. 
        // ===================================================================
        TuscanyRuntime::TuscanyRuntime(const string& moduleComponent, const string& root)
        { 
            LOGENTRY(1, "TuscanyRuntime::constructor");
            setSystemRoot(root);
            setDefaultModuleComponent(moduleComponent);           
            LOGEXIT(1, "TuscanyRuntime::constructor");
        }

        // ===================================================================
        // Destructor for the TuscanyRuntime class.
        // ===================================================================
        TuscanyRuntime::~TuscanyRuntime()
        { 
            LOGENTRY(1, "TuscanyRuntime::destructor");;           
            LOGEXIT(1, "TuscanyRuntime::destructor");
        }

        // ===================================================================
        // Start the runtime. 
        // ===================================================================
        void TuscanyRuntime::start()
        { 
            LOGENTRY(1, "TuscanyRuntime::start");
            SCARuntime::setSystemRoot(systemRoot);
            SCARuntime::setDefaultModuleComponent(defaultModuleComponent);
            SCARuntime::getInstance();
            LOGEXIT(1, "TuscanyRuntime::start");
        }

        // ===================================================================
        // Stop the runtime. 
        // ===================================================================
        void TuscanyRuntime::stop()
        { 
            LOGENTRY(1, "TuscanyRuntime::stop");
            SCARuntime::releaseInstance();
            LOGEXIT(1, "TuscanyRuntime::stop");
        }


                
    } // End namespace sca
} // End namespace tuscany
