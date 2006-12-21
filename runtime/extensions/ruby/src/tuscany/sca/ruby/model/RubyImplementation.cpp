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
#include "tuscany/sca/ruby/model/RubyImplementation.h"
#include "tuscany/sca/ruby/model/RubyServiceBinding.h"
#include "tuscany/sca/ruby/model/RubyReferenceBinding.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/util/Utils.h"

using namespace std;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {

        namespace ruby
        {

            bool RubyImplementation::initialized = false;
            VALUE RubyImplementation::xmlDocumentClass = 0;
            
            // Constructor
            RubyImplementation::RubyImplementation(Composite* composite, const string& module, const string& className, const string& script)
                : ComponentType(composite, script.substr(0, script.find_last_of('.'))),
                    module(module), className(className), script(script)
            {
                loadClass();
            }

            RubyImplementation::~RubyImplementation()
            {
            }
            
            void RubyImplementation::loadClass()
            {
                // Initialize the Ruby runtime
                if (!initialized)
                {
                    ruby_init();
                    ruby_init_loadpath();
                    
                    // Load the Rexml module. Rexml is used to handle XML documents.
                    //rb_require("rexml/document");
                    // Use rb_eval_string for now as it provides better error reporting
                    rb_eval_string("require(\"rexml/document\")");

                    xmlDocumentClass =  rb_path2class("REXML::Document");
                    
                    initialized = true;
                }
    
                // Load the specified Ruby script
                if (script != "")
                {
                    // Convert any windows slashes \ in the root path to unix slashes /
                    string rootpath = getComposite()->getRoot();
                    int pos = 0;
                    while((pos = rootpath.find('\\', pos)) != string::npos)
                    {
                        rootpath = rootpath.replace(pos, 1, "/");
                    }

                    // Use rb_eval_string for now as it provides better error reporting
                    string path = "require(\"" + rootpath + "/" + script +"\")";
                    //rb_require((char *)path.c_str());
                    rb_eval_string(path.c_str());
                }

                // Load the Ruby implementation class                
                implementationClass = rb_path2class(className.c_str());
                
                // Create a default service
                ServiceType* defaultServiceType = new ServiceType(this, "", NULL, NULL);
                addServiceType(defaultServiceType);

                // Introspect the Ruby class and create references and properties for
                // all public attributes
                VALUE methods = rb_class_public_instance_methods(0, NULL, implementationClass);
                int n = RARRAY(methods)->len;
                for (int i = 0; i<n; i++)
                {
                    VALUE method = rb_ary_entry(methods, i);
                    string methodName = string(rb_string_value_cstr(&method));
                    
                    // Create a reference type for each setter method
                    int s = methodName.size();
                    if (s > 1 && methodName[s-1] == '=' && methodName[0] != '=')
                    {
                        string variableName = methodName.substr(0, s-1);
                        ReferenceType* referenceType = new ReferenceType(
                                this, variableName, NULL, NULL, ReferenceType::ONE_ONE);
                        addReferenceType(referenceType);
                        
                        // Create a property type as well
                        addPropertyType(variableName, "http://www.w3.org/2001/XMLSchema#string", false, NULL);
                        
                    }
                }
            }
            
            void RubyImplementation::initializeComponent(Component* component)
            {
                ComponentType::initializeComponent(component);
                
                // Create Ruby bindings for all the services
                const Component::SERVICE_MAP& services = component->getServices();
                Component::SERVICE_MAP::const_iterator iter = services.begin();
                for (int i=0; i< services.size(); i++)
                {
                    Service *service = iter->second;
                    RubyServiceBinding* binding = new RubyServiceBinding(service);
                    service->setBinding(binding);
                    iter++;
                }
                
                // Create Ruby bindings for all the references
                const Component::REFERENCE_MAP& references = component->getReferences();
                Component::REFERENCE_MAP::const_iterator refiter = references.begin();
                for (int ri=0; ri< references.size(); ri++)
                {
                    Reference *reference = refiter->second;
                    RubyReferenceBinding* binding = new RubyReferenceBinding(reference);
                    reference->setBinding(binding);
                    refiter++;
                }
            }
            
        } // End namespace ruby
    } // End namespace sca
} // End namespace tuscany
