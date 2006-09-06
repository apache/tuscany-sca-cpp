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

#include "tuscany/sca/ruby/RubyImplementationExtension.h"
#include "tuscany/sca/ruby/model/RubyImplementation.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"


namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            // ===================================================================
            // Constructor for the RubyImplementationExtension class. 
            // ===================================================================
            RubyImplementationExtension::RubyImplementationExtension()
            { 
                LOGENTRY(1, "RubyImplementationExtension::constructor");     
                LOGEXIT(1, "RubyImplementationExtension::constructor");
            }
            
            // ===================================================================
            // Destructor for the RubyImplementationExtension class.
            // ===================================================================
            RubyImplementationExtension::~RubyImplementationExtension()
            { 
                LOGENTRY(1, "RubyImplementationExtension::destructor");;           
                LOGEXIT(1, "RubyImplementationExtension::destructor");
            }

            const string RubyImplementationExtension::extensionName("ruby");
            const string RubyImplementationExtension::typeQName("http://www.osoa.org/xmlns/sca/1.0#RubyImplementation");

            // ===================================================================
            // loadModelElement - load the info from implementation.ruby 
            // ===================================================================
            ComponentType* RubyImplementationExtension::getImplementation(Composite *composite, DataObjectPtr scdlImplementation)
            {
                string implType = scdlImplementation->getType().getName();
                if (implType == "RubyImplementation")
                {
                    string module = scdlImplementation->getCString("module");
                    string className = scdlImplementation->getCString("class");
                    string script = scdlImplementation->getCString("script");

                    RubyImplementation* rubyImpl = new RubyImplementation(module, className, script);
                    
                    return rubyImpl;
                }
                else
                {
                    return NULL;
                }
            }

        } // End namespace ruby
    } // End namespace sca
} // End namespace tuscany
