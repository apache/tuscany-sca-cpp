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


#include <sstream>

#include "commonj/sdo/SDO.h"

#include "tuscany/sca/ruby/RubyServiceWrapper.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
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

using namespace std;
using namespace commonj::sdo;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace ruby
        {
            
            class RubyServiceInvocation
            {
            public:
                VALUE instance;
                ID method;
                int argc;
                const VALUE* argv;
            };
        }
    }
}

extern "C"
{
    
    static VALUE safe_rb_funcall(VALUE value)
    {
        tuscany::sca::ruby::RubyServiceInvocation* call = (tuscany::sca::ruby::RubyServiceInvocation*)value;
        if (call->argc == 0)
        {
            VALUE result = rb_funcall(call->instance, call->method, 0);
            return result;
        }
        else
        {
            VALUE result = rb_funcall2(call->instance, call->method, call->argc, call->argv);
            return result;
        }
    }

}

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
                logentry();
                
                component = service->getComponent();
                implementation = (RubyImplementation*)component->getType();
                interf = service->getType()->getInterface();
            }
            
            // ==========
            // Destructor
            // ==========
            RubyServiceWrapper::~RubyServiceWrapper()
            {
                logentry();
            }
            
            // ======================================================================
            // invoke: wrapper call to service with setting the component context
            // ======================================================================
            void RubyServiceWrapper::invoke(Operation& operation)
            {
                logentry();
    
                SCARuntime* runtime = SCARuntime::getCurrentRuntime();
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
                    DataObjectPtr properties = component->getProperties();
                    PropertyList pl = properties->getInstanceProperties();
                    for (int i = 0; i < pl.size(); i++)
                    {
                        if (properties->isSet(pl[i]))
                        {
                            string varName = "@";
                            varName += pl[i].getName();
                            string cstr = properties->getCString(pl[i]);
                            VALUE propertyValue;
                            if (cstr == "true")
                            {
                                propertyValue = Qtrue;
                            }
                            else if (cstr == "false")
                            {
                                propertyValue = Qfalse;
                            }
                            else
                            {
                                //TODO use one of the rb_str_to_inum() functions
                                // to convert a numeric value to a Ruby numeric
                                propertyValue = rb_str_new2(cstr.c_str());
                            }  
                            rb_iv_set(instance, varName.c_str(), propertyValue);
                        }
                    }
                    
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
                            
                            const Operation::Parameter& parm = operation.getParameter(i);
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
                                    value = rb_float_new(*(float*)parm.getValue());
                                    break;
                                }
                                case Operation::DOUBLE: 
                                {
                                    value = rb_float_new(*(double*)parm.getValue());
                                    break;
                                }
                                case Operation::LONGDOUBLE: 
                                {
                                    value = rb_float_new(*(long double*)parm.getValue());
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
                                case Operation::DATAOBJECT: 
                                {
                                    DataObjectPtr dob = *(DataObjectPtr*)parm.getValue();

                                    // Convert a DataObject to a REXML Document object
                                    Composite* composite = component->getComposite();                                    
                                    commonj::sdo::XMLHelperPtr xmlHelper = composite->getXMLHelper();
                                    char* str = xmlHelper->save(
                                        dob,
                                        dob->getType().getURI(),
                                        dob->getType().getName());
                                        
                                    loginfo("Converting SDO DataObject to Ruby Rexml document: %s", str);
                                        
                                    VALUE vstr[1];
                                    vstr[0] = rb_str_new2(str);

                                    value = rb_class_new_instance(1, vstr, RubyImplementation::getXMLDocumentClass());
                                    break;
                                }
                                default:
                                {
                                    ostringstream msg;
                                    msg << "Operation parameter type not supported: " << parmType;
                                    throwException(ServiceDataException, msg.str().c_str());
                                }
                            }
                            
                            args[i] = value;
                        }
                    }                    

                    
                    // Invoke the specified method
                    RubyServiceInvocation call;
                    call.instance = instance;
                    call.method = method;
                    call.argc = n;
                    call.argv = args;
                    int error = 0;
                    VALUE result = rb_protect(safe_rb_funcall, (VALUE)&call, &error);
                    if (error)
                    {
                        // Convert a Ruby error to a C++ exception
                        VALUE lasterr = rb_gv_get("$!");
                        ostringstream msg;
                    
                        // class
                        VALUE klass = rb_class_path(CLASS_OF(lasterr));
                        msg << "Ruby Exception " << RSTRING(klass)->ptr << ": "; 
                    
                        // message
                        VALUE message = rb_obj_as_string(lasterr);
                        msg << RSTRING(message)->ptr << endl;
                    
                        // backtrace
                        if(!NIL_P(ruby_errinfo)) {
                            VALUE ary = rb_funcall(ruby_errinfo, rb_intern("backtrace"), 0);
                            int c;
                            for (c=0; c<RARRAY(ary)->len; c++) {
                                msg << "from " << RSTRING(RARRAY(ary)->ptr[c])->ptr << endl;
                            }
                        }
                        throwException(ServiceInvocationException, msg.str().c_str());                        
                    }
                    
                    // Convert the Ruby result value to a C++ result
                    int resultType = TYPE(result);
                    char buf[20];
                    switch(resultType)
                    {
                    case T_FLOAT: 
                        {
                            float* data = new float; 
                            *data = rb_num2dbl(result);
                            
                            // Check if the return type has already been set for typed languages
    		                switch(operation.getReturnType())
    		                {
    			                case Operation::BOOL: 
    			                {
    				                *(bool*)operation.getReturnValue() = (*data != 0.0);
    				                break;
    			                }
    			                case Operation::SHORT: 
    			                {
                                    *(short*)operation.getReturnValue() = (short)*data;
    				                break;
    			                }
    			                case Operation::USHORT: 
    			                {
                                    *(unsigned short*)operation.getReturnValue() = (unsigned short)*data;
    				                break;
    			                }
    			                case Operation::INT: 
    			                {
                                    *(int*)operation.getReturnValue() = (int)*data;
    				                break;
    			                }
    			                case Operation::UINT: 
    			                {
                                    *(unsigned int*)operation.getReturnValue() = (unsigned int)*data;
    				                break;
    			                }
    			                case Operation::LONG: 
    			                {
                                    *(long*)operation.getReturnValue() = (long)*data;
    				                break;
    			                }
    			                case Operation::ULONG: 
    			                {
                                    *(unsigned long*)operation.getReturnValue() = (unsigned long)*data;
    				                break;
    			                }
    			                case Operation::FLOAT: 
    			                {
                                    *(float*)operation.getReturnValue() = (float)*data;
    				                break;
    			                }
    			                case Operation::DOUBLE: 
    			                {
                                    *(double*)operation.getReturnValue() = (double)*data;
    				                break;
    			                }
    			                case Operation::LONGDOUBLE: 
    			                {
                                    *(long double*)operation.getReturnValue() = (long double)*data;
    				                break;
    			                }
    			                case Operation::CHARS: 
    			                {
                                    sprintf(buf, "%f", *data);
                                    *(char**)operation.getReturnValue() = buf;
    				                break;
    			                }
    			                case Operation::STRING: 
    			                {
                                    sprintf(buf, "%f", *data);
                                    *(string*)operation.getReturnValue() = buf;
    				                break;
    			                }
                                default:
                                {
                                    // The type is set as something else or has not been set
                                    operation.setReturnValue(data);
                                }
                            }
                            break;
                        }
                    case T_STRING: 
                        {
                            string* stringdata = new string(rb_string_value_cstr(&result));
                            const char** data = new const char*; 
                            *data = stringdata->c_str();

                            // Check if the return type has already been set (for typed languages)
    		                switch(operation.getReturnType())
    		                {
    			                case Operation::BOOL: 
    			                {
                                    // If the string is empty or "0" or "false" set to false, otherwise true
                                    if(strlen(*data) == 0 || strcmp(*data, "0") == 0 || strcmp(*data, "false") == 0)
                                    {
    				                    *(bool*)operation.getReturnValue() = false;
                                    }
                                    else
                                    {
                                        *(bool*)operation.getReturnValue() = true;
                                    }
    				                break;
    			                }
    			                case Operation::SHORT: 
    			                {
                                    *(short*)operation.getReturnValue() = (short)atoi(*data);
    				                break;
    			                }
    			                case Operation::USHORT: 
    			                {
                                    *(unsigned short*)operation.getReturnValue() = (unsigned short)atoi(*data);
    				                break;
    			                }
    			                case Operation::INT: 
    			                {
                                    *(int*)operation.getReturnValue() = (int)atoi(*data);
    				                break;
    			                }
    			                case Operation::UINT: 
    			                {
                                    *(unsigned int*)operation.getReturnValue() = (unsigned int)atoi(*data);
    				                break;
    			                }
    			                case Operation::LONG: 
    			                {
                                    *(long*)operation.getReturnValue() = (long)atol(*data);
    				                break;
    			                }
    			                case Operation::ULONG: 
    			                {
                                    *(unsigned long*)operation.getReturnValue() = (unsigned long)atol(*data);
    				                break;
    			                }
    			                case Operation::FLOAT: 
    			                {
                                    *(float*)operation.getReturnValue() = (float)atof(*data);
    				                break;
    			                }
    			                case Operation::DOUBLE: 
    			                {
                                    *(double*)operation.getReturnValue() = (double)atof(*data);
    				                break;
    			                }
    			                case Operation::LONGDOUBLE: 
    			                {
                                    *(long double*)operation.getReturnValue() = (long double)atof(*data);
    				                break;
    			                }
    			                case Operation::CHARS: 
    			                {
                                    *(const char**)operation.getReturnValue() = *data;
    				                break;
    			                }
    			                case Operation::STRING: 
    			                {
                                    *(string*)operation.getReturnValue() = *data;
    				                break;
    			                }
                                default:
                                {
                                    // The type is set as something else or has not been set
                                    operation.setReturnValue(data);
                                }
                            }
                            break;
                        }
                    case T_FIXNUM: 
                        {
                            long* data = new long;
                            *data = rb_num2long(result);
                                                            
                            // Check if the return type has already been (set for typed languages)
    		                switch(operation.getReturnType())
    		                {
    			                case Operation::BOOL: 
    			                {
    				                *(bool*)operation.getReturnValue() = (*data != 0);
    				                break;
    			                }
    			                case Operation::SHORT: 
    			                {
                                    *(short*)operation.getReturnValue() = (short)*data;
    				                break;
    			                }
    			                case Operation::USHORT: 
    			                {
                                    *(unsigned short*)operation.getReturnValue() = (unsigned short)*data;
    				                break;
    			                }
    			                case Operation::INT: 
    			                {
                                    *(int*)operation.getReturnValue() = (int)*data;
    				                break;
    			                }
    			                case Operation::UINT: 
    			                {
                                    *(unsigned int*)operation.getReturnValue() = (unsigned int)*data;
    				                break;
    			                }
    			                case Operation::LONG: 
    			                {
                                    *(long*)operation.getReturnValue() = (long)*data;
    				                break;
    			                }
    			                case Operation::ULONG: 
    			                {
                                    *(unsigned long*)operation.getReturnValue() = (unsigned long)*data;
    				                break;
    			                }
    			                case Operation::FLOAT: 
    			                {
                                    *(float*)operation.getReturnValue() = (float)*data;
    				                break;
    			                }
    			                case Operation::DOUBLE: 
    			                {
                                    *(double*)operation.getReturnValue() = (double)*data;
    				                break;
    			                }
    			                case Operation::LONGDOUBLE: 
    			                {
                                    *(long double*)operation.getReturnValue() = (long double)*data;
    				                break;
    			                }
    			                case Operation::CHARS: 
    			                {
                                    sprintf(buf, "%d", *data);
                                    *(char**)operation.getReturnValue() = buf;
    				                break;
    			                }
    			                case Operation::STRING: 
    			                {
                                    sprintf(buf, "%d", *data);
                                    *(string*)operation.getReturnValue() = buf;
    				                break;
    			                }
                                default:
                                {
                                    // The type is set as something else or has not been set
                                    operation.setReturnValue(data);
                                }
    		                }

                            break;
                        }
                    case T_BIGNUM: 
                        {
                            long double* data = new long double; 
                            *data = rb_num2dbl(result);

                            // Check if the return type has already been set (for typed languages)
    		                switch(operation.getReturnType())
    		                {
    			                case Operation::BOOL: 
    			                {
    				                *(bool*)operation.getReturnValue() = (*data != 0.0);
    				                break;
    			                }
    			                case Operation::SHORT: 
    			                {
                                    *(short*)operation.getReturnValue() = (short)*data;
    				                break;
    			                }
    			                case Operation::USHORT: 
    			                {
                                    *(unsigned short*)operation.getReturnValue() = (unsigned short)*data;
    				                break;
    			                }
    			                case Operation::INT: 
    			                {
                                    *(int*)operation.getReturnValue() = (int)*data;
    				                break;
    			                }
    			                case Operation::UINT: 
    			                {
                                    *(unsigned int*)operation.getReturnValue() = (unsigned int)*data;
    				                break;
    			                }
    			                case Operation::LONG: 
    			                {
                                    *(long*)operation.getReturnValue() = (long)*data;
    				                break;
    			                }
    			                case Operation::ULONG: 
    			                {
                                    *(unsigned long*)operation.getReturnValue() = (unsigned long)*data;
    				                break;
    			                }
    			                case Operation::FLOAT: 
    			                {
                                    *(float*)operation.getReturnValue() = (float)*data;
    				                break;
    			                }
    			                case Operation::DOUBLE: 
    			                {
                                    *(double*)operation.getReturnValue() = (double)*data;
    				                break;
    			                }
    			                case Operation::LONGDOUBLE: 
    			                {
                                    *(long double*)operation.getReturnValue() = (long double)*data;
    				                break;
    			                }
    			                case Operation::CHARS: 
    			                {
                                    sprintf(buf, "%f", *data);
                                    *(char**)operation.getReturnValue() = buf;
    				                break;
    			                }
    			                case Operation::STRING: 
    			                {
                                    sprintf(buf, "%f", *data);
                                    *(string*)operation.getReturnValue() = buf;
    				                break;
    			                }
                                default:
                                {
                                    // The type is set as something else or has not been set
                                    operation.setReturnValue(data);
                                }
                            }
                            break;
                        }
                    case T_TRUE: 
                    case T_FALSE: 
                        {
                            bool* data = new bool;
                            *data = (resultType == T_TRUE); 
                            // Check if the return type has already been set (for typed languages)                                
    		                switch(operation.getReturnType())
    		                {
    			                case Operation::BOOL: 
    			                {
    				                *(bool*)operation.getReturnValue() = *data;
    				                break;
    			                }
    			                case Operation::SHORT: 
    			                {
                                    *(short*)operation.getReturnValue() = (short)*data;
    				                break;
    			                }
    			                case Operation::USHORT: 
    			                {
                                    *(unsigned short*)operation.getReturnValue() = (unsigned short)*data;
    				                break;
    			                }
    			                case Operation::INT: 
    			                {
                                    *(int*)operation.getReturnValue() = (int)*data;
    				                break;
    			                }
    			                case Operation::UINT: 
    			                {
                                    *(unsigned int*)operation.getReturnValue() = (unsigned int)*data;
    				                break;
    			                }
    			                case Operation::LONG: 
    			                {
                                    *(long*)operation.getReturnValue() = (long)*data;
    				                break;
    			                }
    			                case Operation::ULONG: 
    			                {
                                    *(unsigned long*)operation.getReturnValue() = (unsigned long)*data;
    				                break;
    			                }
    			                case Operation::FLOAT: 
    			                {
                                    *(float*)operation.getReturnValue() = (float)*data;
    				                break;
    			                }
    			                case Operation::DOUBLE: 
    			                {
                                    *(double*)operation.getReturnValue() = (double)*data;
    				                break;
    			                }
    			                case Operation::LONGDOUBLE: 
    			                {
                                    *(long double*)operation.getReturnValue() = (long double)*data;
    				                break;
    			                }
    			                case Operation::CHARS: 
    			                {
                                    if(*data)
                                    {
                                        *(char**)operation.getReturnValue() = "true";
                                    }
                                    else
                                    {
                                        *(char**)operation.getReturnValue() = "false";
                                    }
    				                break;
    			                }
    			                case Operation::STRING: 
    			                {
                                    if(*data)
                                    {
                                        *(string*)operation.getReturnValue() = "true";
                                    }
                                    else
                                    {
                                        *(string*)operation.getReturnValue() = "false";
                                    }
    				                break;
    			                }
                                default:
                                {
                                    // The type is set as something else or has not been set
                                    operation.setReturnValue(data);
                                }
    		                }

                            break;
                        }
                    case T_OBJECT: 
                        {
                            VALUE klass = rb_obj_class(result);
                            if (klass == RubyImplementation::getXMLDocumentClass())
                            {
                                // Convert a REXML::Document to a DataObject
                                ID to_s = rb_intern("to_s");
                                VALUE vstr = rb_funcall(result, to_s, 0);
                                string str = string(rb_string_value_cstr(&vstr));
                                                                
                                loginfo("Converting Ruby Rexml document to SDO DataObject: %s", str.c_str());
                                        
                                Composite* composite = component->getComposite();                                   
                                commonj::sdo::XMLHelperPtr xmlHelper = composite->getXMLHelper();
                                commonj::sdo::XMLDocumentPtr xmlDoc = xmlHelper->load(str.c_str());
                               
                                DataObjectPtr* dataObjectData = new DataObjectPtr;
                                if (xmlDoc != NULL)
                                {
                                    *dataObjectData = xmlDoc->getRootDataObject();
                                }
                                else
                                {
                                    *dataObjectData = NULL;
                                }
                                if (*dataObjectData != NULL)
                                {
                                    operation.setReturnValue(dataObjectData);
                                }
                                else
                                {
                                    string msg = "Document could not be converted to a DataObject";
                                    throwException(ServiceDataException, msg.c_str());
                                }
                            }
                            else
                            {
                                string msg = "Ruby type not supported: " + resultType;
                                throwException(ServiceDataException, msg.c_str());
                            }
                            break;
                        }
                    default:
                        {
                            string msg = "Ruby type not supported: " + resultType;
                            throwException(ServiceDataException, msg.c_str());
                        } 
                    }
                    
                }
                catch (...)
                {
                    runtime->unsetCurrentComponent();
                    throw;
                }
                
                runtime->unsetCurrentComponent();
            }
            
        } // End namespace ruby        
    } // End namespace sca
} // End namespace tuscany
