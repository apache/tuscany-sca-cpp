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

#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ComponentType.h"
#include "tuscany/sca/util/File.h"

#include <map>
using std::map;

using namespace commonj::sdo;
using namespace tuscany::sca;


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
                ModelLoader(Composite* system);

                /**
                 * Destructor.
                 */
                virtual ~ModelLoader();
                
                /**
                 * Load the model from the configuration information.
                 * @param configurationRoot The location of the deployed SCA
                 * packages and configuration.
                 */
                void load(const string& systemRoot);
                
            private:
                void loadComposite(const char *compositeRoot);
                
                commonj::sdo::XMLHelperPtr myXMLHelper;    // Used to load scdl files
                commonj::sdo::XSDHelperPtr myXSDHelper; // Used to load xsds
                
                const commonj::sdo::XSDHelperPtr getXSDHelper(void);
                const commonj::sdo::XMLHelperPtr getXMLHelper(void);
                
                void loadConfiguration(const string& configurationRoot);
                Composite* loadConfigurationCompositeFile(const File& file);
                Composite* mapConfigurationComposite(DataObjectPtr rootDO, const string compositeRootDir);
                
                void loadPackages(const string& installRoot);
                Composite* loadPackageCompositeFile(const File& file);
                Composite* mapPackageComposite(DataObjectPtr rootDO, const string compositeRootDir);

                void addComponent(Composite* composite, DataObjectPtr componentDO);
                void addCompositeService(Composite* composite, DataObjectPtr compositeServiceDO);
                void addCompositeReference(Composite* composite, DataObjectPtr referenceServiceDO);

                void addServiceTypes(Composite* composite, ComponentType* componentType, DataObjectPtr componentTypeDO);
                void addReferenceTypes(Composite* composite, ComponentType* componentType, DataObjectPtr componentTypeDO);
                void addPropertyTypes(ComponentType* componentType, DataObjectPtr componentTypeDO);

                void loadTypeMetadata(Composite* composite, const string &compositeRootDir);
                
                void loadXMLSchema(Composite* composite, const char *fileName);
                void loadWSDLDefinition(Composite* composite, const char *fileName);
                void initializeWSDLModel(XSDHelperPtr xsdHelper);
    
                Interface* getInterface(Composite* composite, DataObjectPtr obj);

                SCARuntime* runtime;

                /**
                 * The composite describing the composition of the system
                 * All the composites under the configuration root directory are
                 * included in the system composite.
                 */
                Composite* system;
            
                /**
                 * Map of all the package composites installed on the system.
                 */
                typedef map<string, Composite*> PACKAGES;
                PACKAGES packages;

            };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_modelloader_h

