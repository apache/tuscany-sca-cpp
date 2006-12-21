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

#include "tuscany/sca/cpp/TuscanyRuntime.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/core/SCARuntime.h"

using namespace std;

namespace tuscany
{
    namespace sca
    {
        namespace cpp
        {
                
            // ===================================================================
            // Constructor for the TuscanyRuntime class. 
            // ===================================================================
            TuscanyRuntime::TuscanyRuntime(const string& componentName, const string& root, const string& path)
            { 
                logentry();
                setSystemRoot(root);
                setSystemPath(path);
                setDefaultComponentName(componentName);           
            }
    
            // ===================================================================
            // Destructor for the TuscanyRuntime class.
            // ===================================================================
            TuscanyRuntime::~TuscanyRuntime()
            { 
                logentry();
            }
    
            // ==========================================================
            // Set the system configuration root path
            // ==========================================================
            void TuscanyRuntime::setSystemRoot(const string& root)
            {
                logentry();
                systemRoot = root;
                loginfo("System root: %s", root.c_str());
            }
    
            // ==========================================================
            // Set the search path for composites
            // ==========================================================
            void TuscanyRuntime::setSystemPath(const string& path)
            {
                logentry();
                systemPath = path;
                loginfo("System path: %s", path.c_str());
            }
    
            // ==========================================================
            // Set the default component name
            // ==========================================================
            void TuscanyRuntime::setDefaultComponentName(const string& componentName)
            {
                logentry();
                defaultComponentName = componentName;
                loginfo("Default component name: %s", componentName.c_str());
            }
    
            // ===================================================================
            // Start the runtime. 
            // ===================================================================
            void TuscanyRuntime::start()
            {
                logentry(); 
                SCARuntime::setSystemRoot(systemRoot);
                SCARuntime::setSystemPath(systemPath);
                SCARuntime::setDefaultComponentName(defaultComponentName);
                SCARuntime::getInstance();
            }
    
            // ===================================================================
            // Stop the runtime. 
            // ===================================================================
            void TuscanyRuntime::stop()
            { 
                logentry();
                SCARuntime::releaseInstance();
            }

        } // End namespace cpp
    } // End namespace sca
} // End namespace tuscany
