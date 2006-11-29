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

#ifndef tuscany_sca_rest_model_restinterface_h
#define tuscany_sca_rest_model_restinterface_h

#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/rest/exportinterface.h"

#include <map>
using std::map;
#include <string>
using std::string;

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace rest
        {
            /**
             * Holds information about a REST interface
             */
            class RESTInterface : public Interface
            {
                
            public:    
                /**
                 * Constuctor.
                 * @param header Name of the header file containing the class that
                 * describes the interface.
                 * @param className Name of the class in the header file that 
                 * describes the interface. 
                 * @param scope The scope of the interface (stateless or composite).
                 * @param remotable True if the interface is remotable.
                 */
                RESTInterface();  

                /**
                 * Destructor.
                 */
                virtual ~RESTInterface();
                
                /**
                * return the QName of the schema type for this interface type
                * (e.g. "http://www.osoa.org/xmlns/sca/1.0#interface.rest")
                */
                virtual const string& getInterfaceTypeQName() { return typeQName; };
    
                /**
                 * The QName of the schema type for this interface type.
                 */
                SCA_REST_INTERFACE_API static const string typeQName;
                
            private:
            
            };
            
        } // End namespace rest
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_rest_model_restinterface_h

