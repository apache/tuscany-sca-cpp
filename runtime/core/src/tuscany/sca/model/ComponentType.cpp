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

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/ComponentType.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Reference.h"

#include <iostream>

using namespace commonj::sdo;
using namespace std;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            
           // Constructor
            ComponentType::ComponentType(Composite* composite, const string& name)
            : composite(composite), name(name)
            {
                logentry(); 
            }

            // Destructor
            ComponentType::~ComponentType()
            {
                logentry(); 
            }

            void ComponentType::addServiceType(ServiceType* serviceType)
            {
                logentry(); 
                serviceTypes[serviceType->getName()] = serviceType;
            }
            
            ServiceType* ComponentType::findServiceType(const string& serviceName)
            {
                logentry(); 

                // If serviceName is empty then return the ONLY service
                if (serviceName == "" 
                    && serviceTypes.size() == 1)
                {
                    return serviceTypes.begin()->second;
                }
                else
                {
                    return serviceTypes[serviceName];
                }
            }
            
            void ComponentType::addReferenceType(ReferenceType* referenceType)
            {
                logentry(); 

                referenceTypes[referenceType->getName()] = referenceType;
            }
            
            ReferenceType* ComponentType::findReferenceType(const string& referenceName)
            {
                logentry(); 

                return referenceTypes[referenceName];
            }
            
            void ComponentType::addPropertyType(const string& name,
                const string& type,
                bool many,
                DataObjectPtr* defaultValue)
            {
                logentry(); 

                // Create a Type in the Properties dataFactory
                DataFactoryPtr factory = getPropertyDataFactory();
                
                string typeUri, typeName;
                Utils::tokeniseQName(type, typeUri, typeName);
                
                if (typeUri == "http://www.w3.org/2001/XMLSchema")                                
                {
                    typeUri = Type::SDOTypeNamespaceURI;
                    if (typeName == "string")
                    {
                        typeName = "String";
                    }
                    else if (typeName == "anyType")
                    {
                        typeName = "DataObject";
                    }
                    else if (typeName == "int")
                    {
                        typeName = "Integer";
                    }
                    else if (typeName == "integer")
                    {
                        typeName = "Integer";
                    }
                    else if (typeName == "negativeInteger")
                    {
                        typeName = "Integer";
                    }
                    else if (typeName == "nonNegativeInteger")
                    {
                        typeName = "Integer";
                    }
                    else if (typeName == "positiveInteger")
                    {
                        typeName = "Integer";
                    }
                    else if (typeName == "nonPositiveInteger")
                    {
                        typeName = "Integer";
                    }
                    else if (typeName == "unsignedLong")
                    {
                        typeName = "Integer";
                    }
                    else if (typeName == "unsignedShort")
                    {
                        typeName = "Integer";
                    }
                    else if (typeName == "unsignedInt")
                    {
                        typeName = "Long";
                    }
                    else if (typeName == "long")
                    {
                        typeName = "Long";
                    }
                    else if (typeName == "double")
                    {
                        typeName = "Double";
                    }
                    else if (typeName == "short")
                    {
                        typeName = "Short";
                    }
                    else if (typeName == "unsignedByte")
                    {
                        typeName = "Short";
                    }
                    else if (typeName == "float")
                    {
                        typeName = "Float";
                    }
                    else if (typeName == "boolean")
                    {
                        typeName = "Boolean";
                    }
                    else if (typeName == "byte")
                    {
                        typeName = "Byte";
                    }
                    else if (typeName == "base64Binary")
                    {
                        typeName = "Bytes";
                    }
                    else if (typeName == "hexBinary")
                    {
                        typeName = "Bytes";
                    }
                    else if (typeName == "anyURI")
                    {
                        typeName = "URI";
                    }
                    else if (typeName == "QName")
                    {
                        typeName = "URI";
                    }
                    else
                    {
                        // Default unknown xs: types to string??
                        typeName = "String";
                    }
                }
                else
                {
                    // It's not an XML type
                }

                factory->addPropertyToType(
                    "org/osoa/sca",
                    "Properties",
                    name.c_str(),
                    typeUri.c_str(), 
                    typeName.c_str(),
                    many,
                    false,
                    true);

                // Set the default for a dataType
                if (defaultValue!=NULL)
                {
                    try
                    {
                        const Type& propType = factory->getType(typeUri.c_str(), typeName.c_str());
                        if (propType.isDataType())
                        {
                            factory->setDefault("org/osoa/sca", "Properties",
                                name.c_str(),
                                (char*)(*defaultValue)->getCString(""));
                        }
                    }
                    catch (SDOTypeNotFoundException&)
                    {
                    }
                }
            }
            
            const Property* ComponentType::findPropertyType(const string& propertyName)
            {
                logentry();
                const Property* property = NULL;

                DataFactoryPtr dataFactory = getPropertyDataFactory(); 
                const Type& propertiesType = dataFactory->getType("org/osoa/sca", "Properties");

                try
                {
                    const Property& prop = propertiesType.getProperty(propertyName);
                    property = &prop;
                }
                catch(SDOPropertyNotFoundException)
                {
                    loginfo("Property named %s not found, returning null", propertyName.c_str());
                }
                return property;
            }

            DataFactoryPtr ComponentType::getPropertyDataFactory()
            {
                logentry();

                if (!propertyFactory)
                {
                    propertyFactory = DataFactory::getDataFactory();
                    // Add the root type
                    propertyFactory->addType("org/osoa/sca", "Properties", false, false, false, false);
                }
                return propertyFactory;
            }
            
            void ComponentType::initializeComponent(Component* component)
            {
                logentry();

                for (SERVICETYPE_MAP::iterator iter = serviceTypes.begin();
                iter != serviceTypes.end();
                iter++)
                {
                    Service* service = new Service(component, iter->second);
                    component->addService(service);    
                }
                for (REFERENCETYPE_MAP::iterator refiter = referenceTypes.begin();
                refiter != referenceTypes.end();
                refiter++)
                {
                    Reference* reference = new Reference(component, refiter->second);
                    component->addReference(reference);    
                }
            }
            
        } // End namespace model
    } // End namespace sca
} // End namespace tuscany
