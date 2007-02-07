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

#ifdef WIN32

// some strangeness in the build that causes 
// WinSock.h and WinSock2.h to be included leading to redefinitions
#define _WINSOCKAPI_

// sdo.cpp also includes the math libraries and causes redeclarations
// of all the math functions
#define _INC_MATH

#endif //WIN32

#include <iostream>
#include <sstream>

#include <php_embed.h>

#include "tuscany/sca/php/PHPServiceWrapper.h"
#include "tuscany/sca/php/PHPServiceProxy.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/core/Exceptions.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/php/model/PHPImplementation.h"


#include "sca.h"

using namespace std;
using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace php
        {
      
            int embedSAPIInitializeCount = 0;
            
            // Global callbacks used by the PHP engine

            // Callback for SAPI error
            void php_sapi_error(int type, const char *error_msg, ...) 
            {
                logwarning("PHP sapi error: %s", error_msg); 
                //TODO - do something sensible with this output
                zend_bailout(); 
            }

            // Callback for log messages
            void php_log_message(char *message) 
            {
                loginfo("PHP log: %s", message); 
                //TODO - do something sensible with this output                
            }

            // Callback for unbuffered writes (echo, print etc.)
            int php_ub_write(const char *str, unsigned int str_length TSRMLS_DC)
            {
                logentry();
                loginfo("PHP Output: %s", str);
                //TODO - do something sensible with this output
                return str_length;
            }

            // Callback for errors 
            void php_error_cb(int type, 
                              const char *error_filename, 
                              const uint error_lineno,
                              const char *format, va_list args)
            {
                logentry();
                
                char buffer[2048];
                int len;
				
                len = snprintf(buffer, 2048, "Error in file %s on line %d: ", error_filename, error_lineno);
                vsnprintf(buffer + len, (2048 - len), format, args);
                logwarning("PHP error: %s", buffer);
                //TODO - do something sensible with this output                
                zend_bailout(); 
            }


            // ===========
            // Constructor
            // ===========
            PHPServiceWrapper::PHPServiceWrapper(Service* service)
                : ServiceWrapper(service)
            {
                logentry();
    
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
                    throwException(SystemConfigurationException, msg.c_str());
                }

                loginfo("Module: %s", impl->getModule().c_str());
                loginfo("Class: %s", impl->getClass().c_str());
            }
            
            // ==========
            // Destructor
            // ==========
            PHPServiceWrapper::~PHPServiceWrapper()
            {
                logentry();
            }
            
            
            // ======================================================================
            // invoke: wrapper call to service with setting the component context
            // ======================================================================
            void PHPServiceWrapper::invoke(Operation& operation)
            {
                logentry();
string temp;
cout << "Press Key\n";
cin >> temp;
                // set the current component in the SCA runtime
                // so that other things can get at it thorugh this
                // static class
                SCARuntime* runtime = SCARuntime::getCurrentRuntime();
                runtime->setCurrentComponent(component);

                try
                {                
                    // get the component type information
                    PHPImplementation* impl = (PHPImplementation*)component->getType();
                    
                    // get the directory containing the component type
                    const string &compositeDir = impl->getComposite()->getRoot();

                    // get some useful information that will be used later
                    // when we construct the wrapper script 
                    string componentName = component->getName();
                    string className     = impl->getClass();
                    string operationName = operation.getName();
                    loginfo("Component %s class %s operation: %s", 
                            componentName.c_str(),
                            className.c_str(),
                            operationName.c_str());          
                
                    // load the PHP logging and error callback methods
                    php_embed_module.log_message = php_log_message;
                    php_embed_module.ub_write    = php_ub_write;
                    php_embed_module.sapi_error  = php_sapi_error;

// would normally use the following macro before the embeded call
// but we need to take account of multiple nested calls
// and ZTS threading safety is not behaving itself
//                  PHP_EMBED_START_BLOCK(/* argc */ 0, /* argv */ NULL)
// ===================================================================== 
                    {   
#ifdef ZTS
                    static void ***tsrm_ls;
                    loginfo("ZTS enabled");
#endif
                    if ( embedSAPIInitializeCount == 0 )
                    {
                        embedSAPIInitializeCount = 1;
                                           
                        php_embed_init(0, NULL PTSRMLS_CC); 

                        // load up the sca module that provides the interface between
                        // C++ and user space PHP
                        zend_startup_module(&sca_module_entry);                        
                    }
                    else
                    {
                        embedSAPIInitializeCount++;
                    }
// should really be doing a first try but don't want to 
// do it more than once                    
//                    zend_first_try {
                    zend_try {
// =====================================================================
                        // set error handler
                        zend_error_cb = php_error_cb;
                    
                        // create an SCA_Tuscany object and give it the operation
                        // object so that the script in user space PHP can get 
                        // at the arguments and return a response. 
                        long object_id = createSCATuscanyObject(operation TSRMLS_CC);
                        std::stringstream string_stream;
                        string object_id_str;
                        string_stream << object_id;
                        string_stream >> object_id_str;  
                    
                        // create the wrapper script that gets everything up and
                        // going in userspace PHP
                        string script;
                        script += "include 'SCA/SCA.php';";                   
                        script += "include 'SCA/SCA_TuscanyWrapper.php';";
                        script += "$wrapper = new SCA_TuscanyWrapper(";
                        script += object_id_str;
                        script += ", '" + componentName + "', "; 

                        if( &className != NULL && className.size() > 0)
                        {           
                            script += " '" + className + "'";
                        }
                        else
                        {
                            script += "null";
                        }
                  
                        script += ", '" + operationName + "');";
                        script += "include '" + compositeDir + "/" + impl->getModule() + ".php';";                         
                        script += "$response = $wrapper->invoke();";
                        script += "echo $response;"; 
                        loginfo("Script: %s", script.c_str());
                    
						// call the dynamically created script
						// I'm not trapping the return value here 
						// as it seems to prevent any kind of output
						// being produced. Needs investigation.
                        //zval retval;
                        zend_eval_string((char *) script.c_str(), 
                                         NULL,//&retval, 
                                         "PHP Component" TSRMLS_CC);
						
                        // As retval is not behaving post processing is commented out for now 
                        //convert_to_string(&retval);
                        //loginfo("Script returned1: %s", Z_STRVAL(retval));
                        //zval_dtor(&retval);
    
// would normally use the following macro after the embeded call
// but we need to take account of multiple nested calls                    
//                  PHP_EMBED_END_BLOCK()
// =====================================================================
                    } zend_catch { 
                       /* int exit_status = EG(exit_status); */ 
                    } 
                    zend_end_try(); 
                    
                    if ( embedSAPIInitializeCount == 1 )
                    {
                        php_embed_shutdown(TSRMLS_C); 
                        loginfo("Engine shutdown");                        
                    }
                    else
                    {
                        embedSAPIInitializeCount--;
                    }
                    }
// =====================================================================                    
      
                    // The response is set back into 'operation' by the
                    // PECL extension code when the PHP user sapce wrapper
                    // calls setResponse()
                }
                catch (...)
                {
                    runtime->unsetCurrentComponent();
                    throw;
                }
                runtime->unsetCurrentComponent();

            }
            
            // ======================================================================
            // getServiceWrapper: create a wrapper for the target ComponentService
            // ======================================================================
            PHPServiceWrapper* PHPServiceWrapper::getServiceWrapper(Service* service)
            {            
                logentry();
                PHPServiceWrapper* serviceWrapper = 0;
                
                // ---------------------------------
                // Create an instance of the wrapper
                // ---------------------------------                
                serviceWrapper = new PHPServiceWrapper(service);
                if (!serviceWrapper)
                {
                    string msg = "Could not create new PHPServiceWrapper";
                    throwException(SystemConfigurationException, msg.c_str());
                }                
                
                return serviceWrapper;
            }    

        } // End namespace php        
    } // End namespace sca
} // End namespace tuscany
