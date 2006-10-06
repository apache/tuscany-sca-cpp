/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 *   
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* $Rev$ $Date$ */

#include "tuscany/sca/core/TuscanyRuntime.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/core/SCARuntime.h"

namespace tuscany
{
    namespace sca
    {
            
        // ==========================================================
        // Set the system configuration root path
        // ==========================================================
        void TuscanyRuntime::setSystemRoot(const string& root)
        {
            LOGENTRY(1, "TuscanyRuntime::setSystemRoot");
            systemRoot = root;
            LOGINFO_1(3, "TuscanyRuntime::setSystemRoot - set to %s", root.c_str());
            LOGEXIT(1, "TuscanyRuntime::setSystemRoot");
        }

        // ==========================================================
        // Set the search path for composites
        // ==========================================================
        void TuscanyRuntime::setSystemPath(const string& path)
        {
            LOGENTRY(1, "TuscanyRuntime::setSystemPath");
            systemPath = path;
            LOGINFO_1(3, "TuscanyRuntime::setSystemPath - set to %s", path.c_str());
            LOGEXIT(1, "TuscanyRuntime::setSystemPath");
        }

        // ==========================================================
        // Set the default component name
        // ==========================================================
        void TuscanyRuntime::setDefaultComponentName(const string& componentName)
        {
            LOGENTRY(1, "TuscanyRuntime::setDefaultComponentName");
            defaultComponentName = componentName;
            LOGINFO_1(3, "TuscanyRuntime::setDefaultComponentName - set to %s", componentName.c_str());
            LOGEXIT(1, "TuscanyRuntime::setDefaultComponentName");
        }

        // ===================================================================
        // Constructor for the TuscanyRuntime class. 
        // ===================================================================
        TuscanyRuntime::TuscanyRuntime(const string& componentName, const string& root, const string& path)
        { 
            LOGENTRY(1, "TuscanyRuntime::constructor");
            setSystemRoot(root);
            setSystemPath(path);
            setDefaultComponentName(componentName);           
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
            SCARuntime::setSystemPath(systemPath);
            SCARuntime::setDefaultComponentName(defaultComponentName);
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
