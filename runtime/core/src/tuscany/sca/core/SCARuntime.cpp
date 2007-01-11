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

        // Initialize statics        
        SCARuntime* SCARuntime::sharedRuntime = NULL;
        Mutex SCARuntime::sharedRuntimeLock;
        ThreadLocal SCARuntime::current;
 

        // ===================================================================
        // Constructor for the SCARuntime class. This will 
        // hold all the information about the current runtime.
        // ===================================================================
        SCARuntime::SCARuntime(const string& insRoot,
                const string& sysRoot, const string& sysPath,
                const string& base, const string& defName)  
            :   system(0),
                installRoot(insRoot), systemRoot(sysRoot), systemPath(sysPath),
                defaultBaseURI(base), defaultComponentName(defName)
        { 
            logentry();

            if (installRoot == "")
            {
                // Get install dir from environment variable TUSCANY_SCACPP
                const char* root = getenv(TUSCANY_SCACPP);
                if (root != NULL)
                {
                    installRoot = root;
                }
                else
                {
                    string msg = TUSCANY_SCACPP;
                    msg += " environment variable not set";
                    throwException(SystemConfigurationException, msg.c_str());
                }
            }
            loginfo("SCA runtime install root: %s", installRoot.c_str());
        
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

                systemRoot = systemRootEnv;
            }
            else
            {
                loginfo("System root: %s", systemRoot.c_str());
            }
            
            if (systemPath == "")
            {
                
                // Get system path from environment variable TUSCANY_SCACPP_PATH
                char* systemPathEnv = getenv(TUSCANY_SCACPP_PATH);
                if (systemPathEnv != 0)
                {
                    systemPath = systemPathEnv;
                }
            }
            else
            {
                loginfo("System path: %s", systemPath.c_str());
            }

            if (defaultBaseURI == "")
            {
                
                // Get default base URI from environment variable TUSCANY_SCACPP_BASE_URI
                char* baseURI = getenv(TUSCANY_SCACPP_BASE_URI);
                if (baseURI != 0)
                {
                    defaultBaseURI = baseURI;
                }
            }
            else
            {
                loginfo("Default base URI: %s", defaultBaseURI.c_str());
            }

            if (defaultComponentName == "")
            {
                const char* defComp = getenv(TUSCANY_SCACPP_COMPONENT);
                if (!defComp)
                {
                    defComp = getenv(TUSCANY_SCACPP_DEFAULT_COMPONENT);
                }
                if (defComp)
                {
                    defaultComponentName = defComp;
                }
            }
            loginfo("Default component: %s", defaultComponentName.c_str());
            
            SCARuntime* currentRuntime = (SCARuntime*)current.getValue();
            current.setValue(this);
            try
            {

                // Load the runtime extensions
                loadExtensions();
    
                // Load the system composite
                loadSystem();
            }
            catch (...)
            {
                current.setValue(currentRuntime);
                throw;
            }
            current.setValue(currentRuntime);
    
            // Find the default component
            if (defaultComponentName != "")
            {
                Component* comp = system->findComponent(defaultComponentName);
                if (!comp)
                {
                    string message = "Component \'" + defaultComponentName + "\' not found";
                    throwException(SystemConfigurationException, message.c_str());
                }
                defaultComponent.setValue(comp);
            }
                    
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

        // ==========================================================
        // Returns the system configuration root
        // ==========================================================
        const string& SCARuntime::getSystemRoot()
        {
            return systemRoot;
        }

        // ==========================================================
        // Returns the system path
        // ==========================================================
        const string& SCARuntime::getSystemPath()
        {
            return systemPath;
        }

        // ==========================================================
        // Returns the default component name
        // ==========================================================
        const string& SCARuntime::getDefaultComponentName()
        {
            return defaultComponentName ;
        }

        // ==========================================================
        // Returns the default base URI
        // ==========================================================
        const string& SCARuntime::getDefaultBaseURI()
        {
            return defaultBaseURI;
        }

        // ==========================================================
        // Returns the install root
        // ==========================================================
        const string& SCARuntime::getInstallRoot()
        {
            return installRoot;
        }

        // =============================================================
        // Get the runtime associated with the current thread.
        // =============================================================
        SCARuntime* SCARuntime::getCurrentRuntime()
        {
            logentry();
            
            SCARuntime* runtime = (SCARuntime*)current.getValue();
            if (runtime == NULL)
            {
                runtime = getSharedRuntime();
                if (runtime != NULL)
                {
                    setCurrentRuntime(runtime);
                }
                else
                {
                    runtime = new SCARuntime();
                    setCurrentRuntime(runtime);
                }
            }
            loginfo("Runtime: %p", runtime);            
            return runtime;
        }

        // =============================================================
        // Set the runtime associated with the current thread.
        // =============================================================
        void SCARuntime::setCurrentRuntime(SCARuntime* runtime)
        {
            logentry();

            loginfo("Runtime: %p", runtime);            
            current.setValue(runtime);
        }

        // =============================================================
        // Get the runtime associated with the current process.
        // =============================================================
        SCARuntime* SCARuntime::getSharedRuntime()
        {
            logentry();
            
            sharedRuntimeLock.lock();
            SCARuntime* runtime = sharedRuntime;
            sharedRuntimeLock.unlock();
            
            return runtime;
        }

        // =============================================================
        // Initialize the runtime associated with the current process.
        // =============================================================
        SCARuntime* SCARuntime::initializeSharedRuntime(const string& installRoot, const string& systemRoot,
                const string& systemPath, const string& baseURI, const string& defaultComponentName)
        {
            logentry();

            SCARuntime* runtime;
            
            sharedRuntimeLock.lock();
            try
            {
                if (sharedRuntime == NULL)
                {
                    sharedRuntime = new SCARuntime(installRoot, systemRoot, systemPath, baseURI, defaultComponentName);
                }
                else
                {
                    if (installRoot.size() != 0 && sharedRuntime->getInstallRoot() != installRoot)
                    {
                        string msg = "Cannot reconfigure runtime installation directory: " + string(installRoot);
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                    if (systemRoot.size() != 0 && sharedRuntime->getSystemRoot() != systemRoot)
                    {
                        string msg = "Cannot reconfigure SCA system root: " + string(systemRoot);
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                    if (systemPath.size() != 0 && sharedRuntime->getSystemPath() != systemPath)
                    {
                        string msg = "Cannot reconfigure SCA system path: " + string(systemPath);
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                    if (baseURI.size() != 0 && sharedRuntime->getDefaultBaseURI() != baseURI)
                    {
                        string msg = "Cannot reconfigure SCA system URI: " + string(baseURI);
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                    if (defaultComponentName.size() != 0 && sharedRuntime->getDefaultComponentName() != defaultComponentName)
                    {
                        string msg = "Cannot reconfigure main SCA component: " + string(baseURI);
                        throwException(SystemConfigurationException, msg.c_str());
                    }
                }
                
                runtime = sharedRuntime;
            }
            catch (...)
            {
                sharedRuntimeLock.unlock();
                throw;
            }
            sharedRuntimeLock.unlock();
            
            return runtime;
        }

        // ======================================
        // Load the system composite
        // ======================================
        void SCARuntime::loadSystem()
        {
            logentry();

            system = new Composite("tuscany/sca/system", "");
            ModelLoader loader(this, system);
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

            return system;
        }


        // ===================================================
        // setCurrentComponent: push component for this thread
        // ===================================================        
        void SCARuntime::setCurrentComponent(Component* component)
        {
            logentry();

            COMPONENT_STACK* compStack = (COMPONENT_STACK*)componentStack.getValue();
            if (compStack == NULL)
            {
                compStack = new COMPONENT_STACK();
                componentStack.setValue(compStack);
            }
            compStack->push(component);
        }
        
        // ====================================================
        // unsetCurrentComponent: pop component for this thread
        // ====================================================
        Component* SCARuntime::unsetCurrentComponent()
        {
            logentry();
            
            COMPONENT_STACK* compStack = (COMPONENT_STACK*)componentStack.getValue();
            if (compStack != NULL)
            {
                if (compStack->size() > 0)
                {
                    Component* component = compStack->top();
                    compStack->pop();
                    return component;
                }
            }
            
            return NULL;
        }
        
        // =============================================================
        // getCurrentComponent: return current component for this thread
        // =============================================================
        Component* SCARuntime::getCurrentComponent()
        {
            logentry();
            
            COMPONENT_STACK* compStack = (COMPONENT_STACK*)componentStack.getValue();
            if (compStack != NULL)
            {
                if (compStack->size() > 0)
                {
                    return compStack->top();
                }
            }
            
            return NULL;
        }
        
        // ===========================================
        // getDefaultComponent: return the default composite component
        // ===========================================
        Component* SCARuntime::getDefaultComponent()
        {
            logentry();
            
            Component* comp = (Component*)defaultComponent.getValue();
            if (comp == NULL && defaultComponentName != "")
            {
                comp = system->findComponent(defaultComponentName);
                if (!comp)
                {
                    string message = "Component \'" + defaultComponentName + "\' not found";
                    throwException(SystemConfigurationException, message.c_str());
                }
                defaultComponent.setValue(comp);
            }
            return comp;
        }
                
        // ===========================================
        // Set the default composite component
        // ===========================================
        void SCARuntime::setDefaultComponent(Component* component)
        {
            logentry();
            
            return defaultComponent.setValue(component);
        }
                
    } // End namespace sca
} // End namespace tuscany
