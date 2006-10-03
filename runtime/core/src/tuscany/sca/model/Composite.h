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

#ifndef tuscany_sca_model_composite_h
#define tuscany_sca_model_composite_h

#include "tuscany/sca/export.h"
#include <string>
using std::string;

#include <map>
using std::map;
#include <vector>
using std::vector;

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/model/ComponentType.h"


namespace tuscany
{
    namespace sca
    {

        namespace model
        {
            class Component;
            class ComponentType;
            class Service;
            class WSDLDefinition;
            class Wire;
            
            /**
             * Represents a composite.
             * A composite is used to assemble SCA elements in logical groupings.
             * It is the basic unit of composition within an SCA System. An SCA composite contains a
             * set of components, services, references and the wires that interconnect them, plus a set
             * of properties which can be used to configure components.
             */
            class Composite : public ComponentType
            {
            public:

                /**
                 * Constructor.
                 * @param name the name of the composite.
                 * @param root the root of the composite in the file system.
                 */
                SCA_API Composite(const string& name, const string& root);

                /**
                 * Destructor.
                 */
                SCA_API virtual ~Composite();

                /**
                 * Returns the root directory of the composite.
                 * @return The root of the composite in the file system.
                 */
                SCA_API const string& getRoot() const { return root; }
                
                /**
                 * Add a new component to the composite.
                 * @param component The component to add.
                 */
                SCA_API void addComponent(Component* component);

                /**
                 * Add/include a composite in this composite.
                 * @param composite The composite included in this composite.
                 */
                SCA_API void addInclude(Composite* composite);

                /**
                 * Add a wire to the model.
                 * @param source The source location. Either the source component and
                 * reference (optional), or an entry point.
                 * @param target The target location. Either the target component and
                 * service (optional), or an external service.
                 */
                SCA_API void addWire(const string& source, const string& target);
 
                /**
                 * Find a component by name.
                 * @param componentName The name of the component to be found.
                 * @return The component that was found, or 0 if not found.
                 */
                SCA_API Component* findComponent(const string& componentName);

                /**
                 * Find a component and service by name.
                 * @param componentServiceName A string of the form 
                 * "componentName"/"serviceName" where the service name is optional
                 * if there is only one service on the component.
                 * @return The Service that was found, or 0 if not found.
                 */
                SCA_API Service* findComponentService(const string& componentServiceName);

                /**
                 * Add a WSDL definition to the composite.
                 * @param wsdlModel A data object holding all the information about 
                 * the WSDL definition from a WSDL file.
                 */
                SCA_API void addWSDLDefinition(commonj::sdo::DataObjectPtr wsdlModel);

                /**
                 * Find a WSDL definition by target namespace.
                 * @param wsdlNamespace The namespace of the WSDL definitions to find.
                 */
                SCA_API WSDLDefinition* findWSDLDefinition(const string& wsdlNamespace);

                /**
                 * Return a cached SDO XSDHelper.
                 */
                SCA_API commonj::sdo::XSDHelperPtr getXSDHelper(void);

                /**
                 * Return a cached SDO XMLHelper.
                 */
                SCA_API commonj::sdo::XMLHelperPtr getXMLHelper(void);

                /**
                 * Return a data factory which has all the types defined in XSDs and
                 * WSDL files configured for this composite.
                 * @return The data factory for this composite.
                 */
                SCA_API commonj::sdo::DataFactoryPtr getDataFactory(void);

                /**
                 * Work through the list of wires and connect the source and target uris.
                 */
                SCA_API void resolveWires();
 
            private:

                /**
                 * Directory of the root of the composite.
                 */
                string root;

                /**
                 * Cached XSDHelper.
                 */
                commonj::sdo::XSDHelperPtr xsdHelper;

                /**
                 * Cached XMLHelper.
                 */
                commonj::sdo::XMLHelperPtr xmlHelper;

                /**
                 * Map (by name) of all the components in this composite.
                 */
                typedef map<string, Component*> COMPONENT_MAP;
                COMPONENT_MAP components;

                /**
                 * Vector of all the composites included in this composite.
                 */
                typedef vector<Composite*> INCLUDES;
                INCLUDES includes;

                /**
                 * Vector of all the wires in this composite.
                 */
                typedef vector<Wire*> WIRES;
                WIRES wires;

                /**
                 * Map by namespace of all the wsdl definitions in this composite.
                 */
                typedef map<string, WSDLDefinition*> WSDL_MAP;
                WSDL_MAP wsdlDefinitions;
            };

        } // End namespace model
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_model_composite_h
