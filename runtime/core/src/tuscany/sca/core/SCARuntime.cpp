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

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h>
#else
#include "tuscany_sca_config.h"
#endif

#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/ModelLoader.h"
#include "tuscany/sca/util/File.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"

using namespace std;
using namespace tuscany::sca::model;
using namespace tuscany::sca::util;

namespace tuscany
{
    namespace sca
    {
        /** 
         * Environment variable names
         */
        static const char* TUSCANY_SCACPP = "TUSCANY_SCACPP";
        static const char* TUSCANY_SCACPP_SYSTEM_ROOT = "TUSCANY_SCACPP_SYSTEM_ROOT";
        static const char* TUSCANY_SCACPP_DEFAULT_COMPONENT = "TUSCANY_SCACPP_DEFAULT_COMPONENT";
        
        static const char* TUSCANY_SCACPP_ROOT = "TUSCANY_SCACPP_ROOT";
        static const char* TUSCANY_SCACPP_COMPONENT = "TUSCANY_SCACPP_COMPONENT";
        static const char* TUSCANY_SCACPP_PATH = "TUSCANY_SCACPP_PATH";
        static const char* TUSCANY_SCACPP_BASE_URI = "TUSCANY_SCACPP_BASE_URI";
 
        // ==========================================================
        // Initialize static class member to not pointing at anything
        // ==========================================================
        SCARuntime* SCARuntime::instance = 0;
        string SCARuntime::installRoot = "";
        string SCARuntime::systemRoot = "";
        string SCARuntime::systemPath = "";
        string SCARuntime::defaultComponentName = "";
        string SCARuntime::defaultBaseURI = "";
        

        // ==========================================================
        // Set the system configuration root
        // ==========================================================
        void SCARuntime::setSystemRoot(const string& root)
        {
            logentry();
            systemRoot = root;
            loginfo("System root: %s", root.c_str());
        }

        // ==========================================================
        // Returns the system configuration root
        // ==========================================================
        const string& SCARuntime::getSystemRoot()
        {
            return systemRoot;
        }

        // ==========================================================
        // Set the system configuration root
        // ==========================================================
        void SCARuntime::setSystemPath(const string& path)
        {
            logentry();
            systemPath = path;
            loginfo("System path: %s", path.c_str());
        }

        // ==========================================================
        // Set the system configuration root
        // ==========================================================
        const string& SCARuntime::getSystemPath()
        {
            return systemPath;
        }

        // ==========================================================
        // Set the default component name
        // ==========================================================
        void SCARuntime::setDefaultComponentName(const string& componentName)
        {
            logentry();
            defaultComponentName = componentName;
            loginfo("Default component name: %s", componentName.c_str());
        }

        // ==========================================================
        // Returns the default component name
        // ==========================================================
        const string& SCARuntime::getDefaultComponentName()
        {
            return defaultComponentName ;
        }

        // ==========================================================
        // Set the default base URI
        // ==========================================================
        void SCARuntime::setDefaultBaseURI(const string& baseURI)
        {
            logentry();
            defaultBaseURI = baseURI;
            loginfo("Default base URI: %s", baseURI.c_str());
        }

        // ==========================================================
        // Returns the default base URI
        // ==========================================================
        const string& SCARuntime::getDefaultBaseURI()
        {
            return defaultBaseURI;
        }

        // ==========================================================
        // Set the install root
        // ==========================================================
        void SCARuntime::setInstallRoot(const string& root)
        {
            logentry();
            installRoot = root;
            loginfo("SCA runtime install root: %s", installRoot.c_str());
        }

        // ==========================================================
        // Returns the install root
        // ==========================================================
        const string& SCARuntime::getInstallRoot()
        {
            return installRoot;
        }

        // ===================================================================
        // Constructor for the SCARuntime class. This will be a singleton that
        // holds all the information about the current runtime.
        // ===================================================================
        SCARuntime::SCARuntime() : system(0), defaultComponent(0)
        { 
            logentry();
        }

        // ===================================================================
        // Destructor for the SCARuntime class. 
        // ===================================================================
        SCARuntime::~SCARuntime()
        { 
            logentry();

            if (system)
            {
                delete system;
            }
            
        }

        // =============================================================
        // Get the instance of the runtime, creates it if does not exist
        // static method
        // =============================================================
        SCARuntime* SCARuntime::getInstance()
        {
            logentry();
            
            if (instance == NULL) 
            {
                if (installRoot == "")
                {
                    // Get install dir from environment variable TUSCANY_SCACPP
                    const char* root = getenv(TUSCANY_SCACPP);
                    if (root != NULL)
                    {
                        loginfo("SCA runtime install root: %s", root);
                        installRoot = root;
                    }
                    else
                    {
                        string msg = TUSCANY_SCACPP;
                        msg += " environment variable not set";
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                }
            
                if (systemRoot == "")
                {
                    // Get root from environment variable TUSCANY_SCACPP_ROOT
                    char* systemRootEnv = getenv(TUSCANY_SCACPP_ROOT);
                    if (systemRootEnv == 0)
                    {
                        // Get root from environment variable TUSCANY_SCACPP_SYSTEM_ROOT
                        systemRootEnv = getenv(TUSCANY_SCACPP_SYSTEM_ROOT);
                    }
                    if (systemRootEnv == 0)
                    {
                        string msg = TUSCANY_SCACPP_ROOT;
                        msg += " environment variable not set";
                        throwException(SystemConfigurationException, msg.c_str());
                    } 

                    loginfo("System root: %s", systemRootEnv);
                    systemRoot = systemRootEnv;
                }
                if (systemPath == "")
                {
                    
                    // Get system path from environment variable TUSCANY_SCACPP_PATH
                    char* systemPathEnv = getenv(TUSCANY_SCACPP_PATH);
                    if (systemPathEnv != 0)
                    {
                        loginfo("System path: %s", systemPathEnv);
                        systemPath = systemPathEnv;
                    }
                }
                if (defaultBaseURI == "")
                {
                    
                    // Get default base URI from environment variable TUSCANY_SCACPP_BASE_URI
                    char* baseURI = getenv(TUSCANY_SCACPP_BASE_URI);
                    if (baseURI != 0)
                    {
                        loginfo("Default base URI: %s", baseURI);
                        defaultBaseURI = baseURI;
                    }
                }

                // Create new instance of the runtime
                instance = new SCARuntime();
                
                // load extensions
                instance->loadExtensions();

            }
            
            return instance;
            
        }


        // =============================================================
        // Release the instance of the runtime.
        // =============================================================
        void SCARuntime::releaseInstance()
        {
            logentry();
            
            if (instance) 
            {
                delete instance;
                instance = 0;
                systemRoot = "";
                systemPath = "";
                defaultComponentName = "";        
            }
        }

        // ======================================
        // Load up all the details of the runtime
        // ======================================
        void SCARuntime::load()
        {
            logentry();
            
            loginfo("Configuration root: %s", systemRoot.c_str());
            loginfo("Configuration path: %s", systemPath.c_str());
            
            // Load the system composite
            ModelLoader loader(system);
            loader.load(systemRoot, systemPath);
        }
        
        
        // ======================================
        // Load up extensions to the runtime
        // ======================================
        void SCARuntime::loadExtensions()
        {
            logentry();

            string extensionsRoot = installRoot + "/extensions";

#if defined(WIN32)  || defined (_WINDOWS)
            string pattern = "*.dll";
#else
#if defined(IS_DARWIN)
            string pattern = "*.dylib";
#else
            string pattern = "*.so";
#endif
#endif
          
            Files files(extensionsRoot, pattern, true);
            for (unsigned int i=0; i < files.size(); i++)
            {
                try
                {
                    Library lib = Library( files[i].getDirectory() + "/" + files[i].getFileName());
                    extensionsList.push_back(lib);                    
                    TUSCANY_IMPLEMENTATION_EXTENSION_INITIALIZE extension = 
                        (TUSCANY_IMPLEMENTATION_EXTENSION_INITIALIZE)lib.getSymbol("tuscany_sca_extension_initialize");
                    if (extension)
                    {
                        extension();
                    }
                }
                catch (TuscanyRuntimeException& ex)
                {
                    logwarning("Failed to load extension library: %s: %s: %s",
                        files[i].getFileName().c_str(), ex.getEClassName(), ex.getMessageText());
                }
            }
        }

      
        // ======================================
        // register an interfaceExtension 
        // ======================================
        void SCARuntime::registerInterfaceExtension(InterfaceExtension* extension)
        {
            logentry();
            if (extension)
            {
                interfaceExtensions[extension->getExtensionTypeQName()] = extension;          
            }
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
            logentry();
            if (extension)
            {
                implementationExtensions[extension->getExtensionTypeQName()] = extension;          
            }
        }
        
        // ======================================
        // find an implementationExtension 
        // ======================================
        ImplementationExtension* SCARuntime::getImplementationExtension(const string& extensionTypeQName)
        {
            return implementationExtensions[extensionTypeQName];
        }
        
        // ======================================
        // register a referenceBindingExtension 
        // ======================================
        void SCARuntime::registerReferenceBindingExtension(ReferenceBindingExtension* extension)
        {
            logentry();
            if (extension)
            {
                referenceBindingExtensions[extension->getExtensionTypeQName()] = extension;          
            }
        }
        
        // ======================================
        // find a referenceBindingExtension 
        // ======================================
        ReferenceBindingExtension* SCARuntime::getReferenceBindingExtension(const string& extensionTypeQName)
        {
            return referenceBindingExtensions[extensionTypeQName];
        }
        
        // ======================================
        // register a serviceBindingExtension 
        // ======================================
        void SCARuntime::registerServiceBindingExtension(ServiceBindingExtension* extension)
        {
            logentry();
            if (extension)
            {
                serviceBindingExtensions[extension->getExtensionTypeQName()] = extension;          
            }
        }
        
        // ======================================
        // find a serviceBindingExtension 
        // ======================================
        ServiceBindingExtension* SCARuntime::getServiceBindingExtension(const string& extensionTypeQName)
        {
            return serviceBindingExtensions[extensionTypeQName];
        }
        
        
        // ===================================
        // Return the top of the runtime model
        // ===================================
        Composite* SCARuntime::getSystem()
        {
            logentry();
            if (!system)
            {
                system = new Composite("tuscany/sca/system", "");
                load();
            }
            return system;
        }


        // ===================================================
        // setCurrentComponent: push component for this thread
        // ===================================================        
        void SCARuntime::setCurrentComponent(Component* component)
        {
            logentry();

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
            logentry();
            
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
            logentry();
            
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
        // getDefaultComponent: return the default composite component
        // ===========================================
        Component* SCARuntime::getDefaultComponent()
        {
            logentry();

            // -------------------------------------------
            // Get the default component name from the environment
            // -------------------------------------------
            if (defaultComponentName == "")
            {
                const char* defComp = getenv(TUSCANY_SCACPP_COMPONENT);
                if (!defComp)
                {
                    defComp = getenv(TUSCANY_SCACPP_DEFAULT_COMPONENT);
                }
                if (!defComp)
                {
                    string message = TUSCANY_SCACPP_COMPONENT;
                    message += " environment variable not set";
                    throwException(SystemConfigurationException, message.c_str());
                }
                defaultComponentName = defComp;
            }
                    
            // -------------------------------------------
            // Get the default component
            // -------------------------------------------
            if (defaultComponent && defaultComponentName != defaultComponent->getName())
            {
                defaultComponent = NULL;
            }
            if (!defaultComponent)
            {
                
                defaultComponent = getSystem()->findComponent(defaultComponentName);
                if (!defaultComponent)
                {
                    string message = "Component \'" + defaultComponentName + "\' not found";
                    throwException(SystemConfigurationException, message.c_str());
                }
            }
            return defaultComponent;        
        }
                
    } // End namespace sca
} // End namespace tuscany
