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

#ifndef tuscany_sca_model_modelloader_h
#define tuscany_sca_model_modelloader_h

#include <map>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/export.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ComponentType.h"
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
                ModelLoader(tuscany::sca::SCARuntime* runtime, Composite* system);

                /**
                 * Destructor.
                 */
                virtual ~ModelLoader();
                
                /**
                 * Load the model from the configuration information.
                 * @param systemRoot The location of the system configuration
                 * @param systemPath The search path for composites
                 */
                void load(const std::string& systemRoot, const std::string& systemPath);
                
            private:
                void loadSystem(const std::string& systemRoot);
                
                void loadComposites(const std::string& searchPath);
                Composite* loadCompositeFile(const tuscany::sca::util::File& file);
                Composite* mapCompositePass1(const tuscany::sca::util::File& file, commonj::sdo::DataObjectPtr rootDO);
                Composite* mapCompositePass2(const std::string& compositeName, commonj::sdo::DataObjectPtr rootDO);

                void addComponent(Composite* composite, commonj::sdo::DataObjectPtr componentDO);
                void addCompositeService(Composite* composite, commonj::sdo::DataObjectPtr compositeServiceDO);
                void addCompositeReference(Composite* composite, commonj::sdo::DataObjectPtr referenceServiceDO);

                void addServiceTypes(Composite* composite, ComponentType* componentType, commonj::sdo::DataObjectPtr componentTypeDO);
                void addReferenceTypes(Composite* composite, ComponentType* componentType, commonj::sdo::DataObjectPtr componentTypeDO);
                void addPropertyTypes(ComponentType* componentType, commonj::sdo::DataObjectPtr componentTypeDO);

                void loadTypeMetadata(const std::string &compositeRootDir, Composite* composite);
                
                void loadXMLSchema(Composite* composite, const char *fileName);
                void loadWSDLDefinition(Composite* composite, const char *fileName);
    
                Interface* getInterface(Composite* composite, commonj::sdo::DataObjectPtr obj);

                SCARuntime* runtime;

                commonj::sdo::XMLHelperPtr myXMLHelper;    // Used to load scdl files
                commonj::sdo::XSDHelperPtr myXSDHelper; // Used to load xsds
                
                const commonj::sdo::XSDHelperPtr getXSDHelper(void);
                const commonj::sdo::XMLHelperPtr getXMLHelper(void);
                
                /**
                 * The composite describing the composition of the system
                 */
                Composite* system;
            
                /**
                 * Maps of all the composites installed on the system.
                 */
                typedef std::map<std::string, Composite*> COMPOSITE_MODELS;
                COMPOSITE_MODELS compositeModels;
                
                typedef std::map<std::string, commonj::sdo::DataObjectPtr> COMPOSITE_DATAOBJECTS;
                COMPOSITE_DATAOBJECTS compositeDataObjects;

                typedef std::map<std::string, Composite*> COMPOSITE_FILES;
                COMPOSITE_FILES compositeFiles;

            };
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_modelloader_h

