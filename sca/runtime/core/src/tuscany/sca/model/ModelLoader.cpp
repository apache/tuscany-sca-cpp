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

/* $Rev$ $Date$ */

#include "tuscany/sca/export.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/model/ModelLoader.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/CompositeService.h"
#include "tuscany/sca/model/CompositeReference.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/WSDLInterface.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "commonj/sdo/TypeDefinitions.h"
#include "tuscany/sca/util/File.h"


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
            ModelLoader::ModelLoader(Composite* system) : system(system)
            {
                LOGENTRY(1, "ModelLoader::constructor");
                
                runtime = SCARuntime::getInstance();
                
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
            // the SCDL files to the SCA runtime's in memory model.
            // =========================================================
            void ModelLoader::load(const string& systemRoot)
            {
                LOGENTRY(1, "ModelLoader::load");
                LOGINFO_1(2,"system root: %s", systemRoot.c_str());
                
                // The configuration root path will point to a directory structure.
                
                // Implementation packages can occur anywhere under the packages directory
                loadPackages(systemRoot+"/packages");
                
                // The composite files representing the configuration of the system can be located
                // anywhere under the configuration directory.
                loadConfiguration(systemRoot+"/configuration");
                
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
                    Composite* include = loadConfigurationCompositeFile(files[i]);
                    
                    // Include each composite in the system composite
                    system->addInclude(include);
                }
                LOGEXIT(1, "ModelLoader::loadConfiguration");
            }
            
            
            // =====================================================================
            // loadConfigurationFile:
            // This method is called for each composite file found in the configuration
            // folder structure
            // =====================================================================
            Composite* ModelLoader::loadConfigurationCompositeFile(const File& file)
            {
                LOGENTRY(1, "ModelLoader::loadConfigurationCompositeFile");
                LOGINFO_1(2, "configuration filename: %s", file.getFileName().c_str());
                
                Composite* include;
                try
                {
                    string filename = file.getDirectory() + "/" + file.getFileName();
                    XMLDocumentPtr configurationFile = getXMLHelper()->loadFile(filename.c_str());
                    if (configurationFile->getRootDataObject() == 0)
                    {
                        LOGERROR_1(0, "ModelLoader::loadConfigurationCompositeFile: Unable to load file: %s", filename.c_str());
                    }
                    else
                    {    
                        //Utils::printDO(configurationFile->getRootDataObject());
                        include = mapConfigurationComposite(configurationFile->getRootDataObject(), file.getDirectory());        
                    }
                } catch (SDORuntimeException ex) 
                {
                    LOGERROR_1(0, "ModelLoader::loadConfigurationCompositeFile: Exception caught: %s", ex.getMessageText());
                }    
                
                LOGEXIT(1, "ModelLoader::loadConfigurationCompositeFile");
                return include;
            }
            
            // ===============
            // mapConfiguration:
            // ===============
            Composite* ModelLoader::mapConfigurationComposite(DataObjectPtr root, string compositeRootDir)
            {
                LOGENTRY(1, "ModelLoader::mapConfigurationComposite");
                
                LOGINFO_1(2, "ModelLoader::mapConfigurationComposite: Loaded configuration: %s", root->getCString("name"));

                // Create a composite and include it in the system                
                Composite* include = new Composite(root->getCString("name"), compositeRootDir); 
                
                DataObjectList& componentDOs = root->getList("component");
                LOGINFO_1(2, "ModelLoader::mapConfigurationComposite: number of composite components: %d", componentDOs.size());
                
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
                    string implementationType = impl->getType().getName();
                    if (implementationType == "SCAImplementation")
                    {
                        // Add each composite component to the include
                        
                        Composite* composite = packages[impl->getCString("name")];
                        if (!composite)
                        {
                            string message = "Composite not found: ";
                            message = message + impl->getCString("name");
                            throw SystemConfigurationException(message.c_str());
                        }
                        else
                        {
                            Component* component = new Component(include, componentDOs[i]->getCString("name"), composite);
                            include->addComponent(component);
                        }
                    }
                    else
                    {
                        string message = "Atomic component implementation not yet supported in a system include: ";
                        message = message + componentDOs[i]->getCString("name");
                        throw SystemConfigurationException(message.c_str());
                    }
                }
                
                //TODO Add composite services and references
                
                LOGEXIT(1, "ModelLoader::mapConfigurationComposite");
                return include;
            }
            
            
            // =====================================================================
            // loadPackages:
            // Load all the composites from any directory below the packages root.
            // Translate the composite information to the runtime information
            // =====================================================================
            void ModelLoader::loadPackages(const string& installRoot)
            {
                // Get all the composite files
                LOGENTRY(1, "ModelLoader::loadPackages");
                Files files(installRoot, "*.composite", true);
                for (unsigned int i=0; i < files.size(); i++)
                {
                    loadPackageCompositeFile(files[i]);
                }
                LOGEXIT(1, "ModelLoader::loadPackages");
            }
            
            
            // ====================================================================
            // loadPackageCompositeFile:
            // This method is called for each .composite file found in the packages
            // folder structure
            // The location of this composite file will indicate the root of a composite
            // ====================================================================
            Composite* ModelLoader::loadPackageCompositeFile(const File& file)
            {
                LOGENTRY(1, "ModelLoader::loadPackageCompositeFile");
                LOGINFO_1(2, "composite filename: %s", file.getFileName().c_str());

                Composite* composite = NULL;                
                try 
                {
                    string fileName = file.getDirectory() + "/" + file.getFileName();
                    XMLDocumentPtr compositeFile = getXMLHelper()->loadFile(fileName.c_str());
                    if (compositeFile->getRootDataObject() == NULL)
                    {
                        LOGERROR_1(0, "ModelLoader::loadPackageCompositeFile: Unable to load file: %s", fileName.c_str());
                    }
                    else
                    {
                        // Map the SCDL 
                        composite = mapPackageComposite(compositeFile->getRootDataObject(), file.getDirectory());                        

                        // Load the xsd types and wsdl files in the composite
                        loadTypeMetadata(composite, file.getDirectory());
                    }
                    
                } catch (SDORuntimeException ex) 
                {
                    LOGERROR_1(0, "ModelLoader::loadPackageCompositeFile: Exception caught: %s", ex.getMessageText());
                }    
                
                LOGEXIT(1, "ModelLoader::loadPackageCompositeFile");
                return composite;
            }
            
            // ===========
            // mapPackageComposite
            // ===========
            Composite* ModelLoader::mapPackageComposite(DataObjectPtr root, string compositeRootDir)
            {
                LOGENTRY(1, "ModelLoader::mapPackageComposite");
                
                string compositeName = root->getCString("name");
                LOGINFO_2(2, "ModelLoader::mapPackageComposite: Loading composite: %s, root Dir: %s", compositeName.c_str(), compositeRootDir.c_str());
                
                Composite* composite = new Composite(compositeName, compositeRootDir);
                packages[compositeName] = composite; 
                
                // ----------------------------
                // Add components to the composite
                // ----------------------------
                DataObjectList& componentList = root->getList("component");
                int i;
                for (i=0; i < componentList.size(); i++)
                {
                    addComponent(composite, componentList[i]);                        
                }

                // ------------
                // Composite services
                // ------------
                DataObjectList& compositeServiceList = root->getList("service");
                for (i=0; i < compositeServiceList.size(); i++)
                {
                    addCompositeService(composite, compositeServiceList[i]);                        
                }
                    
                // -----------------
                // Composite references
                // -----------------
                DataObjectList& compositeReferenceList = root->getList("reference");
                for (i=0; i < compositeReferenceList.size(); i++)
                {
                    addCompositeReference(composite, compositeReferenceList[i]);
                }

                // -----
                // Wires
                // -----
                DataObjectList& wireList = root->getList("wire");
                for (int l=0; l < wireList.size(); l++)
                {
                    string source = wireList[l]->getCString("source");
                    string target = wireList[l]->getCString("target");
                    composite->addWire(source, target);
                }
                
                composite->resolveWires();
                                
                LOGEXIT(1, "ModelLoader::mapPackageComposite");
                return composite;
            }

            // =================================
            // addComponent:
            // =================================
            void ModelLoader::addComponent(Composite* composite, DataObjectPtr componentDO)
            {
                // -------------------
                // Get the component implementation
                // -------------------
                DataObjectPtr impl = componentDO->getDataObject("implementation");
                if (!impl)
                {
                    string message = "No implementation for component: ";
                    message = message + componentDO->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }

                // Create the component type
                ComponentType* componentType;
                string componentTypeName;
                string componentTypePath;

                string implementationType = impl->getType().getName();
                string implTypeQname = impl->getType().getURI();
                implTypeQname += "#";
                implTypeQname += impl->getType().getName();

                // Locate extension that handles this implementation type
                ImplementationExtension* implExtension = runtime->getImplementationExtension(implTypeQname);
                if (implExtension)
                {
                    componentType = implExtension->getImplementation(composite, impl);
                    
                    // -----------------------
                    // Load the .componentType
                    // -----------------------
                    string typeFileName = composite->getRoot() + "/" + componentType->getName() + ".componentType";

                    // Check that the component type file exists
                    //TODO We need a better and portable way to do this
                    string dirName;
                    string fileName;                    
                    Utils::rTokeniseString("/", typeFileName, dirName, fileName);
                    Files files(dirName, fileName, false);
                    if (files.size() !=0)
                    {
                        try 
                        {
                            XMLDocumentPtr componentTypeFile = getXMLHelper()->loadFile(typeFileName.c_str());
                            if (!componentTypeFile || componentTypeFile->getRootDataObject() == 0)
                            {
                                // Component type files are optional
                                LOGINFO_1(0, "ModelLoader::addComponent: Unable to load file: %s", typeFileName.c_str());
                            }
                            else
                            {                                            
                                //Utils::printDO(componentTypeFile->getRootDataObject());
                                //commonj::sdo::SDOUtils::printDataObject(componentTypeFile->getRootDataObject());
                                addServiceTypes(composite, componentType, componentTypeFile->getRootDataObject());
                                addReferenceTypes(composite, componentType, componentTypeFile->getRootDataObject());
                                addPropertyTypes(componentType, componentTypeFile->getRootDataObject());
                            }
                        } catch (SDORuntimeException& ex) 
                        {
                            LOGERROR_2(0, "ModelLoader::addComponent (%s): Exception caught: %s",
                                typeFileName.c_str(), ex.getMessageText());
                            throw SystemConfigurationException(ex.getMessageText());
                        }
                    }    
                }
                else
                {
                    LOGERROR_1(0, "ModelLoader::addComponent: Unsupported implementation type: %s", implTypeQname.c_str());

                    string message = "Implementation type not supported: ";
                    message = message + implTypeQname;
                    throw SystemConfigurationException(message.c_str());
                }
                
                // First check that references exist, some component types
                // will create all used references automatically
                DataObjectList& refs = componentDO->getList("reference");
                for (int i=0; i<refs.size(); i++)
                {
                    string refName = refs[i]->getCString("name");
                    if (!componentType->findReferenceType(refName))
                    {
                        // Configuration error: reference is not defined
                        string message = "Undefined reference: " + refName;
                        throw SystemConfigurationException(message.c_str());
                    }
                }
                
                // Create the component
                Component* component = new Component(composite, componentDO->getCString("name"), componentType);
                composite->addComponent(component);
                
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
                for (int ri=0; ri<refs.size(); ri++)
                {
                    // ----------------------------------------------------------
                    // Add the reference to the composite wires to be resolved later
                    // ----------------------------------------------------------
                    string refName = refs[ri]->getCString("name");
                    if (!component->findReference(refName))
                    {
                        // Configuration error: reference is not defined
                        string message = "Undefined reference: " + refName;
                        throw SystemConfigurationException(message.c_str());
                    }
                    
                    string src = component->getName() + "/" + refName;

                    // Get the reference value
                    string refValue = refs[ri]->getCString("value");
                    
                    composite->addWire(src, refValue);
                }
            }
            
            // =====================================================================
            // addServiceTypes: add the services to the component type
            // =====================================================================
            void ModelLoader::addServiceTypes(Composite* composite, ComponentType* componentType, DataObjectPtr componentTypeDO)
            {
                DataObjectList& serviceTypes = componentTypeDO->getList("service");
                for (int i=0; i<serviceTypes.size(); i++)
                {
                    Interface* iface = getInterface(composite, serviceTypes[i]);
                    ServiceType* serviceType =  new ServiceType(
                        componentType, serviceTypes[i]->getCString("name"), iface, NULL);
                    componentType->addServiceType(serviceType);
                }
            }
            
            // ===================================================
            // addReferenceTypes: add the references to the component type
            // ===================================================
            void ModelLoader::addReferenceTypes(Composite* composite, ComponentType* componentType, DataObjectPtr componentTypeDO)
            {
                DataObjectList& refs = componentTypeDO->getList("reference");
                for (int i=0; i<refs.size(); i++)
                {
                    ReferenceType::Multiplicity multiplicity;
                    if (refs[i]->isSet("multiplicity"))
                    {
                        string s = refs[i]->getCString("multiplicity");
                        multiplicity = ReferenceType::getMultiplicityFromString(s);
                    }
                    else
                    {
                         multiplicity = ReferenceType::ONE_ONE;
                    }
                    
                    Interface* iface = getInterface(composite, refs[i]);
                    
                    ReferenceType* referenceType = new ReferenceType(
                        componentType, refs[i]->getCString("name"), iface, NULL, multiplicity);
                    componentType->addReferenceType(referenceType);
                                            
                }
            }
            

            // ==============
            // getInterface
            // ==============
            Interface* ModelLoader::getInterface(Composite* composite, DataObjectPtr obj)
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
                
                string typeQname = iface->getType().getURI();
                typeQname += "#";
                typeQname += iface->getType().getName();
                
                if (typeQname == WSDLInterface::typeQName)
                {
                    // Load a WSDL interface
                    string qname = iface->getCString("interface");
                    
                    return new WSDLInterface(qname, true, false);
                }
                else
                {
                    // Locate extension that handles this interface type
                    InterfaceExtension* ifaceExtension = runtime->getInterfaceExtension(typeQname);
                    if (ifaceExtension)
                    {
                        return ifaceExtension->getInterface(composite, iface);
                    }
                    else
                    {
                        // log this for now.
                        LOGERROR_1(1, "ModelLoader::getInterface: Unsupported interface type: %s", typeQname.c_str());
                        return 0;
                    }
                }

            }
            
            // ==============================================
            // addProperties: add properties to the component type
            // ==============================================
            void ModelLoader::addPropertyTypes(ComponentType* componentType, DataObjectPtr componentTypeDO)
            {
                DataObjectList& props = componentTypeDO->getList("property");
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
                    // folks to help understand how to do this...
                    DataObjectPtr defaultValue = props[i]->getDataObject("value");

                    componentType->addPropertyType(name, type, many, &defaultValue);
                }
            }
            
            // ===============================================
            // addCompositeService: add an CompositeService to the composite
            // ===============================================
            void ModelLoader::addCompositeService(Composite* composite, DataObjectPtr compositeServiceDO)
            {

                //Utils::printDO(compositeServiceDO);
                string compositeServiceName = compositeServiceDO->getCString("name");
                
                Interface* iface;
                if (compositeServiceDO->getDataObject("interface"))
                {
                    iface = getInterface(composite, compositeServiceDO);
                }
                else
                {
                    iface = NULL;
                }
                
                ReferenceType::Multiplicity multiplicity;
                if (compositeServiceDO->isSet("multiplicity"))
                {
                    string s = compositeServiceDO->getCString("multiplicity");
                    multiplicity = ReferenceType::getMultiplicityFromString(s);
                }
                else
                {
                     multiplicity = ReferenceType::ONE_ONE;
                }

                CompositeService* compositeService = new CompositeService(
                        composite, compositeServiceName, iface, NULL, false, multiplicity); 
                
                composite->addComponent(compositeService);    

                DataObjectList& refs = compositeServiceDO->getList("reference");
                for (int i=0; i<refs.size(); i++)
                {
                    // ----------------------------------------------------------
                    // Add the reference to the composite wires to be resolved later
                    // ----------------------------------------------------------
                    string targ = refs.getCString(i);
                    composite->addWire(compositeServiceName, targ);
                }

                // Get binding, it will be the first and only binding
                DataObjectList& bindings = compositeServiceDO->getList("binding");
                if (bindings.size()==0)
                {
                    string message = "No binding for compositeService: ";
                    message = message + compositeServiceDO->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }
                DataObjectPtr binding = bindings[0];
                
                // Utils::printDO(binding);
                
                // Determine the binding type
                string bindingType = binding->getType().getName();
                string bindingTypeQname = binding->getType().getURI();
                bindingTypeQname += "#";
                bindingTypeQname += binding->getType().getName();

                // Locate the extension that handles this binding type
                ReferenceBindingExtension* bindingExtension = runtime->getReferenceBindingExtension(bindingTypeQname);
                if (bindingExtension)
                {
                    Reference* reference = compositeService->getReference();
                    ReferenceBinding* referenceBinding = bindingExtension->getReferenceBinding(composite, reference, binding);
                    reference->setBinding(referenceBinding);
                }
                else
                {
                    LOGERROR_1(0, "ModelLoader::addCompositeService: Unsupported binding type: %s", bindingTypeQname.c_str());

                    string message = "Binding type not supported: ";
                    message = message + bindingTypeQname;
                    throw SystemConfigurationException(message.c_str());
                }
            }
            
            
            // =========================================================
            // addCompositeReference: add a CompositeReference to the composite
            // =========================================================
            void ModelLoader::addCompositeReference(Composite* composite, DataObjectPtr compositeReferenceDO)
            {
                string compositeReferenceName = compositeReferenceDO->getCString("name");
                
                Interface* iface;
                if (compositeReferenceDO->getDataObject("interface"))
                {                 
                    iface = getInterface(composite, compositeReferenceDO);
                }
                else
                {
                    iface = NULL;
                }
                
                CompositeReference* compositeReference = new CompositeReference(
                        composite, compositeReferenceName, iface, NULL, false, ReferenceType::ONE_ONE); 
                    
                composite->addComponent(compositeReference);
                
                // Get binding, it will be the first and only binding
                DataObjectList& bindings = compositeReferenceDO->getList("binding");
                if (bindings.size()==0)
                {
                    string message = "No binding for compositeReference: ";
                    message = message + compositeReferenceDO->getCString("name");
                    throw SystemConfigurationException(message.c_str());
                }
                DataObjectPtr binding = bindings[0];
                
                //Utils::printDO(binding);
                
                // Determine the binding type
                string bindingType = binding->getType().getName();
                string bindingTypeQname = binding->getType().getURI();
                bindingTypeQname += "#";
                bindingTypeQname += binding->getType().getName();

                // Locate the extension that handles this binding type
                ServiceBindingExtension* bindingExtension = runtime->getServiceBindingExtension(bindingTypeQname);
                if (bindingExtension)
                {
                    Service *service = compositeReference->getService();
                    ServiceBinding* serviceBinding = bindingExtension->getServiceBinding(composite, service, binding);
                    service->setBinding(serviceBinding);
                }
                else
                {
                    LOGERROR_1(0, "ModelLoader::addCompositeReference: Unsupported binding type: %s", bindingTypeQname.c_str());

                    string message = "Binding type not supported: ";
                    message = message + bindingTypeQname;
                    throw SystemConfigurationException(message.c_str());
                }
            }
            
            
            ///
            /// Use the Tuscany.config file in the composite root directory to
            /// determine which xsds and wsdls to load into a dataFactory.
            ///
            void ModelLoader::loadTypeMetadata(Composite* composite, const string &compositeRootDir)
            {
                LOGENTRY(1, "ModelLoader::loadTypeMetadata");

                // Load the "Tuscany.config" file, if it exists
                Files files(compositeRootDir, "Tuscany.config", false);
                if (files.size() !=0)
                {
                    for (unsigned int i=0; i < files.size(); i++)
                    {
                        string filename = compositeRootDir + "/" + files[i].getFileName();
                        XMLDocumentPtr compositeConfigFile = getXMLHelper()->loadFile(filename.c_str());
                        if (compositeConfigFile->getRootDataObject() == 0)
                        {
                            LOGERROR_1(0, "ModelLoader::loadTypeMetadata: Unable to load file: %s", filename.c_str());
                        }
                        else
                        {    
                            LOGINFO_2(2, "ModelLoader::loadTypeMetadata: Loading composite config for: %s, root Dir: %s", composite->getName().c_str(), compositeRootDir.c_str());
    
                            if(compositeConfigFile->getRootDataObject()->isSet("xsd"))
                            {
                                DataObjectList& xsds = compositeConfigFile->getRootDataObject()->getList("xsd/file");
    
                                for (int i=0; i<xsds.size(); i++)
                                {
                                    if(xsds[i]->isSet("name"))
                                    {
                                        // Load a xsd file -> set the types in the compositeComponents data factory file
                                        string xsdName = compositeRootDir + "/" +xsds[i]->getCString("name");
                                        loadXMLSchema(composite, xsdName.c_str());
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
                                        loadXMLSchema(composite, wsdlName.c_str());
                                        
                                        // Load the contents of the wsdl files
                                        loadWSDLDefinition(composite, wsdlName.c_str());
                                    }
                                }
                            }                            
                        }
                    }
                }
                else
                {
                    // The default scheme is to have no Tuscany.config file, then we simply load all
                    // WSDLs and XSDs that we find under the composite root
                    
                    Files xsdFiles(compositeRootDir, "*.xsd", true);
                    for (unsigned int i=0; i < xsdFiles.size(); i++)
                    {
                        // Load a xsd file -> set the types in the compositeComponents data factory file
                        string xsdName = compositeRootDir + "/" + xsdFiles[i].getFileName();
                        loadXMLSchema(composite, xsdName.c_str());
                        
                    }                    

                    Files wsdlFiles(compositeRootDir, "*.wsdl", true);
                    for (unsigned int wi=0; wi < wsdlFiles.size(); wi++)
                    {
                        // Load a wsdl file -> get the types, then the contents of the wsdl
                        string wsdlName = compositeRootDir + "/" + wsdlFiles[wi].getFileName();
                        loadXMLSchema(composite, wsdlName.c_str());
                        
                        // Load the contents of the wsdl files
                        loadWSDLDefinition(composite, wsdlName.c_str());
                    }                    
                } 
                
                LOGEXIT(1, "ModelLoader::loadTypeMetadata");
            }
            
            
            ///
            /// Use the types from an xsd or wsdl file
            ///
            void ModelLoader::loadXMLSchema(Composite* composite, const char *fileName)
            {
                LOGENTRY(1, "ModelLoader::loadXMLSchema");
                                   
                // Load a xsd file -> set the types in the data factory associated with
                // the composite
                try {
                    composite->getXSDHelper()->defineFile(fileName);                        
                    //Utils::printTypes((*compositeIter)->getXSDHelper()->getDataFactory());
                    
                } catch (SDOTypeNotFoundException ex)
                {
                    LOGERROR_1(0, "ModelLoader: Exception caught: %s", ex.getMessageText());
                    throw ex;
                }
                LOGEXIT(1, "ModelLoader::loadXMLSchema");                            
            }

            ///
            /// Load the web services definition from a wsdl
            ///
            void ModelLoader::loadWSDLDefinition(Composite* composite, const char *fileName)
            {
                LOGENTRY(1, "ModelLoader::loadWSDLDefinition");

                try {
                    // Load the wsdl file
                    XMLDocumentPtr doc = getXMLHelper()->loadFile(fileName);

                    if (doc!=0 && doc->getRootDataObject()!=0) 
                    {
                        //Utils::printDO(doc->getRootDataObject());
                        
                        // Add the root WSDL object to the composite
                        composite->addWSDLDefinition(doc->getRootDataObject());            
                        
                    }
                    else
                    {
                        LOGERROR_1(0, "ModelLoader: Unable to load or parse WSDL %s", fileName);
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
                LOGEXIT(1, "ModelLoader::loadWSDLDefinition");                                            
                
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
                    
                    try {
                    
                        // Load the Assembly model schema    
                        string root = SCARuntime::getInstance()->getInstallRoot();
                        string filename = root + "/xsd/sca.xsd";
                        
                        myXSDHelper->defineFile(filename.c_str());
                        
                        // Tuscany specific xsd for config files
                        filename = root + "/xsd/tuscany.xsd";
                        myXSDHelper->defineFile(filename.c_str());
                     
                        initializeWSDLModel(myXSDHelper);

                        // Load any schema from the extensions directory
                        string extensionsRoot = root + "/extensions";

                        Files files(extensionsRoot, "*.xsd", true);
                        for (unsigned int i=0; i < files.size(); i++)
                        {
                            string extensionSchema = files[i].getDirectory() + "/" + files[i].getFileName();
                            myXSDHelper->defineFile(extensionSchema.c_str());
                        }

                     
                    } catch (SDOTypeNotFoundException ex)
                    {
                        LOGERROR_1(0, "ModelLoader: Exception caught: %s", ex.getMessageText());
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
            
            void ModelLoader::initializeWSDLModel(XSDHelperPtr xsdHelper)
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
