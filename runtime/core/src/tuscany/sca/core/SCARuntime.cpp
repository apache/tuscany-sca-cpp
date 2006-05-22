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

#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/core/ComponentServiceWrapper.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"

#include "tuscany/sca/model/System.h"
#include "tuscany/sca/model/Subsystem.h"
#include "tuscany/sca/model/ModelLoader.h"

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h> 
#endif

namespace tuscany
{
    namespace sca
    {
        /** 
         * Environment variable names
         */
        static const char* TUSCANY_SCACPP = "TUSCANY_SCACPP";
        static const char* TUSCANY_SCACPP_SYSTEM_ROOT = "TUSCANY_SCACPP_SYSTEM_ROOT";
        static const char* TUSCANY_SCACPP_DEFAULT_MODULE = "TUSCANY_SCACPP_DEFAULT_MODULE";
            
 
        // ==========================================================
        // Initialize static class member to not pointing at anything
        // ==========================================================
        SCARuntime* SCARuntime::instance = 0;
        string SCARuntime::systemRoot = "";
        string SCARuntime::defaultModuleName = "";
        

        // ==========================================================
        // Set the system configuration root
        // ==========================================================
        void SCARuntime::setSystemRoot(const string& root)
        {
            LOGENTRY(1, "SCARuntime::");
            systemRoot = root;
            LOGINFO_1(3, "SCARuntime::setSystemRoot - set to %s", root.c_str());
            LOGEXIT(1, "SCARuntime::setSystemRoot");
        }

        // ==========================================================
        // Set the default ModuleComponent name
        // ==========================================================
        void SCARuntime::setDefaultModuleComponent(const string& moduleComponent)
        {
            LOGENTRY(1, "SCARuntime::setDefaultModuleComponent");
            defaultModuleName = moduleComponent;
            LOGINFO_1(3, "SCARuntime::setDefaultModuleComponent - set to %s", moduleComponent.c_str());
            LOGEXIT(1, "SCARuntime::setDefaultModuleComponent");
        }

        // ===================================================================
        // Constructor for the SCARuntime class. This will be a singleton that
        // holds all the information about the current runtime.
        // ===================================================================
        SCARuntime::SCARuntime() : system(new System()), defaultModule(0)
        { 
            LOGENTRY(1, "SCARuntime::constructor");
            
            // Locate the SCA install root
            char*  root = 0;
            root = getenv(TUSCANY_SCACPP);
            if (root == 0)
            {
            	string msg = TUSCANY_SCACPP;
            	msg += " environment variable not set";
                throw SystemConfigurationException(msg.c_str());
            }
            else
            {
                SCARoot = root;
            }
            
            LOGEXIT(1, "SCARuntime::constructor");
        }

        // ===================================================================
        // Destructor for the SCARuntime class. 
        // ===================================================================
        SCARuntime::~SCARuntime()
        { 
            LOGENTRY(1, "SCARuntime::destructor");

            if (system)
            {
                delete system;
            }
            
            LOGEXIT(1, "SCARuntime::destructor");
        }

        // =============================================================
        // Get the instance of the runtime, creates it if does not exist
        // static method
        // =============================================================
        SCARuntime* SCARuntime::getInstance()
        {
            LOGENTRY(1, "SCARuntime::getInstance");
            
            if (instance == NULL) 
            {
                instance = new SCARuntime();

                if (systemRoot == "")
                {
                    
                    // Load the runtime
                    // Get root from environment variable TUSCANY_SCACPP_SYSTEM_ROOT
                    char* systemRootEnv = getenv(TUSCANY_SCACPP_SYSTEM_ROOT);
                    if (systemRootEnv == 0)
                    {
                        string msg = TUSCANY_SCACPP_SYSTEM_ROOT;
                        msg += " environment variable not set";
                        throw SystemConfigurationException(msg.c_str());
                    } 

                    systemRoot = systemRootEnv;
                }

                instance->load(systemRoot);
            }
            
            LOGEXIT(1, "SCARuntime::getInstance");
            
            return instance;
            
        }


        // =============================================================
        // Release the instance of the runtime.
        // =============================================================
        void SCARuntime::releaseInstance()
        {
            LOGENTRY(1, "SCARuntime::releaseInstance");
            
            if (instance) 
            {
                delete instance;
                instance = 0;
                systemRoot = "";
                defaultModuleName = "";        
            }
            
            LOGEXIT(1, "SCARuntime::releaseInstance");           
        }

        // ======================================
        // Load up all the details of the runtime
        // ======================================
        void SCARuntime::load(const string& configurationRoot)
        {
            LOGENTRY(1, "SCARuntime::load");
            
            LOGINFO_1(2,"configuration root: %s", configurationRoot.c_str());
            
            ModelLoader loader(system);
            // Load details of the module
            loader.load(configurationRoot);
            
            LOGEXIT(1, "SCARuntime::load");
        }
        
        
        // ===================================
        // Return the top of the runtime model
        // ===================================
        System* SCARuntime::getSystem()
        {
            return system;
        }


        // ===================================================
        // setCurrentComponent: push component for this thread
        // ===================================================        
        void SCARuntime::setCurrentComponent(Component* component)
        {

#if defined(WIN32)  || defined (_WINDOWS)
            DWORD currentThreadId = GetCurrentThreadId();
#else
            pthread_t currentThreadId = pthread_self();
#endif
            COMPONENTS_MAP::iterator iter = components.find(currentThreadId);
            if (iter == components.end())
            {
                components[currentThreadId] = COMPONENT_STACK();
                iter = components.find(currentThreadId);
            }
            
            COMPONENT_STACK& compStack = iter->second;
            compStack.push(component);
        }

        
        // ====================================================
        // unsetCurrentComponent: pop component for this thread
        // ====================================================
        Component* SCARuntime::unsetCurrentComponent()
        {
#if defined(WIN32)  || defined (_WINDOWS)
            DWORD currentThreadId = GetCurrentThreadId();
#else
            pthread_t currentThreadId = pthread_self();
#endif

            COMPONENTS_MAP::iterator iter = components.find(currentThreadId);
            if (iter != components.end())
            {    
                COMPONENT_STACK& compStack = iter->second;
                if (compStack.size() > 0)
                {
                    Component* component = compStack.top();
                    compStack.pop();
                    return component;
                }
            }
            
            return 0;
        }
        
        // =============================================================
        // getCurrentComponent: return current component for this thread
        // =============================================================
        Component* SCARuntime::getCurrentComponent()
        {
#if defined(WIN32)  || defined (_WINDOWS)
            DWORD currentThreadId = GetCurrentThreadId();
#else
            pthread_t currentThreadId = pthread_self();
#endif

            COMPONENTS_MAP::iterator iter = components.find(currentThreadId);
            if (iter == components.end())
            {
                components[currentThreadId] = COMPONENT_STACK();
                iter = components.find(currentThreadId);
            }
            
            COMPONENT_STACK& compStack = iter->second;
            if (compStack.size() > 0)
            {
                return compStack.top();
            }
            else
            {
                return 0;
            }
            
        }
        

        // ===========================================
        // getCurrentModule: return the current module
        // ===========================================
        Module* SCARuntime::getCurrentModule()
        {
            // ---------------------------------------------
            // Get module from current component if possible
            // ---------------------------------------------
            Component* comp = getCurrentComponent();
            if (comp != 0)
            {
                return comp->getModule();
            }

            // ----------------------
            // Get the default Module
            // ----------------------
            string message;
            if (!defaultModule)
            {
                // -------------------------------------------
                // Get the default module from the environment
                // -------------------------------------------
                if (defaultModuleName == "")
                {
                    const char* defMod = getenv(TUSCANY_SCACPP_DEFAULT_MODULE);
                    if (!defMod)
                    {
                        message = TUSCANY_SCACPP_DEFAULT_MODULE;
                        message += " environment variable not set";
                        throw SystemConfigurationException(message.c_str());
                    }
                    defaultModuleName = defMod;
                }
                string subsystemName, moduleName;
                Utils::tokeniseUri(defaultModuleName, subsystemName, moduleName);

                // ---------------------------
                // Subsystem must be specified
                // ---------------------------
                Subsystem* subsystem = system->findSubsystem(subsystemName);
                if (!subsystem)
                {
                    message = "Default subsystem \'" + subsystemName + "\' not found";
                    throw SystemConfigurationException(message.c_str());
                }
                
                // --------------------------------------------------------------------------
                // If module is not specified then get the default module for this subsystem.
                // This will be the ONLY module for this subsystem
                // --------------------------------------------------------------------------
                if (moduleName == "")
                {
                    defaultModule = subsystem->getDefaultModule();
                    if (!defaultModule)
                    {
                        message = "Default module not found for subsystem \'" + subsystemName + "\'";
                        throw SystemConfigurationException(message.c_str());
                    }
                }
                else 
                {
                    // -----------------------------
                    // get the named ModuleComponent
                    // -----------------------------
                    defaultModule = subsystem->findModuleByComponentName(moduleName);
                    if (!defaultModule)
                    {
                        message = "Default module \'" + moduleName  + "\' not found in subsystem \'" + subsystemName +"\'";
                        throw SystemConfigurationException(message.c_str());
                    }
                }
            }
            return defaultModule;        
        }
                
    } // End namespace sca
} // End namespace tuscany
