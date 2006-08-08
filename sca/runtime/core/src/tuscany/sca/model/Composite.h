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

#ifndef tuscany_sca_model_composite_h
#define tuscany_sca_model_composite_h

#include <string>
using std::string;

#include <map>
using std::map;
#include <vector>
using std::vector;

#include "commonj/sdo/SDO.h"


#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ExternalService.h"
#include "tuscany/sca/model/EntryPoint.h"
#include "tuscany/sca/model/Wire.h"
#include "tuscany/sca/model/Wsdl.h"


namespace tuscany
{
    namespace sca
    {

        namespace model
        {
            /**
             * Information about a composite.
             */
            class Composite
            {
            public:
                /**
                 * Constructor.
                 * @param name the name of the composite.
                 */
                Composite(const std::string& name);

                /**
                 * Destructor.
                 */
                virtual ~Composite();

                /**
                 * Set the root directory of the composite information.
                 * @param rootDirectory The root of the composite in the file system.
                 */
                void setRoot(const std::string& rootDirectory);

                /**
                 * Get the root directory of the composite.
                 * @return The root of the composite in the file system.
                 */
                const std::string& getRoot() {return compositeRoot;}
                
                /**
                 * Return the name of the composite.
                 * @return Name of the composite.
                 */
                const std::string& getName() {return name;}

                /**
                 * Add a new component to the composite.
                 * @param componentName The name of the new component.
                 * @return The Component added to the composite.
                 */
                Component* addComponent(const std::string& componentName);

                /**
                 * Add a new entry point to the composite.
                 * @param name The name of the new entry point.
                 * @return The entry point added to the composite.
                 */
                EntryPoint* addEntryPoint(const std::string& name);

                /**
                 * Find an entry point by name.
                 * @param name The name of the entry point to be found.
                 * @return The entry point that was found, or 0 if not found.
                 */
                EntryPoint* findEntryPoint(const std::string& name);
                
                /**
                 * Add a new external service to the composite.
                 * @param name The name of the new external service.
                 * @return The external service added to the composite.
                 */
                ExternalService* addExternalService(const std::string& name);

                /**
                 * Add a wire to the model.
                 * @param source The source location. Either the source component and
                 * reference (optional), or an entry point.
                 * @param target The target location. Either the target component and
                 * service (optional), or an external service.
                 */
                void addWire(const std::string& source, const std::string& target);
 
                /**
                 * Find a component by name.
                 * @param componentName The name of the component to be found.
                 * @return The component that was found, or 0 if not found.
                 */
                Component* findComponent(const std::string& componentName);

                /**
                 * Find a component and service by name.
                 * @param componentServiceName A string of the form 
                 * "componentName"/"serviceName" where the service name is optional
                 * if there is only one service on the component.
                 * @return The Service that was found, or 0 if not found.
                 */
                Service* findComponentService(const std::string& componentServiceName);

                /**
                 * Find an external service by name.
                 * @param serviceName The name of the external service to be found.
                 * @return The external service that was found, or 0 if not found.
                 */
                ExternalService* findExternalService(const std::string& serviceName);

                /**
                 * Add a WSDL definition to the composite.
                 * @param wsdlModel A data object holding all the information about 
                 * the WSDL definition from a WSDL file.
                 */
                void addWsdl(commonj::sdo::DataObjectPtr wsdlModel);

                /**
                 * Find a WSDL definition by target namespace.
                 * @param wsdlNamespace The namespace of the WSDL definitions to find.
                 */
                Wsdl* findWsdl(const std::string& wsdlNamespace);

                /**
                 * Return a cached SDO XSDHelper.
                 */
                commonj::sdo::XSDHelperPtr getXSDHelper(void);

                /**
                 * Return a cached SDO XMLHelper.
                 */
                commonj::sdo::XMLHelperPtr getXMLHelper(void);

                /**
                 * Return a data factory which has all the types defined in XSDs and
                 * WSDL files configured for this composite.
                 * @return The data factory for this composite.
                 */
                commonj::sdo::DataFactoryPtr getDataFactory(void);

                /**
                 * Work through the list of wires and connect the source and target uris.
                 */
                void resolveWires();
 
            private:
                /**
                 * Name of the composite.
                 */
                string name;

                /**
                 * Directory of the root of the composite.
                 */
                string compositeRoot;

                /**
                 * Cached XSDHelper.
                 */
                commonj::sdo::XSDHelperPtr xsdHelper;

                /**
                 * Cached XMLHelper.
                 */
                commonj::sdo::XMLHelperPtr xmlHelper;

                typedef map<std::string, Component*> COMPONENT_MAP;
                /**
                 * Map (by name) of all the components in this composite.
                 */
                COMPONENT_MAP components;

                typedef map<std::string, ExternalService*> EXTERNALSERVICE_MAP;
                /**
                 * Map (by name) of all the external services in this composite.
                 */
                EXTERNALSERVICE_MAP externalServices;

                typedef map<std::string, EntryPoint*> ENTRYPOINT_MAP;
                /**
                 * Map (by name) of all the entry points in this composite.
                 */
                ENTRYPOINT_MAP entryPoints;

                typedef vector<Wire> WIRES;
                /**
                 * Vector of all the wires in this composite.
                 */
                WIRES wires;

                typedef map<std::string, Wsdl*> WSDL_MAP;
                /**
                 * Map by namespace of all the wsdl definitions in this composite.
                 */
                WSDL_MAP wsdls;
            };

        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // SCA_CompositeComponent_h

