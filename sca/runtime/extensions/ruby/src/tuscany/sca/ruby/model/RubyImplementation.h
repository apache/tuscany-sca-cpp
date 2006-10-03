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


#ifndef tuscany_sca_ruby_model_rubyimplementation_h
#define tuscany_sca_ruby_model_rubyimplementation_h
#include "tuscany/sca/model/ComponentType.h"

#include <map>
using std::map;
#include <string>
using std::string;

#include <ruby.h>

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            /**
             * Holds information about an SCA implementation written in Ruby
             */
            class RubyImplementation : public ComponentType
            {
                
            public:
                /**
                 * Constructor.
                 * @param composite The composite containing this implementation.
                 * @param module Name of the Ruby module.
                 * @param className Name of the Ruby implementation class.
                 * @param script Path of the Ruby script.
                 */
                RubyImplementation(Composite* composite, const string& module, const string& className, const string& script);
                
                /**
                 * Destructor
                 */
                virtual ~RubyImplementation();

                /**
                 * Initialize a component of this type.
                 * @param component The component to initialize.
                 */
                SCA_API virtual void initializeComponent(Component* component);

                /**
                 * Returns the name of the Ruby module.
                 * @return The name of the Ruby module.
                 */
                const string& getModule() const { return module; }

                /**
                 * Returns the name of Ruby class.
                 * @return Name of the Ruby class.
                 */
                const string& getClass() const { return className; }

                /**
                 * Returns the path of the Ruby script.
                 * @return The path of the Ruby script.
                 */
                const string& getScript() const { return script; }
                
                /**
                 * Returns the Ruby implementation class
                 */
                VALUE getImplementationClass() const { return implementationClass; }
                
                /**
                 * Returns the Ruby REXML::Document class
                 */
                 static VALUE getXMLDocumentClass() { return xmlDocumentClass; }

            private:
            
                /**
                 * Load the Ruby implementation class
                 */
                 void loadClass();
                
                /**
                 * Name of the Ruby module.
                 */
                string module;

                /**
                 * Name of the Ruby class.
                 */
                string className;

                /**
                 * Path of the Ruby script.
                 */
                string script;

                /**
                 * The Ruby implementation class
                 */                
                VALUE implementationClass;

                /**
                 * True if the Ruby runtime has been initialized
                 */
                static bool initialized;

                /**
                 * The Ruby REXML::Document class.
                 */
                static VALUE xmlDocumentClass;

            };
            
        } // End namespace ruby
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_ruby_model_rubyimplementation_h
