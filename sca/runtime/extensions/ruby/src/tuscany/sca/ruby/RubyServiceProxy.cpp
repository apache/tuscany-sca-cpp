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


#include "tuscany/sca/ruby/RubyServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/util/Exceptions.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/model/ReferenceType.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/ComponentType.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceBinding.h"
#include "tuscany/sca/ruby/model/RubyImplementation.h"
#include "tuscany/sca/ruby/model/RubyReferenceBinding.h"

extern "C"
{
    
    // Initialize a Ruby proxy
    SCA_RUBY_API VALUE tuscany_sca_ruby_proxy_initialize(VALUE self, VALUE serviceProxy)
    {
        rb_iv_set(self, "@cppProxy", serviceProxy);
        return self;
    }

    // Handle a method_missing message and dispatch to
    // our C++ proxy
    SCA_RUBY_API VALUE tuscany_sca_ruby_proxy_method_missing(int argc, VALUE* argv, VALUE self)
    {
        VALUE proxy = rb_iv_get(self, "@cppProxy");
        
        // Get the target service wrapper
        tuscany::sca::ruby::RubyServiceProxy *serviceProxy;
        Data_Get_Struct(proxy, tuscany::sca::ruby::RubyServiceProxy, serviceProxy);

        // Handle the invocation
        return serviceProxy->invoke(argc, argv); 
        
    }
    
}

namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            
            VALUE RubyServiceProxy::proxyClass = Qnil; 
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            RubyServiceProxy::RubyServiceProxy(Reference* reference)
                : ServiceProxy(reference)
            {
                LOGENTRY(1,"RubyServiceProxy::constructor");
    
                // ----------------------
                // Get the component
                // ----------------------
                component = reference->getComponent();
                string name = reference->getType()->getName();
                
                // Get the service wrapper
                RubyReferenceBinding* referenceBinding = (RubyReferenceBinding*)reference->getBinding();
                
                serviceWrapper = referenceBinding->getTargetServiceBinding()->getServiceWrapper();
    
                // Create the Ruby proxy
                createProxy();
                
                LOGEXIT(1,"RubyServiceProxy::constructor");
            }
            
            // ============================
            // Constructor: Create a proxy
            // ============================
            RubyServiceProxy::RubyServiceProxy(Service* service)
                : ServiceProxy(0)
            {
                LOGENTRY(1,"RubyServiceProxy::constructor");
                
                // ----------------------
                // Get the component
                // ----------------------
                component = service->getComponent();
                string name = service->getType()->getName();
                
                // Get the service wrapper
                serviceWrapper = service->getBinding()->getServiceWrapper();
                
                // Create the Ruby proxy
                createProxy();
                
                LOGEXIT(1,"RubyServiceProxy::constructor");
            }
            
            // ==========
            // Destructor
            // ==========
            RubyServiceProxy::~RubyServiceProxy()
            {
                LOGENTRY(1,"RubyServiceProxy::destructor");
                LOGEXIT(1,"RubyServiceProxy::destructor");
            }
            
            void RubyServiceProxy::createProxy()
            {
                // Create the Ruby proxy class
                if (RubyServiceProxy::proxyClass == Qnil)
                {
                    VALUE module = rb_define_module("Tuscany");
                    proxyClass = rb_define_class_under(module, "ServiceProxy", rb_cObject);
                    rb_define_method(proxyClass, "initialize", (VALUE(*)(ANYARGS))tuscany_sca_ruby_proxy_initialize, 1);
                    rb_define_method(proxyClass, "method_missing", (VALUE(*)(ANYARGS))tuscany_sca_ruby_proxy_method_missing, -1);
                }
                
                // Create the Ruby proxy instance, pass the service wrapper to it
                VALUE* args = new VALUE[1];
                args[0] = Data_Wrap_Struct(rb_cObject, NULL, NULL, this);
                proxyValue = rb_class_new_instance(1, args, proxyClass);
                
                // Mark proxyValue busy so that it doesn't get GC'ed by Ruby 
                rb_gc_register_address(&proxyValue);
            }
            
            
            VALUE RubyServiceProxy::invoke(int argc, VALUE* argv)
            {
                // Get the method name
                char* methodName = rb_id2name(SYM2ID(argv[0]));
        
                // Get the block passed by the caller
                VALUE block =rb_block_given_p() ? rb_block_proc() : Qnil;
                
                // Create new Operation object
                Operation operation(methodName);
        
                // Convert the Ruby parameters to C++
                for (int i = 1; i < argc; i++)
                {
                    VALUE value = argv[i];
                    
                    int valueType = TYPE(value);
                    
                    switch (valueType)
                    {
                    case T_FLOAT: 
                        {
                            float* data = new float; 
                            *data = rb_num2dbl(value);
                            operation.addParameter(data);
                            break;
                        }
                    case T_STRING: 
                        {
                            string* data = new string(rb_string_value_cstr(&value));
                            const char** cdata = new const char*; 
                            *cdata = data->c_str();
                            operation.addParameter(cdata);
                            break;
                        }
                    case T_FIXNUM: 
                        {
                            long* data = new long;
                            *data = rb_num2long(value);
                            operation.addParameter(data);
                            break;
                        }
                    case T_BIGNUM: 
                        {
                            long double* data = new long double; 
                            *data = rb_num2dbl(value);
                            operation.addParameter(data);
                            break;
                        }
                    case T_TRUE: 
                        {
                            bool* data = new bool;
                            *data = true; 
                            operation.addParameter(data);
                            break;
                        }
                    case T_FALSE: 
                        {
                            bool* data = new bool;
                            *data = false; 
                            operation.addParameter(data);
                            break;
                        }
                    case T_OBJECT: 
                        {
                            VALUE klass = rb_obj_class(value);
                            if (klass == RubyImplementation::getXMLDocumentClass())
                            {
                                // Convert a REXML::Document to a DataObject
                                ID to_s = rb_intern("to_s");
                                VALUE vstr = rb_funcall(value, to_s, 0);
                                string str = string(rb_string_value_cstr(&vstr));
                                                                
                                Composite* composite = getReference()->getComponent()->getComposite();                                   
                                commonj::sdo::XMLHelper* xmlHelper = composite->getXMLHelper();
                                commonj::sdo::XMLDocumentPtr xmlDoc = xmlHelper->load(str.c_str());
                                
                                DataObjectPtr* dob = new DataObjectPtr;
                                if (xmlDoc != NULL)
                                {
                                    *dob = xmlDoc->getRootDataObject();
                                }
                                if (*dob != NULL)
                                {
                                    operation.addParameter(dob);
                                }
                                else
                                {
                                    string msg = "Document could not be converted to a DataObject";
                                    rb_raise(rb_eTypeError, msg.c_str());
                                    return Qnil;
                                }
                            }
                            else
                            {
                                string msg = "Ruby type not supported: " + valueType;
                                rb_raise(rb_eTypeError, msg.c_str());
                                return Qnil;
                            }
                            break;
                        }
                    default:;
                        string msg = "Ruby type not supported: " + valueType;
                        rb_raise(rb_eTypeError, msg.c_str());
                        return Qnil;
                    }

                }
                
                try
                {
                    // Call into the target service wrapper
                    serviceWrapper->invoke(operation);
            
                    // Convert the result to a Ruby value
                    VALUE value;
                    Operation::ParameterType resultType = operation.getReturnType();
                    switch(resultType)
                    {
                        case Operation::BOOL: 
                        {
                            if( *(bool*)operation.getReturnValue())
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
                            value = rb_int2inum(*(short*)operation.getReturnValue());
                            break;
                        }
                        case Operation::USHORT: 
                        {
                            value = rb_uint2inum(*(unsigned short*)operation.getReturnValue());
                            break;
                        }
                        case Operation::LONG: 
                        {
                            value = rb_int2inum(*(long*)operation.getReturnValue());
                            break;
                        }
                        case Operation::ULONG: 
                        {
                            value = rb_uint2inum(*(unsigned long*)operation.getReturnValue());
                            break;
                        }
                        case Operation::FLOAT: 
                        {
                            value = rb_dbl2big(*(float*)operation.getReturnValue());
                            break;
                        }
                        case Operation::DOUBLE: 
                        {
                            value = rb_dbl2big(*(double*)operation.getReturnValue());
                            break;
                        }
                        case Operation::LONGDOUBLE: 
                        {
                            value = rb_dbl2big(*(long double*)operation.getReturnValue());
                            break;
                        }
                        case Operation::CHARS: 
                        {
                            value = rb_str_new2(*(char**)operation.getReturnValue());
                            break;
                        }
                        case Operation::STRING: 
                        {
                            value = rb_str_new2((*(string*)operation.getReturnValue()).c_str());
                            break;
                        }
                        case Operation::DATAOBJECT: 
                        {
                            DataObjectPtr dob = *(DataObjectPtr*)operation.getReturnValue();

                            // Convert a DataObject to a REXML Document object
                            Composite* composite = component->getComposite();                                   
                            commonj::sdo::XMLHelper* xmlHelper = composite->getXMLHelper();
                            char* str = xmlHelper->save(
                                dob,
                                dob->getType().getURI(),
                                dob->getType().getName());
                            VALUE vstr[1];
                            vstr[0] = rb_str_new2(str);

                            value = rb_class_new_instance(1, vstr, RubyImplementation::getXMLDocumentClass());
                            break;
                        }
                        default:
                        {
                            //throw new ComponentInvocationException("Operation parameter type not supported");
                            string msg = "Operation parameter type not supported" + resultType;
                            rb_raise(rb_eRuntimeError, msg.c_str());
                            return Qnil;
                        }
                    }
            
                    return value;

                }
                catch(TuscanyRuntimeException &ex)
                {   
                    string msg = "Exception while invoking a service: ";
                    msg += ex.getEClassName();
                    msg += ": ";
                    msg += ex.getMessageText();
                    rb_raise(rb_eRuntimeError, msg.c_str());
                    return Qnil;
                }
                  
                return Qnil;
            }
            
        } // End namespace ruby        
    } // End namespace sca
} // End namespace tuscany
