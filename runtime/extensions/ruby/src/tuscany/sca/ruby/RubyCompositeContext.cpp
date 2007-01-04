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
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ComponentType.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/ruby/RubyServiceProxy.h"

using namespace std;
using namespace tuscany::sca::model;

extern "C"
{

    // Implement the Sca::locateService module function    
    SCA_RUBY_API VALUE tuscany_sca_ruby_locateService(VALUE module, VALUE value)
    {
        
        // Get the default component
        Component* defaultComponent = tuscany::sca::SCARuntime::getCurrentRuntime()->getDefaultComponent();
        Composite* composite = (Composite*)defaultComponent->getType();
                
        // Locate the service
        const char* serviceName = rb_string_value_cstr(&value);
        Service* service = composite->findComponentService(serviceName);
        string msg;
        if (!service)
        {
            string msg = "Service not found: ";
            msg = msg + serviceName;
            rb_raise(rb_eRuntimeError, msg.c_str());
        }

        // Get a Proxy for this service
        tuscany::sca::ruby::RubyServiceProxy* serviceProxy = new tuscany::sca::ruby::RubyServiceProxy(service);

        // Return the Ruby proxy value object        
        return serviceProxy->getProxyValue();
    }
    
    // Initialize the Ruby extension
    SCA_RUBY_API void Init_tuscany_sca_ruby()
    {
        
        // Define the Sca::locateService() function 
        VALUE module = rb_define_module("SCA");
        rb_define_module_function(module, "locateService", (VALUE(*)(ANYARGS))tuscany_sca_ruby_locateService, 1);
        
    }

    SCA_RUBY_API void Init_libtuscany_sca_ruby()
    {
        Init_tuscany_sca_ruby();
    }
}
