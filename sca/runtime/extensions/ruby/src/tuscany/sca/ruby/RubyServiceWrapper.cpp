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
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/ruby/model/RubyImplementation.h"
#include "tuscany/sca/ruby/model/RubyServiceBinding.h"
#include "tuscany/sca/ruby/RubyServiceProxy.h"

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
                implementation = (RubyImplementation*)component->getType();
                interf = service->getType()->getInterface();
                
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

                    // Create a new instance of the Ruby implementation class
                    VALUE instance = rb_class_new_instance(0, NULL, implementation->getImplementationClass());

                    // Set all the references
                    const Component::REFERENCE_MAP& references = component->getReferences();
                    Component::REFERENCE_MAP::const_iterator refiter = references.begin();
                    for (int ri=0; ri< references.size(); ri++)
                    {
                        Reference* reference = refiter->second;
                        RubyServiceProxy* proxy = (RubyServiceProxy*)reference->getBinding()->getServiceProxy();
                        if (proxy != NULL)
                        {
                            VALUE proxyValue = proxy->getProxyValue();
                            string varName = "@" + refiter->first; 
                            rb_iv_set(instance, varName.c_str(), proxyValue);
                        }
                        refiter++;
                    }
                    
                    // Set all the configured properties
                    //TODO
                    
                    // Get the ID of the specified method
                    ID method = rb_intern(operation.getName().c_str());

                    // Convert C++ parameters to Ruby parameters
                    VALUE *args = NULL;
                    int n = operation.getNParms();
                    if (n != 0)
                    {
                        args=new VALUE[n];
                        
                        for(int i = 0; i < operation.getNParms(); i++) 
                        {
                            VALUE value;
                            
                            Operation::Parameter& parm = operation.getParameter(i);
                            Operation::ParameterType parmType = parm.getType();
                            switch(parmType)
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
                                    string msg = "Operation parameter type not supported" + parmType;
                                    throw msg.c_str();
                                }
                            }
                            
                            args[i] = value;
                        }
                    }                    

                    
                    // Invoke the specified method
                    VALUE result;
                    if (n == 0)
                    {
                        result = rb_funcall(instance, method, 0);
                    }
                    else
                    {
                        result = rb_funcall2(instance, method, n, args);
                    }
                    
                    // Convert the Ruby result value to a C++ result
                    int resultType = TYPE(result);
                    switch(resultType)
                    {
                    case T_FLOAT: 
                        {
                            float* data = new float; 
                            *data = rb_num2dbl(result);
                            operation.setReturnValue(data);
                            break;
                        }
                    case T_STRING: 
                        {
                            string* data = new string(rb_string_value_cstr(&result));
                            const char** cdata = new const char*; 
                            *cdata = data->c_str();
                            operation.setReturnValue(cdata);
                            break;
                        }
                    case T_FIXNUM: 
                        {
                            long* data = new long;
                            *data = rb_num2long(result);
                            operation.setReturnValue(data);
                            break;
                        }
                    case T_BIGNUM: 
                        {
                            long double* data = new long double; 
                            *data = rb_num2dbl(result);
                            operation.setReturnValue(data);
                            break;
                        }
                    case T_TRUE: 
                        {
                            bool* data = new bool;
                            *data = true; 
                            operation.setReturnValue(data);
                            break;
                        }
                    case T_FALSE: 
                        {
                            bool* data = new bool;
                            *data = false; 
                            operation.setReturnValue(data);
                            break;
                        }
                    default:
                        {
                            string msg = "Ruby type not supported: " + resultType;
                            throw msg.c_str();
                        } 
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
