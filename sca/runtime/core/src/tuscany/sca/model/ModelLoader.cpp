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
#include "commonj/sdo/TypeDefinitions.h"


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
            void ModelLoader::load(const string& configurationRoot)
            {
                LOGENTRY(1, "ModelLoader::load");
                LOGINFO_1(2,"system root: %s", configurationRoot.c_str());
                
                // The configuration root path will point to a directory structure.
                
                // The composite files representing the configuration of the system can be located
                // anywhere under the configuration directory.
                loadConfiguration(configurationRoot+"/configuration");
                
                // implementations can occur anywhere under the packages directory
                loadComposites(configurationRoot+"/packages");
                
                system->resolveWires();
                LOGEXIT(1, "ModelLoader::load");
            }
            
            // ========================================================================
            // loadConfiguration:
            // Load all the composite files from any directory below the configuration root.
            // Translate the configuration information to the runtime information
            // ========================================================================
            void ModelLoader::loadConfiguration(const string& configurationRoot)
            {
                // Get all the composite files in the configuration directory
                LOGENTRY(1, "ModelLoader::loadConfiguration");
                Files files(configurationRoot, "*.composite", true);
                for (unsigned int i=0; i < files.size(); i++)
                {
                    loadConfigurationFile(files[i]);
                }
                LOGEXIT(1, "ModelLoader::loadConfiguration");
            }
            
            
            // =====================================================================
            // loadConfigurationFile:
            // This method is called for each composite file found in the configuration
            // folder structure
            // =====================================================================
            void ModelLoader::loadConfigurationFile(const File& file)
            {
                LOGENTRY(1, "ModelLoader::loadConfigurationFile");
                LOGINFO_1(2, "configuration filename: %s", file.getFileName().c_str());
                
                try
                {
                    string filename = file.getDirectory() + "/" + file.getFileName();
                    XMLDocumentPtr configurationFile = getXMLHelper()->loadFile(filename.c_str());
                    if (configurationFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::loadConfigurationFile: Unable to load file: %s", filename.c_str());
                    }
                    else
                    {    
                        //Utils::printDO(configurationFile->getRootDataObject());
                        mapConfiguration(configurationFile->getRootDataObject());                        
                    }
                } catch (SDORuntimeException ex) 
                {
                    LOGERROR_1(0, "ModelLoader::loadSubsytemFile: Exception caught: %s", ex.getMessageText());
                }    
                
                LOGEXIT(1, "ModelLoader::loadConfigurationFile");
            }
            
            // ===============
            // mapConfiguration:
            // ===============
            void ModelLoader::mapConfiguration(DataObjectPtr root)
            {
                LOGENTRY(1, "ModelLoader::mapConfiguration");
                
                LOGINFO_1(2, "ModelLoader::mapConfiguration: Loaded configuration: %s", root->getCString("name"));
                
                Subsystem* subsystem;
                subsystem = system->addSubsystem(root->getCString("name"));
                
                DataObjectList& componentDOs = root->getList("component");
                LOGINFO_1(2, "ModelLoader::mapConfiguration: number of composite components: %d", componentDOs.size());
                
                // Iterate over components
                for (int i=0; i<componentDOs.size(); i++)
                {
                    // Get the composite component implementation
                    DataObjectPtr impl = componentDOs[i]->getDataObject("implementation");
                    if (!impl)
                    {
                        string message = "No implementation for component: ";
                        message = message + componentDOs[i]->getCString("name");
                        throw SystemConfigurationException(message.c_str());
                    }
                    
                    // Determine the implementation type
                    string implType = impl->getType().getName();
                    if (implType == "SCAImplementation")
                    {
                        // Add each composite component to the subsystem
                        Composite* composite;
                        composite = subsystem->addCompositeComponent(componentDOs[i]->getCString("name"), impl->getCString("name"));
                    }
                    else
                    {
                        string message = "Atomic component implementation not yet supported in a subsystem: ";
                        message = message + componentDOs[i]->getCString("name");
                        throw SystemConfigurationException(message.c_str());
                    }
                }
                
                //TODO Add composite services and references
                
                LOGEXIT(1, "ModelLoader::mapConfiguration");
            }
            
            
            // =====================================================================
            // loadComposites:
            // Load all the composites from any directory below the configuration root.
            // Translate the composite information to the runtime information
            // =====================================================================
            void ModelLoader::loadComposites(const string& configurationRoot)
            {
                // Get all the main composite files (named like the directory that contains them)
                LOGENTRY(1, "ModelLoader::loadComposites");
                Files files(configurationRoot, "*.composite", true);
                for (unsigned int i=0; i < files.size(); i++)
                {
                    /*string directoryName = files[i].getDirectory();
                    string fileName = files[i].getFileName();
                    int fl = fileName.length()-10;
                    string compositeName = "/" + fileName.substr(0, fl);
                    fl++;
                    int dl = directoryName.length();
                    if (dl>fl && directoryName.substr(dl-fl, fl) == compositeName)
                    {*/
                        loadCompositeFile(files[i]);
                    //}
                }
                LOGEXIT(1, "ModelLoader::loadComposites");
            }
            
            
            // ====================================================================
            // loadCompositeFile:
            // This method is called for each sca.composite file found in the composite 
            // folder structure
            // The location of this composite file will indicate the root of a composite
            // ====================================================================
            void ModelLoader::loadCompositeFile(const File& file)
            {
                LOGENTRY(1, "ModelLoader::loadCompositeFile");
                LOGINFO_1(2, "composite filename: %s", file.getFileName().c_str());
                
                try 
                {
                    string mainFileName = file.getDirectory() + "/" + file.getFileName();
                    
                    XMLDocumentPtr compositeFile = getXMLHelper()->loadFile(mainFileName.c_str());
                    if (compositeFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::loadCompositeFile: Unable to load file: %s", mainFileName.c_str());
                    }
                    else
                    {
                        string compositeName = compositeFile->getRootDataObject()->getCString("name");
                        mapComposite(compositeName, compositeFile->getRootDataObject(), file.getDirectory());                        
                        
                        // --------------------------------------------------------------
                        // Load any other composites in the same folder as the main composite
                        // --------------------------------------------------------------
                        Files files(file.getDirectory(), "*.composite", false);
                        for (unsigned int i=0; i < files.size(); i++)
                        {
                            string filename = file.getDirectory() + "/" + files[i].getFileName();
                            
                            // Skip the main composite file
                            if (filename == mainFileName)
                                continue;
                                
                            compositeFile = getXMLHelper()->loadFile(filename.c_str());
                            if (compositeFile->getRootDataObject() == 0)
                            {
                                LOGERROR_1(0, "ModelLoader::loadCompositeFile: Unable to load file: %s", filename.c_str());
                            }
                            else
                            {    
                                mapComposite(compositeName, compositeFile->getRootDataObject(), file.getDirectory());                        
                            }
                        }

                        // Load the xsd types and wsdl files in the composite
                        loadCompositeConfig(file.getDirectory(), compositeName);
                    }
                    
                } catch (SDORuntimeException ex) 
                {
                    LOGERROR_1(0, "ModelLoader::loadCompositeFile: Exception caught: %s", ex.getMessageText());
                }    
                
                LOGEXIT(1, "ModelLoader::loadCompositeFile");
            }
            
            // ===========
            // mapComposite
            // ===========
            void ModelLoader::mapComposite(const string& compositeName, DataObjectPtr root, string compositeRootDir)
            {
                LOGENTRY(1, "ModelLoader::mapComposite");
                
                LOGINFO_2(2, "ModelLoader::mapComposite: Loading composite: %s, root Dir: %s", compositeName.c_str(), compositeRootDir.c_str());
                
                // Find the CompositeComponent(s) that refer to this composite. If a CompositeComponent does not refer to this
                // composite then ignore it
                COMPOSITE_LIST compositeList = system->findComposites(compositeName);
                COMPOSITE_LIST::iterator compositeIter;
                
                for (compositeIter = compositeList.begin();
                compositeIter != compositeList.end();
                compositeIter++ )
                {
                    LOGINFO_1(2, "ModelLoader::mapComposite: Loading composite details for composite component: %s", (*compositeIter)->getName().c_str());
                    
                    string message; // for exceptions
                    // Set composite root
                    (*compositeIter)->setRoot(compositeRootDir);
                    
                    // ----------------------------
                    // Add components to the composite
                    // ----------------------------
                    DataObjectList& componentList = root->getList("component");
                    int i;
                    for (i=0; i < componentList.size(); i++)
                    {
                        addComponent(*compositeIter, componentList[i]);                        
                    }

                    // ------------
                    // Entry points
                    // ------------
                    DataObjectList& compositeServiceList = root->getList("service");
                    for (i=0; i < compositeServiceList.size(); i++)
                    {
                        addCompositeServiceType(*compositeIter, compositeServiceList[i]);                        
                    }
                    
                    
                    // -----------------
                    // External services
                    // -----------------
                    DataObjectList& compositeReferenceTypeList = root->getList("reference");
                    for (i=0; i < compositeReferenceTypeList.size(); i++)
                    {
                        addCompositeReferenceType(*compositeIter, compositeReferenceTypeList[i]);
                    }

                    // -----
                    // Wires
                    // -----
                    DataObjectList& wireList = root->getList("wire");
                    for (int l=0; l < wireList.size(); l++)
                    {
                        string source = wireList[l]->getCString("source");
                        string target = wireList[l]->getCString("target");
                        (*compositeIter)->addWire(source, target);
                    }
                    
                }
                                
                LOGEXIT(1, "ModelLoader::mapComposite");
            }

            // =================================
            // addComponent:
            // =================================
            void ModelLoader::addComponent(Composite* composite, DataObjectPtr componentDO)
            {
                Component* component = composite->addComponent(componentDO->getCString("name"));                
                
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
                string componentTypePath;
                string implType = impl->getType().getName();
                if (implType == "CPPImplementation")
                {
                    string library = impl->getCString("library");
                    string header = impl->getCString("header");
                    string className = impl->getCString("class");
                    CPPImplementation* cppImpl = new CPPImplementation(library, header, className);
                    componentTypePath = cppImpl->getHeaderPath();
                    componentTypeName = cppImpl->getHeaderStub();
                    component->setImplementation(cppImpl);
                    
                }
                else if (implType == "JavaImplementation")
                {
                }
                
                // -----------------------
                // Load the .componentType
                // -----------------------
                string typeFileName = composite->getRoot() + "/" + componentTypePath + componentTypeName + ".componentType";
                try 
                {
                    XMLDocumentPtr componentTypeFile = getXMLHelper()->loadFile(typeFileName.c_str());
                    if (!componentTypeFile || componentTypeFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::addComponent: Unable to load file: %s", typeFileName.c_str());
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
                    LOGERROR_2(0, "ModelLoader::addComponent (%s): Exception caught: %s",
                        typeFileName.c_str(), ex.getMessageText());
                    throw SystemConfigurationException(ex.getMessageText());
                }    

                
                // ----------
                // Properties
                // ----------
                DataObjectList& props = componentDO->getList("property");
                for (int pi=0; pi<props.size(); pi++)
                {
                    string propName = props[pi]->getCString("name");
                    DataObjectPtr propValue = props[pi]->getDataObject("value");

                    component->setProperty(propName, propValue);
                }
                
                // ----------
                // References
                // ----------
                DataObjectList& refs = componentDO->getList("reference");
                for (int i=0; i<refs.size(); i++)
                {
                    // ----------------------------------------------------------
                    // Add the reference to the composite wires to be resolved later
                    // ----------------------------------------------------------
                    string refName = refs[i]->getCString("name");
                    if (!component->findReference(refName))
                    {
                        // Configuration error: reference is not defined
                        message = "Undefined reference: " + refName;
                        throw SystemConfigurationException(message.c_str());
                    }
                    
                    string src = component->getName() + "/" + refName;

                    // Get the reference value
                    string refValue = refs[i]->getCString("value");
                    
                    composite->addWire(src, refValue);
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
                    //TODO add support for other interface types
                    //string message = "Interface type not yet supported: ";
                    //message = message + ifType;
                    //throw SystemConfigurationException(message.c_str());
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
                    //cout << "Property " << props[i];
                    
                    string name = props[i]->getCString("name");
                    string type = props[i]->getCString("type");
                    bool many=false;
                    if (props[i]->isSet("many"))
                    {
                        many = props[i]->getBoolean("many");
                    }
                    
                    //TODO need to add support for complex properties, need the SDO
                    // folks to shed some light on how to do this...
                    DataObjectPtr defaultValue;
                    if (props[i]->isSet("value")) {
                        defaultValue = props[i]->getDataObject("value");
                    }

                    component->addProperty(name, type, many, defaultValue);
                }
            }
            
            // ===============================================
            // addCompositeServiceType: add an CompositeServiceType to the composite
            // ===============================================
            void ModelLoader::addCompositeServiceType(Composite* composite, DataObjectPtr compositeServiceDO)
            {
				string message;

                //Utils::printDO(compositeServiceDO);
                CompositeServiceType* compositeServiceType = composite->addCompositeServiceType(compositeServiceDO->getCString("name"));    

                string multiplicity = "1..1";
                if (compositeServiceDO->isSet("multiplicity"))
                {
                    multiplicity = compositeServiceDO->getCString("multiplicity");
                }

                compositeServiceType->setMultiplicity(multiplicity);
                compositeServiceType->setInterface(getInterface(compositeServiceDO));                    

                DataObjectList& refs = compositeServiceDO->getList("reference");
                for (int i=0; i<refs.size(); i++)
                {
                    // ----------------------------------------------------------
                    // Add the reference to the composite wires to be resolved later
                    // ----------------------------------------------------------
                    string targ = refs.getCString(i);
                    composite->addWire(compositeServiceType->getName(), targ);
                }

                // Get binding, it will be the first and only binding
                DataObjectList& bindings = compositeServiceDO->getList("binding");
                if (bindings.size()==0)
                {
                    message = "No binding for compositeService: ";
                    message = message + compositeServiceDO->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }
                DataObjectPtr binding = bindings[0];
                
                // Utils::printDO(binding);
                
                string uri = binding->getCString("uri");
                
                // Determine the binding type
                string bindingType = binding->getType().getName();
                if (bindingType == "WebServiceBinding")
                {
                    string port = binding->getCString("port");
                    
                    WSBinding* wsBinding = new WSBinding(uri,port);
                    
                    compositeServiceType->setBinding(wsBinding);
                    
                }
                else if (bindingType == "SCABinding")
                {
                    message = "SCA binding not yet implemented. Binding is for compositeService: ";
                    message = message + compositeServiceDO->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }
            }
            
            
            // =========================================================
            // addCompositeReferenceType: add an CompositeReferenceType to the composite
            // =========================================================
            void ModelLoader::addCompositeReferenceType(Composite* composite, DataObjectPtr compositeReferenceTypeDO)
            {
                string message;

                CompositeReferenceType* compositeReferenceType = composite->addCompositeReferenceType(compositeReferenceTypeDO->getCString("name"));
                    
                // Add the interface
                compositeReferenceType->setInterface(getInterface(compositeReferenceTypeDO));
                
                // Get binding, it will be the first and only binding
                DataObjectList& bindings = compositeReferenceTypeDO->getList("binding");
                if (bindings.size()==0)
                {
                    message = "No binding for compositeReferenceType: ";
                    message = message + compositeReferenceTypeDO->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }
                DataObjectPtr binding = bindings[0];
                
                //Utils::printDO(binding);
                
                string uri = binding->getCString("uri");
                
                // Determine the binding type
                string bindingType = binding->getType().getName();
                if (bindingType == "WebServiceBinding")
                {
                    string port = binding->getCString("port");
                    
                    WSBinding* wsBinding = new WSBinding(uri,port);
                    
                    compositeReferenceType->setBinding(wsBinding);
                    
                }
                else if (bindingType == "SCABinding")
                {
                }
            }
            
            
            
            ///
            /// Use the Tuscany-model.config file in the composite root directory to
            /// determine which xsds and wsdls to load into a dataFactory.
            ///
            void ModelLoader::loadCompositeConfig(const string &compositeRootDir, const string &compositeName)
            {
                LOGENTRY(1, "ModelLoader::loadCompositeConfig");

                // Load the "Tuscany-model.config" file, if it exists
                Files files(compositeRootDir, "Tuscany-model.config", false);
                for (unsigned int i=0; i < files.size(); i++)
                {
                    string filename = compositeRootDir + "/" + files[i].getFileName();
                    XMLDocumentPtr compositeConfigFile = getXMLHelper()->loadFile(filename.c_str());
                    if (compositeConfigFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::loadCompositeConfig: Unable to load file: %s", filename.c_str());
                    }
                    else
                    {    
                        LOGINFO_2(2, "ModelLoader::loadCompositeConfig: Loading composite config for: %s, root Dir: %s", compositeName.c_str(), compositeRootDir.c_str());

                        if(compositeConfigFile->getRootDataObject()->isSet("xsd"))
                        {
                            DataObjectList& xsds = compositeConfigFile->getRootDataObject()->getList("xsd/file");

                            for (int i=0; i<xsds.size(); i++)
                            {
                                if(xsds[i]->isSet("name"))
                                {
                                    // Load a xsd file -> set the types in the compositeComponents data factory file
                                    string xsdName = compositeRootDir + "/" +xsds[i]->getCString("name");
                                    loadTypes(xsdName.c_str(), compositeName);
                                }
                            }
                        }
                        

                        if( compositeConfigFile->getRootDataObject()->isSet("wsdl"))
                        {
                            DataObjectList& wsdls = compositeConfigFile->getRootDataObject()->getList("wsdl/file");
                            for (int j=0; j<wsdls.size(); j++)
                            {
                                if(wsdls[i]->isSet("name"))
                                { 
                                    string wsdlName = compositeRootDir + "/" +wsdls[j]->getCString("name");
                                    // Load a wsdl file -> get the types, then the contents of the wsdl
                                    loadTypes(wsdlName.c_str(), compositeName);
                                    
                                    // Load the contents of the wsdl files
                                    loadWsdl(wsdlName.c_str(), compositeName);
                                }
                            }
                        }                            
                    }
                }
            
                
                LOGEXIT(1, "ModelLoader::loadCompositeConfig");
            }
            
            
            ///
            /// Use the types from an xsd or wsdl file
            ///
            void ModelLoader::loadTypes(const char *fileName, const string &compositeName)
            {
                LOGENTRY(1, "ModelLoader::loadTypes");
                                   
                // Load a xsd file -> set the types in the compositeComponents data factory file

                COMPOSITE_LIST compositeList = system->findComposites(compositeName);
                COMPOSITE_LIST::iterator compositeIter;
                
                for (compositeIter = compositeList.begin();    
                compositeIter != compositeList.end();    
                compositeIter++ )
                {
                    try {
                        (*compositeIter)->getXSDHelper()->defineFile(fileName);                        
                        //Utils::printTypes((*compositeIter)->getXSDHelper()->getDataFactory());
                        
                    } catch (SDOTypeNotFoundException ex)
                    {
                        LOGERROR_1(0, "CompositeLoader: Exception caught: %s", ex.getMessageText());
                        throw ex;
                    }
                }
                LOGEXIT(1, "ModelLoader::loadTypes");                            
            }

            ///
            /// Load the web services definition from a wsdl
            ///
            void ModelLoader::loadWsdl(const char *fileName, const string &compositeName)
            {
                LOGENTRY(1, "ModelLoader::loadWsdl");

                try {
                    // Load the wsdl file
                    XMLDocumentPtr doc = getXMLHelper()->loadFile(fileName);

                    if (doc!=0 && doc->getRootDataObject()!=0) 
                    {
                        //Utils::printDO(doc->getRootDataObject());
                        COMPOSITE_LIST compositeList = system->findComposites(compositeName);
                        COMPOSITE_LIST::iterator compositeIter;
                        
                        for (compositeIter = compositeList.begin();    
                        compositeIter != compositeList.end();    
                        compositeIter++ )
                        {
                            // Add the root object to the composite
                            (*compositeIter)->addWsdl(doc->getRootDataObject());            
                            
                        }
                        
                    }
                    else
                    {
                        LOGERROR_1(0, "CompositeLoader: Unable to load or parse WSDL %s", fileName);
                    }
                    
                } catch (SDOTypeNotFoundException ex)
                {
                    LOGERROR_1(0, "ModelLoader: SDOTypeNotFoundException caught: %s", ex.getMessageText());
                    throw ex;
                }
                catch (SDONullPointerException ex)
                {
                    LOGERROR_1(0, "ModelLoader: SDONullPointerException caught: %s", ex.getMessageText());
                    throw ex;
                }
                LOGEXIT(1, "ModelLoader::loadWsdl");                                            
                
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
                     
                        loadWSDLTypes(myXSDHelper);
                     
                    } catch (SDOTypeNotFoundException ex)
                    {
                        LOGERROR_1(0, "CompositeLoader: Exception caught: %s", ex.getMessageText());
                        throw ex;
                    }
                }
                
                //cout << myXSDHelper->getDataFactory();
                
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
            
            void ModelLoader::loadWSDLTypes(XSDHelperPtr xsdHelper)
            {
                DataFactoryPtr dataFactory = xsdHelper->getDataFactory();
 
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


                TypeDefinition tpart;
                tpart.setUri("http://schemas.xmlsoap.org/wsdl/");
                tpart.setName("tPart");
                tpart.setParentType("http://schemas.xmlsoap.org/wsdl/",
                                    "tExtensibleAttributesDocumented", false);
                PropertyDefinition propdef;
                propdef.setName("name");
                propdef.setLocalName("name");
                propdef.setType("commonj.sdo", "String");
                propdef.setIsContainment(true);
                tpart.addPropertyDefinition(propdef);
                propdef.setName("element");
                propdef.setLocalName("element");
                propdef.setIsQName(true);
                tpart.addPropertyDefinition(propdef);
                propdef.setName("type");
                propdef.setLocalName("type");
                tpart.addPropertyDefinition(propdef);
                TypeDefinitions typedefs;
                typedefs.addTypeDefinition(tpart);
                xsdHelper->defineTypes(typedefs);

                //Utils::printTypes(dataFactory);
                /*dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/", "tPart",
                    false, false, false);*/

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
                /*dataFactory->addPropertyToType(
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
                    false, false, true);*/
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

                
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "RootType",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tAddress",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tBinding",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tBody",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tExtensibilityElementOpenAttrs",
                    false, true, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tFault",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tFaultRes",
                    false, false, true);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tHeader",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tHeaderFault",
                    false, true, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tOperation",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tParts",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tStyleChoice",
                    false, false, false);
                dataFactory->addType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "useChoice",
                    false, false, false);
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tAddress",
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tExtensibilityElementOpenAttrs");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tBinding",
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tExtensibilityElementOpenAttrs");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tBody",
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tExtensibilityElementOpenAttrs");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tExtensibilityElementOpenAttrs",
                    "http://schemas.xmlsoap.org/wsdl/", "tExtensibilityElement");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tFault",
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tFaultRes");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tFaultRes",
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tBody");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tHeader",
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tExtensibilityElementOpenAttrs");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tOperation",
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tExtensibilityElementOpenAttrs");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "tStyleChoice",
                    "commonj.sdo", "String");
                dataFactory->setBaseType(
                    "http://schemas.xmlsoap.org/wsdl/soap12/", "useChoice",
                    "commonj.sdo", "String");  
            }
            
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
