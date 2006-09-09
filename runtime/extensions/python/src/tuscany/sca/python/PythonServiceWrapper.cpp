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

#include "tuscany/sca/python/PythonServiceWrapper.h"

#include "osoa/sca/ServiceRuntimeException.h"
#include "tuscany/sca/util/Logging.h"
#include "tuscany/sca/util/Utils.h"
#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"
#include "tuscany/sca/model/ServiceType.h"
#include "tuscany/sca/model/Interface.h"
#include "tuscany/sca/core/SCARuntime.h"
#include "tuscany/sca/python/model/PythonImplementation.h"

using namespace osoa::sca;

namespace tuscany
{
    namespace sca
    {
        namespace python
        {
             
            /**
            * Prints out PyObject and dir(PyObject)
            * for debugging purposes
            */
            void printPyObject(char * name, PyObject *pObj)
            {
                PyObject* pObjRepr = PyObject_Repr(pObj);    
                LOGINFO_2(5, "PythonServiceWrapper::printPyObject %s: %s", name, PyString_AsString(pObjRepr));
                Py_DECREF(pObjRepr);

                PyObject* pObjDir = PyObject_Dir(pObj);    
                PyObject* pObjDirRepr = PyObject_Repr(pObjDir);
                LOGINFO_2(5, "PythonServiceWrapper::printPyObject dir(%s): %s", name, PyString_AsString(pObjDirRepr));
                Py_DECREF(pObjDirRepr);
                Py_DECREF(pObjDir);
            }

            // ===========
            // Constructor
            // ===========
            PythonServiceWrapper::PythonServiceWrapper(Service* service)
                : ServiceWrapper(service)
            {
                LOGENTRY(1,"PythonServiceWrapper::constructor");
    
                component = service->getComponent();
                interf = service->getType()->getInterface();
                implementation = (PythonImplementation*)component->getType();              

                pythonModule = NULL;
                pythonClassInstance = NULL;
                
                // -----------------------------------------------
                // Get the implementation for the target component
                // -----------------------------------------------
                PythonImplementation* impl = (PythonImplementation*)component->getType();
                if (!impl)
                {
                    string msg = "Component " + component->getName() + " has no implementation defined";
                    throw ServiceNotFoundException(msg.c_str());
                }

                LOGINFO_1(3,"PythonServiceWrapper::getServiceWrapper module %s", impl->getModule().c_str());
                LOGINFO_1(3,"PythonServiceWrapper::getServiceWrapper path %s", impl->getModulePath().c_str());
                LOGINFO_1(3,"PythonServiceWrapper::getServiceWrapper class %s", impl->getClass().c_str());

                // Initialize the Python environment
                Py_Initialize();

                // Add the path to the composite (+ any further path specified) to the Python sys.path
                string path = component->getComposite()->getRoot() + "/" + impl->getModulePath();

                PyObject* pSysName = PyString_FromString("sys");
                PyObject* pSys = PyImport_Import(pSysName);
                Py_DECREF(pSysName);

                if(pSys != NULL)
                {
                    PyObject* pSysPath = PyObject_GetAttrString(pSys, "path");

                    if(pSysPath != NULL && PyList_Check(pSysPath))
                    {
                        PyObject* pPath = PyString_FromString(path.c_str());
                        PyList_Append(pSysPath, pPath);
                        
                        //printPyObject("sys.path", pSysPath);
                        
                        Py_DECREF(pPath);
                        Py_DECREF(pSysPath);
                    }
                    Py_DECREF(pSys);
                }

                if(&(impl->getModule()) != NULL && impl->getModule().size() > 0)
                {
                    // Now import the module
                    PyObject* pModuleName = PyString_FromString(impl->getModule().c_str());

                    pythonModule = PyImport_Import(pModuleName);
                    Py_DECREF(pModuleName);
                }

                if (!pythonModule)
                {
                    if(PyErr_Occurred())
                    {
                        PyErr_Print();
                    }
                    string msg = "Failed to load module named " + impl->getModule();
                    LOGERROR(0, msg.c_str());
                    throw new ComponentInvocationException(msg.c_str());
                }


                LOGEXIT(1,"PythonServiceWrapper::constructor");
                
            }
            
            // ==========
            // Destructor
            // ==========
            PythonServiceWrapper::~PythonServiceWrapper()
            {
                LOGENTRY(1,"PythonServiceWrapper::destructor");
                Py_XDECREF(pythonClassInstance);
                Py_XDECREF(pythonModule);
                Py_Finalize();
                LOGEXIT(1,"PythonServiceWrapper::destructor");
            }
            
            // ======================================================================
            // newInstance: create a new class instance
            // ======================================================================
            PyObject* PythonServiceWrapper::newInstance()
            {
                LOGENTRY(1,"PythonServiceWrapper::newInstance");
                PythonImplementation* impl = (PythonImplementation*)component->getType();
                string className = impl->getClass();

                PyObject* pClassInstance = NULL;
                if (pythonModule != NULL) 
                {
                    if(&className != NULL && className.size() > 0)
                    {
                        // We have a class name, so create an instance and use this to invoke the correct function
                        PyObject* pClass = PyObject_GetAttrString(pythonModule, (char*) className.c_str());

                        if(pClass == NULL)
                        {
                            if(PyErr_Occurred())
                            {
                                PyErr_Print();
                            }
                            string msg = "Cannot find class named " + className + " in Python module";
                            LOGERROR(0, msg.c_str());
                            throw new ComponentInvocationException(msg.c_str());
                        }

                        pClassInstance = PyInstance_New(pClass, NULL, NULL);
                        
                        if(pClassInstance == NULL || !PyInstance_Check(pClassInstance))
                        {
                            if(PyErr_Occurred())
                            {
                                PyErr_Print();
                            }
                            string msg = "Could not create new instance of class named " + className + " in Python module";
                            LOGERROR(0, msg.c_str());
                            throw new ComponentInvocationException(msg.c_str());
                        }
                        Py_XDECREF(pClass);
                    }
                }
                LOGEXIT(1,"PythonServiceWrapper::newInstance");
                return pClassInstance;
            }

            // ======================================================================
            // getInstance: get a class instance for this scope
            // ======================================================================
            PyObject* PythonServiceWrapper::getInstance()
            {
                LOGENTRY(1,"PythonServiceWrapper::getInstance");

                PythonImplementation::Scope scope = implementation->getScope();
                if (scope == PythonImplementation::COMPOSITE)
                {
                    if (!pythonClassInstance)
                    {
                        pythonClassInstance = newInstance();
                    }
                    return pythonClassInstance;
                }
                else // (scope == PythonImplementation::STATELESS)
                {
                    return newInstance();
                }        
                LOGEXIT(1,"PythonServiceWrapper::getInstance");
            }
            
            // ======================================================================
            // releaseImplementation: release the implementation for this scope
            // ======================================================================
            void PythonServiceWrapper::releaseInstance()
            {
                LOGENTRY(1,"PythonServiceWrapper::releaseInstance");
                
                PythonImplementation::Scope scope = implementation->getScope();
                if(scope == PythonImplementation::STATELESS)
                {
                    // Delete the class instance if there is one
                    if(pythonClassInstance != NULL && PyInstance_Check(pythonClassInstance))
                    {
                        Py_DECREF(pythonClassInstance);
                        pythonClassInstance = NULL;
                    }

                    // Need to reload the module
                    PyObject* reloadedPythonModule = PyImport_ReloadModule(pythonModule);

                    if(reloadedPythonModule != NULL)
                    {
                        // Get rid of old pythonModule and replace with the reloaded one
                        Py_DECREF(pythonModule);
                        pythonModule = reloadedPythonModule;
                    }
                }
                LOGEXIT(1,"PythonServiceWrapper::releaseInstance");
            }
            
            // ======================================================================
            // invoke: wrapper call to service with setting the component context
            // ======================================================================
            void PythonServiceWrapper::invoke(Operation& operation)
            {
                LOGENTRY(1,"PythonServiceWrapper::invoke");
    
                SCARuntime* runtime = SCARuntime::getInstance();
                runtime->setCurrentComponent(component);
                
                try
                {
                    LOGINFO_1(4, "PythonServiceWrapper::invoke called with operation name: %s", operation.getName().c_str());

                    PyObject* pFunc = NULL; 
                    pythonClassInstance = getInstance();

                    if(pythonClassInstance != NULL && PyInstance_Check(pythonClassInstance))
                    {
                        // Get the function from the instance
                        pFunc = PyObject_GetAttrString(pythonClassInstance, (char*) operation.getName().c_str());
                    }
                    if(pFunc == NULL && pythonModule != NULL)
                    {
                        // Get the function directly from the module if it could not be got from an instance
                        pFunc = PyObject_GetAttrString(pythonModule, (char*) operation.getName().c_str());
                    }
                    if(pFunc == NULL)
                    {
                        // Can't get the function from the class or module - throw exception
                        string msg = "Python module or class instance has not been created";
                        LOGERROR(0, msg.c_str());
                        throw new ComponentInvocationException(msg.c_str());
                    }

                    if (pFunc && PyCallable_Check(pFunc)) 
                    {
                        PyObject* pArgs = PyTuple_New(operation.getNParms());
                        PyObject* pValue = NULL;

                        for(unsigned int i = 0; i < operation.getNParms(); i++) 
                        {
                            Operation::Parameter& parm = operation.getParameter(i);
    		                switch(parm.getType())
    		                {
    			                case Operation::BOOL: 
    			                {
    				                if( *(bool*)parm.getValue())
                                    {
                                        //boolean true
                                        pValue = Py_True;
                                    }
                                    else
                                    {
                                        pValue = Py_False;
                                    }
    				                break;
    			                }
    			                case Operation::SHORT: 
    			                {
                                    pValue = PyInt_FromLong(*(short*)parm.getValue());
    				                break;
    			                }
    			                case Operation::USHORT: 
    			                {
                                    pValue = PyInt_FromLong(*(unsigned short*)parm.getValue());
    				                break;
    			                }
    			                case Operation::LONG: 
    			                {
                                    pValue = PyLong_FromLong(*(long*)parm.getValue());
    				                break;
    			                }
    			                case Operation::ULONG: 
    			                {
                                    pValue = PyLong_FromUnsignedLong(*(unsigned long*)parm.getValue());
    				                break;
    			                }
    			                case Operation::FLOAT: 
    			                {
                                    pValue = PyFloat_FromDouble(*(float*)parm.getValue());
    				                break;
    			                }
    			                case Operation::DOUBLE: 
    			                {
                                    pValue = PyFloat_FromDouble(*(double*)parm.getValue());
    				                break;
    			                }
    			                case Operation::LONGDOUBLE: 
    			                {
                                    pValue = PyFloat_FromDouble(*(long double*)parm.getValue());
    				                break;
    			                }
    			                case Operation::CHARS: 
    			                {
                                    pValue = PyString_FromString(*(char**)parm.getValue());
    				                break;
    			                }
    			                case Operation::STRING: 
    			                {
                                    pValue = PyString_FromString((*(string*)parm.getValue()).c_str());
    				                break;
    			                }
                                default: throw new ComponentInvocationException("Operation parameter type not supported");
    		                }

                            if (!pValue) 
                            {
                                Py_DECREF(pArgs);

                                if(PyErr_Occurred())
                                {
                                    PyErr_Print();
                                }

                                string msg = "Error converting parameter into Python type";
                                LOGERROR(0, msg.c_str());
                                throw new ComponentInvocationException(msg.c_str());
                                
                            }
                            //printPyObject("Param value", pValue);

                            /* pValue reference stolen here: */
                            PyTuple_SetItem(pArgs, i, pValue);
                        }

                        pValue = PyObject_CallObject(pFunc, pArgs);
                        //printPyObject("Return value", pValue);

                        Py_DECREF(pArgs);
                        if (pValue != NULL) 
                        {
                            switch(operation.getReturnType())
                            {
                            case Operation::BOOL: 
                                {
                                    if(PyBool_Check(pValue))
                                    {
                                        *(bool*)operation.getReturnValue() = (pValue == Py_True);
                                    }
                                    break;
                                }
                            case Operation::SHORT: 
                                {
                                    *(short*)operation.getReturnValue() = (short) PyInt_AsLong(pValue);
                                    break;
                                }
                            case Operation::LONG: 
                                {
                                    *(long*)operation.getReturnValue() =  (long) PyLong_AsLong(pValue);
                                    break;
                                }
                            case Operation::USHORT: 
                                {
                                    *(unsigned short*)operation.getReturnValue() = (unsigned short) PyInt_AsLong(pValue);
                                    break;
                                }
                            case Operation::ULONG: 
                                {
                                    *(unsigned long*)operation.getReturnValue() = (unsigned long) PyLong_AsUnsignedLong(pValue);
                                    break;
                                }
                            case Operation::FLOAT: 
                                {
                                    *(float*)operation.getReturnValue() = (float) PyFloat_AsDouble(pValue);
                                    break;
                                }
                            case Operation::DOUBLE: 
                                {
                                    *(double*)operation.getReturnValue() = (double) PyFloat_AsDouble(pValue);
                                    break;
                                }
                            case Operation::LONGDOUBLE: 
                                {
                                    *(long double*)operation.getReturnValue() = (long double) PyFloat_AsDouble(pValue);
                                    break;
                                }
                            case Operation::CHARS: 
                                {
                                    *(char**)operation.getReturnValue() = strdup(PyString_AsString(pValue));
                                    break;
                                }
                            case Operation::STRING: 
                                {
                                    *(string*)operation.getReturnValue() = PyString_AsString(pValue);
                                    break;
                                }
                            default:;
                            }
                            Py_DECREF(pValue);
                        }
                        else 
                        {
                            Py_DECREF(pFunc);
                            if(PyErr_Occurred())
                            {
                                PyErr_Print();
                            }
                            string msg = "Error whilst calling Python module";
                            LOGERROR(0, msg.c_str());
                            throw new ComponentInvocationException(msg.c_str());
                        }
                    }
                    else 
                    {
                        if (PyErr_Occurred())
                        {
                            PyErr_Print();
                        }
                        string msg = "Cannot find the operation named " + operation.getName() + " in the Python module";
                        LOGERROR(0, msg.c_str());
                        throw new ComponentInvocationException(msg.c_str());
                    }
                    Py_XDECREF(pFunc);
 
                }
                catch (...)
                {
                    releaseInstance();
                    runtime->unsetCurrentComponent();
                    throw;
                }
                releaseInstance();
                runtime->unsetCurrentComponent();
                LOGEXIT(1,"PythonServiceWrapper::invoke");
                
            }
            
            // ======================================================================
            // getServiceWrapper: create a wrapper for the target ComponentService
            // ======================================================================
            PythonServiceWrapper* PythonServiceWrapper::getServiceWrapper(Service* service)
            {            
                LOGENTRY(1,"PythonServiceWrapper::getServiceWrapper");
                PythonServiceWrapper* serviceWrapper = 0;
                
                // ---------------------------------
                // Create an instance of the wrapper
                // ---------------------------------                
                serviceWrapper = new PythonServiceWrapper(service);
                if (!serviceWrapper)
                {
                    string msg = "Could not create new PythonServiceWrapper";
                    LOGERROR(1, msg.c_str());
                    throw ServiceNotFoundException(msg.c_str());
                }                
                
                LOGEXIT(1,"PythonServiceWrapper::getServiceWrapper");
                return serviceWrapper;
            }    

        } // End namespace python        
    } // End namespace sca
} // End namespace tuscany
