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

#ifndef tuscany_sca_extension_implementationextension_h
#define tuscany_sca_extension_implementationextension_h

#include "tuscany/sca/export.h"
#include <string>
using std::string;

#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Service.h"
#include "tuscany/sca/core/ServiceWrapper.h"

#include "commonj/sdo/SDO.h"

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        class SCA_API ImplementationExtension 
        {
        public:
            /**
            * Default constructor
            */
            ImplementationExtension();            
            
            /**
            * Destructor
            */
            virtual ~ImplementationExtension();            
                      
            /**
            * return the name of the extension (e.g. "cpp" "php")
            */
            virtual const string& getExtensionName() = 0;
                      
            /**
            * return the QName of schema elemant for this implementation extension
            * (e.g. "http://www.osoa.org/xmlns/sca/1.0#implementation.cpp")
            */
            virtual const string& getExtensionTypeQName() = 0;

            /**
             * Get an implementation from a DataObject representing
             * an SCDL implementation element
             */ 
            virtual ComponentType* getImplementation(commonj::sdo::DataObjectPtr scdlImplementation) = 0;

         };

        
    } // End namespace sca
} // End namespace tuscany


#endif // tuscany_sca_extension_implementationextension_h

