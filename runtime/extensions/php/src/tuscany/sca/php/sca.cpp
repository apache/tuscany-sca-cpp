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
static zend_object_handlers SCA_Tuscany_object_handlers;

/* {{{ Methods */

/* {{{ proto object getSCATuscanyObject(long object_id)
  return an SCA_Tuscany object based on the object_id
  the object_id 
  */
PHP_METHOD(SCA_Tuscany, getSCATuscanyObject)
{
	long  object_id = 0;

    printf("IN getSCATuscanyObject\n");	
    
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "l", &object_id) == FAILURE) 
	{
		return;
	}
	
    Z_TYPE_P(return_value) = IS_OBJECT;
    
	// poke the object id that has been passed in into the zval
	// to fake the creation of the object
    return_value->value.obj.handle = (zend_object_handle) object_id;
	return_value->value.obj.handlers = &SCA_Tuscany_object_handlers;
	return_value->value.obj.handlers->add_ref(return_value TSRMLS_CC);
}
/* }}} getSCATuscanyObject */

/* {{{ proto void __construct(int operation_handle)
   */
PHP_METHOD(SCA_Tuscany, __construct)
{
	zend_class_entry * _this_ce;
	zval * _this_zval;
	long operation_handle = 0;
	
    printf("IN constructor\n");	

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &operation_handle) == FAILURE) {
		return;
	}

	_this_zval = getThis();
	_this_ce = Z_OBJCE_P(_this_zval);

	PROP_SET_LONG("operation", operation_handle);
}
/* }}} __construct */

/* {{{ proto int invoke(string component_name, string reference_name, string method_name, array arguments)
  Invoke a Tuscany component */
PHP_METHOD(SCA_Tuscany, invoke)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	const char * component_name = NULL;
	int component_name_len = 0;
	const char * reference_name = NULL;
	int reference_name_len = 0;
	const char * method_name = NULL;
	int method_name_len = 0;
	zval * arguments = NULL;
	HashTable * arguments_hash = NULL;

    printf("IN invoke\n");	

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Osssa/", &_this_zval, SCA_Tuscany_ce_ptr, &component_name, &component_name_len, &reference_name, &reference_name_len, &method_name, &method_name_len, &arguments) == FAILURE) {
		return;
	}

	arguments_hash = HASH_OF(arguments);
	
    // find the correct service proxy	
    SCARuntime* runtime = SCARuntime::getCurrentRuntime();
    Component* component = runtime->getCurrentComponent();    
    Reference* ref = component->findReference(reference_name);
    if(!ref)
    {
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
                printf("NULL agument");
            }
            case IS_BOOL: 
            {
                convert_to_boolean(*data);
                bool *newBool = new bool;
                *newBool =  Z_BVAL_PP(data);
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
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, SCA_Tuscany_ce_ptr) == FAILURE) {
		return;
	}

	_this_zval = getThis();
	_this_ce = Z_OBJCE_P(_this_zval);

	array_init(return_value);

    printf("IN getArgArray\n");

    array_init(return_value);
    
    // get the operaton object from the object properties
    
    Operation *operation = (Operation *)(long)PROP_GET_LONG("operation");

    // get the parameters from the operation structure
    for(unsigned int i = 0; i < operation->getNParms(); i++) 
	{
        const Operation::Parameter& parm = operation->getParameter(i);
        printf("Arg %d type %d\n", i, parm.getType());
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
    		    zval * sdo;
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


/* {{{ proto void setResponse(mixed response)
  set the results from the operation */
PHP_METHOD(SCA_Tuscany, setResponse)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	zval * response = NULL;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oz/", &_this_zval, SCA_Tuscany_ce_ptr, &response) == FAILURE) {
		return;
	}

	_this_zval = getThis();
	_this_ce = Z_OBJCE_P(_this_zval);

    printf("IN setResponse\n");	

    // get the operaton object from the object properties
    Operation *operation = (Operation *)(long)PROP_GET_LONG("operation");

    switch(Z_TYPE_P(response))
    {
        case IS_NULL:
        {
            printf("NULL response");
        }
        case IS_BOOL: 
        {
            bool *newBool = new bool;
            *newBool =  Z_BVAL_P(response);
            operation->setReturnValue(newBool);
            break;        
        }
        case IS_LONG: 
        {
            long *newLong = new long;
            *newLong =  Z_LVAL_P(response);
            operation->setReturnValue(newLong);                            
            break;
        }
        case IS_DOUBLE: 
        {
            //double *newDouble = new double;
            float *newDouble = new float;                                
            *newDouble =  (float)Z_DVAL_P(response);
            operation->setReturnValue(newDouble);                            
            break;
        }
        case IS_STRING: 
        {   
            string newString (Z_STRVAL_P(response));
            operation->setReturnValue(&newString);
            break;
        }
        case IS_OBJECT: 
  		{
  		    // convert the PHP SDO into a Tuscany SDO
    		DataObjectPtr sdo = sdo_do_get(response TSRMLS_CC);
    		operation->setReturnValue(&sdo);  		    
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
			          Z_TYPE_P(response)); 
        }                            
  
    }

/*
The following code interprets the return based on what the operation
structure is expecting. In the case of a call from another dynamic script 
component this will be not be set correct so I have changed the code above 
to rely on the type returned from the script instead. Hanging on to the
following switch just in case  
    switch(operation->getReturnType())
    {
        case Operation::BOOL: 
        {
            convert_to_boolean(response);
            *(bool*)operation->getReturnValue() = Z_BVAL_P(response);
            break;
        }
        case Operation::SHORT: 
        {
            convert_to_long(response);
            *(short*)operation->getReturnValue() = (short) Z_LVAL_P(response);
            break;
        }
        case Operation::LONG: 
        {
            convert_to_long(response);
            *(long*)operation->getReturnValue() = (long) Z_LVAL_P(response);                                
            break;
        }
        case Operation::USHORT: 
        {
            convert_to_long(response);
            *(unsigned short*)operation->getReturnValue() = (unsigned short) Z_LVAL_P(response);
            break;
        }
        case Operation::ULONG: 
        {
            convert_to_long(response);
            *(unsigned long*)operation->getReturnValue() = (unsigned long) Z_LVAL_P(response);
            break;
        }
        case Operation::FLOAT: 
        {
            convert_to_double(response);                           
            *(float*)operation->getReturnValue() = (float) Z_DVAL_P(response);
            break;
        }
        case Operation::DOUBLE: 
        {
            convert_to_double(response);
            *(double*)operation->getReturnValue() = (double) Z_DVAL_P(response);
            break;
        }
        case Operation::LONGDOUBLE: 
        {
            convert_to_double(response);
            *(long double*)operation->getReturnValue() = (long double) Z_DVAL_P(response);
            break;
        }
        case Operation::CHARS: 
        {
            convert_to_string(response);
            // need to copy the string out of PHP memory into SCA memory
            char * strCopy = strdup((char *)Z_STRVAL_P(response));
            *(char**)operation->getReturnValue() = strCopy;
            break;
        }
        case Operation::STRING: 
        {
            convert_to_string(response);   
            string reponseString ( Z_STRVAL_P(response) );                        
            *(string*)operation->getReturnValue() = reponseString;
            break;
        }
        case Operation::VOID_TYPE: 
        {
            // do nothing
        }
    	case Operation::DATAOBJECT: 
    	{
    		// need to convert the PHP SDO into a Tuscanu SDO?
    		printf("SDOs are not supported yet");
    		break;
    	}
        default: 
        {
    	    char *class_name;
    	    char *space;
	        class_name = get_active_class_name(&space TSRMLS_CC);
		    php_error(E_ERROR, 
		          "%s%s%s(): Response type %d not supported",
				  class_name, 
				  space, 
				  get_active_function_name(TSRMLS_C), 
				  operation->getReturnType());
        }                            
    }
*/
}
/* }}} setResponse */


static zend_function_entry SCA_Tuscany_methods[] = {
	PHP_ME(SCA_Tuscany, getSCATuscanyObject, SCA_Tuscany_getSCATuscanyObject_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(SCA_Tuscany, __construct, NULL, /**/ZEND_ACC_PUBLIC)
	PHP_ME(SCA_Tuscany, invoke, SCA_Tuscany__invoke_args, /**/ZEND_ACC_PUBLIC)
	PHP_ME(SCA_Tuscany, getArgArray, NULL, /**/ZEND_ACC_PUBLIC)
	PHP_ME(SCA_Tuscany, setResponse, SCA_Tuscany__setResponse_args, /**/ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_SCA_Tuscany(TSRMLS_D)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "SCA_Tuscany", SCA_Tuscany_methods);
	SCA_Tuscany_ce_ptr = zend_register_internal_class(&ce TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(SCA_Tuscany_ce_ptr, 
		"operation", 9, -1, 
		ZEND_ACC_PRIVATE TSRMLS_CC);

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
	PHP_RINIT(sca),     /* Replace with NULL if there is nothing to do at request start */
	PHP_RSHUTDOWN(sca), /* Replace with NULL if there is nothing to do at request end   */
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
	class_init_SCA_Tuscany(TSRMLS_C);

	memcpy(&SCA_Tuscany_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(sca)
{

	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(sca)
{
	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(sca)
{
	/* add your stuff here */

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
  
long createSCATuscanyObject(Operation& operation TSRMLS_DC) 
{
    printf("IN createSCATuscanyObject\n");	
    
    // create an object of type SCA_Tuscany
    zval *_this_zval;
    Z_TYPE_P(_this_zval) = IS_OBJECT;
    if ( object_init_ex(_this_zval, SCA_Tuscany_ce_ptr) == FAILURE) {
    	char *class_name;
    	char *space;
	    class_name = get_active_class_name(&space TSRMLS_CC);
		php_error(E_ERROR, 
		          "%s%s%s(): internal error (%i) - failed to instantiate SCA_Tuscany object",
				  class_name, 
				  space, 
				  get_active_function_name(TSRMLS_C), 
				  __LINE__);
	    return 0;
    }
    
  	zend_class_entry * _this_ce;
    _this_ce = Z_OBJCE_P(_this_zval);
    	
    // Set the address of the C++ operation object as a four byte value
    // in the operation property
   	PROP_SET_LONG("operation", (long) &operation);
   	
   	// get the four byte value of the address of the object
   	// we have just created and return it
    long object_id = (long) Z_OBJ_HANDLE_P(_this_zval);  
    
    printf("IN object_id = %d\n", object_id);
    
    return object_id;
}

#endif /* HAVE_SCA */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
