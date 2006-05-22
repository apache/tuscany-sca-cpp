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

#include "osoa/sca/ComponentContext.h"
using osoa::sca::ComponentContext;

#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/System.h"
using namespace tuscany::sca::model;

#if defined(WIN32)  || defined (_WINDOWS)
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <stack>
#include <string>
#include <map>
using namespace std;

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
             * Set the system root
             * @param root The path to the deployed system.
             */
            static void setSystemRoot(const string& root);

            /**
             * Set the default ModuleComponent for the system
             * @param moduleComponent The name of the default moduleComponent.
             */
            static void setDefaultModuleComponent(const string& moduleComponent);

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
            System* getSystem();

            /**
             * The directory in which the Tuscany runtime has been installed.
             */
            const string& getInstallRoot() {return SCARoot;}

            /**
             * Return the current component for this thread.
             * @return The current component for this thread.
             */
            Component* getCurrentComponent();

            /**
             * Get the current module. The current module will either
             * be the module in which the current component for this thread
             * is defined, or it will be the default module if there is no current
             * component. There will not be a current component if a client of
             * the SCA runtime is making a call into the SCA runtime.
             */
            Module* getCurrentModule();

        private:
            /**
             * Default constructor is private to prevent more than one instance.
             */
            SCARuntime();            

            virtual ~SCARuntime();            

            /**
             * Load the SCA configuration from the scdl files (sca.module, 
             * *.fragment, etc).
             * This will create the runtime model from which the SCA runtime
             * will operate.
             * @param configurationRoot The path to the configuration of the 
             * SCA runtime. Under this root will be the subsystems and modules
             * directories.
             */
            void load(const string& configurationRoot);

            /**
             * The single instance of this class.
             */
            static SCARuntime* instance;

            /**
             * Pointer to the top of the runtime model.
             */
            System* system;

            /**
             * The installed path of the Tuscany runtime.
             */
            string SCARoot;
 
            /**
             * The path to the system root
             */
            static string systemRoot;

            /**
             * The default ModuleComponent.
             */
            static string defaultModuleName;

            /**
             * Get the default module set for this runtime.
             * @return The default module.
             */
            Module* getDefaultModule() {return defaultModule;}

            /**
             * The default module set for this runtime.
             */
            Module* defaultModule;
            
            
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
        };

        
    } // End namespace sca
} // End namespace tuscany




#endif // tuscany_sca_core_scaruntime_h

