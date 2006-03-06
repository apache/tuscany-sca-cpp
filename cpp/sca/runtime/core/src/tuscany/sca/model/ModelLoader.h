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

#ifndef tuscany_sca_model_modelloader_h
#define tuscany_sca_model_modelloader_h

#include "osoa/sca/export.h"
#include "commonj/sdo/SDO.h"
using commonj::sdo::DataObjectPtr;
using commonj::sdo::DataFactoryPtr;

#include "tuscany/sca/model/System.h"
#include "tuscany/sca/model/Subsystem.h"

#include "tuscany/sca/util/File.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            /**
             * Provides methods to load the runtime model from the SCDL file.
             */ 
            class ModelLoader {
            public:
                /**
                 * Constructor.
                 * @param system The SCA system to load.
                 */
                ModelLoader(System* system);

                /**
                 * Destructor.
                 */
                virtual ~ModelLoader();
                
                /**
                 * Load the model from the configuration information.
                 * @param configurationRoot The location of the deployed SCA
                 * modules and subsystems.
                 */
                void load(const char *configurationRoot);
                
            private:
                System* system;
                void loadModule(const char *moduleRoot);
                
                
                commonj::sdo::XMLHelperPtr myXMLHelper;    // Used to load scdl files
                commonj::sdo::XSDHelperPtr myXSDHelper; // Used to load xsds
                const commonj::sdo::XSDHelperPtr getXSDHelper(void);
                const commonj::sdo::XMLHelperPtr getXMLHelper(void);
                
                void loadSubsystems(const char *configurationRoot);
                void loadSubsystemFile(const File& file);
                void mapSubsystem(commonj::sdo::DataObjectPtr rootDO);
                
                void loadModules(const char *configurationRoot);
                void loadModuleFile(const File& file);
                void mapModule(const string& moduleName, commonj::sdo::DataObjectPtr rootDO, std::string moduleRootDir);

                void addComponent(Module* module, DataObjectPtr componentDO);
                void addEntryPoint(Module* module, DataObjectPtr entryPointDO);
                void addExternalService(Module* module, DataObjectPtr externalServiceDO);

                void addServices(Component* component, DataObjectPtr componentType);
                void addReferences(Component* component, DataObjectPtr componentType);
                void addProperties(Component* component, DataObjectPtr componentType);


                void loadModuleConfig(const string &moduleRootDir, const string &moduleName);
                void loadTypes(const char *fileName, const string &moduleName);
                void loadWsdl(const char *fileName, const string &moduleName);

                void loadWSDLTypes(DataFactoryPtr dataFactory);
    
                Interface* getInterface(DataObjectPtr obj);

            
            };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_modelloader_h

