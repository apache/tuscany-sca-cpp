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

#ifndef tuscany_sca_python_model_pythoninterface_h
#define tuscany_sca_python_model_pythoninterface_h

#include "tuscany/sca/python/export.h"
#include "tuscany/sca/model/Interface.h"

#include <map>
using std::map;
#include <vector>
using std::vector;
#include <string>
using std::string;

using namespace tuscany::sca::model;

namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            /**
             * Holds information about an interface described using a C++
             * header file.
             */
            class PythonInterface : public Interface
            {
                
            public:    
                /**
                 * Constuctor.
                 * @param scope The scope of the interface (stateless or composite).
                 * @param remotable True if the interface is remotable.
                 */
                PythonInterface(
                    bool remotable,
                    bool conversational);  

                /**
                 * Destructor.
                 */
                virtual ~PythonInterface();
                               
                /**
                 * return the QName of the schema type for this interface type
                 * (e.g. "http://www.osoa.org/xmlns/sca/1.0#interface.cpp")
                 */
                const string& getInterfaceTypeQName() { return typeQName; };
    
                /**
                 * The QName of the schema type for this interface type.
                 */               
                SCA_PYTHON_API static const string typeQName;
           
            };
            
        } // End namespace python
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_python_model_pythoninterface_h

