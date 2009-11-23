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

#ifndef tuscany_sca_model_component_h
#define tuscany_sca_model_component_h

#include <string>
#include <map>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/export.h"


namespace tuscany
{
    namespace sca
    {
        namespace model
        {

            class Composite;
            class ComponentType;
            class Reference;
            class ReferenceType;
            class Service;
            class ServiceType;

            /**
             * A component is a configured instance of an implementation. Components provide
             * and consume services. More than one component can use and configure the same
             * implementation, where each component configures the implementation differently.
             * For example each component may configure a reference of the same implementation
             * to consume a different service.
             */
            class Component
            {
            public:
            
                /**
                 * Constructor
                 * @param composite The composite containing the component.
                 * @param name The name of the component.
                 */
                SCA_API Component(Composite *composite, const std::string& name, ComponentType *type);

                /**
                 * Destructor.
                 */
                SCA_API virtual ~Component();

                /**
                 * Returns the name of this component.
                 * @return the name of this component
                 */
                SCA_API const std::string& getName() const { return name; }
            
                /**
                 * Returns the composite containing this component.
                 * @return The composite containing this component.
                 */
                SCA_API Composite* getComposite() const { return composite; }
            
                /**
                 * Returns the type of this component.
                 * @return The type of this component.
                 */
                SCA_API ComponentType* getType() const { return type; }
                
                /**
                 * Add a new service to this component.
                 * @param service The service to add.
                 */
                SCA_API void addService(Service* service);

                /**
                 * Find an existing service on this component.
                 * @param serviceName The name of the service to find.
                 * If the serviceName is the zero length string then if there is
                 * only one service it will be returned.
                 * @return The found service, or 0 if not found.
                 */
                SCA_API Service* findService(const std::string& serviceName);

                /**
                 * Add a new reference to this component.
                 * @param reference The reference to add.
                 */
                SCA_API void addReference(Reference* reference);

                /**
                 * Find an existing reference on this component.
                 * @param referenceName The name of the reference to find.
                 * @return The found reference, or 0 if not found.
                 */
                SCA_API Reference* findReference(const std::string& referenceName);

                /**
                 * Returns all the services defined on this component.
                 * @return All the services defined on this component.
                 */
                typedef std::map<std::string, Service*> SERVICE_MAP;
                SCA_API const SERVICE_MAP& getServices() const { return services; }; 

                /**
                 * Returns all the references defined on this component.
                 * @return All the references defined on this component.
                 */
                typedef std::map<std::string, Reference*> REFERENCE_MAP;
                SCA_API const REFERENCE_MAP& getReferences() const { return references; }; 
                
                /**
                 * Set the value of a property defined on this component. The values
                 * will usually come from a component declaration in a composite file.
                 * @param name The name of the property.
                 * @param value The value of the property.
                 */
                SCA_API void setProperty(const std::string& name, commonj::sdo::DataObjectPtr value);

                /**
                 * Returns a data object from which all the properties of the component
                 * and their values can be accessed.
                 * @return A data object holding the property values.
                 */
                SCA_API commonj::sdo::DataObjectPtr getProperties();

            private:
                
                /**
                 * Name of the component.
                 */
                std::string name;
                
                /**
                 * Composite containing the component.
                 */
                 Composite* composite;

                /**
                 * Type of the component.
                 */
                ComponentType* type;

                /**
                 * Map of all the services defined on this component.
                 */
                SERVICE_MAP services;

                /**
                 * Map of all the references defined on this component.
                 */
                REFERENCE_MAP references;

                /**
                 * The properties and their values for this component.
                 */ 
                commonj::sdo::DataObjectPtr properties;

           };

        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_component_h
