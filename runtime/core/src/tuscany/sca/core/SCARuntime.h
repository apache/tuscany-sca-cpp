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

#ifndef tuscany_sca_core_scaruntime_h
#define tuscany_sca_core_scaruntime_h

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <stack>
#include <string>
#include <map>
#include <list>

#include "tuscany/sca/export.h"
#include "tuscany/sca/extension/InterfaceExtension.h"
#include "tuscany/sca/extension/ImplementationExtension.h"
#include "tuscany/sca/extension/ReferenceBindingExtension.h"
#include "tuscany/sca/extension/ServiceBindingExtension.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/util/Library.h"
#include "tuscany/sca/util/ThreadLocal.h"
#include "tuscany/sca/util/Mutex.h"


namespace tuscany
{
    namespace sca
    {
        
        /**
         * Represents an executing SCA runtime.
         */
        class SCARuntime {
        public:
        
            /**
             * Constructor
             */
             SCA_API SCARuntime(const std::string& installRoot = "",
                const std::string& systemRoot = "", const std::string& systemPath = "",
                const std::string& baseURI = "", const std::string& defaultComponentName = "");

            /**
             * Destructor
             */
             SCA_API virtual ~SCARuntime();

            /**
             * Get the runtime associated with the current thread.
             * @return The runtime associated with the current thread.
             */
            SCA_API static SCARuntime* getCurrentRuntime();

            /**
             * Get the runtime associated with the current thread.
             * @return The runtime associated with the current thread.
             */
            SCA_API static void setCurrentRuntime(SCARuntime* runtime);

            /**
             * Get the runtime associated with the current process.
             * @return The runtime associated with the current process.
             */
            SCA_API static SCARuntime* getSharedRuntime();

            /**
             * Initialize the runtime associated with the current process.
             * @return The runtime associated with the current process.
             */
            SCA_API static SCARuntime* initializeSharedRuntime(const std::string& installRoot = "",
                const std::string& systemRoot = "", const std::string& systemPath = "",
                const std::string& baseURI = "",  const std::string& defaultComponentName = "");

            /**
             * Returns the directory in which the Tuscany runtime has been installed.
             */
            SCA_API const std::string& getInstallRoot();

            /**
             * Returns the system root
             */
            SCA_API const std::string& getSystemRoot();

            /**
             * Returns the search path for composites.
             */
            SCA_API const std::string& getSystemPath();

            /**
             * Returns the default component name.
             */
            SCA_API const std::string& getDefaultComponentName();

            /**
             * Returns the default base URI for the system
             */
            SCA_API const std::string& getDefaultBaseURI();

            /**
             * Set the current component for the current thread.
             * @param component The current component.
             */
            SCA_API void setCurrentComponent(tuscany::sca::model::Component* component);

            /**
             * Remove the current component from this thread, and return
             * to the previous component (if there was one).
             * @return The previous component.
             */
            SCA_API tuscany::sca::model::Component* unsetCurrentComponent();

            /**
             * Get a pointer to the configured SCA system which this
             * SCA runtime represents. 
             * The rest of the SCA configuration can be navigated from
             * the System.
             * @return The configured SCA system.
             */
            SCA_API tuscany::sca::model::Composite* getSystem();

            /**
             * Return the current component for this thread.
             * @return The current component for this thread.
             */
            SCA_API tuscany::sca::model::Component* getCurrentComponent();

            /**
             * Get the default component set for the current thread.
             * @return The default composite.
             */
            SCA_API tuscany::sca::model::Component* getDefaultComponent();

            /**
             * Set the default component for the current thread.
             * @return The default component.
             */
            SCA_API void setDefaultComponent(tuscany::sca::model::Component* component);

            /**
             * Register an implementation extension
             */
            SCA_API void registerImplementationExtension(ImplementationExtension* extension);

            /**
             * Returns the implementation extension associated with
             * the specified qname
             */
            SCA_API ImplementationExtension* getImplementationExtension(const std::string& typeQname);

            /**
             * Register a reference binding extension
             */
            SCA_API void registerReferenceBindingExtension(ReferenceBindingExtension* extension);

            /**
             * Returns the reference binding extension associated with
             * the specified qname
             */
            SCA_API ReferenceBindingExtension* getReferenceBindingExtension(const std::string& typeQname);

            /**
             * Register a service binding extension
             */
            SCA_API void registerServiceBindingExtension(ServiceBindingExtension* extension);

            /**
             * Returns the service binding extension associated with
             * the specified qname
             */
            SCA_API ServiceBindingExtension* getServiceBindingExtension(const std::string& typeQname);

            /**
             * Register an interface extension
             */
            SCA_API void registerInterfaceExtension(InterfaceExtension* extension);

            /**
             * Returns the interface extension associated with
             * the specified qname
             */
            SCA_API InterfaceExtension* getInterfaceExtension(const std::string& typeQname);

        private:

            /**
             * The runtime associated with the current thread.
             */
            static tuscany::sca::util::ThreadLocal current;
            
            /**
             * The runtime shared by all threads of the current process.
             */
             static tuscany::sca::util::Mutex sharedRuntimeLock;
             static SCARuntime* sharedRuntime;

            /**
             * Pointer to the top of the runtime model.
             */
            tuscany::sca::model::Composite* system;

            /**
             * The installed path of the Tuscany runtime.
             */
            std::string installRoot;
 
            /**
             * The path to the system configuration
             */
            std::string systemRoot;

            /**
             * The search path for composites.
             */
            std::string systemPath;

            /**
             * The default base URI.
             */
            std::string defaultBaseURI;

            /**
             * The default CompositeComponent name.
             */
            std::string defaultComponentName;

            /**
             * Load the SCA configuration from the scdl files (sca.composite, 
             * *.fragment, etc).
             * This will create the runtime model from which the SCA runtime
             * will operate.
             */
            void loadSystem();

            /**
             * Component stack for the current thread.
             */
            typedef std::stack<tuscany::sca::model::Component*> COMPONENT_STACK; 
            tuscany::sca::util::ThreadLocal componentStack;  
 
            /**
             * The default component for the current thread.
             */
            tuscany::sca::util::ThreadLocal defaultComponent;  
 
            /**
             *  Runtime Extensions
             */
            typedef std::map<std::string, ImplementationExtension*> IMPLEMENTATION_EXTENSIONS_MAP;
            IMPLEMENTATION_EXTENSIONS_MAP implementationExtensions;

            typedef std::map<std::string, ReferenceBindingExtension*> REFERENCE_BINDING_EXTENSIONS_MAP;
            REFERENCE_BINDING_EXTENSIONS_MAP referenceBindingExtensions;

            typedef std::map<std::string, ServiceBindingExtension*> SERVICE_BINDING_EXTENSIONS_MAP;
            SERVICE_BINDING_EXTENSIONS_MAP serviceBindingExtensions;

            typedef std::map<std::string, InterfaceExtension*> INTERFACE_EXTENSIONS_MAP;
            INTERFACE_EXTENSIONS_MAP interfaceExtensions;

            void loadExtensions();

            typedef std::list<tuscany::sca::util::Library> EXTENSIONS_LIST;
            EXTENSIONS_LIST extensionsList;

        };

    } // End namespace sca
} // End namespace tuscany

typedef void (* TUSCANY_IMPLEMENTATION_EXTENSION_INITIALIZE) ();

#endif // tuscany_sca_core_scaruntime_h
