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

#ifndef tuscany_sca_core_scaruntime_h
#define tuscany_sca_core_scaruntime_h

#include "osoa/sca/export.h"

#include "tuscany/sca/extension/InterfaceExtension.h"
#include "tuscany/sca/extension/ImplementationExtension.h"
#include "tuscany/sca/extension/ReferenceBindingExtension.h"
#include "tuscany/sca/extension/ServiceBindingExtension.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/util/Library.h"

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <stack>
#include <string>
#include <map>
#include <list>
using namespace std;

using namespace tuscany::sca::model;


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
             * Set the system root
             * @param root The path to the deployed system.
             */
            static void setSystemRoot(const string& root);

            /**
             * Set the default CompositeComponent for the system
             * @param componentName The name of the default component.
             */
            static void setDefaultComponentName(const string& componentName);

            /**
             * Set the current component for the current thread.
             * @param component The current component.
             */
            SCA_API void setCurrentComponent(Component* component);

            /**
             * Remove the current component from this thread, and return
             * to the previous component (if there was one).
             * @return The previous component.
             */
            SCA_API Component* unsetCurrentComponent();

            /**
             * Get a pointer to the configured SCA system which this
             * SCA runtime represents. 
             * The rest of the SCA configuration can be navigated from
             * the System.
             * @return The configured SCA system.
             */
            SCA_API Composite* getSystem();

            /**
             * The directory in which the Tuscany runtime has been installed.
             */
            SCA_API const string& getInstallRoot() {return SCARoot;}

            /**
             * Return the current component for this thread.
             * @return The current component for this thread.
             */
            SCA_API Component* getCurrentComponent();

            /**
             * Get the default component set for this runtime.
             * @return The default composite.
             */
            SCA_API Component* getDefaultComponent();

            /**
             * Register an implementation extension
             */
            SCA_API void registerImplementationExtension(ImplementationExtension* extension);

            /**
             * Returns the implementation extension associated with
             * the specified qname
             */
            SCA_API ImplementationExtension* getImplementationExtension(const string& typeQname);

            /**
             * Register a reference binding extension
             */
            SCA_API void registerReferenceBindingExtension(ReferenceBindingExtension* extension);

            /**
             * Returns the reference binding extension associated with
             * the specified qname
             */
            SCA_API ReferenceBindingExtension* getReferenceBindingExtension(const string& typeQname);

            /**
             * Register a service binding extension
             */
            SCA_API void registerServiceBindingExtension(ServiceBindingExtension* extension);

            /**
             * Returns the service binding extension associated with
             * the specified qname
             */
            SCA_API ServiceBindingExtension* getServiceBindingExtension(const string& typeQname);

            /**
             * Register an interface extension
             */
            SCA_API void registerInterfaceExtension(InterfaceExtension* extension);

            /**
             * Returns the interface extension associated with
             * the specified qname
             */
            SCA_API InterfaceExtension* getInterfaceExtension(const string& typeQname);

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
            Composite* system;

            /**
             * The installed path of the Tuscany runtime.
             */
            string SCARoot;
 
            /**
             * The path to the system root
             */
            static string systemRoot;

            /**
             * The default CompositeComponent.
             */
            static string defaultComponentName;

            /**
             * The default component set for this runtime.
             */
            Component* defaultComponent;
            
            
            typedef stack<Component*> COMPONENT_STACK; 
#if defined(WIN32)  || defined (_WINDOWS)
            typedef map<DWORD, COMPONENT_STACK> COMPONENTS_MAP;
#else
            typedef map<pthread_t, COMPONENT_STACK> COMPONENTS_MAP;
#endif

            /**
             * A map of threads to components.
             */
            COMPONENTS_MAP components;
 
            typedef map<string, ImplementationExtension*> IMPLEMENTATION_EXTENSIONS_MAP;
            IMPLEMENTATION_EXTENSIONS_MAP implementationExtensions;

            typedef map<string, ReferenceBindingExtension*> REFERENCE_BINDING_EXTENSIONS_MAP;
            REFERENCE_BINDING_EXTENSIONS_MAP referenceBindingExtensions;

            typedef map<string, ServiceBindingExtension*> SERVICE_BINDING_EXTENSIONS_MAP;
            SERVICE_BINDING_EXTENSIONS_MAP serviceBindingExtensions;

            typedef map<string, InterfaceExtension*> INTERFACE_EXTENSIONS_MAP;
            INTERFACE_EXTENSIONS_MAP interfaceExtensions;

            // Runtime Extensions
            void loadExtensions();

            typedef list<Library> EXTENSIONS_LIST;
            EXTENSIONS_LIST extensionsList;

        };

    } // End namespace sca
} // End namespace tuscany

typedef void (* TUSCANY_IMPLEMENTATION_EXTENSION_INITIALIZE) ();

#endif // tuscany_sca_core_scaruntime_h
