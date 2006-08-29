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

#ifndef tuscany_sca_extension_referencebindingextension_h
#define tuscany_sca_extension_referencebindingextension_h

#include "tuscany/sca/export.h"

#include <string>
using std::string;

#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Reference.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ReferenceBinding.h"

#include "commonj/sdo/SDO.h"

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        class SCA_API ReferenceBindingExtension 
        {
        public:
            /**
            * Default constructor
            */
            ReferenceBindingExtension();            
            
            /**
            * Destructor
            */
            virtual ~ReferenceBindingExtension();            
                      
            /**
            * return the name of the extension (e.g. "cpp" "php")
            */
            virtual const string& getExtensionName() = 0;
                      
            /**
            * return the QName of schema elemant for this binding extension
            * (e.g. "http://www.osoa.org/xmlns/sca/1.0#binding.ws")
            */
            virtual const string& getExtensionTypeQName() = 0;

            /**
             * Get a reference binding from a DataObject representing
             * an SCDL binding element
             */ 
            virtual ReferenceBinding* getReferenceBinding(Composite* composite, Reference* reference, commonj::sdo::DataObjectPtr scdlBinding) = 0;

         };

        
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_extension_referencebindingextension_h

