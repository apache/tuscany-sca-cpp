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

#ifndef tuscany_sca_ruby_rubyservicewrapper_h
#define tuscany_sca_ruby_rubyservicewrapper_h

#include "tuscany/sca/export.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/ruby/model/RubyImplementation.h"

#include <ruby.h>

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            class RubyInterface;
            
            /**
             * Wraps the service on a component implementation.
             * This abstract class is extended by generated code which provides 
             * the implementation of some of the methods. 
             * An instance of this class wraps the actual component implementation which 
             * has been written by a developer of an SCA application.
             */
            class SCA_API RubyServiceWrapper : public ServiceWrapper
            {
            public:

                /**
                 * Constructor.
                 * @param target The component service to which this wrapper refers.
                 */
                RubyServiceWrapper(Service* service);
    
                /**
                 * Destructor.
                 */ 
                virtual    ~RubyServiceWrapper();
    
                /**
                 * All business method calls to the target component go through the invoke method.
                 * @param operation The details of the method, paramaters and return value for the
                 * business method to be called on the target component.
                 */
                virtual void invoke(Operation& operation);
    
            private:

                /**
                 * The component to which this wrapper refers.
                 */
                Component* component;
    
                /**
                 * A pointer to the interface which the service exposes.
                 */
                Interface* interf;

                /**
                 * The Ruby implementation
                 */
                 RubyImplementation* implementation;

            };
            
        } // End namespace ruby
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_ruby_rubyservicewrapper_h
