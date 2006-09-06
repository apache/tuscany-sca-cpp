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

#include "tuscany/sca/ruby/RubyServiceWrapper.h"

#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/util/Library.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/ruby/model/RubyImplementation.h"
#include "tuscany/sca/ruby/model/RubyServiceBinding.h"

namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            
            // ===========
            // Constructor
            // ===========
            RubyServiceWrapper::RubyServiceWrapper(Service* service)
                : ServiceWrapper(service)
            {
                LOGENTRY(1,"RubyServiceWrapper::constructor");
    
                component = service->getComponent();
                interf = service->getType()->getInterface();
                remotable = interf->isRemotable();
                
                // Initialize the Ruby runtime
                ruby_init();
                
                // Execute the specified Ruby script
                RubyImplementation* impl = (RubyImplementation*)component->getType();
                if (impl->getScript() != "")
                {
                    string script = component->getComposite()->getRoot() + '/' + impl->getScript();
                    rb_require((char *)script.c_str());
                }
                
                LOGEXIT(1,"RubyServiceWrapper::constructor");
                
            }
            
            // ==========
            // Destructor
            // ==========
            RubyServiceWrapper::~RubyServiceWrapper()
            {
                LOGENTRY(1,"RubyServiceWrapper::destructor");
                LOGEXIT(1,"RubyServiceWrapper::destructor");
            }
            
            // ======================================================================
            // invoke: wrapper call to service with setting the component context
            // ======================================================================
            void RubyServiceWrapper::invoke(Operation& operation)
            {
                LOGENTRY(1,"RubyServiceWrapper::invoke");
    
                SCARuntime* runtime = SCARuntime::getInstance();
                runtime->setCurrentComponent(component);
                
                try
                {
                    // Create a new instance of the component implementation class
                    RubyImplementation* impl = (RubyImplementation*)component->getType();
                    
                    string expr = impl->getClass() + ".new";
                    VALUE instance = rb_eval_string(expr.c_str());

                    // Get the ID of the specified method
                    ID method = rb_intern(operation.getName().c_str());

                    // Convert C arguments to Ruby arguments
                    VALUE *args = NULL;
                    int n = operation.getNParms();
                    if (n != 0)
                    {
                        args=new VALUE[n];
                        
                        for(int i = 0; i < operation.getNParms(); i++) 
                        {
                            VALUE value;
                            
                            Operation::Parameter& parm = operation.getParameter(i);
                            switch(parm.getType())
                            {
                                case Operation::BOOL: 
                                {
                                    if( *(bool*)parm.getValue())
                                    {
                                        //boolean true
                                        value = rb_int2inum(1);
                                    }
                                    else
                                    {
                                        value = rb_int2inum(0);
                                    }
                                    break;
                                }
                                case Operation::SHORT: 
                                {
                                    value = rb_int2inum(*(short*)parm.getValue());
                                    break;
                                }
                                case Operation::USHORT: 
                                {
                                    value = rb_uint2inum(*(unsigned short*)parm.getValue());
                                    break;
                                }
                                case Operation::LONG: 
                                {
                                    value = rb_int2inum(*(long*)parm.getValue());
                                    break;
                                }
                                case Operation::ULONG: 
                                {
                                    value = rb_uint2inum(*(unsigned long*)parm.getValue());
                                    break;
                                }
                                case Operation::FLOAT: 
                                {
                                    value = rb_dbl2big(*(float*)parm.getValue());
                                    break;
                                }
                                case Operation::DOUBLE: 
                                {
                                    value = rb_dbl2big(*(double*)parm.getValue());
                                    break;
                                }
                                case Operation::LONGDOUBLE: 
                                {
                                    value = rb_dbl2big(*(long double*)parm.getValue());
                                    break;
                                }
                                case Operation::CHARS: 
                                {
                                    value = rb_str_new2(*(char**)parm.getValue());
                                    break;
                                }
                                case Operation::STRING: 
                                {
                                    value = rb_str_new2((*(string*)parm.getValue()).c_str());
                                    break;
                                }
                                default:
                                {
                                    //throw new ComponentInvocationException("Operation parameter type not supported");
                                    std::cout << "Operation parameter type not supported" << std::endl;
                                }
                            }
                            
                            args[i] = value;
                        }
                    }                    

                    
                    // Invoke the specified method
                    VALUE value;
                    if (n == 0)
                    {
                        value = rb_funcall(instance, method, 0);
                    }
                    else
                    {
                        value = rb_funcall2(instance, method, n, args);
                    }
                    
                    // Convert the result to a C result
                    switch(operation.getReturnType())
                    {
                    case Operation::BOOL: 
                        {
                            *(bool*)operation.getReturnValue() = (rb_num2long(value)!=0);
                            break;
                        }
                    case Operation::SHORT: 
                        {
                            *(short*)operation.getReturnValue() = (short) rb_num2long(value);
                            break;
                        }
                    case Operation::LONG: 
                        {
                            *(long*)operation.getReturnValue() =  (long) rb_num2long(value);
                            break;
                        }
                    case Operation::USHORT: 
                        {
                            *(unsigned short*)operation.getReturnValue() = (unsigned short) rb_num2ulong(value);
                            break;
                        }
                    case Operation::ULONG: 
                        {
                            *(unsigned long*)operation.getReturnValue() = (unsigned long) rb_num2ulong(value);
                            break;
                        }
                    case Operation::FLOAT: 
                        {
                            *(float*)operation.getReturnValue() = (float) rb_num2dbl(value);
                            break;
                        }
                    case Operation::DOUBLE: 
                        {
                            *(double*)operation.getReturnValue() = (double) rb_num2dbl(value);
                            break;
                        }
                    case Operation::LONGDOUBLE: 
                        {
                            *(long double*)operation.getReturnValue() = (long double) rb_num2dbl(value);
                            break;
                        }
                    case Operation::CHARS: 
                        {
                            *(char**)operation.getReturnValue() = strdup(rb_string_value_cstr(&value));
                            break;
                        }
                    case Operation::STRING: 
                        {
                            *(string*)operation.getReturnValue() = string(rb_string_value_cstr(&value));
                            break;
                        }
                    default:;
                    }
                    
                }
                catch (...)
                {
                    runtime->unsetCurrentComponent();
                    throw;
                }
                runtime->unsetCurrentComponent();
                LOGEXIT(1,"RubyServiceWrapper::invoke");
                
            }
            
        } // End namespace ruby        
    } // End namespace sca
} // End namespace tuscany
