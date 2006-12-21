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


namespace tuscany
{
    namespace sca
    {
        
        /**
         * A singleton which represents the executing SCA runtime.
         */
        class SCARuntime {
        public:

            /**
             * Get the single instance.
             * @return The single instance of the runtime.
             */
            SCA_API static SCARuntime* getInstance();

            /**
             * Release the single instance.
             */
            SCA_API static void releaseInstance();
          
            /**
             * Load the SCA configuration from the scdl files (sca.composite, 
             * *.fragment, etc).
             * This will create the runtime model from which the SCA runtime
             * will operate.
             */
            SCA_API void load();

            /**
             * Set the directory in which the Tuscany runtime has been installed.
             */
            SCA_API static void setInstallRoot(const std::string& root);

            /**
             * Returns the directory in which the Tuscany runtime has been installed.
             */
            SCA_API static const std::string& getInstallRoot();

            /**
             * Set the system root
             * @param root The path to the system configuration.
             */
            SCA_API static void setSystemRoot(const std::string& root);

            /**
             * Returns the system root
             */
            SCA_API static const std::string& getSystemRoot();

            /**
             * Set the search path for composites.
             * @param path The search path for composites.
             */
            SCA_API static void setSystemPath(const std::string& path);

            /**
             * Returns the search path for composites.
             */
            SCA_API static const std::string& getSystemPath();

            /**
             * Set the default Component for the system
             * @param componentName The name of the default component.
             */
            SCA_API static void setDefaultComponentName(const std::string& componentName);

            /**
             * Returns the default Component for the system
             */
            SCA_API static const std::string& getDefaultComponentName();

            /**
             * Set the default base URI for the system
             * @param baseURI The default base URI.
             */
            SCA_API static void setDefaultBaseURI(const std::string& baseURI);

            /**
             * Returns the default base URI for the system
             */
            SCA_API static const std::string& getDefaultBaseURI();

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
             * Get the default component set for this runtime.
             * @return The default composite.
             */
            SCA_API tuscany::sca::model::Component* getDefaultComponent();

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
             * Default constructor is private to prevent more than one instance.
             */
            SCARuntime();            

            virtual ~SCARuntime();            

            /**
             * The single instance of this class.
             */
            static SCARuntime* instance;

            /**
             * Pointer to the top of the runtime model.
             */
            tuscany::sca::model::Composite* system;

            /**
             * The installed path of the Tuscany runtime.
             */
            static std::string installRoot;
 
            /**
             * The path to the system configuration
             */
            static std::string systemRoot;

            /**
             * The search path for composites.
             */
            static std::string systemPath;

            /**
             * The default CompositeComponent.
             */
            static std::string defaultComponentName;

            /**
             * The default base URI.
             */
            static std::string defaultBaseURI;

            /**
             * The default component set for this runtime.
             */
            tuscany::sca::model::Component* defaultComponent;
            
            
            typedef std::stack<tuscany::sca::model::Component*> COMPONENT_STACK; 
#if defined(WIN32)  || defined (_WINDOWS)
            typedef std::map<DWORD, COMPONENT_STACK> COMPONENTS_MAP;
#else
            typedef std::map<pthread_t, COMPONENT_STACK> COMPONENTS_MAP;
#endif

            /**
             * A map of threads to components.
             */
            COMPONENTS_MAP components;
 
            typedef std::map<std::string, ImplementationExtension*> IMPLEMENTATION_EXTENSIONS_MAP;
            IMPLEMENTATION_EXTENSIONS_MAP implementationExtensions;

            typedef std::map<std::string, ReferenceBindingExtension*> REFERENCE_BINDING_EXTENSIONS_MAP;
            REFERENCE_BINDING_EXTENSIONS_MAP referenceBindingExtensions;

            typedef std::map<std::string, ServiceBindingExtension*> SERVICE_BINDING_EXTENSIONS_MAP;
            SERVICE_BINDING_EXTENSIONS_MAP serviceBindingExtensions;

            typedef std::map<std::string, InterfaceExtension*> INTERFACE_EXTENSIONS_MAP;
            INTERFACE_EXTENSIONS_MAP interfaceExtensions;

            // Runtime Extensions
            void loadExtensions();

            typedef std::list<tuscany::sca::util::Library> EXTENSIONS_LIST;
            EXTENSIONS_LIST extensionsList;

        };

    } // End namespace sca
} // End namespace tuscany

typedef void (* TUSCANY_IMPLEMENTATION_EXTENSION_INITIALIZE) ();

#endif // tuscany_sca_core_scaruntime_h
