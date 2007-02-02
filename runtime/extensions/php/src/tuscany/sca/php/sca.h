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

#ifndef SCA_H
#define SCA_H

#include "tuscany/sca/core/Operation.h"

#include <php.h>

#ifdef  __cplusplus
extern "C" {
#endif

// creates an instance of SCA_Tuscany and popoulates
// the operation property. It returns the four byte
// representation of the object id from the objects
// zval. 
long createSCATuscanyObject(tuscany::sca::Operation& operation TSRMLS_DC);

extern zend_module_entry sca_module_entry;

#ifdef  __cplusplus
} // extern "C" 
#endif

#endif /* SCA_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
