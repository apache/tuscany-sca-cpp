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

#include "osoa/sca/export.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/model/ModelLoader.h"
#include "tuscany/sca/model/CPPImplementation.h"
#include "tuscany/sca/model/CPPInterface.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/WSBinding.h"


using namespace commonj::sdo;


namespace tuscany
{
    namespace sca
    {
        
        namespace model
        {
            // ===========
            // Constructor
            // ===========
            ModelLoader::ModelLoader(System* system) : system(system)
            {
                LOGENTRY(1, "ModelLoader::constructor");
                
                
                LOGEXIT(1, "ModelLoader::constructor");
            }
            
            // ==========
            // Destructor
            // ==========
            ModelLoader::~ModelLoader()
            {
            }
            
            // =========================================================
            // load: Load the runtime model from the deployed xml files
            // This class has the responsibility for translating from
            // the SCA scdl files to the SCA runtime's in memory model.
            // =========================================================
            void ModelLoader::load(const char* configurationRoot)
            {
                LOGENTRY(1, "ModelLoader::load");
                LOGINFO_1(2,"configuration root: %s", configurationRoot);
                
                // The configuration root path will point to a directory structure:
                //   root/
                // The sca.subsystem files can be located anywhere under this directory
                // structure.
                loadSubsystems(configurationRoot);
                
                // sca.module files represent the root of a module, and can occur anywhere
                // under the directory structure
                loadModules(configurationRoot);
                
                system->resolveWires();
                LOGEXIT(1, "ModelLoader::load");
            }
            
            // ========================================================================
            // loadSubsystems:
            // Load all the subsystems from any directory below the configuration root.
            // Translate the subsystem information to the runtime information
            // ========================================================================
            void ModelLoader::loadSubsystems(const char* configurationRoot)
            {
                // Get all the sca.subsystem files in the module
                LOGENTRY(1, "ModelLoader::loadSubsystems");
                Files files(configurationRoot, "sca.subsystem", true);
                for (unsigned int i=0; i < files.size(); i++)
                {
                    loadSubsystemFile(files[i]);
                }
                LOGEXIT(1, "ModelLoader::loadSubsystems");
            }
            
            
            // =====================================================================
            // loadSubsystemFile:
            // This method is called for each sca.subsystem file found in the module 
            // folder structure
            // =====================================================================
            void ModelLoader::loadSubsystemFile(const File& file)
            {
                LOGENTRY(1, "ModelLoader::loadSubsystemFile");
                LOGINFO_1(2, "subsystem filename: %s", file.getFileName().c_str());
                
                try
                {
                    string filename = file.getDirectory() + "/" + file.getFileName();
                    XMLDocumentPtr subsystemFile = getXMLHelper()->loadFile(filename.c_str());
                    if (subsystemFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::loadSubsystemFile: Unable to load file: %s", filename.c_str());
                    }
                    else
                    {    
                        //Utils::printDO(subsystemFile->getRootDataObject());
                        mapSubsystem(subsystemFile->getRootDataObject());                        
                    }
                } catch (SDORuntimeException ex) 
                {
                    LOGERROR_1(0, "ModelLoader::loadSubsytemFile: Exception caught: %s", ex.getMessageText());
                }    
                
                LOGEXIT(1, "ModelLoader::loadSubsystemFile");
            }
            
            // ===============
            // mapSubsystem:
            // ===============
            void ModelLoader::mapSubsystem(DataObjectPtr root)
            {
                LOGENTRY(1, "ModelLoader::mapSubsystem");
                
                LOGINFO_1(2, "ModelLoader::mapSubsystem: Loaded subsystem: %s", root->getCString("name"));
                
                Subsystem* subsystem;
                subsystem = system->addSubsystem(root->getCString("name"));
                
                DataObjectList& Modules = root->getList("moduleComponent");
                LOGINFO_1(2, "ModelLoader::mapSubsystem: number of module components: %d", Modules.size());
                
                // Iterate over module components
                for (int i=0; i<Modules.size(); i++)
                {
                    // Add each module component to the subsystem
                    Module* Module;
                    Module = subsystem->addModuleComponent(Modules[i]->getCString("name"), Modules[i]->getCString("module"));
                }
                
                
                /// @todo Add external services and entry points
                
                
                LOGEXIT(1, "ModelLoader::mapSubsystem");
            }
            
            
            // =====================================================================
            // loadModules:
            // Load all the modules from any directory below the configuration root.
            // Translate the module information to the runtime information
            // =====================================================================
            void ModelLoader::loadModules(const char* configurationRoot)
            {
                // Get all the sca.module files in the module
                LOGENTRY(1, "ModelLoader::loadModules");
                Files files(configurationRoot, "sca.module", true);
                for (unsigned int i=0; i < files.size(); i++)
                {
                    loadModuleFile(files[i]);
                }
                LOGEXIT(1, "ModelLoader::loadModules");
            }
            
            
            // ====================================================================
            // loadModuleFile:
            // This method is called for each sca.module file found in the module 
            // folder structure
            // The location of this module file will indicate the root of a module
            // ====================================================================
            void ModelLoader::loadModuleFile(const File& file)
            {
                LOGENTRY(1, "ModelLoader::loadModuleFile");
                LOGINFO_1(2, "module filename: %s", file.getFileName().c_str());
                
                try 
                {
                    string filename = file.getDirectory() + "/" + file.getFileName();
                    
                    XMLDocumentPtr moduleFile = getXMLHelper()->loadFile(filename.c_str());
                    if (moduleFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::loadModuleFile: Unable to load file: %s", filename.c_str());
                    }
                    else
                    {
                        string moduleName = moduleFile->getRootDataObject()->getCString("name");
                        mapModule(moduleName, moduleFile->getRootDataObject(), file.getDirectory());                        
                        
                        // --------------------------------------------------------------
                        // Load any module Fragments in the same folder as the sca.module
                        // --------------------------------------------------------------
                        Files files(file.getDirectory(), "*.fragment", false);
                        for (unsigned int i=0; i < files.size(); i++)
                        {
                            filename = file.getDirectory() + "/" + files[i].getFileName();
                            moduleFile = getXMLHelper()->loadFile(filename.c_str());
                            if (moduleFile->getRootDataObject() == 0)
                            {
                                LOGERROR_1(0, "ModelLoader::loadModuleFile: Unable to load file: %s", filename.c_str());
                            }
                            else
                            {    
                                mapModule(moduleName, moduleFile->getRootDataObject(), file.getDirectory());                        
                            }
                        }

                        // Load the xsd types and wsdl files in the module
                        loadModuleConfig(file.getDirectory(), moduleName);
                    }
                    
                } catch (SDORuntimeException ex) 
                {
                    LOGERROR_1(0, "ModelLoader::loadModuleFile: Exception caught: %s", ex.getMessageText());
                }    
                
                LOGEXIT(1, "ModelLoader::loadModuleFile");
            }
            
            // ===========
            // mapModule
            // ===========
            void ModelLoader::mapModule(const string& moduleName, DataObjectPtr root, string moduleRootDir)
            {
                LOGENTRY(1, "ModelLoader::mapModule");
                
                LOGINFO_2(2, "ModelLoader::mapModule: Loading module: %s, root Dir: %s", moduleName.c_str(), moduleRootDir.c_str());
                
                // Find the ModuleComponent(s) that refer to this module. If a ModuleComponent does not refer to this
                // module then ignore it
                MODULE_LIST moduleList = system->findModules(moduleName);
                MODULE_LIST::iterator moduleIter;
                
                for (moduleIter = moduleList.begin();
                moduleIter != moduleList.end();
                moduleIter++ )
                {
                    LOGINFO_1(2, "ModelLoader::mapModule: Loading module details for module component: %s", (*moduleIter)->getName().c_str());
                    
                    string message; // for exceptions
                    // Set module root
                    (*moduleIter)->setRoot(moduleRootDir);
                    
                    // ----------------------------
                    // Add components to the module
                    // ----------------------------
                    DataObjectList& componentList = root->getList("component");
                    int i;
                    for (i=0; i < componentList.size(); i++)
                    {
                        addComponent(*moduleIter, componentList[i]);                        
                    }

                    // ------------
                    // Entry points
                    // ------------
                    DataObjectList& entryPointList = root->getList("entryPoint");
                    for (i=0; i < entryPointList.size(); i++)
                    {
                        addEntryPoint(*moduleIter, entryPointList[i]);                        
                    }
                    
                    
                    // -----------------
                    // External services
                    // -----------------
                    DataObjectList& externalServiceList = root->getList("externalService");
                    for (i=0; i < externalServiceList.size(); i++)
                    {
                        addExternalService(*moduleIter, externalServiceList[i]);
                    }

                    // -----
                    // Wires
                    // -----
                    DataObjectList& wireList = root->getList("wire");
                    for (int l=0; l < wireList.size(); l++)
                    {
                        string source = wireList[l]->getCString("sourceUri");
                        string target = wireList[l]->getCString("targetUri");
                        (*moduleIter)->addWire(source, target);
                    }
                    
                }
                                
                LOGEXIT(1, "ModelLoader::mapModule");
            }

            // =================================
            // addComponent:
            // =================================
            void ModelLoader::addComponent(Module* module, DataObjectPtr componentDO)
            {
                Component* component = module->addComponent(componentDO->getCString("name"));                
                
                string message;

                // -------------------
                // Implementation type
                // -------------------
                DataObjectPtr impl = componentDO->getDataObject("implementation");
                if (!impl)
                {
                    message = "No implementation for component: ";
                    message = message + componentDO->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }
                // Determine the type
                string componentTypeName;
                string implType = impl->getType().getName();
                if (implType == "CPPImplementation")
                {
                    string dll = impl->getCString("dll");
                    string header = impl->getCString("header");
                    string className = impl->getCString("class");
                    CPPImplementation* cppImpl = new CPPImplementation(dll, header, className);
                    componentTypeName = cppImpl->getHeaderStub();
                    component->setImplementation(cppImpl);
                    
                }
                else if (implType == "JavaImplementation")
                {
                }
                
                // -----------------------
                // Load the .componentType
                // -----------------------
                string typeFileName = module->getRoot() + "/" + componentTypeName + ".componentType";
                try 
                {
                    XMLDocumentPtr componentTypeFile = getXMLHelper()->loadFile(typeFileName.c_str());
                    if (componentTypeFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::mapModule: Unable to load file: %s", typeFileName.c_str());
                    }
                    else
                    {                                            
                        //Utils::printDO(componentTypeFile->getRootDataObject());
                        //commonj::sdo::SDOUtils::printDataObject(componentTypeFile->getRootDataObject());
                        addServices(component, componentTypeFile->getRootDataObject());
                        addReferences(component, componentTypeFile->getRootDataObject());
                        addProperties(component, componentTypeFile->getRootDataObject());
                    }
                } catch (SDORuntimeException& ex) 
                {
                    LOGERROR_1(0, "ModelLoader::mapModule: Exception caught: %s", ex.getMessageText());
                    throw SystemConfigurationException(ex.getMessageText());
                }    
                
                // ----------
                // Properties
                // ----------
                DataObjectPtr props = componentDO->getDataObject("properties");
                component->addProperties(props);
                
                // ----------
                // References
                // ----------
                DataObjectPtr refs = componentDO->getDataObject("references");
                if (refs)
                {
                    PropertyList pl = refs->getInstanceProperties();
                    for (int refI=0; refI < pl.size(); refI++)
                    {
                        // ----------------------------------------------------------
                        // Add the reference to the module wires to be resolved later
                        // ----------------------------------------------------------
                        string refName = pl[refI].getName();
                        if (!component->findReference(pl[refI].getName()))
                        {
                            // Configuration error: reference is not defined
                            message = "Undefined reference: " + refName;
                            throw SystemConfigurationException(message.c_str());
                        }
                        
                        string src = component->getName() + "/" + refName;
                        DataObjectList& reflist = refs->getList(pl[refI]);
                        for (int refslistI=0; refslistI<reflist.size(); refslistI++)
                        {
                            string targ = reflist.getCString(refslistI);
                            module->addWire(src, targ);
                        }
                    }
                }
            }
            
            
            // =====================================================================
            // addServices: add the services from the componentType to the component
            // =====================================================================
            void ModelLoader::addServices(Component* component, DataObjectPtr componentType)
            {
                DataObjectList& services = componentType->getList("service");
                for (int i=0; i<services.size(); i++)
                {
                    Service* service = component->addService(services[i]->getCString("name"));
                    service->setInterface(getInterface(services[i]));                    
                }
            }
            
            // ===================================================
            // addReferences: add the references to the component
            // ===================================================
            void ModelLoader::addReferences(Component* component, DataObjectPtr componentType)
            {
                DataObjectList& refs = componentType->getList("reference");
                for (int i=0; i<refs.size(); i++)
                {
                    ServiceReference* serviceRef = component->addReference(refs[i]->getCString("name"));                    
                    string multiplicity = "1..1";
                    if (refs[i]->isSet("multiplicity"))
                    {
                        multiplicity = refs[i]->getCString("multiplicity");
                    }
                    serviceRef->setMultiplicity(multiplicity);
                    serviceRef->setInterface(getInterface(refs[i]));                    
                }
            }
            

            // ==============
            // getInterface
            // ==============
            Interface* ModelLoader::getInterface(DataObjectPtr obj)
            {
                // -----------------
                // get the interface
                // -----------------
                DataObjectPtr iface = obj->getDataObject("interface");
                if (!iface)
                {
                    string message = "No interface for: ";
                    message = message + obj->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }
                
                // Determine the type
                string componentTypeName;
                string ifType = iface->getType().getName();
                if (ifType == "CPPInterface")
                {
                    string header = iface->getCString("header");
                    string className = iface->getCString("class");
                    string scope = iface->getCString("scope");
                    bool remotable = iface->getBoolean("remotable");
                    
                    return new CPPInterface(header, className, scope, remotable);
                }
                else
                {
                    // Error?
                    return 0;
                }
            }
            
            // ==============================================
            // addProperties: add Properties to the component
            // ==============================================
            void ModelLoader::addProperties(Component* component, DataObjectPtr componentType)
            {
                DataObjectList& props = componentType->getList("property");
                for (int i=0; i<props.size(); i++)
                {
                    string name = props[i]->getCString("name");
                    string type = props[i]->getCString("type");
                    bool many=false;
                    if (props[i]->isSet("many"))
                    {
                        many = props[i]->getBoolean("many");
                    }

                    bool required=false;
                    if (props[i]->isSet("required"))
                    {
                        many = props[i]->getBoolean("required");
                    }
                    
                    const char* defaultValue = 0;
                    if (props[i]->isSet("default"))
                    {
                        defaultValue = props[i]->getCString("default");
                    }
                    
                    component->addProperty(name, type, many, required, defaultValue);
                }
            }
            
            // ===============================================
            // addEntryPoint: add an EntryPoint to the module
            // ===============================================
            void ModelLoader::addEntryPoint(Module* module, DataObjectPtr entryPointDO)
            {
                //Utils::printDO(entryPointDO);
                EntryPoint* entryPoint = module->addEntryPoint(entryPointDO->getCString("name"));    

                string multiplicity = "1..1";
                if (entryPointDO->isSet("multiplicity"))
                {
                    multiplicity = entryPointDO->getCString("multiplicity");
                }

                entryPoint->setMultiplicity(multiplicity);
                entryPoint->setInterface(getInterface(entryPointDO));                    

                DataObjectList& refs = entryPointDO->getList("reference");
                for (int i=0; i<refs.size(); i++)
                {
                    // ----------------------------------------------------------
                    // Add the reference to the module wires to be resolved later
                    // ----------------------------------------------------------
                    string targ = refs.getCString(i);
                    module->addWire(entryPoint->getName(), targ);
                }
            }
            
            
            // =========================================================
            // addExternalService: add an ExternalService to the module
            // =========================================================
            void ModelLoader::addExternalService(Module* module, DataObjectPtr externalServiceDO)
            {
                string message;

                ExternalService* externalService = module->addExternalService(externalServiceDO->getCString("name"));    
                // Add the interface
                externalService->setInterface(getInterface(externalServiceDO));
                
                // Get binding, it will be the first and only binding
                DataObjectPtr binding = externalServiceDO->getList("binding")[0];
                if (!binding)
                {
                    message = "No binding for externalService: ";
                    message = message + externalServiceDO->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }
                
                //Utils::printDO(binding);
                
                string uri = binding->getCString("uri");
                
                // Determine the binding type
                string bindingType = binding->getType().getName();
                if (bindingType == "WebServiceBinding")
                {
                    string port = binding->getCString("port");
                    
                    WSBinding* wsBinding = new WSBinding(uri,port);
                    
                    externalService->setBinding(wsBinding);
                    
                }
                else if (bindingType == "SCABinding")
                {
                }
            }
            
            
            
            ///
            /// Use the Tuscany-model.config file in the module root directory to
            /// determine which xsds and wsdls to load into a dataFactory.
            ///
            void ModelLoader::loadModuleConfig(const string &moduleRootDir, const string &moduleName)
            {
                LOGENTRY(1, "ModelLoader::loadModuleConfig");

                // Load the "Tuscany-model.config" file, if it exists
                Files files(moduleRootDir, "Tuscany-model.config", false);
                for (unsigned int i=0; i < files.size(); i++)
                {
                    string filename = moduleRootDir + "/" + files[i].getFileName();
                    XMLDocumentPtr moduleConfigFile = getXMLHelper()->loadFile(filename.c_str());
                    if (moduleConfigFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::loadModuleConfig: Unable to load file: %s", filename.c_str());
                    }
                    else
                    {    
                        LOGINFO_2(2, "ModelLoader::loadModuleConfig: Loading module config for: %s, root Dir: %s", moduleName.c_str(), moduleRootDir.c_str());

                        DataObjectList& xsds = moduleConfigFile->getRootDataObject()->getList("xsd/file");
                        for (int i=0; i<xsds.size(); i++)
                        {
                            
                            // Load a xsd file -> set the types in the moduleComponents data factory file
                            string xsdName = moduleRootDir + "/" +xsds[i]->getCString("name");
                            loadTypes(xsdName.c_str(), moduleName);
                            
                            
                        }
                        
                        DataObjectList& wsdls = moduleConfigFile->getRootDataObject()->getList("wsdl/file");
                        for (int j=0; j<wsdls.size(); j++)
                        {
                            string wsdlName = moduleRootDir + "/" +wsdls[j]->getCString("name");
                            // Load a wsdl file -> get the types, then the contents of the wsdl
                            loadTypes(wsdlName.c_str(), moduleName);
                            
                            // Load the contents of the wsdl files
                            loadWsdl(wsdlName.c_str(), moduleName);

                        }
                            
                    }
                }
            
                
                LOGEXIT(1, "ModelLoader::loadModuleConfig");
            }
            
            
            ///
            /// Use the types from an xsd or wsdl file
            ///
            void ModelLoader::loadTypes(const char *fileName, const string &moduleName)
            {
                
                    
                // Load a xsd file -> set the types in the moduleComponents data factory file

                MODULE_LIST moduleList = system->findModules(moduleName);
                MODULE_LIST::iterator moduleIter;
                
                for (moduleIter = moduleList.begin();    
                moduleIter != moduleList.end();    
                moduleIter++ )
                {
                    try {
                        (*moduleIter)->getXSDHelper()->defineFile(fileName);                        
                        //Utils::printTypes((*moduleIter)->getXSDHelper()->getDataFactory());
                        
                    } catch (SDOTypeNotFoundException ex)
                    {
                        LOGERROR_1(0, "ModuleLoader: Exception caught: %s", ex.getMessageText());
                        throw ex;
                    }
                }
                            
            }

            ///
            /// Load the web services definition from a wsdl
            ///
            void ModelLoader::loadWsdl(const char *fileName, const string &moduleName)
            {

                try {
                    // Load the wsdl file
                    XMLDocumentPtr doc = getXMLHelper()->loadFile(fileName);


                    if (doc->getRootDataObject()!=0) 
                    {
                        MODULE_LIST moduleList = system->findModules(moduleName);
                        MODULE_LIST::iterator moduleIter;
                        
                        for (moduleIter = moduleList.begin();    
                        moduleIter != moduleList.end();    
                        moduleIter++ )
                        {
                            // Add the root object to the module
                            (*moduleIter)->addWsdl(doc->getRootDataObject());            
                            
                        }
                        
                    }
                    else
                    {
                        LOGERROR_1(0, "ModuleLoader: Unable to load %s", fileName);
                    }
                    
                } catch (SDOTypeNotFoundException ex)
                {
                    LOGERROR_1(0, "ModuleLoader: Exception caught: %s", ex.getMessageText());
                    throw ex;
                }
                
                
                
            }
            
            //////////////////////////////////////////////////////////////////////////////
            // Methods used to load the model into memory
            //////////////////////////////////////////////////////////////////////////////
            
            ///
            /// Get an XSDHelper that has the appropriate XSDs already loaded
            ///
            const XSDHelperPtr ModelLoader::getXSDHelper() 
            {
                if (myXSDHelper == 0)
                {
                    
                    // Create an xsd helper
                    myXSDHelper = HelperProvider::getXSDHelper();
                    
                    // Now add to it some xsd files
                    try {
                        string root = SCARuntime::getInstance()->getInstallRoot();
                        string filename = root + "/xsd/sca.xsd";
                        
                        myXSDHelper->defineFile(filename.c_str());
                        
                        // Tuscany specific xsd for config files
                        filename = root + "/xsd/tuscany.xsd";
                        myXSDHelper->defineFile(filename.c_str());
 
                        // Load types derived from WSDL schema
                        loadWSDLTypes(myXSDHelper->getDataFactory());
 
                        //Utils::printTypes(myXSDHelper->getDataFactory());
                        
                    } catch (SDOTypeNotFoundException ex)
                    {
                        LOGERROR_1(0, "ModuleLoader: Exception caught: %s", ex.getMessageText());
                        throw ex;
                    }
                }
                
                
                return myXSDHelper;
            }
            
            
            ///
            /// Get an XMLHelper to load files
            ///
            const XMLHelperPtr ModelLoader::getXMLHelper() 
            {
                if (myXMLHelper == 0) {
                    
                    // Create an xml helper
                    myXMLHelper = HelperProvider::getXMLHelper(getXSDHelper()->getDataFactory());
                    
                }
                
                return myXMLHelper;
            }
            
            void ModelLoader::loadWSDLTypes(DataFactoryPtr dataFactory)
            {
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBinding",
                    false, true, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperation",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationFault",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationMessage",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDocumentation",
                    true, true, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDocumented",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement",
                    false, false, true);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleAttributesDocumented",
                    false, true, true);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented",
                    false, true, true);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tFault",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tImport",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tMessage",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tOperation",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPart",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPort",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPortType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tService",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tTypes",
                    false, false, false);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "definitions",
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "import",
                    "http://schemas.xmlsoap.org/wsdl/", "tImport",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "types",
                    "http://schemas.xmlsoap.org/wsdl/", "tTypes",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "message",
                    "http://schemas.xmlsoap.org/wsdl/", "tMessage",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "portType",
                    "http://schemas.xmlsoap.org/wsdl/", "tPortType",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "binding",
                    "http://schemas.xmlsoap.org/wsdl/", "tBinding",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "service",
                    "http://schemas.xmlsoap.org/wsdl/", "tService",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "input",
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "output",
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "fault",
                    "http://schemas.xmlsoap.org/wsdl/", "tFault",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "output",
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "input",
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "fault",
                    "http://schemas.xmlsoap.org/wsdl/", "tFault",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "arrayType",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "RootType",
                    "required",
                    "commonj.sdo", "Boolean",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBinding",
                    "operation",
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperation",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBinding",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBinding",
                    "type",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperation",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperation",
                    "input",
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationMessage",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperation",
                    "output",
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationMessage",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperation",
                    "fault",
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationFault",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperation",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationFault",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationFault",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationMessage",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tBindingOperationMessage",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "import",
                    "http://schemas.xmlsoap.org/wsdl/", "tImport",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "types",
                    "http://schemas.xmlsoap.org/wsdl/", "tTypes",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "message",
                    "http://schemas.xmlsoap.org/wsdl/", "tMessage",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "portType",
                    "http://schemas.xmlsoap.org/wsdl/", "tPortType",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "binding",
                    "http://schemas.xmlsoap.org/wsdl/", "tBinding",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "service",
                    "http://schemas.xmlsoap.org/wsdl/", "tService",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "targetNamespace",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDefinitions",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tDocumented",
                    "documentation",
                    "http://schemas.xmlsoap.org/wsdl/", "tDocumentation",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement",
                    "required",
                    "commonj.sdo", "Boolean",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleAttributesDocumented",
                    "http://schemas.xmlsoap.org/wsdl/", "tDocumented");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented",
                    "http://schemas.xmlsoap.org/wsdl/", "tDocumented");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tFault",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleAttributesDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tFault",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tFault",
                    "message",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tImport",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleAttributesDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tImport",
                    "namespace",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tImport",
                    "location",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tMessage",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tMessage",
                    "part",
                    "http://schemas.xmlsoap.org/wsdl/", "tPart",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tMessage",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tOperation",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tOperation",
                    "input",
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tOperation",
                    "output",
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tOperation",
                    "fault",
                    "http://schemas.xmlsoap.org/wsdl/", "tFault",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tOperation",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tOperation",
                    "parameterOrder",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleAttributesDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tParam",
                    "message",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPart",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleAttributesDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPart",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPart",
                    "element",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPart",
                    "type",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPort",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPort",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPort",
                    "binding",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPortType",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleAttributesDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPortType",
                    "operation",
                    "http://schemas.xmlsoap.org/wsdl/", "tOperation",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPortType",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tService",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tService",
                    "port",
                    "http://schemas.xmlsoap.org/wsdl/", "tPort",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/", "tService",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/", "tTypes",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibleDocumented");
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "encodingStyle",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tAddress",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBinding",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBody",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFault",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFaultRes",
                    false, false, true);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeaderFault",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tOperation",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tStyleChoice",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "useChoice",
                    false, false, false);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "binding",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBinding",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "operation",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tOperation",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "body",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBody",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "encodingStyle",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "encodingStyle",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "use",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "useChoice",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "namespace",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "fault",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFault",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "header",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "message",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "part",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "use",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "useChoice",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "encodingStyle",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "encodingStyle",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "namespace",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "headerfault",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeaderFault",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "RootType",
                    "address",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tAddress",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tAddress",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tAddress",
                    "location",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBinding",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBinding",
                    "transport",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBinding",
                    "style",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tStyleChoice",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBody",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBody",
                    "parts",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBody",
                    "encodingStyle",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "encodingStyle",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBody",
                    "use",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "useChoice",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBody",
                    "namespace",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFault",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFaultRes");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFault",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFaultRes",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tBody");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFaultRes",
                    "required",
                    "commonj.sdo", "Boolean",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFaultRes",
                    "parts",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFaultRes",
                    "encodingStyle",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "encodingStyle",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFaultRes",
                    "use",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "useChoice",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tFaultRes",
                    "namespace",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    "headerfault",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeaderFault",
                    true, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    "message",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    "part",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    "use",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "useChoice",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    "encodingStyle",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "encodingStyle",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeader",
                    "namespace",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeaderFault",
                    "message",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeaderFault",
                    "part",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeaderFault",
                    "use",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "useChoice",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeaderFault",
                    "encodingStyle",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "encodingStyle",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tHeaderFault",
                    "namespace",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tOperation",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tOperation",
                    "soapAction",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tOperation",
                    "style",
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tStyleChoice",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "tStyleChoice",
                    "commonj.sdo", "String");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap/", "useChoice",
                    "commonj.sdo", "String");
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "RootType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "addressType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "bindingType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "operationType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "urlEncoded",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "urlReplacement",
                    false, false, false);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "RootType",
                    "address",
                    "http://schemas.xmlsoap.org/wsdl/http/", "addressType",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "RootType",
                    "binding",
                    "http://schemas.xmlsoap.org/wsdl/http/", "bindingType",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "RootType",
                    "operation",
                    "http://schemas.xmlsoap.org/wsdl/http/", "operationType",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "RootType",
                    "urlEncoded",
                    "http://schemas.xmlsoap.org/wsdl/http/", "urlEncoded",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "RootType",
                    "urlReplacement",
                    "http://schemas.xmlsoap.org/wsdl/http/", "urlReplacement",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "addressType",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "addressType",
                    "location",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "bindingType",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "bindingType",
                    "verb",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "operationType",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/http/", "operationType",
                    "location",
                    "commonj.sdo", "URI",
                    false, false, true);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "RootType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "contentType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "multipartRelatedType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "tMimeXml",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "tPart",
                    false, true, false);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "RootType",
                    "content",
                    "http://schemas.xmlsoap.org/wsdl/mime/", "contentType",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "RootType",
                    "multipartRelated",
                    "http://schemas.xmlsoap.org/wsdl/mime/", "multipartRelatedType",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "RootType",
                    "mimeXml",
                    "http://schemas.xmlsoap.org/wsdl/mime/", "tMimeXml",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "contentType",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "contentType",
                    "type",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "contentType",
                    "part",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "multipartRelatedType",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "multipartRelatedType",
                    "part",
                    "http://schemas.xmlsoap.org/wsdl/mime/", "tPart",
                    true, false, true);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "tMimeXml",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "tMimeXml",
                    "part",
                    "commonj.sdo", "String",
                    false, false, true);
                dataFactory->addPropertyToType(
                    "http://schemas.xmlsoap.org/wsdl/mime/", "tPart",
                    "name",
                    "commonj.sdo", "String",
                    false, false, true);
            }
            
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
