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

#ifndef tuscany_sca_model_componenttype_h
#define tuscany_sca_model_componenttype_h

#include <string>
using std::string;

#include <map>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/model/ReferenceType.h"

namespace tuscany
{
    namespace sca
    {
        namespace model
        {
            class ServiceType;
            class ReferenceType;
            class Interface;
            class Component;

            /**
             * Component type represents the configurable aspects of an implementation.
             * A component type consists of services that are offered, references to other services
             * that can be wired and properties that can be set. The settable properties and the settable
             * references to services are configured by a component which uses the implementation.
             * The component type can be thought of as the contract which is honoured by an implementation.
             */
            class ComponentType
            {
            public:
             
                /**
                 * Constructor
                 */
                ComponentType(const string& name);

                /**
                 * Destructor.
                 */
                virtual ~ComponentType();
                
                /**
                 * Returns the name of the component type
                 */
                 const string& getName() { return name; };

                /**
                 * Add a new service type to this component type.
                 * @param serviceType The service type to add.
                 */
                void addServiceType(ServiceType* serviceType);

                /**
                 * Find an existing service type on this component type.
                 * @param serviceName The name of the service type to find.
                 * If the serviceName is the zero length string then if there is
                 * only one service type it will be returned.
                 * @return The found service, or 0 if not found.
                 */
                ServiceType* findServiceType(const string& serviceName);

                /**
                 * Add a new reference type to this component type.
                 * @param referenceType The reference type to add.
                 */
                void addReferenceType(ReferenceType* referenceType);

                /**
                 * Find an existing reference type on this component type.
                 * @param referenceName The name of the reference type to find.
                 * @return The found reference type, or 0 if not found.
                 */
                ReferenceType* findReferenceType(const string& referenceName);

                /**
                 * Returns the service types defined on this component.
                 * @return The service types defined on this component.
                 */
                typedef std::map<std::string, ServiceType*> SERVICETYPE_MAP;
                SERVICETYPE_MAP getServiceTypes() const { return serviceTypes; }; 
                                            
                /**
                 * Returns the reference types defined on this component.
                 * @return The reference types defined on this component.
                 */
                typedef std::map<std::string, ReferenceType*> REFERENCETYPE_MAP;
                REFERENCETYPE_MAP getReferenceTypes() const { return referenceTypes; };                
            
                /**
                 * Add a new property type to this component type. Property types are added
                 * one at a time. The property definitions usually come from a component type file.
                 * @param name The name of the property type.
                 * @param type The full name of the property data type (including uri and local name).
                 * @param many True if this is a many valued property.
                 * @param defaultValue The default value if the property does not have a
                 * value set.
                 */
                void addPropertyType(const string& name,
                    const string& type,
                    bool many,
                    commonj::sdo::DataObjectPtr defaultValue);
    
                /**
                 * Return the SDO data factory which has the types of the properties defined
                 * in this component type.
                 * @return The data factory.
                 */
                commonj::sdo::DataFactoryPtr getPropertyDataFactory();

                /**
                 * Initialize a component of this type.
                 * @param component The component to initialize.
                 */
                virtual void initializeComponent(Component* component);

            private:
            
                /**
                 * The name of the component type
                 */
                string name;
                
                /**
                 * Map of all the service types defined on this component.
                 */
                SERVICETYPE_MAP serviceTypes;

                /**
                 * Map of all the reference types defined on this component.
                 */
                REFERENCETYPE_MAP referenceTypes;

                /**
                 * SDO data factory which has all the types of the properties defined in
                 * this component type
                 */
                commonj::sdo::DataFactoryPtr propertyFactory;

           };

        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_componenttype_h

