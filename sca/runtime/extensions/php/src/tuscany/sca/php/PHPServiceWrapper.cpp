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

// some strangeness in the build that causes 
// WinSock.h and WinSock2.h to be included leading to redefinitions
#define _WINSOCKAPI_

#include "tuscany/sca/php/PHPServiceWrapper.h"

#include "osoa/sca/ServiceRuntimeException.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/php/model/PHPImplementation.h"

#include <php_embed.h>

using namespace osoa::sca;

namespace tuscany
{
    namespace sca
    {
        namespace php
        {
            // a global!! place to put the response
            // as I can't get the PHP return value
            // processing to work. Need to be
            // removed when the proper PHP SAPI is used
            // The variable is only valid between the PHP engine call and
            // the results processing. It stores the last thing that
            // the script echoed
            string scriptResponse;

            // Global callbacks used by the PHP engine

            // Callback for log messages
            void php_log_message(char *message) 
            { 
                LOGINFO_1(5, "PHP Log (%s)", message); 
            }

            // Callback for unbuffered writes (echo, print etc.)
            int php_ub_write(const char *str, unsigned int str_length TSRMLS_DC)
            {
                LOGINFO_1(5, "PHP Write (%s)", str);
                scriptResponse = str;
                return str_length;
            }

            // Callback for errors 
            void php_error_cb(int type, 
                              const char *error_filename, 
                              const uint error_lineno,
                              const char *format, va_list args)
            {
                char buffer[2048];
                int len;
				
                len = snprintf(buffer, 2048, "Error on line %d: ", error_lineno);
                vsnprintf(buffer + len, (2048 - len), format, args);
                LOGINFO_1(5, "PHP Unformatted Error (%s)", buffer);
                zend_bailout(); 
            }

            // Callback for flush (could be used to do something with the scriptResponse)
            void php_flush(void *server_context) 
            { 
                LOGINFO(5, "Flush");
            }

            // ===========
            // Constructor
            // ===========
            PHPServiceWrapper::PHPServiceWrapper(Service* service)
                : ServiceWrapper(service)
            {
                LOGENTRY(1,"PHPServiceWrapper::constructor");
    
                component = service->getComponent();
                interf = service->getType()->getInterface();
                remotable = interf->isRemotable();                

                 // -----------------------------------------------
                // Get the implementation for the target component
                // -----------------------------------------------
                PHPImplementation* impl = (PHPImplementation*)component->getType();
                if (!impl)
                {
                    string msg = "Component " + component->getName() + " has no implementation defined";
                    throw ServiceNotFoundException(msg.c_str());
                }

                LOGINFO_1(3,"PHPServiceWrapper::getServiceWrapper module %s", impl->getModule().c_str());
                LOGINFO_1(3,"PHPServiceWrapper::getServiceWrapper class %s", impl->getClass().c_str());

                LOGEXIT(1,"PHPServiceWrapper::constructor");
                
            }
            
            // ==========
            // Destructor
            // ==========
            PHPServiceWrapper::~PHPServiceWrapper()
            {
                LOGENTRY(1,"PHPServiceWrapper::destructor");
                LOGEXIT(1,"PHPServiceWrapper::destructor");
            }
            
            
            // ======================================================================
            // invoke: wrapper call to service with setting the component context
            // ======================================================================
            void PHPServiceWrapper::invoke(Operation& operation)
            {
                LOGENTRY(1,"PHPServiceWrapper::invoke");
    
                SCARuntime* runtime = SCARuntime::getInstance();
                runtime->setCurrentComponent(component);
                
                try
                {
                    LOGINFO_1(4, "PHPServiceWrapper::invoke called with operation name: %s", operation.getName().c_str());

                    // create a temporary script which
                    // - includes the named script
                    // - creates a class instance if required
                    // - calls the named method with the provided arguments
                    
                    // get the component type information
                    PHPImplementation* impl = (PHPImplementation*)component->getType();
                    
                    // first create the temporay script and include the module
                    string script = "include '" + impl->getModule() + ".php';";
                    
                    // if we have a class create an instance 
                    string className = impl->getClass();
                    if( &className != NULL && className.size() > 0)
                    {           
                      script += "$anobject = new " + className + "();";
                    }

                    // construct the call to the function
                    script += "$response = ";
                    
                    if( &className != NULL && className.size() > 0)
                    { 
                      script += " $anobject->";
                    }
                    
                    script += operation.getName().c_str(); 
                    script += "(";
                    
                    char tempString [32];

                    // add the parameters to the call
                    for(unsigned int i = 0; i < operation.getNParms(); i++) 
					{
                        Operation::Parameter& parm = operation.getParameter(i);
    		            switch(parm.getType())
    		            {
    			            case Operation::BOOL: 
    			            {
    				            if( *(bool*)parm.getValue())
                                {
                                    //boolean true
                                    script += "true";
                                }
                                else
                                {
                                    script += "false";
                                }
    				            break;
    			            }
    			            case Operation::SHORT: 
    			            {
                                sprintf ( tempString, "%d", *(short*)parm.getValue() );
                                script += tempString;
    				            break;
    			            }
    			            case Operation::USHORT: 
    			            {
                                sprintf ( tempString, "%d", *(unsigned short*)parm.getValue() );
                                script += tempString;
    				            break;
    			            }
    			            case Operation::LONG: 
    			            {
                                sprintf ( tempString, "%d", *(long*)parm.getValue() );
                                script += tempString;
    				            break;
    			            }
    			            case Operation::ULONG: 
    			            {
                                sprintf ( tempString, "%d", *(unsigned long*)parm.getValue() );
                                script += tempString;
    				            break;
    			            }
    			            case Operation::FLOAT: 
    			            {
                                sprintf ( tempString, "%g", *(float*)parm.getValue() );
                                script += tempString;
    				            break;
    			            }
    			            case Operation::DOUBLE: 
    			            {
                                sprintf ( tempString, "%g", *(double*)parm.getValue() );
                                script += tempString;
                                break;
                            }
    			            case Operation::LONGDOUBLE: 
    			            {
                                sprintf ( tempString, "%g", *(long double*)parm.getValue() );
                                script += tempString;
    				            break;
    			            }
    			            case Operation::CHARS: 
    			            {
                                script += *(char**)parm.getValue();
    				            break;
    			            }
    			            case Operation::STRING: 
    			            {
                                script += (*(string*)parm.getValue()).c_str();
    				            break;
    			            }
                            default: 
                            {
                                throw new ComponentInvocationException("Operation parameter type not supported");
                            }
    		            }
                           
                        if ( ( i + 1 ) < operation.getNParms() )
                        {
                            script += ", ";
                        }
                    }
                    
                    
                    // the closing bracket of the call 
                    script += ");echo $response;return $response;";                  
                 
                    // we now have the temporary script to make the call
                    LOGINFO_1(5, "Executing PHP script \n%s", script.c_str());

                    // load the PHP logging and error callback methods
                    php_embed_module.log_message = php_log_message;
                    php_embed_module.ub_write    = php_ub_write;
                    php_embed_module.flush		 = php_flush;

                    //PHP_EMBED_START_BLOCK(/* argc */ 0, /* argv */ NULL)
                    void ***tsrm_ls; 
                    int status = php_embed_init(0, NULL PTSRMLS_CC);  
                    LOGINFO_1(5, "Engine startup status %d", status);

                    zend_first_try {
                        // set error handler
                        zend_error_cb = php_error_cb;

						// call the dynamically created script
                        //zval retval;
                        zend_eval_string((char *) script.c_str(), 
                                         NULL,//&retval, 
                                         "PHP Component" TSRMLS_CC);
						
                        // get the response 
                        // This doesn't want to work for some reason
                        // so have chaced the last echo that the script 
                        // returns in the global scriptResponse variable
                        // This is a bit of a rubbish way of doing things so 
                        // needs replacing when proper SAPI is used
                        //convert_to_string(&retval);
                        //LOGINFO_1(5, "Script returned %s", Z_STRVAL(retval));
                        //zval_dtor(&retval);

                        //PHP_EMBED_END_BLOCK()
                    } zend_catch {
                        int exit_status = EG(exit_status); 
                        LOGINFO_1(5, "In catch %d", exit_status);
                    } zend_end_try(); 

                    //clean up
                    php_embed_shutdown(TSRMLS_C); 
                    LOGINFO(5, "Engine shutdown");
                   
                    // get the response values
                    LOGINFO_1(5, "Script returned %s", scriptResponse.c_str());

                    switch(operation.getReturnType())
                    {
                        case Operation::BOOL: 
                            {
                                if(scriptResponse == "true")
                                {
                                    *(bool*)operation.getReturnValue() = true;
                                }
                                break;
                            }
                        case Operation::SHORT: 
                            {
                                *(short*)operation.getReturnValue() = (short) strtol(scriptResponse.c_str(), NULL,10);
                                break;
                            }
                        case Operation::LONG: 
                            {
                                *(long*)operation.getReturnValue() =  (long) strtol(scriptResponse.c_str(), NULL,10);
                                break;
                            }
                        case Operation::USHORT: 
                            {
                                *(unsigned short*)operation.getReturnValue() = (unsigned short) strtoul(scriptResponse.c_str(), NULL,10);
                                break;
                            }
                        case Operation::ULONG: 
                            {
                                *(unsigned long*)operation.getReturnValue() = (unsigned long) strtoul(scriptResponse.c_str(), NULL,10);
                                break;
                            }
                        case Operation::FLOAT: 
                            {
                                *(float*)operation.getReturnValue() = (float) strtod(scriptResponse.c_str(), NULL);
                                break;
                            }
                        case Operation::DOUBLE: 
                            {
                                *(double*)operation.getReturnValue() = (double) strtod(scriptResponse.c_str(), NULL);
                                break;
                            }
                        case Operation::LONGDOUBLE: 
                            {
                                *(long double*)operation.getReturnValue() = (long double) strtod(scriptResponse.c_str(), NULL);
                                break;
                            }
                        case Operation::CHARS: 
                            {
                                *(char**)operation.getReturnValue() = (char *)scriptResponse.c_str();
                                break;
                            }
                        case Operation::STRING: 
                            {
                                *(string*)operation.getReturnValue() = scriptResponse;
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
                LOGEXIT(1,"PHPServiceWrapper::invoke");
                
            }
            
            // ======================================================================
            // getServiceWrapper: create a wrapper for the target ComponentService
            // ======================================================================
            PHPServiceWrapper* PHPServiceWrapper::getServiceWrapper(Service* service)
            {            
                LOGENTRY(1,"PHPServiceWrapper::getServiceWrapper");
                PHPServiceWrapper* serviceWrapper = 0;
                
                // ---------------------------------
                // Create an instance of the wrapper
                // ---------------------------------                
                serviceWrapper = new PHPServiceWrapper(service);
                if (!serviceWrapper)
                {
                    string msg = "Could not create new PHPServiceWrapper";
                    LOGERROR(1, msg.c_str());
                    throw ServiceNotFoundException(msg.c_str());
                }                
                
                LOGEXIT(1,"PHPServiceWrapper::getServiceWrapper");
                return serviceWrapper;
            }    

        } // End namespace php        
    } // End namespace sca
} // End namespace tuscany
