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

#include <sapi/embed/php_embed.h>


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
#include "php_sdo_int.h"

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
				zval z_func_name, retval, z_component_name, z_class_name, z_operation_name, z_arg_array;
				zval *params[5];

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
						printf("tsrm_ls = %x, embedSAPIInitializeCount = %d\n",
							tsrm_ls, embedSAPIInitializeCount);

						if ( embedSAPIInitializeCount == 0 )
						{
							embedSAPIInitializeCount = 1;

							php_embed_init(0, NULL PTSRMLS_CC); 

							// load up the sca module that provides the interface between
							// C++ and user space PHP
							zend_first_try {
								zend_startup_module(&sca_module_entry);     
							} zend_end_try();
						} else {
							embedSAPIInitializeCount++;
						}

						zend_try {
							// set error handler
							zend_error_cb = php_error_cb;

							/*
							* Open the include files 
						 */
							//						includeFile("SCA/SCA.php" TSRMLS_CC);				
							//						includeFile("SCA/Bindings/tuscany/SCA_TuscanyWrapper.php" TSRMLS_CC);

							/* Instantiate a SCA_Tuscany (mediator) object, and 
							 * set its operation property
							 */
							zval z_tuscany_mediator;
							zval *pz_mediator = &z_tuscany_mediator;
							INIT_ZVAL(z_tuscany_mediator);
							zend_class_entry *ce_mediator = 
								zend_fetch_class("SCA_Tuscany", sizeof("SCA_Tuscany") - 1, 0 TSRMLS_CC);
							object_init_ex(pz_mediator, ce_mediator);
							z_tuscany_mediator.value.obj.handlers->add_ref(pz_mediator TSRMLS_CC);

							zend_update_property_long(ce_mediator, pz_mediator, 
								"operation", sizeof("operation") - 1, (long)&operation TSRMLS_CC);

							/* get the class info loaded up */
							zend_eval_string("include 'SCA/SCA.php';", 
								NULL, "Include SCA" TSRMLS_CC);
							zend_eval_string("include 'SCA/Bindings/tuscany/SCA_TuscanyWrapper.php';", 
								NULL, "Include SCA_TuscanyWrapper" TSRMLS_CC);

							/* Now instantiate an SCA_TuscanyWrapper object and call its constructor */
							zval z_tuscany_wrapper;
							zval *pz_tuscany_wrapper = &z_tuscany_wrapper;
							INIT_ZVAL(z_tuscany_wrapper);
							object_init_ex(pz_tuscany_wrapper, 
								zend_fetch_class("SCA_TuscanyWrapper", sizeof("SCA_TuscanyWrapper") - 1, 0 TSRMLS_CC));  
							z_tuscany_wrapper.value.obj.handlers->add_ref(pz_tuscany_wrapper TSRMLS_CC);

							ZVAL_STRING(&z_func_name, "__construct", 1);
							ZVAL_STRING(&z_component_name, (char *)componentName.c_str(), 1);
							if( &className != NULL && className.size() > 0) {           
								ZVAL_STRING(&z_class_name, (char *)className.c_str(), 1);
							} else {
								ZVAL_NULL(&z_class_name);
							}
							ZVAL_STRING(&z_operation_name, (char *)operationName.c_str(), 1);

							array_init(&z_arg_array);

							// get the parameters from the operation structure
							for(unsigned int i = 0; i < operation.getNParms(); i++) 
							{
								const Operation::Parameter& parm = operation.getParameter(i);
								//printf("Arg %d type %d\n", i, parm.getType());
								switch(parm.getType())
								{
								case Operation::BOOL: 
									{
										add_next_index_bool(&z_arg_array, (int)*(bool*)parm.getValue() );
										break;
									}
								case Operation::SHORT: 
								case Operation::USHORT: 
								case Operation::LONG: 
								case Operation::ULONG: 
									{
										add_next_index_long(&z_arg_array, *(long*)parm.getValue() );
										break;
									}
								case Operation::FLOAT: 
									{
										add_next_index_double(&z_arg_array,( double)*(float*)parm.getValue() );
										break;
									}
								case Operation::DOUBLE: 
								case Operation::LONGDOUBLE: 
									{
										add_next_index_double(&z_arg_array, *(double*)parm.getValue() );
										break;
									}
								case Operation::CHARS: 
									{
										add_next_index_string(&z_arg_array, *(char**)parm.getValue(), 1 );
										break;
									}
								case Operation::STRING: 
									{
										add_next_index_string(&z_arg_array, (char*)(*(string*)parm.getValue()).c_str(), 1 );
										break;
									}    		
								case Operation::DATAOBJECT: 
									{
										// convert the tuscany SDO into a PHP SDO

										// create the object
										zval *sdo;
										ALLOC_INIT_ZVAL(sdo);
										sdo_do_new(sdo, *(DataObjectPtr*)parm.getValue() TSRMLS_CC);

										// add it to the arg array
										add_next_index_zval(&z_arg_array, sdo);
										break;
									}
								default: 
									{
										char *class_name;
										char *space;
										class_name = get_active_class_name(&space TSRMLS_CC);            
										php_error(E_ERROR, 
											"%s%s%s(): Argument type %d not supported",
											class_name, 
											space, 
											get_active_function_name(TSRMLS_C), 
											parm.getType());
									}
								}
							}

							params[0] = &z_tuscany_mediator;
							params[1] = &z_component_name;
							params[2] = &z_class_name;
							params[3] = &z_operation_name;
							params[4] = &z_arg_array;

							call_user_function(EG(function_table), 
								&pz_tuscany_wrapper, 
								&z_func_name,
								&retval,
								5,
								params TSRMLS_CC);

							zval_dtor(&z_func_name);
							zval_dtor(&z_component_name);
							zval_dtor(&z_class_name);
							zval_dtor(&z_operation_name);
							zval_dtor(&z_arg_array);

							string script;
							script = "include '" + compositeDir + "/" + impl->getModule() + ".php';";  
							loginfo("Script: %s", script.c_str());

							// call the dynamically created script
							// I'm not trapping the return value here 
							// as it seems to prevent any kind of output
							// being produced. Needs investigation.
							//zval retval;
							zend_eval_string((char *) script.c_str(), 
								NULL, //&retval, 
								"Include module" TSRMLS_CC);

							/* Now call SCA_TuscanyWrapper->invoke() ... */
							ZVAL_STRING(&z_func_name, "invoke", 1);
							call_user_function(EG(function_table), 
								&pz_tuscany_wrapper, &z_func_name, 
								&retval, 0, NULL TSRMLS_CC);

							zval_dtor(&z_func_name);

							/* ... and copy the return value into the operation */
							switch(Z_TYPE(retval))
							{
							case IS_NULL:
								{
									//printf("NULL response");
									break;
								}
							case IS_BOOL: 
								{
									bool *newBool = new bool;
									*newBool =  ZEND_TRUTH(Z_BVAL(retval));
									operation.setReturnValue(newBool);
									break;        
								}
							case IS_LONG: 
								{
									long *newLong = new long;
									*newLong =  Z_LVAL(retval);
									operation.setReturnValue(newLong);                            
									break;
								}
							case IS_DOUBLE: 
								{
									//double *newDouble = new double;
									float *newDouble = new float;                                
									*newDouble =  (float)Z_DVAL(retval);
									operation.setReturnValue(newDouble);                            
									break;
								}
							case IS_STRING: 
								{   
									string newString (Z_STRVAL(retval));
									operation.setReturnValue(&newString);
									break;
								}
							case IS_OBJECT: 
								{
									// convert the PHP SDO into a Tuscany SDO
									DataObjectPtr sdo = sdo_do_get(&retval TSRMLS_CC);
									operation.setReturnValue(&sdo);  		    
									break;
								}
							default: 
								{
									char *class_name;
									char *space;
									class_name = get_active_class_name(&space TSRMLS_CC);
									php_error(E_ERROR, 
										"%s%s%s(): Input argument type %d not supported on invoke",
										class_name, 
										space, 
										get_active_function_name(TSRMLS_C), 
										Z_TYPE(retval)); 
								}                            
							}
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

			/* Not used at the moment
			 * Can find the include file and execute it,
			 * but doesn't make the contents available
			 */
			void PHPServiceWrapper::includeFile(char *includeFileName TSRMLS_DC) 
			{
                zend_file_handle include_file_handle;

				include_file_handle.type = ZEND_HANDLE_FP;
				include_file_handle.filename = includeFileName;
				include_file_handle.opened_path = NULL;
				include_file_handle.free_filename = 0;
				if (!(include_file_handle.handle.fp = php_fopen_with_path(
					  include_file_handle.filename, "rb", PG(include_path), 
					  &include_file_handle.opened_path TSRMLS_CC))) {
					php_error(E_ERROR,
						"Could not open %s", include_file_handle.filename);
					return;
				}
                php_execute_script(&include_file_handle TSRMLS_CC);
			}

        } // End namespace php        
    } // End namespace sca
} // End namespace tuscany
