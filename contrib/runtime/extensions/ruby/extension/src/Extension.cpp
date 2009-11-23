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

#if defined(WIN32)  || defined (_WINDOWS)
#define SCA_RUBYEXTENSION_API __declspec(dllexport)
#else
#define SCA_RUBYEXTENSION_API 
#endif

#include "tuscany/sca/ruby/Ruby.h"
#include "tuscany/sca/ruby/RubyCompositeContext.h"

extern "C"
{

    // Implement the Sca::locateService module function    
    SCA_RUBYEXTENSION_API VALUE tuscany_sca_ruby_locateService(VALUE module, VALUE value)
    {
        // Locate the service
        const char* serviceName = rb_string_value_cstr(&value);
        return tuscany::sca::ruby::RubyCompositeContext::locateService(serviceName);
    }
   
    // Initialize the Ruby extension
    SCA_RUBYEXTENSION_API void Init_tuscany_sca_ruby()
    {
        
        // Define the Sca::locateService() function 
        VALUE module = rb_define_module("SCA");
        rb_define_module_function(module, "locateService", (VALUE(*)(ANYARGS))tuscany_sca_ruby_locateService, 1);
        
    }

    SCA_RUBYEXTENSION_API void Init_libtuscany_sca_ruby()
    {
        Init_tuscany_sca_ruby();
    }
}
