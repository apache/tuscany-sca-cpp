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

/* $ Id: $ */ 

#ifdef WIN32

// some strangeness in the build that causes 
// WinSock.h and WinSock2.h to be included leading to redefinitions
#define _WINSOCKAPI_

#endif //WIN32

#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/php/PHPServiceProxy.h"
#include "commonj/sdo/RefCountingPointer.h"

using namespace std;
using namespace tuscany::sca;
using namespace tuscany::sca::model;
using namespace tuscany::sca::php;
using namespace commonj::sdo;

#define HAVE_SCA 1


#include "php_sca.h"
#include "sca.h"

#if HAVE_SCA

#include "php_sdo.h"
#include "php_sdo_int.h"
/* {{{ Class definitions */

/* {{{ Class SCA_Tuscany */

static zend_class_entry * SCA_Tuscany_ce_ptr = NULL;

/* {{{ Methods */

/* {{{ proto void __construct(int operation_handle)
   */
PHP_METHOD(SCA_Tuscany, __construct)
{
	zval *_this_zval = getThis();
	zend_class_entry *_this_ce = Z_OBJCE_P(_this_zval);

	long operation_handle = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &operation_handle) == FAILURE) {
		return;
	}

	if (!operation_handle) {
		php_error(E_ERROR, "SCA_Tuscany ctor called with NULL operation");
	}

	PROP_SET_LONG(operation, operation_handle);
}
/* }}} __construct */

/* {{{ proto int invoke(string component_name, string reference_name, string method_name, array arguments)
  Invoke a Tuscany component */
PHP_METHOD(SCA_Tuscany, invoke)
{
	//zval * _this_zval = NULL;
	const char * component_name = NULL;
	int component_name_len = 0;
	const char * reference_name = NULL;
	int reference_name_len = 0;
	const char * method_name = NULL;
	int method_name_len = 0;
	zval * arguments = NULL;
	HashTable * arguments_hash = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sssa/", 
		&component_name, &component_name_len, 
		&reference_name, &reference_name_len,
		&method_name, &method_name_len, 
		&arguments) == FAILURE) {
		return;
	}

	arguments_hash = HASH_OF(arguments);
	
    // find the correct service proxy	
    SCARuntime* runtime = SCARuntime::getCurrentRuntime();
    Component* component = runtime->getCurrentComponent();    
    Reference* ref = component->findReference(reference_name);
    if(!ref) {
    	char *class_name;
    	char *space;
	    class_name = get_active_class_name(&space TSRMLS_CC);
		php_error(E_ERROR, 
		          "%s%s%s(): Can't find reference %s",
				  class_name, 
				  space, 
				  get_active_function_name(TSRMLS_C), 
				  reference_name);        
        return;
    }

    ReferenceBinding* refBinding = ref->getBinding();
    PHPServiceProxy *serviceProxy = (PHPServiceProxy*) refBinding->getServiceProxy();

    // construct an operation structure
    Operation operation(method_name);
    
    // add the parameters to the operation
    for ( zend_hash_internal_pointer_reset(arguments_hash);
          zend_hash_has_more_elements(arguments_hash) == SUCCESS;
          zend_hash_move_forward(arguments_hash) )
    {
        zval **data;
        
        if ( zend_hash_get_current_data(arguments_hash, 
                                        (void**)&data  ) == FAILURE )
        {
            continue;
        }            
        
        char *hashKey;
        uint  hashKeyLength;
        ulong hashIndex;
        int   type;
        
        type = zend_hash_get_current_key_ex(arguments_hash,
                                            &hashKey,
                                            &hashKeyLength,
                                            &hashIndex,
                                            0,
                                            NULL);
                                                                                           
        switch(Z_TYPE_PP(data))
        {
            case IS_NULL:
            {
                //printf("NULL argument");
				break;
            }
            case IS_BOOL: 
            {
                convert_to_boolean(*data);
                bool *newBool = new bool;
                *newBool =  ZEND_TRUTH(Z_BVAL_PP(data));
                operation.addParameter(newBool);
                break;
            }
            case IS_LONG: 
            {
                convert_to_long(*data);
                long *newLong = new long;
                *newLong =  Z_LVAL_PP(data);
                operation.addParameter(newLong);                            
                break;
            }
            case IS_DOUBLE: 
            {
                //convert_to_double(*data);
                //double *newDouble = new double;
                convert_to_double(*data);
                float *newDouble = new float;                                
                *newDouble =  (float)Z_DVAL_PP(data);
                operation.addParameter(newDouble);                            
                break;
            }
            case IS_STRING: 
            {
                convert_to_string(*data);   
                string newString (Z_STRVAL_PP(data));
                operation.addParameter(&newString);
                break;
            }
    		case IS_OBJECT: 
    		{
    		    // convert the PHP SDO into a Tuscany SDO
    		    DataObjectPtr sdo = sdo_do_get(*data TSRMLS_CC);
    		    operation.addParameter(&sdo);
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
				          Z_TYPE_PP(data)); 
            }                            
        }                                        
    }
    
    // call the proxy
    serviceProxy->invokeService(operation);
    
   	switch(operation.getReturnType())
    {
        case Operation::BOOL: 
    	{
    	    ZVAL_BOOL(return_value, *(bool*)operation.getReturnValue());
    		break;
    	}
    	case Operation::SHORT: 
    	case Operation::USHORT: 
    	case Operation::LONG: 
    	case Operation::ULONG: 
    	{
            ZVAL_LONG(return_value, *(long*)operation.getReturnValue());
    	    break;
    	}
    	case Operation::FLOAT: 
    	case Operation::DOUBLE: 
    	case Operation::LONGDOUBLE:     			            
    	{
      	    ZVAL_DOUBLE(return_value, (double)*(float*)operation.getReturnValue());
    	    break;
    	}
    	case Operation::CHARS: 
    	{
            ZVAL_STRING(return_value, (char*)operation.getReturnValue(), 1);
    	    break;
    	}
    	case Operation::STRING: 
    	{
            ZVAL_STRING(return_value, (char*)((string*)operation.getReturnValue())->c_str(), 1);    			            
    		break;
    	}
        case Operation::VOID_TYPE: 
        {
            // do nothing
			break;
        }    	
    	case Operation::DATAOBJECT: 
    	{
    	    // convert the tuscany SDO into a PHP SDO
    	    sdo_do_new(return_value, *(DataObjectPtr*)operation.getReturnValue() TSRMLS_CC);
    		break;
    	}
        default: 
        {
    	    char *class_name;
    	    char *space;
	        class_name = get_active_class_name(&space TSRMLS_CC);
		    php_error(E_ERROR, 
		          "%s%s%s(): Response type %d not supported on invoke",
				  class_name, 
				  space, 
				  get_active_function_name(TSRMLS_C), 
				  operation.getReturnType());
        }    
    }

}
/* }}} invoke */

/* {{{ proto array getArgArray()
  return the arguments for the operation as an array */
PHP_METHOD(SCA_Tuscany, getArgArray)
{
	zval * _this_zval = getThis();
	zend_class_entry *_this_ce = Z_OBJCE_P(_this_zval);

	if (ZEND_NUM_ARGS() > 0) {
		WRONG_PARAM_COUNT;
	}

    array_init(return_value);
    
    // get the operation object from the object properties    
	Operation *operation = (Operation *)PROP_GET_LONG(operation);

    // get the parameters from the operation structure
    for(unsigned int i = 0; i < operation->getNParms(); i++) 
	{
        const Operation::Parameter& parm = operation->getParameter(i);
        //printf("Arg %d type %d\n", i, parm.getType());
        switch(parm.getType())
    	{
    	    case Operation::BOOL: 
    		{
    		    add_next_index_bool(return_value,(int)*(bool*)parm.getValue() );
    			break;
    		}
    		case Operation::SHORT: 
    		case Operation::USHORT: 
    		case Operation::LONG: 
    		case Operation::ULONG: 
    		{
    		    add_next_index_long(return_value,*(long*)parm.getValue() );
    			break;
    		}
    		case Operation::FLOAT: 
    		{
    		    add_next_index_double(return_value,(double)*(float*)parm.getValue() );
    			break;
    		}
    		case Operation::DOUBLE: 
    		case Operation::LONGDOUBLE: 
    		{
    		    add_next_index_double(return_value,*(double*)parm.getValue() );
                break;
            }
    		case Operation::CHARS: 
    		{
    		    add_next_index_string(return_value,*(char**)parm.getValue(), 1 );
    			break;
    		}
    		case Operation::STRING: 
    		{
    		    add_next_index_string(return_value,(char*)(*(string*)parm.getValue()).c_str(), 1 );
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
                add_next_index_zval(return_value, sdo);
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
}
/* }}} getArgArray */

static zend_function_entry SCA_Tuscany_methods[] = {
	PHP_ME(SCA_Tuscany, __construct, SCA_Tuscany____construct_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(SCA_Tuscany, invoke, SCA_Tuscany__invoke_args, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(SCA_Tuscany, getArgArray, SCA_Tuscany__getArgArray_args, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_SCA_Tuscany(TSRMLS_D)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "SCA_Tuscany", SCA_Tuscany_methods);
	SCA_Tuscany_ce_ptr = zend_register_internal_class(&ce TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_null(SCA_Tuscany_ce_ptr, 
		"operation", sizeof("operation") -1,
		ZEND_ACC_PUBLIC TSRMLS_CC);

	/* }}} Property registration */

}

/* }}} Class SCA_Tuscany */

/* }}} Class definitions*/

/* {{{ sca_functions[] */
function_entry sca_functions[] = {
	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ cross-extension dependencies */

#if ZEND_EXTENSION_API_NO >= 220050617
static zend_module_dep sca_deps[] = {
	ZEND_MOD_REQUIRED("sdo")
	{NULL, NULL, NULL, 0}
};
#endif
/* }}} */

/* {{{ sca_module_entry */
zend_module_entry sca_module_entry = {
#if ZEND_EXTENSION_API_NO >= 220050617
		STANDARD_MODULE_HEADER_EX, NULL,
		sca_deps,
#else
		STANDARD_MODULE_HEADER,
#endif

	"sca",
	sca_functions,
	PHP_MINIT(sca),     /* Replace with NULL if there is nothing to do at php startup   */ 
	PHP_MSHUTDOWN(sca), /* Replace with NULL if there is nothing to do at php shutdown  */
	NULL,               /* Replace with NULL if there is nothing to do at request start */
	NULL,               /* Replace with NULL if there is nothing to do at request end   */
	PHP_MINFO(sca),
	"0.0.1", 
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SCA
extern "C" {
ZEND_GET_MODULE(sca)
} // extern "C"
#endif


/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(sca)
{
	/* We use the SDO extension for reference because it is a prereq */
	zend_module_entry *req_module_entry;
	char *req_module_name = "libxml";
	if (zend_hash_find(&module_registry, req_module_name, strlen(req_module_name)+1, (void**)&req_module_entry) == SUCCESS) {
		if (req_module_entry->zend_debug != ZEND_DEBUG ||
		       req_module_entry->zts != USING_ZTS ||
		       req_module_entry->zend_api != ZEND_MODULE_API_NO) {
			php_error(E_ERROR, 
				"Cannot initialize module sca.\nModule sca compiled with module API=%d, debug=%d, thread-safety=%d\nModule %n compiled with module API=%d, debug=%d, thread-safety=%d\nThese options need to match",
				req_module_name,
				ZEND_MODULE_API_NO, ZEND_DEBUG, USING_ZTS,
				req_module_entry->zend_api, req_module_entry->zend_debug, req_module_entry->zts);
			return FAILURE;
		}
	} else {
		/* The dependency checker should already have found this, but to be on the safe side ... */
		php_error (E_ERROR, 
			"Cannot load module sca because required module %n is not loaded", 
			req_module_name);
		return FAILURE;
	}

	class_init_SCA_Tuscany(TSRMLS_C);

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(sca)
{
	/*
	 * There is some corruption going on at shutdown.
	 * The following hack eliminates the symptom, but there's probably still
	 * an underlying problem.
	 */
	free(SCA_Tuscany_ce_ptr->name);
	SCA_Tuscany_ce_ptr->name = NULL;
	destroy_zend_class(&SCA_Tuscany_ce_ptr);
	SCA_Tuscany_ce_ptr = NULL;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(sca)
{
	php_info_print_box_start(0);
	php_printf("<p>SCA Extension</p>\n");
	php_printf("<p>Version 0.0.1alpha (2007-01-15)</p>\n");
	php_printf("<p><b>Authors:</b></p>\n");
	php_printf("<p>Simon Laws &lt;slaws@php.net&gt; (lead)</p>\n");
	php_printf("<p>Caroline Maynard &lt;cem@php.net&gt; (lead)</p>\n");
	php_info_print_box_end();
	php_info_print_table_start();
		php_info_print_table_header(2, "SCA", "enabled");
		php_info_print_table_row(2, "SCA Version", "0.0.1");
		php_info_print_table_end();

}
/* }}} */

/* Other functions not directly related to implementing the 
   SCA_Tuscany extension */
 
#endif /* HAVE_SCA */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
