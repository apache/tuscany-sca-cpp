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

#ifndef PHP_SCA_H
#define PHP_SCA_H

#include "tuscany/sca/core/Operation.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <main/php.h>

#ifdef HAVE_SCA

#include <main/php_ini.h>
#include <main/SAPI.h>
#include <ext/standard/info.h>
#include <Zend/zend_extensions.h>
#ifdef  __cplusplus
} // extern "C" 
#endif
#ifdef  __cplusplus
extern "C" {
#endif


//extern zend_module_entry sca_module_entry;
#define phpext_sca_ptr &sca_module_entry

#ifdef PHP_WIN32
#define PHP_SCA_API __declspec(dllexport)
#else
#define PHP_SCA_API
#endif

PHP_MINIT_FUNCTION(sca);
PHP_MSHUTDOWN_FUNCTION(sca);
PHP_MINFO_FUNCTION(sca);

#ifdef ZTS
#include "TSRM/TSRM.h"
#endif

#define FREE_RESOURCE(resource) zend_list_delete(Z_LVAL_P(resource))

#define PROP_GET_LONG(name)    Z_LVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_LONG(name, l) zend_update_property_long(_this_ce, _this_zval, #name, strlen(#name), l TSRMLS_CC)

#define PROP_GET_DOUBLE(name)    Z_DVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_DOUBLE(name, d) zend_update_property_double(_this_ce, _this_zval, #name, strlen(#name), d TSRMLS_CC)

#define PROP_GET_STRING(name)    Z_STRVAL_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_GET_STRLEN(name)    Z_STRLEN_P(zend_read_property(_this_ce, _this_zval, #name, strlen(#name), 1 TSRMLS_CC))
#define PROP_SET_STRING(name, s) zend_update_property_string(_this_ce, _this_zval, #name, strlen(#name), s TSRMLS_CC)
#define PROP_SET_STRINGL(name, s, l) zend_update_property_string(_this_ce, _this_zval, #name, strlen(#name), s, l TSRMLS_CC)


PHP_METHOD(SCA_Tuscany, __construct);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(SCA_Tuscany____construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define SCA_Tuscany____construct_args NULL
#endif

PHP_METHOD(SCA_Tuscany, invoke);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(SCA_Tuscany__invoke_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 4)
  ZEND_ARG_INFO(0, component_name)
  ZEND_ARG_INFO(0, reference_name)
  ZEND_ARG_INFO(0, method_name)
#if (PHP_MINOR_VERSION > 0)
  ZEND_ARG_ARRAY_INFO(0, arguments, 1)
#else
  ZEND_ARG_INFO(0, arguments)
#endif
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define SCA_Tuscany__invoke_args NULL
#endif

PHP_METHOD(SCA_Tuscany, getArgArray);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(SCA_Tuscany__getArgArray_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define SCA_Tuscany__getArgArray_args NULL
#endif

#ifdef  __cplusplus
} // extern "C" 
#endif



#endif /* PHP_HAVE_SCA */

#endif /* PHP_SCA_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
