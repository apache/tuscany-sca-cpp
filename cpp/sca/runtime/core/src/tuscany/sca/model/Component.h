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

#ifndef tuscany_sca_model_component_h
#define tuscany_sca_model_component_h

#include <string>
using std::string;

#include <map>

#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/Implementation.h"
#include "tuscany/sca/model/ServiceReference.h"

#include "commonj/sdo/SDO.h"
using commonj::sdo::DataObjectPtr;
using commonj::sdo::DataFactoryPtr;

namespace tuscany
{
    namespace sca
    {
        namespace model
        {

            class Module;

            /**
             * Information about an SCA component.
             */
            class Component
            {
            public:
                /**
                 * Constructor
                 * @param name The name of the component.
                 * @param module The module containing this component.
                 */
                Component(const std::string& name, Module* module);

                /**
                 * Destructor.
                 */
                virtual ~Component();

                /**
                 * Returns the name of the component.
                 * @return The name of the component.
                 */
                const string& getName() {return name;}

                /** 
                 * Get the module containing this component.
                 * @return The containing module.
                 */
                Module* getModule() {return containingModule;}

                /**
                 * Add a new service to this component.
                 * @param serviceName The name of the service to add.
                 * @return The newly added service.
                 */
                Service* addService(const std::string& serviceName);

                /**
                 * Find an existing service on this component.
                 * @param serviceName The name of the service to find.
                 * If the serviceName is the zero length string then if there is
                 * only one service it will be returned.
                 * @return The found service, or 0 if not found.
                 */
                Service* findService(const std::string& serviceName);

                /**
                 * Add a new reference to this component.
                 * @param referenceName The name of the reference to add.
                 * @return The newly added reference.
                 */
                ServiceReference* addReference(const std::string& referenceName);

                /**
                 * Find an existing reference on this component.
                 * @param referenceName The name of the reference to find.
                 * @return The found reference, or 0 if not found.
                 */
                ServiceReference* findReference(const std::string& referenceName);

                /**
                 * Set the details of the implementation of this component.
                 * @param impl The details of the implementation.
                 */
                void setImplementation(Implementation* impl);

                /**
                 * Returns the details of the implementation of this component.
                 * @return The details of the implementation.
                 */
                Implementation* getImplementation() {return implementation;}

                /**
                 * Add a new property to this component. Properties are 
                 * added one at a time. The property definitions come from the component
                 * type file.
                 * @param name The name of the property.
                 * @param type The full name of the type (including uri and local name).
                 * @param many True if this is a many valued property.
                 * @param required True if this property must have a value set.
                 * @param defaultValue The default value if the property does not have a
                 * value set.
                 */
                void addProperty(const string& name,
                    const string& type,
                    bool many,
                    bool required,
                    const char* defaultValue = 0);
    
                /**
                 * Add the property values to the properties defined on this 
                 * component. The values will come from the sca.module file.
                 * @param properties A data object representing all the values set
                 * for this component.
                 */
                void addProperties(DataObjectPtr properties);

                /**
                 * Returns a data object from which all the properties and their
                 * values can be accessed.
                 * @return A data object holding the property values.
                 */
                DataObjectPtr getProperties();
            private:
                /**
                 * Name of the component.
                 */
                string name;

                /**
                 * Module containing this component for navigating up the tree.
                 */
                Module* containingModule;

                /**
                 * Information about the implementation of this component.
                 */
                Implementation* implementation;

                typedef std::map<std::string, Service*> SERVICE_MAP;
                /**
                 * Map of all the services defined on this component.
                 */
                SERVICE_MAP services;

                typedef std::map<std::string, ServiceReference*> REFERENCE_MAP;
                /**
                 * Map of all the references defined on this component.
                 */
                REFERENCE_MAP references;

                /**
                 * SDO data factory which has all the property types defined from
                 * the component type file
                 */
                DataFactoryPtr propertyFactory;

                /**
                 * Return the SDO data factory which has the property types defined
                 * in it.
                 * @return The data factory.
                 */
                DataFactoryPtr getPropertyDataFactory();

                /**
                 * The properties and their values for this component.
                 */ 
                DataObjectPtr properties;

           };

        } // End namespace model

    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_component_h

