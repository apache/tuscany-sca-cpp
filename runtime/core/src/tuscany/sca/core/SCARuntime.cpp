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
        static const char* TUSCANY_SCACPP_DEFAULT_COMPOSITE = "TUSCANY_SCACPP_DEFAULT_COMPOSITE";
            
 
        // ==========================================================
        // Initialize static class member to not pointing at anything
        // ==========================================================
        SCARuntime* SCARuntime::instance = 0;
        string SCARuntime::systemRoot = "";
        string SCARuntime::defaultCompositeName = "";
        

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
        // Set the default CompositeComponent name
        // ==========================================================
        void SCARuntime::setDefaultCompositeComponent(const string& compositeComponent)
        {
            LOGENTRY(1, "SCARuntime::setDefaultCompositeComponent");
            defaultCompositeName = compositeComponent;
            LOGINFO_1(3, "SCARuntime::setDefaultCompositeComponent - set to %s", compositeComponent.c_str());
            LOGEXIT(1, "SCARuntime::setDefaultCompositeComponent");
        }

        // ===================================================================
        // Constructor for the SCARuntime class. This will be a singleton that
        // holds all the information about the current runtime.
        // ===================================================================
        SCARuntime::SCARuntime() : system(0), defaultComposite(0)
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
                
                // load extensions
                instance->loadExtensions();

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
                defaultCompositeName = "";        
            }
            
            LOGEXIT(1, "SCARuntime::releaseInstance");           
        }

        // ======================================
        // Load up all the details of the runtime
        // ======================================
        void SCARuntime::load()
        {
            LOGENTRY(1, "SCARuntime::load");
            
            LOGINFO_1(2,"configuration root: %s", systemRoot.c_str());
            
            ModelLoader loader(system);
            // Load details of the composite
            loader.load(systemRoot);
            
            LOGEXIT(1, "SCARuntime::load");
        }
        
        
        // ======================================
        // Load up extensions to the runtime
        // ======================================
        void SCARuntime::loadExtensions()
        {
            LOGENTRY(1, "SCARuntime::loadExtensions");

            string extensionsRoot = SCARoot + "/extensions";

#if defined(WIN32)  || defined (_WINDOWS)
            string pattern = "*.dll";
#else
            string pattern = "*.so";
#endif

            Files files(extensionsRoot, pattern, false);
            for (unsigned int i=0; i < files.size(); i++)
            {
                try
                {
                    Library lib = Library(extensionsRoot + "/" + files[i].getFileName());
                    extensionsList.push_back(lib);                    
                    TUSCANY_IMPLEMENTATION_EXTENSION_INITIALIZE extension = 
                        (TUSCANY_IMPLEMENTATION_EXTENSION_INITIALIZE)lib.getSymbol("tuscany_sca_extension_initialize");
                    if (extension)
                    {
                        extension();
                    }
                }
                catch (ServiceRuntimeException &)
                {
                    LOGERROR_1(0, "SCARuntime::loadExtensions failed to load extension library: %s", files[i].getFileName().c_str());
                }
            }
            
            LOGEXIT(1, "SCARuntime::loadExtensions");
        }

      
        // ======================================
        // register an interfaceExtension 
        // ======================================
        void SCARuntime::registerInterfaceExtension(InterfaceExtension* extension)
        {
            LOGENTRY(1, "SCARuntime::registerInterfaceExtension");
            if (extension)
            {
                interfaceExtensions[extension->getExtensionTypeQName()] = extension;          
            }
            LOGEXIT(1, "SCARuntime::registerInterfaceExtension");
        }
        
        // ======================================
        // find an InterfaceExtension 
        // ======================================
        InterfaceExtension* SCARuntime::getInterfaceExtension(const string& extensionTypeQName)
        {
            return interfaceExtensions[extensionTypeQName];
        }
      
        // ======================================
        // register an implementationExtension 
        // ======================================
        void SCARuntime::registerImplementationExtension(ImplementationExtension* extension)
        {
            LOGENTRY(1, "SCARuntime::registerImplementationExtension");
            if (extension)
            {
                implementationExtensions[extension->getExtensionTypeQName()] = extension;          
            }
            LOGEXIT(1, "SCARuntime::registerImplementationExtension");
        }
        
        // ======================================
        // find an implementationExtension 
        // ======================================
        ImplementationExtension* SCARuntime::getImplementationExtension(const string& extensionTypeQName)
        {
            return implementationExtensions[extensionTypeQName];
        }
        
        
        // ===================================
        // Return the top of the runtime model
        // ===================================
        System* SCARuntime::getSystem()
        {
            if (!system)
            {
                system = new System();
                load();
            }
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
        // getCurrentComposite: return the current composite
        // ===========================================
        Composite* SCARuntime::getCurrentComposite()
        {
            // ---------------------------------------------
            // Get composite from current component if possible
            // ---------------------------------------------
            Component* comp = getCurrentComponent();
            if (comp != 0)
            {
                return comp->getComposite();
            }

            // ----------------------
            // Get the default Composite
            // ----------------------
            string message;
            if (!defaultComposite)
            {
                // -------------------------------------------
                // Get the default composite from the environment
                // -------------------------------------------
                if (defaultCompositeName == "")
                {
                    const char* defMod = getenv(TUSCANY_SCACPP_DEFAULT_COMPOSITE);
                    if (!defMod)
                    {
                        message = TUSCANY_SCACPP_DEFAULT_COMPOSITE;
                        message += " environment variable not set";
                        throw SystemConfigurationException(message.c_str());
                    }
                    defaultCompositeName = defMod;
                }
                string subsystemName, compositeName;
                Utils::tokeniseUri(defaultCompositeName, subsystemName, compositeName);

                // ---------------------------
                // Subsystem must be specified
                // ---------------------------
                Subsystem* subsystem = getSystem()->findSubsystem(subsystemName);
                if (!subsystem)
                {
                    message = "Default subsystem \'" + subsystemName + "\' not found";
                    throw SystemConfigurationException(message.c_str());
                }
                
                // --------------------------------------------------------------------------
                // If composite is not specified then get the default composite for this subsystem.
                // This will be the ONLY composite for this subsystem
                // --------------------------------------------------------------------------
                if (compositeName == "")
                {
                    defaultComposite = subsystem->getDefaultComposite();
                    if (!defaultComposite)
                    {
                        message = "Default composite not found for subsystem \'" + subsystemName + "\'";
                        throw SystemConfigurationException(message.c_str());
                    }
                }
                else 
                {
                    // -----------------------------
                    // get the named CompositeComponent
                    // -----------------------------
                    defaultComposite = subsystem->findCompositeByComponentName(compositeName);
                    if (!defaultComposite)
                    {
                        message = "Default composite \'" + compositeName  + "\' not found in subsystem \'" + subsystemName +"\'";
                        throw SystemConfigurationException(message.c_str());
                    }
                }
            }
            return defaultComposite;        
        }
                
    } // End namespace sca
} // End namespace tuscany
