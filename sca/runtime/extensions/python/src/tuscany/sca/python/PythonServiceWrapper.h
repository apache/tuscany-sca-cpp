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

#ifndef tuscany_sca_python_pythonservicewrapper_h
#define tuscany_sca_python_pythonservicewrapper_h

#include "tuscany/sca/python/export.h"
#include "tuscany/sca/core/ServiceWrapper.h"
#include "tuscany/sca/core/Operation.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/python/model/PythonImplementation.h"

using namespace tuscany::sca::model;

#include <Python.h>


namespace tuscany
{
    namespace sca
    {
        namespace python
        {
            class PythonInterface;
            
            /**
             * Wraps the service on a component implementation.
             * This abstract class is extended by generated code which provides 
             * the implementation of some of the methods. 
             * An instance of this class wraps the actual component implementation which 
             * has been written by a developer of an SCA application.
             */
            class PythonServiceWrapper : public ServiceWrapper
            {
            public:

                /**
                 * Constructor.
                 * @param target The component service to which this wrapper refers.
                 */
                PythonServiceWrapper(Service* service);
    
                /**
                 * Destructor.
                 */ 
                virtual    ~PythonServiceWrapper();
    
                /**
                 * All business method calls to the target component go through the invoke method.
                 * @param operation The details of the method, paramaters and return value for the
                 * business method to be called on the target component.
                 */
                virtual void invoke(Operation& operation);
    
                
            protected:
                
                /**
                 * Return the current instance of the python class.
                 * @return A pointer to an instance of the python class.
                 */
                virtual PyObject* getInstance();

                /**
                 * Creates a new instance of the python class.
                 * @return A pointer to a new instance of the python class.
                 */
                virtual PyObject* newInstance();
    
                /**
                 * Indicates that the current instance of the python module or class
                 * has been finished with. 
                 */
                virtual void releaseInstance();
    

            private:
                /**
                 * Holds a class instance if a classname is provided. 
                 * Will be constructed each time if scope is set to STATELESS
                 */
                PyObject* pythonClassInstance;

                /**
                 * Holds the module 
                 */
                PyObject* pythonModule;
                                
                /**
                 * Adds references to the provided implementation module or class instance 
                 */
                void addReferences(PyObject* module);

                /**
                 * Adds properties to the provided implementation module or class instance 
                 */
                void addProperties(PyObject* module);

                /**
                 * The component to which this wrapper refers.
                 */
                Component* component;

                /**
                 * The component implementation
                 */
                PythonImplementation* implementation;
                
            };
            
        } // End namespace python
    } // End namespace sca
} // End namespace tuscany

#endif // tuscany_sca_python_pythonservicewrapper_h
